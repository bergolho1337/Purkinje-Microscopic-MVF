#include "../include/mesh.h"

Mesh* newMesh (int argc, char *argv[])
{
    printf("--------------------------------------------------------------------------------------------\n");
    printf("[!] Reading VTK file '%s' !\n",argv[1]);
    Mesh *mesh = (Mesh*)malloc(sizeof(Mesh));
    mesh->h = DX;
    Graph *g = readPurkinjeNetworkFromFile(argv[1]);
    GraphToMesh(mesh,g);
    //printGraph(g);
    
    #ifdef DEBUG
    writeMeshInfo(mesh);
    #endif
    
    printf("--------------------------------------------------------------------------------------------\n");
    return mesh;
}

// Calcula o vetor direcao indo vertice 1 -> 2, e retorna o unitario
void calcDirection (double d_ori[], Node *n1, Node *n2)
{
    double norm;
    d_ori[0] = n2->x - n1->x; d_ori[1] = n2->y - n1->y; d_ori[2] = n2->z - n1->z;
    norm = sqrt(pow(d_ori[0],2)+pow(d_ori[1],2)+pow(d_ori[2],2));
    for (int i = 0; i < 3; i++) d_ori[i] /= norm;
}

// Constroi uma malha de elementos finitos partir de um grafo gerado a partir de um .vtk
// map_graph_elem -> Mapeamento que relaciona os id's do grafo com os id's dos nodos da malha
void GraphToMesh (Mesh *mesh, Graph *g)
{
    int N = g->total_nodes;
    mesh->map_graph_elem = (int*)calloc(N,sizeof(int));
    // Construir o nodo 0 primeiro
    Node *ptr = g->listNodes;
    Point point; point.x = ptr->x; point.y = ptr->y; point.z = ptr->z;
    mesh->points.push_back(point);
    // Busca em profundidade
    DFS(mesh,ptr);
}

// Busca em profundidade
void DFS (Mesh *mesh, Node *u)
{
    Edge *v = u->edges;
    while (v != NULL)
    {
        growSegment(mesh,u,v);
        DFS(mesh,v->dest);
        v = v->next;
    }
    mesh->nPoints = mesh->points.size();
    mesh->nElem = mesh->elements.size();
}

void growSegment (Mesh *mesh, Node *u, Edge *v)
{
    double d_ori[3], d[3];
    double w = v->w;
    int nElem = w / mesh->h;
    int id_source = mesh->map_graph_elem[u->id];
    calcDirection(d_ori,u,v->dest);
    d[0] = u->x; d[1] = u->y; d[2] = u->z;
    printf("Node %d will grow %d points\n",u->id,nElem);
    // Cresce a quantidade de elementos necessarios de tamanho 'h' para dar o tamanho do segmento
    for (int k = 1; k <= nElem; k++)
    {
        double x, y, z;
        x = d[0] + d_ori[0]*mesh->h*k; y = d[1] + d_ori[1]*mesh->h*k; z = d[2] + d_ori[2]*mesh->h*k;
        Point point; point.x = x; point.y = y; point.z = z;
        mesh->points.push_back(point);
        Element elem; elem.left = id_source; elem.right = mesh->points.size()-1;
        mesh->elements.push_back(elem);
        id_source = mesh->points.size()-1;
    }
    // Salva o id de origem do ultimo nodo adicionado, para caso ele gere filhos
    mesh->map_graph_elem[v->id] = id_source;
}   

// Imprime informacoes sobre a malha em um arquivo
void writeMeshInfo (Mesh *mesh)
{
    FILE *outFile = fopen("infoMesh.txt","w+");
    fprintf(outFile,"---------------------------- MESH INFO --------------------------------------------\n");
    fprintf(outFile,"Number of points = %d\n",mesh->nPoints);
    fprintf(outFile,"Number of elements = %d\n",mesh->nElem);
    fprintf(outFile,"h = %lf\n\n",mesh->h);
    fprintf(outFile,"-> POINTS\n");
    for (int i = 0; i < (int)mesh->points.size(); i++)
        fprintf(outFile,"Point %d = (%lf,%lf,%lf)\n",i,mesh->points[i].x,mesh->points[i].y,mesh->points[i].z);
    fprintf(outFile,"-> ELEMENTS\n");
    for (int i = 0; i < (int)mesh->elements.size(); i++)
        fprintf(outFile,"Element %d = (%d,%d)\n",i,mesh->elements[i].left,mesh->elements[i].right);
    fprintf(outFile,"-----------------------------------------------------------------------------------\n");
    fclose(outFile);
}

// Imprime informacoes sobre a malha
void printMeshInfo (Mesh *mesh)
{
    printf("---------------------------- MESH INFO --------------------------------------------\n");
    printf("Number of points = %d\n",mesh->nPoints);
    printf("Number of elements = %d\n",mesh->nElem);
    printf("h = %lf\n\n",mesh->h);
    printf("-> POINTS\n");
    for (int i = 0; i < (int)mesh->points.size(); i++)
        printf("Point %d = (%lf,%lf,%lf)\n",i,mesh->points[i].x,mesh->points[i].y,mesh->points[i].z);
    printf("-> ELEMENTS\n");
    for (int i = 0; i < (int)mesh->elements.size(); i++)
        printf("Element %d = (%d,%d)\n",i,mesh->elements[i].left,mesh->elements[i].right);
    printf("-----------------------------------------------------------------------------------\n");
}

// Escrever a malha em um arquivo
void writeMeshToFile (Mesh *mesh, char *filename)
{
    if (mesh != NULL)
    {
        //changeExtension(filename);
        printf("[!] Mesh file will be saved in :> %s\n",filename);
        FILE *file = fopen(filename,"w+");
        if (!file) printf("[-] ERROR! Opening file '%s'\n",filename);
        fprintf(file,"%d %d %lf\n",mesh->nElem,mesh->nPoints,mesh->h);
        for (int i = 0; i < mesh->nPoints; i++)
            fprintf(file,"%lf %lf %lf\n",mesh->points[i].x,mesh->points[i].y,mesh->points[i].z);
        for (int i = 0; i < mesh->nElem; i++)
            fprintf(file,"%d %d\n",mesh->elements[i].left,mesh->elements[i].right);
        fclose(file);
    }   
}

// Escrever a malha em um arquivo .vtk
void writeMeshToVTK (Mesh *mesh, const char *filename)
{
    if (mesh != NULL)
    {
        //changeExtension(filename);
        printf("[!] Mesh file will be saved in :> %s\n",filename);
        FILE *outFile = fopen(filename,"w+");
        fprintf(outFile,"# vtk DataFile Version 3.0\n");
        fprintf(outFile,"Purkinje\nASCII\nDATASET POLYDATA\n");
        fprintf(outFile,"POINTS %d float\n",(int)mesh->points.size());
        for (int i = 0; i < (int)mesh->points.size(); i++)
            fprintf(outFile,"%.8lf %.8lf %.8lf\n",mesh->points[i].x,mesh->points[i].y,mesh->points[i].z);
        fprintf(outFile,"LINES %d %d\n",(int)mesh->elements.size(),(int)mesh->elements.size()*3);
        for (int i = 0; i < (int)mesh->elements.size(); i++)
            fprintf(outFile,"2 %d %d\n",mesh->elements[i].left,mesh->elements[i].right);
        fclose(outFile);
    }   
}

// Muda a extensao do arquivo original para .txt
void changeExtension (char *filename)
{
    int N = strlen(filename);
    filename[N-3] = 'm';
    filename[N-2] = 's';
    filename[N-1] = 'h';
    printf("[!] Mesh file will be saved in :> %s\n",filename);
}