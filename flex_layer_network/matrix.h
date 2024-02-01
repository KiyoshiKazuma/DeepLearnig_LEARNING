#ifndef D_MATRIX
#define D_MATRIX

typedef struct {
    double* pElem;
    unsigned int row;
    unsigned int column;
    unsigned int size;
}S_MATRIX;

typedef void * H_MATRIX;

H_MATRIX create_matrix(unsigned int row_size, unsigned int column_size);
int delete_matrix(H_MATRIX hMatrix);
int add_matrix(H_MATRIX hMatix_IN1, H_MATRIX hMatix_IN2, H_MATRIX hMatix_OUT);
int product_matrix(H_MATRIX hMatix_IN1, H_MATRIX hMatix_IN2, H_MATRIX hMatix_OUT);
H_MATRIX transpose_matrix(H_MATRIX hMatrix_IN);
int print_matrix(H_MATRIX hMatrix);
int copy_matrix(H_MATRIX hMatix_IN,H_MATRIX hMatix_OUT);

double element_value_matrix(H_MATRIX hMatrix,unsigned int i,unsigned int j);
unsigned int element_num_matrix(H_MATRIX hMatrix,unsigned int i,unsigned int j);

unsigned int row_size_matrix(H_MATRIX hMatrix);
unsigned int column_size_matrix(H_MATRIX hMatrix);
unsigned int size_matrix(H_MATRIX hMatrix);

#endif //D_MATRIX