#ifndef D_LAYER
#define D_LAYER

#include "matrix.h"

enum LayerType{
	LT_ReLU =1,	
	LT_Sigmoid =2,	
	LT_Affine =3,	
	LT_Softmax =4,	
	LT_SoftmaxWithLoss =5,
};

typedef struct{
    int type;
    unsigned int input_size;
    unsigned int output_size;
    void * pLayerParam;
	H_MATRIX hForwardOutput;
	H_MATRIX hBackwardOutput;
}S_LAYER;

typedef void * H_LAYER;

H_LAYER create_layer(int type,unsigned int input_size, unsigned int output_size);
int delete_layer(H_LAYER hLayer);
int print_layer(H_LAYER hLayer);
void * PointerLayerParameters(H_LAYER hLayer);
H_MATRIX PointerForwardOutput(H_LAYER hLayer);
H_MATRIX PointerBackwardOutput(H_LAYER hLayer);
int calc_forword(H_LAYER hLayer,H_MATRIX hMatrix);
int calc_backword(H_LAYER hLayer,H_MATRIX hMatrix);
#endif //D_LAYER