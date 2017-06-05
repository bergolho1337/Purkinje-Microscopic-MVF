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
struct Retropropagation;
struct Velocity;
struct Plot;

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
  Retropropagation *retro;                    // Estrutura usada para verificar se houve retropropagacao na simulacao
  Plot *plot;                                // Identificador do volume do plot
  Velocity *vel;                             // Estrutura da velocidade propagacao
  Graph *g;                                   // Grafo contendo a estrutura da malha
  char filename[50];                             // Nome do arquivo de saida
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

// Estrutura da verificacao de uma retropropagacao
struct Retropropagation
{
  int id;                                     // Identificador do Node usado como referencia
  int id_prev;                                // Identificador do Node vizinho ao de referencia
  double t;                                   // Tempo em que a menor derivada espacial foi registrada
  double min_deriv;                           // Valor da menor derivada espacial para o Node de referencia
}typedef Retropropagation;

// Estrutura para o calculo da velocidade de propagacao
struct Velocity
{
  FILE *velocityFile;                         // Referencia para o arquivo aonde a velocidade sera gravada
  int np;                                     // Numero de pontos em que a velocidade sera medida
  int id_source;                              // Identificador do primeiro nodo (fonte)
  int *ids;                                   // Identificador dos pontos em que a velocidade sera calculada (id[0] -> fonte) 
  double t1;                                  // Vetor do tempo inicial do ponto de fonte
  double *t2;                                 // Vetor de tempos finais dos pontos
  double *delta_x;                            // Espaco entre os pontos de referencia
}typedef Velocity;

// Estrutura para o plot do grafico de um volume
struct Plot
{
  FILE **plotFile;                            // Referencia para os arquivos de plot
  int np;                                     // Numero de pontos que serao plotados
  int *ids;                                    // Identificador dos volumes que serao plotados
}typedef Plot;

/* ================================= FUNCTIONS ======================================================= */
MonodomainMVF* newMonodomainMVF (int argc, char *argv[]);
void setInitialConditionsModel_FromFile (MonodomainMVF *monoMVF, char filename[]);
Func* buildFunctions ();
void freeMonodomain (MonodomainMVF *monoMVF);
void printInfoModel (MonodomainMVF *monoMVF);
void assembleMatrix (MonodomainMVF *monoMVF);
void solveMonodomain (MonodomainMVF *monoMVF);
void writeSteadyStateFile (FILE *steadyFile, int nPoints, double vm[], double m[], double h[], double n[]);
void assembleLoadVector (MonodomainMVF *monoMVF);
void solveEDO (MonodomainMVF *monoMVF, double t);
void writeVTKFile (double *Vm, Graph *g, int k);
void writePlotData(double t, double *v, Plot *plot);
//void setVelocityPoints (Velocity *v, double dx, int p1, int p2);
void setVelocityPoints (Velocity *v, int np, int ids[], double dx[]);
void setRetropropagation (Retropropagation *r, int id);
void setPlot (Plot *p, int ids[], int np);
void calcMaximumDerivative (Derivative *dvdt, int nPoints, double t, double *vold, double *vnew);
void calcMinimumSpacialDerivative (Retropropagation *r, double t, double v, double v_prev);
void writeMaximumDerivative (Derivative *dvdt, int nPoints);
void writeMinimumSpacialDerivative (Retropropagation *r);
void calcVelocity (Velocity *v, Derivative *dvdt);
void swap (double **a, double **b);


void printError (char *msg);
/* =================================================================================================== */

#endif