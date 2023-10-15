#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "neural_network.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"
#define DH (0.1)

int calc_gradient(S_NETWORK net,S_MATRIX X,S_MATRIX T,S_MATRIX * vW,S_MATRIX * vB ,double * vret){
    double Lb,La;
    int i,j,n;
    S_MATRIX Y;
    S_MATRIX * vWtmp=malloc(sizeof(S_MATRIX)*net.layer_size-1);
    S_MATRIX * vBtmp=malloc(sizeof(S_MATRIX)*net.layer_size-1);
    
    for(i=0;i<net.layer_size-1;i++){
        F_CREATE_MATRIX(net.neurons_size[i],net.neurons_size[i+1],&vWtmp[i]);
    }
    for(i=0;i<net.layer_size-1;i++){
        F_CREATE_MATRIX(net.neurons_size[i],net.neurons_size[i+1],&vBtmp[i]);
    }
    F_CREATE_MATRIX(1,net.neurons_size[net.layer_size-1],&Y);

    double **pnet_value=malloc(sizeof(double)*net.net_amount);
    double **pnet_value_tmp=malloc(sizeof(double)*net.net_amount);

    //ネットワーク変数を一つの配列に格納
    n=0;
    for(i=0;i<net.layer_size-1;i++){
        for(j=0;j<SIZE(vW[i]);j++){
            *(pnet_value+n)=vW[i].elep+j;
            *(pnet_value_tmp+n)=vWtmp[i].elep+j;
            n++;
        }
    }
    for(i=0;i<net.layer_size-1;i++){
        for(j=0;j<SIZE(vB[i]);j++){
            *(pnet_value+n)=&(vB[i].elep[j]);            
            *(pnet_value_tmp+n)=&(vBtmp[i].elep[j]);             
            n++;
        }
    }
    
    //各ネットワーク変数の微分を求める
    for(i=0;i<net.net_amount;i++){
        
        //ネットワーク変数をtmpネットワーク変数にコピー
        for(j=0;j<net.net_amount;j++){
            *(pnet_value_tmp[j])=*(pnet_value[j]);
        }
        //tmpネットワーク変数のi番目の要素を微減し損失を計算
        *(pnet_value_tmp[i])-=DH;
        neural_network(net,X,&Y,vWtmp,vBtmp);
        Lb=cross_entropy_error(Y,T);

#ifdef DEBUG
        printf("---%d---\n",i);
        printf("%f\t",Lb);
        for(j=0;j<net.net_amount;j++){
            printf("%f\t",*(pnet_value_tmp[j]));
        }
        printf("\n");
#endif 

        //tmpネットワーク変数のi番目の要素を微増し損失を計算
        *(pnet_value_tmp[i])+=2*DH;
        neural_network(net,X,&Y,vWtmp,vBtmp);
        La=cross_entropy_error(Y,T);
#ifdef DEBUG
        printf("%f\t",La);
        for(j=0;j<net.net_amount;j++){
            printf("%f\t",*(pnet_value_tmp[j]));
        }
        printf("\n");
#endif        
        //損失のネットワーク変数のi番目の要素の微分
        vret[i]=(La-Lb)/(2.0*DH);
    }

#ifdef DEBUG
    for(i=0;i<size_net;i++){
        printf("%3.3f\t",vret[i]);
    }
    printf("\n");
#endif
    free(pnet_value);
    free(pnet_value_tmp);
    F_DELETE_MATRIX(&Y);
    for(i=0;i<net.layer_size-1;i++){
        F_DELETE_MATRIX(&vWtmp[i]);
    }
    for(i=0;i<net.layer_size-1;i++){
        F_DELETE_MATRIX(&vBtmp[i]);
    }
    free(vWtmp);
    free(vBtmp);
    return 0;
}