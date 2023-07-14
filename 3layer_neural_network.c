#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <math.h>
#include "matrix.h"

#define DEBUG

double sigmoid(double x);
int main(int argc, char* argv[]) {

#ifndef DEBUG
    if (argc != 3) {
        printf("ERROR : set two number\n");
        return -1;
    }
#endif
    S_MATRIX X;
    S_MATRIX Y;
    S_MATRIX W[3];
    S_MATRIX B[3];
    S_MATRIX A[3];
    S_MATRIX Z[4];
    S_MATRIX ret;
    int i;

    F_CREATE_MATRIX(1, 2, &X);
#ifdef DEBUG
    X.elep[0] = 1.0;
    X.elep[1] = 0.5;
#else
    X.elep[0] = atof(argv[1]);
    X.elep[1] = atof(argv[2]);

#endif // DEBUG

    F_CREATE_MATRIX(1, 2, &Y);
    F_CREATE_MATRIX(2, 3, &W[0]);
    F_CREATE_MATRIX(3, 2, &W[1]);
    F_CREATE_MATRIX(2, 2, &W[2]);
    F_CREATE_MATRIX(1, 3, &B[0]);
    F_CREATE_MATRIX(1, 2, &B[1]);
    F_CREATE_MATRIX(1, 2, &B[2]);
    F_CREATE_MATRIX(1, 2, &A[0]);
    F_CREATE_MATRIX(1, 3, &A[1]);
    F_CREATE_MATRIX(1, 2, &A[2]);
    F_CREATE_MATRIX(1, 2, &Z[0]);
    F_CREATE_MATRIX(1, 3, &Z[1]);
    F_CREATE_MATRIX(1, 2, &Z[2]);
    F_CREATE_MATRIX(1, 2, &Z[3]);

    W[0].elep[0] = 0.1;
    W[0].elep[1] = 0.3;
    W[0].elep[2] = 0.5;
    W[0].elep[3] = 0.2;
    W[0].elep[4] = 0.4;
    W[0].elep[5] = 0.6;
    W[1].elep[0] = 0.1;
    W[1].elep[1] = 0.4;
    W[1].elep[2] = 0.2;
    W[1].elep[3] = 0.5;
    W[1].elep[4] = 0.3;
    W[1].elep[5] = 0.6;
    W[2].elep[0] = 0.1;
    W[2].elep[1] = 0.3;
    W[2].elep[2] = 0.2;
    W[2].elep[3] = 0.4;
    B[0].elep[0] = 0.1;
    B[0].elep[1] = 0.2;
    B[0].elep[2] = 0.3;
    B[1].elep[0] = 0.1;
    B[1].elep[1] = 0.2;
    B[2].elep[0] = 0.1;
    B[2].elep[1] = 0.2;

    Z[0].elep[0] = X.elep[0];
    Z[0].elep[1] = X.elep[1];

    printf("X    %4.4f  %f  \n", Z[0].elep[0], Z[0].elep[1]);

    for (i = 0; i < 3; i++) {

        printf("W %d  \n", i);
        F_PRINT(&W[i]);
        printf("B %d  \n", i);
        F_PRINT(&B[i]);

        F_CREATE_MATRIX(1, A->column, &ret);
        F_PRODUCT(&Z[i], &W[i], &ret);
        printf("ret %d  ", i);
        F_PRINT(&ret);
        F_ADD(&B[i], &ret, &A[i]);
        F_FUNCTION(&A[i], &Z[i + 1], sigmoid);
        F_DELETE_MATRIX(&ret);
printf("A %d  ", i);
        F_PRINT(&A[i]);
        printf("Z %d  ", i);
        F_PRINT(&Z[i+1]);
        printf("\n");
    }

    Y.elep[0] = Z[3].elep[0];
    Y.elep[1] = Z[3].elep[1];

    printf("Y    %4.4f  %f  \n", Y.elep[0], Y.elep[1]);

    return 0;

}


double sigmoid(double x) {
    double EXP = exp(-x);
    return 1 / (1 + EXP);
}
