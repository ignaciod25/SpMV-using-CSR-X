#ifndef __SPARSE_MATRICES_H

#include <cstdlib>
#include <time.h>
using namespace std;

// csr struct
typedef struct CSR {
    int *row_ptr;       // array of locations (indices) of first nonzero element of each row
    int *col_idx;       // column index of nonzero elements
    double *values;     // nonzero elements
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
    LIL_Node** rows; // list of rows made up of nodes
    int num_rows;
    int num_cols;
    int num_nonzeros;
    LILNode* data;  
} LILMatrix;

// struct for storing vector
typedef struct Vector {
    int size;
    int* values;
} Vector;

/* CSR matrix functions in csr_matrix.cpp */
// function to initialize a CSR matrix
CSRMatrix* csr_matrix_create(char* filename); // create matrix from mtx file

// function to free memory allocated for CSR matrix
void free_csr_matrix(CSRMatrix* matrix);


// function to initialize an LIL matrix
LILMatrix* lil_matrix_create(int rows, int cols); 

// function to free memory allocated for LIL matrix
void free_lil_matrix(LILMatrix* matrix);
double* spmv_lil(LILMatrix* matrix, double* vector);


/* vector functions in vector.cpp */
// functions to initialize a vector
Vector* GenerateRandVect(int size);
Vector* CreateVectorFromFile(char* filename);
// write to vector to file
void    WriteVectorToFile(Vector* v, char* filename);
// function to free memory allocated for vector
void    free_vector(Vector* v);

#endif