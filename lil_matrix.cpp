#include "sparse_matrices.h"
#include <iostream>



// function to initialize an LIL matrix
LILMatrix* lil_matrix_create(int rows, int cols) {
    LILMatrix *matrix = (LILMatrix*)malloc(sizeof(LILMatrix));  

    matrix->data = NULL;
    matrix->num_rows = rows;
    matrix->num_cols = cols;
    matrix->num_nonzero = 0;

    return matrix; // pointer to new LIL matrix
}


// free memory LIL matrix
void free_lil_matrix() {
    if (matrix != NULL) {
        LIL_node *current = matrix->data;
        while (current != NULL) {
            LIL_Node *temp = current;
            current = current->next;
            free(temp);
        }
        free(matrix);
    }
}

// function that 
