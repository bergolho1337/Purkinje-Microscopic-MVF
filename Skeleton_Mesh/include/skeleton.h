#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

struct Point
{
    double x, y, z;
    Point(double X, double Y, double Z) { x = X; y = Y; z = Z; }
}typedef Point;

struct Element
{
    int left;
    int right;
    Element(int l, int r) { left = l; right = r; }
}typedef Element;

struct Skeleton
{
    int type;                   // Tipo de estrutura
    double fib_size;            // Tamanho da fibra
    vector<Point> points;       // Vetor de pontos
    vector<Element> elements;   // Vetor de elementos
    char filename[200];         // Nome do arquivo de saida
}typedef Skeleton;

Skeleton* newSkeleton (int argc, char *argv[]);
void freeSkeleton (Skeleton *sk);
void buildSkeleton (Skeleton *sk);
void buildSkeleton_K (Skeleton *sk);
void writeSkeletonToFile (Skeleton *sk);
void calcOriginalDirection (Point p1, Point p2, double d_ori[]);
void rotate (double d_ori[], double d_rot[], double teta);