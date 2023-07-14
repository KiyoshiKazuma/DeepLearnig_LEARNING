#ifndef H_MATRIX
#define H_MATRIX

typedef struct {
    double* elep;
    unsigned int row;
    unsigned int column;
}S_MATRIX;

#define SIZE(x) (x.row*x.column)

int F_PRINT(S_MATRIX* mat);
int F_ADD(S_MATRIX* a, S_MATRIX* b, S_MATRIX* ret);
int F_PRODUCT(S_MATRIX* a, S_MATRIX* b, S_MATRIX* ret);
int F_ELEMENT_NUM(unsigned int i,unsigned  int j, S_MATRIX* mat);
int F_CREATE_MATRIX(unsigned int i, unsigned int j, S_MATRIX* mat);
int F_DELETE_MATRIX(S_MATRIX* mat);
int F_FUNCTION(S_MATRIX* in, S_MATRIX* out, double (*pfunc) (double));
int F_SOFTMAX(S_MATRIX* in,S_MATRIX * out);
#endif