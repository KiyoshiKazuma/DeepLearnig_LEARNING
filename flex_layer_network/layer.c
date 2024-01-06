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
    H_MATRIX hX = NULL;
    H_MATRIX hForword = NULL;
    H_MATRIX hBackword = NULL;
    H_MATRIX *vParam = NULL;
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
        hX = create_matrix(input_size, 1);
        if (hW == NULL || hB == NULL || hX == NULL)
        {
            delete_matrix(hW);
            delete_matrix(hB);
            delete_matrix(hX);
            error = 3;
        }
        vParam = (H_MATRIX *)malloc(sizeof(H_MATRIX *) * 3);
        // NULL check
        if (vParam == NULL)
        {
            free(vParam);
            error = 4;
            break;
        }

        vParam[0] = hW;
        vParam[1] = hB;
        vParam[2] = hX;
        pLayer->pLayerParam = vParam;
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
            vParam = (H_MATRIX *)malloc(sizeof(void *) * 2);
            // NULL check
            if (vParam == NULL)
            {
                free(vParam);
                error = 5;
                break;
            }
            vParam[0] = hY;
            vParam[1] = hT;
            pLayer->pLayerParam = vParam;
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
    H_LAYER *vParam = PointerLayerParameters(hLayer);
    printf("---PrintLayer---\n");
    printf("-type \t : %u\n", pLayer->type);
    printf("-input \t : %u\n", pLayer->input_size);
    printf("-output\t : %u\n", pLayer->output_size);
    printf("-Forword\n");
    print_matrix(pLayer->hForwardOutput);
    printf("-Backward\n");
    print_matrix(pLayer->hBackwardOutput);
    switch (pLayer->type)
    {
    case LT_Affine:
        printf("-W\n");
        print_matrix(vParam[0]);
        printf("-B\n");
        print_matrix(vParam[1]);
        printf("-X\n");
        print_matrix(vParam[2]);
        break;
    case LT_SoftmaxWithLoss:
        printf("-Y\n");
        print_matrix(vParam[0]);
        printf("-T\n");
        print_matrix(vParam[1]);
        break;
    default:
        break;
    }
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
    H_MATRIX *vParam;
    H_MATRIX hW = NULL;
    H_MATRIX hB = NULL;
    H_MATRIX hX = NULL;
    S_LAYER *pLayer = (S_LAYER *)hLayer;

    switch (pLayer->type)
    {
    case LT_ReLU:
    case LT_Sigmoid:
    case LT_Softmax:
        break;
    case LT_Affine:
        vParam = PointerLayerParameters(hLayer);
        hW = vParam[0];
        hB = vParam[1];
        hX = vParam[2];
        result += delete_matrix(hW);
        result += delete_matrix(hB);
        result += delete_matrix(hX);
        free(vParam);

        break;
    case LT_SoftmaxWithLoss:
        vParam = pLayer->pLayerParam;
        result += delete_matrix(vParam[0]);
        result += delete_matrix(vParam[1]);
        free(vParam);
        break;
    default:
        break;
    }

    result += delete_matrix(PointerBackwardOutput(hLayer));
    result += delete_matrix(PointerForwardOutput(hLayer));
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
    H_MATRIX X = NULL;
    S_MATRIX *Y = NULL;
    S_MATRIX *T = NULL;
    H_MATRIX *vParam = NULL;

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
        vParam = (H_MATRIX *)pLayer->pLayerParam;
        W = vParam[0];
        B = vParam[1];
        X = vParam[2];
        S_MATRIX *pX = (S_MATRIX *)X;
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

        for (unsigned int i = 0; i < pLayer->input_size; i++)
        {
            pX->pElem[i] = pMatrixInput->pElem[i];
        }
        break;
    case LT_SoftmaxWithLoss:
        vParam = (H_MATRIX *)pLayer->pLayerParam;
        Y = (S_MATRIX *)vParam[0];
        T = (S_MATRIX *)vParam[1];
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
    if (hLayer == NULL)
    {
        return 1;
    }
    S_LAYER *pLayer = (S_LAYER *)hLayer;
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    H_MATRIX hOutput = PointerBackwardOutput(hLayer);
    S_MATRIX *pOutput = (S_MATRIX *)hOutput;
    H_MATRIX *vParam = (H_MATRIX *)PointerLayerParameters(hLayer);
    H_MATRIX hW = NULL;
    H_MATRIX hB = NULL;
    H_MATRIX hX = NULL;
    H_MATRIX hWt = NULL;
    H_MATRIX hBt = NULL;
    H_MATRIX hXt = NULL;
    S_MATRIX *pY = NULL;
    S_MATRIX *pT = NULL;
    H_MATRIX hdW = NULL;
    H_MATRIX hdB = NULL;
    int ret = 0;
    // size check
    // Input Matrix Size should be (OUTPUT_SIZE,1)
    switch (pLayer->type)
    {
    case LT_SoftmaxWithLoss:
        break;
    default:
        if(pMatrix==NULL){
            return 1;
        }
        if (pMatrix->row != pLayer->output_size || pMatrix->column != 1)
        {
            return 1;
        }
        break;
    }

    // execute calclation
    switch (pLayer->type)
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
        for (unsigned int i = 0; i < pLayer->input_size; i++)
        {
            pOutput->pElem[i] = pMatrix->pElem[i] * (1.0 - pMatrix->pElem[i]);
        }
        break;
    case LT_Affine:
        hdW = create_matrix(pLayer->output_size, pLayer->input_size);
        hdB = create_matrix(pLayer->output_size, 1);
        S_MATRIX *pdW = (S_MATRIX *)hdW;
        S_MATRIX *pdB = (S_MATRIX *)hdB;
        hW = vParam[0];
        hB = vParam[1];
        hX = vParam[2];
        hWt = transpose_matrix(hW);
        hBt = transpose_matrix(hB);
        hXt = transpose_matrix(hX);

        S_MATRIX *pW = (S_MATRIX *)hW;
        S_MATRIX *pB = (S_MATRIX *)hB;

        ret = product_matrix(hWt, hMatrix, hOutput);
        ret += product_matrix(hMatrix, hXt, hdW);
        ret += copy_matrix(hMatrix, hdB);

        if (ret == 0)
        {
            for (unsigned int i = 0; i < pW->size; i++)
            {
                pW->pElem[i] -= D_LEARNING_RATE * pdW->pElem[i];
            }
            for (unsigned int i = 0; i < pW->size; i++)
            {
                pB->pElem[i] -= D_LEARNING_RATE * pdB->pElem[i];
            }
        }

        delete_matrix(hdW);
        delete_matrix(hdB);

        break;
    case LT_SoftmaxWithLoss:
        pY = (S_MATRIX *)vParam[0];
        pT = (S_MATRIX *)vParam[1];
        for (unsigned int i = 0; i < pOutput->size; i++)
        {
            pOutput->pElem[i] = pY->pElem[i] - pT->pElem[i];
        }
        break;
    default:
        ret = 1;
        break;
    }

    return ret;
}
