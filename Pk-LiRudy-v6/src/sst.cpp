#include "../include/sst.h"

// Number of threads to solve the system of ODEs
static constexpr int nthreads = 4;

SteadyState::SteadyState (int argc, char *argv[])
{
    dt = atof(argv[2]);
    tmax = atof(argv[3]);
    mesh_filename = argv[4];
    steady_filename = argv[5];
    M = nearbyint(tmax/dt);
    setSensibilityParam(argc,argv);
    g = new Graph(mesh_filename,dx);

    setControlVolumes();
    setInitCondModel();
    setModelParameters();

    //g->printterm();
    //g->print();
    //print();
}

void SteadyState::solve ()
{
    FILE *sstFile = fopen(steady_filename.c_str(),"w+");
    int np = g->getTotalNodes();
    // Build the matrix
    SpMat A(np,np);
    setMatrix(A);
    SparseLU<SpMat> sparseSolver(A);
    
    // Declare RHS and the solution vector
    VectorXd b(np);
    VectorXd x(np);

    printf("[!] Solving transient problem ... \n");
    printf("[!] Progress\n");
    fflush(stdout);
    for (int i = 0; i < M; i++)
    {
        double t = i*dt;

        printProgress(i,M);
        //if (i % 10 == 0) writeVTKFile(i);
        if (i == 40000) writeSteadyStateFile(sstFile);

        // Solve the PDE 
        assembleLoadVector(b);
        x = sparseSolver.solve(b);
        moveVstar(x);

        // Solve the ODE
        solveODE(t);

        nextTimestep();
    }
    fclose(sstFile);
    printf("\nok\n");
}

void SteadyState::moveVstar (const VectorXd vm)
{
    int np = vm.size();
    for (int i = 0; i < np; i++)
    {
        vol[i].vStar = vm(i);
        vol[i].cell->v = vm(i);
    }
}

void SteadyState::solveODE (double t)
{
    int ncells = g->getTotalNodes();
    #pragma omp parallel for num_threads(nthreads)
    for (int id = 0; id < ncells; id++)
    {
        // Advance to timestep n+1 all variables
        vol[id].cell->dvdtclock += dt;

        // Compute the state variables for the current cell
        vol[id].cell->comp_revs();
        vol[id].cell->comp_ina();
        vol[id].cell->comp_inal();
        vol[id].cell->comp_inab();
        vol[id].cell->comp_ical();
        vol[id].cell->comp_icat();
        vol[id].cell->comp_icab();
        vol[id].cell->comp_ito1();
        vol[id].cell->comp_ikr();
        vol[id].cell->comp_iks();
        vol[id].cell->comp_ik1();
        vol[id].cell->comp_inaca();
        vol[id].cell->comp_inak();
        vol[id].cell->comp_ipca();
        vol[id].cell->comp_if();
        vol[id].cell->comp_istim(t);
        vol[id].cell->comp_itot();

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

void SteadyState::print ()
{
    printf("--------- Steady State -------------\n");
    printf("dt = %lf\n",dt);
    printf("tmax = %lf\n",tmax);
    printf("tmax/dt = %d\n",M);
    printf("dx = %lf\n",dx);
    printf("mesh_filename = %s\n",mesh_filename.c_str());
    printf("steady_filename = %s\n",steady_filename.c_str());
    printf("alfa = %lf\n",alfa);
    printf("d1 = %lf\n",d1);
    printf("------------------------------------\n");
}

void SteadyState::setMatrix (SpMat &a)
{
    // Compute the coefficients values
    double A = 1.0 / (RPMJ*dx*d1*d1);
    double B = (SIGMA) / (dx*dx);
    double C = (BETA*Cm) / (dt);
    double D = (BETA*Cm*alfa) / (dt);
    double E = (BETA*Cm*dx*dx) / (dt);
    
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
                    // Purkinje cell - Purkinje cell
                    if (ptrl->dest->type == 0)
                    {
                        double value = -B / C;
                        sum += B;
                        coeff.push_back(T(u,v,value));
                    }
                    // Purkinje cell - PMJ
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

void SteadyState::assembleLoadVector (VectorXd &b)
{
    int np = b.size();
    for (int i = 0; i < np; i++)
        b(i) = vol[i].vOld;
}

void SteadyState::setSensibilityParam (int argc, char *argv[])
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
}

void SteadyState::setControlVolumes ()
{
    Node *ptr = g->getListNodes();
    int np = g->getTotalNodes();
    vol = (Volume*)malloc(sizeof(Volume)*np);
    for (int i = 0; i < np; i++, ptr = ptr->next)
    {
        vol[i].type = ptr->type;
        vol[i].cell = new Cell();
    }
        
}

void SteadyState::setInitCondModel ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        vol[i].cell->setInitialConditions();
        vol[i].vOld = vol[i].cell->v;
    }
        
}

void SteadyState::setModelParameters ()
{
    // Calculate the global variables of the Li-Rudy model
    compGeometrics();
    setTimeSettings(dt,tmax);
    // Set stimulus variables for each cell
    setStimSettings();
    setStimCells();
}

void SteadyState::setStimCells ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < NSC; i++)
        vol[i].cell->setStimulus(true);
    for (int i = NSC; i < np; i++)
        vol[i].cell->setStimulus(false);
}

void SteadyState::setStimSettings ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        vol[i].cell->dvdtclock = 500;
        vol[i].cell->stimtime = 500;
        vol[i].cell->stimcount 	= -1;
    }
}

bool SteadyState::isConnToPMJ (Edge *e)
{
    while (e != NULL)
    {
        if (e->dest->type == 1) return true;
        e = e->next;
    }
    return false;
}

void SteadyState::nextTimestep ()
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        vol[i].vNew = vol[i].cell->v;
        vol[i].vOld = vol[i].vNew;
    }
}

void swap (double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}

void printProgress (int iter, int max_iter)
{
    double progress = iter / (double)max_iter;
    int barWidth = 100;

    cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) 
    {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
}

void SteadyState::writeVTKFile (int iter)
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

void SteadyState::writeSteadyStateFile (FILE *sstFile)
{
    int np = g->getTotalNodes();
    for (int i = 0; i < np; i++)
    {
        fprintf(sstFile,"%.10lf ",vol[i].cell->v);
        fprintf(sstFile,"%.10lf ",vol[i].cell->m);
        fprintf(sstFile,"%.10lf ",vol[i].cell->h);
        fprintf(sstFile,"%.10lf ",vol[i].cell->j);
        fprintf(sstFile,"%.10lf ",vol[i].cell->d);
        fprintf(sstFile,"%.10lf ",vol[i].cell->f);
        fprintf(sstFile,"%.10lf ",vol[i].cell->f2);
        fprintf(sstFile,"%.10lf ",vol[i].cell->fca);
        fprintf(sstFile,"%.10lf ",vol[i].cell->fca2);
        fprintf(sstFile,"%.10lf ",vol[i].cell->xs1);
        fprintf(sstFile,"%.10lf ",vol[i].cell->xs2);
        fprintf(sstFile,"%.10lf ",vol[i].cell->xr);
        fprintf(sstFile,"%.10lf ",vol[i].cell->a);
        fprintf(sstFile,"%.10lf ",vol[i].cell->i);
        fprintf(sstFile,"%.10lf ",vol[i].cell->i2);
        fprintf(sstFile,"%.10lf ",vol[i].cell->ml);
        fprintf(sstFile,"%.10lf ",vol[i].cell->ml3);
        fprintf(sstFile,"%.10lf ",vol[i].cell->hl);
        fprintf(sstFile,"%.10lf ",vol[i].cell->hl3);
        fprintf(sstFile,"%.10lf ",vol[i].cell->jl);
        fprintf(sstFile,"%.10lf ",vol[i].cell->jl3);
        fprintf(sstFile,"%.10lf ",vol[i].cell->casss);  
        fprintf(sstFile,"%.10lf ",vol[i].cell->cajsr);
        fprintf(sstFile,"%.10lf ",vol[i].cell->cacsr);
        fprintf(sstFile,"%.10lf ",vol[i].cell->cansr);
        fprintf(sstFile,"%.10lf ",vol[i].cell->cassl);
        fprintf(sstFile,"%.10lf ",vol[i].cell->nai);
        fprintf(sstFile,"%.10lf ",vol[i].cell->nassl);
        fprintf(sstFile,"%.10lf ",vol[i].cell->nasss);
        fprintf(sstFile,"%.10lf ",vol[i].cell->ki);
        fprintf(sstFile,"%.10lf ",vol[i].cell->cai);
        fprintf(sstFile,"%.10lf ",vol[i].cell->b);
        fprintf(sstFile,"%.10lf ",vol[i].cell->g);
        fprintf(sstFile,"%.10lf ",vol[i].cell->u);
        fprintf(sstFile,"%.10lf ",vol[i].cell->y);
        fprintf(sstFile,"%.10lf\n",vol[i].cell->camktrap);
    }
}
