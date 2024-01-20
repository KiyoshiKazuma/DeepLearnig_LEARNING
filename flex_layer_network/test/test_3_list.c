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

double test_case [5] = {1.0 ,2.0 ,3.0 ,4.0 , 5.0};

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
    hList=create_list();
    S_LIST * pList = (S_LIST *)hList;
    //    (2-1)初めの要素を追加できること
    ret=append_list(hList,&test_case[0]);
    //CHECK
    if(ret != 0){
        result += 0x1<<0;
    }
    //    (2-2)複数の要素を追加できること
    ret=0;
    for(int i=1;i<5;i++){
        ret+=append_list(hList,&test_case[i]);
    }
    //CHECK
    if(ret != 0){
        result += 0x1<<1;
    }
    
    //    (2-3)要素数を正しくカウントできること
    //CHECK
    if(pList->length!=5){
        result += 0x1<<2;
    }

    return result;
}

//(3)リストの要素にアクセスできること
int test_3_list_3_get(void){
    int result = 0;
    int ret;
    H_LIST hList = NULL;
    hList=create_list();
    S_LIST * pList = (S_LIST *)hList;
    for(int i=0;i<5;i++){
        ret+=append_list(hList,&test_case[i]);
    }
    
    
//    (3-1)初めの要素を取得できること
//    (3-2)２番目以降の要素を取得できること
//    (3-3)管理範囲外の要素へのアクセスを防ぐこと

}
