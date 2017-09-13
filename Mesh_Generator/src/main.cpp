#include <cstdio>
#include <cstdlib>
#include "../include/mesh.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 3)
    {
        printf("============================================================================================\n");
        printf("Gera uma malha de elementos finitos a partir em um arquivo .vtk com o esqueleto da mesma.\n");
        printf("--------------------------------------------------------------------------------------------\n");
        printf("Usage:> %s <in_VTK_file> <out_MSH_file> --<type_cell>\n",argv[0]);
        printf("<in_VTK_file> = Nome do arquivo de entrada .vtk\n");
        printf("<out_MSH_file> = Nome do arquivo de saida .msh\n");
        printf("<type_cell> = alien/dog/orc/pig\n");
        printf("Try for example:> %s ./Skeleton-Networks/test1.vtk ./test1.msh --alien\n",argv[0]);
        printf("============================================================================================\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("============================================================================================\n");
        Mesh *mesh = newMesh(argc,argv);
        writeMeshToFile(mesh,argv[2]);
        writeMeshToVTK(mesh,"mesh.vtk");
        printf("[+] Done\n");
        printf("============================================================================================\n");
        return 0;
    }
}