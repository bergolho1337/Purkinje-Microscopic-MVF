#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

void Usage (const char pName[])
{
    cout << "=================================" << endl;
    cout << "Usage:> " << pName << " <mesh_file>" << endl;
    cout << "=================================" << endl;
}

void buildGraph (vector< vector<int> > &adjList, const char mshName[])
{
    ifstream in(mshName);
    int nE, nP;
    double dx;
    in >> nE >> nP >> dx;
    adjList.assign(nP,vector<int>());
    for (int i = 0; i < nP; i++)
    {
        double x, y, z;
        in >> x >> y >> z;
    }
    for (int i = 0; i < nE; i++)
    {
        int s, d;
        in >> s >> d;
        adjList[s].push_back(d);
    }
}

void printGraph (const vector< vector<int> > adjList)
{
    for (int i = 0; i < (int)adjList.size(); i++)
    {
        cout << "|| " << i << " ||";
        for (int j = 0; j < (int)adjList[i].size(); j++)
            cout << " --> || " << adjList[i][j] << " ||";
        cout << endl;
    }
    cout << endl;
}

void getInfoGraph (const vector< vector<int> > adjList)
{
    for (int i = 0; i < (int)adjList.size(); i++)
    {
        if (adjList[i].size() == 0)
            cout << "Terminal point at " << i << endl;
        else if (adjList[i].size() == 2)
            cout << "Bifurcation point at " << i << endl;
    }
    cout << "Total number of points = " << (int)adjList.size() << endl;
    cout << endl;
}

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    vector< vector<int> > adjList;
    buildGraph(adjList,argv[1]);
    getInfoGraph(adjList);
    //printGraph(adjList);

    return 0;
}
