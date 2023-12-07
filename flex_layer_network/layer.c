#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

H_LAYER create_layer(int type,unsigned int input_size,unsigned int output_size)
{
    int error=0;
    S_LAYER *pLayer = NULL;
    H_MATRIX hMatrix=NULL;
    pLayer = (S_LAYER *)malloc(sizeof(S_LAYER));
    if (pLayer == NULL)
    {
        return NULL;
    }
    pLayer->type = type;
    pLayer->input_size = input_size;
    pLayer->output_size = output_size;
    if (input_size < 1 || output_size < 1)
    {
        return NULL;
    }

    switch (pLayer->type)
    {
    case LT_ReLU:
        if (pLayer->input_size != pLayer->output_size)
        {
            pLayer->pLayerParam = NULL;
            pLayer->pBackwardOutput=(void *)malloc(sizeof(double)*pLayer->input_size);
            pLayer->pForwardOutput=(void *)malloc(sizeof(double)*pLayer->output_size);
        }
        else
        {
            error=1;
        }
        break;
    case LT_Sigmoid:
        if (pLayer->input_size != pLayer->output_size)
        {
            pLayer->hMat = NULL;
        }
        else
        {
            error=2;
        }
        break;
    case LT_Affine:
        hMatrix=create_matrix(pLayer->input_size,pLayer->output_size);
        if(hMatrix==NULL){
            error=3;
        }
        pLayer->hMat=hMatrix;
    case LT_Softmax:2
    case LT_SoftmaxWithLoss:
    }
    if(error!=0){
        free(pLayer);
        return NULL;
    }
    return pLayer;
}

int PrintLayer(H_LAYER hLayer)
{
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    printf("id : %d\n", pLayer->id);
    printf("input size : %d\n", pLayer->input_size);
    printf("output size : %d\n", pLayer->output_size);
    return 0;
}

int SetMatrix2Layer(H_LAYER hLayer)
{
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    S_MATRIX *pMatrix = NULL;

    return 0;
}
int delete_layer(H_LAYER hLayer);
int print_layer(H_LAYER hLayer);
int calc_forword(H_LAYER hLayer, void *pInput);
int calc_backword(H_LAYER hLayer, void *pInput);
int update_params(H_LAYER hlayer);