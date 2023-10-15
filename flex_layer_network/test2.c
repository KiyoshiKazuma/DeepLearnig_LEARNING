#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "loss_function.h"

int main(){
    S_MATRIX Y;
    S_MATRIX T;
    F_CREATE_MATRIX(1,3,&Y);
    F_CREATE_MATRIX(1,3,&T);
    int i,j;
    double ret;

    for(i=0;i<3;i++){
        Y.elep[i]=(rand()%100)/100.0+10.0;
    }
    for(j=-10;j<10;j++){
        for(i=0;i<3;i++){
            T.elep[i]=Y.elep[i]+0.1*(double)j;
        }
        F_PRINT(&Y);
        F_PRINT(&T);
        ret=cross_entropy_error(&Y,&T);
        printf("cross entropy error  = %f\n\n",ret);
    }
    return 0;
}