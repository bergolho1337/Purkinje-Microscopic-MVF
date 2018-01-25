#include <cstdio>
#include <cstdlib>
#include "../include/skeleton.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 4)
    {
        printf("============================================================================================\n");
        printf("Generate a .vtk file that will be the reference to build the mesh of the network.\n");
        printf("--------------------------------------------------------------------------------------------\n");
        printf("Usage:> %s <Xmax> <biff> <type> <out_VTK_file>\n",argv[0]);
        printf("<Xmax> = Size of the fiber\n");
        printf("<biff> = Number of fiber leaving a bifurcation\n");
        printf("<type> = Type of network.\n");
        printf("\t1 - Fiber with a bifurcation at the end\n");
        printf("\t2 - Fiber iteratively generated\n");
        printf("\t3 - Fiber iteratively generated with angle control\n");
        printf("<out_VTK_file> = Name of the output .vtk file\n");
        printf("Try for example:> %s 1.0 2 1 ./Networks/simple.vtk (Type 1)\n",argv[0]);
        printf("Try for example:> %s 1.0 2 2 ./Networks/simple.vtk (Type 2)\n",argv[0]);
        printf("Try for example:> %s 1.0 2 3 ./Networks/simple.vtk (Type 3)\n",argv[0]);
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