#ifndef H_GRADIENT_DESCENT
#define H_GRADIENT_DESCENT

#include "matrix.h"

int aggregate_network_data(S_MATRIX* W,S_MATRIX* B,double **pnet_value);
int gradient_descent(S_MATRIX *W,S_MATRIX *B,S_MATRIX* X,S_MATRIX *T,int learning_size);

#endif // H_GRADIENT_DESCENT