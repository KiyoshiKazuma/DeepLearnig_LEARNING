#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"

#define DH (0.01)

int calc_gradient(S_MATRIX* W,S_MATRIX* B,S_MATRIX* X, S_MATRIX* T,double * ret){
    double L,L0;
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

    //変分0の場合の損失L0を求める
    two_layer_net(X,&Y,Wtmp,Btmp);
    L0=cross_entropy_error(&Y,T);

    //各ネットワーク変数の微分を求める
    for(i=0;i<size_net;i++){
        for(j=0;j<size_net;j++){
            *(pnet_value_tmp[j])=*(pnet_value[j]);
        }
        *(pnet_value_tmp[i])+=DH;
        two_layer_net(X,&Y,Wtmp,Btmp);
        L=cross_entropy_error(&Y,T);
        ret[i]=(L-L0);
    }

    free(pnet_value);
    free(pnet_value_tmp);
    F_DELETE_MATRIX(&Y);
    F_DELETE_MATRIX(&Wtmp[0]);
    F_DELETE_MATRIX(&Wtmp[1]);
    F_DELETE_MATRIX(&Btmp[0]);
    F_DELETE_MATRIX(&Btmp[1]);
    return 0;
}