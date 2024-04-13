#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "mmio.h"
#include "sparse_matrices.h"

using namespace std;

CSRMatrix* csr_matrix_create(char* filename);
void free_csr_matrix(CSRMatrix* matrix);

CSRMatrix* csr_matrix_create(char* filename) {
    FILE* rptr;
    MM_typecode matcode;
    CSRMatrix* csrm;

    rptr = fopen(filename, "r");
    if (mm_read_banner(rptr, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    int rows, cols, entries;
    if ((mm_read_mtx_crd_size(rptr, &rows, &cols, &entries)) !=0)
        exit(1);

    int row; // row idx
    int col; // col idx
    double val;
    int i = 0;
    int* row_num_values     = new int[rows+1]; // keeps track of how many nonzero values in each row
    int* coo_row_indices    = new int[entries];
    int* coo_col_indices    = new int[entries];
    double* coo_data        = new double[entries];

    /* Initialize number of values in each row to 0 */
    for (int k=0; k<rows+1; k++)
        row_num_values[k] = 0;
    
    // there's probably a more efficient way lol
    // similar while loop for storing sparse matrices in coo format
    while (i<entries) {
        fscanf(rptr, "%d %d %lg\n", &row, &col, &val);
        // decrement row and col values because of how mtx files are written
        row--;
        col--;

        coo_row_indices[i] = row;
        coo_col_indices[i] = col;
        coo_data[i] = val;

        row_num_values[row]++;
        i++;
    }
    fclose(rptr);

    csrm = new CSRMatrix;
    csrm->num_rows      = rows;
    csrm->num_cols      = cols;
    csrm->num_nonzeros  = entries;

    csrm->row_ptr   = new int[rows+1]; 
    /* Initialize all ptrs to col index where first the row's nonzero value is */  
    int running_idx = 0;
    for (int k=0; k<rows+1; k++) {
        csrm->row_ptr[k] = running_idx + row_num_values[k];
        running_idx = csrm->row_ptr[k];
    }

    csrm->col_idx   = new int[entries];
    csrm->values    = new double[entries];

    i=0;
    int num_row_elements_left; // will decrement row_num_values[row] after every entry
    int curr_col_idx_start; // stores val of csrm->row_ptr[row]
    int idx; // stores relevant idx
    // idx = curr_col_idx_start + num_row_elements_left
    while (i<entries) {
        row = coo_row_indices[i];
        col = coo_col_indices[i];
        val = coo_data[i];
        
        num_row_elements_left = row_num_values[row];
        curr_col_idx_start = csrm->row_ptr[row];
        idx = curr_col_idx_start + num_row_elements_left;

        csrm->col_idx[idx] = col;
        csrm->values[idx] = val;
        
        row_num_values[row]--;
        i++;
    }

    delete[] row_num_values;
    delete[] coo_row_indices;
    delete[] coo_col_indices;
    delete[] coo_data;

    return csrm;
} /* csr_matrix_create */

void free_csr_matrix(CSRMatrix* matrix) {
    // need to free row_ptr, col_idx, values
    // then free matrix pointer
    
    delete[] matrix->row_ptr;
    delete[] matrix->col_idx;
    delete[] matrix->values;

    delete matrix;
    matrix = NULL;

} /* free_csr_matrix */