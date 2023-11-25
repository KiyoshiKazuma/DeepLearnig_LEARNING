#ifndef H_GRADIENT_DESCENT
#define H_GRADIENT_DESCENT

#include "matrix.h"

int aggregate_network_data(S_NETWORK net,S_MATRIX vW[],S_MATRIX vB[],double **pnet_value);
int gradient_descent(S_NETWORK net,S_MATRIX *vX,S_MATRIX *vT, S_MATRIX * vW,S_MATRIX * vB,int learning_size);

#endif // H_GRADIENT_DESCENT