/* 
    Compile:    g++ -g -Wall csr_main.cpp csr_matrix.cpp vector.cpp -o csr_test
    Run:        ./csr_test <matrix file> <vector file> <output file>
    Flags:      -DDEBUG prints labels as processes occur within the program
                
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0]  << " <matrix_file> <vector_file> <output_file>" << endl;
        exit(0);
    }

    CSRMatrix* csrm;
    vector<double> v;

    csrm = csr_matrix_create(argv[1]);
    v = CreateVectorFromFile(argv[2]);

    int i=0;
    int j=0;
    int col, row;
    double val;
    while (i<csrm->num_rows) {
        row = i;
        while (j<csrm->row_ptr[i+1]) {
            col = csrm->col_idx[j];
            val = csrm->values[j];
            #ifdef DEBUG
            printf("%d, %d, %lg\n", row, col, val);
            #endif
            j++;
        }  
        i++;
    }

    #ifdef DEBUG
    cout << "size of matrix (num rows/cols): " << csrm->num_rows << endl;
    cout << "size of vector: " << v.size() << endl;

    cout << "vector v: \n";
    for (auto i: v)
        cout << i << ", ";
    cout << endl;

    cout << "performing serial spmv.................." << endl;
    #endif
    csr_serial_spmv(csrm, v, argv[3]);

    delete csrm;

    return 0;
} /* int main */

