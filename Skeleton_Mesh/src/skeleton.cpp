#include "../include/skeleton.h"

Skeleton* newSkeleton (int argc, char *argv[])
{
    printf("[!] Building structure for the Skeleton ... ");
    fflush(stdout);
    Skeleton *sk = (Skeleton*)malloc(sizeof(Skeleton));
    sk->fib_size = atof(argv[1]);
    sk->type = atoi(argv[2]);
    strcpy(sk->filename,argv[3]);
    printf("ok\n");
    return sk;
}

void buildSkeleton (Skeleton *sk)
{
    buildSkeleton_K(sk);
    writeSkeletonToFile(sk);
}

void buildSkeleton_K (Skeleton *sk)
{
    printf("[!] Constructing Skeleton type %d - Bifurcation fiber ... ",sk->type);
    fflush(stdout);
    // Create the points
    double d_ori[3], d_rot[3];
    Point p1(0,0,0);
    Point p2(sk->fib_size,0,0);
    calcOriginalDirection(p1,p2,d_ori);
    sk->points.push_back(p1);
    sk->points.push_back(p2);
    if (sk->type % 2 == 0)
    {
        int k = sk->type / 2;
        double ang_rot = M_PI / (double)sk->type;
        double ang = ang_rot;
        for (int i = 0; i < k; i++, ang += ang_rot)
        {
            rotate(d_ori,d_rot,ang);
            Point p3(p2.x + d_rot[0]*sk->fib_size,p2.y + d_rot[1]*sk->fib_size,p2.z + d_rot[2]*sk->fib_size);
            rotate(d_ori,d_rot,-ang);
            Point p4(p2.x + d_rot[0]*sk->fib_size,p2.y + d_rot[1]*sk->fib_size,p2.z + d_rot[2]*sk->fib_size);
            sk->points.push_back(p3);
            sk->points.push_back(p4);
        }
    }
    else
    {
        int k = (sk->type-1) / 2;
        Point p3(p2.x + d_ori[0]*sk->fib_size,p2.y + d_ori[1]*sk->fib_size,p2.z + d_ori[2]*sk->fib_size);
        sk->points.push_back(p3);
        double ang_rot = M_PI / (double)(sk->type-1);
        double ang = ang_rot;
        for (int i = 0; i < k; i++, ang += ang_rot)
        {
            rotate(d_ori,d_rot,ang);
            Point p4(p2.x + d_rot[0]*sk->fib_size,p2.y + d_rot[1]*sk->fib_size,p2.z + d_rot[2]*sk->fib_size);
            rotate(d_ori,d_rot,-ang);
            Point p5(p2.x + d_rot[0]*sk->fib_size,p2.y + d_rot[1]*sk->fib_size,p2.z + d_rot[2]*sk->fib_size);
            sk->points.push_back(p4);
            sk->points.push_back(p5);
        }
    }
    // Create elements
    Element e1(0,1);
    sk->elements.push_back(e1);
    for (int i = 0; i < sk->type; i++)
    {
        Element e2(1,i+2); 
        sk->elements.push_back(e2);
    }
}

void calcOriginalDirection (Point p1, Point p2, double d_ori[])
{
    double norm;
    d_ori[0] = p2.x - p1.x; d_ori[1] = p2.y - p1.y; d_ori[2] = p2.z - p1.z;
    norm = sqrt(pow(d_ori[0],2) + pow(d_ori[1],2) + pow(d_ori[2],2));
    for (int i = 0; i < 3; i ++) d_ori[i] /= norm;
}

void rotate (double d_ori[], double d_rot[], double teta)
{
    d_rot[0] = d_ori[0]*cos(teta) + d_ori[1]*sin(teta);
    d_rot[1] = d_ori[1]*cos(teta) - d_ori[0]*sin(teta);
    d_rot[2] = d_ori[2];
}

void writeSkeletonToFile (Skeleton *sk)
{
    printf("%s\n",sk->filename);
    FILE *outFile = fopen(sk->filename,"w+");
    fprintf(outFile,"# vtk DataFile Version 3.0\n");
    fprintf(outFile,"Purkinje\nASCII\nDATASET POLYDATA\n");
    fprintf(outFile,"POINTS %d float\n",(int)sk->points.size());
    for (int i = 0; i < (int)sk->points.size(); i++)
        fprintf(outFile,"%.8lf %.8lf %.8lf\n",sk->points[i].x,sk->points[i].y,sk->points[i].z);
    fprintf(outFile,"LINES %d %d\n",(int)sk->elements.size(),(int)sk->elements.size()*3);
    for (int i = 0; i < (int)sk->elements.size(); i++)
        fprintf(outFile,"2 %d %d\n",sk->elements[i].left,sk->elements[i].right);
    fclose(outFile);
}

void freeSkeleton (Skeleton *sk)
{
    printf("[!] Desalocating memory ... ");
    fflush(stdout);
    sk->points.clear();
    sk->elements.clear();
    free(sk);
    printf("ok\n");
}