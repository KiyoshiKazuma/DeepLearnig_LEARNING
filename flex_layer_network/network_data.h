#ifndef H_NETWORK
#define H_NETWORK
#include "configuration.h"

int net_data_amount(S_NETWORK * pnet);
int init_network_data(S_NETWORK net);
int get_network_info(S_NETWORK * pnet);
int read_network_data(S_NETWORK net,double **pnet_value);
int update_network_data(S_NETWORK net,double **pnet_value);
#endif