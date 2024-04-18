/*
    Compile:    g++ -g -Wall -fopenmp -pthread benchmark_testing.cpp csr_matrix.cpp vector.cpp -o test
    Run:        ./test
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>
#include <string>
#include "timer.h"

int main() {
    vector<string> files({
        "G9.mtx",
        "Goodwin_013.mtx",
        "abtaha1.mtx",
        "adder_dcop_66.mtx",
        "barth5.mtx",
        "blckhole.mtx",
        "brainpc2.mtx",
        "jnlbrng1.mtx",
        "lp_qap12.mtx",
        "t60k.mtx"
    });

    vector<double> v;
    string file;
    string v_out;
    string out;
    long thread_count;

    int num_rows, num_cols, num_nonzeros;
    double start, finish, elapsed;

    ofstream csv_file("test_outputs/test_results.csv");
    csv_file << "SPMV Performance\n";
    csv_file << "Matrix,Rows,Columns,NNZ,NNZ/Rows,SPMV Function,Thread Count,Time\n";
    // there should be 8 columns in each entry

    for (auto e: files) {
        file = "mtxfiles/" + e;
        CSRMatrix* csrm_ptr = csr_matrix_create((char*)file.c_str());
        num_rows = csrm_ptr->num_rows;
        num_cols = csrm_ptr->num_cols;
        num_nonzeros = csrm_ptr->num_nonzeros;

        v_out = "vectors/" + file + ".vector.txt";
        v = CreateRandomVector(num_cols, (char*)v_out.c_str());

        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        
        out = "test_outputs/" + e + "_serial_output.txt";
        GET_TIME(start);
        csr_serial_spmv(csrm_ptr, v, (char*)out.c_str());
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << "csr_serial_spmv" << "," << 1 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_omp_output.txt";
        GET_TIME(start);
        csr_omp_spmv(csrm_ptr, v, (char*)out.c_str(), 2);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_omp_spmv" << "," << 2 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_omp_output.txt";
        GET_TIME(start);
        csr_omp_spmv(csrm_ptr, v, (char*)out.c_str(), 4);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_omp_spmv" << "," << 4 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_omp_output.txt";
        GET_TIME(start);
        csr_omp_spmv(csrm_ptr, v, (char*)out.c_str(), 6);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_omp_spmv" << "," << 6 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_pth_output.txt";
        GET_TIME(start);
        csr_pth_spmv(csrm_ptr, v, (char*)out.c_str(), 2);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_pth_spmv" << "," << 2 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_pth_output.txt";
        GET_TIME(start);
        csr_pth_spmv(csrm_ptr, v, (char*)out.c_str(), 4);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_pth_spmv" << "," << 4 << "," << elapsed << "\n";

        out = "test_outputs/" + e + "_pth_output.txt";
        GET_TIME(start);
        csr_pth_spmv(csrm_ptr, v, (char*)out.c_str(), 4);
        GET_TIME(finish);
        elapsed = finish - start;
        csv_file << e << "," << num_rows << "," << num_cols << ",";
        csv_file << num_nonzeros << "," << (double)num_nonzeros / num_rows << ",";
        csv_file << "csr_pth_spmv" << "," << 4 << "," << elapsed << "\n";
        
        
    }

    csv_file.close();
    delete[] csrm_ptr;

    return 0;
}