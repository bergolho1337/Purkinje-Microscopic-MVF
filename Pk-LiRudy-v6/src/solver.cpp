#include "../include/solver.h"

// Number of threads to solve the system of ODEs
static constexpr int nthreads = 2;

Solver::Solver (int argc, char *argv[])
{
    dt = atof(argv[2]);
    tmax = atof(argv[3]);
    mesh_filename = argv[4];
    steady_filename = argv[5];
    plot_filename = argv[6];
    M = nearbyint(tmax/dt);
    setTypeCell();
    g = new Graph(mesh_filename,dx);
    //g->dijkstra(plot->ids[0]);

    setControlVolumes();
    setInitCondFromFile();
    setModelParameters();

    setDerivative();
    setVelocityPoints();
    setPlot(); 
    setSensibilityParam(argc,argv);

    //g->print();   
}

void Solver::setModelParameters ()
{
    // Calculate the global variables of the Li-Rudy model
    compGeometrics();
    setTimeSettings(dt,tmax);
    // Set stimulus variables for each cell
    setStimSettings();
    setStimCells();
}

void Solver::setStimCells ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < NSC; i++)
        vol[i].cell->setStimulus(true);
    for (int i = NSC; i < np; i++)
        vol[i].cell->setStimulus(false);
}

void Solver::setStimSettings ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        vol[i].cell->dvdtclock = 500;
        vol[i].cell->stimtime = 500;
        vol[i].cell->stimcount 	= -1;
    }
}

void Solver::solve ()
{
    #ifdef OUTPUT
    printf("[!] Solving transient problem ... \n");
    printf("[!] Progress\n");
    fflush(stdout);
    #endif

    int np = g->getTotalNodes();
    // Build the matrix
    SpMat A(np,np);
    setMatrix(A);
    SparseLU<SpMat> sparseSolver(A);
    
    // Declare RHS and the solution vector
    VectorXd b(np);
    VectorXd x(np);
    
    // Iterar o metodo a cada passo de tempo
    for (int i = 0; i < M; i++)
    {
        double t = i*dt;

        // Imprime o progresso da solucao
        #ifdef OUTPUT
        printProgress(i,M);
        #endif

        // Escrever o arquivo de plot
        writePlotData(t);

        // Escreve no .vtk
        #ifdef VTK
        if (i % 10 == 0) writeVTKFile(i);
        #endif

        // Resolver a EDP (parte difusiva)
        assembleLoadVector(b);
        x = sparseSolver.solve(b);
        moveVstar(x);

        // Resolver as EDOs (parte reativa)
        solveODE(t);

        // Calcular o valor da derivada maxima de cada ponto
        calcMaxDerivative(t);

        // Passa para a proxima iteracao
        nextTimestep();
    }
    #ifdef OUTPUT
    printf("ok\n");
    #endif

    // Calcular a velocidade de propagacao nos pontos pre-definidos
    calcVelocity();
}

void Solver::setSensibilityParam (int argc, char *argv[])
{
    // Default values
    if (argc-1 == 6)
    {
        alfa = 1.375;
        d1 = 0.002;
    }
    // User-defined
    else
    {
        alfa = atof(argv[7]);
        d1 = atof(argv[8]);
    }
    BETA = 4.0 / d1 * 1.0e-04;
}

void Solver::setControlVolumes ()
{
    int np = g->getTotalNodes();
    vol = (CVolume*)malloc(sizeof(CVolume)*np);
    for (int i = 0; i < np; i++)
        vol[i].cell = new Cell();
}

void Solver::setTypeCell ()
{
    // Alocar e inicializar o vetor 'ids' com os volumes a serem plotados a partir do arquivo de entrada .plt
    plot = (Plot*)malloc(sizeof(Plot));
    FILE *pltFile = fopen(plot_filename.c_str(),"r");

    if (!fscanf(pltFile,"%d",&plot->np)) error("Reading PLT file");
    plot->ids = (int*)malloc(sizeof(int)*plot->np);

    for (int i = 0; i < plot->np; i++)
        if (!fscanf(pltFile,"%d",&plot->ids[i])) error("Reading PLT file");

    fclose(pltFile);
}

void Solver::setInitCondFromFile ()
{
    FILE *sstFile = fopen(steady_filename.c_str(),"r");
    if (!sstFile) error("Could not open SST file");
    int np = g->getTotalNodes();
    
    // Loop de pontos
    for (int i = 0; i < np; i++)
    {
        if (!fscanf(sstFile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",\
         &vol[i].cell->v,&vol[i].cell->m,&vol[i].cell->h,&vol[i].cell->j,&vol[i].cell->d,\
         &vol[i].cell->f,&vol[i].cell->f2,&vol[i].cell->fca,&vol[i].cell->fca2,&vol[i].cell->xs1,\
         &vol[i].cell->xs2,&vol[i].cell->xr,&vol[i].cell->a,&vol[i].cell->i,&vol[i].cell->i2,\
         &vol[i].cell->ml,&vol[i].cell->ml3,&vol[i].cell->hl,&vol[i].cell->hl3,&vol[i].cell->jl,\
         &vol[i].cell->jl3,&vol[i].cell->casss,&vol[i].cell->cajsr,&vol[i].cell->cacsr,&vol[i].cell->cansr,\
         &vol[i].cell->cassl,&vol[i].cell->nai,&vol[i].cell->nassl,&vol[i].cell->nasss,&vol[i].cell->ki,\
         &vol[i].cell->cai,&vol[i].cell->b,&vol[i].cell->g,&vol[i].cell->u,&vol[i].cell->y,&vol[i].cell->camkactive))
            error("Reding SteadyState file.");
        vol[i].vOld = vol[i].cell->v;
    }
    fclose(sstFile);
}

void Solver::setVelocityPoints ()
{
    vel = (Velocity*)malloc(sizeof(Velocity));
    vel->velocityFile = fopen("Output/velocity.txt","w+");
    // First point is always the source
    vel->np = plot->np-1;
    vel->id_source = plot->ids[0];
    vel->ids = (int*)malloc(sizeof(int)*vel->np);
    vel->t2 = (double*)malloc(sizeof(double)*vel->np);
    for (int i = 0; i < vel->np; i++) 
        vel->ids[i] = plot->ids[i+1];
}

void Solver::setPlot ()
{
    char filename[200];
    plot->plotFile = (FILE**)malloc(sizeof(FILE*)*(plot->np-1));
    for (int i = 1; i < plot->np; i++)
    {
        plot->plotFile[i-1] = (FILE*)malloc(sizeof(FILE));
        sprintf(filename,"Output/data%d.dat",plot->ids[i]);
        plot->plotFile[i-1] = fopen(filename,"w+");
    }
}

void Solver::setDerivative ()
{
    int np = g->getTotalNodes();
    dvdt = (Derivative*)malloc(np*sizeof(Derivative));
    for (int i = 0; i < np; i++) dvdt[i].value = 0;
}

void Solver::setMatrix (SpMat &a)
{
    // Compute the coefficients values
    double A = 4.0 / (RPMJ*M_PI*d1*d1*dx);
    double B = (SIGMA) / (dx*dx);
    double C = (BETA*Cm) / (dt);
    double D = (BETA*Cm*alfa) / (dt);
    double E = (BETA*Cm*dx*dx) / (dt);

    // Compute the coefficients values
    //double delta = (BETA*Cm*alfa) / dt;
    //double gamma = RPMJ*SIGMA*d1;
    //double eta = (BETA*Cm*dx*dx*d1*RPMJ) / (dt);
    //double phi = (BETA*Cm*dx*dx) / (dt);

    // Non-zero coefficients
    vector<T> coeff;

    Node *ptr = g->getListNodes();
    while (ptr != NULL)
    {
        int u = ptr->id;
        int type = ptr->type;
        Edge *ptrl = ptr->edges;
        
        // PMJ
        if (type == 1)
        {
            double value = -1.0 / D;
            while (ptrl != NULL)
            {
                int v = ptrl->dest->id;
                coeff.push_back(T(u,v,value));
                ptrl = ptrl->next;
            }
            value = (1.0 + D) / D;
            coeff.push_back(T(u,u,value)); 
        }
        // Purkinje cell
        else
        {
            bool isPMJ = isConnToPMJ(ptr->edges);
            //Not link to a PMJ, so normal edge with a Purkinje cell
            if (isPMJ == false)
            {
                double value = -SIGMA / E;
                while (ptrl != NULL)
                {
                    int v = ptrl->dest->id;
                    coeff.push_back(T(u,v,value));
                    ptrl = ptrl->next;
                }
                value = (ptr->num_edges*SIGMA + E) / E;
                coeff.push_back(T(u,u,value));
            }
            // Is a special link to a Purkinje cell - PMJ
            else
            {
                double sum = C;
                while (ptrl != NULL)
                {
                    int v = ptrl->dest->id;
                    if (ptrl->dest->type == 0)
                    {
                        double value = -B / C;
                        sum += B;
                        coeff.push_back(T(u,v,value));
                    }
                    else
                    {
                        double value = -A / C;
                        sum += A;
                        coeff.push_back(T(u,v,value));
                    }
                    ptrl = ptrl->next;
                }
                sum /= C;
                coeff.push_back(T(u,u,sum));
            }  
        }
        ptr = ptr->next;
    }
    a.setFromTriplets(coeff.begin(),coeff.end());
    a.makeCompressed();
}

void Solver::assembleLoadVector (VectorXd &b)
{
    int np = b.size();
    for (int i = 0; i < np; i++)
        b(i) = vol[i].vOld;
}

void Solver::moveVstar (const VectorXd vm)
{
    int np = vm.size();
    for (int i = 0; i < np; i++)
    {
        vol[i].vStar = vm(i);
        vol[i].cell->v = vm(i);
    }
}

void Solver::solveODE (double t)
{
    int ncells = g->getTotalNodes();
    // Solve ODE for each cell
    #pragma omp parallel for num_threads(nthreads)
    for (int id = 0; id < ncells; id++)
    {
        // Advance to timestep n+1 all variables
        vol[id].cell->dvdtclock += dt;

        // Compute the state variables for the current cell
        vol[id].cell->comp_revs();
        vol[id].cell->comp_ina ();
        vol[id].cell->comp_inal ();
        vol[id].cell->comp_inab ();
        vol[id].cell->comp_ical ();
        vol[id].cell->comp_icat ();
        vol[id].cell->comp_icab ();
        vol[id].cell->comp_ito1 ();
        vol[id].cell->comp_ikr ();
        vol[id].cell->comp_iks ();
        vol[id].cell->comp_ik1 ();
        vol[id].cell->comp_inaca ();
        vol[id].cell->comp_inak ();
        vol[id].cell->comp_ipca ();
        vol[id].cell->comp_if ();
        vol[id].cell->comp_istim (t);
        vol[id].cell->comp_itot ();

        vol[id].cell->comp_ip3 ();
        vol[id].cell->comp_qrel1 ();
        vol[id].cell->comp_qrel2 ();
        vol[id].cell->comp_qup1 ();
        vol[id].cell->comp_qup2 ();
        vol[id].cell->comp_qtr1 ();
        vol[id].cell->comp_qtr2 ();

        vol[id].cell->comp_conc ();

        vol[id].cell->dvdt	 = -vol[id].cell->itot;
        vol[id].cell->v	    += vol[id].cell->dvdt*dt;
    }
}

void Solver::writePlotData (double t)
{
    for (int i = 1; i < plot->np; i++)
        fprintf(plot->plotFile[i-1],"%.10lf %.10lf\n",t,vol[plot->ids[i]].vOld);
}

void Solver::writeVTKFile (int iter)
{
    FILE *file;
    int np, ne;
    char filename[50];
    Node *ptr = g->getListNodes();
    np = g->getTotalNodes();
    ne = g->getTotalEdges();

    // Escrever o potencial transmembranico
    sprintf(filename,"VTK/sol%d.vtk",iter);
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
    ptr = g->getListNodes();
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
    ptr = g->getListNodes();
    while (ptr != NULL)
    {
        fprintf(file,"%e\n",vol[ptr->id].vOld);
        ptr = ptr->next;
    }
    fclose(file);
}

bool Solver::isConnToPMJ (Edge *e)
{
    while (e != NULL)
    {
        if (e->dest->type == 1) return true;
        e = e->next;
    }
    return false;
}

void Solver::calcMaxDerivative (double t)
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        double diff = vol[i].cell->v - vol[i].vOld;
        // Considerar o primeiro estimulo
        if (diff > dvdt[i].value && t > 0.0)
        {
            dvdt[i].value = diff;
            dvdt[i].t = t;
        }
    }
}

void Solver::calcVelocity ()
{
    FILE *vFile = fopen("Output/v.txt","w+");
    FILE *dFile = fopen("Output/delay.txt","w+");
    FILE *tFile = fopen("Output/time.txt","w+");
    int np = vel->np;
    int nterm = g->getNTerm();
    int *term = g->getTerm();
    for (int i = 0; i < np; i++)
    {
        // Calcular a velocidade instantanea. Usar 10 volumes para tras do volume de referencia
        g->dijkstra(vel->ids[i]);
        double *dist = g->getDist();
        double t = dvdt[vel->ids[i]].t - dvdt[vel->ids[i] - OFFSET].t;
        double velocity = dist[vel->ids[i] - OFFSET] / t;

        // Checar se eh maior que a tolerancia
	    if (t < 0 || fabs(dvdt[vel->ids[i]].value - dvdt[vel->ids[i] - OFFSET].value) > 16.0)
            velocity = 0.0;
        fprintf(vel->velocityFile,"\n\n[!] Propagation velocity! Id = %d\n",vel->ids[i]);
        fprintf(vel->velocityFile,"t1 = %.10lf\n",dvdt[vel->ids[i] - OFFSET].t);
        fprintf(vel->velocityFile,"dvdt[%d] = %.10lf\n\n",vel->ids[i]- OFFSET ,dvdt[vel->ids[i] - OFFSET].value);
        fprintf(vel->velocityFile,"t2 = %.10lf\n",dvdt[vel->ids[i]].t);
        fprintf(vel->velocityFile,"dvdt[%d] = %.10lf\n",vel->ids[i],dvdt[vel->ids[i]].value);
        fprintf(vel->velocityFile,"delta_x = %.10lf\n",dist[vel->ids[i] - OFFSET]);
        fprintf(vel->velocityFile,"delta_t = %.10lf\n",t);
        fprintf(vel->velocityFile,"\n!!!!!!!! Propagation velocity = %lf cm/s !!!!!!!!!!\n",velocity*1000.0);
        fprintf(vel->velocityFile,"\n=============================================================\n\n");

        fprintf(vFile,"%lf\n",velocity*1000.0);
        fprintf(tFile,"%lf\n",dvdt[vel->ids[i]].t);
    }
    for (int i = 0; i < nterm; i++)
    { 
        printf("Delay between %d and %d\n",term[i],term[i]-10);
        fprintf(dFile,"%.10lf\n",dvdt[term[i]].t-dvdt[term[i]-10].t);
    }
    //fprintf(dFile,"%.10lf\n",dvdt[plot->delay[1]].t-dvdt[plot->ids[0]].t);
    fclose(vel->velocityFile);
    fclose(dFile);
    fclose(vFile);
    fclose(tFile);
}

void Solver::nextTimestep ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        vol[i].vNew = vol[i].cell->v;
        vol[i].vOld = vol[i].vNew;
    }
}

void Solver::error (const char msg[])
{
    printf("[-] ERROR ! %s !\n",msg);
    exit (EXIT_FAILURE);
}
