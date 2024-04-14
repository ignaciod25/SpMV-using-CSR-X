#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

typedef struct CSR {
    // int *row_ptr;       // array of locations (indices) of first nonzero element of each row
    // int *col_idx;       // column index of nonzero elements
    // double *values;     // nonzero elements

    vector<int> row_ptr;
    vector<int> col_idx;
    vector<double> values;
    int num_rows;       // number of rows, num_rows + 1 = length of row_ptr array
    int num_cols;       // number of cols
    int num_nonzeros;   // number of entries, length of col_idx array and values array
} CSRMatrix;

int main(int argc, char* argv[]) {

    // FILE* rptr;
    CSRMatrix* csrm;

    // rptr = fopen(argv[1], "r");
    ifstream fin(argv[1]);

    string banner;
    int total_rows, total_cols, entries;

    getline(fin, banner);
    cout << banner << endl;

    fin >> total_rows >> total_cols >> entries;
    printf("rows: %d\ncols: %d\nentries: %d\n", total_rows, total_cols, entries);

    int row; // row idx
    int col; // col idx
    double val;
    int i = 0;
    // int* row_num_values     = new int[rows+1]; // keeps track of how many nonzero values in each row
    // int* coo_row_indices    = new int[entries];
    // int* coo_col_indices    = new int[entries];
    // double* coo_data        = new double[entries];

    vector<int> row_num_values; 
    row_num_values.reserve(total_rows+1);
    vector<int> coo_row_indices;
    coo_row_indices.reserve(entries);
    vector<int> coo_col_indices;
    coo_col_indices.reserve(entries);
    vector<double> coo_data;
    coo_data.reserve(entries);

    /* Initialize number of values in each row to 0 */
    // for (int k=0; k<rows+1; k++)
    //     row_num_values[k] = 0;
    row_num_values.assign(row+1, 0);
    
    // there's probably a more efficient way lol
    // similar while loop for storing sparse matrices in coo format
    // while (i<entries) {
    //     // fscanf(rptr, "%d %d %lg\n", &row, &col, &val);

    //     fin >> row >> col >> val;

    //     // decrement row and col values because of how mtx files are written
    //     row--;
    //     col--;

    //     coo_row_indices.push_back(row);
    //     coo_col_indices.push_back(col);
    //     coo_data.push_back(val);
    //     // coo_row_indices[i] = row;
    //     // coo_col_indices[i] = col;
    //     // coo_data[i] = val;

    // //    printf("%d %d %lg", row, col, val);

    //     row_num_values[row]++;
    //     i++;
    // }
    // // fclose(rptr);
    // fin.close();

    csrm = new CSRMatrix;
    csrm->num_rows      = total_rows;
    csrm->num_cols      = total_cols;
    csrm->num_nonzeros  = entries;

    csrm->row_ptr.reserve(total_rows+1);
    csrm->row_ptr.assign(total_rows+1, 0);
    /* Initialize all ptrs to col index where first the row's nonzero value is */  
    // int running_idx = 0;
    // for (int k=0; k<rows+1; k++) {
    //     csrm->row_ptr.push_back(running_idx + row_num_values[k]);
    //     running_idx = csrm->row_ptr[k];
    // }

    csrm->col_idx.reserve(entries);
    csrm->values.reserve(entries);

    i=0;
    int num_row_elements_left; // will decrement row_num_values[row] after every entry
    int curr_col_idx_start; // stores val of csrm->row_ptr[row]
    int idx; // stores relevant idx
    // idx = curr_col_idx_start + num_row_elements_left
    while (i<entries) {

        fin >> row >> col >> val;

        // decrement row and col values because of how mtx files are written
        row--;
        col--;

        // coo_row_indices.push_back(row);
        // coo_col_indices.push_back(col);
        // coo_data.push_back(val);

        // row = coo_row_indices[i];
        // col = coo_col_indices[i];
        // val = coo_data[i];
        
        // num_row_elements_left = row_num_values[row];
        // curr_col_idx_start = csrm->row_ptr[row];
        // idx = curr_col_idx_start + num_row_elements_left;

        // csrm->col_idx[idx] = col;
        // csrm->values[idx] = val;

        idx = 0;
        for (int r=row+1; r<total_rows+1; r++)
            csrm->row_ptr[r]++;
        
        idx = csrm->row_ptr[row];
        
        csrm->col_idx.insert(csrm->col_idx.begin() + idx, col);
        csrm->values.insert(csrm->values.begin() + idx, val);
        // row_num_values[row]++;

        i++;
    }
    fin.close();

    // delete[] row_num_values;
    // delete[] coo_row_indices;
    // delete[] coo_col_indices;
    // delete[] coo_data;

    // delete[] csrm->row_ptr;
    // delete[] csrm->col_idx;
    // delete[] csrm->values;

    i=0;
    int j;
    while (i<entries) {
        j=0;
        col = csrm->col_idx[i];
        val = csrm->values[i];
        while (csrm->row_ptr[j] < i) j++;
        row = j;

        printf("row: %d, col: %d, val: %lg\n", row, col, val);
        i++;
    }

    delete csrm;

    return 0;
}