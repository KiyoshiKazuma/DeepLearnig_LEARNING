#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"
#include "teacher_file.h"
#include "network_data.h"

void init(S_MATRIX* W,S_MATRIX* B,double **pnet_value){
    int i,j,n;

    //ネットワークの変数W,Bの各要素へのポインタの配列を作成。
    n=0;
    for(i=0;i<2;i++){
        for(j=0;j<SIZE(W[i]);j++){
            *(pnet_value+n)=W[i].elep+j;
            n++;
        }
    }
    for(i=0;i<2;i++){
        for(j=0;j<SIZE(B[i]);j++){
            *(pnet_value+n)=B[i].elep+j;             
            n++;
        }
    }

    //ネットワーク変数の初期値をランダムに生成
    for(i=0;i<n;i++){
        *(pnet_value[i])=rand()%100/100.0;
    }
}


int main(void){
    int size_input,size_output;
    int i,j,k,n,ret;
    int size_teacher;
    int layer_size;
    int netamount;
    int neuron_layer[MAX_SIZE_TEACHER];

    size_input=2;
    size_output=2;
    layer_size=1;

    
    S_MATRIX X;
    S_MATRIX Y;
    F_CREATE_MATRIX(1,size_input,&X);
    F_CREATE_MATRIX(1,size_output,&Y);
    
    //学習用パラメータをセット
    S_MATRIX W[layer_size+1];  
    S_MATRIX B[layer_size+1];
    F_CREATE_MATRIX(size_input,HIDEN_SIZE,&W[0]);
    F_CREATE_MATRIX(HIDEN_SIZE,size_output,&W[1]);
    F_CREATE_MATRIX(1,HIDEN_SIZE,&B[0]);
    F_CREATE_MATRIX(1,size_output,&B[1]);

    //ネットワークデータを読み込み
    get_network_info(&layer_size,neuron_layer);
    netamount=net_data_amount(layer_size,neuron_layer,size_input,size_output);
    double **pnet_value=malloc(sizeof(double)*netamount);
    init(W,B,pnet_value);
    read_network_data(pnet_value,netamount);


/***********以下にテスト用コードを記述*****************************/

    printf("put input data \"x1 x2\" :");
    scanf("%lf %lf",&X.elep[0],&X.elep[1]);
    two_layer_net(&X,&Y,W,B);
    printf("input data\n");
    F_PRINT(&X);
    printf("output data\n");
    F_PRINT(&Y);
/******************************************************************/

//動的メモリの解放
    printf("##phase (5)\n");
    free(pnet_value);
    F_DELETE_MATRIX(&W[0]);
    F_DELETE_MATRIX(&W[1]);
    F_DELETE_MATRIX(&B[0]);
    F_DELETE_MATRIX(&B[1]);
    return 0;
}
