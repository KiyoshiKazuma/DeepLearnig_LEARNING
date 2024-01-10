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
void *get_list(H_LIST hList, unsigned int num)
{
    // NULL check
    if (hList == NULL)
    {
        return NULL;
    }
    S_LIST *pList = (S_LIST *)pList;
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
    }
    return pListElem->pElem;
}

unsigned int find_list(H_LIST hList, void *pElem)
{
    // NULL check
    if (hList == NULL)
    {
        return 0;
    }

    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListCurrentElem = NULL;

    pListCurrentElem = pList->pFirst;
    unsigned int num = 0;
    for (unsigned int i = 1; i <= pList->length; i++)
    {
        if (pListCurrentElem->pElem == pElem)
        {
            num = i;
            break;
        }
        else
        {
            pListCurrentElem = pListCurrentElem->pNextList;
        }
    }
    return num;
}

int append_list(H_LIST hList, void *pElem)
{
    if (hList == NULL || pElem == NULL)
    {
        return 1;
    }
    S_LIST *pList = (S_LIST *)hList;
    S_LIST_ELEM *pListElem = (S_LIST_ELEM *)malloc(sizeof(pListElem));
    if (pListElem == NULL)
    {
        return 1;
    }
    S_LIST_ELEM *pListElemLast = get_list(hList, pList->length);
    pListElem->pNextList = NULL;
    pListElem->pElem = pElem;
    pListElemLast->pNextList = pListElem;

    return 0;
}
int remove_list(H_LIST hList, unsigned int num)
{
    if (hList == NULL)
    {
        return 1;
    }
    S_LIST *pList = (S_LIST *)hList;
    if (num <= pList->length && num > 0)
    {
        return 1;
    }
    S_LIST_ELEM *pListElem = get_list(hList, num);
    if (pListElem == NULL)
    {
        return 1;
    }
    if (num == 1)
    {
        pList->pFirst = pListElem->pNextList;
        free(pListElem);
    }
    else
    {
        S_LIST_ELEM *pListElemPre = get_list(hList, num-1);
        pListElemPre->pNextList=pListElem->pNextList;
        free(pListElem);
    }
    pList->length--;
    return 0;
}