#ifndef WMDDEAL_FUNC_H
#define WMDDEAL_FUNC_H

int get_ap_nums(CLIENT_INFO_HANDLE all_client_info, int len);
void destroy_neighbor_list(CLIENT_INFO_HANDLE client_info);
int check_neighbor_flag(CLIENT_INFO_HANDLE all_client_info, 
	int len, NEIGHBOR_LIST_HANDLE pneighbor_ap);
void remove_neighbors_common_aps(CLIENT_INFO_HANDLE pclient_info ,int len);
int get_valid_neighbors_nums(const CLIENT_INFO_OBJ client_info);

#endif