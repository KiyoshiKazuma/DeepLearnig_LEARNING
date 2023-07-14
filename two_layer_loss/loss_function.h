#ifndef H_LOSS_FUNCTION
#define H_LOSS_FUNCTION

#include "matrix.h"

double mean_squared_error(S_MATRIX* Y,S_MATRIX* T);
double cross_entropy_error(S_MATRIX* Y,S_MATRIX* T);

#endif