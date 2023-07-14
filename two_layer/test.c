#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"

void init(S_MATRIX* W,S_MATRIX* B,double **pnet_value);

void main(){
    int i;
    S_MATRIX X;
    S_MATRIX T;
    F_CREATE_MATRIX(1,2,&X);
    F_CREATE_MATRIX(1,2,&T);
    X.elep[0]=1.0;
    X.elep[1]=0.0;
    T.elep[0]=0.6;
    T.elep[1]=0.4;
    gradient_descent(&X,&T);

}