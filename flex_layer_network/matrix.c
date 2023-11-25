
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include"matrix.h"

/*関数　create_matrix
概要    行列を生成する
引数    unsinged int row_size   行番号
        unsigned int column_size   列番号
戻り値  異常：NULL   
        正常：MATRIXハンドラ
*/
H_MATRIX create_matrix(unsigned int row_size, unsigned int column_size) {
    S_MATRIX * hMatrix=NULL;
    hMatrix = (S_MATRIX*)malloc(sizeof(S_MATRIX));
    if(hMatrix==NULL){
        return NULL;
    }
    hMatrix->column=column_size;
    hMatrix->row=row_size;
    hMatrix->size=row_size*column_size;
    double * vec =(double *) malloc(sizeof(double) * hMatrix->size);
    hMatrix->pElem=vec;

    return hMatrix;
}

/*関数　delete_matrix
概要    行列を削除する
引数    H_MATRIX hMatrix  行列ハンドラ
戻り値   0:正常
        -1:異常
*/
int delete_matrix(H_MATRIX hMatrix) {
    S_MATRIX * pMatrix=(S_MATRIX *)hMatrix;
    free(pMatrix->pElem);
    free(pMatrix);
    return 0;
}


/*関数　element_num_matrix
概要　行列の行・列番号から配列の番号を返す。
引数　  int i   行番号
        int j   列番号
        S_MATRIX * mat 配列のポインタ
戻り値 非負値　配列の番号
        -1    エラー
*/
int element_num_matrix(unsigned int i,unsigned int j, S_MATRIX* mat) {
    if (mat == NULL) {
        printf("ERROR:###element_num_matrix###\ninput pointa contins NULL\n");
        return -1;
    }
    if (i >= mat->row || j >= mat->column) {
        printf("ERROR:###element_num_matrix###\nmatrix element num out of range\n");
        return -2;
    }
    int n = mat->column * i + j;
    return n;
}

/*  関数　print_matrix
概要　行列を表示する
引数    S_MATRIX *mat 表示する行列
戻り値  0:正常　１：異常
*/
int print_matrix(S_MATRIX* mat) {
    if (mat == NULL) {
        printf("ERROR:###print_matrix###\ninput pointa contins NULL\n");
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

/*  関数　add_matrix
概要　行列の和を求めてretに返す。
引数    S_MATRIX *a,b 和を求める行列のポインタ
        S_MATRIX *ret 返す行列のポインタ
戻り値  0:正常　1:行列のサイズが異なる　2:そのほかエラー
*/

int add_matrix(S_MATRIX* a, S_MATRIX* b, S_MATRIX* ret) {
    unsigned int i, j,size;
    if (a == NULL || b == NULL || ret == NULL) {
        printf("ERROR:###add_matrix###\ninput pointa contins NULL\n");
        return 2;
    }
    if (a->row != b->row || a->column != b->column) {
        printf("ERROR:###add_matrix###\nmatrix size not match\n");
        return 1;
    }
    size=ret->column*ret->row;
    for (i = 0; i < size; i++) {
        ret->elep[i] = a->elep[i] + b->elep[i];
    }

    return 0;
}

/*  関数　product_matrix
概要　行列の内積を求めてretに返す。
引数    S_MATRIX *a,b 積を求める行列のポインタ
        S_MATRIX *ret 返す行列のポインタ
戻り値  0:正常　1:行列のサイズが異なる　2:そのほかエラー
*/
int product_matrix(S_MATRIX* a, S_MATRIX* b, S_MATRIX* ret) {
    unsigned int i, j, k, n;
    //NULLチェック
    if (a == NULL || b == NULL || ret == NULL) {
        printf("ERROR:###add_matrix###\ninput pointa contins NULL\n");
        return 2;
    }
    //サイズチェック
    if (a->column != b->row) {
        printf("ERROR:###add_matrix###\nmatrix size not match\n");
        return 1;
    }
    //出力配列のサイズを計算
    unsigned int row = a->row, column = b->column;
    ret->row = row;
    ret->column = column;
    unsigned int size = row * column;
    n = a->column;

    for (i = 0; i < size; i++)ret->elep[i] = 0;

    //要素を計算
    for (i = 0; i < row; i++) {
        for (j = 0; j < column; j++) {
            for (k = 0; k < n; k++) {
                ret->elep[element_num_matrix(i, j, ret)] += a->elep[element_num_matrix(i, k, a)] * b->elep[element_num_matrix(k, j, b)];
            }
        }
    }

    return 0;
}
