#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>

using namespace std;

const double INF = __DBL_MAX__;

class Edge;
class Node;

// =============================================================================================================
// =============================================================================================================
// Estrutura de uma aresta no grafo
class Edge
{
public:
    Edge (int id, double w, Node *dest);

public:
	int id;				// Identificador do no destino
	double w;		    // Tamanho da aresta, distancia euclidiana
	Edge *next;			// Ponteiro para a proxima aresta
	Node *dest;			// Ponteiro para o no destino
};

// =============================================================================================================
// =============================================================================================================
// Estrutura de nó do grafo
class Node
{
public:
    Node (int id, int type, double x, double y, double z);

public:
	int type;				// 0 = Purkinje cell || 1 = PMJ
	int id;					// Identificador do nodo
	double x, y, z;			// Coordenadas (x,y,z)
	int num_edges;			// Contador do número de arestas
	Node *next;				// Ponteiro para o próximo nó na lista de nós
	Edge *edges;			// Ponteiro para a lista de arestas
};
// =============================================================================================================
// =============================================================================================================
// Estrutura do grafo
class Graph
{
public:
    Graph (string filename, double &dx);
    void print ();
	void printterm ();
    void error (const char msg[]);
	void dijkstra (int s);
	// Inline
	int getTotalNodes () { return total_nodes; }
	int getTotalEdges () { return total_edges; }
	Node* getListNodes () { return listNodes; }
	double* getDist () { return dist; }
	int* getTerm () { return term; }
	int getNTerm() { return nterm; }
private:
	Node *listNodes;			// Ponteiro para a lista de nós
	Node *lastNode;				// Ponteiro para último nó da lista de nós
	int total_nodes;			// Contador de nós
	int total_edges;			// Contador de arestas
	int nterm;
	double *dist;				// Distance from the source node to all the others
	int *term;

    void initGraph ();
    void insertNodeGraph (int type, double p[]);
	void insertEdgeGraph (int id_1, int id_2);
	Node* searchNode (int id);
	void insertPMJ ();
	void calcPosition (Node *p1, Node *p2, double p[]);
	void setTerm ();

};

// Funcoes do grafo
/*
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
*/
// =============================================================================================================
// =============================================================================================================
// Funcoes auxiliares
double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2);
void calcPosition (Node *p1, Node *p2, double p[]);
// =============================================================================================================

#endif