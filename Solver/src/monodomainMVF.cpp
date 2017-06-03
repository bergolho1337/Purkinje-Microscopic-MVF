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
    monoMVF->dvdt = (Derivative*)malloc(monoMVF->g->total_nodes*sizeof(Derivative));
    monoMVF->retro = (Retropropagation*)malloc(sizeof(Retropropagation));
    monoMVF->vel = (Velocity*)malloc(sizeof(Velocity));
    monoMVF->plot = (Plot*)malloc(sizeof(Plot));
    for (int i = 0; i < monoMVF->g->total_nodes; i++)
      monoMVF->dvdt[i].value = 0;

    // Atribuir as funcoes do modelo celular 
    monoMVF->functions = buildFunctions();

    // Atribuir as condicoes iniciais 
    setInitialConditionsModel_FromFile(monoMVF,argv[4]);

    // Construir a matriz global do sistema linear ligado a solucao da EDP
    assembleMatrix(monoMVF);    

    // Decompor a matriz em LU
    LUDecomposition(monoMVF->K,monoMVF->g->total_nodes);

    // Atribuir pontos em que iremos calcular a velocidade
    //setVelocityPoints(monoMVF->vel,monoMVF->dx,58,258);       // 1cm
    setVelocityPoints(monoMVF->vel,monoMVF->dx,29,129);     // 0.5 cm
    //setVelocityPoints(monoMVF->vel,monoMVF->dx,14,64);      // 0.25 cm

    // Atribuir o ponto de referencia para a retropropagacao
    //setRetropropagation(monoMVF->retro,258);  // 1 cm
    setRetropropagation(monoMVF->retro,129);    // 0.5 cm
    //setRetropropagation(monoMVF->retro,64);   // 0.25 cm

    // Atribuir o ponto de referncia para o plot
    //setPlot(monoMVF->plot,258);                // 1 cm
    setPlot(monoMVF->plot,60);                // 0.5 cm
    //setPlot(monoMVF->plot,64);                 // 0.25 cm

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

        // Escrever a solucao estacionaria
        //if (i == 40000)
        //    writeSteadyStateFile(steadyFile,np,monoMVF->VOld,monoMVF->mOld,monoMVF->hOld,monoMVF->nOld);

        // Resolver a EDP (parte difusiva)
        assembleLoadVector(monoMVF);
        solveLinearSystem_LU(monoMVF->K,monoMVF->F,monoMVF->Vstar,np);

        // Resolver as EDOs (parte reativa)
        solveEDO(monoMVF,t);

        // Calcular o valor da derivada maxima de cada ponto
        calcMaximumDerivative(monoMVF->dvdt,np,t,monoMVF->VOld,monoMVF->VNew);

        // Calcular o valor da derivada espacial minima no ponto de referencia da retropropagacao
        calcMinimumSpacialDerivative(monoMVF->retro,t,monoMVF->VOld[monoMVF->retro->id],monoMVF->VOld[monoMVF->retro->id_prev]);


        #ifdef DEBUG
        printVector("Vstar",monoMVF->Vstar,monoMVF->nPoints*2);
        printVector("VNew",monoMVF->VNew,monoMVF->nPoints*2);
        #endif

        // Passa para a proxima iteracao
        swap(&monoMVF->VOld,&monoMVF->VNew);
        swap(&monoMVF->mOld,&monoMVF->mNew);
        swap(&monoMVF->hOld,&monoMVF->hNew);
        swap(&monoMVF->nOld,&monoMVF->nNew);
        //memcpy(monoMVF->VOld,monoMVF->VNew,sizeof(double)*np);
        //memcpy(monoMVF->mOld,monoMVF->mNew,sizeof(double)*np);
        //memcpy(monoMVF->hOld,monoMVF->hNew,sizeof(double)*np);
        //memcpy(monoMVF->nOld,monoMVF->nNew,sizeof(double)*np);
    }
    printf("ok\n");
    
    // Escrever em arquivo os valores das derivadas maximas de cada ponto
    writeMaximumDerivative(monoMVF->dvdt,np);
    
    // Calcular a velocidade de propagacao nos pontos pre-definidos
    calcVelocity(monoMVF->vel,monoMVF->dvdt);

    // Escrever em arquivo o valor da derivada minima do ponto de referencia
    // Aonde houver mudanca na ordem de grandeza da derivada eh o ponto em que ocorre bloqueio
    writeMinimumSpacialDerivative(monoMVF->retro);
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
void setVelocityPoints (Velocity *v, double dx, int p1, int p2)
{
    v->velocityFile = fopen("velocity.txt","w+");
    v->id_1 = p1;
    v->id_2 = p2;
    v->delta_x = (v->id_2 - v->id_1)*dx;
}

// Inicializar a estrutura Retropropagation
void setRetropropagation (Retropropagation *r, int id)
{
  r->id = id;
  r->id_prev = id-1;
  r->min_deriv = 9999.0;
}

// Inicializar a estrutura Plot
void setPlot (Plot *p, int id)
{
    char filename[50];
    sprintf(filename,"data%d.dat",id);
    p->id = id;
    p->plotFile = fopen(filename,"w+");
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

// Escreve o potencial de um volume em arquivo para cada instante de tempo
void writePlotData(double t, double *v, Plot *plot)
{
    fprintf(plot->plotFile,"%.10lf %.10lf\n",t,v[plot->id]);
}

// Escreve o valor das derivadas maximas de todos os pontos na malha
void writeMaximumDerivative (Derivative *dvdt, int nPoints)
{
    FILE *dvdtFile = fopen("max-dvdt.dat","w+");
    for (int i = 0; i < nPoints; i++)
        fprintf(dvdtFile,"Point %d --> || t = %.10lf || max_dvdt = %.10lf ||\n",i,dvdt[i].t,dvdt[i].value);
    fclose(dvdtFile);
}

void writeMinimumSpacialDerivative (Retropropagation *r)
{
  FILE *retroFile = fopen("block.txt","w+");
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

// Calcula velocidade: v = dx/dt
void calcVelocity (Velocity *v, Derivative *dvdt)
{
    double t = dvdt[v->id_2].t - dvdt[v->id_1].t;
    double velocity = v->delta_x / t;
    fprintf(v->velocityFile,"\n\n[!] Propagation velocity!\n");
    fprintf(v->velocityFile,"t1 = %.10lf\n",dvdt[v->id_1].t);
    fprintf(v->velocityFile,"dvdt[%d] = %.10lf\n\n",v->id_1,dvdt[v->id_1].value);
    fprintf(v->velocityFile,"t2 = %.10lf\n",dvdt[v->id_2].t);
    fprintf(v->velocityFile,"dvdt[%d] = %.10lf\n",v->id_2,dvdt[v->id_2].value);
    fprintf(v->velocityFile,"delta_x = %.10lf\n",v->delta_x);
    fprintf(v->velocityFile,"delta_t = %.10lf\n",t);
    fprintf(v->velocityFile,"\n!!!!!!!! Propagation velocity = %lf cm/s !!!!!!!!!!\n",velocity*1000.0);
    fclose(v->velocityFile); 
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
    fclose(monoMVF->plot->plotFile);
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
