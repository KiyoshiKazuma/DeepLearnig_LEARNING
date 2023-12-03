#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

/*関数　create_matrix
概要    行列を生成する
引数    unsinged int row_size   行番号
        unsigned int column_size   列番号
戻り値  異常：NULL
        正常：MATRIXハンドラ
*/
H_MATRIX create_matrix(unsigned int row_size, unsigned int column_size)
{
    S_MATRIX *hMatrix = NULL;
    hMatrix = (S_MATRIX *)malloc(sizeof(S_MATRIX));
    if (hMatrix == NULL)
    {
        return NULL;
    }
    hMatrix->column = column_size;
    hMatrix->row = row_size;
    hMatrix->size = row_size * column_size;
    double *vec = (double *)malloc(sizeof(double) * hMatrix->size);
    hMatrix->pElem = vec;

    return hMatrix;
}

/*関数　delete_matrix
概要    行列を削除する
引数    H_MATRIX hMatrix  行列ハンドラ
戻り値   0:正常
        -1:異常
*/
int delete_matrix(H_MATRIX hMatrix)
{
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    free(pMatrix->pElem);
    free(pMatrix);
    return 0;
}

/*関数　element_num_matrix
概要　行列の行・列番号から配列の番号を返す。
引数    H_MATRIX * hMatrix 行列のハンドラ
        int i   行番号
        int j   列番号
戻り値 非負値　配列の番号
        -1    エラー
*/
int element_num_matrix(H_MATRIX hMatrix, int i, int j)
{
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;
    if (i >= pMatrix->row || j >= pMatrix->column)
    {
        printf("ERROR:###element_num_matrix###\nmatrix element num out of range\n");
        return -2;
    }
    int n = pMatrix->column * i + j;
    return n;
}

/*  関数　add_matrix
概要　行列の和を求めてretに返す。
引数    H_MATRIX hMatrix_IN1 入力行列のハンドラー
        H_MATRIX hMatrix_IN2 入力行列のハンドラー
        H_MATRIX hMatrix_OUT 出力行列のハンドラー
戻り値   0:正常
　　　　-1:異常
*/
int add_matrix(H_MATRIX hMatrix_IN1, H_MATRIX hMatrix_IN2, H_MATRIX hMatrix_OUT)
{
    S_MATRIX *pMatrix_IN1 = (S_MATRIX *)hMatrix_IN1;
    S_MATRIX *pMatrix_IN2 = (S_MATRIX *)hMatrix_IN2;
    S_MATRIX *pMatrix_OUT = (S_MATRIX *)hMatrix_OUT;
    int i, j, size;

    // format check
    if (pMatrix_IN1->row != pMatrix_IN2->row || pMatrix_IN1->column != pMatrix_IN2->column)
    {
        printf("ERROR:###add_matrix###\nmatrix size not match\n");
        return -1;
    }
    if (pMatrix_IN1->row != pMatrix_OUT->row || pMatrix_IN1->column != pMatrix_OUT->column)
    {
        printf("ERROR:###add_matrix###\nmatrix size not match\n");
        return -1;
    }

    // execute calucuration
    for (i = 0; i < pMatrix_OUT->size; i++)
    {
        pMatrix_OUT->pElem[i] = pMatrix_IN1->pElem[i] + pMatrix_IN2->pElem[i];
    }

    return 0;
}

/*  関数　product_matrix
概要　行列の内積を求めてretに返す。
引数    H_MATRIX hMatrix_IN1 入力行列のハンドラー
        H_MATRIX hMatrix_IN2 入力行列のハンドラー
        H_MATRIX hMatrix_OUT 出力行列のハンドラー
戻り値   0:正常
　　　　-1:異常
*/
int product_matrix(H_MATRIX hMatrix_IN1, H_MATRIX hMatrix_IN2, H_MATRIX hMatrix_OUT)
{
    S_MATRIX *pMatrix_IN1 = (S_MATRIX *)hMatrix_IN1;
    S_MATRIX *pMatrix_IN2 = (S_MATRIX *)hMatrix_IN2;
    S_MATRIX *pMatrix_OUT = (S_MATRIX *)hMatrix_OUT;
    unsigned int i, j, k, n;

    // format check
    if (pMatrix_IN1->column != pMatrix_IN2->row)
    {
        printf("ERROR:###add_matrix###\nmatrix size not match\n");
        return -1;
    }
    if (pMatrix_IN1->row != pMatrix_OUT->row || pMatrix_IN2->column != pMatrix_OUT->column)
    {
        printf("ERROR:###add_matrix###\nmatrix size not match\n");
        return -1;
    }

    // execute calucuration
    for (i = 0; i < pMatrix_OUT->row; i++)
    {
        for (j = 0; j < pMatrix_OUT->column; j++)
        {
            pMatrix_OUT->pElem[element_num_matrix(hMatrix_OUT, i, j)] = 0;
            for (k = 0; k < pMatrix_IN1->column; k++)
            {
                pMatrix_OUT->pElem[element_num_matrix(hMatrix_OUT, i, j)] += pMatrix_IN1->pElem[element_num_matrix(hMatrix_OUT, i, k)] * pMatrix_IN2->pElem[element_num_matrix(hMatrix_OUT, k, j)];
            }
        }
    }

    return 0;
}

/*  関数　print_matrix
概要　行列を表示する
引数    H_MATRIX hMatrix 表示する行列
戻り値   0:正常　
        -1:異常
*/
int print_matrix(H_MATRIX hMatrix)
{
    S_MATRIX *pMatrix = (S_MATRIX *)hMatrix;

    for (int i = 0; i < pMatrix->row; i++)
    {
        for (int j = 0; j < pMatrix->column; j++)
        {
            printf("%4.4f  ", pMatrix->pElem[element_num_matrix(hMatrix,i,j)]);
        }
        printf("\n");
    }
}

