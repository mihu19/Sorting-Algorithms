#include <fstream>
#include <cstdlib>
#include <ctime>
#include "utils.h"
using namespace std;

vector<int> readFromFile(const string& filename) 
{
    ifstream in(filename);
    vector<int> data;
    int value;

    while (in >> value) 
        data.push_back(value);

    in.close();
    return data;
}

void writeToFile(const string& filename, const vector<int>& data) 
{
    ofstream out(filename);

    for (int value : data) 
        out << value << " ";
    out.close();
}
