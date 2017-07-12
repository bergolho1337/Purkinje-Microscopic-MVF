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
    //printGraph(monoMVF->g);

    // Calcular o parametro alfa do sistema linear: alfa = (BETA*Cm*dx*dx) / (SIGMA*dt)
    monoMVF->alfa = (BETA*Cm*monoMVF->dx*monoMVF->dx) / (monoMVF->dt);

    // Alocar memoria
    allocateMonodomain(monoMVF);
    
    // Computar as variaveis ligadas a geometria da celula
    compGeometrics();

    // Atribuir as condicoes iniciais 
    setInitialConditionsModel(monoMVF->vol,monoMVF->g->total_nodes);

    // Configurar as constantes do tempo do modelo celular
    setTimeSettingsModel(monoMVF->vol,monoMVF->g->total_nodes,monoMVF->dt,monoMVF->t_max);
    
    // Configurar as celulas de estimulo
    setStimulusCells(monoMVF->vol,monoMVF->g->total_nodes);

    // Construir a matriz global do sistema linear ligado a solucao da EDP
    assembleMatrix(monoMVF);    

    // Decompor a matriz em LU
    LUDecomposition(monoMVF->K,monoMVF->g->total_nodes);

    return monoMVF;
}

// Alocar memoria de todas as estruturas
void allocateMonodomain (MonodomainMVF *monoMVF)
{
    int np = monoMVF->g->total_nodes;
    monoMVF->vol = (Volume*)malloc(sizeof(Volume)*np);
    for (int i = 0; i < np; i++)
        monoMVF->vol[i].cell = (Cell*)malloc(sizeof(Cell));
    monoMVF->Vstar = (double*)calloc(np,sizeof(double));
    monoMVF->F = (double*)calloc(np,sizeof(double));
}

// Atribuir as condicoes iniciais para todos os pontos da malha
void setInitialConditionsModel (Volume vol[], int np)
{
    // Loop de pontos
    for (int i = 0; i < np; i++)
    {
        setInitialConditions(vol[i].cell);
        vol[i].vOld = vol[i].cell->v; 
    }
          
}

void setTimeSettingsModel (Volume vol[], int np, double dt, double tmax)
{
    setTimeSettings(dt,tmax);
    // Loop de pontos
    for (int i = 0; i < np; i++)
    {
        vol[i].cell->dvdtclock = 1000;
        vol[i].cell->stimtime = 1000;
        vol[i].cell->stimcount 	= -1;
    }
}

void setStimulusCells (Volume vol[], int n)
{
    // Percorrer as celulas de estimulo
    for (int i = 0; i < NSC; i++)
        setStimulus(vol[i].cell,true);
    // Percorrer as demais celulas que nao sao de estimulo
    for (int i = NSC; i < n; i++)
        setStimulus(vol[i].cell,false);
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

// Liberar memoria de todas as estruturas
void freeMonodomain (MonodomainMVF *monoMVF)
{
    printf("[!] Liberando memoria ... ");
    fflush(stdout);
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
    for (int i = 0; i < np; i++) free(vol[i].cell), vol[i].cell = NULL;
    free(vol);
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
        monoMVF->F[u] = monoMVF->vol[u].vOld;
        ptr = ptr->next;
    }
}

// Mover a solucao da EDP(Vstar) para o vetor yStar[]
void moveVstar (Volume vol[], double Vstar[], int np)
{
    for (int i = 0; i < np; i++)
    {
        vol[i].vStar = Vstar[i];
        vol[i].cell->v = Vstar[i];
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
        fprintf(file,"%e\n",vol[ptr->id].vOld);
        ptr = ptr->next;
    }
    fclose(file);
}

// Escrever o estado de todos os volumes de controle em um arquivo
void writeSteadyStateFile (FILE *steadyFile, int nPoints, Volume vol[])
{
    for (int i = 0; i < nPoints; i++)
    {
        fprintf(steadyFile,"%.10lf ",vol[i].cell->v);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->m);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->h);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->j);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->d);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->f);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->f2);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->fca);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->fca2);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->xs1);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->xs2);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->xr);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->a);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->i);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->i2);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->ml);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->ml3);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->hl);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->hl3);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->jl);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->jl3);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->casss);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->cajsr);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->cacsr);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->cansr);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->cassl);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->nai);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->nassl);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->nasss);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->ki);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->cai);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->b);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->g);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->u);
        fprintf(steadyFile,"%.10lf ",vol[i].cell->y);
        fprintf(steadyFile,"%.10lf\n",vol[i].cell->camktrap);
    }
}

// Resolve o sistema de EDO's
void solveEDO (MonodomainMVF *monoMVF, double t)
{
    Node *ptr = monoMVF->g->listNodes;
    Volume *vol = monoMVF->vol;
    int i;
    double dvdt, dt;
    dt = monoMVF->dt;
    
    // Resolver o sistema de EDO para cada ponto (Potencial e as variaveis de estado)
    while (ptr != NULL)
    {
        i = ptr->id;

        // Avancar para o tempo n+1 todas as variaveis
        vol[i].cell->dvdtclock += dt;

        comp_revs(vol[i].cell);
        comp_ina (vol[i].cell);
        comp_inal (vol[i].cell);
        comp_inab (vol[i].cell);
        comp_ical (vol[i].cell);
        comp_icat (vol[i].cell);
        comp_icab (vol[i].cell);
        comp_ito1 (vol[i].cell);
        comp_ikr (vol[i].cell);
        comp_iks (vol[i].cell);
        comp_ik1 (vol[i].cell);
        comp_inaca (vol[i].cell);
        comp_inak (vol[i].cell);
        comp_ipca (vol[i].cell);
        comp_if (vol[i].cell);
        comp_istim (vol[i].cell,t);
        comp_itot (vol[i].cell);

        comp_ip3 (vol[i].cell);
        comp_qrel1 (vol[i].cell);
        comp_qrel2 (vol[i].cell);
        comp_qup1 (vol[i].cell);
        comp_qup2 (vol[i].cell);
        comp_qtr1 (vol[i].cell);
        comp_qtr2 (vol[i].cell);

        comp_conc (vol[i].cell);

        dvdt	           = -vol[i].cell->itot;
        vol[i].cell->v	   += dvdt*dt;
        ptr = ptr->next;
    }
}

void nextTimestep (Volume vol[], int np)
{
    for (int i = 0; i < np; i++)
    {
        vol[i].vNew = vol[i].cell->v;
        vol[i].vOld = vol[i].vNew;
    }
}

/*
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


*/