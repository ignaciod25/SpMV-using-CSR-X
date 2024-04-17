/*
    Compile:    g++ -g -Wall -fopenmp csr_main.cpp csr_matrix.cpp vector.cpp -o csr_test
    Run:        ./csr_test <matrix file> <vector file>
    Flags:      -DDEBUG prints labels as processes occur within the program

    This program will result in two output files: 
    - output_csr_serial.txt
    - output_csr_omp.txt
                
*/

#include "sparse_matrices.h"
#include "timer.h"
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0]  << " <matrix_file> <vector_file>" << endl;
        exit(0);
    }

    double start, finish, elapsed;
    CSRMatrix* csrm;
    vector<double> v;
    char* out_serial;
    char* out_omp;
    long num_threads;

    csrm = csr_matrix_create(argv[1]);
    v = CreateVectorFromFile(argv[2]);
    out_serial = "output_csr_serial.txt";
    out_omp = "output_csr_omp.txt";
    num_threads = 2;

    #ifdef DEBUG
    int i=0;
    int j=0;
    int col, row;
    double val;
    while (i<csrm->num_rows) {
        row = i;
        while (j<csrm->row_ptr[i+1]) {
            col = csrm->col_idx[j];
            val = csrm->values[j];
            
            printf("%d, %d, %lg\n", row, col, val);
            
            j++;
        }  
        i++;
    }
    #endif

    #ifdef DEBUG
    cout << "size of matrix (num rows/cols): " << csrm->num_rows << endl;
    cout << "size of vector: " << v.size() << endl;

    cout << "vector v: \n";
    for (auto i: v)
        cout << i << ", ";
    cout << endl;

    cout << "performing serial spmv.................." << endl;
    #endif

    GET_TIME(start);
    csr_serial_spmv(csrm, v, out_serial);
    GET_TIME(finish);
    elapsed = finish - start;
    cout << "csr_serial_spmv performance: " << elapsed << endl;

    GET_TIME(start);
    csr_omp_spmv(csrm, v, out_omp, num_threads);
    GET_TIME(finish);
    elapsed = finish - start;
    cout << "csr_omp_spm performance: " << elapsed << endl;

    delete csrm;
    
    return 0;
} /* int main */

