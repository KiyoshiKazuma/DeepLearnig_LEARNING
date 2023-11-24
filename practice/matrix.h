#ifndef D_MATRIX
#define D_MATRIX
#define SIZE(x) (x.row*x.column)

typedef struct {
    double* elep;
    unsigned int row;
    unsigned int column;
}S_MATRIX;

typedef void * H_MATRIX;

H_LAYER create_matrix(unsigned int i, unsigned int j);
int print_matrix(H_MATRIX* mat);
int add_matrix(H_MATRIX* a, H_MATRIX* b, H_MATRIX* ret);
int product_matrix(H_MATRIX* a, H_MATRIX* b, H_MATRIX* ret);
int element_num_matrix(unsigned int i,unsigned  int j, H_MATRIX* mat);
int delete_matrix(H_MATRIX* mat);
#endif //D_MATRIX