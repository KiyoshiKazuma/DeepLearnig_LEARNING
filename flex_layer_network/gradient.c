#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"
#define DH (0.1)

int calc_gradient(S_MATRIX* W,S_MATRIX* B,S_MATRIX* X, S_MATRIX* T,double * ret){
    double Lb,La;
    int size_net,i,j,n;
    S_MATRIX Y;
    S_MATRIX Wtmp[2];
    S_MATRIX Btmp[2];
    F_CREATE_MATRIX(INPUT_SIZE,HIDEN_SIZE,&Wtmp[0]);
    F_CREATE_MATRIX(HIDEN_SIZE,OUTPUT_SIZE,&Wtmp[1]);
    F_CREATE_MATRIX(1,HIDEN_SIZE,&Btmp[0]);
    F_CREATE_MATRIX(1,OUTPUT_SIZE,&Btmp[1]);
    F_CREATE_MATRIX(1,2,&Y);

    size_net=calc_size_net(W,B);   
    double **pnet_value=malloc(sizeof(double)*size_net);
    double **pnet_value_tmp=malloc(sizeof(double)*size_net);

    //ネットワーク変数を一つの配列に格納
    n=0;
    for(i=0;i<2;i++){
        for(j=0;j<SIZE(W[i]);j++){
            *(pnet_value+n)=W[i].elep+j;
            *(pnet_value_tmp+n)=Wtmp[i].elep+j;
            n++;
        }
    }
    for(i=0;i<2;i++){
        for(j=0;j<SIZE(B[i]);j++){
            *(pnet_value+n)=B[i].elep+j;            
            *(pnet_value_tmp+n)=Btmp[i].elep+j;             
            n++;
        }
    }
    
    //各ネットワーク変数の微分を求める
    for(i=0;i<size_net;i++){
        
        //ネットワーク変数をtmpネットワーク変数にコピー
        for(j=0;j<size_net;j++){
            *(pnet_value_tmp[j])=*(pnet_value[j]);
        }
        //tmpネットワーク変数のi番目の要素を微減し損失を計算
        *(pnet_value_tmp[i])-=DH;
        two_layer_net(X,&Y,Wtmp,Btmp);
        Lb=cross_entropy_error(&Y,T);

#ifdef DEBUG
        printf("---%d---\n",i);
        printf("%f\t",Lb);
        for(j=0;j<size_net;j++){
            printf("%f\t",*(pnet_value_tmp[j]));
        }
        printf("\n");
#endif 

        //tmpネットワーク変数のi番目の要素を微増し損失を計算
        *(pnet_value_tmp[i])+=2*DH;
        two_layer_net(X,&Y,Wtmp,Btmp);
        La=cross_entropy_error(&Y,T);
#ifdef DEBUG
        printf("%f\t",La);
        for(j=0;j<size_net;j++){
            printf("%f\t",*(pnet_value_tmp[j]));
        }
        printf("\n");
#endif        
        //損失のネットワーク変数のi番目の要素の微分
        ret[i]=(La-Lb)/(2.0*DH);
    }

#ifdef DEBUG
    for(i=0;i<size_net;i++){
        printf("%3.3f\t",ret[i]);
    }
    printf("\n");
#endif
    free(pnet_value);
    free(pnet_value_tmp);
    F_DELETE_MATRIX(&Y);
    F_DELETE_MATRIX(&Wtmp[0]);
    F_DELETE_MATRIX(&Wtmp[1]);
    F_DELETE_MATRIX(&Btmp[0]);
    F_DELETE_MATRIX(&Btmp[1]);
    return 0;
}