#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

// Maximum number of growing iterations
const int MAX_ITER = 2;

// Size of the bifurcation angle
const double ANG = M_PI / 3.0;

struct Point
{
    int id;
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
    int type;                   // Type of network
    int biff;                   // Number of fiber leaving a bifurcation
    double fib_size;            // Size of a fiber
    vector<Point> points;       // Vector with the points of the network
    vector<Element> elements;   // Vector with the links between the points
    char filename[200];         // Output filename
}typedef Skeleton;

Skeleton* newSkeleton (int argc, char *argv[]);
Point* newPoint (double x, double y, double z);
void freeSkeleton (Skeleton *sk);
void buildSkeleton (Skeleton *sk);
void buildSkeleton_K (Skeleton *sk);
void buildSkeleton_K_Iter (Skeleton *sk);
void buildSkeleton_K_Iter_Ang (Skeleton *sk);
void writeSkeletonToFile (Skeleton *sk);
void calcOriginalDirection (Point p1, Point p2, double d_ori[]);
void rotate (double d_ori[], double d_rot[], double teta);
