/* 
    Compile:    g++ -g -Wall vector.cpp -fopenmp -pthread csr_matrix.cpp vector_main.cpp -o v_test
    Run:        ./v_test 
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>

int main() {
    
    vector<string> files({
        // "494_bus.mtx", 
        // "G9.mtx",
        // "Goodwin_013.mtx", 
        // "abtaha1.mtx", 
        // "adder_dcop_66.mtx", 
        "bp_800.mtx",
        "brainpc2.mtx", 
        "coater1.mtx",
        "lp_qap12.mtx",
        "t60k.mtx"
    });
    
    int i, num_cols;
    vector<double> v;
    string file;
    string v_out;
    CSRMatrix** csrms = (CSRMatrix**)malloc(sizeof(CSRMatrix) * files.size());
    CSRMatrix* csrm_ptr;

    i=0;
    for (auto e: files) {
        csrm_ptr = csrms[i];
        file = "mtxfiles/" + e;
        csrm_ptr = csr_matrix_create((char*)file.c_str());

        num_cols = csrm_ptr->num_cols;
        v_out = "vectors/" + e + ".vector.txt";
        v = CreateRandomVector(num_cols, (char*)v_out.c_str());
        cout << "size of vector: " << v.size() << endl;

        delete csrm_ptr;
        i++;
    }    

    free(csrms);

    return 0;
}