#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WMDHorstNetwork.h"

static int check_neighbor_mac(const unsigned char *base_mac, const unsigned char *inter_mac);
static void print_neighbor_list(const CLIENT_INFO_OBJ client_info);



void remove_neighbors_local_aps (CLIENT_INFO_HANDLE pclient_info, int num)
{
	if (NULL == pclient_info || NULL == pclient_info->neighbor_link_head)
	{
		return;
	}
	
	NEIGHBOR_LIST_HANDLE pcur = pclient_info->neighbor_link_head, pnxt, tmp = NULL;
	int i, ret = 0;

	while (NULL != pcur) {
		if (pcur == pclient_info->neighbor_link_head) {
			ret = check_neighbor_mac(pclient_info->ap_mac, pcur->mac);
			if (ret >= 0 && ret < 8) {
				pclient_info->neighbor_link_head = pcur->next;
				free(pcur);
				pcur = pclient_info->neighbor_link_head;
				pclient_info->neighbor_cnt--;
			} 
			else
				break;
		}
		else
			break;
	}
	if (NULL == pcur) {
		return;
	}
	pnxt = pcur->next;
	for (i = 0; i < num && pnxt != NULL; i++) {
		ret = check_neighbor_mac(pclient_info->ap_mac, pnxt->mac);
		if (ret >= 0 && ret < 8) {
				pcur->next = pnxt->next;
				tmp = pnxt->next;
				free(pnxt);
				pnxt = tmp;
				pclient_info->neighbor_cnt--;
		} else {
			pcur = pcur->next;
			pnxt = pnxt->next;
		}
	}

}

//Horst Scanner can scan itself and it's sender
void remove_neighbors_common_aps(CLIENT_INFO_HANDLE pclient_info ,int len)
{
	if (NULL == pclient_info || NULL == pclient_info->neighbor_link_head) {
		return;
	}
	NEIGHBOR_LIST_HANDLE pcur = pclient_info->neighbor_link_head, pre, pnxt, p;
	int i, ret = 0;
	
	for (i = 0; i < len && pcur != NULL; i++) {
		pnxt = pcur->next;
		pre = pcur;
		while (NULL != pnxt) {
			ret = check_neighbor_mac(pnxt->mac, pcur->mac);
			if (ret >= 0 && ret < 8) {
				pre->next = pnxt->next;
				p = pnxt->next;    //store tmp's memery address	
				free(pnxt);        //free the memery
				pnxt = p;
				pclient_info->neighbor_cnt--;
			}else {
				pre  = pre->next;
				pnxt = pnxt->next;
			}
		}
		pcur = pcur->next;
	}
}


int check_neighbor_mac_flag(const unsigned char *local_mac, const unsigned char *neighbor_mac)
{
	if (NULL == local_mac || NULL == neighbor_mac) {
		return -2;
	}

	if (memcmp(local_mac, neighbor_mac, MAC_LEN - 1) == 0) {
		return (neighbor_mac[5] - local_mac[5]);
	}
	return -1;
}


//check if the neighbor ap is controled by local ac, yes return 1, no return 0, other return -1;
int check_neighbor_flag(const CLIENT_INFO_HANDLE all_client_info, int len, const NEIGHBOR_LIST_HANDLE pneighbor_ap)
{
#ifdef DEBUG
	assert(NULL != all_client_info);
	assert(NULL != pneighbor_ap);
#endif
	if (NULL == all_client_info || NULL == pneighbor_ap) {
		return -2;
	}
	int i, j, ret = 0;
	
	for (j = 0, i = 0; j < FD_MAX_SIZE; j++) {
		if (all_client_info[j].fd < 0)
			continue;
		ret = check_neighbor_mac_flag(all_client_info[j].ap_mac, pneighbor_ap->mac);
		if (ret >= 0 && ret < 8)
			return i;
		else {
			i++;
			continue;
		}
	}
	
	return -1;
}

static int check_neighbor_mac(const unsigned char *base_mac, const unsigned char *inter_mac)
{
	if (NULL == base_mac || NULL == inter_mac) {
		return -2;
	}

#if 1
	if (memcmp(base_mac, inter_mac, MAC_LEN - 1) == 0) {
		return abs(inter_mac[5] - base_mac[5]);
	}
#else
	if ((memcmp(base_mac, inter_mac, MAC_LEN - 3) == 0) && (base_mac[4] == inter_mac[4])) {
		if (inter_mac[5] == base_mac[5]){
			return abs(inter_mac[3] - base_mac[3]);
		}
	}
#endif
	return -1;
}

//free all the neighbors' nodes
void destroy_neighbor_list(CLIENT_INFO_HANDLE client_info)
{
	#ifdef DEBUG
		assert(NULL != client_info);
	#endif
	if (NULL == client_info) {
		return;
	}
	NEIGHBOR_LIST_HANDLE p = client_info->neighbor_link_head, q = NULL;
	
	while (NULL != p) {
		q = p->next;
		free(p);
		p = q;
	}
	client_info->neighbor_link_head = NULL;
	client_info->neighbor_cnt = 0;

	return;
}

//insert sort
void simp_link_insert_sort(CLIENT_INFO_HANDLE pclient_info)
{
	if (NULL == pclient_info || NULL == pclient_info->neighbor_link_head)
	{
		return;
	}

	NEIGHBOR_LIST_HANDLE pcur = pclient_info->neighbor_link_head->next, ptmp;
	NEIGHBOR_LIST_HANDLE pnew_head = pclient_info->neighbor_link_head;
	NEIGHBOR_LIST_HANDLE pnew_cur, pnew_nxt;
	
	pnew_head->next = NULL;
	while (NULL != pcur) {
		ptmp = pcur->next;
		if (pnew_head->rssi < pcur->rssi) {
			pcur->next = pnew_head;
			pnew_head = pcur;			
		} else {
			pnew_cur = pnew_head;
			pnew_nxt = pnew_head->next;
			while (NULL != pnew_nxt) {
				if (pcur->rssi > pnew_nxt->rssi) {
					pcur->next = pnew_nxt;
					pnew_cur->next = pcur;
					break;
				} else {
					pnew_cur = pnew_cur->next;
					pnew_nxt = pnew_nxt->next;
				}
			}
			if (NULL == pnew_nxt) {
				pnew_cur->next = pcur;
				pcur->next = NULL;
			}			
		}
		pcur = ptmp;
	}
	pclient_info->neighbor_link_head = pnew_head;
}

//function: deal with ap neighbors' info and return the number of the aps
int get_ap_nums(CLIENT_INFO_HANDLE all_client_info, int len)
{
	if (NULL == all_client_info || len <= 0) {
		return 0;
	}
	int i, cnt = 0;
	unsigned char mac[MAC_LEN] = { 0 };
	
	for (i = 0; i < len; i++) {
		if (all_client_info[i].fd > 0 
			&& memcmp(all_client_info[i].ap_mac, mac, MAC_LEN) != 0) {
			simp_link_insert_sort(&all_client_info[i]);
			print_neighbor_list(all_client_info[i]);
			remove_neighbors_common_aps(&all_client_info[i], all_client_info[i].neighbor_cnt);		
			remove_neighbors_local_aps(&all_client_info[i], all_client_info[i].neighbor_cnt);
			cnt++;
		}
	}
	return cnt;
}


int get_valid_neighbors_nums(const CLIENT_INFO_OBJ client_info)
{
	int cnt = 0;
	int max_nums = 0;// max_neighbors = 0;
	unsigned char mac[MAC_LEN] =  { 0 };
	NEIGHBOR_LIST_HANDLE pcur = client_info.neighbor_link_head;

#if 0
	if (0 == client_info.type) {
		max_neighbors = MAX_2G_VALID_CNT;
	} else if (1 == client_info.type) {
		max_neighbors = MAX_5G_VALID_CNT;
	}
#endif
	while (NULL != pcur && max_nums < MAX_VALID_CNT) {
		if (pcur->rssi > -65) {
			if (0 == client_info.type && pcur->channel <= 14 && 
				0 != memcmp(pcur->mac, mac, MAC_LEN)) {
				cnt++;
				max_nums++;
			} else if (1 == client_info.type && pcur->channel > 14
				&& 0 != memcmp(pcur->mac, mac, MAC_LEN)) {
				cnt++;
				max_nums++;
			}
		}
		pcur = pcur->next;
	}

	return cnt;
}


static void print_neighbor_list(const CLIENT_INFO_OBJ client_info)
{
	if (NULL == client_info.neighbor_link_head)
	{
		return;
	}
	int i, j;
	printf("------->MAC: \n");
	for (j = 0; j < MAC_LEN-1; j++) {
			fprintf(stdout, "%02x:", client_info.ap_mac[j]);
	}
	fprintf(stdout, "%02x\n", client_info.ap_mac[j]);
	
	NEIGHBOR_LIST_HANDLE ptmp = client_info.neighbor_link_head;
	for (i = 0; i < client_info.neighbor_cnt && NULL != ptmp; i++) {
		fprintf(stdout, "NO.%d--->\t", i+1);
		fprintf(stdout, "BSSID(-->MAC)--->");
		for (j = 0; j < MAC_LEN-1; j++) {
			fprintf(stdout, "%02x:", (unsigned char)(ptmp->mac[j]));
		}
		fprintf(stdout, "%02x\t", (unsigned char)(ptmp->mac[j]));
		fprintf(stdout, "RSSI: %d\t", (char)(ptmp->rssi));
		fprintf(stdout, "Channel: %d\n", (unsigned char)(ptmp->channel));
		ptmp = ptmp->next;
	}
}



