/* 
    Compile:    g++ -g -Wall vector.cpp vector_main.cpp -o v_test
    Run:        ./v_test 
*/

#include "sparse_matrices.h"
#include <iostream>
#include <fstream>

int main() {

    vector<double> v = CreateRandomVector(7, "vector7");
    cout << "size of vector: " << v.size() << endl;

    for (auto i: v) cout << i << endl;

    return 0;
}