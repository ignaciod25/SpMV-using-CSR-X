#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include "sparse_matrices.h"

using namespace std;
/* vector functions in vector.cpp */
// functions to initialize a vector
vector<double> CreateVectorFromFile(char* filename);
// write to vector to file
void WriteVectorToFile(vector<double> v, char* filename);


vector<double> CreateVectorFromFile(char* filename) {
    vector<double> v;
    ifstream fin(filename);
    double val;

    while (fin >> val) {
        v.push_back(val);
    }
    fin.close();

    #ifdef DEBUG
    cout << "size of vector: " << v.size() << endl;
    #endif

    return v;
} /* vector<int> CreateVectorFromFile */