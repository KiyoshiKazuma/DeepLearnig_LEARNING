#ifndef H_TWO_LAYER_NET
#define H_TWO_LAYER_NET

#include"matrix.h"
#include"configuration.h"

int neural_network(S_NETWORK net,S_MATRIX X,S_MATRIX *pY,S_MATRIX *vW,S_MATRIX *vB);
int calc_size_net(S_MATRIX* W,S_MATRIX*B);
#endif //H_TWO_LAYER_NET