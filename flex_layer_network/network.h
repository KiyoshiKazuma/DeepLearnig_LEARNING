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

#endif //D_NETWORK