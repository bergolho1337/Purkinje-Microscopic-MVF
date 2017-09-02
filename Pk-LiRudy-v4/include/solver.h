#ifndef SOLVER_H_
#define SOLVER_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <Eigen/Sparse>
#include "../include/graph.h"
#include "../include/lirudy.h"

using namespace std;
using namespace Eigen;

// ------------ Cable ------------------------------------
#define NPLOT 6                   
//#define NPLOT 11
// ------------ Bifurcation ------------------------------
//#define NPLOT 16
//#define NPLOT 31

// Macros for the Eigen library structures
typedef SparseMatrix<double> SpMat;
typedef Triplet<double> T;

// Function pointer
typedef double (*Func) (int type, int point, double t, double y[]);

// ----------------------------------------------------------------------------------------------
// -------------------- Cable -------------------------------------------------------------------
// Vetor com os identificadores dos volumes do plot: 2 cm 
const int ids_dog[NPLOT] = {50,65,80,95,110,120};                   // Dog
const int ids_pig[NPLOT] = {50,100,150,200,250,294};              // Pig
const int ids_alien[NPLOT] = {50,80,110,140,170,200};               // Alien
const int ids_orc[NPLOT] = {50,60,70,80,90,100};                  // Orc

// Vetor com os identificadores dos volumes do plot: 5 cm
//const int ids_dog[NPLOT] = {50,75,100,125,150,175,200,225,250,275,304};       // Dog
//const int ids_pig[NPLOT] = {50,118,186,254,322,390,458,526,594,662,734};      // Pig
//const int ids_alien[NPLOT] = {50,95,140,185,230,275,320,365,410,455,500};     // Alien
//const int ids_orc[NPLOT] = {50,70,90,110,130,150,170,190,210,230,250};        // Orc

// ----------------------------------------------------------------------------------------------
// -------------------- Bifurcation -------------------------------------------------------------
// Vetor com os identificadores dos volumes do plot: 2 cm 
//const int ids_dog[NPLOT] = {50,65,80,95,110,121, 122,152,182,212,242, 243,273,303,333,363};    
//const int ids_pig[NPLOT] = {50,100,150,200,250,294, 344,394,444,494,588, 638,688,738,788,882};              
//const int ids_alien[NPLOT] = {50,80,110,140,170,200, 240,280,320,360,400, 440,480,520,560,600};              
//const int ids_orc[NPLOT] = {50,60,70,80,90,100, 105,125,145,165,200, 205,225,245,265,300};               

// Vetor com os identificadores dos volumes do plot: 5 cm
//const int ids_dog[NPLOT] = {50,75,100,125,150,175,200,225,250,275,304, 334,364,394,424,454,484,514,544,574,608, 638,668,698,728,758,788,818,848,878,912};      
//const int ids_pig[NPLOT] = {50,118,186,254,322,390,458,526,594,662,735, 805,875,945,1015,1085,1155,1225,1295,1365,1470, 1540,1610,1680,1750,1820,1890,1960,2030,2100,2205};     
//const int ids_alien[NPLOT] = {50,95,140,185,230,275,320,365,410,455,500, 550,600,650,700,750,800,850,900,950,1000, 1050,1100,1150,1200,1250,1300,1350,1400,1450,1500};    
//const int ids_orc[NPLOT] = {50,70,90,110,130,150,170,190,210,230,250, 275,300,325,350,375,400,425,450,475,500, 525,550,575,600,625,650,675,700,725,750};        


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
    static constexpr double SIGMA = 0.004;
    static constexpr double h2 = 0.25;
    static constexpr double d2 = 0.002;
    static constexpr double RPMJ = 11000.0;
    static constexpr int NSC = 5;               // Number of stimulus cells
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