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
#define NPLOT 6       // Numero de volumes usados para o plot 
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
const double ALFA = 1.375;                  // Analise de sensibilidade: (h2*h2*d2*RPMJ)

/* Vetor com os identificadores dos volumes do plot: 2 cm */
const int ids_dog[NPLOT] = {50,65,80,95,110,120};                   // Dog
const int ids_pig[NPLOT] = {50,100,150,200,250,294};              // Pig
const int ids_alien[NPLOT] = {50,80,110,140,170,200};               // Alien
const int ids_orc[NPLOT] = {50,60,70,80,90,100};                  // Orc
/* ============================================================================================ */

struct MonodomainMVF;
struct Point;
struct Derivative;
struct Volume;
struct Velocity;
struct Plot;

// Estrutura do resolvedor da equacao do Monodominio
struct MonodomainMVF
{
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
  Plot *plot;                                 // Vetor com identificadores dos volumes do plot
  Velocity *vel;                              // Vetor com as velocidade de propagacao dos volumes
  Graph *g;                                   // Grafo contendo a estrutura da malha
  char filename[200];                         // Nome do arquivo de saida
  int *ids;                                   // Vetor com os identificadores dos volumes a serem plotados
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

// Estrutura para o calculo da velocidade de propagacao
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
  int *ids;                                    // Identificador dos volumes que serao plotados
}typedef Plot;

// Estrutura de um volume de controle
struct Volume
{
  double *yOld;                               // Contem a solucao no instante n     {v^n, gate^n}
  double *yNew;                               // Contem a solucao no instante n+1   {v^n+1, gate^n+1}
  double *yStar;                              // Contem a solucao no instante n+1/2 {v^n+1/2, gate^n}
}typedef Volume;

/* ================================= FUNCTIONS ======================================================= */
MonodomainMVF* newMonodomainMVF (int argc, char *argv[]);
void setInitialConditionsModel_FromFile (Volume vol[], int np, const char filename[]);
Func* buildFunctions ();
void allocateMonodomain (MonodomainMVF *monoMVF);
void setTypeCell (MonodomainMVF *monoMVF, const char filename[]);
void setVelocityPoints (Velocity *v, int ids[]);
void setPlot (Plot *p, int ids[]);
void assembleMatrix (MonodomainMVF *monoMVF);
void solveMonodomain (MonodomainMVF *monoMVF);
void assembleLoadVector (MonodomainMVF *monoMVF);
void moveVstar (Volume vol[], double Vstar[], int np);
void solveEDO (MonodomainMVF *monoMVF, double t);
void writeVTKFile (Volume *vol, Graph *g, int k);
void writeSteadyStateFile (FILE *steadyFile, int nPoints, Volume vol[]);
void writePlotData(double t, Volume vol[], Plot *plot);
void calcMaximumDerivative (Derivative dvdt[], int np, double t, Volume vol[]);
void calcVelocity (Velocity *v, Derivative dvdt[], double dist[]);
void nextTimestep (Volume vol[], int np);
void printError (char *msg);
void printInfoModel (MonodomainMVF *monoMVF);
void freeMonodomain (MonodomainMVF *monoMVF);
void freeVolume (Volume vol[], int np);
void freeVelocity (Velocity *v);
void freePlot (Plot *plot);
void swap (double **a, double **b);
/* =================================================================================================== */

#endif