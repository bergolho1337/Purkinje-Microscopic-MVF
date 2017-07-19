/*
========= METODO DOS VOLUMES FINITOS ----- EQUACAO DO MONODOMINIO (CABO) =====================================
  Problema: resolver a equacao do cabo utilizando a equacao do monodominio aplicando o MVF
    { BETA*Cm*V_t = SIGMA*V_xx
    { V'(0,t) = V'(1,t) = 0           Condicao de Neumann homogenea
    { V(x,0) = 0
***********************************************************************************************************
    BETA = Razao entre area superficial por volume da celula (cm^-1)
    Cm = Capacitancia da membrana celular (uF/cm^2)
    SIGMA = Condutividade da membrana celular (mS/cm^2) -- Toda fibra possui o mesmo SIGMA
***********************************************************************************************************
==============================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include "../include/monodomainMVF.h"

int main (int argc, char *argv[])
{
  
  if (argc-1 < 4)
  {
    printf("=============================================================================================\n");
    printf("Usage:> %s <dt> <t_max> <mesh_file> <in_steady_state_file> [ALPHA] [d1]\n",argv[0]);
    printf("---------------------------------------------------------------------------------------------\n");
    printf("<dt> = Tamanho da discretizacao no tempo\n");
    printf("<t_max> = Tempo maximo de simulacao\n");
    printf("<mesh_file> = Arquivo contendo a malha com os pontos definindo os volumes e as suas conexoes\n");
    printf("<in_steady_state_file> = Arquivo de entrada com a solucao estacionaria\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf(" !! Parametros opcionais !! (Analise de sensibilidade)\n");
    printf("[ALPHA] = Rpmj*Vpmj\n");
    printf("[d1] = Diametro da celula de Purkinje\n");
    printf("[!] DEBUGACAO EH ATIVADA POR FLAG NO ARQUIVO \"monodomainMVF.h\".\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("Exemplo: %s 0.1 1000 cable.msh steadystate.sst\n",argv[0]);
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
