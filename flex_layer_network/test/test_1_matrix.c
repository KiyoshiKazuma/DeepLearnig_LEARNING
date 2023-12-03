#include <stdio.h>
#include "../matrix.h"

int test_1_create_matrix (void);
int test_1_delete_matrix (void);
int test_1_add_matrix (void);
int test_1_product_matrix (void);
int test_1_print_matrix (void);

int main(void){
    if(test_1_create_matrix()!=0){
        printf("TEST NG in : test_1_create_matrix\n");
    }
    if(test_1_delete_matrix()!=0){
        printf("TEST NG in : test_1_delete_matrix\n");
    }
    if(test_1_add_matrix()!=0){
        printf("TEST NG in : test_1_add_matrix\n");
    }
    if(test_1_product_matrix()!=0){
        printf("TEST NG in : test_1_product_matrix\n");
    }
    if(test_1_print_matrix()!=0){
        printf("TEST NG in : test_1_print_matrix\n");
    }

}

int test_1_create_matrix (void){
    H_MATRIX hMatrix;
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

