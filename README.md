# SpMV-using-CSR-and-ELL

File: serial_spmv_a4.cpp
+ Compile:
    - g++ -g -Wall -fopenmp -pthread serial_spmv_a4.cpp csr_matrix.cpp vector.cpp -o serial_test
+ Run: 
    - ./serial_test <\matrix format> <\matrix file> <\vector file> <\output file>
    - Matrix Format: either 'csr' or 'ell'


File: parallel_spmv_a4.cpp
+ Compile: 
    - g++ -g -Wall -fopenmp -pthread parallel_spmv_a4.cpp csr_matrix.cpp vector.cpp -o parallel_test
+ Run: 
    - ./parallel_test <\matrix format> <\parallelization method> <\num threads> <\matrix file> <\vector file> <\output file>
    - Matrix Format: either 'csr' or 'ell'
    - Parallelization Method: either 'omp' or 'pth'


File: csr_benchmark_testing.cpp
+ Compile:    
    - g++ -g -Wall -fopenmp -pthread csr_benchmark_testing.cpp csr_matrix.cpp vector.cpp -o test 
+ Run:        
    - ./test <\csv file for results> <\list of matrix file names>
+ Uses established mtxfiles and compatible vectors in the mtxfiles/ and the vectors/ directories


File: vector_main.cpp
+ Compile:    
    - g++ -g -Wall vector.cpp -fopenmp -pthread csr_matrix.cpp vector_main.cpp -o v_test
+ Run:        
    - ./v_test 
+ Manually add list of mtxfiles to the vector in the main function
