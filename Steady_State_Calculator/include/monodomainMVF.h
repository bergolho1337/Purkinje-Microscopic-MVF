#ifndef MONODOMAINFEM_H_
#define MONODOMAINFEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include "../include/noble.h"
#include "../include/plot.h"
#include "../include/linSolver.h"
#include "../include/graph.h"

using namespace std;

// >>>>>>>>>>> DESCOMENTAR ESSES defines PARA ATIVAR AS FLAGS <<<<<<<<<<<
//#define DEBUG 1                                       // Flag para debugacao e imprimir informacoes na tela (matrizes e vetores)
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// Function pointer
typedef double (*Func) (int point, double t, double vm, double m, double h, double n);     

/* ============================== CONSTANTES ================================================== */
const double BETA = 0.14;                     // Razao area superficial por volume (cm^-1) (Bruno)
const double Cm = 1.0;                        // Capacitancia da membrana (uF/cm^2) (Bruno)
const double SIGMA = 0.004;                  // Condutividade citoplasmatica da celula (mS/cm) (default=0.004)
/* ============================================================================================ */

struct MonodomainMVF;
struct Point;
struct Derivative;

// Estrutura do resolvedor da equacao do Monodominio
struct MonodomainMVF
{
  int id;                                     // Identificador da malha
  int nVolFiber;                              // Numero de volumes por fibra
  int M;                                      // Numero de subintervalos no tempo
  double dx;                                  // Tamanho da discretizacao no espaco (h)
  double dt;                                  // Tamanho da discretizacao no tempo (k)
  double t_max;                               // Tempo maximo de simulacao
  double alfa;                                // Numero usado no sistema linear da EDP
  Func *functions;                            // Vetor de ponteiros para as funcoes envolvidas no metodo
  double *K;                                  // Matriz global do sistema linear
  double *F;                                  // Vetor de carga global
  double *VNew;                               // Vetor com o valor do potencial transmembranico de cada ponto no tempo n
  double *VOld;                               // Vetor com o valor do potencial transmembranico de cada ponto no tempo n-1
  double *Vstar;                              // Vetor com o valor do potencial transmembranico de cada ponto no tempo intermediario *
  double *mNew;                               // Vetor com o valor da variavel de estado de cada ponto no tempo n
  double *mOld;                               // Vetor com o valor da variavel de estado de cada ponto no tempo n
  double *hNew;                               // Vetor com o valor da variavel de estado de cada ponto no tempo n
  double *hOld;                               // Vetor com o valor da variavel de estado de cada ponto no tempo n
  double *nNew;                               // Vetor com o valor da variavel de estado de cada ponto no tempo n
  double *nOld;                               // Vetor com o valor da variavel de estado de cada ponto no tempo n
  Derivative *dvdt;                           // Vetor das derivadas maximas de cada ponto da malha
  Graph *g;                                   // Grafo contendo a estrutura da malha
  char filename[50];                          // Nome do arquivo de saida
}typedef MonodomainMVF;

// Estrutura de um ponto do dominio
struct Point
{
  double x, y, z;
}typedef Point; 

// Estrutura do calculo da derivada maxima
struct Derivative
{
  double t;                                   // Tempo em que a derivada aconteceu
  double value;                               // Valor da derivada
}typedef Derivative;

/* ================================= FUNCTIONS ======================================================= */
MonodomainMVF* newMonodomainMVF (int argc, char *argv[]);
void setInitialConditionsModel (MonodomainMVF *monoMVF, int argc, char *argv[]);
Func* buildFunctions ();
void freeMonodomain (MonodomainMVF *monoMVF);
void printInfoModel (MonodomainMVF *monoMVF);
void assembleMatrix (MonodomainMVF *monoMVF);
void solveMonodomain (MonodomainMVF *monoMVF);
void writeSteadyStateFile (FILE *steadyFile, int nPoints, double vm[], double m[], double h[], double n[]);
void assembleLoadVector (MonodomainMVF *monoMVF);
void solveEDO (MonodomainMVF *monoMVF, double t);
void writeVTKFile (double *Vm, Graph *g, int k);
/*
double* buildLocalMassMatrix (double h);
double* buildLocalStiffMatrix (double h);
double* buildGlobalMatrixFromLocal (double *local_A, int *map, int np, int ne);
double* buildGlobalMatrix (double *A, double *B, double dt, int np);
void setBoundaryConditions (double *K, int np);
void scaleFactor (double *V, double scale, int np);
void setInitialConditionsModel (MonodomainFEM *monoFEM);
void setInitialConditionsModel_FromFile (MonodomainFEM *monoFEM, char *filename);
void calcPropagationVelocity (Derivative *dvdt, double t);
void findBifurcation (MonodomainFEM *monoFEM);
void kirchoffCondition_Matrix (MonodomainFEM *monoFEM);
void kirchoffCondition_Vector (MonodomainFEM *monoFEM);
void setVelocityPoints (double dx, int p1, int p2);

void calcMaximumDerivative (Derivative *dvdt, int nPoints, double t, double *vold, double *vnew);
void calcVelocity (Derivative *dvdt);

void solveMonodomain (MonodomainFEM *monoFEM);
;

void writeMaximumDerivative (Derivative *dvdt, int nPoints);

*/

void printError (char *msg);
/* =================================================================================================== */

#endif