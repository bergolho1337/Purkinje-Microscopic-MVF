#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include "../include/graph.h"

using namespace std;

struct Mesh;
struct Point;
struct Element;

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
    double xMax;                    // Tamanho da fibra
    double h;                       // Distancia entre dois pontos
    vector<Point> points;           // Vetor de pontos
    vector<Element> elements;       // Vetor de elementos
}typedef Mesh;

Mesh* newMesh (int argc, char *argv[]);
void GraphToMEF (Mesh *mesh, Graph *g);
void calcDirection (double d_ori[], Node *n1, Node *n2);
void writeMeshToFile (Mesh *mesh, char *filename);
void writeMeshInfo (Mesh *mesh);
void printMeshInfo (Mesh *mesh);
void changeExtension (char *filename);