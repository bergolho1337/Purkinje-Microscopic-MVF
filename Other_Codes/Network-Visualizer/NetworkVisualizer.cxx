// ------------------------------------------------------------------------------------------------------
// Programa que recebe uma rede de Purkinje representada por um grafo a partir de um arquivo de
// entrada .vtp e transforma em uma representacao em que as arestas sao representadas por cilindros e
// os vertices terminais por esferas.
// ------------------------------------------------------------------------------------------------------

#define USER_MATRIX

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
 
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include <vtkVersion.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkAppendPolyData.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkCylinderSource.h>
#include <vtkMath.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

using namespace std;

struct Point;
struct Line;

void Usage (const char pName[]);
void readInputVTP (std::vector<Point> &Points, std::vector<Line> &Lines, const char inFile[]);
void processData (const std::vector<Point> Points, const std::vector<Line> Lines,std::vector< vector< pair<int,int> > > Graph, const char outFile[]);
void buildGraph (std::vector< vector< pair<int,int> > > &Graph, const std::vector<Point> Points, const std::vector<Line> Lines);
void DFS (std::vector< vector< pair<int,int> > > &Graph, int u, int lvl);
void createCylinder (vtkSmartPointer<vtkAppendPolyData> &appendFilter, const std::vector<Point> points, const Line line);
void createSphere (vtkSmartPointer<vtkAppendPolyData> &appendFilter, Point point);
void printVTP (const std::vector<Point> Points, const std::vector<Line> Lines);
void printGraph (const std::vector< vector< pair<int,int> > > Graph);

const double DIAMETER = 0.1;
const double ALPHA = 0.15;
std::vector<int> levels;

struct Point
{
	double x,y,z;
    double scalar;
    int l;
	Point(const double xin, const double yin, const double zin, const double scalarin) : x(xin), y(yin), z(zin), scalar(scalarin) {}
};

struct Line
{
    int from;
    int to;
    Line (const int fromin, const int toin) : from(fromin), to(toin) {}
};

int main (int argc, char *argv[])
{
    if (argc-1 < 2)
    {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Read in all of the points
        std::vector<Point> Points;
        // Read in all of the lines
        std::vector<Line> Lines;
        // Graph
        std::vector< vector< pair<int,int> > > Graph;

        readInputVTP(Points,Lines,argv[1]);
        printVTP(Points,Lines);

        buildGraph(Graph,Points,Lines);

        processData(Points,Lines,Graph,argv[2]);

        return 0;

    }
}

void readInputVTP (std::vector<Point> &Points, std::vector<Line> &Lines, const char inFile[])
{
    // Get all data from the file
    vtkXMLPolyDataReader* reader = vtkXMLPolyDataReader::New();
    reader->SetFileName(inFile);
    reader->Update();
    vtkPolyData* polydata = reader->GetOutput();

    // Get the number of points the file contains
    vtkIdType NumPoints = polydata->GetNumberOfPoints();

    // If there are no points, quit
    if(!(NumPoints > 0) )
    {
        exit(EXIT_FAILURE);
    }

    // Read in all of the points
    double point[3], w;
    for(vtkIdType i = 0; i < NumPoints; i++)
    {
        polydata->GetPoint(i, point);
        // If the point has a scalar descomment this line
        //w = polydata->GetPointData()->GetScalars()->GetComponent(0,i);
        w = 1;
        Points.push_back(Point(point[0], point[1], point[2],w)); 
    }
    
    // Get the number of lines the file contains
    vtkIdType NumLines = polydata->GetNumberOfLines();

    //if there are no lines, quit
    if(!(NumLines > 0) )
    {
        exit(EXIT_FAILURE);
    }

    // Read in all of the lines
    vtkCellArray* LinesCells = polydata->GetLines();
    vtkIdType npts;
    vtkIdType *pts;

    while(LinesCells->GetNextCell(npts, pts))
    {
        Line line(pts[0],pts[1]);
        Lines.push_back(line);
    }
}

void processData (const std::vector<Point> Points, const std::vector<Line> Lines, \
                    const std::vector< vector< pair<int,int> > > Graph, const char outFile[])
{
    int NumPoints = (int)Points.size();
    int NumLines = (int)Lines.size();
    vtkSmartPointer<vtkAppendPolyData> appendFilter =
        vtkSmartPointer<vtkAppendPolyData>::New();

    // Create a cylinder for each line
    for (int i = 0; i < NumLines; i++)
        createCylinder(appendFilter,Points,Lines[i]);
    
    // Create a sphere for each terminal
    for (int i = 0; i < NumPoints; i++)
    {
        if ((int)Graph[i].size() == 0)
            createSphere(appendFilter,Points[i]);
    }

    // Write the file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer =  
        vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(outFile);
    writer->SetInputConnection(appendFilter->GetOutputPort());
    writer->Write();
  
}

void createSphere (vtkSmartPointer<vtkAppendPolyData> &appendFilter, Point point)
{
    vtkSmartPointer<vtkSphereSource> sphereSource = 
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(point.x,point.y,point.z);
    sphereSource->SetRadius(0.1);
    sphereSource->SetThetaResolution(15.0);
    sphereSource->SetPhiResolution(15.0);
    sphereSource->Update();

    // Color the points of the sphere with the same scalar of the point
    int numPtsSphere = sphereSource->GetOutput()->GetPoints()->GetNumberOfPoints();
    vtkSmartPointer<vtkFloatArray> scalars =
        vtkSmartPointer<vtkFloatArray>::New();
    scalars->SetNumberOfValues( numPtsSphere );
    for( int j = 0; j < numPtsSphere; j++ )
        scalars->SetValue(j,point.scalar);
    sphereSource->GetOutput()->GetPointData()->SetScalars(scalars);
    sphereSource->Update();

    // Append the sphere to the filter
    appendFilter->AddInputConnection(sphereSource->GetOutputPort());
    appendFilter->Update();
}

void createCylinder (vtkSmartPointer<vtkAppendPolyData> &appendFilter, const std::vector<Point> points, \
                        const Line line)
{

    // Get coordinate of the points of the line
    int from, to;
    double startPoint[3], endPoint[3];
    from = line.from; to = line.to;
    startPoint[0] = points[from].x; startPoint[1] = points[from].y; startPoint[2] = points[from].z;
    endPoint[0] = points[to].x; endPoint[1] = points[to].y; endPoint[2] = points[to].z;

    // Get the diameter of the cylinder
    double diameter = DIAMETER - levels[to]*DIAMETER*ALPHA;

    //Create an cylinder.
    // Cylinder height vector is (0,1,0).
    // Cylinder center is in the middle of the cylinder
    vtkSmartPointer<vtkCylinderSource> cylinderSource =
        vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetResolution(15);
    cylinderSource->SetRadius(diameter);
    cylinderSource->Update();

    // Color the points of the cylinder with the same scalar of the first point
    //int numPtsCylinder = cylinderSource->GetOutput()->GetPoints()->GetNumberOfPoints();
    //vtkSmartPointer<vtkFloatArray> scalars =
    //    vtkSmartPointer<vtkFloatArray>::New();
    //scalars->SetNumberOfValues( numPtsCylinder );
    //for( int j = 0; j < numPtsCylinder; j++ )
    //    scalars->SetValue(j,points[from].scalar);
    //cylinderSource->GetOutput()->GetPointData()->SetScalars(scalars);
    //cylinderSource->Update();

    // Compute a basis
    double normalizedX[3];
    double normalizedY[3];
    double normalizedZ[3];

    // The X axis is a vector from start to end
    vtkMath::Subtract(endPoint, startPoint, normalizedX);
    double length = vtkMath::Norm(normalizedX);
    vtkMath::Normalize(normalizedX);

    // The Z axis is an arbitrary vector cross X
    double arbitrary[3];
    arbitrary[0] = vtkMath::Random(-10,10);
    arbitrary[1] = vtkMath::Random(-10,10);
    arbitrary[2] = vtkMath::Random(-10,10);
    vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
    vtkMath::Normalize(normalizedZ);

    // The Y axis is Z cross X
    vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
    vtkSmartPointer<vtkMatrix4x4> matrix =
        vtkSmartPointer<vtkMatrix4x4>::New();

    // Create the direction cosine matrix
    matrix->Identity();
    for (unsigned int i = 0; i < 3; i++)
    {
        matrix->SetElement(i, 0, normalizedX[i]);
        matrix->SetElement(i, 1, normalizedY[i]);
        matrix->SetElement(i, 2, normalizedZ[i]);
    }

    // Apply the transforms
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->Translate(startPoint);   // translate to starting point
    transform->Concatenate(matrix);     // apply direction cosines
    transform->RotateZ(-90.0);          // align cylinder to x axis
    transform->Scale(1.0, length, 1.0); // scale along the height vector
    transform->Translate(0, .5, 0);     // translate to start of cylinder

    // Transform the polydata
    vtkSmartPointer<vtkTransformPolyDataFilter> transformPD =
        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPD->SetTransform(transform);
    transformPD->SetInputConnection(cylinderSource->GetOutputPort());

    // Append the cylinder to the filter
    appendFilter->AddInputConnection(transformPD->GetOutputPort());
    appendFilter->Update();
    
}

void buildGraph (std::vector< vector< pair<int,int> > > &Graph, const std::vector<Point> Points, const std::vector<Line> Lines)
{
    int NumPoints = (int)Points.size();
    int NumLines = (int)Lines.size();
    Graph.assign(NumPoints,vector< pair<int,int> >());
    for (int i = 0; i < NumLines; i++)
    {
        int from = Lines[i].from;
        int to = Lines[i].to;
        Graph[from].push_back(make_pair(to,1));
    }
    // Do a DFS to calculate the level of each node on the network
    levels.assign(NumPoints,0);
    DFS(Graph,0,0);
    //printGraph(Graph);
}

void DFS (std::vector< vector< pair<int,int> > > &Graph, int u, int lvl)
{
    levels[u] = lvl;
    for (size_t j = 0; j < Graph[u].size(); j++)
    {
        pair<int,int> v = Graph[u][j];
        DFS(Graph,v.first,lvl+1);
    }
}

void printGraph (const std::vector< vector< pair<int,int> > > Graph)
{
    for (int i = 0; i < (int)Graph.size(); i++)
    {
        cout << "|| " << i << " ||";
        for (int j = 0; j < (int)Graph[i].size(); j++)
            cout << " --> || " << Graph[i][j].first << " ||";
        cout << endl;
    }
}

void printVTP (const std::vector<Point> Points, const std::vector<Line> Lines)
{
    int NumPoints = (int)Points.size();
    cout << "Points" << endl;
    for (int i = 0; i < NumPoints; i++)
        cout << Points[i].x << " " << Points[i].y << " " << Points[i].z << endl;

    int NumLines = (int)Lines.size();
    cout << "Lines" << endl;
    for (int i = 0; i < NumLines; i++)
        cout << Lines[i].from << " " << Lines[i].to << endl;
}

void Usage (const char pName[])
{
    cout << "=======================================================" << endl;
    cout << "Usage:> " << pName << " <in_vtp_file> <out_vtp_file>" << endl;
    cout << "=======================================================" << endl;
}