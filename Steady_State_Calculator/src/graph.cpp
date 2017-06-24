#include "../include/graph.h"                            

// Inicializa os atributos do grafo
void initGraph (Graph **g)
{
    (*g)->total_nodes = 0;
    (*g)->total_edges = 0;
    (*g)->listNodes = NULL;
    (*g)->lastNode = NULL;
}

// Le um arquivo .msh contendo a malha da rede de Purkinje e transforma em um grafo
Graph* readPurkinjeNetworkFromFile (char *filename, double &dx)
{
    int V, E;
    FILE *inFile = fopen(filename,"r");
    if (inFile == NULL) error("Cannot open MSH file!");
    Graph *g = (Graph*)malloc(sizeof(Graph));

    initGraph(&g);
    // Ler a quantidade de vertices e arestas
    if (!fscanf(inFile,"%d %d %lf",&E,&V,&dx)) error("Reading file");
    // Ler os vertices
    for (int i = 0; i < V; i++)
    {
        double p[3];
        if (!fscanf(inFile,"%lf %lf %lf",&p[0],&p[1],&p[2])) error("Reading file");
        insertNodeGraph(g,0,p);
    }
    // Ler as arestas
    for (int i = 0; i < E; i++)
    {
        int e[2];
        if (!fscanf(inFile,"%d %d",&e[0],&e[1])) error("Reading file");
        insertEdgeGraph(&g,e[0],e[1]);
        insertEdgeGraph(&g,e[1],e[0]);
    }
    fclose(inFile);
    return g;
}

// Construtor de um nodo
Node* newNode (int id, int type, double x, double y, double z)
{
    Node *node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->id = id;
    node->x = x;
    node->y = y;
    node->z = z;
    node->num_edges = 0;
    node->next = NULL;
    node->edges = NULL;
    return node;
}

// Cosntrutor de uma aresta
Edge* newEdge (int id, double w, Node *dest)
{
	Edge *edge = (Edge*)malloc(sizeof(Edge));
	edge->id = id;
	edge->w = w;
	edge->dest = dest;
	edge->next = NULL;
	return edge;
}


// Insere um nodo com coordenadas (px,py,pz) no grafo
void insertNodeGraph (Graph *g, int type, double p[])
{
    Node *ptr = g->listNodes;
    Node *ptrNew = newNode(g->total_nodes++,type,p[0],p[1],p[2]);
    // Primeiro nodo do grafo
    if (ptr == NULL)
        g->listNodes = ptrNew;
    // Percorrer a lista de nodos
    else
    {
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = ptrNew;
    }
}

// Insere uma aresta no grafo
void insertEdgeGraph (Graph **g, int id_1, int id_2)
{
	Node *ptr1, *ptr2;
	Edge *edge;
	double norm;
	// Checar se a aresta eh invalida
	if (id_1 == id_2) return;

	ptr1 = searchNode(*g,id_1);
	ptr2 = searchNode(*g,id_2);
	
    norm = calcNorm(ptr1->x,ptr1->y,ptr1->z,ptr2->x,ptr2->y,ptr2->z);
    edge = newEdge(id_2,norm,ptr2);
    // Primeira aresta
    if (ptr1->edges == NULL)
        ptr1->edges = edge;
    // Percorrer ate o final 
    else
    {
        Edge *ptrl = ptr1->edges;
        while (ptrl->next != NULL)
            ptrl = ptrl->next;
        ptrl->next = edge;
    }
    // Incrementar o contador de arestas do Node origem
    ptr1->num_edges++;
    // Incrementar o numero total de arestas no grafo
    (*g)->total_edges++;
}

// Busca por um Node no grafo
Node* searchNode (Graph *g, int id)
{
	Node *ptr = g->listNodes;
	while (ptr != NULL)
	{
		if (ptr->id == id)
			return ptr;
		ptr = ptr->next;
	}
    error("Node not found!");
    return NULL;
}


void error (char *msg)
{
    printf("[-] ERROR! %s\n",msg);
    exit(-1);   
}

// Calcula a norma euclidiana entre dois pontos
double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt(pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2));
}

// Calcula a posicao do novo Node
void calcPosition (Node *p1, Node *p2, double p[])
{
    double size = 2.00;
    double norm;
    norm = calcNorm(p1->x,p1->y,p1->z,p2->x,p2->y,p2->z);
    p[0] = (p1->x - p2->x)/norm; p[1] = (p1->y - p2->y)/norm; p[2] = (p1->z - p2->z)/norm;
    p[0] = p1->x + size*p[0]; p[1] = p1->y + size*p[1]; p[2] = p1->z + size*p[2];
}

// Insere um volume de PMJ para cada folha da arvore 
void insertPMJ (Graph *g)
{
    double p[3];
    Node *ptr = g->listNodes;
    while (ptr != NULL)
    {
        // Node eh uma folha, nao eh a raiz e eh do tipo Purkinje cell
        if (ptr->type == 0 && ptr->num_edges == 1 && ptr->id != 0)
        {
            calcPosition(ptr,ptr->edges->dest,p);
            insertNodeGraph(g,1,p);
            insertEdgeGraph(&g,ptr->id,g->total_nodes-1);
            insertEdgeGraph(&g,g->total_nodes-1,ptr->id);
        }
        ptr = ptr->next;
    }
}

/* Verifica se um Node esta conectado a algum PMJ */
bool isConnectToPMJ (Edge *ptrl)
{
    while (ptrl != NULL)
    {
        if (ptrl->dest->type == 1) return true;
        ptrl = ptrl->next;
    }
    return false;
}

void printGraph (Graph *g)
{
	Node *ptr;
	Edge *ptrl;
	ptr = g->listNodes;
	printf("======================= PRINTING GRAPH ================================\n");
	while (ptr != NULL)
	{
	    printf("|| %d (%d) (%.4lf %.4lf %.4lf) %d ||",ptr->id,ptr->type,ptr->x,ptr->y,ptr->z,ptr->num_edges);
		ptrl = ptr->edges;
		while (ptrl != NULL)
		{
			printf(" --> || %d %.4lf (%.4lf %.4lf %.4lf) ||",ptrl->id,ptrl->w,ptrl->dest->x,ptrl->dest->y, \
					ptrl->dest->z);
			ptrl = ptrl->next;
		}
		printf("\n");
		ptr = ptr->next;
	}
	printf("=======================================================================\n");
}
