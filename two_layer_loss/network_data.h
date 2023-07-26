#ifndef H_NETWORK
#define H_NETWORK
#define MAX_LAYER_NETWORK (100)

#define NEURON_FILE_NAME "neuron.dat"

int init_network_data(int layer_size,int *neuron_size);
int get_network_info(int *layer_size,int *neuron_size);
int read_network_data(double **pnet_value);
int update_network_data(double **pnet_value);
#endif