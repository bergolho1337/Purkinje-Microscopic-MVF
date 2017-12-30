#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

const double INF = 1.0e+30;

class Point
{
private:
    double x, y, z;
public:
    Point (double x, double y, double z) : x(x), y(y), z(z) {};
    double calcNorm (const Point p);
    friend ostream& operator<< (ostream& os, const Point &p)  
    {  
        os << "(" << p.x << "," << p.y << "," << p.z << ")" << endl;  
        return os;  
    }
};

void Usage (const char pName[]);
void buildGraph (const char filename[]);
void getTerminals ();
void runDijkstra (int s);
double calcNorm (const Point p1, const Point p2);
void printGraph ();
void printError (const char msg[]);
void printNodes ();
void printDistToTerminals ();