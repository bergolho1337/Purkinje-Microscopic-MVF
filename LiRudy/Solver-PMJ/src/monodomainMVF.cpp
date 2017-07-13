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
    setTypeCell(monoMVF,argv[3]);
    
    // Ler arquivo da malha, adicionar os PMJ e montar o grafo
    monoMVF->g = readPurkinjeNetworkFromFile(argv[3],monoMVF->dx);
    // Calcular as distancia do ponto de referencia para todos os outros (delta_x)
    Dijkstra(monoMVF->g,monoMVF->ids[0]);

    // Adicionar os PMJ e montar o grafo
    insertPMJ(monoMVF->g);
    monoMVF->delta = (BETA*Cm*ALFA) / monoMVF->dt;
    monoMVF->gamma = RPMJ*SIGMA*d1;
    monoMVF->eta = (BETA*Cm*monoMVF->dx*monoMVF->dx*d1*RPMJ) / (monoMVF->dt);

    // Calcular o parametro alfa do sistema linear: alfa = (BETA*Cm*dx*dx) / (SIGMA*dt)
    monoMVF->alfa = (BETA*Cm*monoMVF->dx*monoMVF->dx) / (monoMVF->dt);

    // Alocar memoria
    allocateMonodomain(monoMVF);

    // Computar as variaveis ligadas a geometria da celula
    compGeometrics();

    // Atribuir as condicoes iniciais 
    setInitialConditionsModel_FromFile(monoMVF->vol,monoMVF->g->total_nodes,argv[4]);

    // Configurar as constantes do tempo do modelo celular
    setTimeSettingsModel(monoMVF->vol,monoMVF->g->total_nodes,monoMVF->dt,monoMVF->t_max);
    
    // Configurar as celulas de estimulo
    setStimulusCells(monoMVF->vol,monoMVF->g->total_nodes);

    // Atribuir pontos em que iremos calcular a velocidade
    setVelocityPoints(monoMVF->vel,monoMVF->ids);
    setPlot(monoMVF->plot,monoMVF->ids);

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
    for (int i = 0; i < np; i++) monoMVF->vol[i].cell = (Cell*)malloc(sizeof(Cell));
    monoMVF->Vstar = (double*)calloc(np,sizeof(double));
    monoMVF->F = (double*)calloc(np,sizeof(double));
    monoMVF->vel = (Velocity*)malloc(sizeof(Velocity));
    monoMVF->plot = (Plot*)malloc(sizeof(Plot));
    monoMVF->dvdt = (Derivative*)malloc(np*sizeof(Derivative));
    for (int i = 0; i < monoMVF->g->total_nodes; i++) monoMVF->dvdt[i].value = 0;
}

// Atribuir as condicoes iniciais para todos os pontos da malha a partir do arquivo de entrada
void setInitialConditionsModel_FromFile (Volume vol[], int np, const char filename[])
{
    FILE *steadyFile = fopen(filename,"r");
    // Loop de pontos
    for (int i = 0; i < np; i++)
    {
        if (!fscanf(steadyFile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",\
         &vol[i].cell->v,&vol[i].cell->m,&vol[i].cell->h,&vol[i].cell->j,&vol[i].cell->d,\
         &vol[i].cell->f,&vol[i].cell->f2,&vol[i].cell->fca,&vol[i].cell->fca2,&vol[i].cell->xs1,\
         &vol[i].cell->xs2,&vol[i].cell->xr,&vol[i].cell->a,&vol[i].cell->i,&vol[i].cell->i2,\
         &vol[i].cell->ml,&vol[i].cell->ml3,&vol[i].cell->hl,&vol[i].cell->hl3,&vol[i].cell->jl,\
         &vol[i].cell->jl3,&vol[i].cell->casss,&vol[i].cell->cajsr,&vol[i].cell->cacsr,&vol[i].cell->cansr,\
         &vol[i].cell->cassl,&vol[i].cell->nai,&vol[i].cell->nassl,&vol[i].cell->nasss,&vol[i].cell->ki,\
         &vol[i].cell->cai,&vol[i].cell->b,&vol[i].cell->g,&vol[i].cell->u,&vol[i].cell->y,&vol[i].cell->camkactive))
            printError("Reding SteadyState file.");
        vol[i].vOld = vol[i].cell->v;
    }
    fclose(steadyFile);  
}

/* Iniciliazar as variaveis ligadas ao tempo de cada celula */
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

/* Definir as celulas de estimulo */
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

// Resolver a equacao do problema
void solveMonodomain (MonodomainMVF *monoMVF)
{
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

        // Escrever o arquivo de plot
        writePlotData(t,monoMVF->vol,monoMVF->plot);

        // Escreve no .vtk
        if (i % 10 == 0) writeVTKFile(monoMVF->vol,monoMVF->g,i);

        // Resolver a EDP (parte difusiva)
        assembleLoadVector(monoMVF);
        solveLinearSystem_LU(monoMVF->K,monoMVF->F,monoMVF->Vstar,np);
        moveVstar(monoMVF->vol,monoMVF->Vstar,np);

        // Resolver as EDOs (parte reativa)
        solveEDO(monoMVF,t);

        // Calcular o valor da derivada maxima de cada ponto
        calcMaximumDerivative(monoMVF->dvdt,np,t,monoMVF->vol);

        // Passa para a proxima iteracao
        nextTimestep(monoMVF->vol,np);
    }
    printf("ok\n");

    // Calcular a velocidade de propagacao nos pontos pre-definidos
    calcVelocity(monoMVF->vel,monoMVF->dvdt,monoMVF->g->dist);
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

// Calcular a maior derivada no tempo de cada volume 
void calcMaximumDerivative (Derivative dvdt[], int np, double t, Volume vol[])
{
    double diff;
    for (int i = 0; i < np; i++)
    {
        diff = vol[i].cell->v - vol[i].vOld;
        // Considerar apos o primeiro estimulo
        if (diff > dvdt[i].value && t > 250.0)
        {
            dvdt[i].value = diff;
            dvdt[i].t = t;
        }
    }
}

// Calcula velocidade de cada volume controle: v = dx/dt
void calcVelocity (Velocity *v, Derivative dvdt[], double dist[])
{
    FILE *vFile = fopen("Output/v.txt","w+");
    double t, velocity;
    for (int i = 0; i < v->np; i++)
    {
        t = dvdt[v->ids[i]].t - dvdt[v->id_source].t;
        velocity = dist[v->ids[i]] / t;
        fprintf(v->velocityFile,"\n\n[!] Propagation velocity! Id = %d\n",v->ids[i]);
        fprintf(v->velocityFile,"t1 = %.10lf\n",dvdt[v->id_source].t);
        fprintf(v->velocityFile,"dvdt[%d] = %.10lf\n\n",v->id_source,dvdt[v->id_source].value);
        fprintf(v->velocityFile,"t2 = %.10lf\n",dvdt[v->ids[i]].t);
        fprintf(v->velocityFile,"dvdt[%d] = %.10lf\n",v->ids[i],dvdt[v->ids[i]].value);
        fprintf(v->velocityFile,"delta_x = %.10lf\n",dist[v->ids[i]]);
        fprintf(v->velocityFile,"delta_t = %.10lf\n",t);
        fprintf(v->velocityFile,"\n!!!!!!!! Propagation velocity = %lf cm/s !!!!!!!!!!\n",velocity*1000.0);
        fprintf(v->velocityFile,"\n=============================================================\n\n");

        fprintf(vFile,"%d %lf\n",v->ids[i],velocity*1000.0);
    } 
    fprintf(v->velocityFile,"Delay = %.10lf\n",dvdt[v->ids[v->np-1]].t-dvdt[v->ids[v->np-2]].t);
    fprintf(v->velocityFile,"\n=============================================================\n\n");
    fclose(v->velocityFile);
    fclose(vFile);
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

// Escreve o potencial transmembranico de cada volume em arquivo para cada instante de tempo
void writePlotData(double t, Volume vol[], Plot *plot)
{
    for (int i = 0; i < plot->np; i++)
        fprintf(plot->plotFile[i],"%.10lf %.10lf\n",t,vol[plot->ids[i]].vOld);
}

// Mover a solucao da EDP(Vstar) para a variavel vStar
void moveVstar (Volume vol[], double Vstar[], int np)
{
    for (int i = 0; i < np; i++)
    {
        vol[i].vStar = Vstar[i];
        vol[i].cell->v = Vstar[i];
    }
}

// Identificar a partir do nome do arquivo qual o tipo de celula utilizado
void setTypeCell (MonodomainMVF *monoMVF, const char filename[])
{
    // Alocar e inicializar o vetor 'ids' com os volumes a serem plotados
    string str(filename);
    monoMVF->ids = (int*)malloc(sizeof(int)*NPLOT);
    if (str.find("alien") != string::npos)
        memcpy(monoMVF->ids,ids_alien,sizeof(int)*NPLOT);
    else if (str.find("dog") != string::npos)
        memcpy(monoMVF->ids,ids_dog,sizeof(int)*NPLOT);
    else if (str.find("pig") != string::npos)
        memcpy(monoMVF->ids,ids_pig,sizeof(int)*NPLOT);
    else if (str.find("orc") != string::npos)
        memcpy(monoMVF->ids,ids_orc,sizeof(int)*NPLOT);
    else
        printError("Invalid type of cell");
}

// Inicializar a estrutura Velocity a partir do array global 'ids'
void setVelocityPoints (Velocity *v, int ids[])
{
    v->velocityFile = fopen("Output/velocity.txt","w+");
    // Primeiro ponto eh sempre o source
    v->np = NPLOT-1;
    v->id_source = ids[0];
    v->ids = (int*)malloc(sizeof(int)*v->np);
    v->t2 = (double*)malloc(sizeof(double)*v->np);
    for (int i = 0; i < v->np; i++) 
        v->ids[i] = ids[i+1];
}

// Inicializar a estrutura a partir do array global 'ids'
void setPlot (Plot *p, int ids[])
{
    char filename[200];
    // Primeiro ponto eh sempre o source
    p->np = NPLOT-1;
    p->ids = (int*)malloc(sizeof(int)*p->np);
    p->plotFile = (FILE**)malloc(sizeof(FILE*)*p->np);
    for (int i = 0; i < p->np; i++)
    {
        p->ids[i] = ids[i+1];
        p->plotFile[i] = (FILE*)malloc(sizeof(FILE));
        sprintf(filename,"Output/data%d.dat",p->ids[i]);
        p->plotFile[i] = fopen(filename,"w+");
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
    free(monoMVF->dvdt);
    free(monoMVF->ids);
    freeVolume(monoMVF->vol,monoMVF->g->total_nodes);
    freeVelocity(monoMVF->vel);
    freePlot(monoMVF->plot);
    freeGraph(monoMVF->g);
    free(monoMVF);
    printf("ok\n");
}

void freePlot (Plot *plot)
{
    free(plot->ids); plot->ids = NULL;
    //free(p->plotFile);
    //free(p);    
}

void freeVelocity (Velocity *v)
{
    free(v->ids); v->ids = NULL;
    free(v->t2); v->t2 = NULL;
    free(v);
}

void freeVolume (Volume vol[], int np)
{
    for (int i = 0; i < np; i++)
        free(vol[i].cell), vol[i].cell = NULL;
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

/* Levar as variaveis do potencial para a proxima iteracao no tempo */
void nextTimestep (Volume vol[], int np)
{
    for (int i = 0; i < np; i++)
    {
        vol[i].vNew = vol[i].cell->v;
        vol[i].vOld = vol[i].vNew;
    }
}

/* Funcao de troca de ponteiros */
void swap (double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}

