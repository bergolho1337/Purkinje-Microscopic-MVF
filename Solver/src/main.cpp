/*
******************************************
    Autor: Lucas Berg
    Last Update: 09/05/17
******************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include "../include/monodomainMVF.h"

int main (int argc, char *argv[])
{
  /*
  printf("========= METODO DOS VOLUMES FINITOS ----- EQUACAO DO MONODOMINIO (CABO) ===== v.2.0 ========\n");
  printf("Problema: resolver a equacao do cabo utilizando a equacao do monodominio aplicando o MVF.\n");
  printf("\t{ BETA*Cm*V_t = SIGMA*V_xx\n");
  printf("\t{ V'(0,t) = V'(1,t) = 0\n");
  printf("\t{ V(x,0) = 0\n");
  printf("***********************************************************************************************\n");
  printf("BETA = Razao entre area superficial por volume da celula (cm^-1)\n");
  printf("Cm = Capacitancia da membrana celular (uF/cm^2)\n");
  printf("SIGMA = Condutividade da membrana celular (mS/cm^2) -- Toda fibra possui o mesmo SIGMA\n");
  printf("***********************************************************************************************\n");
  */
  if (argc-1 < 4)
  {
    printf("=============================================================================================\n");
    printf("Usage:> %s <dt> <t_max> <mesh_file> <steady_state_file> [lu_file]\n",argv[0]);
    printf("---------------------------------------------------------------------------------------------\n");
    printf("<dt> = Tamanho da discretizacao no tempo\n");
    printf("<t_max> = Tempo maximo de simulacao\n");
    printf("<mesh_file> = Arquivo contendo a malha com os pontos definindo os volumes e as suas conexoes\n");
    printf("<steady_state_file> = Arquivo contendo a solucao do estado estacionario dos pontos da malha\n");
    printf("<lu_file> = Arquivo contendo a decomposicao LU jah calculada para a malha\n");
    printf("[!] DEBUGACAO EH ATIVADA POR FLAG NO ARQUIVO \"monodomainMVF.h\".\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("Exemplo: %s 0.1 1000 Malhas/0.50/E_75/test1.msh SteadyState/e-75/steadystate1.dat\n",argv[0]);
    printf("=============================================================================================\n");
    return 1;
  }
  else
  {
    double start, finish, elapsed;

    MonodomainMVF *monoMVF = newMonodomainMVF(argc,argv);
    start = clock();
    solveMonodomain(monoMVF);
    finish = clock();
    elapsed = (double)(finish-start) / (double)CLOCKS_PER_SEC;

    freeMonodomain(monoMVF);
    printf("==========================================================\n");
    printf("[!] Time elapsed = %.10lf\n",elapsed);
    printf("==========================================================\n");

    return 0;
  }
}
