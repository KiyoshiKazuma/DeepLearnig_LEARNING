#ifndef D_LAYER
#define D_LAYER

#include "matrix.h"
typedef struct{
    int id;
    int input_size;
    int output_size;
    H_MATRIX * hMat;
    int next_layer_id;
}S_LAYER;

typedef void * H_LAYER;

H_LAYER create_layer(int id,int input_size, int output_size);
int PrintLayer(H_LAYER hLayer);

#endif //D_LAYER