#include <iostream>
#include "mmio.h"
#include "sparse_matrices.h"

CSRMatrix* csr_matrix_create(char* filename) {
    FILE* rptr;
    MM_typecode matcode;
    CSRMatrix* csrm;

    rptr = fopen(filename, "r");
    mm_read_banner(rptr, &matcode);

    int rows, cols, entries;
    mm_read_mtx_crd_size(rptr, &rows, &cols, &entries);
    
    csrm = new CSRMatrix;
    csrm->num_rows = rows;
    csrm->num_cols = cols;
    csrm->nnz = entries;

    csrm->row_ptr = new int[rows];     
    csrm->col_idx = new int[entries];
    csrm->values = new double[entries];

    
    return csrm;
} /* csr_matrix_create */

void free_csr_matrix(CSRMatrix* matrix) {
    /*
        need to free row_ptr, col_idx, values
        then free matrix pointer
    */

} /* free_csr_matrix */