#ifndef __SPARSE_MATRICES_H

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <sstream>

using namespace std;

// csr struct
typedef struct CSRMATRIX {
    vector<int> row_ptr;       // array of locations (indices) of first nonzero element of each row
    vector<int> col_idx;       // column index of nonzero elements
    vector<double> values;     // nonzero elements
    int num_rows;       // number of rows, num_rows + 1 = length of row_ptr array
    int num_cols;       // number of cols
    int num_nonzeros;   // number of entries, length of col_idx array and values array
} CSRMatrix;

/* CSR matrix functions in csr_matrix.cpp */
// function to initialize a CSR matrix
CSRMatrix* csr_matrix_create(char* filename); // create matrix from mtx file, will need to delete matrix ptr after creation
void csr_serial_spmv(CSRMatrix* csrm, vector<double> v, char* output);
void csr_omp_spmv(CSRMatrix* csrm, vector<double> v, char* output, long thread_count);
void csr_pth_spmv(CSRMATRIX* csrm, vector<double> v, char* output, long thread_count);

/* vector functions in vector.cpp */
// functions to initialize a vector
vector<double> CreateVectorFromFile(char* filename);
// create random vector and print to file in vectors/ directory
vector<double> CreateRandomVector(int size, char* outfile);

#endif