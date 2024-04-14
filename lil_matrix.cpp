#include "sparse_matrices.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// function to initialize an LIL matrix
LILMatrix* lil_matrix_create(int rows, int cols) {
    LILMatrix *matrix = (LILMatrix*)malloc(sizeof(LILMatrix)); 
    
    matrix->rows = (LILNode**)malloc(rows * sizeof(LILNode*)); // Allocate memory for rows
    if (matrix->rows == NULL) {
        cerr << "Memory allocation failed" << endl;
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix->rows[i] = NULL;
    }

    matrix->num_rows = rows;
    matrix->num_cols = cols;
    matrix->num_nonzeros = 0;

    return matrix; // pointer to new LIL matrix
}


// free memory LIL matrix
void free_lil_matrix(LILMatrix* matrix) {
    for (int i = 0; i < matrix->num_rows; i++) {
        LILNode* current = matrix->rows[i];
        while (current != NULL) {
            LILNode* time = current;
            current = current->next;
            free(time);
        }
    }
    free(matrix->rows);
    free(matrix);
}

// function that LIL Spmv
double* spmv_lil(LILMATRIX *matrix, double *vector) {
    double *result = (double*)malloc(matrix->num_rows * sizeof(double));
    for (int i = 0; i < matrix->num_rows; i++) {
        result[i] = 0.0;
        LILNode* current = matrix->rows[i];
        while (current != NULL) {
            result[i] += current->value * vector[current->col];
            current = current->next;
        }
    }
    return result;

}
