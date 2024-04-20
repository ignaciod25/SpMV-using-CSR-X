/* 
    Compile:    g++ -g -Wall -fopenmp -pthread ell_actual_main.cpp ell_matrix.cpp vector.cpp -o ell_test
    Run:        ./ell_test <matrix file> <vector file>
    Flags:      -DDEBUG prints labels as processes occur within the program

*/

#include "sparse_matrices.h"
#include "timer.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0]  << " <matrix_file> <vector_file>" << endl;
        exit(0);
    }

    double start, finish, elapsed;
    ELLMatrix* ellm;
    vector<double> v;
    char* out_serial;
    char* out_omp;
    char* out_pth;
    long num_threads;

    ellm = ell_matrix_create(argv[1]);
    v = CreateVectorFromFile(argv[2]);
    out_serial = "output_csr_serial.txt";
    out_omp = "output_csr_omp.txt";
    out_pth = "output_csr_pth.txt";
    num_threads = 6;

    #ifdef DEBUG
    int col, row, idx;
    double val;
    int maxrowsize = ellm->MaxRowSize;

    for (int i=0; i<ellm->num_rows; i++) {
        for (int j=0; j<maxrowsize; j++) {
            idx = i * maxrowsize + j;
            col = ellm->col_idx[idx];
            val = ellm->values[idx];
            row = i;
            if (col >= 0)
                printf("%d  %d  %lg\n", row, col, val);
        }
    }

    cout << "cols: ";
    for (auto c: ellm->col_idx) {
        if (c >= 0)
            cout << c << "  ";
    }
    cout << endl;

    #endif

    // GET_TIME(start);
    // csr_serial_spmv(csrm, v, out_serial);
    // GET_TIME(finish);
    // elapsed = finish - start;
    // cout << "csr_serial_spmv performance: " << elapsed << endl;

    // GET_TIME(start);
    // csr_omp_spmv(csrm, v, out_omp, num_threads);
    // GET_TIME(finish);
    // elapsed = finish - start;
    // cout << "csr_omp_spm performance: " << elapsed << endl;

    // GET_TIME(start);
    // csr_pth_spmv(csrm, v, out_pth, num_threads);
    // GET_TIME(finish);
    // elapsed = finish - start;
    // cout << "csr_pth_spmv performance: " << elapse
    ellm->col_idx.clear();
    ellm->values.clear();
    ellm = NULL;
    // ndl;

    delete ellm;
    
    return 0;
} /* int main */

