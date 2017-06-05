#include "../include/monodomainMVF.h"

// Construtor da estrutura MonodomainMVF
MonodomainMVF* newMonodomainMVF (int argc, char *argv[])
{
    MonodomainMVF *monoMVF = (MonodomainMVF*)malloc(sizeof(MonodomainMVF));
    // Receber a entrada da linha de comando
    monoMVF->dt = atof(argv[1]);
    monoMVF->t_max = atof(argv[2]);
    monoMVF->M = nearbyint(monoMVF->t_max / monoMVF->dt);
    
    // Ler arquivo da malha e montar o grafo
    monoMVF->g = readPurkinjeNetworkFromFile(argv[3],monoMVF->dx);

    // Calcular o parametro alfa do sistema linear: alfa = (BETA*Cm*dx*dx) / (SIGMA*dt)
    monoMVF->alfa = (BETA*Cm*monoMVF->dx*monoMVF->dx) / (monoMVF->dt);

    // Montar o nome do arquivo do steady-state
    monoMVF->id = atoi(argv[4]);
    monoMVF->nVolFiber = monoMVF->g->total_edges / (monoMVF->id + 1);
    sprintf(monoMVF->filename,"steadystate%d.dat",monoMVF->id);

    // Alocar memoria
    monoMVF->VOld = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->Vstar = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->VNew = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->mOld = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->mNew = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->hOld = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->hNew = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->nOld = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->nNew = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));
    monoMVF->F = (double*)calloc(monoMVF->g->total_nodes,sizeof(double));

    // Atribuir as funcoes do modelo celular 
    monoMVF->functions = buildFunctions();

    // Atribuir as condicoes iniciais 
    setInitialConditionsModel(monoMVF,argc,argv);

    // Construir a matriz global do sistema linear ligado a solucao da EDP
    assembleMatrix(monoMVF);    

    // Decompor a matriz em LU
    LUDecomposition(monoMVF->K,monoMVF->g->total_nodes);

    //printInfoModel(monoMVF);

    return monoMVF;
}

// Constroi o vetor de funcoes que serao utilizados no programa
// 0 = Funcao do potencial transmembranico (Vm)
// 1 = Funcao da variavel de estado (m)
// 2 = Funcao da variavel de estado (h)
// 3 = Funcao da variavel de estado (n)
Func* buildFunctions ()
{
  Func *func = (Func*)malloc(sizeof(Func)*num_eq);
  func[0] = dvdt__Nob;
  func[1] = dmdt__Nob;
  func[2] = dhdt__Nob;
  func[3] = dndt__Nob;
  return func;
}

// Atribuir as condicoes iniciais para todos os pontos da malha
void setInitialConditionsModel (MonodomainMVF *monoMVF, int argc, char *argv[])
{
    int np = monoMVF->g->total_nodes;
    // Se o usuario nao passar o 6 argumento na linha de comando, inicializar com os valores padroes do modelo celular
    if (argc-1 == 4)
    {
        int i;
        for (i = 0; i < np; i++)
        {
            monoMVF->VOld[i] = v0__Nob;
            monoMVF->mOld[i] = m0__Nob;
            monoMVF->hOld[i] = h0__Nob;
            monoMVF->nOld[i] = n0__Nob;
        }
    }
    // Senao atribuir as condicoes iniciais a partir do arquivo passado
    else
    {
        int i, j;
        double v, m, h, n;
        FILE *steadyFile = fopen(argv[5],"r");
        i = 0;
        while (fscanf(steadyFile,"%lf %lf %lf %lf",&v,&m,&h,&n) != EOF)
        {
            monoMVF->VOld[i] = v;
            monoMVF->mOld[i] = m;
            monoMVF->hOld[i] = h;
            monoMVF->nOld[i] = n;
            i++;
        }
        for (j = i - monoMVF->nVolFiber; i < np; j++, i++)
        {
            monoMVF->VOld[i] = monoMVF->VOld[j];
            monoMVF->mOld[i] = monoMVF->mOld[j];
            monoMVF->hOld[i] = monoMVF->hOld[j];
            monoMVF->nOld[i] = monoMVF->nOld[j];
        }
        fclose(steadyFile);
    }  
}

// Montar a matriz do sistema linear, relacionado a solucao da EDP
void assembleMatrix (MonodomainMVF *monoMVF)
{
    printf("[!] Construindo matriz ...\n");
    fflush(stdout);

    Node *ptr;
    Edge *ptrl;
    int np, u, v;
    np = monoMVF->g->total_nodes;
    monoMVF->K = (double*)calloc(np*np,sizeof(double));
    
    // Percorrer o grafo e usar os indices dos vertices para preencher a matriz
    ptr = monoMVF->g->listNodes;
    while (ptr != NULL)
    {
        u = ptr->id;
        ptrl = ptr->edges;
        while (ptrl != NULL)
        {
            v = ptrl->dest->id;
            monoMVF->K[u*np+v] = -SIGMA;
            ptrl = ptrl->next;
        }
        monoMVF->K[u*np+u] = ptr->num_edges*SIGMA + monoMVF->alfa;
        ptr = ptr->next;
    }
}

// Resolver a equacao do problema
void solveMonodomain (MonodomainMVF *monoMVF)
{
    FILE *steadyFile = fopen(monoMVF->filename,"w+");
    double t;
    // A matriz global do problema jah se encontra como LU
    printf("[!] Resolvendo o problema transiente ... \n");
    printf("[!] Progress\n");
    fflush(stdout);
    int i, np, M;
    M = monoMVF->M;
    np = monoMVF->g->total_nodes;
    // Iterar o metodo a cada passo de tempo
    for (i = 0; i < M; i++)
    {
        t = i*monoMVF->dt;

        // Imprime o progresso da solucao
        printProgress2(i,M);

        // Escreve no .vtk
        if (i % 10 == 0)
          writeVTKFile(monoMVF->VOld,monoMVF->g,i);

        // Escrever a solucao estacionaria
        if (i == 40000)
            writeSteadyStateFile(steadyFile,np,monoMVF->VOld,monoMVF->mOld,monoMVF->hOld,monoMVF->nOld);

        // Resolver a EDP (parte difusiva)
        assembleLoadVector(monoMVF);
        solveLinearSystem_LU(monoMVF->K,monoMVF->F,monoMVF->Vstar,np);

        // Resolver as EDOs (parte reativa)
        solveEDO(monoMVF,t);

        //calcPropagationVelocity(monoMVF->VNew,t);

        #ifdef DEBUG
        printVector("Vstar",monoMVF->Vstar,monoMVF->nPoints*2);
        printVector("VNew",monoMVF->VNew,monoMVF->nPoints*2);
        #endif

        // Passa para a proxima iteracao
        memcpy(monoMVF->VOld,monoMVF->VNew,sizeof(double)*np);
        memcpy(monoMVF->mOld,monoMVF->mNew,sizeof(double)*np);
        memcpy(monoMVF->hOld,monoMVF->hNew,sizeof(double)*np);
        memcpy(monoMVF->nOld,monoMVF->nNew,sizeof(double)*np);
    }
    fclose(steadyFile); 
    printf("ok\n");

}

// Constroi o vetor de termos independentes do sistema linear
void assembleLoadVector (MonodomainMVF *monoMVF)
{
    int np;
    double alfa;
    np = monoMVF->g->total_nodes;
    alfa = monoMVF->alfa;
    for (int i = 0; i < np; i++)
        monoMVF->F[i] = alfa*monoMVF->VOld[i];
}

// Resolve o sistema de EDO's
void solveEDO (MonodomainMVF *monoMVF, double t)
{
    int np;
    int point;
    double f, dt;
    np = monoMVF->g->total_nodes;
    dt = monoMVF->dt;
    // Resolver o sistema de EDO para cada ponto (Potencial e as variaveis de estado)
    for (point = 0; point < np; point++)
    {
        // V^{n+1} = V^{*} + f*dt
        f = monoMVF->functions[0](point,t,monoMVF->Vstar[point],monoMVF->mOld[point],monoMVF->hOld[point],monoMVF->nOld[point]);
        monoMVF->VNew[point] = monoMVF->Vstar[point] + f*dt;
        // m^{n+1} = m^{n} + f*dt
        f = monoMVF->functions[1](point,t,monoMVF->VOld[point],monoMVF->mOld[point],monoMVF->hOld[point],monoMVF->nOld[point]);
        monoMVF->mNew[point] = monoMVF->mOld[point] + f*dt;   
        // h^{n+1} = h^{n} + f*dt
        f = monoMVF->functions[2](point,t,monoMVF->VOld[point],monoMVF->mOld[point],monoMVF->hOld[point],monoMVF->nOld[point]);
        monoMVF->hNew[point] = monoMVF->hOld[point] + f*dt;
        // n^{n+1} = n^{n} + f*dt
        f = monoMVF->functions[3](point,t,monoMVF->VOld[point],monoMVF->mOld[point],monoMVF->hOld[point],monoMVF->nOld[point]);
        monoMVF->nNew[point] = monoMVF->nOld[point] + f*dt;
    }
}

// Escrever o arquivo .vtk com a solucao
void writeVTKFile (double *Vm, Graph *g, int k)
{
    FILE *file;
    int i, np, ne;
    char filename[50];
    Node *ptr = g->listNodes;
    np = g->total_nodes;
    ne = g->total_edges;
    // Escrever o potencial transmembranico
    sprintf(filename,"VTK/solucao%d.vtk",k);
    file = fopen(filename,"w+");
    fprintf(file,"# vtk DataFile Version 3.0\n");
    fprintf(file,"Monodomain MVF\n");
    fprintf(file,"ASCII\n");
    fprintf(file,"DATASET POLYDATA\n");
    fprintf(file,"POINTS %d float\n",np);
    while (ptr != NULL)
    {
        fprintf(file,"%e %e %e\n",ptr->x,ptr->y,ptr->z);
        ptr = ptr->next;
    }
    fprintf(file,"LINES %d %d\n",ne,ne*3);
    ptr = g->listNodes;
    while (ptr != NULL)
    {
        Edge *ptrl = ptr->edges;
        while (ptrl != NULL)
        {
            fprintf(file,"2 %d %d\n",ptr->id,ptrl->dest->id);
            ptrl = ptrl->next;
        }
        ptr = ptr->next;
    }

    fprintf(file,"POINT_DATA %d\n",np);
    fprintf(file,"SCALARS vm float 1\n");
    fprintf(file,"LOOKUP_TABLE default\n");
    ptr = g->listNodes;
    while (ptr != NULL)
    {
        fprintf(file,"%e\n",Vm[ptr->id]);
        ptr = ptr->next;
    }
    fclose(file);
}


// Escrever o estado de todos os volumes de controle em um arquivo
void writeSteadyStateFile (FILE *steadyFile, int nPoints, double vm[], double m[], double h[], double n[])
{
    for (int i = 0; i < nPoints; i++)
        fprintf(steadyFile,"%.10lf %.10lf %.10lf %.10lf\n",vm[i],m[i],h[i],n[i]);
}

// Liberar memoria de todas as estruturas
void freeMonodomain (MonodomainMVF *monoMVF)
{
    printf("[!] Liberando memoria ... ");
    fflush(stdout);
    free(monoMVF->functions);
    free(monoMVF->K);
    free(monoMVF->F);
    free(monoMVF->VNew);
    free(monoMVF->VOld);
    free(monoMVF->Vstar);
    free(monoMVF->mOld);
    free(monoMVF->mNew);
    free(monoMVF->hOld);
    free(monoMVF->hNew);
    free(monoMVF->nOld);
    free(monoMVF->nNew);
    // freeGraph(monoMVF->g);
    free(monoMVF);
    printf("ok\n");
}

void printInfoModel (MonodomainMVF *monoMVF)
{
    printf("======================== INFO MODEL ============================\n");
    printf("Number of volumes = %d\n",monoMVF->g->total_nodes);
    printf("Number of fibers = %d\n",monoMVF->id+1);
    printf("Number of volumes per fiber = %d\n",monoMVF->nVolFiber);
    printf("dt = %.4lf\n",monoMVF->dt);
    printf("t_max = %.4lf\n",monoMVF->t_max);
    printf("dx = %.4lf\n",monoMVF->dx);
    printf("M = %d\n",monoMVF->M);
    printGraph(monoMVF->g);
    printf("================================================================\n");
}