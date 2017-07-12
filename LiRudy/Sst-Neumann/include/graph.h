#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge;
struct Node;

// =============================================================================================================
// =============================================================================================================
// Estrutura de uma aresta no grafo
struct Edge
{
	int id;				// Identificador do no destino
	double w;		    // Tamanho da aresta, distancia euclidiana
	Edge *next;			// Ponteiro para a proxima aresta
	Node *dest;			// Ponteiro para o no destino
}typedef Edge;

// Funcoes de Edge
Edge* newEdge (int id, double w, Node *dest);
// =============================================================================================================
// =============================================================================================================
// Estrutura de nó do grafo
struct Node
{
	int type;				// 0 = Purkinje cell || 1 = PMJ
	int id;					// Identificador do nodo
	double x, y, z;			// Coordenadas (x,y,z)
	int num_edges;			// Contador do número de arestas
	Node *next;				// Ponteiro para o próximo nó na lista de nós
	Edge *edges;			// Ponteiro para a lista de arestas
}typedef Node;
// =============================================================================================================
// Funcoes de Node
Node* newNode (int id, int type, double x, double y, double z);
// =============================================================================================================
// Estrutura do grafo
struct Graph
{
	Node *listNodes;			// Ponteiro para a lista de nós
	Node *lastNode;				// Ponteiro para último nó da lista de nós
	int total_nodes;			// Contador de nós
	int total_edges;			// Contador de arestas
}typedef Graph;

// Funcoes do grafo
void initGraph (Graph **g);
Graph* readPurkinjeNetworkFromFile (char *filename, double &dx);
Node* searchNode (Graph *g, int id);
void insertNodeGraph (Graph *g, int type, double p[]);
void insertEdgeGraph (Graph **g, int id_1, int id_2);
void insertPMJ (Graph *g);
void printGraph (Graph *g);
bool isConnectToPMJ (Edge *ptrl);
void freeGraph (Graph *g);
void freeNodes (Node *listNodes);
void freeEdges (Edge *listEdges);
// =============================================================================================================
// =============================================================================================================
// Funcoes auxiliares
double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2);
void calcPosition (Node *p1, Node *p2, double p[]);
void error (char *msg);
// =============================================================================================================