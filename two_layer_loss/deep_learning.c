#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "teacher_file.h"
#include "network_data.h"
#include "configuration.h"
#include "gradient_descent.h"

#define D_DEBUG
#define MULTI

int main(void){
    //変数宣言
    srand((unsigned int)time(NULL));
    int input_size,output_size;
    int hiden_size[NETWORK_MAX_LAYER];
    int teacher_size;
    int layer_size;
    int ret;
    int network_size;
    double ** pnet_value;
    FILE * fp;
    fp=fopen("data.dat","w");
    S_MATRIX W[NETWORK_MAX_LAYER];
    S_MATRIX B[NETWORK_MAX_LAYER];
    S_MATRIX X[TEACHER_MAX_SIZE];
    S_MATRIX T[TEACHER_MAX_SIZE];
    S_MATRIX X_epoch[EPOCH_SIZE];
    S_MATRIX T_epoch[EPOCH_SIZE];
    S_MATRIX X_test[TEST_SIZE];
    S_MATRIX T_test[TEST_SIZE];
    S_MATRIX Y;
    
    //教師データを読み込む
    teacher_size=read_teacher_data(&input_size,&output_size,X,T);
    if(teacher_size<0){
        printf("ERROR occor in \"read_teacher_data\"\n");
        printf("error code %d\n",teacher_size);
    }

    //ネットワーク変数データを読み込む
    network_size=get_network_info(&layer_size,&hiden_size);
    ret=read_network_data(pnet_value,network_size);

    //学習用パラメータをセット
    for(int i=0;i<layer_size;i++){
        F_CREATE_MATRIX(network_size[i],network_size[i+1],W[i]);
    }


    int size_net=calc_size_net(W,B);    
    double **pnet_value=malloc(sizeof(double)*size_net);
    double * dL = malloc(sizeof(double)*size_net);

    aggregate_network_data(W,B,pnet_value);

    //ネットワーク変数の初期値をランダムに生成
    //OR
    //ネットワークデータを読み込み
    printf("init network file? (y/n) :");
    char cmd;
    scanf("%c",&cmd);
    if(cmd=='y'){
        for(int i=0;i<size_net;i++){
            *(pnet_value[i])=rand()%100/100.0;
        }
    }else{
        read_network_data(pnet_value,size_net);
    }

    //学習を実行
    

    //学習データをファイルに記録
    update_network_data(pnet_value,size_net);

    //動的メモリの解放
    free(pnet_value);
    free(dL);

    F_DELETE_MATRIX(&Y);
    F_DELETE_MATRIX(&W[0]);
    F_DELETE_MATRIX(&W[1]);
    F_DELETE_MATRIX(&B[0]);
    F_DELETE_MATRIX(&B[1]);
    fclose(fp);
    return 0;
}


