#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <pthread.h>
#include "sparse_matrices.h"

using namespace std;

/* Global Variable for pthread functions */
long num_threads;
CSRMatrix* csrm_global_ptr;
vector<double>* vp;
vector<double>* result_ptr;
int* flags;
long tasks_per_thread;
long leftovers;

/* List of functions */
CSRMatrix* csr_matrix_create(char* filename);
void csr_serial_spmv(CSRMatrix* csrm, vector<double> v, char* output);
void csr_omp_spmv(CSRMatrix* csrm, vector<double> v, char* output, long thread_count);
void* ThreadRoutine(void* rank);
void csr_pth_spmv(CSRMatrix* csrm, vector<double> v, char* output, long thread_count);

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

void csr_omp_spmv(CSRMatrix* csrm, vector<double> v, char* output, long thread_count) {
    int row, col, i, j;
    double val;
    int total_rows;
    vector<double> result;
    ofstream fout;

    total_rows  = csrm->num_rows;

    result.reserve(total_rows);
    result.assign(total_rows, 0.0);

    omp_set_num_threads(thread_count);
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

void* ThreadRoutine(void* rank) {
    long my_rank = (long) rank;
    int i, j, total_rows;
    int first_i, last_i;
    int row, col;
    double val;

    first_i = my_rank * tasks_per_thread;
    last_i = first_i + tasks_per_thread;

    if (last_i > (csrm_global_ptr->num_nonzeros - leftovers)) last_i++;

    for (i=first_i; i<last_i; i++) {
        row = i;
        for (j=csrm_global_ptr->row_ptr[i]; j<csrm_global_ptr->row_ptr[i+1]; j++) {
            col = csrm_global_ptr->col_idx[j];
            val = csrm_global_ptr->values[j];

            while (flags[row] != 0);
            flags[row] = 1;
            result_ptr->at(row) += val * vp->at(col);
            flags[row] = 0;

            #ifdef DEBUG
            cout << "thread rank: " << my_rank << endl;
            cout << row << "   " << col << "   " << val << endl;
            #endif
        }
    }

} /* void* ThreadRoutine */

void csr_pth_spmv(CSRMatrix* csrm, vector<double> v, char* output, long thread_count) {
    int row, col, i, j;
    double val;
    int total_rows;
    long entries;
    ofstream fout;
    long thread;

    num_threads = thread_count;
    pthread_t thread_handles[num_threads];
    csrm_global_ptr = csrm;
    vp = &v;
    total_rows  = csrm->num_rows;
    flags = new int[total_rows];
    for (int i=0; i<total_rows; i++)
        flags[i] = 0;

    entries = csrm_global_ptr->num_nonzeros;
    leftovers = entries % num_threads;
    tasks_per_thread = entries / num_threads;

    result_ptr->reserve(total_rows);
    result_ptr->assign(total_rows, 0.0);

    for (thread=0; thread<num_threads; thread++) 
        pthread_create(&thread_handles[thread], NULL, ThreadRoutine, (void*) thread);

    for (thread=0; thread<num_threads; thread++)
        pthread_join(thread_handles[thread], NULL);

    fout.open(output);
    for (auto i: *result_ptr) {
        fout << i << endl;
    }
    fout.close();

    delete[] flags;
}