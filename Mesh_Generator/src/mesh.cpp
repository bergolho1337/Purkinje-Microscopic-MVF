#include "../include/mesh.h"

Mesh* newMesh (int argc, char *argv[])
{
    printf("--------------------------------------------------------------------------------------------\n");
    printf("[!] Reading VTK file '%s' !\n",argv[1]);
    Mesh *mesh = (Mesh*)malloc(sizeof(Mesh));
    mesh->h = DX;
    Graph *g = readPurkinjeNetworkFromFile(argv[1]);
    //printGraph(g);
    GraphToMEF(mesh,g);
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


// Converte uma estrutura em grafo para uma de Elementos Finitos
// map_graph_elem -> Mapeamento que relaciona os id's do grafo com os id's dos nodos da malha
void GraphToMEF (Mesh *mesh, Graph *g)
{
    int N = g->total_nodes;
    int map_graph_elem[N];
    memset(map_graph_elem,0,sizeof(map_graph_elem));
    int contNodes = 0;
    int contElem = 0;
    Node *ptr = g->listNodes;
    // Construir o nodo 0 primeiro
    Point point; point.x = ptr->x; point.y = ptr->y; point.z = ptr->z;
    mesh->points.push_back(point);
    // Percorrer cada nodo construindo os elementos
    while (ptr != NULL)
    {
        Edge *ptrl = ptr->edges;
        while (ptrl != NULL)
        {
            // Descobre qual o id relacionado do nodo grafo na malha de elementos
            int id_ori = map_graph_elem[ptr->id];
            // So pegar arestas que vao de um nodo menor para um maior
            if (ptrl->dest->id > ptr->id)
            {
                double w = ptrl->w;
                int nElem = w / mesh->h;
                double d_ori[3], d[3];
                calcDirection(d_ori,ptr,ptrl->dest);
                d[0] = ptr->x; d[1] = ptr->y; d[2] = ptr->z;
                // Cresce a quantidade de elementos necessarios de tamanho 'h' para dar o tamanho do segmento
                for (int k = 1; k <= nElem; k++)
                {
                    double x, y, z;
                    x = d[0] + d_ori[0]*mesh->h*k; y = d[1] + d_ori[1]*mesh->h*k; z = d[2] + d_ori[2]*mesh->h*k;
                    Point point; point.x = x; point.y = y; point.z = z;
                    contNodes++;
                    mesh->points.push_back(point);
                    Element elem; elem.left = id_ori; elem.right = contNodes;
                    contElem++;
                    mesh->elements.push_back(elem);
                    id_ori = contNodes;
                }
                // Salva o id de origem do ultimo nodo adicionado, para caso ele gere filhos
                map_graph_elem[ptrl->dest->id] = id_ori;
            }
            ptrl = ptrl->next;
        }
        ptr = ptr->next;
    }
    mesh->nPoints = mesh->points.size();
    mesh->nElem = mesh->elements.size();  
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

// Muda a extensao do arquivo original para .txt
void changeExtension (char *filename)
{
    int N = strlen(filename);
    filename[N-3] = 'm';
    filename[N-2] = 's';
    filename[N-1] = 'h';
    printf("[!] Mesh file will be saved in :> %s\n",filename);
}