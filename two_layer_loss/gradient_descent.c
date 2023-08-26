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

#define RATE (0.1)
//#define D_DEBUG
//#define MULTI


//勾配降下法
//概要：1つのデータに対する学習を行う
//引数  S_MATRIX *W :ネットワーク変数へのポインタ
//      S_MATRIX *B :ネットワーク変数へのポインタ
//      S_MATRIX *X :教師データ入力ベクトル
//      S_MATRIX *T :教師データ出力ベクトル
//      int     learning_size :学習を実行するデータの数
//戻り値     0:正常終了
//          -1:ポインタエラー
//          -2:ファイルエラー
//          -3:そのほかエラー
int gradient_descent(S_MATRIX *W,S_MATRIX *B,S_MATRIX* X,S_MATRIX *T,int learning_size){
    //NULL CHECK
    if(W==NULL || B==NULL || X==NULL ||T==NULL){
        return -1;
    }

    int size_net=calc_size_net(W,B);
    double **pnet_value=malloc(sizeof(double)*size_net);
    double * dL = malloc(sizeof(double)*size_net);
    double * rate=malloc(sizeof(double)*size_net);

    aggregate_network_data(W,B,pnet_value);
    //重み変数を計算
    for(int i=0;i<size_net;i++){
        rate[i]=1.0;
    }

    //学習を実行
    for(int i=0;i<learning_size;i++){
        calc_gradient(W,B,&X[i],&T[i],dL);
        for(int k=0;k<size_net;k++){
            *(pnet_value[k])-=RATE*rate[k]*dL[k];
        }
    }

    //動的メモリの解放
    free(pnet_value);
    free(dL);
    free(rate);
  
    return 0;
}

//ネットワーク変数を集約する
//概要：ネットワーク変数の行列W,Bを一つの配列pnet_valueに集約する
//引数  S_MATRIX *W :ネットワーク変数へのポインタ
//      S_MATRIX *B :ネットワーク変数へのポインタ
//      double **pnet_value :ネットワーク行列の各変数へのポインタ列
//戻り値     0:正常終了
//          -1:ポインタエラー
//          -2:ファイルエラー
//          -3:そのほかエラー
int aggregate_network_data(S_MATRIX* W,S_MATRIX* B,double **pnet_value){
    //NULL CHECK
    if(W==NULL || B==NULL){
        return -1;
    }

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
    return 0;
}
