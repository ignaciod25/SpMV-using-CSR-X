/* 
    Example: 
    g++ parallel_spmv_a4.cpp -o exec ...
    ./exec csr omp 3 matrix.mtx vect.txt out1.txt
    ./exec x pth 5 matrix.mtx vect.txt out2.txt

    Compile: g++ -g -Wall -fopenmp -pthread parallel_spmv_a4.cpp csr_matrix.cpp vector.cpp -o parallel_test
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <sstream>
#include <vector>
#include "timer.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 7) {
        cerr << "Usage: " << argv[0] << " <matrix format> <parallelization method> <num threads> <matrix file> <vector file> <output file>\n";
        cerr << "Matrix Format: either 'csr' or 'ell'\n";
        cerr << "Parallelization Method: either 'omp' or 'pth'\n";
        exit(0);
    }

    string matrixFormat = argv[1];
    string parallelMethod = argv[2];
    long numThreads = strtol(argv[3], NULL, 10);
    if (numThreads > 1024) {
        cerr << "Max Threads = 1024 is 1024\n";
        exit(0);
    }
    char* matrixFile = argv[2];
    char* vectorFile = argv[3];
    char* outputFile = argv[4];

    CSRMatrix * csrm;

    if (matrixFormat == "csr" || matrixFormat == "CSR") {
        csrm = csr_matrix_create(matrixFile);
        cout << csrm->num_rows << "  " << csrm->num_cols << "  " << csrm->num_nonzeros << endl;

    } else {
        cerr << "Matrix Format: either 'csr' or 'ell'\n";
        exit(0);
    }

    vector<double> v;
    v = CreateVectorFromFile(vectorFile);

    if (parallelMethod == "omp" || parallelMethod == "OMP") {
        if (csrm->num_rows > 0) {
            csr_omp_spmv(csrm, v, outputFile, numThreads);
            cout << csrm->num_rows << "  " << csrm->num_cols << "  " << csrm->num_nonzeros << endl;
        }

    } else if (parallelMethod == "pth" || parallelMethod == "PTH") {
        if (csrm->num_rows > 0) {
            csr_pth_spmv(csrm, v, outputFile, numThreads);
        }
    } else {
        cerr << "Parallelization Method: either 'omp' or 'pth'\n";
        if (csrm->num_rows > 0) delete csrm;
        exit(0);
    }

    if (csrm) delete csrm;
    return 0;
} /* int main */