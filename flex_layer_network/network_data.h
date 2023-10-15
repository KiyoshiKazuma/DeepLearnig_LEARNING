#ifndef H_NETWORK
#define H_NETWORK


int net_data_amount(S_NETWORK * pnet);
int init_network_data(S_NETWORK net);
int get_network_info(int *layer_size,int *neurons_size);
int read_network_data(double **pnet_value,int net_amount);
int update_network_data(double **pnet_value,int net_amount);
#endif