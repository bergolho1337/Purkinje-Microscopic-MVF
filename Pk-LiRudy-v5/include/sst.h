#ifndef SST_H_
#define SST_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <omp.h>
#include <Eigen/Sparse>
#include "../include/graph.h"
#include "../include/lirudy.h"

using namespace std;
using namespace Eigen;

// Macros for the Eigen library structures
typedef SparseMatrix<double> SpMat;
typedef Triplet<double> T;

// Function pointer
typedef double (*Func) (int type, int point, double t, double y[]);

struct Volume
{
  int type;  
  double vOld;                                
  double vNew;
  double vStar;
  Cell *cell;
}typedef Volume;

class SteadyState
{
    static constexpr double BETA = 0.14;
    static constexpr double Cm = 1.0;
    static constexpr double SIGMA = 0.002;	// default 0.004
    static constexpr double h2 = 0.25;          // default 0.25
    static constexpr double d2 = 0.002;
    static constexpr double RPMJ = 11000.0;     // default 11000
    static constexpr int NSC = 5;               // Number of stimulus cells
public:
    SteadyState (int argc, char *argv[]);
    void solve ();
    void print ();
private:
    Graph *g;                           // Graph representing the Purkinje network
    Volume *vol;                        // Vector of control volumes
    Func *func;                         // Vector of function of the celullar model
    int M;                              // Number of timesteps in time
    double dt;                          // Size timestep in time
    double tmax;                        // Maximum time of the simulation
    double dx;                          // Size timestep in space
    string mesh_filename;               // Mesh filename
    string steady_filename;             // Output Steady-State filename

    double alfa;                        // Parameter: R_pmj * Vol_pmj
    double d1;                          // Parameter: d1

    void setSensibilityParam (int argc, char *argv[]);
    void setControlVolumes ();
    void setInitCondModel ();
    void setStimCells ();
    void setStimSettings ();
    void setModelParameters ();
    void setMatrix (SpMat &A);
    void assembleLoadVector (VectorXd &b);
    void moveVstar (const VectorXd vm);
    bool isConnToPMJ (Edge *e);
    void solveODE (double t);
    void nextTimestep ();
    void writeVTKFile (int iter);
    void writeSteadyStateFile (FILE *sstFile);

};

void swap (double **a, double **b);
void printProgress (int iter, int max_iter);

#endif