// Codigo que retorna os pontos de plot para a velocidade de propagacao
// Este codigo so funciona para malhas de bifurcacao do Experimento 2 (hardcode os pontos do meio)

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

void Usage (char pName[])
{
    cout << "Usage:> " << pName << " <size_fiber> <input_mesh> <output_plot>" << endl;
}

void readMesh (const string input, const double sf, int &N, vector< vector<int> > &g)
{
    ifstream in(input);
    int ne, np;
    double sc;

    in >> ne >> np >> sc;
    N = sf / sc;

    // Build the graph
    g.assign(np,vector<int>());
    for (int i = 0; i < np; i++)
    {
        double x, y, z;
        in >> x >> y >> z;
    }
    for (int i = 0; i < ne; i++)
    {
        int s, d;
        in >> s >> d;
        g[s].push_back(d);
    }
}

void getInfoGraph (const vector< vector<int> > g, vector<int> &biff, vector<int> &term)
{
    for (size_t i = 0; i < g.size(); i++)
    {
        if (g[i].size() == 0)
            term.push_back(i);
        else if (g[i].size() == 2)
            biff.push_back(i);
    }
}

void writePlotPoints (const string output, const vector<int> biff, const vector<int> term, const int N)
{
    ofstream out(output);
    out << 14 << endl;
    out << N/2 << " " << 3*N/2 << " " << 5*N/2 << " " << 7*N/2 << " " << 9*N/2 << " "\
        << 11*N/2 << " " << 13*N/2 << " ";
    for (auto id : biff)
        out << id << " ";
    for (auto id : term)
        out << id << " ";
    out << endl;
}

int main (int argc, char *argv[])
{
    if (argc-1 < 3)
    {
        Usage(argv[0]);
        exit (EXIT_FAILURE);
    }
    vector< vector<int> > g;
    vector<int> biff, term;
    int N;
    double sf = atof(argv[1]);
    string input(argv[2]);
    string output(argv[3]);

    readMesh(input,sf,N,g);
    getInfoGraph(g,biff,term);
    writePlotPoints(output,biff,term,N);
    
    return 0;
}


/*
Cable 3L
    out << 6 << endl;
    out << N/2 << " " << N << " " << 3*N/2 << " " << 2*N << " " << 5*N/2 << " " << 3*N << endl
*/