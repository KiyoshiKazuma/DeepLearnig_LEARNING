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
    srand((unsigned int)time(NULL));
    int size_input,size_output;
    int i,j,k,n,ret;
    int size_teacher=0;
    int layer_size=1;
    int neuron_layer={3};
    S_MATRIX W[2];
    S_MATRIX B[2];

    //学習用パラメータをセット
    F_CREATE_MATRIX(size_input,HIDEN_SIZE,&W[0]);
    F_CREATE_MATRIX(HIDEN_SIZE,size_output,&W[1]);
    F_CREATE_MATRIX(1,HIDEN_SIZE,&B[0]);
    F_CREATE_MATRIX(1,size_output,&B[1]);

    int size_net=calc_size_net(W,B);    
    double **pnet_value=malloc(sizeof(double)*size_net);

    init(W,B,pnet_value);

    init_network_data(layer_size,neuron_layer);


    //動的メモリの解放
    free(pnet_value);
    F_DELETE_MATRIX(&W[0]);
    F_DELETE_MATRIX(&W[1]);
    F_DELETE_MATRIX(&B[0]);
    F_DELETE_MATRIX(&B[1]);
    return 0;
}
