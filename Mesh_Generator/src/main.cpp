#include <cstdio>
#include <cstdlib>
#include "../include/mesh.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 4)
    {
        printf("============================================================================================\n");
        printf("Gera uma malha de elementos finitos a partir em um arquivo .vtk com o esqueleto da mesma.\n");
        printf("--------------------------------------------------------------------------------------------\n");
        printf("Usage:> %s <Xmax> <nElem> <in_VTK_file> <out_MSH_file>\n",argv[0]);
        printf("<Xmax> = Tamanho de uma fibra\n");
        printf("<nElem> = Numero de elementos em uma fibra\n");
        printf("<in_VTK_file> = Nome do arquivo de entrada .vtk\n");
        printf("<out_MSH_file> = Nome do arquivo de saida .msh\n");
        printf("Try for example:> %s 2 50 ./Skeleton-Networks/test1.vtk ./test1.msh\n",argv[0]);
        printf("============================================================================================\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("============================================================================================\n");
        Mesh *mesh = newMesh(argc,argv);
        writeMeshToFile(mesh,argv[4]);
        printf("[+] Done\n");
        printf("============================================================================================\n");
        return 0;
    }
}