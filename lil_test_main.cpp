#include "sparse_matrices.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <matrix_file> <vector_file> <output_file>" << endl;
        return 1;
    }

    // opens the matrix file
    ifstream matrix_file(argv[1]);
    if (!matrix_file.is_open()) {
        cerr << "Error: Unable to open matrix file." << endl;
        return 1;
    }

    // opens vector file
    ifstream vector_file(argv[2]);
    if (!vector_file.is_open()) {
        cerr << "Error: Unable to open vector file." << endl;
        return 1;
    }

    // opens output file
    ofstream output_file(argv[3]);
    if (!output_file.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    // reads the matrix dimensions from the matrix file
    int rows, cols, num_nonzero;
    matrix_file >> rows >> cols >> num_nonzero;
    cout << "Matrix dimensions: " << rows << " x " << cols << endl;

    // creates the LIL matrix
    LILMatrix* matrix = lil_matrix_create(rows, cols);

    // reads vector size from vector file
    int vector_size;
    vector_file >> vector_size;
    cout << "Vector size: " << vector_size << endl;

    // allocates memory for vector
    double* vector = new double[vector_size];

    // reads vector values from the vector files
    cout << "Vector values:" << endl;
    for (int i = 0; i < vector_size; i++) {
        vector_file >> vector[i];
        cout << vector[i] << endl;
    }

    //  SpMV 
    double* result = spmv_lil(matrix, vector);

    // writes to output file
    cout << "Writing result to output file..." << endl;
    for (int i = 0; i < rows; i++) {
        output_file << result[i] << endl;
    }
    cout << "Result written to output file successfully." << endl;

    // free memory
    free_lil_matrix(matrix);
    delete[] vector;
    free(result);

    // closes files
    matrix_file.close();
    vector_file.close();
    output_file.close();

    return 0;
}
