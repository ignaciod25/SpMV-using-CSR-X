#include <random>
#include <ctime>
#include "sparse_matrices.h"

using namespace std;
/* vector functions in vector.cpp */
// functions to initialize a vector
vector<int> GenerateRandVect(int size);
vector<int> CreateVectorFromFile(char* filename);
// write to vector to file
void WriteVectorToFile(vector<int> v, char* filename);
