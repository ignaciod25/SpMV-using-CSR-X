/*

This code implements sparse matrix-vector multiplication using two formats: 
Compressed Sparse Row (CSR) and Ellpack-Itpack (ELL). 
It reads a sparse matrix and a vector from files, performs the multiplication, and writes the result to an output file. 
Additionally, it supports parallel execution using OpenMP for improved performance when processing large datasets.

To compile the code, run:
g++ ELL_main.cpp -o ELL_exec -fopenmp -pthread

To run omp:
./ELL_exec CSR omp 4 matrix.mtx vector.txt output_ELL_omp.txt

To run serial:
./ELL_exec CSR serial 1 matrix.mtx vector.txt output_ELL_serial.txt
*/

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <algorithm>
#include <omp.h>
#include <pthread.h>
#include <chrono>

struct Vector
{
    std::vector<double> elements;

    Vector(size_t size) : elements(size) {}
    Vector() = default;
    double& operator[](size_t index)
    {
        return elements[index];
    }
    const double& operator[](size_t index) const
    {
        return elements[index];
    }
    size_t size() const
    {
        return elements.size();
    }
    void resize(size_t newSize)
    {
        elements.resize(newSize);
    }
};

struct CSRMatrix;
struct SparseMatrix
{
    virtual ~SparseMatrix() {}
    virtual void multiply(const Vector& vec, Vector& result) const = 0;
};

struct CSRMatrix : SparseMatrix
{
    std::vector<double> values;
    std::vector<int> rowPointers;
    std::vector<int> colIndices;
    int numRows;

    int getMaxRowSize() const
    {
        int maxRowSize = 0;
        for (int i = 0; i < numRows; ++i)
        {
            int rowSize = 0;
            for (int j = 0; j < maxRowSize; j++)
            {
                int index = i * maxRowSize + j;
                if (colIndices[index] != -1)
                {
                    rowSize++;
                }
            }
            if (rowSize > maxRowSize)
            {
                maxRowSize = rowSize;
            }
        }
        return maxRowSize;
    }

    void addValue(double value, int rowIndex, int colIndex)
    {
        if (rowPointers.size() <= rowIndex)
            rowPointers.resize(rowIndex + 1);
        if (rowPointers[rowIndex] == 0 || values.empty())
        {
            rowPointers[rowIndex] = values.size();
        }
        values.push_back(value);
        colIndices.push_back(colIndex);
    }

    void finalize()
    {
        if (rowPointers.size() <= numRows)
            rowPointers.resize(numRows + 1);
        rowPointers[numRows] = values.size();
    }

    void multiply(const Vector& vec, Vector& result) const override
    {
        for (int i = 0; i < numRows; ++i)
        {
            double sum = 0.0;
            for (int j = rowPointers[i]; j < rowPointers[i + 1]; ++j)
            {
                sum += values[j] * vec.elements[colIndices[j]];
            }
            result.elements[i] = sum;
        }
    }
};

struct ELLMatrix : SparseMatrix
{
    std::vector<double> values;
    std::vector<int> colIndices;
    std::vector<int> rowSizes;
    int numRows;
    int maxRowSize;

    int getMaxRowSize() const
    {
        int maxRowSize = 0;
        for (int i = 0; i < numRows; ++i)
        {
            int rowSize = 0;
            for (int j = 0; j < maxRowSize; ++j)
            {
                int index = i * maxRowSize + j;
                if (index < numRows * maxRowSize && colIndices[index] != -1)
                {
                    rowSize++;
                }
            }
            if (rowSize > maxRowSize)
            {
                maxRowSize = rowSize;
            }
        }
        return maxRowSize;
    }

    ELLMatrix(int nRows, int maxRSize) : numRows(nRows), maxRowSize(maxRSize)
    {
    values.resize(numRows * maxRowSize);
    colIndices.resize(numRows * maxRowSize);
    rowSizes.resize(numRows, 0);
    std::fill(values.begin(), values.end(), 0);
    std::fill(colIndices.begin(), colIndices.end(), -1);
    }

    void addValue(int row, int col, double val, int position)
    {
        int index = row * maxRowSize + position;
        values[index] = val;
        colIndices[index] = col;
        if (position >= rowSizes[row]) {
            rowSizes[row] = position + 1;
        }
    }

    void multiply(const Vector& vec, Vector& result) const override
    {
        std::fill(result.elements.begin(), result.elements.end(), 0.0);
        for (int i = 0; i < numRows; ++i)
        {
            double sum = 0.0;
            for (int j = 0; j < maxRowSize; ++j)
            {
                int index = i * maxRowSize + j;
                if (j < rowSizes[i])
                {
                    std::cout << "Index: " << index << ", Col Index: " << colIndices[index] << ", Value: " << values[index] << ", Sum: " << sum << std::endl;
                    sum += values[index] * vec.elements[colIndices[index]];
                }
            }
            result.elements[i] = sum;
        }
    }
};

struct ParallelCSRMatrix : CSRMatrix
{
    void multiply(const Vector& vec, Vector& result) const override
    {
        #pragma omp parallel for
        for (int i = 0; i < numRows; ++i)
        {
            double sum = 0.0;
            for (int j = 0; j < getMaxRowSize(); ++j)
            {
                int index = i * getMaxRowSize() + j;
                if (colIndices[index] != -1)
                {
                    std::cout << "Index: " << index << ", Col Index: " << colIndices[index] << ", Value: " << values[index] << ", Sum: " << sum << std::endl;
                    sum += values[index] * vec.elements[colIndices[index]];
                }
            }
            result.elements[i] = sum;
        }
    }
};

struct ParallelELLMatrix : ELLMatrix
{
    std::vector<int> rowSizes;

    void multiply(const Vector& vec, Vector& result) const override
    {
        for (int i = 0; i < numRows; ++i)
        {
            double sum = 0.0;
            for (int j = 0; j < maxRowSize; ++j)
            {
                int index = i * maxRowSize + j;
                if (j < rowSizes[i])
                {   
                    std::cout << "Index: " << index << ", Col Index: " << colIndices[index] << ", Value: " << values[index] << ", Sum: " << sum << std::endl;
                    sum += values[index] * vec.elements[colIndices[index]];
                }
            }
            result.elements[i] = sum;
        }
    }
};

struct MMIO
{
    static SparseMatrix* readMatrix(const std::string& filename, const std::string& format)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file");
        }
        int rows, cols, nonzeros;
        std::string line;
        while (std::getline(file, line))
        {
            if (line[0] != '%') break;
        }
        std::istringstream iss(line);
        iss >> rows >> cols >> nonzeros;
        SparseMatrix* matrix = nullptr;
        if (format == "CSR")
        {
            matrix = new CSRMatrix();
            static_cast<CSRMatrix*>(matrix)->numRows = rows;
        }
        else if (format == "ELL")
        {
            matrix = new ELLMatrix(rows, cols);
        }
        else
        {
            throw std::runtime_error("Unsupported format");
        }
        int row, col;
        double value;
        while (file >> row >> col >> value)
        {
            if (format == "CSR")
            {
                static_cast<CSRMatrix*>(matrix)->addValue(value, row - 1, col - 1);
            }
            else if (format == "ELL")
            {
                static_cast<ELLMatrix*>(matrix)->addValue(row - 1, col - 1, value, 0);
            }
        }
        if (format == "CSR")
        {
            static_cast<CSRMatrix*>(matrix)->finalize();
        }
        return matrix;
    }

    static Vector readVector(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file");
        }
        Vector vector;
        std::string line;
        double value;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '%') continue;
            std::istringstream iss(line);
            while (iss >> value)
            {
                vector.elements.push_back(value);
            }
        }
        return vector;
    }

    static void writeVector(const Vector& vec, const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file");
        }
        for (size_t i = 0; i < vec.size(); ++i)
        {
            file << vec.elements[i] << std::endl;
        }
    }
};

int main(int argc, char** argv)
{
    if (argc != 7)
    {
        std::cerr << "Usage: " << argv[0] << " <matrix format> <parallel method> <number of threads> <matrix file> <vector file> <output file>\n";
        return 1;
    }
    std::string matrixFormat = argv[1];
    std::string parallelMethod = argv[2];
    int numThreads = std::stoi(argv[3]);
    std::string matrixFile = argv[4];
    std::string vectorFile = argv[5];
    std::string outputFile = argv[6];
    omp_set_num_threads(numThreads);
    SparseMatrix* matrix;
    if (matrixFormat == "CSR")
    {
        matrix = MMIO::readMatrix(matrixFile, "CSR");
    }
    else if (matrixFormat == "ELL")
    {
        matrix = MMIO::readMatrix(matrixFile, "ELL");
    }
    else
    {
        std::cerr << "Unsupported matrix format.\n";
        return 1;
    }
    Vector vec = MMIO::readVector(vectorFile);
    Vector result(vec.size());
    matrix->multiply(vec, result);
    MMIO::writeVector(result, outputFile);
    delete matrix;
    return 0;
}
