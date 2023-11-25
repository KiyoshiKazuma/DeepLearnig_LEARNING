#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "teacher_file.h"
#include "configuration.h"

#include <time.h>

//#define D_DEBUG
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
int read_teacher_size(S_NETWORK * pnet){
    int SIZE_X;
    int SIZE_T;
    int block_size;
    int count=0;
    float buf;
    FILE * fp;
    fp=fopen(TEACHER_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }
    //skip header file
    fscanf(fp,"%d",&SIZE_X);
    fscanf(fp,"%d",&SIZE_T);

    while(fscanf(fp,"%f",&buf)!=EOF)count++;
    
    block_size=SIZE_X+SIZE_T;

    if(count%block_size!=0){
        printf("WARNIGN read_teacher_size : teacher file size is not consistent.\n");
    }
    pnet->teacher_size=count/block_size;
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
int read_teacher_data(S_NETWORK * pnet,S_MATRIX vX[],S_MATRIX vT[]){
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
    for(i=0;i<pnet->teacher_size*block_size;i++){
        char ret=fscanf(fp,"%lf",&buf);
        if(ret==EOF){
            break;
        }
        int block_num=i/block_size;
        int data_pos=i%block_size;

        if(data_pos<SIZE_X){
            vX[block_num].elep[data_pos]=buf;
        }else{
            vT[block_num].elep[data_pos-SIZE_X]=buf;
        }
    }
    fclose(fp);
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

    // //追加データが既存ファイルと形式が一致するか確認
    // if(SIZE_X!=pnet->neurons_size[0]||SIZE_T!=pnet->neurons_size[pnet->layer_size-1]){
    //     printf("ERROR in add_teacher_data : data structure not match with the statement in file\n");
    //     return -1;
    // }

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


/*関数　pick_random_teacher_dataset*/
/*概要　教師データセットから指定数のデータセットをランダムに抽出する*/
/*引数  教師データセット入力    S_MATRIX * vX
/*      教師データセット正解    S_MATRIX * vT
/*      抽出データセット入力    S_MATRIX * vX_out
/*      抽出データセット正解     S_MATRIX * vT_out
/*      教師データ数            int size_teacher
/*      抽出数                  int size_out */
/*戻り値     0  正常終了*/
/*          -1  ポインタエラー*/
/*          -2そのほかエラー*/
int pick_random_teacher_dataset(S_MATRIX * vX,S_MATRIX * vT,S_MATRIX * vX_out,S_MATRIX * vT_out,int size_teacher,int size_out){
    
    srand((unsigned int)time(NULL));
    //NULL CHECK
    if(vX==NULL || vT==NULL || vX_out==NULL || vT_out==NULL){
        return -1;
    }
    if(size_teacher<size_out){
        printf("ERROR ###pick_random_teacher_dataset###\n\tinput error : size_out is larger than size_teacher\n");
        return -2;
    }

    int data_arrow[TEACHER_MAX_SIZE];
    int data_arrow_out[TEACHER_MAX_SIZE];
    
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
        for(int j=0;j<vX->column;j++){
            vX_out[i].elep[j]=vX[data_arrow_out[i]].elep[j];
        }
        for(int j=0;j<vT->column;j++){
            vT_out[i].elep[j]=vT[data_arrow_out[i]].elep[j];
        }
        #ifdef D_DEBUG
        for(int j=0;j<vX->column;j++){
            printf("\t%lf",vX_out[i].elep[j]);
        }
        for(int j=0;j<vT->column;j++){
            printf("\t%lf",vT_out[i].elep[j]);
        }
        printf("\n");
        #endif//D_DEBUG
    }

    return 0;
}