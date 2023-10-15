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
/*teacher data setの基本情報を取得する*/
//引数  int * size_teacher  データセット数を返すポインタ
//      int * size_X    入力データサイズを返すポインタ
//      int * size_T    出力データサイズを返すポインタ
//戻り値    0 :正常
//          -1:データセット数不正
//          -2:ファイルエラー
//          -3:そのほかエラー
int get_teacher_file_info(int * size_teacher,int * size_X,int *size_T){
    //NULL check
    if(size_teacher==NULL || size_X==NULL || size_T==NULL){
        return(-3);
    }
    //file open
    FILE * fp;
    fp=fopen(TEACHER_FILE_NAME,"r");
    if(fp==NULL){
        fclose(fp);
        return -2;
    }

    //file info 取得
    fscanf(fp,"%d",size_X);
    fscanf(fp,"%d",size_T);

    int line_size =(*size_teacher)*(*size_T)*(*size_X);
    int line=0;
    char buf[128];
    while(fscanf(fp,"%s",buf)!=EOF){
        line++;
    }
    *size_teacher=line/((*size_T)*(*size_X));
        
    //file正規性確認    
    if(*size_teacher>TEACHER_MAX_SIZE){
        printf("WARNING: in %s \n\tdata amount is %d, which over threshould\n",TEACHER_FILE_NAME,*size_teacher);
    }
    fclose(fp);
    return 0;
}

/*教師データをデータファイルから読み込む*/
//引数　size_X  入力パラメタ数
//      size_T  出力パラメタ数
//      X:入力データの行列 
//      T:正解データの行列
//戻り値    正数：データの総数
//          -1:ファイルエラー
//          -2:ポインタエラー
//          -3:そのほかのエラー
int read_teacher_data(int *size_X,int *size_T,S_MATRIX *X,S_MATRIX *T){
    int block_size=0;
    int size_teacher=0;
    double buf;
    FILE * fp=NULL;
    //NULL check
    if(size_X==NULL||size_T==NULL){
        return -2;
    }
    
    //read file info
    get_teacher_file_info(&size_teacher,size_X,size_T);

    //File open
    fp=fopen(TEACHER_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }

    //skip header file
    fscanf(fp,"%lf",&buf);
    fscanf(fp,"%lf",&buf);

    if(&size_X==0||&size_T==0){
        fclose(fp);
        return -1;
    }
    block_size= *size_T * *size_X;

    //データセット変数の作成
    for(int i=0;i<size_teacher;i++){
        F_CREATE_MATRIX(1,*size_X,&X[i]);
        F_CREATE_MATRIX(1,*size_T,&T[i]);
    }

    //read data
    for(int i=0;i<size_teacher*block_size;i++){
        char ret=fscanf(fp,"%lf",&buf);
        if(ret==EOF){
            for(int k=0;k<i;k++){
                F_DELETE_MATRIX(&X[k]);
                F_DELETE_MATRIX(&T[k]);
            }
            fclose(fp);
            return -1;
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
        if(data_pos<*size_X){
            X[block_num].elep[data_pos]=buf;
        }else{
            T[block_num].elep[data_pos-*size_X]=buf;
        }
    }
    fclose(fp);
    return size_teacher;
}

/*教師データをデータファイルに追加する*/
//引数　size_X  入力パラメタ数
//      size_T  出力パラメタ数
//      X:入力データの行列 
//      T:正解データの行列
//戻り値    正数：データの総数
//          -1:ファイルエラー
//          -3:そのほかのエラー
int add_teacher_data(int size_X,int size_T,S_MATRIX X,S_MATRIX T){
    int size_X_infile;
    int size_T_infile;
    int size_teacher;
    
    //既存ファイルを確認
    int ret=get_teacher_file_info(&size_teacher,&size_X_infile,&size_T_infile);
    if(ret!=0){
        printf("ERROR @FUNC:get_teacher_file_info\n");
        printf("\terror cmd:%d\n",ret);
        return -1;
    }
    //追加データが既存ファイルと形式が一致するか確認
    if(size_X!=size_X_infile||size_T!=size_T_infile){
        printf("ERROR data structure not match with the statement in file\n");
        return -1;
    }

    //データセット数が閾値を超えていないか確認
    if(size_teacher>=TEACHER_MAX_SIZE){
        printf("WARNING : data amount get over the threshould\n");
    }

    //追記モードでファイルをオープン
    FILE * fp;
    fp=fopen(TEACHER_FILE_NAME,"a");
    if(fp==NULL){
        return -1;
    }
    for(int i=0;i<size_X;i++){
            fprintf(fp,"\n%lf",X.elep[i]);
    }
    for(int i=0;i<size_T;i++){
            fprintf(fp,"\n%lf",T.elep[i]);
    }
    fclose(fp);

    return 0;
}