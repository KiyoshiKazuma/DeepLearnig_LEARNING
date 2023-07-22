#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "teacher_file.h"

#define REPEAT_COUNT (100)
#define REPEAT_COUNT2 (300)
#define RATE (0.1)

#define D_DEBUG
#define MULTI

void init(S_MATRIX* W,S_MATRIX* B,double **pnet_value);
int read_teacher_data(int *size_X,int *size_T,S_MATRIX *X,S_MATRIX *T);

int main(void){
    srand((unsigned int)time(NULL));
    int i,j,k,n,ret;
    int size_teacher=0;
    int parm_size_input=0;
    int parm_size_output=0;
    S_MATRIX W[2];
    S_MATRIX B[2];
    S_MATRIX Y;
    S_MATRIX X[MAX_SIZE_TEACHER];
    S_MATRIX T[MAX_SIZE_TEACHER];

    FILE * fp;
    fp=fopen("tmp.dat","w");
    size_teacher=read_teacher_data(&parm_size_input,&parm_size_output,X,T);
    if(size_teacher<0){
        printf("ERROR occor in \"read_teacher_data\"\n");
        printf("error code %d\n",size_teacher);
    }
    #ifdef D_DEBUG
    printf("%d %d %d\n",size_teacher,parm_size_input,parm_size_output);
    for(i=0;i<size_teacher;i++){
        printf("data set %d \n",i);
        F_PRINT(&X[i]);
        printf("V  V  V  V  V  V  V  V\n",i);
        F_PRINT(&T[i]);
        printf("\n\n");
    }
    #endif

    F_CREATE_MATRIX(1,OUTPUT_SIZE,&Y);
    F_CREATE_MATRIX(INPUT_SIZE,HIDEN_SIZE,&W[0]);
    F_CREATE_MATRIX(HIDEN_SIZE,OUTPUT_SIZE,&W[1]);
    F_CREATE_MATRIX(1,HIDEN_SIZE,&B[0]);
    F_CREATE_MATRIX(1,OUTPUT_SIZE,&B[1]);

    int size_net=calc_size_net(W,B);    
    double **pnet_value=malloc(sizeof(double)*size_net);
    double * dL = malloc(sizeof(double)*size_net);

    init(W,B,pnet_value);
    F_PRINT(&W[0]);
    for(i=0;i<REPEAT_COUNT2;i++){
        for(j=0;j<REPEAT_COUNT;j++){
#ifdef MULTI
            n=rand()%4;
            calc_gradient(W,B,&X[n],&T[n],dL);
            for(k=0;k<size_net;k++){
                *(pnet_value[k])-=RATE*dL[k];
            }
#else
            n=0;
            calc_gradient(W,B,&X[n],&T[n],dL);
            for(k=0;k<size_net;k++){
                *(pnet_value[k])-=RATE*dL[k];
            }
#endif               
        }
        ret=0;
#ifdef MULTI
        fprintf(fp,"%d  ",i);
        for(n=0;n<4;n++){
            two_layer_net(&X[n],&Y,W,B);
            fprintf(fp,"%f  ",Y.elep[0]);
            ret+=cross_entropy_error(&Y,&T[n]);   
        }
        fprintf(fp,"%f\n",ret);
#else        
        two_layer_net(&X[0],&Y,W,B);
        ret=cross_entropy_error(&Y,&T[0]);
        fprintf(fp,"%d  %f\n",i,ret);
#endif   
    }
    

    free(pnet_value);
    free(dL);

    F_DELETE_MATRIX(&Y);
    F_DELETE_MATRIX(&W[0]);
    F_DELETE_MATRIX(&W[1]);
    F_DELETE_MATRIX(&B[0]);
    F_DELETE_MATRIX(&B[1]);
    fclose(fp);
    return 0;
}


void init(S_MATRIX* W,S_MATRIX* B,double **pnet_value){
    int i,j,n;

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

    //ネットワーク変数の初期値をランダムに生成
    for(i=0;i<n;i++){
        *(pnet_value[i])=rand()%100/100.0;
    }
}
