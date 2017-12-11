#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
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
const double PIG_DX = 0.0068;	                      // T. Stankovičová, 2003 (cm) -- Pig Purkinje cell
const double DOG_DX = 0.0164;                         // Michael F. Sheets (1983)   -- Dog Purkinje cell
const double ALIEN_DX = 0.01;                         // Teste -- Alien Purkinje cell
const double ORC_DX = 0.02;				              // Teste 2 -- Orc Purinje cell

const double TEST1_DX = 0.0075;
const double TEST2_DX = 0.0100;
const double TEST3_DX = 0.0125;
const double TEST4_DX = 0.0150;
const double TEST5_DX = 0.0175;
const double TEST6_DX = 0.0200;

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
double setTypeCell (char cName[]);
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
