#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#include "neural_network.h"
#include "loss_function.h"
#include "gradient.h"
#include "teacher_file.h"
#include "network_data.h"
#include "configuration.h"
#include "gradient_descent.h"
#include "configuration.h"

#define REPEAT_MAX (1000)
#define THRESHOULD (0.01)
//#define D_DEBUG
#define MULTI


int main(void){
    //変数宣言
    int input_size,output_size;
    S_NETWORK net;
    int ret,i;
    FILE *fp;
    fp=fopen("tmp.dat","w");

    //read layer size
    ret=get_network_info(&net);
    printf("#PHASE# : net work info===========\n");
    printf("layer size : %d\n",net.layer_size);
    for(i=0;i<net.layer_size;i++){
        printf("%d layer : %d\n",i,net.neurons_size[i]);
    }
    printf("====================================================\n");
    
    //read teacher size
    printf("#PHASE# : teacher file info==============\n");
    ret=read_teacher_size(&net);
    printf("teacher size : %d\n",net.teacher_size);
    printf("====================================================\n");
    
    //generate network matrix
    printf("#PHASE# : generate network matrix==============\n");
    S_MATRIX * vW=malloc(sizeof(S_MATRIX)*(net.layer_size-1));
    S_MATRIX * vB=malloc(sizeof(S_MATRIX)*(net.layer_size-1));
    S_MATRIX * vX = malloc(sizeof(S_MATRIX)*net.teacher_size);
    S_MATRIX * vT = malloc(sizeof(S_MATRIX)*net.teacher_size);
    S_MATRIX vX_epoch[EPOCH_SIZE];
    S_MATRIX vT_epoch[EPOCH_SIZE];
    S_MATRIX vX_test[TEST_SIZE];
    S_MATRIX vT_test[TEST_SIZE];
    S_MATRIX Y;    

    input_size=net.neurons_size[0];
    output_size=net.neurons_size[net.layer_size-1];
    
    if(input_size<2||output_size<2||net.layer_size<2){
        printf("\t#WARNING# @main : network size is too small\n");
    }

    for(i=0;i<net.teacher_size;i++){
        F_CREATE_MATRIX(1,input_size,&vX[i]);
    }
    for(i=0;i<net.teacher_size;i++){
        F_CREATE_MATRIX(1,output_size,&vT[i]);
    }
    ret=read_teacher_data(&net,vX,vT);
    if(ret!=0){
        ;
    }

    #ifdef D_DEBUG
    printf("\t#DEBUG# @main : read teacher data set----------\n");
    printf("\tinput data\texpected data\n");
    for(i=0;i<10;i++){
        printf("\t#%d\t,",i);
        for(int j=0;j<vX[i].column;j++)printf("%f\t,",vX[i].elep[j]);
        for(int j=0;j<vT[i].column;j++)printf("%f\t,",vT[i].elep[j]);
        printf("\n");
    }
    printf("\t---------------------------------------------------\n");
    #endif //D_DEBUG
    
    //学習用パラメータをセット
    for(i=0;i<net.layer_size-1;i++){
        F_CREATE_MATRIX(net.neurons_size[i],net.neurons_size[i+1],&vW[i]);
    }
    for(i=0;i<net.layer_size-1;i++){
        F_CREATE_MATRIX(1,net.neurons_size[i+1],&vB[i]);
    }


    for(i=0;i<EPOCH_SIZE;i++){
        F_CREATE_MATRIX(1,input_size,&vX_epoch[i]);
        F_CREATE_MATRIX(1,output_size,&vT_epoch[i]);
    }
    for(i=0;i<TEST_SIZE;i++){
        F_CREATE_MATRIX(1,input_size,&vX_test[i]);
        F_CREATE_MATRIX(1,output_size,&vT_test[i]);
    }
    F_CREATE_MATRIX(1,output_size,&Y);

    
    printf("====================================================\n");

    ret=net_data_amount(&net);
    
    double **pnet_value=malloc(sizeof(double)*net.net_amount);
    double *dL = malloc(sizeof(double)*net.net_amount);

    ret=aggregate_network_data(net,vW,vB,pnet_value);

    //ネットワーク変数の初期値をランダムに生成
    //OR
    //ネットワークデータを読み込み
    printf("#PHASE# : generate network data set============\n");
    printf("#INPUT# : init network file?  (y/n) :");
    char cmd='y';
    scanf("%c",&cmd);
    if(cmd=='y'){
        for(int i=0;i<net.net_amount;i++){
            *(pnet_value[i])=rand()%100/100.0;
        }
    }else{
        read_network_data(net,pnet_value);
    }
    printf("\n====================================================\n");

    printf("#PHASE# : excute deep learning=====================\n");
    /***ミニバッチで学習を実行***/
    for(int i=0;i<REPEAT_MAX;i++){
        printf("%d th patch excute\n",i);
        //学習データを生成
        ret=pick_random_teacher_dataset(vX,vT,vX_epoch,vT_epoch,net.teacher_size,EPOCH_SIZE);
        if(ret!=0){
            printf("\t#ERROR# @main : error in \'pick_random_teacher_dataset\'\n");
            break;
        }
        #ifdef D_DEBUG
        printf("\t#DEBUG# main\n\tmini patch dataset \n");
        printf("\tinput data\t,\texpected data\n");
        for(int j=0;j<EPOCH_SIZE;j++){
            printf("\t#%d\t,",j);
            for(int k=0;k<vX_epoch[j].column;k++)printf("%f\t,",vX_epoch[j].elep[k]);
            for(int k=0;k<vX_epoch[j].column;k++)printf("%f\t,",vX_epoch[j].elep[k]);
            printf("\n");
        }
        printf("\t----------------------------------------------\n");
        #endif//D_DEBUG

        //testデータを生成
        ret=pick_random_teacher_dataset(vX,vT,vX_test,vT_test,net.teacher_size,TEST_SIZE);
        if(ret!=0){
            printf("\t#ERROR# @main : error in \'pick_random_teacher_dataset\'\n");
            break;
        }
        //学習を実行
        ret=gradient_descent(net,vX_epoch,vT_epoch,vW,vB,EPOCH_SIZE);
        if(ret!=0){
            printf("\t#ERROR# @main : error in \'gradient_descent\'\n");
            break;
        }

        //testを実行
        #ifdef D_DEBUG
        printf("\t#DEBUG# @main : test data------------------------\n");
        #endif //D_DEBUG
        
        double error=0;
        for(int j=0;j<TEST_SIZE;j++){
            neural_network(net,vX_test[j],&Y,vW,vB);
            double error_tmp=cross_entropy_error(Y,vT_test[j]);
            error+=error_tmp;
            #ifdef D_DEBUG
            printf("#%d\t,error=%f\n",j,error_tmp);
            printf("X\t:\t");
            for(int k=0;k<vX_test[j].column;k++)printf("%f\t,",vX_test[j].elep[k]);
            printf("\nT\t:\t");
            for(int k=0;k<vT_test[j].column;k++)printf("%f\t,",vT_test[j].elep[k]);
            printf("\nY\t:\t");
            for(int k=0;k<vT_test[j].column;k++)printf("%f\t,",Y.elep[k]);
            printf("\n");
            #endif //D_DEBUG
        }
        error/=TEST_SIZE;

        
        #ifdef D_DEBUG
        printf("\t-----------------------------------------\n");
        #endif //D_DEBUG

        //test結果をログファイルに出力
        fprintf(fp,"%d\t%lf",i,error);
        for(int j=0;j<net.net_amount;j++){
            fprintf(fp,",\tlf",*pnet_value[j]);    
        }
        fprintf(fp,"\n");

        //終了条件を判定
        if(error<THRESHOULD){
            printf("END LEARNING\n");
            break;
        }
    }
    printf("====================================================\n");

    //学習データをファイルに記録
    update_network_data(net,pnet_value);

    //動的メモリの解放
    free(pnet_value);
    free(dL);
    F_DELETE_MATRIX(&Y);
    for(i=0;i<net.layer_size-1;i++)F_DELETE_MATRIX(&vW[i]);
    for(i=0;i<net.layer_size-1;i++)F_DELETE_MATRIX(&vB[i]);
    for(i=0;i<net.teacher_size;i++)F_DELETE_MATRIX(&vX[i]);
    for(i=0;i<net.teacher_size;i++)F_DELETE_MATRIX(&vT[i]);
    for(i=0;i<EPOCH_SIZE;i++)F_DELETE_MATRIX(&vX_epoch[i]);
    for(i=0;i<EPOCH_SIZE;i++)F_DELETE_MATRIX(&vT_epoch[i]);
    for(i=0;i<TEST_SIZE;i++)F_DELETE_MATRIX(&vX_test[i]);
    for(i=0;i<TEST_SIZE;i++)F_DELETE_MATRIX(&vT_test[i]);
    free(vW);
    free(vB);
    free(vX);
    free(vT);
    fclose(fp);
    return 0;
}

