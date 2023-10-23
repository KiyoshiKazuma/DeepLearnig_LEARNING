#include "loss_function.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "configuration.h"

//#define D_DEBUG

double mean_squared_error(S_MATRIX Y,S_MATRIX T){
    int i;
    double ret =0;
    if(Y.row!=1 || T.row!=1 || Y.column != T.column){
        printf("###ERROR##mean_squared_error\tinput error\n");
        return 0;
    }
    for(i=0;i<Y.column;i++){
        ret += (Y.elep[i]-T.elep[i])*(Y.elep[i]-T.elep[i]);
    }
    ret /=2;
    return ret;
}

double cross_entropy_error(S_MATRIX Y,S_MATRIX T){
    int i;
    double ret =0;
    if(Y.row!=1 || T.row!=1 || Y.column != T.column){
        printf("###ERROR##closs_entropy_error\tinput error\n");
        return 0;
    }
    #ifdef D_DEBUG
        printf("#DEBUG# cross_entropy_error\n");
        printf("expected : ");
        for(i=0;i<T.column;i++)printf("\t%f",T.elep[i]);
        printf("\ncalculated : ");        
        for(i=0;i<Y.column;i++)printf("\t%f",Y.elep[i]);
        printf("\n");
        printf("-----------------------------\n");
    #endif //D_DEBUG
    for(i=0;i<Y.column;i++){
        ret -= T.elep[i]*log(Y.elep[i]);
    }
    return ret;
}