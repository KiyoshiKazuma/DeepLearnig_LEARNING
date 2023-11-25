#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

int main(void){
    H_LAYER layer1=NULL;
    layer1=create_layer(1,2,2);
    PrintLayer(layer1);

    return 0;

}