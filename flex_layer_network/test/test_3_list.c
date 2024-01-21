#include "../list.h"
#include <stdio.h>

//(1)リストが生成できること
/*
    (1-1)リストが生成できること
    (1-2)リストが初期化されていること
*/
//(2)リストに要素を追加できること
/*
    (2-1)初めの要素を追加できること
    (2-2)複数の要素を追加できること
    (2-3)要素数を正しくカウントできること
*/
//(3)リストの要素にアクセスできること
/*
    (3-1)初めの要素を取得できること
    (3-2)２番目以降の要素を取得できること
    (3-3)管理範囲外の要素へのアクセスを防ぐこと
    (3-4)要素0へのアクセス要求にはNULLを返すこと
*/
//(4)リストから要素を削除できること
/*
    (4-1)初めの要素を削除できること
    (4-2)２番目以降の要素を削除できること
    (4-3)範囲外の要素へのアクセスを防ぐこと
*/
//(5)リストを削除できること
/*
    (5-1)所属するすべての要素を削除できること
*/
//(6)リスト要素を検索できること
/*
    (6-1)見つかった場合、要素の番号を返す
    (6-2)見つからなかった場合0を返す
*/

double test_case[5] = {1.0, 2.0, 3.0, 4.0, 5.0};

int list_check(H_LIST hList, unsigned int exp_length, double *exp_value)
{
    int result = 0;
    if (hList == NULL)
    {
        return 1;
    }
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = NULL;
    if (pList->length != exp_length)
    {
        return 2;
    }
    double *p_cur_val = NULL;
    pListElem = pList->pFirst;
    for (unsigned int i = 0; i < exp_length; i++)
    {
        p_cur_val = (double *)pListElem->pElem;
        if (*p_cur_val != exp_value[i])
        {
            return 3 + i;
        }
        pListElem = pListElem->pNextList;
    }

    if (pListElem != NULL)
    {
        return 0xFF;
    }

    return 0;
}

//(1)リストが生成できること
int test_3_list_1_create(void)
{
    int result = 0;
    H_LIST hList = NULL;
    S_LIST *pList = NULL;
    //    (1-1)リストが生成できること
    hList = create_list();
    // CHECK
    if (hList == NULL)
    {
        result += 0x1 << 0;
    }
    //    (1-2)リストが初期化されていること
    pList = (S_LIST *)hList;
    // CHECK
    if (
        pList->length != 0 || pList->pFirst != NULL)
    {
        result += 0x1 << 1;
    }

    delete_list(hList);
    return result;
}

//(2)リストに要素を追加できること
int test_3_list_2_append(void)
{
    int result = 0;
    int ret;
    H_LIST hList = NULL;
    hList = create_list();
    S_LIST *pList = (S_LIST *)hList;
    //    (2-1)初めの要素を追加できること
    ret = append_list(hList, &test_case[0]);
    // CHECK
    if (ret != 0)
    {
        result += 0x1 << 0;
    }
    //    (2-2)複数の要素を追加できること
    ret = 0;
    for (int i = 1; i < 5; i++)
    {
        ret += append_list(hList, &test_case[i]);
    }
    // CHECK
    if (ret != 0)
    {
        result += 0x1 << 1;
    }

    //    (2-3)要素数を正しくカウントできること
    // CHECK
    if (pList->length != 5)
    {
        result += 0x1 << 2;
    }

    return result;
}

//(3)リストの要素にアクセスできること
int test_3_list_3_get(void)
{
    int result = 0;
    int ret = 0;
    H_LIST hList = NULL;
    hList = create_list();
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = NULL;
    for (int i = 0; i < 5; i++)
    {
        ret += append_list(hList, &test_case[i]);
    }

    //    (3-1)初めの要素を取得できること
    pListElem = get_list(hList, 1);
    double exp_3_1[1] = {1};
    // CHECK
    if (
        pListElem == NULL ||
        *((double *)pListElem->pElem) != test_case[0])
    {
        result += 0x1 << 0;
    }
    //    (3-2)２番目以降の要素を取得できること
    for (int i = 1; i < 5; i++)
    {
        pListElem = get_list(hList, i + 1);
        // CHECK
        if (*((double *)pListElem->pElem) != test_case[i])
        {
            result += 0x1 << 1;
        }
    }
    //    (3-3)管理範囲外の要素へのアクセスを防ぐこと
    pListElem = NULL;
    pListElem = get_list(hList, 6);
    if (pListElem != NULL)
    {
        result += 0x1 << 2;
    }
    //(3-4)要素0へのアクセス要求にはNULLを返すこと
    pListElem = NULL;
    pListElem = get_list(hList, 6);
    if (pListElem != NULL)
    {
        result += 0x1 << 2;
    }

    delete_list(hList);
    return result;
}

//(4)リストから要素を削除できること
int test_3_list_4_remove(void)
{
    int result = 0;
    int ret;
    H_LIST hList = NULL;
    hList = create_list();
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = NULL;
    for (int i = 0; i < 5; i++)
    {
        ret += append_list(hList, &test_case[i]);
    }
    //(4-1)初めの要素を削除できること
    ret = remove_elem_list(hList, 1);
    double exp_4_1[4] = {2, 3, 4, 5};
    if (ret != 0 || list_check(hList, 4, exp_4_1))
    {
        result += 0x1 << 0;
    }

    //(4-2)２番目以降の要素を削除できること
    ret = remove_elem_list(hList, 3);
    double exp_4_2[3] = {2, 3, 5};
    if (ret != 0 || list_check(hList, 3, exp_4_2))
    {
        result += 0x1 << 1;
    }
    //(4-3)範囲外の要素へのアクセスを防ぐこと
    ret = remove_elem_list(hList, 4);
    double exp_4_3[3] = {2, 3, 5};
    if (ret == 0 || list_check(hList, 3, exp_4_3))
    {
        result += 0x1 << 2;
    }

    delete_list(hList);
    return result;
}
//(5)リストを削除できること
int test_3_list_5_delete(void)
{
    int result = 0;
    int ret;
    H_LIST hList = NULL;
    hList = create_list();
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = NULL;
    for (int i = 0; i < 5; i++)
    {
        append_list(hList, &test_case[i]);
    }
    //(5-1)所属するすべての要素を削除できること
    ret = delete_list(hList);
    if (ret != 0)
    {
        result += 0x1 << 0;
    }
    return result;
}

//(6)リスト要素を検索できること
int test_3_list_6_find(void)
{
    int result = 0;
    int ret = 0;
    H_LIST hList = NULL;
    hList = create_list();
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = NULL;
    for (int i = 0; i < 5; i++)
    {
        append_list(hList, &test_case[i]);
    }
    //    (6-1)見つかった場合、要素の番号を返す
    ret = find_list(hList, &test_case[3]);
    if (ret != 4)
    {
        result += 0x1 << 0;
    }
    //    (6-2)見つからなかった場合0を返す
    double *ptmp;
    ret = find_list(hList, ptmp);
    if (ret != 0)
    {
        result += 0x1 << 1;
    }
    delete_list(hList);
    return result;
}
int main(void)
{
    int ret = 0;
    //(1)
    printf("=================\n");
    printf("TEST CASE : 1\n");
    ret = test_3_list_1_create();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }

    //(2)
    printf("=================\n");
    printf("TEST CASE : 2\n");
    ret = test_3_list_2_append();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }

    //(3)
    printf("=================\n");
    printf("TEST CASE : 3\n");
    ret = test_3_list_3_get();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }

    //(4)
    printf("=================\n");
    printf("TEST CASE : 4\n");
    ret = test_3_list_4_remove();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }

    //(5)
    printf("=================\n");
    printf("TEST CASE : 5\n");
    ret = test_3_list_5_delete();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }

    //(6)
    printf("=================\n");
    printf("TEST CASE : 6\n");
    ret = test_3_list_6_find();
    if (ret != 0)
    {
        printf("FAILED : %x\n", ret);
    }
    else
    {
        printf("SUCCESS\n");
    }
}