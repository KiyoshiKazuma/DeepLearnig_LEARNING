#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"matrix.h"
#include"neural_network.h"
#include "configuration.h"

double sigmoid(double x);

int neural_network(S_NETWORK net,S_MATRIX X,S_MATRIX *pY,S_MATRIX *vW,S_MATRIX *vB){
    S_MATRIX *vA=malloc(sizeof(S_MATRIX)*net.layer_size-1);
    S_MATRIX *vZ=malloc(sizeof(S_MATRIX)*net.layer_size);
    S_MATRIX ret;
    int i;
    for(i=0;i<net.layer_size-1;i++){
        F_CREATE_MATRIX(1,net.neurons_size[i+1],&vA[i]);
    }
    for(i=0;i<net.layer_size;i++){
        F_CREATE_MATRIX(1,net.neurons_size[i],&vZ[i]);
    }

    for(i=0;i<net.neurons_size[0];i++){
        vZ[0].elep[i]=X.elep[i];
    }
    
    for(i=0;i<net.layer_size-1;i++){
        F_CREATE_MATRIX(1,vA[i].column,&ret);
        F_PRODUCT(&vZ[i],&vW[i],&ret);
        if(F_ADD(&vB[i],&ret,&vA[i])){
            printf("B %d\n",i);
            F_PRINT(&vB[i]);
            printf("ret %d\n",i);
            F_PRINT(&ret);
        }
        F_FUNCTION(&vA[i],&vZ[i+1],sigmoid);
        F_DELETE_MATRIX(&ret);
    }

    F_SOFTMAX(&vZ[net.layer_size-1],pY);
    
    for(i=0;i<net.layer_size-1;i++){
        F_DELETE_MATRIX(&vA[i]);
    }
    for(i=0;i<net.layer_size;i++){
        F_DELETE_MATRIX(&vZ[i]);
    }
    free(vA);
    free(vZ);
    return 0;    
}

double sigmoid(double x) {
    double EXP = exp(-x);
    double ret =1 / (1 + EXP); 
//    printf("--sigmoid--\n%f -->  %f \n",x,ret);
    return ret;
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