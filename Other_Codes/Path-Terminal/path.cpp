#include <bits/stdc++.h>

using namespace std;

class Node
{
public:
    double x, y, z;                     // Coordenates of the point (x,y,z)
    vector< pair<int,double> > edges;   // Vector of edges 'pair(to,weight)'
    Node () {};
};

vector<Node> adjList;
vector<bool> visit;

void Usage (const char pname[])
{
    cout << "Usage:> " << pname << " <mesh_file>" << endl;
}

double calcDist (int i, int j)
{
    return     sqrt( pow(adjList[i].x - adjList[j].x,2) + \
                     pow(adjList[i].y - adjList[j].y,2) + \
                     pow(adjList[i].z - adjList[j].z,2) );
}

void buildGraph (const char name[])
{
    ifstream in(name);
    double dx;
    int np, ne;
    in >> ne >> np >> dx;
    visit.assign(np,false);
    // Read points
    for (int i = 0; i < np; i++)
    {
        Node node;
        in >> node.x >> node.y >> node.z;
        adjList.push_back(node);
    }
    for (int i = 0; i < ne; i++)
    {
        int o, d;
        in >> o >> d;
        double w = calcDist(o,d);
        adjList[o].edges.push_back(make_pair(d,w));
    }
}

void printGraph ()
{
    for (size_t i = 0; i < adjList.size(); i++)
    {
        cout << "|| (" << adjList[i].x << "," << adjList[i].y << "," << adjList[i].z << ") ||";
        for (size_t j = 0; j < adjList[i].edges.size(); j++)
            cout << " --> || (" << adjList[i].edges[j].first << "," << adjList[i].edges[j].second << ")";
        cout << endl;
    }
    cout << endl;
}

void DFS (int u, double w, int b)
{
    // Found a bifurcation
    if (adjList[u].edges.size() == 2)
        b++;
    // Found a terminal
    if (adjList[u].edges.size() == 0)
    {
        // Write the segment size from the source to this specific terminal
        // On the format: <segment_size> <number_bifurcations_founded>
        cout << w << " " << b << endl; 
    }

    visit[u] = true;
    for (size_t j = 0; j < adjList[u].edges.size(); j++)
    {
        int v = adjList[u].edges[j].first;
        double weight = adjList[u].edges[j].second;
        if (!visit[v])
            DFS(v,w+weight,b);
    }

}

int main (int argc, char *argv[])
{
    if (argc-1 != 1)
    {
        Usage(argv[0]);
        exit (EXIT_FAILURE);
    }
    buildGraph(argv[1]);
    //printGraph();
    DFS(0,0,0);  

    return 0;
}