#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "../include/graph.h"

using namespace std;

struct Mesh;
struct Point;
struct Element;

// Tamanho de uma celula de Purkinje
const double DX = 0.006666666666666667094565124074;
//const double DX = 0.5;

struct Point
{
    double x, y, z;                 // Coordenadas do ponto
}typedef Point;

struct Element
{
    int left;                       // Identificador do ponto a esquerda do elemento
    int right;                      // Identificador do ponto a direita do elemento
}typedef Element;

struct Mesh
{
    int nElem;                      // Numero de elementos
    int nPoints;                    // Numero de pontos
    double h;                       // Distancia entre dois pontos
    int *map_graph_elem;            // Mapeamento dos pontos (grafo -> elemento)
    vector<Point> points;           // Vetor de pontos
    vector<Element> elements;       // Vetor de elementos
}typedef Mesh;

Mesh* newMesh (int argc, char *argv[]);
void GraphToMesh (Mesh *mesh, Graph *g);
void calcDirection (double d_ori[], Node *n1, Node *n2);
void writeMeshToFile (Mesh *mesh, char *filename);
void writeMeshToVTK (Mesh *mesh, const char *filename);
void writeLevelToFile (Mesh *mesh, Graph *g);
void writeMeshInfo (Mesh *mesh);
void printMeshInfo (Mesh *mesh);
void changeExtension (char *filename);
void DFS (Mesh *mesh, Node *u);
void growSegment (Mesh *mesh, Node *u, Edge *v);