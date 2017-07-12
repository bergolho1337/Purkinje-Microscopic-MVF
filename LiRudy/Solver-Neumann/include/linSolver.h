#ifndef LINSOLVER_H_
#define LINSOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/plot.h"

#define ITER_MAX 1000				// Numero maximo de iteracoes
#define EPSILON 1.0e-08				// Tolerancia para a resposta do sistema linear

double dotProduct (double *a, double *b, int n);
void saxpy (double *x, double *y, double alpha, int n);
void saxpy2 (double *x, double *y, double alpha, int n);
void matvec (double *A, double *b, double *x, int n);
double multiply_row (int row_size, int *Aj, double *Av, double *b);
double norm (double *v, int n);

// Resolve um sistema linear usando o Metodo Gradiente Conjugado
void solveLinearSystem_CG (double *A, double *b, double *x, int N);

// Resolve um sistema linear usando Decomposicao LU
void solveLinearSystem_LU (double *A, double *b, double *x, int N);

// Decompor uma matriz A em LU
void LUDecomposition (double *A, int N);

// Checar se o sistema foi resolvido corretamente.
double checkSystem (double *A, double *b, double *x, int N);

#endif