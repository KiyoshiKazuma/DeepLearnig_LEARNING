#include "network.h"
#include <stdio.h>
#include <stdlib.h>

H_LAYER get_layer(H_NETWORK hNetwork, unsigned int num)
{
    if (hNetwork == NULL)
    {
        return NULL;
    }
    H_LIST hList = list_network(hNetwork);
    S_LIST_ELEM *pListElem = get_list(hList, num);
    return pListElem->pElem;
}

H_NETWORK create_network(unsigned int input_size, unsigned int output_size)
{
    // size check
    if (input_size == 0 || output_size == 0)
    {
        return NULL;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)malloc(sizeof(S_NETWORK));
    H_LIST hList = create_list();

    pNetwork->input_size = input_size;
    pNetwork->output_size = output_size;
    pNetwork->size = 0;
    pNetwork->hList_Layers = hList;

    return pNetwork;
}
int delete_network(H_NETWORK hNetwork)
{
    // NULL CHECK
    if (hNetwork == NULL)
    {
        return 1;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    H_LIST hList = list_network(hNetwork);
    H_LAYER hLayer = NULL;
    unsigned int size = size_network(hNetwork);

    for (unsigned int i = 1; i <= size; i++)
    {
        hLayer = get_layer(hNetwork, i);
        delete_layer(hLayer);
    }

    delete_list(hList);

    free(pNetwork);
    return 0;
}

int add_network(H_NETWORK hNetwork, enum LayerType type, unsigned int IF_size)
{
    if (hNetwork == NULL)
    {
        return 1;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    H_LAYER hLayerNew = NULL;
    H_LAYER hLayerLast = NULL;
    H_LIST hListLayer = NULL;
    unsigned int input_size_new;
    unsigned int output_size_new;

    hListLayer = list_network(hNetwork);

    // set input size of new layer
    if (pNetwork->size == 0)
    { // for first layer
        input_size_new = pNetwork->input_size;
    }
    else
    { // for another layer
        hLayerLast = get_layer(hNetwork, pNetwork->size);
        input_size_new = output_size_layer(hLayerLast);
    }

    // allocare new layer
    switch (type)
    {
    case LT_ReLU:
    case LT_Sigmoid:
    case LT_Softmax:
        hLayerNew = create_layer(type, input_size_new, input_size_new);
        break;
    case LT_Affine:
        hLayerNew = create_layer(type, input_size_new, IF_size);
        break;
    case LT_SoftmaxWithLoss:
        if (input_size_new == pNetwork->output_size)
        {
            hLayerNew = create_layer(type, input_size_new, 1);
        }
        else
        {
            return 1;
        }
        break;
    default: // not defined
        return 1;
        break;
    }

    // append new layer
    append_list(hListLayer, hLayerNew);
    pNetwork->size++;

    return 0;
}

int print_network(H_NETWORK hNetwork)
{
    if (hNetwork == NULL)
    {
        return 1;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    H_LAYER hLayer = NULL;
    unsigned int size = size_network(hNetwork);

    printf("=====================\n");
    printf("PRINT NETWORK\n");
    printf(">LENGETH : %u\n", size);
    printf(">INPUT SIZE : %u\n", input_size_network(hNetwork));
    printf(">OUTPUT SIZE : %u\n", output_size_network(hNetwork));
    printf(">LAYERS\n");

    for (unsigned int i = 1; i <= size; i++)
    {
        hLayer = get_layer(hNetwork, i);
        switch (type_layer(hLayer))
        {
        case LT_Affine:
            printf(" AffineLayer %u:%u\n", input_size_layer(hLayer), output_size_layer(hLayer));
            break;
        case LT_Sigmoid:
            printf(" SigmoidLayer %u:%u\n", input_size_layer(hLayer), output_size_layer(hLayer));
            break;
        case LT_ReLU:
            printf(" ReLULayer %u:%u\n", input_size_layer(hLayer), output_size_layer(hLayer));
            break;
        case LT_SoftmaxWithLoss:
            printf(" SoftmaxWithLoss %u:%u\n", input_size_layer(hLayer), output_size_layer(hLayer));
            break;
        default:
            printf(" ND Layer\n");
            break;
        }
    }
}

int calc_network(H_NETWORK hNetwork, H_MATRIX hInput, H_MATRIX hOutput, double *pLoss)
{
    // NULL CHECK
    if (hNetwork == NULL || hInput == NULL || hOutput == NULL || pLoss == NULL)
    {
        return 1;
    }
    // size check
    if (input_size_network(hNetwork) != row_size_matrix(hInput) || output_size_network(hNetwork) != row_size_matrix(hOutput) || column_size_matrix(hInput) != 1 || column_size_matrix(hInput) != 1){
        return 1;
    }

    H_LAYER hLayer = NULL;
    H_MATRIX hInputTmp = NULL;
    H_MATRIX hOutputTmp = NULL;
    H_MATRIX hY=NULL;
    int ret = 0;
    int result = 0;
    unsigned int length = size_network(hNetwork);

    hInputTmp = hInput;

    for (unsigned int i = 1; i <= length; i++)
    {
        hLayer = get_layer(hNetwork, i);
        ret = calc_forword(hLayer, hInputTmp);
        if (ret)
        {
            return 1;
        }
        hOutputTmp = PointerForwardOutput(hLayer);
        hInputTmp = hOutputTmp;
    }
    ret=copy_matrix(hOutputTmp,hOutput); 
    hY=PointerLayerParameter(hLayer,0);
    *pLoss = element_value_matrix(hY,0,0);

    return 0;
}
int backpropagation_network(H_NETWORK hNetwork)
{
    return 0;
}

unsigned int size_network(H_NETWORK hNetwork)
{
    if (hNetwork == NULL)
    {
        return 0;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    return pNetwork->size;
}

unsigned int input_size_network(H_NETWORK hNetwork)
{
    // NULL CHECK
    if (hNetwork == NULL)
    {
        return 0;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    return pNetwork->input_size;
}
unsigned int output_size_network(H_NETWORK hNetwork)
{
    // NULL CHECK
    if (hNetwork == NULL)
    {
        return 0;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    return pNetwork->output_size;
}
H_LIST list_network(H_NETWORK hNetwork)
{
    // NULL CHECK
    if (hNetwork == NULL)
    {
        return NULL;
    }
    S_NETWORK *pNetwork = (S_NETWORK *)hNetwork;
    return pNetwork->hList_Layers;
}

unsigned int row_size_matrix(H_MATRIX hMatrix)
{
    if (hMatrix == NULL)
    {
        return 0;
    }
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    return pMatrix->row;
}
unsigned int column_size_matrix(H_MATRIX hMatrix)
{
    if (hMatrix == NULL)
    {
        return 0;
    }
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    return pMatrix->column;
}

unsigned int size_matrix(H_MATRIX hMatrix)
{
    if (hMatrix == NULL)
    {
        return 0;
    }
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    return pMatrix->size;
}