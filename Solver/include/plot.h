#ifndef PLOT_H_
#define PLOT_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

// Function pointer
typedef double (*Func) (int type, int point, double t, double vm, double m, double h, double n);

void printMatrix (char *str, double *A, int N);
void printVector (char *str, double *b, int N);
void printProgress (int iter, int max_iter);
void printProgress2 (int iter, int max_iter);
void printError (const char *msg);


#endif