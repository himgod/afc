#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <unistd.h>

#include <assert.h>
#include "WMDHorstNetwork.h"

//#define CFJ_DEBUG
int msgqid;
static int accept_client_connection(int listenfd, CLIENT_INFO_HANDLE client, int cnt);
static NEIGHBOR_LIST_HANDLE create_node(struct neighbor_info *pdata);
static int net_receive(CLIENT_INFO_HANDLE pclient);
static int deal_client_data(CLIENT_INFO_HANDLE pclient, char *buffer, int len);

static void init_client_info(CLIENT_INFO_HANDLE client_info, int len);
static void reinit_client_info(CLIENT_INFO_HANDLE pclient);

static int get_local_ap_info(char *buf, CLIENT_INFO_HANDLE client_info);
static int net_init_server_socket(void);

#ifdef CFJ_DEBUG
static void net_print_neighbor_list(char *buf);
static void net_print_extra_info(char *buf);
#endif
static int store_neighbors_list(char *buf, int len, CLIENT_INFO_HANDLE pclient_info);

extern void destroy_neighbor_list(CLIENT_INFO_HANDLE pclient);

void getmsgid(int *msgqid)
{
	key_t key;
	
	if ((key = ftok(".", 'W')) == -1) {
		perror("ftok");
		exit(1);
	}
	if ((*msgqid = msgget(key, IPC_CREAT|0644)) == -1) {
		perror("msgget");
		exit(1);
	}
}

//Recv thread: storage the neighbors' information into a link
void *wmd_store_data(void *arg)
{
	int ret = -1, i;

	MSGQ_OBJ msg;
	
	while (1) {
		ret = msgrcv(msgqid, (MSGQ_HANDLE)&msg, sizeof(msg), 0, 0);
		if (-1 == ret) {
			perror("msgrcv");
		}

		for (i = 0; i < FD_MAX_SIZE; i++) {
			if (msg.sock_fd == horst_client[i].fd) {
				deal_client_data(&horst_client[i], msg.buffer, msg.buf_len);
			}
		}
			
	}

	return NULL;
}


//
int deal_client_conn(CLIENT_INFO_HANDLE client, int connect_cnt)
{	
	int listenfd = net_init_server_socket();
	int sockfd;
	int nready, i, k;

	fd_set rset, allset;        //select所需的文件描述符集合
	
	//init select
    int maxfd = listenfd;
    int maxi = -1, n;

	init_client_info(client, connect_cnt);
    FD_ZERO(&allset);           //clear the socket descriptor set
    FD_SET(listenfd, &allset);  //add listened socket descriptor into  select detection set
	
	while (1) {
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);    //call select
        //printf("Select() break and the return num is %d. \n", nready);
		if (FD_ISSET(listenfd, &rset))
        {   //Detect whether there is a new client request
            i = accept_client_connection(listenfd, client, connect_cnt);
			if (i < 0) 
				continue;
			if (i == connect_cnt)
                printf("Too many clients!\n");
            FD_SET(client[i].fd, &allset); //add new socket into select detection set
            if (client[i].fd > maxfd)
                maxfd = client[i].fd;  //确认maxfd是最大描述符
            if (i > maxi)       //数组最大元素值
                maxi = i;
			//send_client_data(client[i].fd, 3);
			for (k = 0; k < connect_cnt; k++) {
				if (client[k].active > 0)
					send_client_data(client[k].fd, 3);    //send config info to client
			}

            if (--nready <= 0)
                continue;       //如果没有新客户端连接，继续循环
		}
		
		for (i = 0; i <= maxi; i++)
        {	
            if ((sockfd = client[i].fd) < 0)    //如果客户端描述符小于0，则没有客户端连接，检测下一个
                continue;
            // Have client connection, check if there is a data 
            if (FD_ISSET(sockfd, &rset))
            {
            	printf("You Receive Data from %s:%d.\n", 
					inet_ntoa(client[i].addr.sin_addr), ntohs(client[i].addr.sin_port));
				n = net_receive(&client[i]);
				if (n < 0) {
					FD_CLR(sockfd, &allset);    //从监听集合中删除此socket连接
					close(sockfd);
					reinit_client_info(&client[i]);
				}
                if (--nready <= 0)
                    break;      //如果没有新客户端有数据，跳出for循环回到while循环
            }
        }
	}
}

//accept the connection from the clients
static int accept_client_connection(int listenfd, CLIENT_INFO_HANDLE client, int cnt)
{
#ifdef CFJ_DEBUG
	assert(NULL != client);
#endif
	if (NULL == client)
		return -1;
		
	struct sockaddr_in cli_addr;
	socklen_t cli_len = sizeof(cli_addr);
	int connectfd = -1;
	int i;
	
	if ((connectfd =
        accept(listenfd, (struct sockaddr *)&cli_addr, (socklen_t *) &cli_len)) == -1)
    {
        perror("Accept: error");
        return -1;
    }
    //将新客户端的加入数组
    for (i = 0; i < cnt; i++) {
		if (client[i].fd < 0) {
            char buffer[20];
            client[i].fd = connectfd;   //保存客户端描述符
            client[i].active = 1;
            memset(buffer, '0', sizeof(buffer));
            sprintf(buffer, "Client[%.2d]", i);
            memcpy(client[i].name, buffer, strlen(buffer));
            client[i].addr = cli_addr;
            printf("You got a connection from %s:%d.\n", 
				inet_ntoa(client[i].addr.sin_addr), ntohs(client[i].addr.sin_port));
            printf("Add a new connection:%s\n", client[i].name);
            break;
        }
    }
	
	return i;	
} 

//
static int deal_client_data(CLIENT_INFO_HANDLE pclient, char *buffer, int len)
{
	if (NULL == pclient || NULL == buffer) {
		return 0;
	}
	
	int ret = 0;
	
	while (len > 0) {
		struct net_header *pbuf = (struct net_header *)buffer;
		if (pbuf->version != PROTO_VERSION) {
			fprintf(stderr, "ERROR: protocol version %x\n", pbuf->version);
			return 0;
		}
		switch (pbuf->type)
		{
			case PROTO_EXTRA_INFO:
				if (len < sizeof(struct net_extra_info)) {
					len = 0;
					return 0;
				}
				ret = get_local_ap_info(buffer, pclient);
				if (ret <= 0) {
					return 0;
				}
				len -= ret;
				buffer += ret;
			break;

			case PROTO_ANALYSE_RESULT:
				if (len < sizeof(struct net_header))
					return 0;
				ret = store_neighbors_list(buffer, len,  pclient);
				if (ret <= 0) {
					return 0;
				}
				len -= ret;
				buffer += ret;
			break;
		
			default:
				fprintf(stdout, "Not support!\n");
			break;
		}
	}
	
	return len;
}

static int net_receive(CLIENT_INFO_HANDLE pclient)
{
	int sockfd = pclient->fd;
	char recvbuf[MAXDATASIZE];
	MSGQ_OBJ msg;
	int ret = -1, len;
#if 1

	len = recv(sockfd, recvbuf, MAXDATASIZE, 0);
	if (len == 0) {
		fprintf(stderr, "Peer has shutdown!!!!\n");
		return -1;	
	}
	memset(msg.buffer, 0, MAXDATASIZE);
	msg.sock_fd = sockfd;
	msg.buf_len = len;
	memcpy(msg.buffer, recvbuf, len);
	//fprintf(stdout, "msgsnd: fd = %d, len = %d++++++++++++++++=\n", sockfd, len);
	ret = msgsnd(msgqid, &msg, sizeof(msg), 0);
	if (-1 == ret) {
		perror("msgsnd");
		return 0;
	}

	return 0;
#else
	int buflen = 0;
	int len, consumed = 0;

	char recvbuf[MAXDATASIZE];	//recv buffer
	len = recv(sockfd, recvbuf + buflen, MAXDATASIZE - buflen, 0);
	
	if (len == 0) {
		fprintf(stderr, "Peer has shutdown!!!!\n");
		return -1;	
	}
	printf("len = %d+++++++++++++++++++++++++++++\n", len);
	buflen += len;

	while (buflen > sizeof(struct net_header)) {
		len = deal_client_data(pclient, recvbuf + consumed, buflen);
		if (len == 0)
			break;
		buflen -= len;
		consumed += len;
	}
	memmove(recvbuf, recvbuf + consumed, buflen);

	return consumed;
#endif
}

/**********************************************************************
* Function Name: init_client_info
* Description: Init clients information.
* Input Param: 
*    all_client_info: client information pointer.
*    len: client's number
* Output Param: void
***********************************************************************/
static void init_client_info(CLIENT_INFO_HANDLE all_client_info, int number)
{
#ifdef CFJ_DEBUG
	assert(NULL != all_client_info);
#endif
	if (NULL == all_client_info)
		return;
	int i;
	for (i = 0; i < number; i++) {
		all_client_info[i].fd = -1;
		all_client_info[i].active = 0;
		all_client_info[i].neighbor_link_head = NULL;    //Neighbors' link head
		all_client_info[i].neighbor_cnt = 0;
		//local AP information
		all_client_info[i].type = -1;
		all_client_info[i].channel = 0;
		memset(all_client_info[i].ap_mac, 0, MAC_LEN);
	}
}

static void reinit_client_info(CLIENT_INFO_HANDLE pclient)
{
	if (NULL == pclient)
		return;

	pclient->fd = -1;    //数组元素设初始值，表示没客户端连接
	pclient->active = 0;
	pclient->type = -1;
	pclient->channel = 0;
	memset(pclient->ap_mac, 0, MAC_LEN);
	if (NULL != pclient->neighbor_link_head)
		destroy_neighbor_list(pclient);
	else
		pclient->neighbor_cnt = 0;

	return;		
}

//init server socket and return fd
static int net_init_server_socket(void)
{
	struct sockaddr_in sock_in;
	int reuse = 1;
	int srv_sockfd = -1;
	
	memset(&sock_in, 0, sizeof(struct sockaddr_in));
	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_in.sin_port = htons(atoi(DEFAULT_PORT));
	
	if ((srv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Could not open server socket.\n");
		return -1;
	}
	
	if (setsockopt(srv_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		fprintf(stderr, "setsockopt: %s\n", strerror(errno));
		return -1;
	}

	if (bind(srv_sockfd, (struct sockaddr*)&sock_in, sizeof(sock_in)) < 0) {
		fprintf(stderr, "bind: %s\n", strerror(errno));
		return -1;
	}
	
	if (listen(srv_sockfd, BACKLOG) < 0) {
		fprintf(stderr, "listen: %s\n", strerror(errno));
		return -1;
	}
		
	return srv_sockfd;
}

//config the information to clients
int send_client_data(int fd, int tm_interval)
{
	struct net_conf_ctrl ctrl;
	int ret = -1;
	
	ctrl.proto.version = PROTO_VERSION;
	ctrl.proto.type = PROTO_CONF_CTRL;
	ctrl.cmd = AUTELAN_SET_UPLOAD;   // AUTELAN_GET_RESULT = 0x5a
	ctrl.interval = tm_interval;
	
	ret = send(fd, (unsigned char *)&ctrl, sizeof(ctrl), 0);
	
	#ifdef CFJ_DEBUG
	fprintf(stdout, "has send %d bytes to client!\n", ret);
	#endif
	
	return ret;
}

//Get local AP's information.
static int get_local_ap_info(char *buf, CLIENT_INFO_HANDLE client_info)
{
	if (NULL == buf || NULL == client_info) {
		return 0;
	}
	struct net_extra_info *pbuf = (struct net_extra_info*)buf;
	
	if (0x2a == pbuf->phy_mode) {
		client_info->type = 0;
	} else if (0x5a == pbuf->phy_mode) {
		client_info->type = 1;
	}
	client_info->channel = pbuf->channel;
	//fprintf(stdout, "CHANNEL = %d\n", client_info->channel);

	if (abs(pbuf->scan_mac[5] - pbuf->ap_mac[5]) < 0x08) {    //Get radio base MAC
		memcpy(client_info->ap_mac, pbuf->ap_mac, MAC_LEN);
	} else {
		pbuf->ap_mac[5] += 0x08;
		memcpy(client_info->ap_mac, pbuf->ap_mac, MAC_LEN);
	}
	client_info->neighbor_cnt = pbuf->mem_cnt;
	//fprintf(stdout, "CNT = %d\n", pbuf->mem_cnt);
	
#ifdef CFJ_DEBUG
	net_print_extra_info(buf);
#endif

	return sizeof(struct net_extra_info);
}

#ifdef CFJ_DEBUG
static void net_print_extra_info(char *buf)
{
	struct net_extra_info *pbuf = (struct net_extra_info*)buf;
	int j;
	for (j = 0; j < MAC_LEN-1; j++) {
		fprintf(stdout, "%02x:", (unsigned char)(pbuf->ap_mac[j]));
	}
	fprintf(stdout, "%02x \t", (unsigned char)(pbuf->ap_mac[j]));
	fprintf(stdout, "Channel: %d \t", (unsigned char)(pbuf->channel));
	fprintf(stdout, "Radio Type: %d\t", (unsigned char)pbuf->phy_mode);
	fprintf(stdout, "Neighbor Nums: %d\n", (unsigned char)pbuf->mem_cnt);
}
#endif

//simple link, create a simple link;
static int store_neighbors_list(char *buf, int len, CLIENT_INFO_HANDLE pclient_info)
{
	if (NULL == buf || NULL == pclient_info)
	{
		return 0;
	}
	struct net_analyse_result neighbor_ap_info;
	int i, offset = 0;
	int valid_neighbor_cnt;
	
	memcpy((char *)&neighbor_ap_info, buf, len);
	valid_neighbor_cnt = neighbor_ap_info.valid_cnt;
	for (i = 0; i < valid_neighbor_cnt; i++) {	
		NEIGHBOR_LIST_HANDLE pnew = create_node((struct neighbor_info *)(neighbor_ap_info.buf+offset));	
		if (NULL == pclient_info->neighbor_link_head) {
			pclient_info->neighbor_link_head = pnew;
			pclient_info->neighbor_cnt++;
		} else {
			NEIGHBOR_LIST_HANDLE pcur = pclient_info->neighbor_link_head;
			while (NULL != pcur->next)
				pcur = pcur->next;
			pcur->next = pnew;
			pclient_info->neighbor_cnt++;
		}
		offset += sizeof(struct neighbor_info);
	}
		
	return offset+sizeof(struct net_header)+1;
}

//create a node of a neighbor 
static NEIGHBOR_LIST_HANDLE create_node(struct neighbor_info *pdata)
{
	NEIGHBOR_LIST_HANDLE pnew = NULL;
	pnew = (NEIGHBOR_LIST_HANDLE)malloc(sizeof(NEIGHBOR_LIST_OBJ));
	if (NULL == pnew) {
		perror("malloc error");
		return NULL;
	}
	memcpy(pnew->mac, pdata->mac, MAC_LEN);
	pnew->rssi    = pdata->phy_signal;
	pnew->channel = pdata->channel;
	pnew->next    = NULL;
	
	return pnew;
}

#ifdef CFJ_DEBUG
static void net_print_neighbor_list(char *buf)
{
	struct net_analyse_result *pbuf = (struct net_analyse_result*)buf;
	int j;

	fprintf(stdout, "NO.%d--->\t", pbuf->tag);
	fprintf(stdout, "BSSID(-->MAC)--->");
	for (j = 0; j < MAC_LEN-1; j++) {
		fprintf(stdout, "%02x:", (unsigned char)(pbuf->mac[j]));
	}
	fprintf(stdout, "%02x\t", (unsigned char)(pbuf->mac[j]));
	fprintf(stdout, "RSSI: %d\t", (char)(pbuf->phy_signal));
	fprintf(stdout, "Channel: %d\n", (unsigned char)(pbuf->channel));
}
#endif
