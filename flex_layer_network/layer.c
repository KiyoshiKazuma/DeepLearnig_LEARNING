#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "layer.h"

double calc_sigmoid(double x)
{
    double y = 0;
    y = 1.0 / (1.0 + exp(-x));
    return y;
}

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
    case LT_ReLU:
    case LT_Sigmoid:
    case LT_Affine:
        //	LT_Softmax :
    case LT_SoftmaxWithLoss:
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
    H_MATRIX hForword = NULL;
    H_MATRIX hBackword = NULL;
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
        if (pLayer->input_size != pLayer->output_size)
        {
            error = 1;
        }
        pLayer->pLayerParam = NULL;

        break;
    case LT_Sigmoid:
        if (pLayer->input_size == pLayer->output_size)
        {
            pLayer->pLayerParam = NULL;
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

    if (error == 0)
    {
        pLayer->hBackwardOutput = create_matrix(input_size, 1);
        pLayer->hForwardOutput = create_matrix(output_size, 1);
        // NULL check
        if (pLayer->hBackwardOutput == NULL || pLayer->hBackwardOutput == NULL)
        {
            delete_matrix(pLayer->hBackwardOutput);
            delete_matrix(pLayer->hForwardOutput);
            error = 6;
        }
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

    int result = 0;
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
        result += delete_matrix((H_MATRIX)pParam[0]);
        result += delete_matrix((H_MATRIX)pParam[1]);
        free(pParam);

        break;
    case LT_SoftmaxWithLoss:
        pParam = pLayer->pLayerParam;
        result += delete_matrix((H_MATRIX)pParam[0]);
        result += delete_matrix((H_MATRIX)pParam[1]);
        free(pParam);
        break;
    default:
        break;
    }

    result += delete_matrix(pLayer->hBackwardOutput);
    result += delete_matrix(pLayer->hForwardOutput);
    free(pLayer);

    return result;
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
H_MATRIX PointerForwardOutput(H_LAYER hLayer)
{
    // NULL CEHCK
    if (hLayer == NULL)
    {
        return NULL;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    return pLayer->hForwardOutput;
}
H_MATRIX PointerBackwardOutput(H_LAYER hLayer)
{
    // NULL CEHCK
    if (hLayer == NULL)
    {
        return NULL;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    return pLayer->hBackwardOutput;
}

int calc_forword(H_LAYER hLayer, H_MATRIX hMatrix)
{
    if (hLayer == NULL || hMatrix == NULL)
    {
        return 1;
    }
    int ret = 0;
    S_LAYER *pLayer = NULL;
    S_MATRIX *pMatrixInput = NULL;
    S_MATRIX *pMatrixOutput = NULL;
    H_MATRIX W = NULL;
    H_MATRIX B = NULL;
    S_MATRIX *Y = NULL;
    S_MATRIX *T = NULL;
    H_MATRIX *pParam = NULL;

    pLayer = (S_LAYER *)hLayer;
    pMatrixOutput = (S_MATRIX *)pLayer->hForwardOutput;
    pMatrixInput = (S_MATRIX *)hMatrix;

    if (pMatrixInput->row != pLayer->input_size || pMatrixInput->column != 1 || pMatrixOutput->row != pLayer->output_size || pMatrixOutput->column != 1)
    {
        return 1;
    }

    switch (pLayer->type)
    {
    case LT_ReLU:
        for (unsigned int i = 0; i < pLayer->input_size; i++)
        {
            if (pMatrixInput->pElem[i] > 0)
            {
                pMatrixOutput->pElem[i] = pMatrixInput->pElem[i];
            }
            else
            {
                pMatrixOutput->pElem[i] = 0;
            }
        }
        break;
    case LT_Sigmoid:
        for (unsigned int i = 0; i < pLayer->input_size; i++)
        {
            pMatrixOutput->pElem[i] = calc_sigmoid(pMatrixInput->pElem[i]);
        }
        break;

    case LT_Affine:
        ret = 0;
        pParam = (H_MATRIX *)pLayer->pLayerParam;
        W = pParam[0];
        B = pParam[1];
        H_MATRIX hOutput1 = NULL;
        H_MATRIX hOutput2 = NULL;
        hOutput1 = create_matrix(pLayer->output_size, 1);
        hOutput2 = create_matrix(pLayer->output_size, 1);

        ret += product_matrix(W, hMatrix, hOutput1);
        ret += add_matrix(hOutput1, B, hOutput2);

        if (ret == 0)
        {
            ret += copy_matrix(hOutput2, (void *)pMatrixOutput);
        }
        delete_matrix(hOutput1);
        delete_matrix(hOutput2);
        break;
    case LT_SoftmaxWithLoss:
        pParam = (H_MATRIX *)pLayer->pLayerParam;
        Y = (S_MATRIX *)pParam[0];
        T = (S_MATRIX *)pParam[1];
        S_MATRIX *pOutput = NULL;
        pOutput = (S_MATRIX *)create_matrix(pLayer->input_size, 1);
        double exp_sum = 0;
        double L = 0;
        for (int i = 0; i < pOutput->size; i++)
        {
            exp_sum += exp(pMatrixInput->pElem[i]);
        }
        for (int i = 0; i < pOutput->size; i++)
        {
            Y->pElem[i] = exp(pMatrixInput->pElem[i]) / exp_sum;
        }

        for (int i = 0; i < pOutput->size; i++)
        {
            L -= T->pElem[i] * log(Y->pElem[i]);
        }
        pMatrixOutput->pElem[0] = L;
        delete_matrix((void *)pOutput);
        break;
    default:
        break;
    }

    return 0;
}
int calc_backword(H_LAYER hLayer, H_MATRIX hMatrix)
{
    // NULL CHECK
    if (hLayer == NULL || hMatrix == NULL)
    {
        return 1;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    S_MATRIX *pOutput = (S_MATRIX *)PointerBackwardOutput(hLayer);

    // size check
    // Input Matrix Size should be (OUTPUT_SIZE,1)
    if (pMatrix->row != pLayer->output_size || pMatrix->column != 1)
    {
        return 1;
    }

    // execute calclation
    switch(pLayer->type)
    {
    case LT_ReLU:
        for (unsigned int i = 0; i < pLayer->output_size; i++)
        {
            if (pMatrix->pElem[i] > 0)
            {
                pOutput->pElem[i] = pMatrix->pElem[i];
            }
            else
            {
                pOutput->pElem[i] = 0;
            }
        }
        break;
    case LT_Sigmoid:
        break;
    case LT_Affine:
        break;
    case LT_SoftmaxWithLoss:
        break;
    default:
        break;
    }

    return 0;
}
