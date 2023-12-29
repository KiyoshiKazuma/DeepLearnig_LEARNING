#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../layer.h"

#define D_TEST_CREATE_LAYER
#define D_TEST_DELETE_LAYER
// #define D_TEST_PRINT_LAYER
#define D_TEST_P2_LAYER_PARAMS
#define D_TEST_P2_FORWARD_OUTPUT
#define D_TEST_P2_BACKWARD_OUTPUT
// #define D_TEST_CALC_FORWARD
// #define D_TEST_CAL_BACKWARD

typedef struct
{
    // 入力値情報
    int type;
    unsigned int input_size;
    unsigned int output_size;
    // 期待値情報
    int exp_create; // create_layerのテスト結果 0:成功 1:失敗
    // テスト結果
    int result;
} S_test_case;
S_test_case test[] = {
    LT_ReLU, 2, 2, 0, 0,
    LT_ReLU, 2, 0, 1, 0,
    LT_ReLU, 0, 3, 1, 0,
    LT_ReLU, 3, 2, 1, 0,
    LT_Sigmoid, 2, 2, 0, 0,
    LT_Sigmoid, 2, 0, 1, 0,
    LT_Sigmoid, 0, 3, 1, 0,
    LT_Sigmoid, 3, 2, 1, 0,
    LT_Affine, 2, 2, 0, 0,
    LT_Affine, 0, 3, 1, 0,
    LT_Affine, 2, 0, 1, 0,
    LT_Affine, 3, 2, 0, 0,
    LT_SoftmaxWithLoss, 4, 1, 0, 0,
    LT_SoftmaxWithLoss, 0, 3, 1, 0,
    LT_SoftmaxWithLoss, 2, 0, 1, 0,
    LT_SoftmaxWithLoss, 3, 2, 1, 0,
    -1, 0, 0, 0, 0
    // end marker
};

int test_2_create_layer(void);
int test_2_delete_layer(void);
int test_2_print_layer(void);
int test_2_PointerLayerParameters(void);
int test_2_PointerForwardOutput(void);
int test_2_PointerBackwardOutput(void);
int test_2_calc_forword(void);
int test_2_calc_backword(void);

int main(void)
{
    int ret = 0;

#ifdef D_TEST_CREATE_LAYER
    printf("RUNNING : test_2_create_layer\n");
    ret = 0;
    ret = test_2_create_layer();
    if (ret != 0)
    {
        printf("error : test_2_create_layer     \nerror id : %d\n", ret);
        for (int i = 0; test[i].type != -1; i++)
        {
            if ((test[i].result >> 0) & 0x1)
            {
                printf("test case : %d is failed\n", i);
            }
        }
    }
#endif // D_TEST_CREATE_LAYER

#ifdef D_TEST_DELETE_LAYER
    printf("RUNNING : test_2_delete_layer\n");
    ret = 0;
    ret = test_2_delete_layer();
    if (ret != 0)
    {
        printf("error : test_2_delete_layer     \nerror id : %d\n", ret);
        for (int i = 0; test[i].type != -1; i++)
        {
            if ((test[i].result >> 1) & 0x1)
            {
                printf("test case : %d is failed\n", i);
            }
        }
    }
#endif // D_TEST_DELETE_LAYER

#ifdef D_TEST_PRINT_LAYER
    printf("RUNNING : test_2_print_layer\n");
    ret = 0;
    ret = test_2_print_layer();
    if (ret != 0)
    {
        printf("error : test_2_print_layer      \nerror id : %d\n", ret);
        for (int i = 0; test[i].type != -1; i++)
        {
            if ((test[i].result >> 2) & 0x1)
            {
                printf("test case : %d is failed\n", i);
            }
        }
    }
#endif // D_TEST_PRINT_LAYER

#ifdef D_TEST_P2_LAYER_PARAMS
    printf("RUNNING : test_2_PointerLayerParameters\n");
    ret = test_2_PointerLayerParameters();
    if (ret != 0)
    {
        printf("error : test_2_PointerLayerParameters  \n error id : %d\n , ret");
        for (int i = 0; test[i].type != -1; i++)
        {
            if ((test[i].result >> 3) & 0x1)
            {
                printf("test case : %d is failed\n", i);
            }
        }
    }
#endif // D_TEST_P2_LAYER_PARAMS

#ifdef D_TEST_P2_FORWARD_OUTPUT
    printf("RUNNING : test_2_PointerForwardOutput\n");
    ret = test_2_PointerForwardOutput();
    if (ret != 0)
    {
        printf("error : test_2_PointerForwardOutput  \n error id : %d\n , ret");
        for (int i = 0; test[i].type != -1; i++)
        {
            if ((test[i].result >> 4) & 0x1)
            {
                printf("test case : %d is failed\n", i);
            }
        }
    }
#endif // D_TEST_P2_FORWARD_OUTPUT

#ifdef D_TEST_P2_BACKWARD_OUTPUT
    printf("RUNNING : test_2_PointerBackwardOutput\n");
    ret = test_2_PointerBackwardOutput();
    if (ret != 0)
    {
        printf("error : test_2_PointerBackwardOutput  \n error id : %d\n , ret");
    }
#endif // D_TEST_P2_BACKWARD_OUTPUT

#ifdef D_TEST_CALC_FORWARD
    printf("RUNNING : test_2_calc_forword\n");
    ret = test_2_calc_forword();
    if (ret != 0)
    {
        printf("error : test_2_calc_forword     \nerror id : %d\n", ret);
    }
#endif // D_TEST_CALC_FORWARD

#ifdef D_TEST_CAL_BACKWARD
    ret = test_2_calc_backword();
    if (ret != 0)
    {
        printf("error : test_2_calc_backword    \nerror id : %d\n", ret);
    }
#endif // D_TEST_CAL_BACKWAR
    return 0;
}

int test_2_create_layer(void)
{
    int result = 0;
    int ret = 0;
    H_LAYER hLayer = NULL;
    for (int i = 0; test[i].type != -1; i++)
    {
        hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
        if (hLayer == NULL)
        {
            if (test[i].exp_create == 0) // 期待値正常で異常状態判定
            {
                test[i].result |= 0x1;
                result = 1;
            }
        }
        else
        {
            if (test[i].exp_create != 0) // 期待値異常で正常状態判定
            {
                test[i].result |= 0x1;
                result = 1;
            }
        }
        ret = delete_layer(hLayer);
    }

    return result;
}
int test_2_delete_layer(void)
{
    int result = 0;
    int ret = 0;
    H_LAYER hLayer = NULL;
    for (int i = 0; test[i].type != -1; i++)
    {
        hLayer = NULL;
        hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
        ret = delete_layer(hLayer);
        if (test[i].exp_create == 0) // hLayer not NULL --> ret should be E_OK(0)
        {
            if (ret != 0)
            {
                test[i].result |= 0x2;
                result = 1;
            }
        }
        else
        { // hLayer is NULL --> ret should NOT be E_OK(0)
            if (ret == 0)
            {
                test[i].result |= 0x2;
                result = 1;
            }
        }
    }

    return result;
}

int test_2_print_layer(void)
{
    int result = 0;
    int ret = 0;
    H_LAYER hLayer = NULL;
    for (int i = 0; test[i].type != -1; i++)
    {
        hLayer = NULL;
        hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
        ret = print_layer(hLayer);
        if ((ret == 0) != (test[i].exp_create == 0))
        {
            test[i].result |= 0x4;
            result = 1;
        }
    }
    return result;
}

int test_2_PointerLayerParameters(void)
{

    int result = 0;
    int ret = 0;
    void *pParam = NULL;
    H_MATRIX *vMatrix = NULL;
    H_LAYER hLayer = NULL;
    H_MATRIX W = NULL;
    H_MATRIX B = NULL;
    H_MATRIX Y = NULL;
    H_MATRIX T = NULL;
    S_MATRIX *pMatrix;

    for (int i = 0; test[i].type != -1; i++)
    {
        hLayer = NULL;
        pParam = NULL;

        hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
        pParam = PointerLayerParameters(hLayer);
        if (test[i].exp_create == 0)
        {
            // Layers with types where LayerParameter is defined should have valid LayerParameters.
            // Layers with types where LayerParameter is NOT defined should not have valid LayerParameters.(NULL)
            switch (test[i].type)
            {
            case LT_Affine:
                if (pParam == NULL)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                vMatrix = (H_LAYER *)pParam;
                W = vMatrix[0];
                B = vMatrix[1];
                // size check
                pMatrix = (S_MATRIX *)W;
                if (pMatrix->row != test[i].output_size || pMatrix->column != test[i].input_size || pMatrix->size != test[i].input_size * test[i].output_size)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                pMatrix = (S_MATRIX *)B;
                if (pMatrix->row != test[i].output_size || pMatrix->column != 1 || pMatrix->size != test[i].output_size)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                break;
            case LT_SoftmaxWithLoss:
                if (pParam == NULL)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                vMatrix = (H_LAYER *)pParam;
                Y = vMatrix[0];
                T = vMatrix[1];
                // size check
                pMatrix = (S_MATRIX *)Y;
                if (pMatrix->row != test[i].input_size || pMatrix->column != 1 || pMatrix->size != test[i].input_size)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                pMatrix = (S_MATRIX *)T;
                if (pMatrix->row != test[i].input_size || pMatrix->column != 1 || pMatrix->size != test[i].input_size)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                break;
            default:
                if (pParam != NULL)
                {
                    test[i].result |= 0x8;
                    result = 1;
                }
                break;
            }
        }
        else
        {
            if (pParam != NULL)
            {
                test[i].result |= 0x8;
                result = 1;
            }
        }
    }

    return result;
}
int test_2_PointerForwardOutput()
{

    int result = 0;
    int ret = 0;
    S_MATRIX *pMatrix = NULL;
    H_LAYER hLayer = NULL;

    for (int i = 0; test[i].type != -1; i++)
    {
        hLayer = NULL;
        pMatrix = NULL;
        hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
        pMatrix = (S_MATRIX *)PointerForwardOutput(hLayer);
        if (test[i].exp_create == 0)
        {
            if (pMatrix == NULL)
            {
                test[i].result |= 0x10;
                result = 1;
            }
            // size check
            if (pMatrix->column != 1 || pMatrix->row != test[i].output_size)
            {
                test[i].result |= 0x10;
                result = 1;
            }
        }
        else
        {
            if (pMatrix != NULL)
            {
                test[i].result |= 0x10;
                result = 1;
            }
        }
    }
    return result;
}
int test_2_PointerBackwardOutput()
{
    int result = 0;
    int ret = 0;
    S_MATRIX *pMatrix = NULL;
    H_LAYER hLayer = NULL;

    for (int i = 0; test[i].type != -1; i++)
    {
        pMatrix = NULL;
        hLayer = NULL;
        hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
        pMatrix = (S_MATRIX *)PointerBackwardOutput(hLayer);
        if (test[i].exp_create == 0)
        {
            if (pMatrix == NULL)
            {
                test[i].result |= 0x20;
                result = 1;
            }
            // size check
            if (pMatrix->column != 1 || pMatrix->row != test[i].input_size)
            {
                test[i].result |= 0x20;
                result = 1;
            }
        }
        else
        {
            if (pMatrix != NULL)
            {
                test[i].result |= 0x20;
                result = 1;
            }
        }
    }
    return result;
}

int test_2_calc_forword(void)
{
    int result = 0;
    int ret = 0;
    H_LAYER hLayer = NULL;
    H_MATRIX hInput = NULL;
    H_MATRIX hOutput = NULL;
    H_MATRIX hW = NULL;
    H_MATRIX hB = NULL;
    S_LAYER *pLayer = NULL;
    S_MATRIX *pInput = NULL;
    S_MATRIX *pOutput = NULL;
    S_MATRIX *pW = NULL;
    S_MATRIX *pB = NULL;
    H_MATRIX *vLayerParams = NULL;

    //(1) ReLu collect
    double input_1[3] = {-1.0, 0.0, 1.0};
    double exp_1[3] = {0.0, 0.0, 1.0};
    hLayer = create_layer(LT_ReLU, 3, 3);
    hOutput = PointerForwardOutput(hLayer);
    hInput = create_matrix(3, 1);
    pLayer = (S_LAYER *)hLayer;
    pOutput = (S_MATRIX *)hOutput;
    pInput = (S_MATRIX *)hInput;
    for (int i = 0; i < 3; i++)
    {
        pInput->pElem[i] = input_1[i];
    }
    ret = calc_forword(hLayer, hInput);
    if (ret == 0)
    {
        // compare output data and exp data
        for (int i = 0; i < 3; i++)
        {
            double diff = pOutput->pElem[i] - exp_1[i];
            if (diff > 0.01 || diff < -0.01)
            {
                ret = 1;
                break;
            }
        }
    }
    if (ret != 0)
    {
        result += 0x1;
    }
    printf("test_2_calc_forword : (1) result\n");
    printf("\texp \t: ");
    for (int i = 0; i < 3; i++)
        printf("  %f  ", exp_1[i]);
    printf("\n\toutput \t");
    for (int i = 0; i < 3; i++)
        printf("  %f  ", pOutput->pElem[i]);
    printf("\n");

    delete_layer(hLayer);
    delete_matrix(hInput);

    //(2) Sigmoid collect
    double input_2[3] = {-1.0, 0.0, 1.0};
    double exp_2[3] = {0.2689, 0.5, 0.7310};
    hLayer = NULL;
    hInput = NULL;
    hOutput = NULL;
    pLayer = NULL;
    pInput = NULL;
    pOutput = NULL;

    hLayer = create_layer(LT_Sigmoid, 3, 3);
    hInput = create_matrix(3, 1);
    hOutput = PointerForwardOutput(hLayer);
    pLayer = (S_LAYER *)hLayer;
    pInput = (S_MATRIX *)hInput;
    pOutput = (S_MATRIX *)hOutput;

    for (int i = 0; i < 3; i++)
    {
        pInput->pElem[i] = input_2[i];
    }

    ret = calc_forword(hLayer, hInput);
    if (ret == 0)
    {
        // compare output data and exp data
        for (int i = 0; i < 3; i++)
        {
            double diff = pOutput->pElem[i] - exp_2[i];
            if (diff > 0.01 || diff < -0.01)
            {
                ret = 1;
                break;
            }
        }
    }
    if (ret != 0)
    {
        result += 0x2;
    }
    delete_layer(hLayer);
    delete_matrix(hInput);

    //(3)Affine Collect

    double input_3[3] = {-1.0, 0.0, 1.0};
    double exp_3[2] = {9.0, 10.0};
    double W_3[6] = {1, 2, 3, 4, 5, 6};
    double B_3[2] = {7, 8};

    hLayer = NULL;
    hInput = NULL;
    hOutput = NULL;
    hW = NULL;
    hB = NULL;
    pLayer = NULL;
    pInput = NULL;
    pOutput = NULL;
    pW = NULL;
    pB = NULL;

    hLayer = create_layer(LT_Affine, 3, 2);
    hInput = create_matrix(3, 1);
    hOutput = PointerForwardOutput(hLayer);
    pLayer = (S_LAYER *)hLayer;
    pInput = (S_MATRIX *)hInput;
    pOutput = (S_MATRIX *)hOutput;
    vLayerParams = (H_MATRIX *)PointerLayerParameters(hLayer);
    hW = vLayerParams[0];
    hB = vLayerParams[1];
    pW = (S_MATRIX *)hW;
    pB = (S_MATRIX *)hB;
    for (int i = 0; i < 6; i++)
    {
        pW->pElem[i] = W_3[i];
    }
    for (int i = 0; i < 2; i++)
    {
        pB->pElem[i] = B_3[i];
    }

    for (int i = 0; i < 3; i++)
    {
        pInput->pElem[i] = input_3[i];
    }

    ret = calc_forword(hLayer, hInput);
    if (ret == 0)
    {
        // compare output data and exp data
        for (int i = 0; i < 2; i++)
        {
            double diff = pOutput->pElem[i] - exp_3[i];
            if (diff > 0.01 || diff < -0.01)
            {
                ret = 1;
                break;
            }
        }
    }
    if (ret != 0)
    {
        result += 0x4;
    }
    delete_layer(hLayer);
    delete_matrix(hInput);

    return result;
}
int test_2_calc_backword(void)
{
    return 0;
}
