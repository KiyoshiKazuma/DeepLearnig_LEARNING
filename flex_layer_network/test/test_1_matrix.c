#include <stdio.h>
#include <stdlib.h>
#include "../matrix.h"

int test_1_create_matrix (void);
int test_1_delete_matrix (void);
int test_1_add_matrix (void);
int test_1_product_matrix (void);
int test_1_print_matrix (void);

int main(void){
    int ret = 0;
    if(ret=test_1_create_matrix()!=0){
        printf("TEST NG in : test_1_create_matrix\n");
        printf("ERROR CODE : %d\n",ret);
    }
    if(ret=test_1_delete_matrix()!=0){
        printf("TEST NG in : test_1_delete_matrix\n");
    }
    if(ret=test_1_add_matrix()!=0){
        printf("TEST NG in : test_1_add_matrix\n");
    }
    if(ret=test_1_product_matrix()!=0){
        printf("TEST NG in : test_1_product_matrix\n");
    }
    if(ret=test_1_print_matrix()!=0){
        printf("TEST NG in : test_1_print_matrix\n");
    }

}

int test_1_create_matrix (void){
    H_MATRIX hMatrix=NULL;
    unsigned int row=3;
    unsigned int column=3;
    unsigned int size = row*column;

    hMatrix=create_matrix(3,3);
    //NULL CHECK
    if(hMatrix==NULL){
        return 1;
    }
    S_MATRIX *pMatrix = NULL;
    pMatrix = (S_MATRIX *)malloc(sizeof(S_MATRIX));
    
    //SHAPE CHECK
    if(pMatrix->column!=column || pMatrix->row!=row){
        return 2;
    }
    //SIZE CHECK
    if(pMatrix->size!=size){
        return 3;
    } 
    //ELEMENT POINTER CHECK
    if(pMatrix->pElem==NULL){
        return 4;
    }
    return 0;
}
int test_1_delete_matrix (void){
    return 0;
}
int test_1_add_matrix (void){
    return 0;
}
int test_1_product_matrix (void){
    return 0;
}
int test_1_print_matrix (void){
    return 0;
}

