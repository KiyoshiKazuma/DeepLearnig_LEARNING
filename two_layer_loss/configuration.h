#ifndef H_CONFIG
#define H_CONFIG
#include matrix.h
//諸々変数を設定する。

extern S_NETWORK_INFO g_network_info;

#define NETWORK_MAX_LAYER (100) //ネットワーク層数の最大値
#define TEACHER_MAX_SIZE (1000) //学習データセットの最大数
#define TEACHER_FILE_NAME "teacher.dat"//学習データセットを格納するファイル名
#define NEURON_FILE_NAME "neuron.dat"//ニューラルネットワーク変数を格納するファイル名
#define EPOCH_SIZE (100)
#define TEST_SIZE (10)
#define REPEAT_COUNT (100)
#define INPUT_SIZE (2)
#define HIDEN_SIZE (4)
#define OUTPUT_SIZE (2)
#define LEARNING_RATE (0.1)

typedef struct{
    int layer_size;//ニューラルネットワークの層数
    int neurons_size[NETWORK_MAX_LAYER];//各層のパラメタ数
    int teacher_size;//教師データの層数
    S_MATRIX * W;
    S_MATRIX * B;
    double ** pnet_valuee;
}S_NETWORK_INFO;
#endif