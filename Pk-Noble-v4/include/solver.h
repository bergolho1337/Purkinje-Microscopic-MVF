#ifndef SOLVER_H_
#define SOLVER_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <Eigen/Sparse>
#include "../include/graph.h"
#include "../include/noble.h"

using namespace std;
using namespace Eigen;

//#define NPLOT 6                    // Numero de volumes usados para o plot
#define NPLOT 11

// Macros for the Eigen library structures
typedef SparseMatrix<double> SpMat;
typedef Triplet<double> T;

// Function pointer
typedef double (*Func) (int type, int point, double t, double y[]);

// ----------------------------------------------------------------------------------------------
// Vetor com os identificadores dos volumes do plot: 2 cm 
//const int ids_dog[NPLOT] = {50,65,80,95,110,120};                   // Dog
//const int ids_pig[NPLOT] = {50,100,150,200,250,294};              // Pig
//const int ids_alien[NPLOT] = {50,80,110,140,170,200};               // Alien
//const int ids_orc[NPLOT] = {50,60,70,80,90,100};                  // Orc

// Vetor com os identificadores dos volumes do plot: 5 cm
const int ids_dog[NPLOT] = {50,75,100,125,150,175,200,225,250,275,304};       // Dog
const int ids_pig[NPLOT] = {50,118,186,254,322,390,458,526,594,662,734};      // Pig
const int ids_alien[NPLOT] = {50,95,140,185,230,275,320,365,410,455,500};     // Alien
const int ids_orc[NPLOT] = {50,70,90,110,130,150,170,190,210,230,250};        // Orc

// ----------------------------------------------------------------------------------------------

struct CVolume
{
  double *yOld;                               // Solution at timestep n     {v^n, gate^n}
  double *yNew;                               // Solution at timestep n+1   {v^n+1, gate^n+1}
  double *yStar;                              // Solution at timestep n+1/2 {v^n+1/2, gate^n}
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
    static constexpr double SIGMA = 0.004;
    static constexpr double h2 = 0.25;
    static constexpr double d2 = 0.002;
    static constexpr double RPMJ = 11000.0;
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

    double alfa;                        // Parameter: R_pmj * Vol_pmj
    double d1;                          // Parameter: d1

    void setSensibilityParam (int argc, char *argv[]);
    void setTypeCell (const char s[]);  
    void setControlVolumes ();
    void setFunctions ();
    void setInitCondFromFile ();
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