#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Point
{
    double x, y, z;
}typedef Point;

struct Element
{
    int to, from;
}typedef Element;

int main ()
{
    int nP, nE, aux;
    int to, from;
    string str;
    ifstream in("CO-Tree.vtk");
    for (int i = 0; i < 11; i++)
        in >> str;
    
    in >> nP >> str;
    Point p[nP];
    for (int i = 0; i < nP; i++)
    {
        in >> p[i].x >> p[i].y >> p[i].z;
        p[i].x /= 10.0; p[i].y /= 10.0; p[i].z /= 10.0;
    }
    in >> str >> nE >> aux;
    Element e[nE];
    for (int i = 0; i < nE; i++)
    {
        in >> str >> e[i].to >> e[i].from;
    }
    ofstream out("COTree-2.vtk");
    out << "# vtk DataFile Version 3.0\nvtk output\nASCII\nDATASET POLYDATA" << endl;
    out << "POINTS " << nP << " float" << endl;
    for (int i = 0; i < nP; i++)
        out << p[i].x << " " << p[i].y << " " << p[i].z << endl;
    out << "LINES " << nE << " " << nE*3 << endl;
    for (int i = 0; i < nE; i++)
        out << "2 " << e[i].to << " " << e[i].from << endl;
    
}
