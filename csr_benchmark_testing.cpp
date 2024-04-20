/*
    Compile:    g++ -g -Wall -fopenmp -pthread csr_benchmark_testing.cpp csr_matrix.cpp vector.cpp -o test
    Run:        ./test <csv file for results> <list of mtxfile names>
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>
#include "timer.h"

int main(int argc, char* arv[]) {
    if (argc != 3) {
        cerr << "Usage: ./test <csv file for results> <list of mtxfile names>\n";
        exit(0);
    }

    vector<string> files({
        "494_bus.mtx",
        "G9.mtx",
        "Goodwin_013.mtx",
        "abtaha1.mtx",
        "adder_dcop_66.mtx",
        "bp_800.mtx", 
        "brainpc2.mtx", 
        "coater1.mtx",
        "lp_qap12.mtx", 
        "t60k.mtx"
    });

    vector<double> v;
    CSRMatrix** csrms = (CSRMatrix**)malloc(sizeof(CSRMatrix) * files.size());
    CSRMatrix* csrm_ptr;
    string file;
    string v_file;
    string out;
    long thread_count;

    int num_rows, num_cols, num_nonzeros;
    double start, finish, elapsed;

    ofstream csv_file("test_outputs/test_results.csv");
    csv_file << "Matrix,Rows,Columns,NNZ,NNZ/Rows,SPMV Function,Thread Count,Time\n";
    // there should be 8 columns in each entry

    int i=0;
    for (auto e: files) {
        csrm_ptr = csrms[i];
        file = "mtxfiles/" + e;
        csrm_ptr = csr_matrix_create((char*)file.c_str());
        num_rows = csrm_ptr->num_rows;
        num_cols = csrm_ptr->num_cols;
        num_nonzeros = csrm_ptr->num_nonzeros;

        v_file = "vectors/" + e + ".vector.txt";
        v = CreateVectorFromFile((char*)v_file.c_str());

        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        
        out = "test_outputs/" + e + "_csr_serial_output.txt";
        GET_TIME(start);
        csr_serial_spmv(csrm_ptr, v, (char*)out.c_str());
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << "csr_serial_spmv" << "," << 1 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_csr_omp_output.txt";
        GET_TIME(start);
        csr_omp_spmv(csrm_ptr, v, (char*)out.c_str(), 2);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_omp_spmv" << "," << 2 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_csr_omp_output.txt";
        GET_TIME(start);
        csr_omp_spmv(csrm_ptr, v, (char*)out.c_str(), 4);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_omp_spmv" << "," << 4 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_csr_omp_output.txt";
        GET_TIME(start);
        csr_omp_spmv(csrm_ptr, v, (char*)out.c_str(), 6);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_omp_spmv" << "," << 6 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_csr_pth_output.txt";
        GET_TIME(start);
        csr_pth_spmv(csrm_ptr, v, (char*)out.c_str(), 2);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_pth_spmv" << "," << 2 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_csr_pth_output.txt";
        GET_TIME(start);
        csr_pth_spmv(csrm_ptr, v, (char*)out.c_str(), 4);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_pth_spmv" << "," << 4 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_csr_pth_output.txt";
        GET_TIME(start);
        csr_pth_spmv(csrm_ptr, v, (char*)out.c_str(), 4);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_pth_spmv" << "," << 4 << "," << elapsed << "\n";
        
        csrm_ptr->row_ptr.clear();
        csrm_ptr->col_idx.clear();
        csrm_ptr->values.clear();
        delete csrm_ptr;
        i++;
        
        cout << "-----" << e << endl;
    }

    csv_file.close();
    free(csrms);

    cout << "Done with benchmark testing for csr formatted matrices\n";

    return 0;
}