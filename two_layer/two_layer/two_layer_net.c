#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"matrix.h"
#include"two_layer_net.h"

double sigmoid(double x);

int two_layer_net(S_MATRIX * input,S_MATRIX * output,S_MATRIX *W,S_MATRIX *B){
    S_MATRIX A[2];
    S_MATRIX Z[3];
    S_MATRIX Y;
    S_MATRIX ret;
    int i;
    F_CREATE_MATRIX(1,HIDEN_SIZE,&A[0]);
    F_CREATE_MATRIX(1,OUTPUT_SIZE,&A[1]);
    F_CREATE_MATRIX(1,INPUT_SIZE,&Z[0]);
    F_CREATE_MATRIX(1,HIDEN_SIZE,&Z[1]);
    F_CREATE_MATRIX(1,OUTPUT_SIZE,&Z[2]);
    F_CREATE_MATRIX(1,OUTPUT_SIZE,&Y);

    for(i=0;i<2;i++){
        A[0].elep[i]=input->elep[i];
    }
    
    for(i=0;i<2;i++){
        F_CREATE_MATRIX(1,A[i].column,&ret);
        F_PRODUCT(&Z[i],&W[i],&ret);
        F_ADD(&B[i],&ret,&A[i]);
        F_FUNCTION(&A[i],&Z[i+1],sigmoid);
        F_DELETE_MATRIX(&ret);
    }
    F_SOFTMAX(&Z[i+1],&Y);
    
    for(i=0;i<2;i++){
        output->elep[i]=Y.elep[i];
    }
    return 0;    
}

double sigmoid(double x) {
    double EXP = exp(-x);
    return 1 / (1 + EXP);
}

int calc_size_net(S_MATRIX* W,S_MATRIX*B){
    int size=0;
    int i;
    for(i=0;i<2;i++){
        size += SIZE(W[i]);
        size += SIZE(B[i]);
    }
    return size;
}