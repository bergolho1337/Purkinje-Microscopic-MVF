#ifndef NOBLE_H_
#define NOBLE_H_

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

// Maximum time = 2000 ms
// dt = 0.1 ms

// Function pointer
typedef double (*Func) (int type, int point, double t, double vm, double m, double h, double n);

/* ============================== CONSTANTS ========================================== */

// Number of equations
const int num_eq = 4;

// Stimulus current
const double v_stim__Nob = 1000.0;

// Cycle length
const double cycle_length = 500.0;  

// Initial conditions (default)
const double v0__Nob = -75.5344986658;
const double m0__Nob = 0.0605467272;
const double h0__Nob = 0.7259001355;
const double n0__Nob = 0.4709239708;
// Steady state
// -79.0666103401 0.0499158420 0.8049140578 0.2603110528
// -75.5344986658 0.0605467272 0.7259001355 0.4709239708

// Parameters:
const double g_Na_max = 4.0e+02;
const double E_Na = 4.0e+01;
const double g_L = 7.5e-02;
const double E_L = -6.0e+01;
const double CM = 1.2e+01;

/* ********************************************************************************************************************** */
// dV/dt

double I_Stim__Nob (int point, double t);
double I_Leak__Nob (double t, double vm, double m, double h, double n);
double g_K2__Nob (double t, double vm, double m, double h, double n);
double g_K1__Nob (double t, double vm, double m, double h, double n);
double I_K__Nob (double t, double vm, double m, double h, double n);
double g_Na__Nob (double t, double vm, double m, double h, double n);
double I_Na__Nob (double t, double vm, double m, double h, double n);
double dvdt__Nob (int type, int point, double t, double vm, double m, double h, double n);
/* ********************************************************************************************************************** */
// dm/dt

double beta_m__Nob (double t, double vm, double m, double h, double n);
double alpha_m__Nob (double t, double vm, double m, double h, double n);
double dmdt__Nob (int type, int point, double t, double vm, double m, double h, double n);
/* ********************************************************************************************************************** */
// dh/dt

double beta_h__Nob (double t, double vm, double m, double h, double n);
double alpha_h__Nob (double t, double vm, double m, double h, double n);
double dhdt__Nob (int type, int point, double t, double vm, double m, double h, double n);
/* ********************************************************************************************************************** */
// dn/dt

double beta_n__Nob (double t, double vm, double m, double h, double n);
double alpha_n__Nob (double t, double vm, double m, double h, double n);
double dndt__Nob (int type, int point, double t, double vm, double m, double h, double n);
/* ********************************************************************************************************************** */

void setInitialConditions__Nob (double *y, int num_eq);
void setFunctions__Nob (Func *f, int num_eq);
void setBCL (double bcl);

#endif
