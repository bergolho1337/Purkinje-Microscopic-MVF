#include "../include/graph.h"

// Structure that stores the graph as an adjacency list
vector< vector< pair< int,double > > > adjList;
// Array storing the coordinates of each node of the graph
vector<Point> nodes;
// Array storing the distance between the source and all the other nodes
vector<double> dist;
// Array storing the terminals of the network
vector<int> term;

void Usage (const char pName[])
{
    cout << "---------------------------------------------------------" << endl;
    cout << "Usage:> " << pName << " <input_MSH>" << endl;
    cout << "<input_MSH> = Input mesh of the Purkinje network" << endl;
    cout << "---------------------------------------------------------" << endl;
}

void buildGraph (const char filename[])
{
    ifstream in(filename);
    if (!in) printError("Cannot open input file !");
    
    int V, E;
    double h;
    in >> E >> V >> h;
    for (int i = 0; i < V; i++)
    {
        double x, y, z;
        in >> x >> y >> z;
        Point p(x,y,z);
        nodes.push_back(p);
    }
    adjList.assign(V,vector< pair<int,double> >());
    for (int i = 0; i < E; i++)
    {
        int o, d;
        double w;
        in >> o >> d;
        w = nodes[o].calcNorm(nodes[d]);
        adjList[o].push_back(make_pair(d,w));
    }
    in.close();
}

double Point::calcNorm (const Point p)
{
    return sqrt( pow(x-p.x,2) + pow(y-p.y,2) + pow(z-p.z,2) );
}

void printGraph ()
{
    for (size_t i = 0; i < adjList.size(); i++)
    {
        cout << "|| " << i << " ||";
        for (size_t j = 0; j < adjList[i].size(); j++)
        {
            pair<int,double> e = adjList[i][j];
            cout << " --> || " << e.first << " " << e.second << " ||"; 
        }
        cout << endl;
    }
}

void printError (const char msg[])
{
    cerr << "[-] ERROR ! " << msg << endl;
    exit(EXIT_FAILURE);
}

void printNodes ()
{
    for (auto p : nodes) cout << p;
    cout << "-------------------------------------------------------------" << endl;
    cout << "[!] There are " << nodes.size() << " nodes !" << endl;
    cout << "-------------------------------------------------------------" << endl;
}

void printDistToTerminals ()
{
    for (size_t i = 0; i < term.size(); i++)
    {
        cout << "Terminal index " << term[i] << " -- Distance from source = " << dist[term[i]] << endl; 
    }
}

void runDijkstra (int s)
{
    size_t V = nodes.size();
    dist.assign(V,INF);
    for (size_t i = 0; i < V; i++) dist[i] = INF;
    dist[s] = 0;
    priority_queue< pair<double,int>, vector< pair<double,int> >, greater< pair<double,int> > > pq;
    pq.push(make_pair(0,s));

    while (!pq.empty())
    {
        pair<double,int> front = pq.top(); pq.pop();
        double d = front.first;
        int u = front.second;
        if (d > dist[u]) continue;
        for (size_t j = 0; j < adjList[u].size(); j++)
        {
            pair<int,double> e = adjList[u][j];
            if (dist[u] + e.second < dist[e.first])
            {
                dist[e.first] = dist[u] + e.second;
                pq.push(make_pair(dist[e.first],e.first));
            }
        }
    }
}

void getTerminals ()
{
    for (size_t i = 0; i < adjList.size(); i++)
    {
        if (adjList[i].size() == 0)
            term.push_back(i);
    }
}