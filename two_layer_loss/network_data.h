#ifndef H_NETWORK
#define H_NETWORK

int net_data_amount(int layer_size,int *hiden_size,int input_size,int output_size);
int init_network_data(int layer_size,int *hiden_size);
int get_network_info(int *layer_size,int *hiden_size);
int read_network_data(double **pnet_value,int net_amount);
int update_network_data(double **pnet_value,int net_amount);
#endif