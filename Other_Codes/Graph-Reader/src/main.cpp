// ----------------------------------------------------------------------------------------
// Program that reads a mesh on format ".msh" and convert it to a graph structure
// There are additional functions to display more information about the network
// ----------------------------------------------------------------------------------------

#include <iostream>
#include <cstdio>
#include "../include/graph.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 != 1)
    {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    buildGraph(argv[1]);
    runDijkstra(0);
    getTerminals();
    printDistToTerminals();
    //printGraph();

    return 0;
}