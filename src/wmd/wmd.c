#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>


#include "WMDHorstNetwork.h"

//#define CFJ_DEBUG_DEMO

#ifdef CFJ_DEBUG_DEMO
#include <unistd.h>
//for test DBus
static void test_for_demo(CLIENT_INFO_HANDLE all_client_info, int len);
#endif
extern void *wmd_dbus_thread();


extern int msgqid;

int main(int argc, char *argv[])
{
	pthread_t thread_dbus;
	pthread_t thread_rcv;
	getmsgid(&msgqid);
	pthread_create(&thread_dbus, NULL, wmd_dbus_thread, NULL);
	pthread_create(&thread_rcv, NULL , wmd_store_data, NULL);

	deal_client_conn(horst_client, FD_MAX_SIZE);
	#endif
	return 0;
}

#ifdef CFJ_DEBUG_DEMO
static void rand_generated_mac(unsigned char *mac)
{
	int i;
	for (i = 0; i < 6; i++)
		mac[i] = rand() % 255;
}
static void print_mac(unsigned char *mac)
{
	int i;
	
	for (i = 0; i < 5; i++)
		fprintf(stdout, "%02x:", mac[i]);
	fprintf(stdout, "%02x", mac[i]);
}

static NEIGHBOR_LIST_HANDLE create_node_demo(NEIGHBOR_LIST_OBJ data)
{
	NEIGHBOR_LIST_HANDLE pnew = NULL;
	pnew = (NEIGHBOR_LIST_HANDLE)malloc(sizeof(NEIGHBOR_LIST_OBJ));
	if (NULL == pnew) {
		perror("malloc error");
		return NULL;
	}
	memcpy(pnew->mac, data.mac, MAC_LEN);
	pnew->rssi    = data.rssi;
	pnew->channel = data.channel;
	pnew->neighbor_flag = data.neighbor_flag;
	pnew->next    = NULL;
	
	return pnew;
}

static void create_link(CLIENT_INFO_HANDLE one_client, NEIGHBOR_LIST_OBJ neighbor_ap)
{
	NEIGHBOR_LIST_HANDLE pnew = create_node_demo(neighbor_ap);
		
	if (NULL == one_client->neighbor_link_head) {
		one_client->neighbor_link_head = pnew;
		one_client->neighbor_cnt++;
	} else {
		NEIGHBOR_LIST_HANDLE cur = one_client->neighbor_link_head;
		while (NULL != cur->next)
			cur = cur->next;
		cur->next = pnew;
		one_client->neighbor_cnt++;
	}
}


static void create_ap_info(CLIENT_INFO_HANDLE ap_info, int lens)
{
#ifdef DEBUG
	assert(NULL != ap_info);
#endif
	if (NULL == ap_info) {
		fprintf(stderr, "Error parameter!\n");
		return;
	}
	int i, j;
	int idx, chan_idx, mac_idx;
	#define    NEIGHBOR_NUMS  5

	unsigned char channel5[] = { 149, 153, 157, 161, 165 };
	unsigned char channel2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
	
	srand(time(NULL));
	
	//create the aps' information
	for (i = 0; i < lens; i++) { 
		//sleep(1);
		rand_generated_mac(ap_info[i].ap_mac); //create mac address
			
		ap_info[i].channel = 0; //undistributed
		ap_info[i].fd = i+1;
		//ap_info[i].txpower = rand() % 100;
		
		idx = rand() % 2;    //0: 2.4G, 1:5.8G
		ap_info[i].type = idx;
	}

	NEIGHBOR_LIST_OBJ tmp;
	memset(&tmp, 0, sizeof(tmp));
	//create the ap's neighbors' information
	for (i = 0; i < lens; i++) { 
		
		unsigned int flag = 0u;
		for (j = 0; j < NEIGHBOR_NUMS; j++) {
			idx = rand() % 2;    //0: controled by local ap, 1: not
			tmp.neighbor_flag = idx;
			if (idx) {	//not local ap
				rand_generated_mac(tmp.mac);
				if (ap_info[i].type) {    //5.8G
					chan_idx = rand () % 5; 
					tmp.channel = channel5[chan_idx];
				} else {    //2.4G
					chan_idx = rand () % 13; 
					tmp.channel = channel2[chan_idx];
				}
				tmp.rssi = rand() % 100 + 1;
			} else {    //local ap
				while (1) {
					mac_idx = rand () % lens;
					if ((mac_idx != i) && ((flag & (1u << mac_idx)) == 0) 
							&& (ap_info[mac_idx].type == ap_info[i].type)) {
						flag |= (1u << mac_idx);
						break;
					}
				}
				memcpy(tmp.mac, ap_info[mac_idx].ap_mac, 6);
				tmp.channel = 0;
				tmp.rssi = rand() % 100;
			}
			create_link(&ap_info[i], tmp);
			memset(&tmp, 0, sizeof(tmp));
		}
	}
}


static void print_all_client_info(CLIENT_INFO_HANDLE all_client_info, int len)
{
	int i, j;

	for (i = 0; i < len; i++) {
		fprintf(stdout, "AP MAC: ");
		print_mac(all_client_info[i].ap_mac);
		if (all_client_info[i].type)
			fprintf(stdout, "   Radio Type:5.8G");
		else
			fprintf(stdout, "   Radio Type:2.4G");
		fprintf(stdout, "   Channel:%d", all_client_info[i].channel);
		//fprintf(stdout, "   TxPower:%d\n", all_client_info[i].txpower);
		fprintf(stdout, "Neighbor List: \n");
		NEIGHBOR_LIST_HANDLE ptmp = all_client_info[i].neighbor_link_head;
		for (j = 0; j < all_client_info[i].neighbor_cnt; j++) {
			fprintf(stdout, "No.%d-->", j+1);
			fprintf(stdout, "Neighbor MAC: ");
			print_mac(ptmp->mac);
			if (ptmp->neighbor_flag) {
				fprintf(stdout, "  Not Local AP");
			} else {
				fprintf(stdout, "      Local AP");
			}
			fprintf(stdout, "    Channel:%3d", ptmp->channel);
			fprintf(stdout, "    Rssi: %d\n", ptmp->rssi);
			ptmp = ptmp->next;
		}
	}
}

static void test_for_demo(CLIENT_INFO_HANDLE all_client_info, int len)
{
	create_ap_info(all_client_info, len);
	print_all_client_info(all_client_info, len);

	while (1) {
		//create_ap_info(all_client_info, len);	
		sleep(180);
	}
}

#endif
