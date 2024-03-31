#ifndef D_NETWORK
#define D_NETWORK

#include "layer.h"
#include "matrix.h"
#include "list.h"

typedef struct 
{
    unsigned int size; //Layer num included in network
    H_LIST hList_Layers; //Array of Pointers to Layers included in network
    unsigned int input_size; //input size of network
    unsigned int output_size;//output size of network
}S_NETWORK;

typedef void * H_NETWORK;

H_NETWORK create_network(unsigned int input_size,unsigned int output_size);
int delete_network(H_NETWORK hNetwork);
int add_network(H_NETWORK hNetwork,enum LayerType type,unsigned int IF_size);
int print_network(H_NETWORK hNetwork);
int calc_network(H_NETWORK hNetwork,H_MATRIX hInput,H_MATRIX hOutput,double * pLoss);
int backpropagation_network(H_NETWORK hNetwork);
unsigned int size_network(H_NETWORK hNetwork);
unsigned int input_size_network(H_NETWORK hNetwork);
unsigned int output_size_network(H_NETWORK hNetwork);
H_LIST list_network(H_NETWORK hNetwork);
H_LAYER get_layer(H_NETWORK hNetwork, unsigned int num);

#endif //D_NETWORK