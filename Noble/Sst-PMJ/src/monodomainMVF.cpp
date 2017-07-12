#include "../include/monodomainMVF.h"

// Construtor da estrutura MonodomainMVF
MonodomainMVF* newMonodomainMVF (int argc, char *argv[])
{
    MonodomainMVF *monoMVF = (MonodomainMVF*)malloc(sizeof(MonodomainMVF));
    // Receber a entrada da linha de comando
    monoMVF->dt = atof(argv[1]);
    monoMVF->t_max = atof(argv[2]);
    monoMVF->M = nearbyint(monoMVF->t_max / monoMVF->dt);
    sprintf(monoMVF->filename,"%s",argv[4]);
    
    // Ler arquivo da malha, adicionar os PMJ e montar o grafo
    monoMVF->g = readPurkinjeNetworkFromFile(argv[3],monoMVF->dx);
    insertPMJ(monoMVF->g);
    //printGraph(monoMVF->g);
    monoMVF->delta = (BETA*Cm*ALFA) / monoMVF->dt;
    monoMVF->gamma = RPMJ*SIGMA*d1;
    monoMVF->eta = (BETA*Cm*monoMVF->dx*monoMVF->dx*d1*RPMJ) / (monoMVF->dt);

    // Calcular o parametro alfa do sistema linear: alfa = (BETA*Cm*dx*dx) / (SIGMA*dt)
    monoMVF->alfa = (BETA*Cm*monoMVF->dx*monoMVF->dx) / (monoMVF->dt);

    // Alocar memoria
    allocateMonodomain(monoMVF);

    // Atribuir as funcoes do modelo celular 
    monoMVF->functions = buildFunctions();

    // Atribuir as condicoes iniciais 
    setInitialConditionsModel(monoMVF->vol,monoMVF->g->total_nodes,argc,argv);

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
void setInitialConditionsModel (Volume vol[], int np, int argc, char *argv[])
{
    int neq = num_eq;
    // Loop de pontos
    for (int i = 0; i < np; i++)
    {
        // Loop das equacoes do sistema de EDO's
        for (int j = 0; j < neq; j++)
            vol[i].yOld[j] = y0__Nob[j];
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
                        monoMVF->K[u*np+v] = -1.0 / monoMVF->eta;
                        monoMVF->K[u*np+u] += 1.0;
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
          writeVTKFile(monoMVF->vol,monoMVF->g,i);

        // Escrever a solucao estacionaria
        if (i == 40000)
            writeSteadyStateFile(steadyFile,np,monoMVF->vol);

        // Resolver a EDP (parte difusiva)
        assembleLoadVector(monoMVF);
        solveLinearSystem_LU(monoMVF->K,monoMVF->F,monoMVF->Vstar,np);
        moveVstar(monoMVF->vol,monoMVF->Vstar,np);

        // Resolver as EDOs (parte reativa)
        solveEDO(monoMVF,t);

        // Passa para a proxima iteracao
        nextTimestep(monoMVF->vol,np);
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
        monoMVF->F[u] = monoMVF->vol[u].yOld[0];
        ptr = ptr->next;
    }
}

// Resolve o sistema de EDO's
void solveEDO (MonodomainMVF *monoMVF, double t)
{
    Node *ptr;
    int i, type, neq;
    double f, dt;
    ptr = monoMVF->g->listNodes;
    dt = monoMVF->dt;
    neq = num_eq;
    // Resolver o sistema de EDO para cada ponto (Potencial e as variaveis de estado)
    while (ptr != NULL)
    {
        i = ptr->id;
        type = ptr->type;

        // V^n+1 = V^n+1/2 + dt*f
        f = monoMVF->functions[0](type,i,t,monoMVF->vol[i].yStar);
        monoMVF->vol[i].yNew[0] = monoMVF->vol[i].yStar[0] + f*dt;
        // gate^n+1 = gate^n + dt*f
        for (int j = 1; j < neq; j++)
        {
            f = monoMVF->functions[j](type,i,t,monoMVF->vol[i].yOld);
            monoMVF->vol[i].yNew[j] = monoMVF->vol[i].yOld[j] + f*dt;    
        }
        ptr = ptr->next;
    }
}

// Escrever o arquivo .vtk com a solucao
void writeVTKFile (Volume *vol, Graph *g, int k)
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
        fprintf(file,"%e\n",vol[ptr->id].yOld[0]);
        ptr = ptr->next;
    }
    fclose(file);
}


// Escrever o estado de todos os volumes de controle em um arquivo
void writeSteadyStateFile (FILE *steadyFile, int nPoints, Volume vol[])
{
    int neq = num_eq;
    for (int i = 0; i < nPoints; i++)
    {
        for (int j = 0; j < neq; j++)
            fprintf(steadyFile,"%.10lf ",vol[i].yOld[j]);
        fprintf(steadyFile,"\n");
    }
}

// Alocar memoria de todas as estruturas
void allocateMonodomain (MonodomainMVF *monoMVF)
{
    // Capturar o numero de equacoes do modelo celular e numero de total de volumes
    int neq = num_eq;
    int np = monoMVF->g->total_nodes;
    monoMVF->vol = (Volume*)malloc(sizeof(Volume)*np);
    for (int i = 0; i < np; i++)
    {
        monoMVF->vol[i].yOld = (double*)calloc(neq,sizeof(double));
        monoMVF->vol[i].yNew = (double*)calloc(neq,sizeof(double));
        monoMVF->vol[i].yStar = (double*)calloc(neq,sizeof(double));
    }
    monoMVF->Vstar = (double*)calloc(np,sizeof(double));
    monoMVF->F = (double*)calloc(np,sizeof(double));
}

// Mover a solucao da EDP(Vstar) para o vetor yStar[]
void moveVstar (Volume vol[], double Vstar[], int np)
{
    int neq = num_eq;
    for (int i = 0; i < np; i++)
    {
        vol[i].yStar[0] = Vstar[i];
        for (int j = 1; j < neq; j++)
            vol[i].yStar[j] = vol[i].yOld[j];
    }
}

// Liberar memoria de todas as estruturas
void freeMonodomain (MonodomainMVF *monoMVF)
{
    printf("[!] Liberando memoria ... ");
    fflush(stdout);
    free(monoMVF->functions);
    free(monoMVF->K);
    free(monoMVF->F);
    free(monoMVF->Vstar);
    freeVolume(monoMVF->vol,monoMVF->g->total_nodes);
    freeGraph(monoMVF->g);
    free(monoMVF);
    printf("ok\n");
}

void freeVolume (Volume vol[], int np)
{
    for (int i = 0; i < np; i++)
    {
        free(vol[i].yNew); vol[i].yNew = NULL;
        free(vol[i].yOld); vol[i].yOld = NULL;
        free(vol[i].yStar); vol[i].yStar = NULL;
    }
    free(vol);
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

void nextTimestep (Volume vol[], int np)
{
    for (int i = 0; i < np; i++)
        swap(&vol[i].yOld,&vol[i].yNew);
}

void swap (double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}


