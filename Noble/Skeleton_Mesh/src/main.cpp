#include <cstdio>
#include <cstdlib>
#include "../include/skeleton.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 4)
    {
        printf("============================================================================================\n");
        printf("Gera o arquivo .vtk que servira como molde para a malha de elementos finitos.\n");
        printf("--------------------------------------------------------------------------------------------\n");
        printf("Usage:> %s <Xmax> <biff> <type> <out_VTK_file>\n",argv[0]);
        printf("<Xmax> = Tamanho de uma fibra\n");
        printf("<biff> = Numero de fibras saindo da bifurcacao\n");
        printf("<type> = Tipo de estrutura a ser gerada.\n");
        printf("\t1 - Fibra com uma bifurcacao no final\n");
        printf("\t2 - Fibra gerada iterativamente\n");
        printf("\t3 - Fibra gerada iterativamente com controle do angulo\n");
        printf("<out_VTK_file> = Nome do arquivo de saida .vtk\n");
        printf("Try for example:> %s 1.0 2 1 ./Networks/simple.vtk (Fibra tipo 1)\n",argv[0]);
        printf("Try for example:> %s 1.0 2 2 ./Networks/simple.vtk (Fibra tipo 2)\n",argv[0]);
        printf("Try for example:> %s 1.0 2 3 ./Networks/simple.vtk (Fibra tipo 3)\n",argv[0]);
        printf("============================================================================================\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("============================================================================================\n");
        Skeleton *skeleton = newSkeleton(argc,argv);
        buildSkeleton(skeleton);
        freeSkeleton(skeleton);
        printf("[+] Done\n");
        printf("============================================================================================\n");
        return 0;
    }
}