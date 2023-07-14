#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include"matrix.h"

/*関数　F_CREATE_MATRIX
概要    行列を生成する
引数    int i   行番号
        int j   列番号
        S_MATRIX * mat  配列のポインタ
戻り値  int 0   異常
            1   正常
*/
int F_CREATE_MATRIX(unsigned int i, unsigned int j, S_MATRIX* mat) {
    unsigned int size = i * j;
    double * vec =(double *) malloc(sizeof(double) * size);
    mat->elep = vec;
    mat->row = i;
    mat->column = j;

    return 1;
}

/*関数　F_DELETE_MATRIX
概要    行列を削除する
引数    S_MATRIX * mat  配列のポインタ
戻り値  int 0   異常
            1   正常
*/
int F_DELETE_MATRIX(S_MATRIX* mat) {
    free(mat->elep);
    return 1;
}


/*関数　F_ELEMENT_NUM
概要　行列の行・列番号から配列の番号を返す。
引数　  int i   行番号
        int j   列番号
        S_MATRIX * mat 配列のポインタ
戻り値 非負値　配列の番号
        -1    エラー
*/
int F_ELEMENT_NUM(unsigned int i,unsigned int j, S_MATRIX* mat) {
    if (mat == NULL) {
        printf("ERROR:###F_ELEMENT_NUM###\ninput pointa contins NULL\n");
        return -1;
    }
    if (i >= mat->row || j >= mat->column) {
        printf("ERROR:###F_ELEMENT_NUM###\nmatrix element num out of range\n");
        return -2;
    }
    int n = mat->column * i + j;
    return n;
}

/*  関数　F_PRINT
概要　行列を表示する
引数    S_MATRIX *mat 表示する行列
戻り値  0:正常　１：異常
*/
int F_PRINT(S_MATRIX* mat) {
    if (mat == NULL) {
        printf("ERROR:###F_PRINT###\ninput pointa contins NULL\n");
        return 1;
    }
    int i, j, n;
    n = 0;
    for (i = 0; i < mat->row; i++) {
        for (j = 0; j < mat->column; j++) {
            printf("%4.4f  ", mat->elep[n]);
            n++;
        }
        printf("\n");
    }
}

/*  関数　F_ADD
概要　行列の和を求めてretに返す。
引数    S_MATRIX *a,b 和を求める行列のポインタ
        S_MATRIX *ret 返す行列のポインタ
戻り値  0:正常　1:行列のサイズが異なる　2:そのほかエラー
*/

int F_ADD(S_MATRIX* a, S_MATRIX* b, S_MATRIX* ret) {
    unsigned int i, j;
    if (a == NULL || b == NULL || ret == NULL) {
        printf("ERROR:###F_ADD###\ninput pointa contins NULL\n");
        return 2;
    }
    if (a->row != b->row || a->column != b->column) {
        printf("ERROR:###F_ADD###\nmatrix size not match\n");
        return 1;
    }
    unsigned int row = a->row, column = a->column;
    ret->row = row;
    ret->column = column;
    unsigned int size = row * column;
    if (!(ret->elep = (double*)malloc(sizeof(double) * size))) {
        printf("ERROR:###F_ADD###\nmalloc failed\n");
        return 2;
    }

    for (i = 0; i < size; i++) {
        ret->elep[i] = a->elep[i] + b->elep[i];
    }

    return 0;
}

/*  関数　F_PRODUCT
概要　行列の内積を求めてretに返す。
引数    S_MATRIX *a,b 積を求める行列のポインタ
        S_MATRIX *ret 返す行列のポインタ
戻り値  0:正常　1:行列のサイズが異なる　2:そのほかエラー
*/
int F_PRODUCT(S_MATRIX* a, S_MATRIX* b, S_MATRIX* ret) {
    unsigned int i, j, k, n;
    //NULLチェック
    if (a == NULL || b == NULL || ret == NULL) {
        printf("ERROR:###F_ADD###\ninput pointa contins NULL\n");
        return 2;
    }
    //サイズチェック
    if (a->column != b->row) {
        printf("ERROR:###F_ADD###\nmatrix size not match\n");
        return 1;
    }
    //出力配列のサイズを計算
    unsigned int row = a->row, column = b->column;
    ret->row = row;
    ret->column = column;
    unsigned int size = row * column;
    n = a->column;
    //出力配列のメモリを確保/初期化
    if (!(ret->elep = (double*)malloc(sizeof(double) * size))) {
        printf("ERROR:###F_ADD###\nmalloc failed\n");
        return 2;
    }
    for (i = 0; i < size; i++)ret->elep[i] = 0;

    //要素を計算
    for (i = 0; i < row; i++) {
        for (j = 0; j < column; j++) {
            for (k = 0; k < n; k++) {
                ret->elep[F_ELEMENT_NUM(i, j, ret)] += a->elep[F_ELEMENT_NUM(i, k, a)] * b->elep[F_ELEMENT_NUM(k, j, b)];
            }
        }
    }

    return 0;
}

/*  関数　F_FUNCTION
概要　  入力ベクトルの各要素を関数に入れて出力ベクトルの各要素に返す。
引数    S_MATRIX *in,out 入力、出力ベクトルのポインタ
        double  (*pfunc) (double) 用いる関数の関数ポインタ
戻り値  0:正常　1:行列のサイズが異なる　2:そのほかエラー
*/
int F_FUNCTION(S_MATRIX* in, S_MATRIX* out, double (*pfunc) (double)) {
    if (in->column != out->column || in->row != 1 || out->row != 1 || in->elep == 0 || out->elep == 0) {
        printf("ERROR###matrix.c--F_FUNCTION");
        return 1;
    }
    int i;
    for (i = 0; i < in->column; i++) {
        out->elep[i] = pfunc(in->elep[i]);
    }
    return 0;
}
/*  関数　F_SOFTMAX
概要　  ベクトルにソフトマックス関数を作用させて返す
引数    S_MATRIX *int 入力ベクトルのポインタ
        S_MATRIX *out 返すベクトルののポインタ
戻り値  0:正常　1:行列のサイズが異なる　2:そのほかエラー
*/
/// <summary>
/// ソフトマックス関数を計算します
/// </summary>
int F_SOFTMAX(S_MATRIX* in,S_MATRIX * out){
    int i;
    double max,base;
    if(in==NULL || out==NULL || in->row != 1 || out->row !=1 ||in->column!=out->column){
        printf("ERROR###matrix.c--F_SOFTMAX");
        return 1;
    }

    max=0;
    for(i=0;i<in->column;i++){
        if(max<in->elep[i])max=in->elep[i];
    }

    base=0;
    for(i=0;i<in->column;i++){
        base+=exp(in->elep[i]-max);
    }
    for(i=0;i<in->column;i++){    
        out->elep[i]=exp(in->elep[i]-max)/base;
    }
    return 0;
}
