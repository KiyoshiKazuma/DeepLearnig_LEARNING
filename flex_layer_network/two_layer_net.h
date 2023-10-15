#ifndef H_TWO_LAYER_NET
#define H_TWO_LAYER_NET

#include"matrix.h"


int two_layer_net(S_MATRIX * input,S_MATRIX * output,S_MATRIX *W,S_MATRIX *B);
int calc_size_net(S_MATRIX* W,S_MATRIX*B);
#endif //H_TWO_LAYER_NET