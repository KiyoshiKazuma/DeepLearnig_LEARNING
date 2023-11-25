#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "network_data.h"
#include "configuration.h"

/*ネットワークのデータ総数を求めて返す*/
//引数  S_NETWORK * pnet
//戻り値 正数:データ総数
//      -1:エラー
int net_data_amount(S_NETWORK * pnet){
    //NULL check
    if(pnet==NULL){
        return -1;
    }
    int ret=0;
    for(int i=0;i<pnet->layer_size-1;i++){
        ret+=pnet->neurons_size[i]*pnet->neurons_size[i+1];
    }
    for(int i=0;i<pnet->layer_size-1;i++){
        ret+=pnet->neurons_size[i+1];
    }
    pnet->net_amount=ret;
    return 0;
}


/*ネットワークファイルを初期化する*/
//引数  S_NETWORK net
//戻り値　   0:正常
//          -1:ファイル異常
//          -2:引数エラー
int init_network_data(S_NETWORK net){
    //ファイルを開く
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"w");
    if(fp==NULL){
        return -1;
    }
    if(net.layer_size<2){
        printf("ERROR###init_network_data###\n\tlayer size must be more than 2\n");
        return -2;
    }

    fprintf(fp,"%d\n",net.layer_size);
    for(int i=0;i<net.layer_size;i++){
        fprintf(fp,"%d\n",net.neurons_size[i]);
    }
    fclose(fp);
    return 0;
}

/*ネットワークファイルから情報を読み込む*/
//引数  int *layer_size ネットワークの層数を返す変数へのポインタ
//      int *neuron_size 各層のニューロン数を返す変数配列のポインタ
//戻り値    正値:ネットワークのサイズ
//          -1:ファイル異常
//          -2:そのほか異常
int get_network_info(S_NETWORK * pnet){
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"r");
    if(fp==NULL){
        printf("ERROR get_network_info : can't open file\n");
        return -1;
    }
    fscanf(fp,"%d",&(pnet->layer_size));
    if(pnet->layer_size<2){
        printf("ERROR get_network_info : layer size is too small\n");
        fclose(fp);
        return -1;
    }
    if(pnet->layer_size>NETWORK_MAX_LAYER){
        printf("WARNING : %s\n",NEURON_FILE_NAME);
        printf("\tlayer size in file is over the threshould :%d / %d\n",pnet->layer_size,NETWORK_MAX_LAYER);
    }
    for(int i=0;i<pnet->layer_size;i++){
        int tmp;
        if(fscanf(fp,"%d",&tmp)==EOF){
            printf("ERROR get_network_info : neuron info not match with layer size\n");
            fclose(fp);
            return -1;
        }
        pnet->neurons_size[i]=tmp;
    }
    fclose(fp);
    return 0;
}

/*ネットワークファイルからデータを読み込む*/
//引数  double **pnet_value データの中身への配列へのポインタ
//      int net_amount      ネットワーク変数の変数数
//戻り値     0:正常
//          -1:ファイル異常
//          -2:そのほか異常
int read_network_data(S_NETWORK net,double **pnet_value){
    double buf;
    int i;
    int ret;
    int datasize;

    // //ネットワーク情報を取得する
    // ret=get_network_info(net);
    // if(ret!=0){
    //     printf("ERROR : get_network_info\n");
    //     printf("\teroor code : %d\n",ret);
    //     return -2;
    // }
    
    //ファイルを開く
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"r");
    if(fp==NULL){
        return -1;
    }
    //ファイルの情報を読み飛ばす
    for(i=0;i<net.layer_size;i++){
        fscanf(fp,"%f",&buf);
    }
    
    //ネットワークデータを読み込む
    for(i=0;i<net.net_amount;i++){
        fscanf(fp,"%lf",pnet_value[i]);
    }

    //ファイルを閉じる
    fclose(fp);

    return 0;
}

/*ネットワークファイルのデータを更新する*/
//引数  double **pnet_value データの中身への配列へのポインタ
//      int     net_amount  ネットワーク変数の変数数
//戻り値     0:正常
//          -1:ファイル異常
//          -2:そのほか異常
int update_network_data(S_NETWORK net,double **pnet_value){
    double buf;
    int i;
    int ret;
    int datasize;

    //ネットワーク情報を取得する
    ret=get_network_info(&net);
    if(ret!=0){
        printf("ERROR : get_network_info\n");
        printf("\teroor code : %d\n",ret);
        return -2;
    }
    
    //ファイルを初期化
    init_network_data(net);

    //ファイルにデータを追記
    FILE * fp;
    fp=fopen(NEURON_FILE_NAME,"a");
    if(fp==NULL){
        return -1;
    }
    //データを更新
    for(i=0;i<net.net_amount;i++){
        fprintf(fp,"%lf\n",*(pnet_value[i]));
    }

    //ファイルを閉じる
    fclose(fp);

    return 0;
       
}