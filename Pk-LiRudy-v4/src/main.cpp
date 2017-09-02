/*
========= METODO DOS VOLUMES FINITOS ----- EQUACAO DO MONODOMINIO (CABO) =====================================
  Problema: resolver a equacao do cabo utilizando a equacao do monodominio aplicando o MVF
    { BETA*Cm*V_t = SIGMA*V_xx
    { V'(0,t) = V'(1,t) = I_PMJ           Condicao de Neumann nao-homogenea nos PMJ
    { V(x,0) = V_inf
***********************************************************************************************************
    BETA = Razao entre area superficial por volume da celula (cm^-1)
    Cm = Capacitancia da membrana celular (uF/cm^2)
    SIGMA = Condutividade da membrana celular (mS/cm^2) -- Toda fibra possui o mesmo SIGMA
***********************************************************************************************************
==============================================================================================================
*/

#include <cstdio>
#include "../include/timer.h"
#include "../include/model.h"

using namespace std;

int main (int argc, char *argv[])
{
  
  if (argc-1 < 5)
  {
    Usage(argv[0]);
    return 1;
  }
  else
  {
    double start, finish, elapsed;

    GET_TIME(start);
    solveModel(argc,argv);
    GET_TIME(finish);
    elapsed = finish - start;
  
    printf("==========================================================\n");
    printf("[!] Time elapsed = %.10lf seconds\n",elapsed);
    printf("==========================================================\n");

    return 0;
  }
}
