#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

H_LAYER create_layer(int id,int input_size, int output_size){
    S_LAYER * pLayer=NULL;
    pLayer = (S_LAYER * )malloc(sizeof(S_LAYER));
    if(pLayer==NULL){
        return NULL;
    }
    pLayer->id=id;
    pLayer->input_size=input_size;
    pLayer->output_size=output_size;
    return pLayer;
}

int PrintLayer(H_LAYER hLayer){
    S_LAYER * pLayer = (S_LAYER *)hLayer;
    printf("id : %d\n",pLayer->id);
    printf("input size : %d\n",pLayer->input_size);
    printf("output size : %d\n",pLayer->output_size);
    return 0;
}


int SetMatrix2Layer(H_LAYER hLayer){
    S_LAYER * pLayer = (S_LAYER *)hLayer;
    S_MATRIX * pMatrix=NULL;
    
    
    return 0;
}