#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../matrix.h"

int test_1_create_matrix(void);
int test_1_delete_matrix(void);
int test_1_add_matrix(void);
int test_1_product_matrix(void);
int test_1_print_matrix(void);
int test_1_copy_matrix(void);

int main(void)
{
    srand((unsigned int)time(NULL));
    int ret = 0;
    if (test_1_create_matrix() != 0)
    {
        printf("TEST NG in : test_1_create_matrix\n");
        printf("ERROR CODE : %d\n", ret);
    }
    if (test_1_delete_matrix() != 0)
    {
        printf("TEST NG in : test_1_delete_matrix\n");
    }
    if ( test_1_add_matrix() != 0)
    {
        printf("TEST NG in : test_1_add_matrix\n");
    }
    if ( test_1_product_matrix() != 0)
    {
        printf("TEST NG in : test_1_product_matrix\n");
    }
    if ( test_1_print_matrix() != 0)
    {
        printf("TEST NG in : test_1_print_matrix\n");
    }
    if ( test_1_copy_matrix() != 0)
    {
        printf("TEST NG in : test_1_print_matrix\n");
    }
}

int test_1_create_matrix(void)
{
    H_MATRIX hMatrix = NULL;
    unsigned int row = rand() % 10 + 1;
    unsigned int column = rand() % 10 + 1;
    unsigned int size = row * column;

    hMatrix = create_matrix(row, column);
    // NULL CHECK
    if (hMatrix == NULL)
    {
        return 1;
    }
    S_MATRIX *pMatrix = NULL;
    pMatrix = (S_MATRIX *)hMatrix;

    // SHAPE CHECK
    if (pMatrix->column != column || pMatrix->row != row)
    {
        return 2;
    }
    // SIZE CHECK
    if (pMatrix->size != size)
    {
        return 3;
    }
    // ELEMENT POINTER CHECK
    if (pMatrix->pElem == NULL)
    {
        return 4;
    }
    return 0;
}

int test_1_delete_matrix(void)
{
    return 0;
}

int test_1_add_matrix(void)
{
    // GENERATE TEST DATA
    double vec1[6];
    double vec2[6];
    double vec3[6];
    for (int i = 0; i < 6; i++)
    {
        vec1[i] = (double)(rand() % 20 - 10) / 10.0;
        vec2[i] = (double)(rand() % 20 - 10) / 10.0;
        vec3[i] = vec1[i] + vec2[i];
    }
    H_MATRIX hMatrix1 = NULL;
    H_MATRIX hMatrix2 = NULL;
    H_MATRIX hMatrix3 = NULL;

    hMatrix1 = create_matrix(2, 3);
    hMatrix2 = create_matrix(2, 3);
    hMatrix3 = create_matrix(2, 3);

    S_MATRIX *pMatrix1 = NULL;
    S_MATRIX *pMatrix2 = NULL;
    S_MATRIX *pMatrix3 = NULL;

    pMatrix1 = (S_MATRIX *)hMatrix1;
    pMatrix2 = (S_MATRIX *)hMatrix2;
    pMatrix3 = (S_MATRIX *)hMatrix3;

    for (int i = 0; i < 6; i++)
    {
        pMatrix1->pElem[i] = vec1[i];
        pMatrix2->pElem[i] = vec2[i];
    }

    add_matrix(hMatrix1, hMatrix2, hMatrix3);

    for (int i = 0; i < 6; i++)
    {
        if (pMatrix3->pElem[i] != vec3[i])
        {
            return 1;
        }
    }

    return 0;
}

int test_1_product_matrix(void)
{
    // GENERATE TEST DATA
    double vec1[6] = {1, 2, 3, 4, 5, 6};
    double vec2[6] = {7, 8, 9, 1, 2, 3};
    double vec3[4] = {31, 19, 85, 55};

    H_MATRIX hMatrix1 = NULL;
    H_MATRIX hMatrix2 = NULL;
    H_MATRIX hMatrix3 = NULL;

    hMatrix1 = create_matrix(2, 3);
    hMatrix2 = create_matrix(3, 2);
    hMatrix3 = create_matrix(2, 2);

    S_MATRIX *pMatrix1 = NULL;
    S_MATRIX *pMatrix2 = NULL;
    S_MATRIX *pMatrix3 = NULL;

    pMatrix1 = (S_MATRIX *)hMatrix1;
    pMatrix2 = (S_MATRIX *)hMatrix2;
    pMatrix3 = (S_MATRIX *)hMatrix3;

    for (unsigned int i = 0; i < 6; i++)
    {
        pMatrix1->pElem[i] = vec1[i];
    }
    for (unsigned int i = 0; i < 6; i++)
    {
        pMatrix2->pElem[i] = vec2[i];
    }

    product_matrix(hMatrix1, hMatrix2, hMatrix3);

    for (unsigned int i = 0; i < 4; i++)
    {
        if (pMatrix3->pElem[i] != vec3[i])
        {

            print_matrix(hMatrix1);
            printf("\n");
            print_matrix(hMatrix2);
            printf("\n");
            print_matrix(hMatrix3);
            printf("\n");
            return 1;
        }
    }

    return 0;
}

int test_1_print_matrix(void)
{
    return 0;
}

int test_1_copy_matrix(void)
{
    H_MATRIX hMatIN = NULL;
    H_MATRIX hMatOUT = NULL;
    S_MATRIX *pMatIN = NULL;
    S_MATRIX *pMatOUT = NULL;
    pMatIN = (S_MATRIX *)hMatIN;
    pMatOUT = (S_MATRIX *)hMatOUT;
    int ret = 0;
    int result = 0;

    //(1)collect copy
    hMatIN = create_matrix(2, 3);
    hMatOUT = create_matrix(2, 3);
    for (int i = 0; i < 6; i++)
    {
        pMatIN->pElem[i] = (double)i;
    }
    ret = copy_matrix(hMatIN, hMatOUT);
    if (ret == 0)
    {
        for (int i = 0; i < 6; i++)
        {
            if (pMatOUT->pElem[i] != (double)i)
            {
                ret += 1;
                break;
            }
        }
    }
    if (ret != 0)
    {
        result += 0x1;
    }
    delete_matrix(hMatIN);
    delete_matrix(hMatOUT);

    //(2)size failure

    hMatIN = create_matrix(2, 3);
    hMatOUT = create_matrix(3, 3);
    ret = copy_matrix(hMatIN, hMatOUT);
    if (ret == 0)
    {
        result += 0x2;
    }
    delete_matrix(hMatIN);
    delete_matrix(hMatOUT);

    //(3)pointer failure
    hMatIN = create_matrix(2, 3);
    ret = copy_matrix(hMatIN, NULL);
    if (ret == 0)
    {
        result += 0x4;
    }
    delete_matrix(hMatIN);
    return result;
}