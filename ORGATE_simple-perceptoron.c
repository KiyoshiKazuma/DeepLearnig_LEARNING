#include <stdio.h>
#include <stdlib.h>
double w1=0.4,w2=0.4,w0=-0.6;

int perceptoron(int x1,int x2){
    double u=w0+w1*x1+w2*x2;
    if(u<0){
        return 0;
    }else{
        return 1;
    }
}

int main(int argc,char *argv[]){
    int x1,x2;

    if(argc!=3){
        printf("ERROR : set two number\n");
        return -1;
    }

    x1=atoi(argv[1]);
    x2=atoi(argv[2]);

    printf("ORGATE input : %d %d  ==>  %d \n",x1,x2,perceptoron(x1,x2));
    return 0;
}