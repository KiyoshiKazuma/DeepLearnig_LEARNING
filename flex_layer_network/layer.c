#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

H_LAYER create_layer(int type, unsigned int input_size, unsigned int output_size)
{
    // argument check
    if (input_size < 1 || output_size < 1)
    {
        return NULL;
    }
    switch (type)
    {
    // defined types
    LT_ReLU:
    LT_Sigmoid:
    LT_Affine:
        //	LT_Softmax :
    LT_SoftmaxWithLoss:
        break;

    default: // not defined type
        return NULL;
        break;
    }

    int error = 0;

    H_MATRIX hY = NULL;
    H_MATRIX hT = NULL;
    H_MATRIX hW = NULL;
    H_MATRIX hB = NULL;
    H_MATRIX *pParam = NULL;
    S_LAYER *pLayer = (S_LAYER *)malloc(sizeof(S_LAYER));

    if (pLayer == NULL)
    {
        return NULL;
    }

    pLayer->type = type;
    pLayer->input_size = input_size;
    pLayer->output_size = output_size;

    switch (pLayer->type)
    {
    case LT_ReLU:
        if (pLayer->input_size == pLayer->output_size)
        {
            pLayer->pLayerParam = NULL;
            pLayer->pBackwardOutput = (void *)malloc(sizeof(double) * pLayer->input_size);
            pLayer->pForwardOutput = (void *)malloc(sizeof(double) * pLayer->output_size);
            // NULL check
            if (pLayer->pBackwardOutput == NULL || pLayer->pForwardOutput == NULL)
            {
                free(pLayer->pBackwardOutput);
                free(pLayer->pBackwardOutput);
            }
        }
        else
        {
            error = 1;
        }
        break;
    case LT_Sigmoid:
        if (pLayer->input_size == pLayer->output_size)
        {
            pLayer->pLayerParam = NULL;
            pLayer->pBackwardOutput = (void *)malloc(sizeof(double) * pLayer->input_size);
            pLayer->pForwardOutput = (void *)malloc(sizeof(double) * pLayer->output_size);
            // NULL check
            if (pLayer->pBackwardOutput == NULL || pLayer->pForwardOutput == NULL)
            {
                free(pLayer->pBackwardOutput);
                free(pLayer->pBackwardOutput);
            }
        }
        else
        {
            error = 2;
        }
        break;
    case LT_Affine:

        hW = create_matrix(output_size, input_size);
        hB = create_matrix(output_size, 1);
        if (hW == NULL || hB == NULL)
        {
            delete_matrix(hW);
            delete_matrix(hB);
            error = 3;
        }
        pParam = (H_MATRIX *)malloc(sizeof(void *) * 2);
        // NULL check
        if (pParam == NULL)
        {
            free(pParam);
            error = 4;
            break;
        }

        pParam[0] = hW;
        pParam[1] = hB;
        pLayer->pLayerParam = pParam;
        break;
    case LT_SoftmaxWithLoss:
        if (pLayer->output_size == 1)
        {
            hY = create_matrix(input_size, 1);
            hT = create_matrix(input_size, 1);
            if (hY == NULL || hT == NULL)
            {
                delete_matrix(hY);
                delete_matrix(hT);
                error = 4;
            }
            pParam = (H_MATRIX *)malloc(sizeof(void *) * 2);
            // NULL check
            if (pParam == NULL)
            {
                free(pParam);
                error = 5;
                break;
            }
            pParam[0] = hY;
            pParam[1] = hT;
            pLayer->pLayerParam = pParam;
        }
        else
        {
            error = 5;
        }
        break;
    }
    if (error != 0)
    {
        free(pLayer);
        return NULL;
    }
    return pLayer;
}

int print_layer(H_LAYER hLayer)
{
    if (hLayer == NULL)
    {
        return 1;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    printf("type : %u\n", pLayer->type);
    printf("input size : %u\n", pLayer->input_size);
    printf("output size : %u\n", pLayer->output_size);
    return 0;
}

int delete_layer(H_LAYER hLayer)
{
    // NULL CEHCK
    if (hLayer == NULL)
    {
        return 1;
    }
    H_MATRIX *pParam;
    S_LAYER *pLayer = (S_LAYER *)hLayer;

    switch (pLayer->type)
    {
    case LT_ReLU:
    case LT_Sigmoid:
    case LT_Softmax:
        break;
    case LT_Affine:
        pParam = pLayer->pLayerParam;
        delete_matrix((H_MATRIX)pParam[0]);
        delete_matrix((H_MATRIX)pParam[1]);
        free(pParam);
        break;
    case LT_SoftmaxWithLoss:
        pParam = pLayer->pLayerParam;
        delete_matrix((H_MATRIX)pParam[0]);
        delete_matrix((H_MATRIX)pParam[1]);
        free(pParam);
        break;
    default:
        break;
    }
    free(pLayer->pBackwardOutput);
    free(pLayer->pForwardOutput);
    free(pLayer);

    return 0;
}

void *PointerLayerParameters(H_LAYER hLayer)
{
    // NULL CEHCK
    if (hLayer == NULL)
    {
        return NULL;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    return pLayer->pLayerParam;
}
void *PointerForwardOutput(H_LAYER hLayer)
{
    // NULL CEHCK
    if (hLayer == NULL)
    {
        return NULL;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    return pLayer->pForwardOutput;
}
void *PointerBackwardOutput(H_LAYER hLayer)
{
    // NULL CEHCK
    if (hLayer == NULL)
    {
        return NULL;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    return pLayer->pBackwardOutput;
}
int calc_forword(H_LAYER hLayer, double *vInput);
int calc_backword(H_LAYER hLayer, double *vInput);
int update_params(H_LAYER hlayer);