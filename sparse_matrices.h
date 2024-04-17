#ifndef __SPARSE_MATRICES_H

#include <cstdlib>
#include <cstdio>
#include <vector>
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

// 
typedef struct LILNode {
    int row;
    int col;
    double value;
    struct LILNode* next;  // will point to next node in the same row
} LILNode;

typedef struct LILMATRIX {
    LILNode** rows; // list of rows made up of nodes
    int num_rows;
    int num_cols;
    int num_nonzeros;
    LILNode* data;  
} LILMatrix;

/* CSR matrix functions in csr_matrix.cpp */
// function to initialize a CSR matrix
CSRMatrix* csr_matrix_create(char* filename); // create matrix from mtx file, will need to delete matrix ptr after creation
void csr_serial_spmv(CSRMatrix* csrm, vector<double> v, char* output);
void csr_omp_spmv(CSRMatrix* csrm, vector<double> v, char* output, long thread_count);
void csr_pth_spmv(CSRMATRIX* csrm, vector<double> v, char* output, long thread_count);

// function to initialize an LIL matrix
LILMatrix* lil_matrix_create(int rows, int cols); 

// function to free memory allocated for LIL matrix
void free_lil_matrix(LILMatrix* matrix);
double* spmv_lil(LILMatrix* matrix, double* vector);


/* vector functions in vector.cpp */
// functions to initialize a vector
vector<double> CreateVectorFromFile(char* filename);
// create random vector and print to file in vectors/ directory
vector<double> CreateRandomVector(int size, char* outfile);

#endif