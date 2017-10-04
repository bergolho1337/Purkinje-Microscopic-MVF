#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

constexpr double sc[] = { 0.0068, 0.01, 0.0168, 0.02 };
const string cells[] = { "pig", "alien", "dog", "orc" };

void Usage (char pName[])
{
    cout << "Usage:> " << pName << " <size_fiber>" << endl;
}

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        Usage(argv[0]);
        exit (EXIT_FAILURE);
    }
    double sf = atof(argv[1]);
    for (int i = 0; i < 4; i++)
    {
        int N = sf / sc[i];
        stringstream ss;
        ss << "cable-" << cells[i] << "-" << sf << "cm.plt";
        string filename = ss.str();
        ofstream out(filename);
        
        out << 6 << endl;
        out << N/2 << " " << N << " " << 3*N/2 << " " << 2*N << " " << 5*N/2 << " " << 3*N << endl;    
    }
    for (int i = 0; i < 4; i++)
    {
        int N = sf / sc[i];
        stringstream ss;
        ss << "biff-" << cells[i] << "-" << sf << "cm.plt";
        string filename = ss.str();
        ofstream out(filename);

        out << 14 << endl;
        out << N/2 << " " << N << " " << 3*N/2 << " " << 2*N << " " << 5*N/2 << " " << 3*N << " "\
        << 7*N/2 << " " << 4*N << " " << 9*N/2 << " " << 5*N << " " << 11*N/2 << " " << 6*N << " "\
        << 13*N/2 << " " << 7*N << " ";    
    } 

    return 0;
}