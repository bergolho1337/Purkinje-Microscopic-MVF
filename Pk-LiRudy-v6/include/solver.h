#ifndef SOLVER_H_
#define SOLVER_H_

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

struct CVolume
{
  double vOld;                              
  double vNew;                            
  double vStar;
  Cell *cell;                              
}typedef CVolume;

struct Derivative
{
  double t;                                   // Time of the maximum derivative
  double value;                               // Derivative value
}typedef Derivative;

// Structure for the calculus of the propagation velocity
struct Velocity
{
  FILE *velocityFile;                         // Reference to the velocity file
  int np;                                     // Number of points that the velocity will be calcualted
  int id_source;                              // Identifier of the fist node (source)
  int *ids;                                   // Identifienr of the terminals nodes (sinks) (ids[0] --> source)  
  double t1;                                  // Time when the first node despolarized (source)
  double *t2;                                 // Time when the other nodes despolarized (sink)
}typedef Velocity;

// Structure for the plot of the graphics
struct Plot
{
  FILE **plotFile;                            // Reference to the plot files 
  int np;                                     // Number of points to be plot
  int *ids;                                   // Identifier of the plot points
}typedef Plot;

class Solver
{
    // Constants of the monodomain equation
    //static constexpr double BETA = 0.14;
    static constexpr double Cm = 1.2;
    static constexpr double SIGMA = 0.0019;	
    static constexpr double h2 = 0.25;          
    static constexpr double d2 = 0.002;
    static constexpr double RPMJ = 11000.0;     
    static constexpr int NSC = 5;               
    static constexpr int OFFSET = 5;
public:
    Solver (int argc, char *argv[]);
    void solve ();
    void print ();
    void error (const char msg[]);
private:
    Graph *g;                           // Graph representing the Purkinje network
    CVolume *vol;                       // Vector of control volumes
    Derivative *dvdt;                   // Vector with maximum derivative for each volume
    Plot *plot;                         // Vector with the plot points 
    Velocity *vel;                      // Vector with the velocity of the plot points
    Func *func;                         // Vector of function of the celullar model
    int M;                              // Number of timesteps in time
    double dt;                          // Size timestep in time
    double tmax;                        // Maximum time of the simulation
    double dx;                          // Size timestep in space
    string mesh_filename;               // Mesh filename
    string steady_filename;             // Input Steady-State filename
    string plot_filename;               // Plot id filename

    double alfa;                        // Parameter: R_pmj * Vol_pmj
    double d1;                          // Parameter: d1
    double BETA;                        // Parameter: 4 / d

    void setSensibilityParam (int argc, char *argv[]);
    void setTypeCell ();  
    void setControlVolumes ();
    void setInitCondFromFile ();
    void setStimCells ();
    void setStimSettings ();
    void setModelParameters ();
    void setVelocityPoints ();
    void setDerivative ();
    void setPlot ();
    void setTerm ();
    void setMatrix (SpMat &a);
    void setMatrix2 (SpMat &a);
    void assembleLoadVector (VectorXd &b);
    void moveVstar (const VectorXd vm);
    bool isConnToPMJ (Edge *e);
    void solveODE (double t);
    void nextTimestep ();
    void calcMaxDerivative (double t);
    void calcVelocity ();
    void writeVTKFile (int iter);
    void writePlotData (double t);
};



void swap (double **a, double **b);
void printProgress (int iter, int max_iter);

#endif
