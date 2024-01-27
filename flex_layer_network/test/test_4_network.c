#include <stdio.h>
#include <stdlib.h>

#include "../network.h"

//(1)ネットワークを生成できること
// 　(1-1)生成が成功すること
//  (1-2)初期化が正しく実行されていること
//(2)ネットワークにLAYERを追加できること
//(3)フォワード計算が正しく実行できること
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
        hLayer = get_layer(hList, i + 1);
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