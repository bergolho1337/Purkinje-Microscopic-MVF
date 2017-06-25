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
    // Calcular as distancia do ponto de referencia para todos os outros (delta_x)
    Dijkstra(monoMVF->g,50);

    // Inserir os PMJs nas folhas
    #ifdef PMJ
    insertPMJ(monoMVF->g);
    monoMVF->gamma = (SIGMA) / (monoMVF->dx*monoMVF->dx);
    monoMVF->delta = (BETA*Cm*h2*h2*d2) / monoMVF->dt;
    monoMVF->theta = (1) / (RPMJ*monoMVF->dx*monoMVF->dx*d1);
    monoMVF->eta = (BETA*Cm) / (monoMVF->dt);
    #endif

    // Calcular o parametro alfa do sistema linear: alfa = (BETA*Cm*dx*dx) / (SIGMA*dt)
    monoMVF->alfa = (BETA*Cm*monoMVF->dx*monoMVF->dx) / (monoMVF->dt);

    // Alocar memoria
    allocateMonodomain(monoMVF);

    // Atribuir as funcoes do modelo celular 
    monoMVF->functions = buildFunctions();

    // Atribuir as condicoes iniciais 
    setInitialConditionsModel_FromFile(monoMVF,argv[4]);    

    // Matriz LU foi passada como argumento ? 
    if (argc-1 == 5)
        monoMVF->K = readLUDecompositionFromFile(argv[5]);
    else
    {
        // Construir a matriz global do sistema linear ligado a solucao da EDP
        assembleMatrix(monoMVF);
        // Decompor a matriz em LU
        LUDecomposition(monoMVF->K,monoMVF->g->total_nodes);
        writeLUDecomposition(monoMVF->K,monoMVF->g->total_nodes);
    }

    // Atribuir pontos em que iremos calcular a velocidade
    int ids[6] = {50,100,200,220,250,270};
    setVelocityPoints(monoMVF->vel,5,ids);
    setPlot(monoMVF->plot,ids,5);

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
void setInitialConditionsModel_FromFile (MonodomainMVF *monoMVF, char filename[])
{
    FILE *steadyFile = fopen(filename,"r");
    int np = monoMVF->g->total_nodes;
    double v, m, h, n;
    for (int i = 0; i < np; i++)
    {
        if (!fscanf(steadyFile,"%lf %lf %lf %lf",&v,&m,&h,&n)) printError("Reding SteadyState file.");
        monoMVF->VOld[i] = v;
        monoMVF->mOld[i] = m;
        monoMVF->hOld[i] = h;
        monoMVF->nOld[i] = n;        
    }    
    fclose(steadyFile);  
}

// Montar a matriz do sistema linear, relacionado a solucao da EDP
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
        writePlotData(t,monoMVF->VOld,monoMVF->plot);

        // Escreve no .vtk
        if (i % 10 == 0)
          writeVTKFile(monoMVF->VOld,monoMVF->g,i);

        // Resolver a EDP (parte difusiva)
        assembleLoadVector(monoMVF);
        solveLinearSystem_LU(monoMVF->K,monoMVF->F,monoMVF->Vstar,np);

        // Resolver as EDOs (parte reativa)
        solveEDO(monoMVF,t);

        // Calcular o valor da derivada maxima de cada ponto
        calcMaximumDerivative(monoMVF->dvdt,np,t,monoMVF->VOld,monoMVF->VNew);

        // Calcular o valor da derivada espacial minima no ponto de referencia da retropropagacao
        //calcMinimumSpacialDerivative(monoMVF->retro,t,monoMVF->VOld[monoMVF->retro->id],monoMVF->VOld[monoMVF->retro->id_prev]);

        // Passa para a proxima iteracao
        swap(&monoMVF->VOld,&monoMVF->VNew);
        swap(&monoMVF->mOld,&monoMVF->mNew);
        swap(&monoMVF->hOld,&monoMVF->hNew);
        swap(&monoMVF->nOld,&monoMVF->nNew);
    }
    printf("ok\n");
    
    // Escrever em arquivo os valores das derivadas maximas de cada ponto
    writeMaximumDerivative(monoMVF->dvdt,np);
    
    // Calcular a velocidade de propagacao nos pontos pre-definidos
    calcVelocity(monoMVF->vel,monoMVF->dvdt,monoMVF->g->dist);

    // Escrever em arquivo o valor da derivada minima do ponto de referencia
    // Aonde houver mudanca na ordem de grandeza da derivada eh o ponto em que ocorre bloqueio
    //writeMinimumSpacialDerivative(monoMVF->retro);
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

void calcMaximumDerivative (Derivative *dvdt, int nPoints, double t, double *vold, double *vnew)
{
    for (int i = 0; i < nPoints; i++)
    {
        // Considerar apos o primeiro estimulo
        if (vnew[i] - vold[i] > dvdt[i].value && t > 250.0)
        {
            dvdt[i].value = vnew[i] - vold[i];
            dvdt[i].t = t;
        }
    }
}

// Inicializar a estrutura Velocity
void setVelocityPoints (Velocity *v, int np, int ids[])
{
    v->velocityFile = fopen("Output/velocity.txt","w+");
    v->np = np;
    v->id_source = ids[0];
    v->ids = (int*)malloc(sizeof(int)*np);
    v->t2 = (double*)malloc(sizeof(double)*np);
    for (int i = 0; i < np; i++) 
        v->ids[i] = ids[i+1];
}

// Inicializar a estrutura Retropropagation
void setRetropropagation (Retropropagation *r, int id)
{
  r->id = id;
  r->id_prev = id-1;
  r->min_deriv = 9999.0;
}

// Inicializar a estrutura Plot
void setPlot (Plot *p, int ids[], int np)
{
    char filename[50];
    p->np = np;
    p->ids = (int*)malloc(sizeof(int)*np);
    p->plotFile = (FILE**)malloc(sizeof(FILE*)*np);
    for (int i = 0; i < np; i++)
    {
        p->ids[i] = ids[i+1];
        p->plotFile[i] = (FILE*)malloc(sizeof(FILE));
        sprintf(filename,"Output/data%d.dat",p->ids[i]);
        p->plotFile[i] = fopen(filename,"w+");
    }
}

// Calcular o menor valor assumido pela derivada espacial no ponto de referencia
void calcMinimumSpacialDerivative (Retropropagation *r, double t, double v, double v_prev)
{
  double dif = v_prev - v;
  if (dif < 0 && dif < r->min_deriv)
  {
    r->min_deriv = dif;
    r->t = t;
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

// Escreve o potencial de cada volume em arquivo para cada instante de tempo
void writePlotData(double t, double *v, Plot *plot)
{
    for (int i = 0; i < plot->np; i++)
        fprintf(plot->plotFile[i],"%.10lf %.10lf\n",t,v[plot->ids[i]]);
}

// Escreve o valor das derivadas maximas de todos os pontos na malha
void writeMaximumDerivative (Derivative *dvdt, int nPoints)
{
    FILE *dvdtFile = fopen("Output/max-dvdt.dat","w+");
    for (int i = 0; i < nPoints; i++)
        fprintf(dvdtFile,"Point %d --> || t = %.10lf || max_dvdt = %.10lf ||\n",i,dvdt[i].t,dvdt[i].value);
    fclose(dvdtFile);
}

void writeMinimumSpacialDerivative (Retropropagation *r)
{
  FILE *retroFile = fopen("Output/block.txt","w+");
  fprintf(retroFile,"The minimum spacial derivative on Node %d happen at t = %.5lf and value is %.10lf\n", \
  r->id, r->t,r->min_deriv);
  fclose(retroFile);
}

// Escrever o estado de todos os volumes de controle em um arquivo
void writeSteadyStateFile (FILE *steadyFile, int nPoints, double vm[], double m[], double h[], double n[])
{
    for (int i = 0; i < nPoints; i++)
        fprintf(steadyFile,"%.10lf %.10lf %.10lf %.10lf\n",vm[i],m[i],h[i],n[i]);
}

// Calcula velocidade de cada volume controle: v = dx/dt
void calcVelocity (Velocity *v, Derivative *dvdt, double dist[])
{
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
    }
    fclose(v->velocityFile); 
}

// Alocar memoria para as estruturas
void allocateMonodomain (MonodomainMVF *monoMVF)
{
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
    monoMVF->dvdt = (Derivative*)malloc(monoMVF->g->total_nodes*sizeof(Derivative));
    monoMVF->retro = (Retropropagation*)malloc(sizeof(Retropropagation));
    monoMVF->vel = (Velocity*)malloc(sizeof(Velocity));
    monoMVF->plot = (Plot*)malloc(sizeof(Plot));
    for (int i = 0; i < monoMVF->g->total_nodes; i++)
      monoMVF->dvdt[i].value = 0;
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
    //fclose(monoMVF->plot->plotFile);
    free(monoMVF->plot);
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

void swap (double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}
