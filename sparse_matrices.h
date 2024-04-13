#ifndef __SPARSE_MATRICES_H

#include <cstdlib>
#include <time.h>
using namespace std;

// csr struct
typedef struct CSR {
    int *row_ptr;   // locations of first nonzero element of each row in values
    int *col_idx;   // column index of nonzero elements
    double *values; // nonzero elements
    int num_rows; 
    int num_cols;
    int nnz;        // number of nonzero elements
} CSRMatrix;

// 
typedef struct LIL_Node {
    int row;
    int col;
    double value;
    LILNode* next;  // will point to next node in the same row
} LILNode;

typedef struct LIL {
    LIL_Node** rows; // list of rows made up of nodes
    int num_rows;
    int num_cols;
    int num_nonzeros;  
} LILMatrix;

// struct for storing vector
typedef struct Vector {
    int size;
    int* values;


} Vector;

// function to initialize a CSR matrix
CSRMatrix* csr_matrix_create(char* filename); // create matrix from mtx file


// function to initialize an LIL matrix
LILMatrix* lil_matrix_create(int rows, int cols); 


// function to initialize a vector
Vector* GenerateRandVect(int size);
Vector* CreateVectorFromFile(char* filename);
void    PrintVectorToFile(Vector* v, char* filename);

// function to free memory allocated for CSR matrix
void free_csr_matrix(CSRMatrix* matrix);

// function to free memory allocated for LIL matrix
void free_lil_matrix(LILMatrix *matrix);

// function to free memory allocated for vector


#endif