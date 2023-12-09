#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../layer.h"

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
int test_2_calc_forword(void);
int test_2_calc_backword(void);

int main(void)
{
    int ret = 0;
    ret = test_2_create_layer();
    if (ret != 0)
    {
        printf("error : test_2_create_layer     \nerror id : %d\n", ret);
    }
    ret = test_2_delete_layer();
    if (ret != 0)
    {
        printf("error : test_2_delete_layer     \nerror id : %d\n", ret);
    }
    ret = test_2_print_layer();
    if (ret != 0)
    {
        printf("error : test_2_print_layer      \nerror id : %d\n", ret);
    }
    ret = test_2_calc_forword();
    if (ret != 0)
    {
        printf("error : test_2_calc_forword     \nerror id : %d\n", ret);
    }
    ret = test_2_calc_backword();
    if (ret != 0)
    {
        printf("error : test_2_calc_backword    \nerror id : %d\n", ret);
    }
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
            if (test[i].exp_create != 1)
            {
                test[i].result |= 0x1;
                result = 1;
            }
        }
        else
        {
            if (test[i].exp_create != 0)
            {
                test[i].result |= 0x1;
                result = 1;
            }
            ret = delete_layer(hLayer);
        }
        hLayer = NULL;
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
        hLayer=NULL;
        if (test[i].exp_create == 0)
        {
            hLayer = create_layer(test[i].type, test[i].input_size, test[i].output_size);
            ret=delete_layer(hLayer);
            if(ret!=0){
                test[i].result|=0x2;
                result=1;
            }
        }else{
            ret=delete_layer(hLayer);
            if(ret==0){
                test[i].result|=0x2;
                result=1;
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
        hLayer=NULL;
        hLayer=create_layer(test[i].type,test[i].input_size,test[i].output_size);
        ret=print_layer(hLayer);
        if(ret!=test[i].exp_create){
            test[i].result|=0x4;
            result = 1;
        }
    }
    return result;
}
int test_2_calc_forword(void)
{
    int result = 0;
    int ret = 0;
    H_LAYER hLayer = NULL;
    for (int i = 0; test[i].type != -1; i++)
    {
        ;
    }
    return result;
}
int test_2_calc_backword(void)
{
    return 0;
}
