/*
    Receive two files with the activation times of the terminal points.
    The first file must be the data from the cable fiber.
    The second file must be the data from the bifurcation fiber.
    The output is the difference between the activation time of the bifurcation and the cable.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

void Usage (const char pName[])
{
    cout << "Usage:> " << pName << " <input_time_cable> <input_time_biff> <output_time_delay>" << endl;
}

void readTimes (const string name, vector<double> &arr)
{
    double value;
    ifstream in(name);
    while (in >> value)
        arr.push_back(value);
}

void calcDelay (string cable, string biff, string delay)
{
    vector<double> c, b;
    readTimes(cable,c);
    readTimes(biff,b);

    ofstream out(delay);
    for (size_t i = 0; i < c.size(); i++)
        out << b[i]-c[i] << "\t";
    out << endl;
}

int main (int argc, char *argv[])
{
    if (argc-1 < 3)
    {
        Usage(argv[0]);
        exit (EXIT_FAILURE);
    }
    string filename_cable(argv[1]);
    string filename_biff(argv[2]);
    string filename_delay(argv[3]);
    calcDelay(filename_cable,filename_biff,filename_delay);

    return 0;
} 