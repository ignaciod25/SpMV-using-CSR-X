/* 
    Compile:    g++ -g -Wall -fopenmp -pthread create_vectors.cpp csr_matrix.cpp vector.cpp -o v.out
    Run:        ./create_vectors

    This program will create random vectors of appropriate length for each mtx file listed in 
    the files vector. Each vector will be stored in the vectors directory and will be named
    <mtx filename>+"_vector.txt"

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
    string file;
    string outfile;
    vector<double> outvector;
    
    for (int i=0; i<files.size(); i++) {
        cout << endl << files[i] << endl;
        file = "mtxfiles/" + files[i];
        csrm_ptr = csr_matrix_create((char*)file.c_str());
        cout << csrm_ptr->num_rows << "  " << csrm_ptr->num_cols << endl;
        rows = csrm_ptr->num_rows;
        outfile = "vectors/" + file;
        outfile += "_vector.txt";
        outvector = CreateRandomVector(rows, (char*)outfile.c_str());
        cin.get();
        delete csrm_ptr;
    }

    return 0;
}
