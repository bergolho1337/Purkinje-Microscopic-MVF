#include <cstdio>
#include <cstdlib>
#include "../include/skeleton.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 3)
    {
        printf("============================================================================================\n");
        printf("Gera o arquivo .vtk que servira como molde para a malha de elementos finitos.\n");
        printf("--------------------------------------------------------------------------------------------\n");
        printf("Usage:> %s <Xmax> <biff> <out_VTK_file>\n",argv[0]);
        printf("<Xmax> = Tamanho de uma fibra\n");
        printf("<biff> = Numero de fibras saindo da bifurcacao\n");
        printf("<out_VTK_file> = Nome do arquivo de saida .vtk\n");
        printf("Try for example:> %s 2.0 1 ./Networks/simple.vtk\n",argv[0]);
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