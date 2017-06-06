#include "../include/skeleton.h"
#include "../include/queue.h"

int total_nodes;                                            // Numero total de nodos atualmente na arvore

Skeleton* newSkeleton (int argc, char *argv[])
{
    printf("[!] Building structure for the Skeleton ... ");
    fflush(stdout);
    Skeleton *sk = (Skeleton*)malloc(sizeof(Skeleton));
    sk->fib_size = atof(argv[1]);
    sk->biff = atoi(argv[2]);
    sk->type = atoi(argv[3]);
    strcpy(sk->filename,argv[4]);
    printf("ok\n");
    return sk;
}

Point* newPoint (double x, double y, double z)
{
    Point *p = (Point*)malloc(sizeof(Point));
    p->id = total_nodes;
    p->x = x;
    p->y = y;
    p->z = z;
    total_nodes++;
    return p;
}

void buildSkeleton (Skeleton *sk)
{
    printf("[!] Constructing Skeleton type %d - Iterative fiber number %d ... \n",sk->type,sk->biff);

    switch (sk->type)
    {
        case 1: buildSkeleton_K(sk);
                break;
        case 2: buildSkeleton_K_Iter(sk);
                break;
        case 3: buildSkeleton_K_Iter_Ang(sk);
                break;
        default: exit(EXIT_FAILURE);
    }
    writeSkeletonToFile(sk);
}

void buildSkeleton_K (Skeleton *sk)
{
    // Create the points
    double d_ori[3], d_rot[3];
    Point p1(0,0,0);
    Point p2(sk->fib_size,0,0);
    calcOriginalDirection(p1,p2,d_ori);
    sk->points.push_back(p1);
    sk->points.push_back(p2);
    if (sk->biff % 2 == 0)
    {
        int k = sk->biff / 2;
        double ang_rot = M_PI / (double)sk->biff;
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
        int k = (sk->biff-1) / 2;
        Point p3(p2.x + d_ori[0]*sk->fib_size,p2.y + d_ori[1]*sk->fib_size,p2.z + d_ori[2]*sk->fib_size);
        sk->points.push_back(p3);
        double ang_rot = M_PI / (double)(sk->biff-1);
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
    for (int i = 0; i < sk->biff; i++)
    {
        Element e2(1,i+2); 
        sk->elements.push_back(e2);
    }
}

// Gera uma fibra do tipo 2 - Crescimento iterativo para qualquer numero de bifurcacao
void buildSkeleton_K_Iter (Skeleton *sk)
{
    int id, k;
    double d_ori[3], d_rot[3];
    double ang, ang_rot;
    Queue *q = newQueue();

    // Create the first segment
    total_nodes = 0;
    Point *p1 = newPoint(0,0,0);
    Point *p2 = newPoint(sk->fib_size,0,0);
    calcOriginalDirection(*p1,*p2,d_ori);
    sk->points.push_back(*p1);
    sk->points.push_back(*p2);
    Element e1(p1->id,p2->id);
    sk->elements.push_back(e1);
    Enqueue(&q,p2->id,d_ori);

    for (int i = 0; i < MAX_ITER; i++)
    {
        int cont = q->in_the_queue;
        //printQueue(q);
        while (cont > 0)
        {
            QNode *qnode = Dequeue(&q);
            id = qnode->id;

            if (sk->biff % 2 == 0)
            {
                k = sk->biff / 2;
                ang_rot = M_PI / (double)sk->biff;
                ang = ang_rot;
            }
            else
            {
                k = (sk->biff-1) / 2;
                ang_rot = M_PI / (double)(sk->biff-1);
                ang = ang_rot;
                Point *p3 = newPoint(sk->points[id].x + qnode->d_ori[0]*sk->fib_size,sk->points[id].y + qnode->d_ori[1]*sk->fib_size,sk->points[id].z + qnode->d_ori[2]*sk->fib_size);
                sk->points.push_back(*p3);
                calcOriginalDirection(sk->points[id],*p3,d_ori);
                Element e2(id,p3->id);
                sk->elements.push_back(e2);
                Enqueue(&q,p3->id,d_ori);
            }

            for (int i = 0; i < k; i++, ang += ang_rot)
            {
                rotate(qnode->d_ori,d_rot,ang);
                Point *p3 = newPoint(sk->points[id].x + d_rot[0]*sk->fib_size,sk->points[id].y + d_rot[1]*sk->fib_size,sk->points[id].z + d_rot[2]*sk->fib_size);
                sk->points.push_back(*p3);
                calcOriginalDirection(sk->points[id],*p3,d_ori);
                Element e2(id,p3->id);
                sk->elements.push_back(e2);
                Enqueue(&q,p3->id,d_ori);

                
                rotate(qnode->d_ori,d_rot,-ang);
                Point *p4 = newPoint(sk->points[id].x + d_rot[0]*sk->fib_size,sk->points[id].y + d_rot[1]*sk->fib_size,sk->points[id].z + d_rot[2]*sk->fib_size);
                sk->points.push_back(*p4);
                calcOriginalDirection(sk->points[id],*p4,d_ori);
                Element e3(id,p4->id);
                sk->elements.push_back(e3);
                Enqueue(&q,p4->id,d_ori);
            }
            cont--;
        }
    }    
}

// Gera uma fibra do tipo 3 - Crescimento iterativo e com controle de angulo
void buildSkeleton_K_Iter_Ang (Skeleton *sk)
{
    int id, k;
    double d_ori[3], d_rot[3];
    double ang_rot;
    Queue *q = newQueue();

    // Create the first segment
    total_nodes = 0;
    ang_rot = ANG;
    Point *p1 = newPoint(0,0,0);
    Point *p2 = newPoint(sk->fib_size,0,0);
    calcOriginalDirection(*p1,*p2,d_ori);
    sk->points.push_back(*p1);
    sk->points.push_back(*p2);
    Element e1(p1->id,p2->id);
    sk->elements.push_back(e1);
    Enqueue(&q,p2->id,d_ori);

    for (int i = 0; i < MAX_ITER; i++)
    {
        int cont = q->in_the_queue;
        // Descomentar para diminuir o tamanho da fibra a cada iteracao de crescimento
        //sk->fib_size *= 0.6;
        //printQueue(q);
        while (cont > 0)
        {
            QNode *qnode = Dequeue(&q);
            id = qnode->id;

            rotate(qnode->d_ori,d_rot,ang_rot);
            Point *p3 = newPoint(sk->points[id].x + d_rot[0]*sk->fib_size,sk->points[id].y + d_rot[1]*sk->fib_size,sk->points[id].z + d_rot[2]*sk->fib_size);
            sk->points.push_back(*p3);
            calcOriginalDirection(sk->points[id],*p3,d_ori);
            Element e2(id,p3->id);
            sk->elements.push_back(e2);
            Enqueue(&q,p3->id,d_ori);

            
            rotate(qnode->d_ori,d_rot,-ang_rot);
            Point *p4 = newPoint(sk->points[id].x + d_rot[0]*sk->fib_size,sk->points[id].y + d_rot[1]*sk->fib_size,sk->points[id].z + d_rot[2]*sk->fib_size);
            sk->points.push_back(*p4);
            calcOriginalDirection(sk->points[id],*p4,d_ori);
            Element e3(id,p4->id);
            sk->elements.push_back(e3);
            Enqueue(&q,p4->id,d_ori);
            cont--;
        }
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