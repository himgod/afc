#ifndef _WMDHORSTNETWORK_H
#define _WMDHORSTNETWORK_H

#include <sys/socket.h>
#include <arpa/inet.h>

#define    DEFAULT_PORT         "4444"
#define    PROTO_VERSION	    2
#define    BACKLOG              10       //listen队列中等待的连接数
#define    MAC_LEN              6
#define    MAXDATASIZE          2048      //缓冲区大小
#define    AUTELAN_GET_RESULT   0x5a
#define    AUTELAN_SET_UPLOAD   0X5b     //upload config
#define    FD_MAX_SIZE          (64)
#define    MAX_2G_VALID_CNT        (4)
#define    MAX_5G_VALID_CNT        (5)
#define    MAX_VALID_CNT         (5)
#define    NEIGHBORS_BUF_LEN        (512)

#define PROTO_VERSION	2


typedef struct _tagNEIGHBOR_LIST
{
	unsigned char mac[MAC_LEN];      //neighbor's mac
	unsigned char channel;           //neighbor's channel
	char neighbor_flag;     		 //neighbor's flag >=0:local AC, <0:other AC
	char rssi;                       //neighbor's RSSI
	//int txpower;                   //neighbor's transport power
	struct _tagNEIGHBOR_LIST *next;
}NEIGHBOR_LIST_OBJ, *NEIGHBOR_LIST_HANDLE;

typedef struct _tagCLIENT_INFO
{
    int fd;                      //AP's socket description
    int active;                  //socket status
    char name[32];               //AP's essid
    struct sockaddr_in addr;     //AP's ip address
    unsigned char channel;       //ap's channel
	unsigned char type;              //2.4G 0, 5G, 1
	unsigned char ap_mac[MAC_LEN];   //client's MAC
	NEIGHBOR_LIST_HANDLE neighbor_link_head;    //neighbors list
	int neighbor_cnt;           //neighbors' count
}CLIENT_INFO_OBJ, *CLIENT_INFO_HANDLE;


CLIENT_INFO_OBJ horst_client[FD_MAX_SIZE];  //FD_MAX_SIZE为select函数支持的最大描述符个数


struct net_header {
	unsigned char version;
	unsigned char type;
} __attribute__ ((packed));


struct net_extra_info {
	struct net_header	proto;
	unsigned char ap_mac[MAC_LEN];
	unsigned char scan_mac[MAC_LEN];
	unsigned char phy_mode;    //radio type: 2.4G or 5.8G
	unsigned char channel;     //current AP's channel
	unsigned char mem_cnt;
} __attribute__ ((packed));

struct neighbor_info {
	unsigned char mac[MAC_LEN];    //mac address
	char phy_signal;     //RSSI
	unsigned char channel;    //channel
}__attribute__ ((packed));


struct net_analyse_result {
	struct net_header	proto;
	unsigned char valid_cnt;    //neighbor's valid neighbors count
	char buf[MAXDATASIZE];
}__attribute__ ((packed));

struct net_conf_ctrl {
	struct net_header	proto;
	unsigned char cmd;
	unsigned int interval;    //upload information's interval of time, per minute
} __attribute__ ((packed));


typedef struct _tagMSGQ {
	int sock_fd;
	int buf_len;
	char buffer[MAXDATASIZE];
}MSGQ_OBJ, *MSGQ_HANDLE;

enum pkt_type {
	PROTO_PKT_INFO		= 0,
	PROTO_CHAN_LIST		= 1,
	PROTO_CONF_CHAN		= 2,
	PROTO_CONF_FILTER	= 3,
	PROTO_CONF_CTRL	    = 4,
	PROTO_EXTRA_INFO	= 5,
	PROTO_ANALYSE_RESULT	= 6,
};


extern int send_client_data(int fd, int tm_interval);
extern int deal_client_conn(CLIENT_INFO_HANDLE, int );
extern void *wmd_store_data(void *arg);
extern void getmsgid(int *msgqid);
#endif