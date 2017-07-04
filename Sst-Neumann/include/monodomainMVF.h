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
//#define DEBUG 1     // Flag para debugacao e imprimir informacoes na tela (matrizes e vetores)
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// Function pointer
typedef double (*Func) (int type, int point, double t, double y[]);     

/* ============================== CONSTANTES ================================================== */
const double BETA = 0.14;                     // Razao area superficial por volume (cm^-1) (Bruno)
const double Cm = 1.0;                        // Capacitancia da membrana (uF/cm^2) (Bruno)
const double SIGMA = 0.004;                  // Condutividade citoplasmatica da celula (mS/cm) (default=0.004)
const double h2 = 0.25;                        // Tamanho do volume de controle do PMJ (cm) (default=40)
const double d2 = 0.002;                        // Diametro do volume de controle do PMJ (cm)
const double d1 = 0.002;                      // Diametro do volume de controle de uma Purkinje cell (cm)
const double RPMJ = 11000;                  // Resistencia do PMJ (k ohm) (default=11000)
/* ============================================================================================ */

struct MonodomainMVF;
struct Point;
struct Derivative;
struct Volume;

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
  double gamma;                               // Numero usado no sistema linear da EDP
  double delta;                               // Numero usado no sistema linear da EDP
  double theta;                               // Numero usado no sistema linear da EDP
  double eta;                                 // Numero usado no sistema linear da EDP
  Func *functions;                            // Vetor de ponteiros para as funcoes envolvidas no metodo
  double *K;                                  // Matriz global do sistema linear
  double *F;                                  // Vetor de carga global
  Volume *vol;                                // Vetor de volumes de controle
  double *Vstar;                              // Vetor com o valor do potencial transmembranico de cada ponto no tempo intermediario *
  Derivative *dvdt;                           // Vetor das derivadas maximas de cada ponto da malha
  Graph *g;                                   // Grafo contendo a estrutura da malha
  char filename[200];                         // Nome do arquivo de saida
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

// Estrutura de um volume de controle
struct Volume
{
  double *yOld;                               // Contem a solucao no instante n     {v^n, gate^n}
  double *yNew;                               // Contem a solucao no instante n+1   {v^n+1, gate^n+1}
  double *yStar;                              // Contem a solucao no instante n+1/2 {v^n+1/2, gate^n}
}typedef Volume;

/* ================================= FUNCTIONS ======================================================= */
void UsageSteady (char *pName);
void UsageSolver (char *pName);
MonodomainMVF* newMonodomainMVF (int argc, char *argv[]);
void setInitialConditionsModel (Volume vol[], int np, int argc, char *argv[]);
Func* buildFunctions ();
void allocateMonodomain (MonodomainMVF *monoMVF);
void freeMonodomain (MonodomainMVF *monoMVF);
void freeVolume (Volume vol[], int np);
void printInfoModel (MonodomainMVF *monoMVF);
void assembleMatrix (MonodomainMVF *monoMVF);
void solveMonodomain (MonodomainMVF *monoMVF);
void assembleLoadVector (MonodomainMVF *monoMVF);
void moveVstar (Volume vol[], double Vstar[], int np);
void solveEDO (MonodomainMVF *monoMVF, double t);
void writeVTKFile (Volume *vol, Graph *g, int k);
void writeSteadyStateFile (FILE *steadyFile, int nPoints, Volume vol[]);
void nextTimestep (Volume vol[], int np);
void swap (double **a, double **b);
void printError (char *msg);
/* =================================================================================================== */

#endif