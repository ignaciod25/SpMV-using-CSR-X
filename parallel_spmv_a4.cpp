/* 
    Example: 
    g++ parallel_spmv_a4.cpp -o exec ...
    ./exec csr omp 3 matrix.mtx vect.txt out1.txt
    ./exec x pth 5 matrix.mtx vect.txt out2.txt

    Compile: 
    g++ -g -Wall -fopenmp -pthread parallel_spmv_a4.cpp csr_matrix.cpp vector.cpp -o parallel_test
    
*/

#include "sparse_matrices.h"
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
    char* matrixFile = argv[4];
    char* vectorFile = argv[5];
    char* outputFile = argv[6];

    CSRMatrix * csrm;

    if (matrixFormat == "csr" || matrixFormat == "CSR") {
        csrm = csr_matrix_create(matrixFile);
    } else {
        cerr << "Matrix Format: either 'csr' or 'ell'\n";
        exit(0);
    }

    vector<double> v;
    v = CreateVectorFromFile(vectorFile);

    if (parallelMethod == "omp" || parallelMethod == "OMP") {
        if (csrm->num_rows > 0) {
            csr_omp_spmv(csrm, v, outputFile, numThreads);
        }

    } else if (parallelMethod == "pth" || parallelMethod == "PTH") {
        if (csrm->row_ptr.size() > 0) {
            csr_pth_spmv(csrm, v, outputFile, numThreads);
        }

    } else {
        cerr << "Parallelization Method: either 'omp' or 'pth'\n";
        if (csrm->row_ptr.size() > 0) delete csrm;
        exit(0);
    }

    if (csrm->row_ptr.size() > 0) delete csrm;
    return 0;
} /* int main */