#include <iostream>
#include <fstream>
#include <string>
#include "sparse_matrices.h"

using namespace std;

CSRMatrix* csr_matrix_create(char* filename) {
    CSRMatrix* csrm;
    string banner;
    int total_rows, total_cols, entries;
    int row, col, i, idx;
    double val;

    ifstream fin(filename);
    getline(fin, banner);

    #ifdef DEBUG
    cout << banner << endl;
    #endif;

    fin >> total_rows >> total_cols >> entries;
    
    #ifdef DEBUG
    printf("rows: %d, cols: %d, entries: %d\n", total_rows, total_cols, entries);
    #endif;

    csrm = new CSRMatrix;
    csrm->num_rows      = total_rows;
    csrm->num_cols      = total_cols;
    csrm->num_nonzeros  = entries;

    csrm->row_ptr.reserve(total_rows+1);
    csrm->row_ptr.assign(total_rows+1, 0);

    csrm->col_idx.reserve(entries);
    csrm->values.reserve(entries);

    i = 0;
    while (i<entries) {
        fin >> row >> col >> val;

        // decrement row and col values because of how mtx files are written
        row--;
        col--;

        for (int r=row+1; r<total_rows; r++) 
            csrm->row_ptr[r]++;
        
        idx = csrm->row_ptr[row];
        csrm->col_idx.insert(csrm->col_idx.begin() + idx, col);
        csrm->values.insert(csrm->values.begin() + idx, val);
        i++;
    }
    fin.close();
    
    return csrm;
} /* csr_matrix_create */
