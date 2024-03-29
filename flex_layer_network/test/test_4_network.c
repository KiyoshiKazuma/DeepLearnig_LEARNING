#include <stdio.h>
#include <stdlib.h>

#include "../network.h"

//(1)ネットワークを生成できること
//  (1-1)生成が成功すること
//  (1-2)初期化が正しく実行されていること
//(2)ネットワークにLAYERを追加できること
//  (2-1)add関数が正常を返すこと
//  (2-2)ネットワークの総数がaddした回数と一致すること
//  (2-3)追加した層のタイプ、サイズが正しいこと
//(3)フォワード計算が正しく実行できること
//  (3-1)フォワード計算関数が正常に動作すること
//  (3-2)outputが期待値通りであること
//  (3-3)損失値が期待値通りであること
//(4)誤差逆伝播計算が正しく実行できること

typedef struct
{
    enum LayerType type;
    unsigned int IF_size;
} s_test_case;

s_test_case test_case[5] =
    {{LT_Sigmoid, 3},
     {LT_Affine, 4},
     {LT_ReLU, 4},
     {LT_Affine, 4},
     {LT_SoftmaxWithLoss, 1}};

unsigned int IF_size_exp[] = {3, 3, 4, 4, 4, 1};
double input[3] = {-1, 0, 1};
double affine_params_1[3][4] = {
    {-0.5, -0.4, -0.3},
    {-0.2, -0.1, 0},
    {0.1, 0.2, 0.3},
    {0.4, 0.5, 0.6}};
double affine_params_2[4][4] = {
    {-0.5, -0.4, -0.3, -0.2},
    {-0.1, 0, 0.1, 0.2},
    {0.3, 0.4, 0.5, 0.6},
    {0.7, 0.8, 0.9, 1}};
double T[4] = {0.1, 0.2, 0.3, 0.4};
double output_exp[4] = {0.2006937351, 0.2305031957, 0.2647403178, 0.3040627514};
double Loss_exp = 4.592992837;
//(1)ネットワークを生成できること
int test_4_network_1_create(void)
{
    H_NETWORK hNetwork = NULL;
    int ret = 0;
    int result = 0;
    unsigned int input_size = 3;
    unsigned int output_size = 4;
    // 　(1-1)生成が成功すること
    hNetwork = create_network(input_size, output_size);
    if (hNetwork == NULL)
    {
        result += 0x1 << 0;
    }
    //  (1-2)初期化が正しく実行されていること
    if (input_size_network(hNetwork) != input_size ||
        output_size_network(hNetwork) != output_size ||
        size_network(hNetwork) != 0 || list_network(hNetwork) == NULL)
    {
        result += 0x1 << 1;
    }

    delete_network(hNetwork);
    return result;
}

//(2)ネットワークにLAYERを追加できること
int test_4_network_2_add(void)
{
    H_NETWORK hNetwork = NULL;
    H_LIST hList = NULL;
    H_LAYER hLayer = NULL;
    int ret = 0;
    int result = 0;
    unsigned int input_size = 3;
    unsigned int output_size = 4;

    hNetwork = create_network(input_size, output_size);
    hList = list_network(hNetwork);

    //  (2-1)add関数の戻り値が正常(0)であること
    for (unsigned int i = 0; i < 5; i++)
    {
        ret = add_network(hNetwork, test_case[i].type, test_case[i].IF_size);
        if (ret != 0)
        {
            result += 0x1 << i;
        }
    }

    //  (2-2)追加したLayerが正しくカウントされていること

    if (size_network(hNetwork) != 5)
    {
        result += 0x1 << 5;
    }

    //  (2-3)追加したLayerが正しく設定されていること
    for (unsigned int i = 0; i < 5; i++)
    {
        hLayer = get_layer(hNetwork, i + 1);
        enum LayerType type_tmp = type_layer(hLayer);
        unsigned int input_size_tmp = input_size_layer(hLayer);
        unsigned int output_size_tmp = output_size_layer(hLayer);

        if (type_tmp != test_case[i].type || input_size_tmp != IF_size_exp[i] || output_size_tmp != IF_size_exp[i + 1])
        {
            result += 0x1 << 5 + i;
        }
    }

    print_network(hNetwork);

    delete_network(hNetwork);
    return result;
}

//(3)フォワード計算が正しく実行できること
int test_4_network_3_calcforward(void)
{
    H_NETWORK hNetwork = NULL;
    H_LIST hList = NULL;
    H_LAYER hLayer = NULL;
    int ret = 0;
    int result = 0;
    unsigned int input_size = 3;
    unsigned int output_size = 4;

    hNetwork = create_network(input_size, output_size);
    hList = list_network(hNetwork);

    for (unsigned int i = 0; i < 5; i++)
    {
        ret = add_network(hNetwork, test_case[i].type, test_case[i].IF_size);
        if (ret != 0)
        {
            result += 0x1 << i;
        }
    }

    //  (3-1)フォワード計算関数が正常に動作すること
    //  (3-2)outputが期待値通りであること
    //  (3-3)損失値が期待値通りであること

}

void main()
{
    int ret = 0;

    //(1)
    printf("=================\n");
    printf("TEST CASE : 1\n");
    ret = test_4_network_1_create();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }

    //(2)
    printf("=================\n");
    printf("TEST CASE : 2\n");
    ret = test_4_network_2_add();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }
}