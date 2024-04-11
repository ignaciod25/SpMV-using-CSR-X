// csr struct
struct CSR {
    int *row_ptr;    
    int *col_idx; 
    double *values; 
    int num_rows; 
    int num_cols;
    int nnz;   
} CSRMatrix;

// x struct
struct X {
    int *indices;
    double *values; 
    int num_nonzeros;
    int size;       
} XMatrix;

// struct for storing vector


// function to initialize a CSR matrix


// function to initialize an X matrix


// function to initialize a vector


// function to free memory allocated for CSR matrix


// function to free memory allocated for X matrix


// function to free memory allocated for vector
