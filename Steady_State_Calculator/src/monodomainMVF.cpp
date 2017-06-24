#include "../include/monodomainMVF.h"

// Construtor da estrutura MonodomainMVF
MonodomainMVF* newMonodomainMVF (int argc, char *argv[])
{
    MonodomainMVF *monoMVF = (MonodomainMVF*)malloc(sizeof(MonodomainMVF));
    // Receber a entrada da linha de comando
    monoMVF->dt = atof(argv[1]);
    monoMVF->t_max = atof(argv[2]);
    monoMVF->M = nearbyint(monoMVF->t_max / monoMVF->dt);
    
    // Ler arquivo da malha, adicionar os PMJ e montar o grafo
    monoMVF->g = readPurkinjeNetworkFromFile(argv[3],monoMVF->dx);
    #ifdef PMJ
    insertPMJ(monoMVF->g);
    monoMVF->gamma = (SIGMA) / (monoMVF->dx*monoMVF->dx);
    monoMVF->delta = (BETA*Cm*h2*h2*d2) / monoMVF->dt;
    monoMVF->theta = (1) / (RPMJ*monoMVF->dx*monoMVF->dx*d1);
    monoMVF->eta = (BETA*Cm) / (monoMVF->dt);
    #endif
    //printGraph(monoMVF->g);

    // Calcular o parametro alfa do sistema linear: alfa = (BETA*Cm*dx*dx) / (SIGMA*dt)
    monoMVF->alfa = (BETA*Cm*monoMVF->dx*monoMVF->dx) / (monoMVF->dt);
    
    // Montar o nome do arquivo do steady-state
    sprintf(monoMVF->filename,"%s",argv[4]);

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
// As divisoes nos indices da matriz sao para simplificar depois o vetor do lado direito
void assembleMatrix (MonodomainMVF *monoMVF)
{
    printf("[!] Construindo matriz ...\n");
    fflush(stdout);

    Node *ptr;
    Edge *ptrl;
    int np, u, v, type;
    np = monoMVF->g->total_nodes;
    monoMVF->K = (double*)calloc(np*np,sizeof(double));
    
    // Percorrer o grafo e usar os indices dos vertices para preencher a matriz
    ptr = monoMVF->g->listNodes;
    while (ptr != NULL)
    {
        u = ptr->id;
        type = ptr->type;
        ptrl = ptr->edges;

        // PMJ
        if (type == 1)
        {
            while (ptrl != NULL)
            {
                v = ptrl->dest->id;
                monoMVF->K[u*np+v] = -1.0 / monoMVF->delta;
                ptrl = ptrl->next;
            }
            monoMVF->K[u*np+u] = (1.0 + monoMVF->delta) / monoMVF->delta;
        }
        // Purkinje cell
        else
        {
            // Esta ligada a algum PMJ ?
            bool isPMJ = isConnectToPMJ(ptr->edges);
            ptrl = ptr->edges;
            // Se nao tiver PMJ por perto eh ligacao normal com Purkinje cells
            if (isPMJ == false)
            {
                while (ptrl != NULL)
                {
                    v = ptrl->dest->id;
                    monoMVF->K[u*np+v] = -SIGMA / monoMVF->alfa;
                    ptrl = ptrl->next;
                }
                monoMVF->K[u*np+u] = (ptr->num_edges*SIGMA + monoMVF->alfa) / monoMVF->alfa;
            }
            // Senao eh ligacao especial Purkinje cell - PMJ
            else
            {
                monoMVF->K[u*np+u] = monoMVF->eta;
                while (ptrl != NULL)
                {
                    v = ptrl->dest->id;
                    if (ptrl->dest->type == 0)
                    {
                        monoMVF->K[u*np+v] = -monoMVF->gamma / monoMVF->eta;
                        monoMVF->K[u*np+u] += monoMVF->gamma;
                    }
                    else
                    {
                        monoMVF->K[u*np+v] = -monoMVF->theta / monoMVF->eta;
                        monoMVF->K[u*np+u] += monoMVF->theta;
                    }
                    ptrl = ptrl->next;
                }
                monoMVF->K[u*np+u] /= monoMVF->eta;
            }    
        }
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

        // Passa para a proxima iteracao
        swap(&monoMVF->VOld,&monoMVF->VNew);
        swap(&monoMVF->mOld,&monoMVF->mNew);
        swap(&monoMVF->hOld,&monoMVF->hNew);
        swap(&monoMVF->nOld,&monoMVF->nNew);
    }
    fclose(steadyFile); 
    printf("ok\n");

}

// Constroi o vetor de termos independentes do sistema linear
void assembleLoadVector (MonodomainMVF *monoMVF)
{
    Node *ptr = monoMVF->g->listNodes;
    while (ptr != NULL)
    {
        int u = ptr->id;
        // Purkinje cell
        if (ptr->type == 0)
        {
            bool isPMJ = isConnectToPMJ(ptr->edges);
            // Purkinje cell - Purkinje cell
            if (isPMJ == false)
                monoMVF->F[u] = monoMVF->VOld[u];
            // Purkinje cell - PMJ
            else
                monoMVF->F[u] = monoMVF->VOld[u];
        }
        // PMJ
        else
        {
            // Purkinje cell - PMJ
            monoMVF->F[u] = monoMVF->VOld[u];
        }
        ptr = ptr->next;
    }
}

// Resolve o sistema de EDO's
void solveEDO (MonodomainMVF *monoMVF, double t)
{
    Node *ptr;
    int id, type;
    double f, dt;
    dt = monoMVF->dt;
    ptr = monoMVF->g->listNodes;
    // Resolver o sistema de EDO para cada ponto (Potencial e as variaveis de estado)
    while (ptr != NULL)
    {
        id = ptr->id;
        type = ptr->type;

        // V^{n+1} = V^{*} + f*dt
        f = monoMVF->functions[0](type,id,t,monoMVF->Vstar[id],monoMVF->mOld[id],monoMVF->hOld[id],monoMVF->nOld[id]);
        monoMVF->VNew[id] = monoMVF->Vstar[id] + f*dt;
        // m^{n+1} = m^{n} + f*dt
        f = monoMVF->functions[1](type,id,t,monoMVF->VOld[id],monoMVF->mOld[id],monoMVF->hOld[id],monoMVF->nOld[id]);
        monoMVF->mNew[id] = monoMVF->mOld[id] + f*dt;   
        // h^{n+1} = h^{n} + f*dt
        f = monoMVF->functions[2](type,id,t,monoMVF->VOld[id],monoMVF->mOld[id],monoMVF->hOld[id],monoMVF->nOld[id]);
        monoMVF->hNew[id] = monoMVF->hOld[id] + f*dt;
        // n^{n+1} = n^{n} + f*dt
        f = monoMVF->functions[3](type,id,t,monoMVF->VOld[id],monoMVF->mOld[id],monoMVF->hOld[id],monoMVF->nOld[id]);
        monoMVF->nNew[id] = monoMVF->nOld[id] + f*dt;

        ptr = ptr->next;
    }
}

// Escrever o arquivo .vtk com a solucao
void writeVTKFile (double *Vm, Graph *g, int k)
{
    FILE *file;
    int np, ne;
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
    printf("Number of conections = %d\n",monoMVF->g->total_edges);
    printf("dt = %.4lf\n",monoMVF->dt);
    printf("t_max = %.4lf\n",monoMVF->t_max);
    printf("dx = %.4lf\n",monoMVF->dx);
    printf("M = %d\n",monoMVF->M);
    printGraph(monoMVF->g);
    printf("================================================================\n");
}

void swap (double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}