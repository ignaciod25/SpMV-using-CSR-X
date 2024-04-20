#include "sparse_matrices.h"

/* Global Variable for pthread functions */
long num_threads;
ELLMatrix* ellm_global_ptr;
vector<double>* vp;
vector<double> result;
int tasks_per_thread;
int leftovers;
// pthread_mutex_t* mutexes;

/* List of functions */
ELLMatrix* ell_matrix_create(char* filename); // create matrix from mtx file, will need to delete matrix ptr after creation
void ell_serial_spmv(ELLMatrix* ellm, vector<double> v, char* output);
void ell_omp_spmv(ELLMatrix* ellm, vector<double> v, char* output, long thread_count);
void* ThreadRoutine(void* rank);
void ell_pth_spmv(ELLMatrix* ellm, vector<double> v, char* output, long thread_count);

ELLMatrix* ell_matrix_create(char* filename) {
    ELLMatrix* ellm;
    string banner;
    int total_rows, total_cols, entries, max_row_size;
    int row, col, i, idx;
    double val;

    ifstream fin(filename);
    getline(fin, banner);

    #ifdef DEBUG
    cout << banner << endl;
    #endif

    while (banner.find("%") != string::npos) getline(fin, banner);
    stringstream ss(banner);
    string rows, cols, ent;
    ss >> rows >> cols >> ent;
    total_rows = atoi(rows.c_str());
    total_cols = atoi(cols.c_str());
    entries = atoi(ent.c_str());

    #ifdef DEBUG
    printf("rows: %d, cols: %d, entries: %d\n", total_rows, total_cols, entries);
    #endif 

    vector<int> row_num_values; 
    row_num_values.reserve(total_rows);
    row_num_values.assign(total_rows, 0);

    ellm = new ELLMatrix;
    ellm->num_rows      = total_rows;
    ellm->num_cols      = total_cols;
    ellm->num_nonzeros  = entries;

    max_row_size = 0;

    i = 0;
    while (i < entries) {
        fin >> row >> col >> val;
        row--;
        col--;
        
        row_num_values[row]++;
        if (row_num_values[row] > max_row_size) {
            max_row_size++;
            for (int r=0; r<total_rows; r++) {
                ellm->col_idx.insert(ellm->col_idx.begin() + r*max_row_size-1, -1);
                ellm->values.insert(ellm->values.begin() + r*max_row_size-1, 0);
            }
            idx = row * max_row_size;
            idx--;
            #ifdef DEBUG
            cout << "resized, array size = " << ellm->values.size() << endl << endl;
            #endif
            ellm->col_idx[idx] = col;
            ellm->values[idx] = val;
        } else {
            idx = (row+1) * max_row_size;
            idx -= max_row_size - (row_num_values[row]);
            ellm->col_idx[idx] = col;
            ellm->values[idx] = val;
        }

        #ifdef DEBUG
        cout << "row: " << row << endl;
        cout << "col: " << col << endl;
        cout << "val: " << val << endl;
        cout << "idx: " << idx << endl << endl;
        #endif

        i++;
    }
    fin.close();

    ellm->MaxRowSize = max_row_size;

    row_num_values.clear();
    return ellm;
} /* ELLMatrix* ell_matrix_create */
