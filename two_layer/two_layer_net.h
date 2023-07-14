#ifndef H_TWO_LAYER_NET
#define H_TWO_LAYER_NET

#include"matrix.h"

#define INPUT_SIZE (2)
#define HIDEN_SIZE (3)
#define OUTPUT_SIZE (2)

int two_layer_net(S_MATRIX * input,S_MATRIX * output,S_MATRIX *W,S_MATRIX *B);
int calc_size_net(S_MATRIX* W,S_MATRIX*B);
#endif //H_TWO_LAYER_NET