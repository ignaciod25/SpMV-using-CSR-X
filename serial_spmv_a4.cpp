/* 
    Example: 
    g++ serial_spmv_a4.cpp -o exec ...
    ./exec csr matrix.mtx vect.txt out1.txt
    ./exec x matrix.mtx vect.txt out2.txt

    Compile: g++ -g -Wall -fopenmp -pthread serial_spmv_a4.cpp csr_matrix.cpp vector.cpp -o serial_test
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>
#include "timer.h"

using namespace std;
#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>
#include "timer.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <matrix format> <matrix file> <vector file> <output file>\n";
        cerr << "Matrix Format: either 'csr' or 'ell'\n";
        exit(0);
    }

    string matrixFormat = argv[1];
    char* matrixFile = argv[2];
    char* vectorFile = argv[3];
    char* outputFile = argv[4];

    if (matrixFormat == "csr" || matrixFormat == "CSR") {
        CSRMatrix * csrm;
        csrm = csr_matrix_create(matrixFile);
        // cout << csrm->num_rows << "  " << csrm->num_cols << "  " << csrm->num_nonzeros << endl;

        vector<double> v;
        v = CreateVectorFromFile(vectorFile);

        csr_serial_spmv(csrm, v, outputFile);

        delete csrm;

    } else {
        cerr << "Matrix Format: either 'csr' or 'ell'\n";
        exit(0);
    }

    return 0;
} /* int main */