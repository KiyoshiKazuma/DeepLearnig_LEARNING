#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "neural_network.h"
#include "loss_function.h"
#include "gradient.h"
#include "teacher_file.h"
#include "network_data.h"
#include "configuration.h"
#include "gradient_descent.h"

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
int gradient_descent(S_NETWORK net,S_MATRIX *vX,S_MATRIX *vT, S_MATRIX * vW,S_MATRIX * vB,int learning_size){
    double **pnet_value=malloc(sizeof(double)*net.net_amount);
    double * dL = malloc(sizeof(double)*net.net_amount);
    double * rate=malloc(sizeof(double)*net.net_amount);

    aggregate_network_data(net,vW,vB,pnet_value);
    //重み変数を計算
    for(int i=0;i<net.net_amount;i++){
        rate[i]=1.0;
    }

    //学習を実行
    for(int i=0;i<learning_size;i++){
        calc_gradient(net,vX[i],vT[i],vW,vB,dL);
        for(int k=0;k<net.net_amount;k++){
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
int aggregate_network_data(S_NETWORK net,S_MATRIX* vW,S_MATRIX* vB,double **pnet_value){
    int i,j,n;

    //ネットワークの変数W,Bの各要素へのポインタの配列を作成。
    n=0;
    for(i=0;i<net.layer_size-1;i++){
        for(j=0;j<SIZE(vW[i]);j++){
            *(pnet_value+n)=vW[i].elep+j;
            n++;
        }
    }
    for(i=0;i<net.layer_size-1;i++){
        for(j=0;j<SIZE(vB[i]);j++){
            *(pnet_value+n)=vB[i].elep+j;             
            n++;
        }
    }
    return 0;
}
