#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "two_layer_net.h"
#include "loss_function.h"
#include "gradient.h"
#include "gradient_descent.h"
#include "teacher_file.h"
#include "network_data.h"

void main(){
    int neuron_size[MAX_LAYER_NETWORK]={0};
    int layer_size=0;
    int ret=get_network_info(&layer_size,neuron_size);
    if(ret!=0){
        printf("ERROR : get_network_info\n");
        printf("\t error num:%d\n",ret);
    }
    printf("%d\n%d\n%d\n%d\n",layer_size,neuron_size[0],neuron_size[1],neuron_size[2]);
}