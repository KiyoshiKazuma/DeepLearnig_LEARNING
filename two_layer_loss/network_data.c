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

/*ネットワークのデータ総数を求めて返す*/
//引数  int layer_size ネットワークの総数
//      int *neuron_size 各層のニューロン数
//戻り値 正数:データ総数
//      -1:エラー
int data_amount(int layer_size,int *neuron_size){
    //NULL check
    if(neuron_size==NULL){
        return -1;
    }
    int ret=0;
    for(int i=0;i<layer_size;i++){
        ret+=neuron_size[i];
    }
    return ret;
}




/*ネットワークファイルを初期化する*/
//引数  int layer_size  ネットワークの層数
//      int *neuron_size 各層のニューロン数
//戻り値　   0:正常
//          -1:ファイル異常
int init_network_data(int layer_size,int *neuron_size){
    //ファイルを開く
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"w");
    if(fp==NULL){
        return -1;
    }

    fprintf(fp,"%d\n",layer_size);
    for(int i=0;i<layer_size;i++){
        fprintf(fp,"%d\n",neuron_size[i]);
    }
    return 0;
}

/*ネットワークファイルから情報を読み込む*/
//引数  int *layer_size ネットワークの層数を返す変数へのポインタ
//      int *neuron_size 各層のニューロン数を返す変数配列のポインタ
//戻り値    正値:ネットワークのサイズ
//          -1:ファイル異常
//          -2:そのほか異常
int get_network_info(int *layer_size,int *neuron_size){
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }
    fscanf(fp,"%d",layer_size);
    if(*layer_size<=0){
        fclose(fp);
        return -1;
    }
    if(*layer_size>MAX_LAYER_NETWORK){
        printf("WARNING : %s\n",NEURON_FILE_NAME);
        printf("\tlayer size in file is over the threshould :%d / %d\n",*layer_size,MAX_LAYER_NETWORK);
    }
    for(int i=0;i<*layer_size;i++){
        int tmp;
        if(fscanf(fp,"%d",&tmp)==EOF){
            printf("ERROR : %s\n",NEURON_FILE_NAME);
            printf("\tformat error : neuron info not match with layer size\n");
            fclose(fp);
            return -1;
        }
        neuron_size[i]=tmp;
    }
    fclose(fp);
    return 0;
}

/*ネットワークファイルからデータを読み込む*/
//引数  double **pnet_value データの中身への配列へのポインタ
//戻り値     0:正常
//          -1:ファイル異常
//          -2:そのほか異常
int read_network_data(double **pnet_value){
    double buf;
    int layer_size,i;
    int neuron_size[MAX_LAYER_NETWORK];
    int ret;
    int datasize;

    //ネットワーク情報を取得する
    ret=get_network_info(&layer_size,neuron_size);
    if(ret!=0){
        printf("ERROR : get_network_info\n");
        printf("\teroor code : %d\n",ret);
        return -2;
    }
    
    //ファイルを開く
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }
    //ファイルの情報を読み飛ばす
    for(i=0;i<layer_size+1;i++){
        fscanf(fp,"%f",&buf);
    }
    
    //ネットワークデータを読み込む
    datasize=data_amount(layer_size,neuron_size);
    for(i=0;i<datasize;i++){
        fscanf(fp,"%lf",pnet_value[i]);
    }

    //ファイルを閉じる
    fclose(fp);

    return 0;
}

/*ネットワークファイルのデータを更新する*/
//引数  double **pnet_value データの中身への配列へのポインタ
//戻り値     0:正常
//          -1:ファイル異常
//          -2:そのほか異常
int update_network_data(double **pnet_value){
    double buf;
    int layer_size,i;
    int neuron_size[MAX_LAYER_NETWORK];
    int ret;
    int datasize;

    //ネットワーク情報を取得する
    ret=get_network_info(&layer_size,neuron_size);
    if(ret!=0){
        printf("ERROR : get_network_info\n");
        printf("\teroor code : %d\n",ret);
        return -2;
    }
    
    //ファイルを初期化
    init_network_data(layer_size,neuron_size);

    //ファイルにデータを追記
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"a");
    if(fp==NULL){
        return -1;
    }
    datasize=data_amount(layer_size,neuron_size);

    //データを更新
    for(i=0;i<datasize;i++){
        fprintf(fp,"%lf\n",*(pnet_value[i]));
    }

    //ファイルを閉じる
    fclose(fp);

    return 0;
       
}