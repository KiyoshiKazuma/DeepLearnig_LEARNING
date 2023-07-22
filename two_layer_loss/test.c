#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"
#include "teacher_file.h"

void main(){
    S_MATRIX X;
    S_MATRIX T;
    F_CREATE_MATRIX(1,2,&X);
    F_CREATE_MATRIX(1,2,&T);
    X.elep[0]=0.0;
    X.elep[1]=1.0;
    T.elep[0]=1.0;
    T.elep[1]=0.0;
    int ret=add_teacher_data(2,2,X,T);
    if(ret!=0){
        printf("ERROR:something happen!\n");
    }
}