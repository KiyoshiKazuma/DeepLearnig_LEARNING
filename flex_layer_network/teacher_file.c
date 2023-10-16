#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "teacher_file.h"
#include "configuration.h"

/*teacher fileを初期化*/
//引数  size_X  入力データサイズ
//      size_T  出力データサイズ
//戻り値 0:正常
//      -1:ファイル異常
int init_teacher_file(int size_X,int size_T){
    FILE * fp;
    fp=fopen(TEACHER_FILE_NAME,"w");
    if(fp==NULL){
        fclose(fp);
        return -1;
    }
    fprintf(fp,"%d\n%d",size_X,size_T);
    fclose(fp);
    return 0;
}

/*教師データをデータファイルから読み込む*/
//引数  X:入力データの行列 
//      T:正解データの行列
//戻り値    正数：データの総数
//          -1:ファイルエラー
//          -2:ポインタエラー
//          -3:そのほかのエラー
int read_teacher_data(S_NETWORK * pnet,S_MATRIX *vX,S_MATRIX *vT){
    int block_size=0;
    int i;
    int SIZE_X;
    int SIZE_T;
    double buf;
    FILE * fp=NULL;
    
    //File open
    fp=fopen(TEACHER_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }

    //skip header file
    fscanf(fp,"%d",&SIZE_X);
    fscanf(fp,"%d",&SIZE_T);

    block_size= SIZE_T+SIZE_X;

    //read data
    for(i=0;i<TEACHER_MAX_SIZE*block_size;i++){
        char ret=fscanf(fp,"%lf",&buf);
        if(ret==EOF){
            break;
        }
        int block_num=i/block_size;
        int data_pos=i%block_size;
        #ifdef D_DEBUG
            printf("#debug line:%d  dataset_num:%d  data_pos:%d \n",i+4,block_num,data_pos);
            if(data_pos<*size_X){
                printf("#debug data:%f is storage in X[%d] (0,%d)\n",buf,block_num,data_pos);
                X[block_num].elep[data_pos]=buf;
            }else{
                printf("#debug data:%f is storage in T[%d] (0,%d)\n",buf,block_num,data_pos-*size_X);
            }    
        #endif
        if(data_pos<SIZE_X){
            vX[block_num].elep[data_pos]=buf;
        }else{
            vT[block_num].elep[data_pos-SIZE_X]=buf;
        }
    }
    fclose(fp);
    pnet->teacher_size=i/block_size;
    return 0;
}

/*教師データをデータファイルに追加する*/
//引数　size_X  入力パラメタ数
//      size_T  出力パラメタ数
//      X:入力データの行列 
//      T:正解データの行列
//戻り値    正数：データの総数
//          -1:ファイルエラー
//          -3:そのほかのエラー
int add_teacher_data(S_NETWORK * pnet,S_MATRIX X,S_MATRIX T){
    int SIZE_X;
    int SIZE_T;
    FILE * fp;

    //File open
    fp=fopen(TEACHER_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }

    //read header file
    fscanf(fp,"%d",&SIZE_X);
    fscanf(fp,"%d",&SIZE_T);
    fclose(fp);

    //追加データが既存ファイルと形式が一致するか確認
    if(SIZE_X!=pnet->neurons_size[0]||SIZE_T!=pnet->neurons_size[pnet->layer_size-1]){
        printf("ERROR data structure not match with the statement in file\n");
        return -1;
    }

    //追記モードでファイルをオープン
    fp=fopen(TEACHER_FILE_NAME,"a");
    if(fp==NULL){
        return -1;
    }
    for(int i=0;i<SIZE_X;i++){
            fprintf(fp,"\n%lf",X.elep[i]);
    }
    for(int i=0;i<SIZE_T;i++){
            fprintf(fp,"\n%lf",T.elep[i]);
    }
    fclose(fp);

    return 0;
}