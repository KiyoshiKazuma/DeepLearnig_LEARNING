#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"

#define REPEAT_COUNT (1000000)
#define RATE (-1)
void init(S_MATRIX* W,S_MATRIX* B,double **pnet_value);

int gradient_descent(S_MATRIX* X,S_MATRIX *T){
    int i,j;
    S_MATRIX W[2];
    S_MATRIX B[2];
    S_MATRIX Y;

    FILE * fp;
    fp=fopen("dat","w");
    int count=0;
    int next_count=1;

    F_CREATE_MATRIX(1,OUTPUT_SIZE,&Y);
    F_CREATE_MATRIX(INPUT_SIZE,HIDEN_SIZE,&W[0]);
    F_CREATE_MATRIX(HIDEN_SIZE,OUTPUT_SIZE,&W[1]);
    F_CREATE_MATRIX(1,HIDEN_SIZE,&B[0]);
    F_CREATE_MATRIX(1,OUTPUT_SIZE,&B[1]);
    int size_net=calc_size_net(W,B);
    
    double **pnet_value=malloc(sizeof(double)*size_net);
    double * dL = malloc(sizeof(double)*size_net);

    init(W,B,pnet_value);

    for(i=0;i<REPEAT_COUNT;i++){
        calc_gradient(W,B,X,T,dL);
        for(j=0;j<size_net;j++){
            **(pnet_value+j)+=RATE*dL[j];
        }        
        two_layer_net(X,&Y,W,B);
        count++;
        if(count==next_count){
            next_count*=2;
            fprintf(fp,"%d,%f\n",i,cross_entropy_error(&Y,T));
            printf("\n%d\n",i);
            F_PRINT(&Y);
            F_PRINT(T);
            F_PRINT(&W[0]);
            F_PRINT(&W[1]);
            F_PRINT(&B[0]);
            F_PRINT(&B[1]);
        }             
        
    }
    fclose(fp);

    return 0;
}


void init(S_MATRIX* W,S_MATRIX* B,double **pnet_value){
    int i,j,n;
        
    //ネットワーク変数に初期値を入れる。
    for(i=0;i<2;i++){
        for(j=0;j<SIZE(W[i]);j++){
            W[i].elep[j]=0.5;
        }
    }
    for(i=0;i<2;i++){
        for(j=0;j<SIZE(B[i]);j++){
            B[i].elep[j]=0.5;
        }
    }

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
}