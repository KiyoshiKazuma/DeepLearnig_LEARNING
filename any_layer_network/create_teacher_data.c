#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "teacher_file.h"

#define MESH_X (100)
#define MESH_Y (100)
#define MAX_X (1.0)
#define MIN_X (-1.0)
#define MAX_Y (1.0)
#define MIN_Y (-1.0)


int main(void){
    S_NETWORK net;
    S_MATRIX X;
    S_MATRIX T;
    F_CREATE_MATRIX(1,2,&X);
    F_CREATE_MATRIX(1,2,&T);
    X.elep[0]=0.0;
    X.elep[1]=1.0;
    T.elep[0]=1.0;
    T.elep[1]=0.0;
    int ret;

    srand((unsigned int)time(NULL));

    net.teacher_size=0;
    ret=init_teacher_file(2,2);
    
    for(int i=0;i<MESH_X;i++){
        for(int j=0;j<MESH_Y;j++){
            double x=MIN_X+(double)((MAX_X-MIN_X)*i)/(MESH_X-1.0);
            double y=MIN_Y+(double)((MAX_Y-MIN_Y)*j)/(MESH_Y-1.0);
            X.elep[0]=x;
            X.elep[1]=y;
            /*****ここに条件式を記述***/
            if(x*y>0){
                T.elep[0]=1.0;
                T.elep[1]=0.0;
            }else{
                T.elep[0]=0.0;
                T.elep[1]=1.0;
            }
            /*************************/
            add_teacher_data(&net,X,T);
        }

    }


    if(ret!=0){
        printf("ERROR:something happen!\n");
    }
    return 0;
}