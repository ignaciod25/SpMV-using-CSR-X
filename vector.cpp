#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 
#include "sparse_matrices.h"

using namespace std;

/* vector functions in vector.cpp */
// functions to initialize a vector
vector<double> CreateVectorFromFile(char* filename);
// create random vector and print to file in vectors/ directory
vector<double> CreateRandomVector(int size, char* outfile);

vector<double> CreateVectorFromFile(char* filename) {
    vector<double> v;
    ifstream fin;
    double val;

    fin.open(filename);
    while (fin >> val) {
        v.push_back(val);
    }
    fin.close();

    #ifdef DEBUG
    cout << "size of vector: " << v.size() << endl;
    #endif

    return v;
} /* vector<int> CreateVectorFromFile */

vector<double> CreateRandomVector(int size, char* outfile) {
    vector<double> v;
    ofstream fout;
    string path("vectors/");
    path.append(string(outfile));
    const char* out = path.c_str();

    v.reserve(size);
    v.assign(size, 0.0);
    srand(time(0));
    
    fout.open(out);
    for (int i=0; i<size; i++) {
        v[i] = rand() % 1000;
        fout << v[i] << endl;
    }
    fout.close();

    return v;

} /* vector<double> CreateRandomVector */