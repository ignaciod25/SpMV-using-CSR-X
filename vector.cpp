#include <random>
#include <ctime>
#include "sparse_matrices.h"

using namespace std;

// functions to initialize a vector
Vector* GenerateRandVect(int size);
Vector* CreateVectorFromFile(char* filename);
// write to vector to file
void    WriteVectorToFile(Vector* v, char* filename);
// function to free memory allocated for vector
void    free_vector(Vector* v);
