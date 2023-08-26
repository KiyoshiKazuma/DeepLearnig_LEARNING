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
#include "network_data.h"
#include "configuration.h"
#include "gradient_descent.h"

#define REPEAT_MAX (1000)
#define THRESHOULD (0.001)
#define D_DEBUG
#define MULTI

int pick_random_teacher_dataset(S_MATRIX * X,S_MATRIX * T,S_MATRIX *X_out,S_MATRIX * T_out,int size_teacher,int size_out);

int main(void){
    //変数宣言
    srand((unsigned int)time(NULL));
    int input_size,output_size;
    int hiden_size[NETWORK_MAX_LAYER];
    int teacher_size;
    int layer_size;
    int ret;
    int network_size;
    double ** pnet_value;
    FILE * fp;
    fp=fopen("data.dat","w");
    S_MATRIX W[NETWORK_MAX_LAYER];
    S_MATRIX B[NETWORK_MAX_LAYER];
    S_MATRIX X[TEACHER_MAX_SIZE];
    S_MATRIX T[TEACHER_MAX_SIZE];
    S_MATRIX X_epoch[EPOCH_SIZE];
    S_MATRIX T_epoch[EPOCH_SIZE];
    S_MATRIX X_test[TEST_SIZE];
    S_MATRIX T_test[TEST_SIZE];
    S_MATRIX Y;
    
    //教師データを読み込む
    teacher_size=read_teacher_data(&input_size,&output_size,X,T);
    if(teacher_size<0){
        printf("ERROR occor in \"read_teacher_data\"\n");
        printf("error code %d\n",teacher_size);
    }

    //ネットワーク変数データを読み込む
    network_size=get_network_info(&layer_size,&hiden_size);
    ret=read_network_data(pnet_value,network_size);

    //学習用パラメータをセット
    for(int i=0;i<layer_size;i++){
        F_CREATE_MATRIX(network_size[i],network_size[i+1],W[i]);
        F_CREATE_MATRIX(1,network_size[i])
    }
    for(int i=0;i<teacher_size;i++){
        F_CREATE_MATRIX(1,input_size,&X[i]);
        F_CREATE_MATRIX(1,output_size,&T[i]);
    }
    for(int i=0;i<EPOCH_SIZE;i++){
        F_CREATE_MATRIX(1,input_size,&X_epoch[i]);
        F_CREATE_MATRIX(1,output_size,&T_epoch[i]);
    }
    for(int i=0;i<TEST_SIZE;i++){
        F_CREATE_MATRIX(1,input_size,&X_test[i]);
        F_CREATE_MATRIX(1,output_size,&T_test[i]);
    }
    F_CREATE_MATRIX(1,HIDEN_SIZE,Y);

    int size_net=calc_size_net(W,B);    
    double **pnet_value=malloc(sizeof(double)*size_net);
    double * dL = malloc(sizeof(double)*size_net);

    aggregate_network_data(W,B,pnet_value);

    //ネットワーク変数の初期値をランダムに生成
    //OR
    //ネットワークデータを読み込み
    printf("init network file? (y/n) :");
    char cmd;
    scanf("%c",&cmd);
    if(cmd=='y'){
        for(int i=0;i<size_net;i++){
            *(pnet_value[i])=rand()%100/100.0;
        }
    }else{
        read_network_data(pnet_value,size_net);
    }

    for(int i=0;i<REPEAT_MAX;i++){
        //学習データを生成
        ret=pick_random_teacher_dataset(X,T,X_epoch,T_epoch,teacher_size,EPOCH_SIZE);
        if(ret!=0){
            printf("ERROR ###deep_learning.c###\n\terror in \'pick_random_teacher_dataset\'\n");
            break;
        }
        //testデータを生成
        ret=pick_random_teacher_dataset(X,T,X_test,T_test,teacher_size,TEST_SIZE);
        if(ret!=0){
            printf("ERROR ###deep_learning.c###\n\terror in \'pick_random_teacher_dataset\'\n");
            break;
        }
        //学習を実行
        ret=gradient_descent(W,B,X_epoch,T_epoch,EPOCH_SIZE);
        if(ret!=0){
            printf("ERROR ###deep_learning.c###\n\terror in \'gradient_descent\'\n");
            break;
        }

        //testを実行
        double error=0;
        for(int j=0;j<TEST_SIZE;j++){
            two_layer_net(&X_test[j],&Y,W,B);
            error+=cross_entropy_error(&Y,&T[T_test]);
        }
        error/=TEST_SIZE;

        //test結果をログファイルに出力
        fprintf(fp,"%d,%lf\n",i,error);

        //終了条件を判定
        if(error<THRESHOULD){
            printf("END LEARNING\n");
            break;
        }
    }
    

    //学習データをファイルに記録
    update_network_data(pnet_value,size_net);

    //動的メモリの解放
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


/*関数　pick_random_teacher_dataset*/
/*概要　教師データセットから指定数のデータセットをランダムに抽出する*/
/*引数  教師データセット入力    S_MATRIX * X*/
/*      教師データセット正解    S_MATRIX * T*/
/*      抽出データセット入力    S_MATRIX * X_out*/
/*      抽出データセット正解     S_MATRIX * T_out*/
/*      教師データ数            int size_teacher*/
/*      抽出数                  int size_out */
/*戻り値     0  正常終了*/
/*          -1  ポインタエラー*/
/*          -2そのほかエラー*/
int pick_random_teacher_dataset(S_MATRIX * X,S_MATRIX * T,S_MATRIX *X_out,S_MATRIX * T_out,int size_teacher,int size_out){
    //NULL CHECK
    if(X==NULL || T==NULL || X_out==NULL || T_out==NULL){
        return -1;
    }
    if(size_teacher<size_out){
        printf("ERROR ###pick_random_teacher_dataset###\n\tinput error : size_out is larger than size_teacher\n");
        return -2;
    }

    int data_arrow[size_teacher];
    int data_arrow_out[size_out];
    for(int i=0;i<size_teacher;i++){
        data_arrow[i]=i;
    }

    for(int i=0;i<size_out;i++){
        int rnd=rand()%(size_teacher-i);
        data_arrow_out[i]=data_arrow[rnd];
        for(int j=rnd;j<size_teacher-i-1;j++){
            data_arrow[j]=data_arrow[j+1];
        }
    }

    for(int i=0;i<size_out;i++){
        X_out[data_arrow_out[i]]=X[data_arrow_out[i]];
        T_out[data_arrow_out[i]]=T[data_arrow_out[i]];
    }

    return 0;
}