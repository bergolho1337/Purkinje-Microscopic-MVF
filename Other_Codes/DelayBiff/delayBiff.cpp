#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

void Usage (const char pName[])
{
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << "Usage:> " << pName << " <time_file_1> <time_file_2>" << endl;
    cout << "<time_file> = File with the activation time of the terminal points" << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
}

void readFile (vector<double> &v, const char fName[])
{
    double val;
    ifstream in(fName);
    while (in >> val)
        v.push_back(val);
}

void computeDiff (const vector<double> t1, const vector<double> t2)
{
    int n1, n2, n3;
    vector<double> diff;
    
    n1 = t1.size() / 2;
    n2 = t2.size() / 2;
    n3 = n2 / 2;

    // Compute the t1 elements
    for (int i = 0; i < n1; i++)
    {
        for (int j = i; j < n2; j += n3)
        {
            diff.push_back(fabs(t1[i]-t2[j]));
        }
    }
    // Compute the t2 elements
    for (int i = n1; i < (int)t1.size(); i++)
    {
        for (int j = i+n3; j < (int)t2.size(); j += n3)
        {
            diff.push_back(fabs(t1[i]-t2[j]));
        }
    }

    // Print results
    cout << diff.size() << endl;
    ofstream out("diff.txt");
    int n = diff.size() / 2;
    for (int i = 0; i < n; i += 2)
        out << diff[i] << "\t";
    out << endl;
    for (int i = n; i < (int)diff.size(); i += 2)
        out << diff[i] << "\t";
    out << endl;
}

int main (int argc, char *argv[])
{
    if (argc-1 != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    vector<double> t1, t2;
    readFile(t1,argv[1]);
    readFile(t2,argv[2]);
    computeDiff(t1,t2);

    //cout << (int)t1.size() << endl;
    //cout << (int)t2.size() << endl;

    return 0;

}