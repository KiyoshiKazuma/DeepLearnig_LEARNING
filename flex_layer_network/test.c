#include <stdio.h>
#include <stdlib.h>
#include "layer.h"


int test_create_layer(int type,unsigned int input_size, unsigned int output_size);
int test_delete_layer(H_LAYER hLayer);
int test_print_layer(H_LAYER hLayer);
int test_calc_forword(H_LAYER hLayer,double * vInput);
int test_calc_backword(H_LAYER hLayer,double * vInput);
int test_main(void){
    H_LAYER layer1=NULL;
    layer1=create_layer(LT_Sigmoid,1,2);
    print_layer(layer1);

    return 0;
}