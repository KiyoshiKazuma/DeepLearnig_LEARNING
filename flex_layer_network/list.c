#include <stdio.h>
#include <stdlib.h>
#include "list.h"

H_LIST create_list(void)
{
    S_LIST *pList = NULL;
    pList = (S_LIST *)malloc(sizeof(S_LIST));
    pList->length = 0;
    pList->pFirst = NULL;
    return pList;
}
int delete_list(H_LIST hList)
{
    // NULL check
    if (hList == NULL)
    {
        return 1;
    }
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pNextListElem = NULL;
    S_LIST_ELEM *pCurrentListElem = NULL;

    pNextListElem = pList->pFirst;
    while (pNextListElem != NULL)
    {
        pCurrentListElem = pNextListElem;
        pNextListElem = pCurrentListElem->pNextList;
        free(pCurrentListElem);
    }
    return 0;
}
S_LIST_ELEM * get_list(H_LIST hList, unsigned int num)
{
    // NULL check
    if (hList == NULL)
    {
        return NULL;
    }
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = NULL;

    // SIZE check
    if (num == 0 || num > pList->length)
    {
        return NULL;
    }
    pListElem = pList->pFirst;
    for (unsigned int cnt = 1; cnt < num; cnt++)
    {
        pListElem = pListElem->pNextList;
    pListElem = pList->pFirst;
    }
    for (unsigned int cnt = 1; cnt < num; cnt++)
    {
        pListElem = pListElem->pNextList;
    }
    return pListElem;
}

unsigned int find_list(H_LIST hList, void *pElem)
{

    if (hList == NULL)
    {
        return 0;
    }

    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElemCurrent = NULL;
    S_LIST_ELEM *pListElemNext = NULL;

    pListElemNext = pList->pFirst;

    unsigned int num = 0;
    for (unsigned int i = 1; i <= pList->length; i++)
    {
        pListElemCurrent = pListElemNext;
        if (pListElemCurrent->pElem == pElem)
        {
            num = i;
            break;
        }
        pListElemNext = pListElemCurrent->pNextList;
    }

    return num;
}
int append_list(H_LIST hList, void *pElem)
{
    // NULL CHECK
    if (hList == NULL || pElem == NULL)
    {
        return 1;
    }

    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElemLast = NULL;
    S_LIST_ELEM *pListElemNew = NULL;

    pListElemNew = (S_LIST_ELEM *)malloc(sizeof(S_LIST_ELEM));
    pListElemNew->pElem = pElem;
    pListElemNew->pNextList = NULL;

    if (pList->length > 0)
    {
        pListElemLast = get_list(hList, pList->length);
        if (pListElemLast == NULL)
        {
            free(pListElemNew);
            return 1;
        }
        pListElemLast->pNextList = pListElemNew;
    }
    else
    {
        pList->pFirst=pListElemNew;
    }
    pList->length++;

    return 0;
}
int remove_elem_list(H_LIST hList, unsigned int num)
{
    //NULL CHECK
    if(hList==NULL){
        return 1;
    }

    S_LIST * pList = (S_LIST *)hList;

    //size check
    if(num>pList->length || num==0){
        return 1;
    }

    S_LIST_ELEM * pListElemTarget =NULL;
    S_LIST_ELEM * pListElemPre =NULL;

    pListElemTarget=get_list(hList,num);
    if(num==1){
        pList->pFirst=pListElemTarget->pNextList;
    }else{
        pListElemPre=get_list(hList,num-1);
        pListElemPre->pNextList=pListElemTarget->pNextList;
    }
    free(pListElemTarget);
    pList->length--;
    return 0;
}

int length_list(H_LIST hList){
    if(hList==NULL){
        return 0;
    }
    S_LIST * pList=(S_LIST *)hList;
    return pList->length;
}
