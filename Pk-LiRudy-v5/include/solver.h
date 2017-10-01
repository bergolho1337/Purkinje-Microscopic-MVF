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

struct Velocity
{
  FILE *velocityFile;                         // Referencia para o arquivo aonde a velocidade sera gravada
  int np;                                     // Numero de pontos em que a velocidade sera medida
  int id_source;                              // Identificador do primeiro nodo (fonte)
  int *ids;                                   // Identificador dos pontos em que a velocidade sera calculada (id[0] -> fonte) 
  double t1;                                  // Vetor do tempo inicial do ponto de fonte
  double *t2;                                 // Vetor de tempos finais dos pontos
}typedef Velocity;

// Estrutura para o plot do grafico de um volume
struct Plot
{
  FILE **plotFile;                            // Referencia para os arquivos de plot
  int np;                                     // Numero de pontos que serao plotados
  int *ids;                                   // Identificador dos volumes que serao plotados
}typedef Plot;

class Solver
{
    static constexpr double BETA = 0.14;
    static constexpr double Cm = 1.0;
    static constexpr double SIGMA = 0.002;	// default 0.004
    static constexpr double h2 = 0.25;          // default 0.25
    static constexpr double d2 = 0.002;
    static constexpr double RPMJ = 11000.0;     // default 11000
    static constexpr int NSC = 5;               // Number of stimulus cells
    static constexpr int OFFSET = 10;
public:
    Solver (int argc, char *argv[]);
    void solve ();
    void print ();
    void error (const char msg[]);
private:
    Graph *g;                           // Graph representing the Purkinje network
    CVolume *vol;                       // Vector of control volumes
    Derivative *dvdt;                   // Vetor das derivadas maximas de cada ponto da malha
    Plot *plot;                         // Vetor com identificadores dos volumes do plot
    Velocity *vel;                      // Vetor com as velocidade de propagacao dos volumes
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
    void setMatrix (SpMat &A);
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
