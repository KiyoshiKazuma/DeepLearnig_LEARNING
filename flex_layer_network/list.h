#ifndef D_LIST
#define D_LIST

typedef struct list{
    void * pElem;//要素へのポインタ
    struct  list * pNextList;//次のリスト要素へのポインタ    
}S_LIST_ELEM;

typedef struct{
    S_LIST_ELEM * pFirst;//リストの先頭要素へのポインタ
    unsigned int length;//リストの長さ（要素数)
}S_LIST;

typedef void * H_LIST;

H_LIST create_list(void);
int delete_list(H_LIST hList);
S_LIST_ELEM * get_list(H_LIST hList,unsigned int num);
unsigned int find_list(H_LIST hList,void * pElem);
int append_list(H_LIST hList,void * pElem);
int remove_elem_list(H_LIST hList,unsigned int num);


#endif // D_LIST