#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include "sparse_matrices.h"

using namespace std;

/* List of functions */
CSRMatrix* csr_matrix_create(char* filename);
void csr_serial_spmv(CSRMatrix* csrm, vector<double> v, char* output);
void csr_omp_spmv(CSRMatrix* csrm, vector<double> v, char* output, long num_threads);

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
    #endif

    fin >> total_rows >> total_cols >> entries;
    
    #ifdef DEBUG
    printf("rows: %d, cols: %d, entries: %d\n", total_rows, total_cols, entries);
    #endif

    vector<int> row_num_values; 
    row_num_values.reserve(total_rows);
    row_num_values.assign(total_rows, 0);
    vector<int> coo_row_indices;
    coo_row_indices.reserve(entries);
    vector<int> coo_col_indices;
    coo_col_indices.reserve(entries);
    vector<double> coo_data;
    coo_data.reserve(entries);

    csrm = new CSRMatrix;
    csrm->num_rows      = total_rows;
    csrm->num_cols      = total_cols;
    csrm->num_nonzeros  = entries;

    csrm->row_ptr.reserve(total_rows+1);
    csrm->row_ptr.assign(total_rows+1, 0);
    csrm->col_idx.reserve(entries);
    csrm->col_idx.assign(entries, 0);
    csrm->values.reserve(entries);
    csrm->values.assign(entries, 0);

    // Read mtx file similarly to the method used for COO formatted matrices
    #ifdef DEBUG
    cout << "starting first while loop\n";
    #endif
    i = 0;
    while (i<entries) {
        fin >> row >> col >> val;

        // decrement row and col values because of how mtx files are written
        row--;
        col--;

        for (int r=row+1; r<total_rows+1; r++) 
            csrm->row_ptr[r]++;
        
        row_num_values[row]++;
        coo_row_indices.push_back(row);
        coo_col_indices.push_back(col);
        coo_data.push_back(val);

        i++;
    }
    fin.close();

    #ifdef DEBUG
    cout << "starting second while loop\n";
    #endif
    i=0;
    while (i<entries) {
        row = coo_row_indices[i];
        col = coo_col_indices[i];
        val = coo_data[i];

        idx = csrm->row_ptr[row] + row_num_values[row] - 1;

        csrm->col_idx[idx] = col;
        csrm->values[idx] = val;

        row_num_values[row]--;
        i++;
    }
    
    return csrm;
} /* CSRMatrix* csr_matrix_create */

void csr_serial_spmv(CSRMatrix* csrm, vector<double> v, char* output) {
    int row, col, i, j;
    double val;
    int total_rows;
    vector<double> result;
    ofstream fout;

    total_rows  = csrm->num_rows;

    result.reserve(total_rows);
    result.assign(total_rows, 0.0);

    for (i=0; i<total_rows; i++) {
        row = i;
        for (j=csrm->row_ptr[i]; j<csrm->row_ptr[i+1]; j++) {
            col = csrm->col_idx[j];
            val = csrm->values[j];
            result[row] += val * v[col];

            #ifdef DEBUG
            cout << row << "   " << col << "   " << val << endl;
            #endif
        }
    }

    fout.open(output);
    for (auto i: result) {
        fout << i << endl;
    }
    
    fout.close();

} /* void csr_serial_spmv */

void csr_omp_spmv(CSRMatrix* csrm, vector<double> v, char* output, long num_threads) {
    int row, col, i, j;
    double val;
    int total_rows;
    vector<double> result;
    ofstream fout;

    total_rows  = csrm->num_rows;

    result.reserve(total_rows);
    result.assign(total_rows, 0.0);

    omp_set_num_threads(num_threads);
    #pragma omp parallel for private(i, j, row, col, val)
    for (i=0; i<total_rows; i++) {
        row = i;
        for (j=csrm->row_ptr[i]; j<csrm->row_ptr[i+1]; j++) {
            col = csrm->col_idx[j];
            val = csrm->values[j];
            result[row] += val * v[col];

            #ifdef DEBUG
            cout << row << "   " << col << "   " << val << endl;
            #endif
        }
    }

    fout.open(output);
    for (auto i: result) {
        fout << i << endl;
    }
    
    fout.close();

} /* void csr_omp_spmv*/