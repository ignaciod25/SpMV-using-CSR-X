/* 
    Compile:    g++ -g -Wall -fopenmp -pthread create_vectors.cpp csr_matrix.cpp vector.cpp -o create_vectors
    Run:        ./create_vectors

    This program will create random vectors of appropriate length for each mtx file listed in 
    the files vector. Each vector will be stored in the vectors directory and will be named
    <mtx filename>+"_vector.txt". 
    Don't need to run more than once before benchmark testing.
    
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    ofstream fout; 
    vector<string> files({
        "G9.mtx",
        "Goodwin_013.mtx",
        "abtaha1.mtx",
        "adder_dcop_66.mtx",
        "barth5.mtx",
        "bcsstk14.mtx",
        "bcsstk15.mtx",
        "blckhole.mtx",
        "brainpc2.mtx",
        "jnlbrng1.mtx",
        "lp_qap12.mtx",
        "t60k.mtx"
    });

    CSRMatrix* csrm_ptr;
    int rows;
    string outfile;
    vector<double> outvector;
    
    for (auto file: files) {
        csrm_ptr = csrm_matrix_create(file);
        rows = csrm_ptr->num_rows;
        delete csrm_ptr;
        outfile = file;
        outfile += "_vector.txt";
        outvector = CreateRandomVector(rows, )
    }

}
