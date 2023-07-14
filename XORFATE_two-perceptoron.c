#include <stdio.h>
#include <stdlib.h>
double w1or=0.4,w2or=0.4,w0or=-0.2;
double w1and=0.4,w2and=0.4,w0and=-0.6;
double w1nand=-0.4,w2nand=-0.4,w0nand=0.6;
int orperceptoron(int x1,int x2){
    double u=w0or+w1or*x1+w2or*x2;
    if(u<0){
        return 0;
    }else{
        return 1;
    }
}
int andperceptoron(int x1,int x2){
    double u=w0and+w1and*x1+w2and*x2;
    if(u<0){
        return 0;
    }else{
        return 1;
    }
}int nandperceptoron(int x1,int x2){
    double u=w0nand+w1nand*x1+w2nand*x2;
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

    printf("XORGATE input : %d %d  ==>  %d \n\tANDGATE input : %d %d  ==>  %d \n\tNANDGATE input : %d %d  ==>  %d \n",x1,x2,andperceptoron(orperceptoron(x1,x2),nandperceptoron(x1,x2)),x1,x2,andperceptoron(x1,x2),x1,x2,nandperceptoron(x1,x2));
    return 0;
}