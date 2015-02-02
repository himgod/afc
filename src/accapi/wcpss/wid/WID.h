/*
*this file define structs descripting framework and information of 
*WLAN/WTP/STA which creared,added,set by AC
*/



#ifndef _WID_DEFINE_H
#define _WID_DEFINE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include "wcpss/waw.h"
#include "dbus/wcpss/ACDbusDef1.h"
#include "ACMsgq.h"
#include <linux/tipc.h>
#include<unistd.h>
#include<stdio.h>
#ifndef _WID_TYPE_DEF
#define _WID_TYPE_DEF

#ifdef _LLONG_AVAILABLE
typedef unsigned long long u64;
#else
/*
 * TODO: 64-bit variable not available. Using long as a workaround to test the
 * build, but this will likely not work for all operations.
 */
typedef unsigned long u64;
#endif
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef unsigned char 		u_int8_t;
typedef unsigned short 		u_int16_t;
typedef unsigned int		u_int32_t;
#endif/*_WCPSS_TYPE_DEF*/
#define WID_ID_LEN	32
#define WID_MAXN_INDEX 64
#define WID_IP_LEN	4
#define WID_MAC_LEN	6
#define WID_DEFAULT_NUM	16
#define WPA_ELEMENT_ID 221
//#define ETH_IF_NAME_LEN 16/*Added by weiay 20080710*/
#define ROGUE_AP_REPORT_INT 3600				/*xdw modify, change 120 to 3600, 20101217*/
#define WIFI_IOC_IF_CREATE  _IOWR(243, 1, struct interface_basic_INFO)
#define WIFI_IOC_IF_DELETE  _IOWR(243, 2, struct interface_basic_INFO) 
#define WIFI_IOC_IF_UPDATE  _IOWR(243, 6, struct interface_INFO)
#define WIFI_IOC_WSM_SWITCH  _IOWR(243, 7, unsigned int)
#define WIFI_IOC_ASD_SWITCH  _IOWR(243, 8, unsigned int)/*ht add for wifi and asd communicate switch,110308*/
#define WIFI_IOC_HANSISTATE_UPDATE 	_IOWR(243, 9, unsigned int) /*wuwl add control data packet*/
#define WIFI_IOC_ASD_THRED_ID _IOWR(243,10,unsigned int)/*wuwl add asd pid notice to wifi for netlink*/
#define WIFI_IOC_ADD_STA		_IOWR(243, 11, struct asd_to_wifi_sta) 
#define WIFI_IOC_DEL_STA		_IOWR(243, 12, struct asd_to_wifi_sta)
#define WIFI_IOC_BATCH_IF_CREATE  _IOWR(243, 13, struct interface_batch_INFO)
#define WIFI_IOC_BATCH_IF_DELETE  _IOWR(243, 14, struct interface_batch_INFO)
#define WIFI_IOC_QOS_MAP_UPDATE  _IOWR(243, 15, struct wifi_qos_map_info)

#define TEST_SWITCH_WAY  1 /*zhanglei change*/
#define WID_SYSTEM_CMD_LENTH 256
#define WTP_WEP_NUM 4
#define DEFAULT_SN_LENTH 20
#define D_LEN	128
#define SECTOR_NUM 4
#define TX_CHANIMASK_NUM 3

#define USER_GROUP_NUM 17
#define USER_GROUP_DEFAUID 1
/* xingqj add for aclgrp */
#define ACL_GROUP_NUM 129
#define ACL_RULE_NUM 1025
#define GROUP_ACL_COUNT 64
#define STANDARD_ACLGRP_RULE 0
#define EXTENDED_ACLGRP_RULE 1
#define INSTANCE_CREATED		(1)		/* instance have created.		*/
#define INSTANCE_NO_CREATED	(0)		/* instance have not created.	*/

/*dynamic-vlan*/
#define ADD_DYNAMIC_VLAN   1
#define DELETE_DYNAMIC_VLAN   0
/* add for electronic-menu */
#define MAC_GROUP_NUM 129
#define GROUP_MAC_COUNT 128

//add for electronic-menu
typedef struct MACLIST
{	
	unsigned char mac[MAC_LEN];
	struct MACLIST *next;
}MAC_list;

typedef enum
{
	GROUP_POLICY_NONE = 0,
	GROUP_POLICY_WHITE = 1,
	GROUP_POLICY_BLACK = 2
}mac_group_policy;

typedef enum
{
	LEVEL_AP	= 0,
	LEVEL_RADIO	= 1,
	LEVEL_WLAN	= 2
}mac_level;

typedef enum
{
	OP_ADD	= 0,
	OP_DEL	= 1,
	OVERWRITE	= 2,
	CANCEL 	=3
}mac_oprate;

typedef struct BSSLIST
{
	u_int32_t 	BSSIndex;
	struct BSSLIST 	*next;
}BSSlist;

typedef struct WTPLIST
{
	u_int32_t 	WTPIndex;
	struct WTPLIST 	*next;
}WTPlist;

typedef struct WLANLIST
{
	u_int32_t 	WlanID;
	struct WLANLIST	*next;
}WLANlist;

typedef struct
{
	u_int32_t	macgroupID;
	char		*name;
	pthread_mutex_t macListMutex;
	MAC_list	*macList;
	u_int8_t	count;
	WTPlist		*WTPList;
	WLANlist	*WLANList;
}WID_MAC_GROUP;

extern WID_MAC_GROUP	*MAC_GROUP[MAC_GROUP_NUM];

typedef struct
{
	u_int8_t macgrp_num;
	WID_MAC_GROUP **MACGROUP;
}DCLI_MACGRP_API_GROUP;

#define WIFI_IOC_MAGIC 244
#define WIFI_IOC_IP_ADD   _IOWR(WIFI_IOC_MAGIC, 4, ex_ip_info)
#define WIFI_IOC_IP_DEL   _IOWR(WIFI_IOC_MAGIC, 5, ex_ip_info)

#define		CW_CREATE_OBJECT_ERR(obj_name, obj_type, on_err)	{obj_name = (obj_type*) (malloc(sizeof(obj_type))); if(!(obj_name)) {on_err}}
#define		CW_FREE_OBJECT(obj_name)				{if(obj_name){free((obj_name)); (obj_name) = NULL;}}
#define		CW_CREATE_STRING_ERR(str_name, str_length, on_err)	{str_name = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if(!(str_name)) {on_err}}
#define		CW_CREATE_STRING_ERR_UNSIGNED(str_name, str_length, on_err)	{str_name = (unsigned char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if(!(str_name)) {on_err}}

//#define		CW_ADD_OBJECT_ELE(obj_name1, obj_type1,obj_count1,,obj_ele1,obj_name2,obj_type2)	{if((obj_name2 == NULL)||((*obj_name1) == NULL)){printf("insert_elem_into_list_heads parameter error\n");	return -1;}	if((*obj_name1)->obj_count1 == 0){(*obj_name1)->obj_ele1 = obj_name2;(*obj_name1)->obj_count1++;return 0;}struct obj_type2 *pnode = (*obj_name1)->obj_ele1;(*obj_name1)->obj_ele1 = obj_name2;obj_name2->next = pnode;(*obj_name1)->obj_count1++; return 0;}
#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifndef OUIMAC2STR
#define OUIMAC2STR(a) (a)[0],(a)[1],(a)[2]
#define OUIMACSTR "%02x:%02x:%02x"
#endif 

/*dynamic-vlan*/
#define ADD_DYNAMIC_VLAN   1
#define DELETE_DYNAMIC_VLAN   0

typedef struct ex_ip_INFO
{
 unsigned char  if_name[16];
 unsigned int dip;
 unsigned int sip;
 unsigned char wtpmac[6];
}ex_ip_info;

/*add 20130225 for ENR4-3*/
struct vlangrpm{
	unsigned int radioid;
	struct vlangrpm *next; 
};

typedef struct{
	char name[VLANGRP_NAME_LEN];
	unsigned int group_id;
	unsigned int radio_num;
	unsigned int wlan_id;
	unsigned int vlan_id;
	pthread_mutex_t radioListMutex;
	struct vlangrpm *radio_member;
	unsigned char service_policy;
}WID_VLAN_GROUP;
/*****************Qos mapping******/

enum qos_map_type{
	QOS_MAP_11E_TO_1P = 0,
	QOS_MAP_DSCP_TO_DSCP = 1,
	QOS_MAP_DSCP_TO_1P = 2,
	QOS_MAP_11E_TO_DSCP = 3,
	QOS_MAP_DSCP_TO_11E = 4,
	QOS_MAP_1P_TO_11E = 5
};

struct wifi_qos_map_info{
	unsigned char BSSID[MAC_LEN];
	unsigned int type;
	unsigned char map[MAX_QOS_DSCP_VAL+1];
};

/************end*******************/

/*

#define WLAN_NUM		15
#define WTP_NUM		15
#define L_RADIO_NUM		4
#define G_RADIO_NUM		60
#define BSS_NUM		240

typedef struct{
	u_int8_t WlanID;
	char WlanName[20];
	char ESSID[20];
}wASD_WLAN;

typedef struct{	
	u_int8_t	BSSIndex;
	u_int8_t	Radio_L_ID;
	u_int8_t	Radio_G_ID;
	u_int8_t	WlanID;
	char	BSSID[6];

}wASD_BSS;



typedef enum{
	WID_ADD = 0,
	WID_DEL = 1,
	WID_MODIFY = 2
}WIDOperate;

typedef enum{
	WLAN_TYPE = 0,
	WTP_TYPE = 1,
	RADIO_TYPE = 2,
	BSS_TYPE = 3
}WIDMsgType;

typedef struct {
	WIDMsgType MsgType;
	WIDOperate MsgOp;
	wASD_WLAN WLAN;
	wASD_BSS  BSS;
}WIDMsg;
*/
typedef enum {
	CW_FALSE_DCLI = 0,
	CW_TRUE_DCLI = 1
} CWBool_DCLI;
enum wid_debug{
	WID_DEFAULT = 0x1,
	WID_DBUS = 0x2,
	WID_WTPINFO = 0x4,
	WID_MB = 0x8,/*master and bak*/
	WID_ALL = 0xf
};

typedef enum {
	WID_WAIT_UPGRADE = 0,
	WID_IS_UPGRADING = 1,
	WID_UPGRADE_SUCCESS = 2,
}WTPUpdateState;

typedef enum {
	WID_LOGIN = 0,
	WID_RELOGIN = 1,
}WTPLoginState;

typedef enum
{
	WID_SULKING = 0,
	WID_DISCOVERY = 1,
	WID_JOIN = 2,
	WID_CONFIGURE = 3,
	WID_DATA_CHECK = 4,
	WID_RUN = 5,
	WID_RESET = 6,
	WID_QUIT = 7,
	WID_IMAGE_DATA = 8,
	WID_BAK_RUN = 9,
	WID_OFFLINE = 10,
	WID_REJOIN = 11,
	WID_WIRE_UPSIDE = 12,
}WTPState;

typedef enum
{
	WTP_INIT = 0,
	WTP_UNUSED = 1,
	WTP_NORMAL = 2,
	IF_NOINDEX = 3,
	IF_NOFLAGS = 4,
	IF_DOWN = 5,
	IF_NOADDR = 6,	
	WTP_TIMEOUT = 7
}WTPQUITREASON;

struct radio_info_type_dcli{
	char radio_type;
	char radio_id;
	char bss_count;
	char txpower;
	char reserved1;
	char reserved2;
	unsigned short reserved3;	
};


typedef enum{
	DOWN_LINK_IF_TYPE = 0x1,		
	DOWN_LINK_IP_TYPE = 0x2,
	LIC_TYPE = 0x4
}LISTEN_FLAG;

struct disturb_info
{
	unsigned char disturb_type;
	unsigned char disturb_rssi;
};

struct wid_radio_disturb
{
	unsigned char disturb_num;
	struct disturb_info radio_disturb[DEFAULT_LEN];
};
typedef struct wid_radio_disturb radio_disturb_info;


struct ifi {
  char    ifi_name[ETH_IF_NAME_LEN];	/* interface name, null-terminated */
  short   ifi_index;			/* interface index */
  unsigned int nas_id_len;
  char nas_id[NAS_IDENTIFIER_NAME];
  char isipv6addr;
  unsigned int addr;
  struct ifi  *ifi_next;	/* next of these structures */
  struct ifi  *ifi_list;
  struct ifi  *ifi_last;
  LISTEN_FLAG lic_flag;
};

typedef struct
{	
	unsigned char Type;
	unsigned char Op;
	unsigned char L_RadioID;
	unsigned char state;   
	unsigned int distance;
	
}Acktimeout;/*wcl add for RDIR-33*/

typedef struct
{	
	unsigned char Type;
	unsigned char Op;
	unsigned char L_RadioID;
	unsigned char WlanID;
	unsigned char Able;
	unsigned char subframe;  //zhangshu add, 2010-10-09
	unsigned int AmpduLimit;
	
}AmpduParameter;

/* zhangshu add for amsdu, 2010-10-09 */
typedef struct
{	
	unsigned char Type;
	unsigned char Op;
	unsigned char L_RadioID;
	unsigned char WlanID;
	unsigned char Able;
	unsigned char subframe;  //zhangshu add, 2010-10-09
	unsigned int AmsduLimit;
	
}AmsduParameter;

typedef struct
{	
	unsigned char Type;
	unsigned char Op;
	unsigned char L_RadioID;
	unsigned char WlanID;
	unsigned char Mixed_Greenfield;
	unsigned char Wlan_Mixed[WLAN_NUM];	
}MixedGreenfieldParameter;


struct tag_wtpid{
	unsigned int wtpid;
	struct tag_wtpid *next;
};

struct tag_wtpid_list{
 	 int count;	 
	 struct tag_wtpid * wtpidlist;
} ;
typedef struct tag_wtpid_list update_wtp_list;

struct tag_radioid{
	unsigned int radioid;
	struct tag_radioid *next; 
};

struct tag_radioid_list{
 	int count;	 
	struct tag_radioid * radioidlist;
};
typedef struct tag_radioid_list update_radio_list;

struct wlanid{
	unsigned char wlanid;
	struct wlanid *next; 
};

struct Radio_Wlan_Pair{
	unsigned char radioid;
	unsigned char wlanid;
};

struct interface_INFO{
	// wifi kernal module only support max count 15,this will change late when kernel support more weianying 2010/04/21
	char if_name[ETH_IF_NAME_LEN-1];
	unsigned char wlanID;
    int    BSSIndex;	
	unsigned int vrid;
	unsigned int isIPv6;
	unsigned int acip;
	unsigned int apip;
	unsigned char acipv6[IPv6_LEN];
	unsigned char apipv6[IPv6_LEN];
	unsigned int protect_type;
	unsigned short acport;
	unsigned short apport;
	unsigned char bssid[MAC_LEN];	
	unsigned char apmac[MAC_LEN];
	unsigned char acmac[MAC_LEN];
	unsigned char ifname[ETH_IF_NAME_LEN];	
	unsigned char WLANID;
	unsigned char wsmswitch;
	unsigned char if_policy;		/*0-local,1-tunnel,ht add 110314*/
	unsigned char Eap1XServerMac[MAC_LEN];	
	unsigned char Eap1XServerSwitch;
	unsigned char f802_3;//if 1---capwap+802.3,else 0---802.11
	unsigned short	vlanid;	
	unsigned char	vlanSwitch;
	unsigned char apname[DEFAULT_LEN];
	unsigned char essid[ESSID_LENGTH];
	unsigned char roaming_flag;
#if DYNAMIC_VLAN_SUPPORT
	unsigned short dyvlanid;      /* dynamic vlan */
#endif	
#if WIFI_QOS_SUPPORT
	unsigned char qos_11e_to_1p[MAX_QOS_11E_VAL+1];
	unsigned char qos_dscp_to_1p[MAX_QOS_DSCP_VAL+1];
	unsigned char qos_dscp_to_dscp[MAX_QOS_DSCP_VAL+1];
#endif
};
typedef struct interface_INFO IF_info;

struct interface_basic_INFO{
	char if_name[ETH_IF_NAME_LEN-1];
	unsigned char wlanID;
    int    BSSIndex;	
	unsigned int vrid;
};
typedef struct interface_basic_INFO if_basic_info;

struct interface_batch_INFO{
	int count;
	if_basic_info ifinfo[320];
};
typedef struct interface_batch_INFO if_batch_info;

typedef struct HANSI_INFO
{
	unsigned int instId;	
	unsigned char hpre_state;
	unsigned char hstate;
	unsigned char	vlanSwitch;	
	unsigned int asd_pid;	
	unsigned char dhcpoption82;
}HANSI_info;

/*added by weiay for rogue ap detection 2008/11/11*/
struct Neighbor_AP_ELE
{
	unsigned char BSSID[MAC_LEN];
	unsigned short Rate; /*10 - 540 1080*/
	
	unsigned char Channel; /* 1 - 11	*/
	unsigned char RSSI;
	unsigned char NOISE;
	unsigned char BEACON_INT;
	
	unsigned int identity_flag;
	unsigned int wtpid;
	unsigned char l_radioid;
	unsigned char status;		/*0 none 1 rogue ap 2 normal ap*/
	unsigned char opstatus;
	unsigned char agingstatus;  /*0 new:add 1 exist:do nothing 2 Count reached critical mass, delete*/
	unsigned short capabilityinfo;
	unsigned char encrp_type; /*/0 x 1 none 2 web 3shared*/
	unsigned char polcy; /* 0 no policy 1 have policy*/
	
	time_t fst_dtc_tm;
	time_t lst_dtc_tm;
	
	/*added more status*/

	/*fengwenchao add 20110401 for dot11RogueTable*/
	/*---------------------------------------------------------------------------*/
	unsigned int RogueAPAttackedStatus;  //是否对非法AP进行攻击，在黑名单中存在--1，不存在--0。
	unsigned int RogueAPToIgnore;        //是否忽略非法AP，在白名单中存在--1，不存在--0.
	/*---------------------------------------------------------------------------*/
	unsigned char ESSID[ESSID_DEFAULT_LEN+1];
	char *IEs_INFO;
	struct Neighbor_AP_ELE *next;
	struct Neighbor_AP_ELE *neighborapInfos_list;
	struct Neighbor_AP_ELE *neighborapInfos_last;
}Neighbor_AP_ELE;


/*add for cpu average by nl 20100713*/
struct ap_cpu_info
{
	unsigned int value;
	struct ap_cpu_info *next;
};

struct ap_snr_info
{
	unsigned char value;
	struct ap_snr_info *next;
};

struct delay_info
{
	unsigned char pkt_loss;
	unsigned char delay_type;
	unsigned short delay_time;
};


//qiuchen copy from v1.3
/*fengwenchao add 20120314 for onlinebug-162*/
struct wifi_snr_info
{
	unsigned int snr_times;
	unsigned char snr_max_value;
	unsigned char snr_min_value;
	unsigned char snr_average;
	double snr_math_average;
	struct ap_snr_info * ap_snr_info_head;
	unsigned int ap_snr_info_length;
};
/*fengwenchao add end*/
//qiuchen copy end

struct Vlan_Group_Member
{
	unsigned char name[VLANGRP_NAME_LEN];
	unsigned int Group_id;
	unsigned int radio_num;
	unsigned int wlan_id;
	unsigned int vlan_id;
	unsigned char service_policy;
	struct vlangrpm *radio_member;
	struct Vlan_Group_Member *next; 
	struct Vlan_Group_Member *list;
	struct Vlan_Group_Member *last;
};


struct ap_cpu_mem_statistics
{
  unsigned int cpu_value[10];
  unsigned int mem_value[10];
  unsigned int cpu_average;
  unsigned int cpu_times;
  unsigned int cpu_peak_value;
  unsigned int mem_average;
  unsigned int mem_times;
  unsigned int mem_peak_value;
  struct ap_cpu_info * ap_cpu_info_head;
  unsigned int ap_cpu_info_length;
  struct ap_cpu_info * ap_mem_info_head;
  unsigned int ap_mem_info_length;
  struct wifi_snr_info wifi_snr[L_RADIO_NUM];/*fengwenchao add 20120314 for onlinebug-162*///qiuchen copy from v1.3
  
  /*nl add for snr 2010-09-08*/
  unsigned int snr_times;		
  unsigned char snr_max_value;
  unsigned char snr_min_value;
  unsigned char snr_average;
  double snr_math_average;
  struct ap_snr_info * ap_snr_info_head;
  unsigned int ap_snr_info_length;
};
typedef struct ap_cpu_mem_statistics ap_cm_statistics;

typedef struct
{
	unsigned char type;
	unsigned char ifindex;
	unsigned char state;/*0-not exist/1-up/2-down/3-error*/
	unsigned int eth_rate;/*10M,100M*/
	unsigned int eth_mtu;   /*fengwenchao add 20110126 for XJDEV-32 from 2.0*/
	time_t state_time;
}if_state_time;

struct ap_if_state_time
{
	unsigned char report_switch;
	unsigned short report_interval;
	unsigned char eth_num;
	unsigned char wifi_num;
	if_state_time eth[AP_ETH_IF_NUM];
	if_state_time wifi[AP_WIFI_IF_NUM];
} ;
typedef struct ap_if_state_time wid_ap_if_state_time;

struct tag_wids_set
{
	unsigned char flooding;
	unsigned char sproof;
	unsigned char weakiv;
	unsigned char reserved; /*reserved*/
} ;
typedef struct tag_wids_set wid_wids_set;

struct tag_wids_statistics
{
	unsigned int floodingcount;
	unsigned int sproofcount;
	unsigned int weakivcount;
} ;

typedef struct tag_wids_statistics wid_wids_statistics;

struct AP_VERSION
{
	char *apmodel;
	char *versionname;
	char *versionpath;
	unsigned char radionum;
	unsigned char bssnum;
	struct AP_VERSION *next;
} ;

//typedef struct AP_VERSION AP_VERSION_ELE;
/*
typedef struct{
	int list_len;
	AP_VERSION_ELE *VERSION;
}AP_VERSION_INFO;
*/
/*
typedef struct{
	int list_len;
	AP_VERSION_ELE **VERSION_LIST;
}AP_VERSION_INFO;
*/

struct tag_wids_device_ele
{
  unsigned char bssid[MAC_LEN]; /*attack device mac*/
  unsigned char vapbssid[MAC_LEN]; /* attack des*/
  unsigned char attacktype;
  unsigned char frametype;

  unsigned int attackcount;

  time_t fst_attack;
  time_t lst_attack;

  unsigned char channel;
  unsigned char rssi;
  unsigned char RogStaAttackStatus; //是否对可疑端站采取措施fengwenchao add 20110415 
  struct tag_wids_device_ele *next;
} ;

struct tag_wids_device_info
{
	int count;
	struct tag_wids_device_ele * wids_device_info;
} ;
typedef struct tag_wids_device_info wid_wids_device;


struct sample_rate_info
{
 unsigned char  time;
 unsigned int	past_uplink_throughput;
 unsigned int	current_uplink_throughput;
 unsigned int	past_downlink_throughput;
 unsigned int	current_downlink_throughput;
 unsigned int	uplink_rate;
 unsigned int	downlink_rate;
};
typedef struct sample_rate_info wid_sample_rate_info;

typedef struct
{
	unsigned char monitorMode;
	unsigned int num;
	unsigned char scanningMode;
	unsigned int channel[SCANNING_CHANNEL_NUM];
} SCNANNING_MODE;

typedef struct
{
	int neighborapInfosCount;
	int DeviceInterference;
	int wtp_online_num;  //fengwenchao add 20110402
 	int checkrogueapcount;
	struct Neighbor_AP_ELE *neighborapInfos;
} Neighbor_AP_INFOS;

typedef struct
{
	Neighbor_AP_INFOS 	 *rouge_ap_list[L_RADIO_NUM];	
	wid_wids_device		 *wids_device_list;
}DCLI_AC_API_GROUP_TWO;


struct allwtp_neighborap
{
	unsigned int wtpid;
	unsigned char WTPmac[MAC_LEN];
	unsigned char radio_num;
	struct allwtp_neighborap_radioinfo *radioinfo_head;
	struct allwtp_neighborap *next;
	struct allwtp_neighborap *allwtp_neighborap_list;
	struct allwtp_neighborap *allwtp_neighborap_last;
};

struct allwtp_neighborap_radioinfo
{
	unsigned char l_radioid;
	unsigned char Radio_Channel;
	unsigned char radiomac[MAC_LEN];
	u_int16_t Radio_TXP;/*TX power*/
	int Radio_Type;
	int rouge_ap_count;              //临近AP的数量
	int failreason;
	struct Neighbor_AP_ELE *neighborapInfos_head;
	struct allwtp_neighborap_radioinfo *next;
	struct allwtp_neighborap_radioinfo *radioinfo_list;
	struct allwtp_neighborap_radioinfo *radioinfo_last;
};

typedef struct
{	
	unsigned char opstate; /*0disable 1 enable*/
	unsigned char flag;
	unsigned short reportinterval;
	unsigned char countermeasures_mode;//0 ap 1 adhoc 2 all
	unsigned char countermeasures_switch;//0 close default 1 open
	unsigned short reserved; //reseved
}APScanningSetting;

struct ap_average_delay_info
{
	unsigned char delay_num;  
	struct delay_info ap_average_delay[TOTAL_AP_DELAY_NUM];
};
typedef struct ap_average_delay_info ap_delay_info;

struct  wlan_stats_info_profile
{
	unsigned char type;  /*0-ath, 1-eth, 2-wifi*/  
	unsigned char ifname[16];
	unsigned char radioId;
	unsigned char wlanId;
	unsigned char mac[6];
	unsigned int rx_packets;
	unsigned int tx_packets;
	unsigned int rx_errors;   
 	unsigned int tx_errors; 
	unsigned int rx_drop;   
 	unsigned int tx_drop; 
	unsigned long long rx_bytes;
	unsigned long long tx_bytes; 
	unsigned int rx_rate;
	unsigned int tx_rate;
	unsigned int ast_rx_crcerr;
	unsigned int ast_rx_badcrypt;
	unsigned int ast_rx_badmic;
	unsigned int ast_rx_phyerr;

	unsigned int rx_frame;
	unsigned int tx_frame;	
	unsigned int rx_error_frame;
	unsigned int tx_error_frame;	
	unsigned int rx_drop_frame;
	unsigned int tx_drop_frame;	
	
	unsigned int rx_band;
	unsigned int tx_band;	
	unsigned long long rx_unicast;  // zhangshu modify rx_unicast 32bit to 64bit  2010-09-13
	unsigned long long tx_unicast;  // zhangshu modify tx_unicast 32bit to 64bit  2010-09-13
	unsigned int rx_multicast;
	unsigned int tx_multicast;
	unsigned int rx_broadcast;
	unsigned int tx_broadcast;
	unsigned int rx_pkt_unicast;
	unsigned int tx_pkt_unicast;
	unsigned int rx_pkt_multicast;
	unsigned int tx_pkt_multicast;
	unsigned int rx_pkt_broadcast;
	unsigned int tx_pkt_broadcast;
	unsigned int rx_pkt_retry;
	unsigned int tx_pkt_retry;
	unsigned int rx_pkt_data;
	unsigned int tx_pkt_data;
	unsigned int rx_retry;
	unsigned int tx_retry;

	unsigned int rx_pkt_mgmt;   // packets received of management          zhangshu modify 2010-09-13
	unsigned int tx_pkt_mgmt;	  // packets transtmitted of management    zhangshu modify 2010-09-13
	unsigned long long rx_mgmt;
	unsigned long long tx_mgmt;	

	unsigned long long rx_sum_bytes;  // zhangshu add,20100913  total number sent by interface
    unsigned long long tx_sum_bytes;  // zhangshu add,20100913  total number received by interface

	unsigned int rx_pkt_control;//zhangshu add for 1.3v,20100913
	unsigned int tx_pkt_control;//zhangshu add for 1.3v,20100913	
	unsigned int rx_errors_frames;   //zhangshu add for error frames, 2010-09-26
	unsigned int is_refuse_lowrssi; //fengwenchao add for chinamobile-177,20111122
	struct  wlan_stats_info_profile *next;
};
typedef struct wlan_stats_info_profile wlan_stats_info;

/*added end*/
typedef struct
{     
	unsigned char radioId;
	unsigned char wlanId;
	unsigned char mac[6];
	unsigned char mode;  /*11b-0x01,11a-0x02,11g-0x04,11n-0x08,*/
	unsigned char channel;
	unsigned char rssi;
	//unsigned short nRate;
	unsigned short tx_Rate;
	unsigned char isPowerSave;
	unsigned char isQos;
	unsigned int rx_bytes;
	unsigned int tx_bytes;

	/* zhangshu add for 1.3v 2010-09-14 */
	unsigned long long rx_data_bytes;        //add 64bit for rx_bytes
    unsigned long long tx_data_bytes;        //add 64bit for tx_bytes
    unsigned int rx_data_frames;             //add data frames from user to ap
    unsigned int tx_data_frames;             //add data frames from ap to user
    unsigned int rx_frames;                  //add total frames from user to ap
    unsigned int tx_frames;                  //add total frames from ap to user
    unsigned int rx_frag_packets;            //add frag packets from user to ap
    unsigned int tx_frag_packets;            //add frag packets from ap to user
    unsigned short rx_Rate;                  //receive rate

	//weichao add
	unsigned char sta_reason;
	unsigned short sub_reason;
	unsigned char ptk_res;
} WIDStationInfo;

typedef struct {
	u_int32_t CMD;
	u_int32_t wlanCMD;
	u_int8_t  radiowlanid[L_RADIO_NUM][WLAN_NUM];
	u_int32_t setCMD;
	u_int32_t radioid[L_RADIO_NUM];
	int staCMD;
	char StaInf[8];
	int keyCMD;
	wAW_StaKey key;
}WID_CMD;

typedef struct {
	unsigned int STAOP;
	unsigned char STAMAC[WID_MAC_LEN];
	unsigned char WLANDomain;
}WID_STA;

typedef struct {
	char*	VlanName;
	u_int32_t	VID;
}WID_VLAN;

struct WID_TUNNEL_WLAN_VLAN {
  char		ifname[ETH_IF_NAME_LEN];	
  struct WID_TUNNEL_WLAN_VLAN  *ifnext;	
};

/*for mib*/
typedef struct {
	unsigned char	wlanid;
	unsigned char	l2_isolation_switch;
}WID_wtp_wlan_l2_isolation;
typedef struct {
	unsigned char	ifindex;
	unsigned char	snr_max;
	unsigned char	snr_min;
	unsigned char	snr_average;
	double 	snr_math_average;
	unsigned char	snr[10];
}WID_wtp_wifi_snr_stats;

typedef struct {
	WID_wtp_wlan_l2_isolation	wlan_l2isolation[L_BSS_NUM];
	unsigned char	dos_def_switch;
	unsigned char	igmp_snoop_switch;
}WID_mib_info;
typedef struct {
	unsigned int state;/*enable or not*/
	unsigned int tx_power;
}WID_oem_sector;
typedef struct {
	unsigned int state;/*enable or not*/
}WID_oem_tx_chainmask;

typedef struct {
	unsigned short supper_g_type;/*enable or not*/
	unsigned int supper_g_state;
}WID_oem_netgear_g;

typedef enum{
	DISABLE = 0,
	WDS_ANY = 1,
	WDS_SOME = 2
}WDS_STAT;

struct wds_bssid{
	unsigned char BSSID[MAC_LEN];
	struct wds_bssid *next;
};
struct wds_rbmac{
	unsigned char mac[MAC_LEN];
	unsigned char key[32];
	struct wds_rbmac *next;
};

/*fengwenchao copy from ht2.0 for requirements-407*/
struct tag_mcsid{
	unsigned int mcsid;
	struct tag_mcsid *next; 
	
};
 struct tag_mcsid_list{

 	 int count;	 
	 struct tag_mcsid * mcsidlist;
} ;

typedef struct tag_mcsid_list update_mcs_list;
/*fengwenchao add end*/
struct wlan_service_control{
	int TimerID;
	int TimerState;
	int wday[7];
	int times;
	int is_once;
};
typedef struct wlan_service_control WID_WSC;


struct vlan_id{
	unsigned short vlanId;
	struct vlan_id *next;
};

struct cpe_apply_wlan_vlan{
	unsigned char wlanid;
	unsigned int vlan_count;
	struct vlan_id *vlanid;
};

/* acl-group */
#define ACL_GROUP_NUM 129
#define ACL_RULE_NUM 1025
#define GROUP_ACL_COUNT 64
#define STANDARD_ACLGRP_RULE 0
#define EXTENDED_ACLGRP_RULE 1

typedef enum
{
	ACLGRP_ACT_DENY = 0,
	ACLGRP_ACT_PERMIT = 1
}ACLGRP_ACTION;

typedef enum
{
	ACLGRP_DIR_INGRESS = 0,
	ACLGRP_DIR_EGRESS = 1
}ACLGRP_DIRECT;

typedef enum
{
	ACLGRP_PROTO_ICMP = 0,
	ACLGRP_PROTO_TCP = 1,
	ACLGRP_PROTO_UDP = 2,
	ACLGRP_PROTO_DOMAIN
}ACLGRP_PROTOCOL;

typedef struct sta_acl
{
	unsigned short rule_index;
	unsigned char rule_type;
	unsigned char rule_action;
	unsigned char rule_proto;
	unsigned char direct;
	unsigned char acl_refcnt;/* ref count when be add in acl group */
	union
	{
		struct
		{
			unsigned int dip;
			unsigned int sip;
			unsigned short dport;
			unsigned short sport;
			unsigned char dmask;
			unsigned char smask;
		}tcpudp;
		struct
		{
			unsigned int dip;
			unsigned int sip;
			unsigned char dmask;
			unsigned char smask;
			unsigned char type;
			unsigned char typemask;
			unsigned char code;
			unsigned char codemask;
		}icmp;
		struct
		{
			unsigned char len;
			unsigned char *dom_str;
		}domain;
	}u;
}STA_ACL;

typedef struct aclwlanlist
{
	unsigned char WlanID;
	struct aclwlanlist *next;
}ACL_WLANlist;

typedef struct aclrulelist
{
	unsigned int rule_id;
	struct aclrulelist *next;
}ACL_RULElist;


typedef struct sta_acl_group
{
	unsigned char groupid;
	pthread_mutex_t AclGrpMutex;
	unsigned char *groupname;
	ACL_WLANlist *WlanList;
	unsigned char rule_cnt;
	ACL_RULElist *rule_list;
}STA_ACLGRP;

/*
typedef enum{
	OP_ADD	= 0,
	OP_DEL	= 1,
	OVERWRITE	= 2,
	CANCEL 	=3
}STA_ACL_OP;
*/
typedef enum{
	ROGUE_ADD = 1,
	ROGUE_DEL = 2,
	ROGUE_MODIFY = 3,
	ROGUE_CANCEL = 4
}ROGUE_AP_OP;
/*
typedef enum{
	LEVEL_AP	= 0,
	LEVEL_RADIO	= 1,
	LEVEL_WLAN	= 2
}op_level;
*/
typedef struct sta_acl_show
{
	unsigned int rule_index;
	unsigned char rule_type;
	unsigned char rule_action;
	unsigned char rule_proto;
	unsigned char direct;
	unsigned int dip;
	unsigned char dmask;
	unsigned int sip;
	unsigned char smask;
	unsigned int dport;
	unsigned int sport;
	unsigned char type;
	unsigned char typemask;
	unsigned char code;
	unsigned char codemask;
	unsigned char *dom_str;
}STA_ACL_SHOW;

typedef struct dcli_acl_show
{
	unsigned int num;
	STA_ACL_SHOW ** AclList;
}DCLI_ACL_LIST;

typedef struct dcli_acl_group
{
	unsigned char num;
	STA_ACLGRP **ACLGROUP;
}DCLI_ACLGRP_SHOW;


/* SPECTRUM ANALYSIS  */
#define SA_MIN_CYCLE			(1)	/* s */
#define SA_MAX_CYCLE			(10)	/* s */
#define SA_DEFAULT_CYCLE		(5)	/* s */

#define SA_MIN_TIME				(1)	/* ms */
#define SA_MAX_TIME				(100)	/* ms */
#define SA_DEFAULT_TIME			(40)	/* ms */

#define SA_MIN_REPORT_INTERVAL		(30)	/* s */
#define SA_MAX_REPORT_INTERVAL		(1800)	/* s */
#define SA_DEFAULT_REPORT_INTERVAL	(30)	/* s */

enum spectrum_type
{
	SPECTRUM_TYPE_VEDIO_BRIDGE = 1,			/* Vedio Bridge */
	SPECTRUM_TYPE_BLUETOOTH_STEREO = 2,		/* Bluetooth Stereo */
	SPECTRUM_TYPE_BLUETOOTH_HEADSET = 3,	/* Bluetooth HeadSet */
	SPECTRUM_TYPE_CORDLESS_PHONE = 4,		/* Cordless Phone */
	SPECTRUM_TYPE_CWA = 5,					/* CWA */
	SPECTRUM_TYPE_MICROWAVE_OVEN = 6,		/* Microwave Oven */
	SPECTRUM_TYPE_WLN = 7,					/* WLN */
	SPECTRUM_TYPE_MAX = 8,					/* max  */
};

struct sa_disturb_info
{
	unsigned short cnt;
	unsigned char rssi;
	unsigned char type;
};

struct sa_channel_info
{
	unsigned short channel;
	
	unsigned int disturb_num;
	struct sa_disturb_info *info;
};

struct sa_res
{
	unsigned int radioid;
	
	unsigned int channel_num;
	struct sa_channel_info *info;
};

struct sa_head
{
	unsigned int radio_num;
	struct sa_res *node;
};


enum spectrum_analysis_state
{
	SPECTRUM_ANALYSIS_STATE_NONE = 0,
	SPECTRUM_ANALYSIS_STATE_RUNNING = 1,
	SPECTRUM_ANALYSIS_STATE_DONE = 2,
	SPECTRUM_ANALYSIS_STATE_TIMEOUT = 3,
};

struct spectrum_analysis
{
	unsigned char enalbe;			/* 1 : enable 0 : disable */	
	unsigned short scan_cycle;		/* 30-600 s */
	unsigned short scan_time;		/* 1-100 ms */	
	unsigned short report_interval;	/* 30-600 s */

	struct sa_res *res;
};
/* 
 * the way radio type calculated
 */
#define mode11b	(1<<0)
#define mode11a	(1<<1)
#define mode11g	(1<<2)
#define mode11n	(1<<3)
#define modepure11an	(1<<4)
#define modepure11gn	(1<<5)
#define mode11ac	(1<<6)
#define mode_11bg	(mode11b | mode11g)
#define mode_11an	(mode11a | mode11n)
#define mode_11gn	(mode11g | mode11n)
#define mode_11bgn	(mode11b | mode11g | mode11n)
#define mode_11a_11an	(mode_11an | modepure11an)
#define mode_11g_11gn	(mode_11gn | modepure11gn)
#define mode_11an_11ac	(mode11ac | mode_11an)
#define mode_11a_11an_11ac	(mode_11an | mode_11a_11an | mode11ac)
	
enum radio_type_e
{
	Radio_none = 0,
	Radio_11b = 1 /* mode11b */,
	Radio_11a = 2 /* mode11a */,
	Radio_11g = 4 /* mode11g */,
	Radio_11bg = 5 /* mode_11bg */,
	Radio_11n = 8 /* mode11n */,
	Radio_11an = 10 /* mode_11an */,
	Radio_11gn = 12 /* mode_11gn */,
	Radio_11bgn = 13 /* mode_11bgn */,
	Radio_11a_11an = 26 /* mode_11a_11an */,
	Radio_11g_11gn = 44 /* mode_11g_11gn */,
	Radio_11ac = 64 /* mode11ac */,
	Radio_11an_11ac = 74 /* mode_11an_11ac */,
	Radio_11a_11an_11ac = 90 /* mode_11a_11an_11ac */,
};

extern char* wai_radio_type_str[];

#define WAI_RADIO_TYPE_STR(t)  \
	((t) <= IEEE80211_11AN)  ?  wai_radio_type_str[t]: \
	(Radio_11a_11an == (t))  ? "a/an": \
	(Radio_11g_11gn == (t))  ? "g/gn": \
	(Radio_11ac == (t))  ? "ac": \
	(Radio_11an_11ac == (t)) ? "an/ac" : \
	(Radio_11a_11an_11ac == (t)) ? "a/an/ac" : "ukn"
#define WAI_RADIO5G_TYPE(t) \
	((Radio_11a &  t) ||(Radio_11ac & t))

#define WAI_RADIO_CWMODE_STR(t)  \
	(0 == (t)) ? "ht20": \
	(1 == (t)) ? "ht20/40": \
	(2 == (t)) ? "ht40": \
	(3 == (t)) ? "vht20" : \
	(4 == (t)) ? "vht40" : \
	(5 == (t)) ? "vht80" : "ukn"


enum sta_type_e
{
	Others = 0,
	iPhone = 1,
	Phone = 2,
	iPad = 3,
	Pad = 4,
	MacBook = 5,
	PC = 6,
	MI = 7,
	Sta_Type_Num = 8,
};

#define WAI_STA_DEVTYPE_STR(t)  \
	(iPhone == (t)) ? "iPhone": \
	(Phone == (t)) ? "Phone": \
	(iPad == (t)) ? "iPad": \
	(Pad == (t)) ? "Pad" : \
	(MacBook == (t)) ? "MacBook" : \
	(PC == (t)) ? "PC" : \
	(MI == (t)) ? "MI" : "Others"
	
#define WAI_STA_PORTALSTATUS_STR(t)  \
    (0 == (t)) ? "unauthorized": \
    (1 == (t)) ? "authorized": \
    (2 == (t)) ? "to be authorized" : "sta"

#define WAI_WLAN_OP_STR(t)  \
	(WLAN_ADD == (t)) ? "WLAN_ADD": \
	(WLAN_DEL == (t)) ? "WLAN_DEL": \
	(WLAN_HIDE_ESSID == (t)) ? "WLAN_HIDE_ESSID": \
	(WLAN_WDS_ENABLE == (t)) ? "WLAN_WDS_ENABLE": \
	(WLAN_WDS_DISABLE == (t)) ? "WLAN_WDS_DISABLE" : \
	(WLAN_CHANGE_TUNNEL == (t)) ? "WLAN_CHANGE_TUNNEL" : "ukn"

/* Maximum number of supported rates (from both Supported Rates and Extended
 * Supported Rates IEs). */
#define WLAN_SUPP_RATES_MAX 32

#define STA_INFO_INADDR_LEN		(16)	/* length of sta_info->in_addr */
#define STA_INFO_NAME_LEN		(64)	/* length of sta_info->sta_name */

struct acl_policy
{
	unsigned int id;
	unsigned int prev_id;
};
typedef struct acl_policy acl_policy_t;

struct d_qos_policy
{
	unsigned int id;
};
typedef struct d_qos_policy qos_policy_t;


struct sta_info {
	struct sta_info *next; /* next entry in sta list */
	struct sta_info *hnext; /* next entry in hash table list */
	u8 sta_name[STA_INFO_NAME_LEN];
	u8 ostype[DEFAULT_LEN];
	u8 pd_model[DEFAULT_LEN];
	u8 hostname[DEFAULT_LEN];
	u8 cpu_model[DEFAULT_LEN];
	u8 addr[MAC_LEN];
	unsigned short roam_type;   /* 0-未漫游 1-漫游离开  2-漫游接入 */
	u32 flags;
	char *in_addr;
	struct in_addr ip_addr;
	unsigned int sta_average_delay;
	unsigned long long rxbytes;
	unsigned long long txbytes;
	unsigned long long pre_rxbytes;
	unsigned long long pre_txbytes;
	u64 rxbytes_old;
	u64 txbytes_old;
	u64 rxdatapackets;		/*ap report*/ 
	u64 txdatapackets;		/*ap report*/ 
	u64 rxpackets;
	u64 txpackets;
	u64 pre_rxpackets;
	u64 pre_txpackets;
	u64 rxframepackets;		/*ap report*/ 
	u64 txframepackets;		/*ap report*/ 
	u64 errpackets;
	u64 retrybytes;
	u64 retrypackets;
	unsigned char rx_retry_per;
	unsigned char tx_retry_per;
	unsigned char pkt_loss_per;
	u16 capability;
	u16 listen_interval; /* or beacon_int for APs */
	u8 supported_rates[WLAN_SUPP_RATES_MAX];
	int supported_rates_len;
	unsigned long long rr;  
	unsigned long long tr;	//receive rate    transmit rate byte
	u64 tp;					//throughput
	u32 snr;
	unsigned int nonerp_set:1;
	unsigned int no_short_slot_time_set:1;
	unsigned int no_short_preamble_set:1;
	unsigned int no_ht_gf_set:1;
	unsigned int no_ht_set:1;
	unsigned int ht_20mhz_set:1;
	int portal_auth_success;

	u16 auth_alg;
	u8 previous_ap[6];

	enum {
		STA_NULLFUNC = 0, STA_DISASSOC, STA_DEAUTH, STA_REMOVE
	} timeout_next;

	/* IEEE 802.1X related data */
	//struct eapol_state_machine *eapol_sm;

	/* IEEE 802.11f (IAPP) related data */
	//struct ieee80211_mgmt *last_assoc_req;

	u32 acct_session_id_hi;
	u32 acct_session_id_lo;
	time_t acct_session_start;
	int acct_session_started;
	int acct_terminate_cause;	/* Acct-Terminate-Cause */
	int acct_interim_interval;	/* Acct-Interim-Interval */

	unsigned long last_rx_bytes;
	unsigned long last_tx_bytes;
	u32 acct_input_gigawords;	/* Acct-Input-Gigawords */
	u32 acct_output_gigawords;	/* Acct-Output-Gigawords */

	u8 *challenge; /* IEEE 802.11 Shared Key Authentication Challenge */
	
	int alive_flag;
	int alive_total;
	unsigned int initiative_leave; /*1-sta initiative_leave 0-others*/
	struct timeval start;
	struct timeval end;
	int vlan_id;
	int vlan_id2;			//to nevis STA vlan tag in wifi
    unsigned int dvlan_id;  //dynamic-vlan id
	unsigned int security_type;

#ifdef ASD_IEEE80211N
	struct ht_cap_ie ht_capabilities; /* IEEE 802.11n capabilities */
#endif /* ASD_IEEE80211N */
	
	time_t *add_time;
	
	unsigned char account_stop_flag;
	unsigned char sta_type;			/* 0-user 1-guest */
	unsigned char dev_type;			/* 设备种别 0）iPhone 1）Phone 2）iPad 3）Pad 3）PC */
	unsigned char portal_type;		/* 0-wifidog */
	unsigned char portal_status;	/* 0-未认证 1-认证通过  2-待认证 */

	/*sta info single intensity etc.*/
	unsigned char mode;				//11b-0x01,11a-0x02,11g-0x04,11n-0x08,
	unsigned char channel;
	unsigned char rssi;
	unsigned int nRate;				/*sta rx rate*/
	unsigned int txRate;
	unsigned char isQos;
	unsigned char info_channel;
	//struct auth_sta_info_t wapi_sta_info;
	unsigned char isPowerSave;
	unsigned char vip_flag;					/*bit0 for up flag,bit1 for down flag*/
	unsigned int sta_traffic_limit;			/*上行*//*指station 上传到AP的带宽限制 */
	unsigned int sta_send_traffic_limit;	/*下行*//*指station 从AP下载的带宽限制 */
	unsigned int BssIndex;
	unsigned int Fstbssindex;
	unsigned int PreBssIndex;
	unsigned char PreBSSID[MAC_LEN];
	unsigned char bssid[MAC_LEN];
	unsigned int PreACIP;
	unsigned int ipaddr;
	unsigned int gifidx;
	char arpifname[16];
    time_t add_time_sysruntime;
    time_t sta_online_time;
	time_t inactive_time;			/* 单位 ms */
    unsigned int vir_ip;
    unsigned int realip;
	acl_policy_t acl;				/* ACL policy ENR-5 */
	qos_policy_t qos;	
    PORTAL_INFO portal_server;
};
typedef struct sta_info WID_STAINFO;


/* Maximum number of AIDs to use for STAs; must be 2007 or lower
 * (802.11 limitation) */
#define MAX_AID_TABLE_SIZE 128

#define STA_HASH_SIZE 256
#define STA_HASH(sta) (sta[5])
#define STA_ROAM_TYPE_NONE		(0)
#define STA_ROAM_TYPE_LEAVE		(1)
#define STA_ROAM_TYPE_ACCESS	(2)

typedef struct {
	u_int32_t	BSSIndex;/*Index of BSSID in AC*/
	u_int32_t	Radio_G_ID;
	u_int8_t	Radio_L_ID;
	u_int8_t	WlanID;
	u_int8_t	State;	
	u_int8_t	HideESSid; /* 1  hide  0 not hode*/
	u_int8_t	keyindex;
	u_int8_t	Wlan_Index;
	unsigned char limit_sta_rssi; /* RDIR-25 */
	unsigned char aliasSetFlag;
	unsigned char aliasESSID[ESSID_LENGTH];
	unsigned char ESSID_CN_STR[ESSID_DEFAULT_LEN];
	char WlanKey[DEFAULT_LEN];
	unsigned int KeyLen;
	int AuthIp;			/*wpa_e auth server ip*/
	int AuthPort;		/*wpa_e auth server port*/
	u_int8_t	*BSSID;
	WID_VLAN	WVLAN;
	char	BSS_IF_NAME[ETH_IF_NAME_LEN];
	u_int8_t	BSS_IF_POLICY;
	u_int8_t	BSS_TUNNEL_POLICY;
	u_int8_t	ath_l2_isolation;
	u_int8_t	cwmmode;/*two states:0---20mode;1---20/40 mode*/
	char	nas_id[NAS_IDENTIFIER_NAME];
	u_int32_t	nas_id_len;
	unsigned int bss_accessed_sta_num;
	unsigned int bss_max_allowed_sta_num;
	WID_STAINFO *sta_list; /* STA info list head */
	WID_STAINFO *sta_hash[STA_HASH_SIZE];
	unsigned int vlanid;	/*bss vlanid,first*/
	unsigned int wlan_vlanid;	/*wlan vlanid,second*/
#if DYNAMIC_VLAN_SUPPORT
	unsigned int dyvlanid;      /* dynamic vlan */
	unsigned int vlangrpid;
#endif
	unsigned char wifi_snr;
	unsigned char wds_mesh;
	u_int8_t	ip_mac_binding;/*disable-0 enable-1*/
	u_int8_t	band_width;
	u_int8_t	traffic_limit_able;/*disable-0 able-1*/
	u_int32_t	traffic_limit;/*bss traffic limit*/
	u_int32_t	average_rate;/*sta average*/
	u_int32_t	send_traffic_limit;/*bss send traffic limit*/
	u_int32_t	send_average_rate;/*sta send average*/
	u_int32_t 	upcount;
	u_int32_t 	downcount;
	BSSStatistics	BSS_pkt_info;
	struct acl_config *acl_conf;
	WDS_STAT	WDSStat;
	struct wds_bssid *wds_bss_list;
	unsigned int wblwm;/*wds_bss_list wds 0,mesh 1*/
	unsigned int vMAC_STATE;
	unsigned int sta_static_arp_policy;
	char arp_ifname[ETH_IF_NAME_LEN];
	char nas_port_id[32];

	unsigned int noResToStaProReqSW;
	unsigned int unicast_sw;
	unsigned int muti_bro_cast_sw;
	unsigned int muti_rate;
	unsigned int wifi_sw;
	unsigned int hotspot_id;
	
	unsigned char multi_user_optimize_switch;
	unsigned char asic_hex;/* 0 asic; 1 hex*/
	char master_to_disable;//qiuchen add it for AXSSZFI-1191
	unsigned char packet_power;
	unsigned char sta_auth_per;
	unsigned char sta_abn_deauth_per;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned int tx_packets;
	unsigned int rx_packets;
	unsigned long long tx_bytes;		/*total tx byte for this ath*/
	unsigned long long rx_bytes;		/*total rx byte for this ath*/
}WID_BSS;

struct asd_stainfo{
	WID_BSS *bss;
	WID_STAINFO	*sta;
};

struct asd_stalist{
	struct asd_stainfo *stainfo;
	struct asd_stalist *next;
};

struct radio{
	u_int32_t	WTPID;
	unsigned char Radio_MAC[MAC_LEN];
	u_int8_t	Radio_Chan;/*Channel*/
	u_int8_t	Radio_L_ID;/*Radio Local ID*/
	u_int32_t	Radio_G_ID;/*Radio Global ID*/
	u_int32_t	Radio_Type;/*a/b/g/n*/
	u_int32_t	Radio_Type_Bank;
	/*u_int16_t   Radio_Rate;//11m/bps or 54m/bps*/
	u_int32_t	Support_Rate_Count;
	struct Support_Rate_List *Radio_Rate;/*sz*/
	int **RadioRate;
	char *excommand;
	u_int32_t 	upcount;
	u_int32_t 	downcount;
	u_int32_t 	rx_data_deadtime;
	u_int32_t	CMD;/*command tag*/
	u_int32_t   bss_num;
	WID_BSS *BSS[L_BSS_NUM];
	int			QOSID;
	int			QOSstate;
	int 		Radio_country_code; /*wcl add for OSDEVTDPB-31*/
	u_int16_t	Radio_TXP;/*TX power*/
	u_int16_t	Radio_TXPOF;/*TX  power offset*/
	unsigned short  txpowerstep;
	u_int16_t   FragThreshold;/*Max fragmation size*/
	u_int16_t	BeaconPeriod;/*Beacon interval*/
	u_int8_t	IsShortPreamble;/*short preamble is 1*/
	u_int8_t	DTIMPeriod;
	
	u_int8_t	ShortRetry;
	u_int8_t	LongRetry;
	u_int16_t	rtsthreshold;
	u_int8_t	AdStat;/*Admin State 2(disable)/1(enable)*/
	u_int8_t	OpStat;/*Operate State 2(disable)/1(enable)*/
	unsigned char chan_per;
    unsigned char disturb_rssi;
    unsigned char wifi_snr;
	u_int8_t	wifi_state;
	u_int8_t	bandwidth;
	
	radio_disturb_info disturbinfo;
	u_int8_t	ishighpower;
	u_int8_t	diversity;		/*disable-0/able-1*/
	u_int8_t	txantenna;		/*auto-0/main-1/vice-2*/
	u_int8_t	txpowerautostate;
	
	u_int8_t    radio_countermeasures_flag; /*AP功率压制标志位,fengwenchao add 20110325*/
	u_int8_t	max_bss_count;
	unsigned char radio_disable_flag;  /*fengwenchao add 20110920 for radio disable config save flag*/
	u_int8_t 	isBinddingWlan;
	u_int8_t	BindingWlanCount;
	u_int16_t	channelchangetime;
	u_int8_t	auto_channel;
	u_int8_t	auto_channel_cont;
	u_int8_t    channelsendtimes;
	
	/*11 n Parameters start*/
	char channel_offset;
	u_int16_t guardinterval;
	u_int16_t mcs;
	u_int16_t cwmode;				//0:ht20 1:ht20/40 2:ht40 3:VHT20 4:VHT40 5:VHT80
	Acktimeout ack;/*wcl add for RDIR-33*/
	AmpduParameter	Ampdu;
	AmsduParameter	Amsdu;  //zhangshu add for a-msdu, 2010-10-09
	MixedGreenfieldParameter	MixedGreenfield;
	unsigned char  chainmask_num;  // 1-3  to indicate the number of radio chainmask
	unsigned char  tx_chainmask_state_value;/*tx chainmask state.such as 0x0011*/
	unsigned char  rx_chainmask_state_value;/*rx chainmask state.such as 0x0011, zhangshu add 2010-10-09 */
	WID_oem_tx_chainmask *tx_chainmask[TX_CHANIMASK_NUM];
	/*11 n Parameters end*/
/*A8 start*/
	WID_oem_sector	*sector[SECTOR_NUM];
	unsigned short	sector_state_value;/*sector state.such as 0x0011.here only sector0 and sector1 is enable*/
	WID_oem_netgear_g supper_g;
	unsigned int REFlag;/*RadioExternFlag*/
	int distance; /*zhanglei add for A8*/
	unsigned int cipherType;/*0 disable  1 wep 2 aes*/
	char wepkey[32];
	int rbmacNum;
	struct wds_rbmac *rbmac_list;
	unsigned char inter_vap_able;
	unsigned char intra_vap_able;
	unsigned int keep_alive_period;
	unsigned int keep_alive_idle_time;
	unsigned char congestion_avoidance;
/*A8 end*/	
	struct wlanid	*Wlan_Id; /*binding wlan id*/
	unsigned char	*WlanId; /*binding wlan id*/
	char br_ifname[WLAN_NUM][IF_NAME_MAX];
	u_int32_t	wep_flag[WTP_WEP_NUM];/*which bss binding a wlan use wep*/
	WID_WSC	StartService;
	WID_WSC	StopService;
	struct radio *next;
	char mcs_list[L_BSS_NUM];/*fengwenchao add 20120314 for requirements-407*/
	int mcs_count;/*fengwenchao add 20120314 for requirements-407*/
	
	struct spectrum_analysis spec_analysis; /*spectrum analysis */
	struct cpe_apply_wlan_vlan cpe_intf[8];
};
typedef struct radio WID_WTP_RADIO;
struct wtp_extend {
	
	unsigned char type;
	unsigned char op;
	unsigned char radioId;
	unsigned char wlanid;/*binding wlan id*/
	unsigned char state;/*switch state*/
	u_int8_t	Radio_L_ID;/*Radio Local ID*/
	//unsigned int reseved;
};
typedef struct wtp_extend WID_WTP_EXTEND;

/* struct of wid trap */
/* zhangshu append 20100824 */
struct wid_trap{
	unsigned int ignore_percent;
	u_int8_t ignore_switch;
	unsigned int sta_addr_redirect;
	unsigned char rogue_terminal_trap_flag;
};

/* zhangshu add for Terminal Disturb Info Report, 2010-10-08 */
typedef struct terminalDisturbInfo{
    unsigned char reportswitch;   // 0-off, 1-on
	unsigned short reportpkt;
	unsigned short sta_trap_count;
} terminal_disturb_info;


/*fengwenchao add 20111117 for GM-3*/
struct heart_time_value_head
{
	unsigned int heart_time_value;
	struct heart_time_value_head *next;
};

struct heart_time
{	
	unsigned char heart_statistics_switch;
	unsigned int heart_statistics_collect;
	unsigned int heart_time_delay;
	unsigned int heart_transfer_pkt;
	unsigned int heart_lose_pkt;
	unsigned int heart_time_value_length;
	unsigned int heart_time_avarge;
	struct heart_time_value_head *heart_time_value_head;
};
#define WTP_IMAGENAME_LEN			(128)
#define WTP_VERSION_LEN				(128)	
#define WTP_MODE_LEN				        (128)	
#define WID_MD5_LEN					(16)
/*fengwenchao add end*/

/* REQUIREMENTS-536/ENR4-2 for 5G-FIRST */
struct _join5g
{
	unsigned char join5g_switch;
	unsigned char discard_count;
	unsigned short scan_time;
	unsigned int aging_time;
	unsigned char sta_count;
	unsigned char signal_strength;
};


#define WID_MAX_VLANID		(4096)
struct wtp_lan_vlan		/* ENR-23 */
{
	unsigned char state;		/* 0 : disable, 1 : enable */
	unsigned short vlanid;
};

enum wtp_upgrade_state	/* cur state */
{
	WTP_UPGRADE_STATE_DONE = 0,		/* init or upgrade finished */
	WTP_UPGRADE_STATE_JOIN,		/* transmit Image Information = {size,hash} */		
	WTP_UPGRADE_STATE_INITMD5,		/* transmit Image Information = {size,hash} */
	WTP_UPGRADE_STATE_TXMD5,		/* transmit md5  */
	WTP_UPGRADE_STATE_IMAGEING,		/* transmit Image  */
	WTP_UPGRADE_STATE_IMAGEEND, 	/* transmit Image end */
};
enum wtp_upgrade_mode
{
	WTP_UPGRADE_MODE_NONE = 0,		/* old upgrade method*/
	WTP_UPGRADE_MODE_CAPWAP = 1,
	WTP_UPGRADE_MODE_FTP = 2,
	WTP_UPGRADE_MODE_TFTP = 3,
	WTP_UPGRADE_MODE_HTTP = 4,
};

#define WTP_IMAGENAME_LEN			(128)
#define WTP_VERSION_LEN				(128)	
#define WTP_MODE_LEN				        (128)	
#define WID_MD5_LEN					(16)

struct wtp_upgrade
{
	enum wtp_upgrade_state state;
	enum wtp_upgrade_mode mode;
	FILE *fp;
	int filesize;	/* image size */
	char version[WTP_VERSION_LEN];		/* EXP : 1.6.2113 */
	char ImageName[WTP_IMAGENAME_LEN];	/* EXP : PC-APv5-v1.6.2113.img */
	//char Mode[WTP_MODE_LEN];	/* EXP : PC-APv5-v1.6.2113.img */
	unsigned char hash[WID_MD5_LEN];
	int pktcnt;	/* send imagedata request cnt */
};

struct wtp
{
	u_int32_t	WTPID;
	char*	WTPIP;
	char* 	WTPSN;				/* WTP Serial No */
	u_int8_t*	WTPMAC;
	char*	WTPNAME;
	char*	WTPModel;			/* model of wtp which determin the number of Radios and rates supported in WTP*/
	char*	APCode;				/* inner develop code */
	u_int8_t	RadioCount;
	u_int8_t	ViaL3IF;		/* interface that Data of WTP come from */
	u_int8_t	WTPStat;		/* wtp state (online(1) /offline(0)) */
	u_int8_t	BindL3IF;		/* force interface that Data of WTP come from */
	u_int32_t	WFR_Index;		/* First Radio Global Index */
	u_int8_t	CTR_ID;			/* CAPWAP Control Tunnel ID */
	u_int8_t	DAT_ID;			/* CAPWAP Data Tunnel ID */
	u_int8_t	Reboot_s;		/* ????? */
	u_int8_t 	isused;
	
	u_int8_t	quitreason;		/*quit reason*/
	u_int8_t	unused_flag;
	u_int8_t	resetflag;		/*dhcp 0;static 1*/
	u_int8_t	tpc_policy;
	u_int8_t 	isBinddingWlan;
	u_int8_t	BindingWlanCount;
	unsigned char apply_wlan_num;
	unsigned char apply_wlanid[WLAN_NUM];/*binding wlan id store apply wlan id*/
	int	tunnel_mode;
	int wtp_login_mode;			/*static 0;dynamic 1*/
	int cap_pro_ver;
	struct wlanid *Wlan_Id;		/*binding wlan id*/
	u_int32_t	BindingSock;	/*special sock binding to this WTP added by weiay 20080530*/
	u_int32_t	BindingSystemIndex;
	char BindingIFName[ETH_IF_NAME_LEN];
	char nas_id[NAS_IDENTIFIER_NAME];
	struct alert_info wtp_alert_info[EVENT_RECORD_SIZRE] ;
	WID_CMD	*CMD;
	Neighbor_AP_INFOS *NeighborAPInfos[L_RADIO_NUM];
	Neighbor_AP_INFOS *rouge_ap_infos[L_RADIO_NUM];
	unsigned int  wtp_allowed_max_sta_num;
	unsigned int  wtp_accessed_sta_num;
	unsigned int  wtp_triger_num;
	unsigned int rx_echocount;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned long long rx_bytes;		/*total rx byte for this ap*/
	unsigned long long tx_bytes;		/*total tx byte for this ap*/
	unsigned long long rx_bytes_before;	/*tmp total rx byte for this ap*/
	unsigned long long tx_bytes_before;	/*tmp total tx byte for this ap*/
	unsigned int wtp_flow_triger;
	unsigned int ap_ipadd;
	unsigned int ap_mask_new;
	unsigned int ap_gateway;
	unsigned int ap_dnsfirst;
	unsigned int ap_dnssecend;
	unsigned int img_now_state;
	char sendsysstart;
	char isipv6addr;
	u_int8_t ap_mask;
	u_int8_t ap_sta_report_switch;
	unsigned char moment_infor_report_switch;
	unsigned char sta_deauth_message_reportswitch;
	unsigned char sta_flow_information_reportswitch;
	unsigned char wifi_extension_reportswitch;
	unsigned short wifi_extension_reportinterval;
	u_int16_t ap_sta_report_interval;
	
	u_int8_t updateStat;		//0 waitupdate//1 update 2 updatesuccess
	u_int8_t updatefailcount;	//0 no access //1 over max count 2 pkt error
	u_int8_t updatefailstate;	//0 waitupdate//1 update 2 update success
	unsigned char radio_num;
	WID_WTP_RADIO *WTP_Radio[L_RADIO_NUM];
	char *sysver;
	char *ver;
	char *ApReportVer;  /*fengwenchao add 20110216 for ap updata successful or fail*/
	char *codever;		/*used to recognize the version of oem production,so we can config the txantenna*/
	char *location;
	char *netid;
	int collect_time;	/*for cpu average computing by nl*/
	wid_wifi_info	wifi_extension_info;
	wid_sample_rate_info	wid_sample_throughput;
	WID_mib_info	mib_info;
	ap_cm_statistics	apcminfo;
	wid_ap_if_state_time	apifinfo;	
	wlan_stats_info apstatsinfo[TOTAL_AP_IF_NUM]; /*ath 4 wifi 2 eth 2 total num 8*/
	ap_delay_info apdelayinfo;
	wid_wids_statistics wids_statist;
	wid_wids_device *wids_device_list;
	time_t	*add_time;
	time_t	*quit_time;
	time_t	imagedata_time;
	time_t	config_update_time;
	time_t	manual_update_time;
	u_int32_t	ElectrifyRegisterCircle;
	/*u_int32_t	wep_flag[WTP_WEP_NUM];*//*which bss binding a wlan use wep*/
	WID_wtp_wifi_snr_stats wtp_wifi_snr_stats;
	struct msgqlist *ControlList;
	struct msgqlist *ControlWait;
	unsigned int rate;
	unsigned int old_bytes;
	char *updateversion;
	char *updatepath;
	char *apply_interface_name;
	char *option60_param;//to ap
	unsigned int RSSI;
	unsigned int wtp_trap_switch;
	unsigned int wtp_seqnum_switch;
	unsigned int wtp_trap_lev;
	unsigned int wtp_cpu_use_threshold;
	unsigned int wtp_mem_use_threshold;
	unsigned int wtp_rogue_ap_threshold;
	unsigned int wtp_rogue_terminal_threshold;
	unsigned char cpu_per;
	unsigned char mem_per;
	unsigned char flash_per;
	u_int8_t	ap_sta_wapi_report_switch;
	unsigned int ap_sta_wapi_report_interval;
	unsigned int trap_collect_time;
	unsigned int cputrap_resend_times;
	unsigned int memtrap_resend_times;
	unsigned int rogueaptrap_resend_times;
	unsigned int rogueteminaltap_resend_times;
	unsigned int cpu_resend_times;
	unsigned int cpu_clear_resend_times;
	unsigned int memtrap_clear_resend_times;
	unsigned int ap_temp_resend_times;
	unsigned int ap_temp_clear_resend_times;
	int EchoTimer;
	struct wtp *next;
	//WID_WTP_EXTEND	WtpExtend;
	int dhcp_snooping;
	int sta_ip_report;
	unsigned char cpuType[WTP_TYPE_DEFAULT_LEN];
	unsigned char flashType[WTP_TYPE_DEFAULT_LEN];
	unsigned char memType[WTP_TYPE_DEFAULT_LEN];
	unsigned int flashSize;
	unsigned int memSize;
	int neighbordeatimes;
	char login_interfaceIP[128];	//xiaodawei transplant from 2.0 for telecom test, 20100301
	unsigned int apstatisticsinterval;
	unsigned int APGroupID;
	char master_to_disable;
  /*zhaoruijia,tranlate  neighbor_channel_interference to 1.3,start*/
	unsigned char neighborchannel_trap_flag;
  	unsigned char channel_device_interference_flag; /*fengwenchao add 20110221 for wid_dbus_trap_wtp_channel_device_interference */
	unsigned char ap_rogue_threshold_flag;          /*fengwenchao add 20110221 for wid_dbus_trap_ap_rogue_threshold */
	unsigned char ac_discovery_danger_ap_flag;      /*fengwenchao add 20110221 for wid_dbus_trap_wtp_ac_discovery_danger_ap*/ 
	unsigned char find_wids_attack_flag;            /*fengwenchao add 20110221 for wid_dbus_trap_wtp_find_wids_attack */
	unsigned char channel_count_minor_flag;         /*fengwenchao add 20110221 for wid_dbus_trap_wtp_channel_count_minor*/
	unsigned char samechannel_trap_flag;
	int neighborchannelrssithold;
	int samechannelrssithold;
  /*zhaoruijia,tranlate  neighbor_channel_interference to 1.3,end*/
    unsigned int ntp_interval;     /*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,start*/
	unsigned char ntp_state;
    unsigned char ntp_trap_flag;  /*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,end*/
	SCNANNING_MODE WIDS;
	struct wid_trap wid_trap;
	int apcodeflag;
	unsigned int elem_num;
	/* zhangshu add for Terminal Disturb Info Report, 2010-10-08 */
	terminal_disturb_info ter_dis_info;
	struct heart_time heart_time;   /*fengwenchao add 20111117 for GM-3*/
	struct _join5g join5g;
	struct wtp_lan_vlan lan_vlan;	/* ENR-23 */
	mac_group_policy	mac_grp_policy;
	u_int32_t	macgroupID;
	u_int8_t	electronic_menu;
	unsigned int four_way_handshake_on_ac:1;
	unsigned int reserved:31;
	struct wtp_upgrade upgrade;
	char *comment;
};
typedef struct wtp WID_WTP;


/*xm add for balence sta*/
struct bss_s{
	unsigned int bss_index;
	unsigned int arrival_num;
	unsigned int assoc_cnt;
	int is_24g;			// 0 : 2.4g    1 : 5.8g
	int rssi;			// Received Signal Strength Indication
	/*unsigned int sta_num;
	unsigned int rd_flow;   xm add 09/02/06*/
	unsigned long timestamp;
	
	struct bss_s *next;
};

typedef struct bss_s bss_arrival_num;


struct log_node_s
{	
	unsigned char mac[6]; 
	unsigned long timestamp;
	bss_arrival_num *from_bss_list;
	unsigned int list_len;
	
	struct log_node_s *next;
	
};
typedef struct log_node_s log_node;

struct PMK_BSSInfo{
	unsigned int BSSIndex;
	struct PMK_BSSInfo *next;
};

struct PMK_STAINFO{
	struct PMK_STAINFO *next;
	struct PMK_STAINFO *hnext;
	unsigned char addr[MAC_LEN];
	unsigned int PreBssIndex;
	unsigned char *BSSIndex;
	struct PMK_BSSInfo *bss;
	unsigned int BssNum;
	unsigned int idhi;
	unsigned int idlo;	
};

struct ROAMING_STAINFO{
	struct ROAMING_STAINFO *next;
	struct ROAMING_STAINFO *hnext;
	unsigned char addr[MAC_LEN];
	unsigned int BssIndex;
	unsigned int PreBssIndex;
	unsigned char BSSID[MAC_LEN];
	unsigned char PreBSSID[MAC_LEN];
	unsigned int need_notice_wifi;
};

struct PreAuth_BSSINFO{
	struct PreAuth_BSSINFO *next;
	struct PreAuth_BSSINFO *hnext;
	unsigned char addr[MAC_LEN];
	unsigned int BSSIndex;
};
#if 0
struct wlan_service_control{
	int TimerID;
	int TimerState;
	int wday[7];
	int times;
	int is_once;
};
typedef struct wlan_service_control WID_WSC;
#endif

struct ip_info{
	struct ip_info * next;
	struct ip_info * hnext;
	unsigned int ip;
	char mac[MAC_LEN];
};

#define VIR_DHCP_HASH_SIZE     (1024)
struct vir_pool{
	struct ip_info *ip_list;
	struct ip_info *ip_hash[VIR_DHCP_HASH_SIZE];
	int ipnum;
	struct ip_info *last;	/* list last node pointer */
};


struct vir_dhcp{
	char ifname[ETH_IF_NAME_LEN];
	char ifmac[MAC_LEN];
	unsigned int low;
	unsigned int high;
	struct vir_pool dhcpfree;
	struct vir_pool dhcplease;
};

struct a_sta_info{
	struct a_sta_info *next;
	struct a_sta_info *hnext;

	unsigned char addr[MAC_LEN];
};

/* WEB BY PASS (wp) */
#define WP_AGING_TIMER	(1800)

/* previous mac auth expired or never auth before, need mac auth  */
#define WP_AUHT_INVALID		(1)
/* previous  mac auth valid, no need mac auth again */
#define WP_AUHT_VALID		(0)
#if 0
struct sta_policy
{
	int id;			/* policy id */
};

typedef struct sta_policy qos_policy_t;
typedef struct sta_policy acl_policy_t;

struct vlan_policy 
{
	unsigned short vlanid;
};
typedef struct vlan_policy vlan_policy_t;

struct policy_group
{
	qos_policy_t qos;
	acl_policy_t acl;
	vlan_policy_t vlan;
};
typedef struct policy_group policy_group_t;
typedef struct policy_group mac_policy_group_t;
typedef struct policy_group web_policy_group_t;
typedef struct policy_group wpa_policy_group_t;
#endif

/* silence period (SP)when kick sta */
#define SP_UNIT					(60)
#define SP_HASH_SIZE			(64)
#define SP_DEFAULT_TIME			(60 * 5)
struct sp_node
{
	unsigned int sp;	/* silence period (SP)when kick sta (cur_time + SP) */
	unsigned int bssindex;
	unsigned char addr[MAC_LEN];

	struct sp_node *hnext;
};
struct sp_head
{
	int init;	/* first init SET */

	unsigned int enable;
	unsigned int sp_time;	/* silence period (uint: seconds)*/
	unsigned int wlanid;
	void *private_data ;	/* private data  */
	
	pthread_mutex_t mutex;

	int node_num;			/* node number of hash buckets */
	struct sp_node *buckets[SP_HASH_SIZE];	
};
/*ENR-17-BEGIN*/
#define CTR_NODE_FLAG_ONLINE		(0x01)
struct ctr_node
{
	unsigned int online_time;	/* total online time */
	unsigned long logon_time;
	unsigned int flag;
	unsigned int bssindex;
	unsigned char addr[MAC_LEN];

	struct ctr_node *hnext;
};
#define CTR_LIST_BUCKET_SIZE				(256)
#define CTR_LIST_DEFAULT_EFFECTIVE_TIME		(60 * 60 * 24)
#define CTR_LIST_DEFAULT_SESSION_TIME		(60 * 20)
/*ENR-17-END*/

struct ctr_list_head
{
	unsigned int enable;
	unsigned int activities_effective_time;
	unsigned int session_time;

	int init;	/* first init SET */
	pthread_mutex_t ctr_mutex;

	long session_started_time;	/* ctr timer start time */
	int ctr_node_num;
	struct ctr_node *ctr_buckets[CTR_LIST_BUCKET_SIZE];	
};

/* WEB BY PASS (wp) */
#define WP_AGING_TIMER	(1800)

/* previous mac auth expired or never auth before, need mac auth  */
#define WP_AUHT_INVALID		(1)
/* previous  mac auth valid, no need mac auth again */
#define WP_AUHT_VALID		(0)
#define	WP_FLAG_MACAUTH			(0x01)
#define	WP_FLAG_WEBAUTH			(0x02)

#define WPUPD_QOS			(0x01)	/* update wp_node qos */
#define WPUPD_VALN			(0x02)	/* update wp_node vlan */
#define WPUPD_ACL			(0x04)	/* update wp_node acl */
#define WPUPD_FLAG			(0x08)	/* update wp_node flag */

struct wp_node					/* WEB BY PASS (wp) */
{
	unsigned int bssindex;
	unsigned char addr[MAC_LEN];

	unsigned int expire_time;	/* logon time + expire time */
	unsigned int flag;
	
//	policy_group_t policy;
	
	struct wp_node *hnext;		/* WEB BY PASS (wp) */
};
#define WP_HASH_SIZE			(1024)	/* WEB BY PASS (wp) */

struct wp_head					/* WEB BY PASS (wp) */
{
	int init;	/* first init SET */
	pthread_mutex_t mutex;		/* TODO */
	
	unsigned int enable;	/* 1 : enable 0 : disable */
	unsigned int expire;	/* mac auth expiry time */
	
	unsigned int wlanid;
	void *private_data ;	/* private data  */

//	mac_policy_group_t default_macauth_policy;
//	web_policy_group_t default_webauth_policy;
//	
	int node_num;			/* node number of hash buckets */
	struct wp_node *buckets[WP_HASH_SIZE];	
};


#define WLAN_STA_HASH_SIZE				(256)	/* wlan->sta_hash bucket size */
#define WLAN_PREAUTH_BSSINFO_HASH_SIZE	(256)	/* wlan->bss_hash bucket size */
#define WLAN_ROAMING_STAINFO_HASH_SIZE	(256)	/* wlan->r_sta_hash bucket size */
 
typedef struct{
	unsigned int enable;   //0:dynamic vlan disable 1:dynamic vlan enable
	unsigned cnt;     //vlan number in the message
	unsigned short *vlan;  //dyanmic vlan number array
	unsigned iterator;  //dynamic iterator 
}d_vlan_wlan;

typedef struct
{
	unsigned int wtpid;
	unsigned char wtpname[WTP_NAME_LEN];
	unsigned int sta_num;
	unsigned int sta_auth_per;
	unsigned int sta_abn_deauth_per;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned int tx_packets;
	unsigned int rx_packets;
	unsigned int sta_type_num[Sta_Type_Num];
	unsigned long long tx_bytes;
	unsigned long long rx_bytes;
}wlan_wtp_wifi_info;

typedef struct
{
	unsigned int num;
	wlan_wtp_wifi_info wtp_wifi[MAX_WTP_NUM+1];
}wid_wlan_wtp_wifi;

typedef struct
{
	unsigned int wlanid;
	unsigned int sta_num;
	unsigned int sta_auth_per;
	unsigned int sta_abn_deauth_per;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned int tx_packets;
	unsigned int rx_packets;
	unsigned long long tx_bytes;
	unsigned long long rx_bytes;
	unsigned int sta_type_num[Sta_Type_Num];
}wtp_wlan_wifi_info;

typedef struct
{
	unsigned int num;
	wtp_wlan_wifi_info wlan_wifi[WLAN_NUM];
}wid_wtp_wlan_wifi;


#define WLAN_NAME_LEN 16

enum ConfigScheme_mode
{
    TEMPLATE_NONE = 0,
	SIMPLE_OFFICE = 1,
	SENIOR_OFFICE = 2,
	SIMPLE_BUSINESS = 3,
	SENIOR_BUSINESS = 4,
	GUEST_MODE = 5,
};

enum forwarding_mode
{
	CW_FWMODE_BRIDGE_UNTAGGED = 0,
	CW_FWMODE_BRIDGE_TAGGED = 1,
	CW_FWMODE_ROUTER = 2,
};

struct portal_conf
{
    unsigned char prover;
    unsigned char protype;
    unsigned char method;       //0:wifidog
    unsigned char secret_key[DEFAULT_LEN];
    unsigned char gateway_id[PATH_LEN];
    unsigned char hostname[DEFAULT_LEN];
    unsigned char path[DEFAULT_LEN];
    unsigned short http_port[DEFAULT_LEN];
};

struct radius_conf
{
    unsigned char ser_ip[IPv6_LEN];
    unsigned char secret_key[DEFAULT_LEN];
    unsigned short ser_port;
};
typedef struct radius_conf RADIUSCONF;

struct wpae_conf
{
    unsigned char host_ip[IPv6_LEN];
    unsigned short eap_reauth_period;
    unsigned short eap_alive_period;
    unsigned short acct_interim_interval;
    RADIUSCONF mradius_conf;
    RADIUSCONF sradius_conf;
};

struct wlan
{
	u_int8_t used;
	char WlanName[WLAN_NAME_LEN];
	u_int8_t WlanID;
	unsigned char ESSID[ESSID_LENGTH];
	unsigned char ESSID_CN_STR[ESSID_LENGTH];
	u_int8_t	chinaEssid;
	u_int8_t	AAW;/*aute add wtp 0(disable)/1(default enable)*/
	u_int32_t	S_WTP_BSS_List[WTP_MAX_MAX_NUM][L_RADIO_NUM];/*Static WTP BSS List*/
	u_int32_t	D_WTPADD_List[WID_DEFAULT_NUM];/*Dynamic WTP ID List*/
	u_int8_t	L3_IF_Index[WID_MAXN_INDEX];
	u_int8_t	Status;/*WLAN Status 1(default disable)/0(enable)*/
	u_int8_t	VidIf_Bitmap;
	u_int16_t   flow_check;
	u_int32_t   no_flow_time;
	u_int32_t   limit_flow;
	WID_VLAN	WVLAN;
	u_int8_t	Wlan_Index;
	unsigned char ifcount;
	struct ifi *Wlan_Ifi;
	WID_WTP * WLAN_WTP;
	u_int32_t	CMD;
	unsigned char SecurityID;
	u_int8_t	HideESSid; /* 1  hide  0 not hode*/
	u_int8_t 	wlan_if_policy;
	char WlanL3IFName[ETH_IF_NAME_LEN];
	int  wpa_group_rekey;
	char WlanKey[DEFAULT_LEN];
	unsigned int KeyLen;
	unsigned int SecurityType;
	unsigned int EncryptionType;
	unsigned char SecurityIndex;
	int AuthIp;								/*wpa_e auth server ip*/
	int AuthPort;							/*wpa_e auth server port*/
	unsigned char OldSecurityIndex;			/*fengwenchao add 20110310 记录绑新的安全策略之前的安全策略*/
	unsigned char NowSecurityIndex_flag;	/*fengwenchao add 20110310 记录当前的安全策略是否为(open||shared)&&(wep),1-->YES,2-->NO*/
	unsigned char OldSecurityIndex_flag;	/*fengwenchao add 20110310 记录先前的安全策略是否为(open||shared)&&(wep),1-->YES,2-->NO*/
	unsigned char asic_hex; 				/* 0 asic; 1 hex*/
	char	AsIp[DEFAULT_LEN];
	unsigned int IpLen;
	char	ASCerPath[DEFAULT_LEN];
	unsigned int ASCerLen;
	char	AECerPath[DEFAULT_LEN];
	unsigned int AECerLen;
	unsigned int vlanid;
	unsigned char wlan_1p_priority;			/*0-7;0 means no priority*/
	struct WID_TUNNEL_WLAN_VLAN *tunnel_wlan_vlan;
	struct wlan *next;

	char	nas_port_id[NAS_PORT_ID_LEN];
	unsigned int wlan_accessed_sta_num;
 	unsigned int wlan_max_allowed_sta_num;
	unsigned int sta_list_len;
	log_node  *sta_from_which_bss_list;
	unsigned int balance_para;
	unsigned int flow_balance_para;
	unsigned char wlan_balance_triger_num;
	unsigned char extern_balance;
	unsigned char flow_compute;
	unsigned char balance_switch;               /*remember init them*/
	unsigned char balance_method;               /*remember init them*/
	unsigned char Roaming_Policy;			    /*Roaming (1 enable /0 disable)*/
	unsigned char isolation_policy;				/* (1 enable /0 disable)*/
	unsigned char multicast_isolation_policy;	/* (1 enable /0 disable)*/
	unsigned char sameportswitch;		        /* (1 enable /0 disable)*/
	unsigned char bridge_ucast_solicit_stat;	/* (1 enable / 0 disable) */
	unsigned char bridge_mcast_solicit_stat;	/* (1 enable / 0 disable) */
	struct acl_config *acl_conf;
	
	struct PMK_STAINFO *sta_list;		
	struct PMK_STAINFO *sta_hash[256];	
	unsigned int num_sta;	
	struct PreAuth_BSSINFO *bss_list;		
	struct PreAuth_BSSINFO *bss_hash[256];	
	unsigned int num_bss;
	unsigned int PreAuth;	
	struct ROAMING_STAINFO *r_sta_list;		
	struct ROAMING_STAINFO *r_sta_hash[256];	
	unsigned int r_num_sta;	
	struct a_sta_info *a_sta_list;
	struct a_sta_info *a_sta_hash[256];
	unsigned int a_num_sta;				//sta accessed num
	WDS_STAT	WDSStat;
	unsigned int wds_mesh;
	unsigned char AC_Roaming_Policy;
	unsigned char group_id;
	unsigned int sta_ip_mac_bind;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned int wlan_send_traffic_limit;	//下行
	unsigned int wlan_traffic_limit;		//上行
	unsigned int wlan_station_average_send_traffic_limit;
	unsigned int wlan_station_average_traffic_limit;
	wid_wifi_info	wifi_extension_info;
	WID_WSC	StartService;
	WID_WSC	StopService;
	unsigned char bkid[16];

	/* zhangshu add for set eap mac ,2010-10-22 */
	unsigned char eap_mac_switch;
	unsigned char *eap_mac;	
	unsigned char eap_mac2[MAC_LEN];
	/*zhanglei add for uplink detect*/	
	unsigned char uplinkState;
	unsigned char uplinkWlanId;
	char *uplink_addr;
	unsigned int wlan_noResToStaProReqSW;
	unsigned int wlan_unicast_sw;
	unsigned int wlan_muti_bro_cast_sw;
	unsigned int wlan_muti_rate;
	unsigned int wlan_wifi_sw;
	unsigned int bss_allow_max_sta_num;
	unsigned char wlan_ath_l2_isolation;
	unsigned int wlan_sta_static_arp_policy;
	char wlan_arp_ifname[ETH_IF_NAME_LEN];
	unsigned char wlan_limit_sta_rssi;
	unsigned int ap_max_inactivity;
	unsigned int  hotspot_id;	
	u_int8_t	WLAN_TUNNEL_POLICY;
	unsigned char multi_user_optimize_switch ;
	
	unsigned char packet_power;
	unsigned char usergrp_id;	/*user-group*/
	unsigned char aclgrp_id;	/*acl-group*/
	
	struct vir_dhcp *wlan_dhcp;
	int wlan_dhcp_state;
	int wlan_tunnel_switch;
	mac_group_policy	mac_grp_policy;	
	u_int32_t	macgroupID;
	u_int8_t	electronic_menu;

	struct ctr_list_head ctr_head; 	/*ENR-17*/
	struct rsn_pmksa_cache *pmksa;	/* STA PMK CACHE */
	struct sp_head sp_head;		/* silence period (SP) when kick sta */
	struct wp_head wp_head;		/* web by pass */
#if WIFI_QOS_SUPPORT
	unsigned char qos_ac_11e_to_1p[MAX_QOS_11E_VAL+1];
	unsigned char qos_ac_dscp_to_1p[MAX_QOS_DSCP_VAL+1];
	unsigned char qos_ac_dscp_to_dscp[MAX_QOS_DSCP_VAL+1];
	unsigned char qos_ap_11e_to_1p[MAX_QOS_11E_VAL+1];
	unsigned char qos_ap_11e_to_dscp[MAX_QOS_11E_VAL+1];
	unsigned char qos_ap_dscp_to_11e[MAX_QOS_DSCP_VAL+1];
	unsigned char qos_ap_1p_to_11e[MAX_QOS_1P_VAL+1];
	unsigned char qos_ap_11e_to_1p_switch;
	unsigned char qos_ap_11e_to_dscp_switch;
	
	int (*wlan_bind_security)(u_int8_t	WlanID);

#endif
#if DYNAMIC_VLAN_SUPPORT
	d_vlan_wlan dvlan;   //dynamic vlan
#endif

    u_int8_t confschemeid;
    u_int8_t forward_mode;
    u_int8_t guest_flag;
    unsigned int gateway_ip;
    unsigned int gateway_mask;
    union{
        struct portal_conf portalconf;
        struct wpae_conf  wpaeconf;
    }Au;  
};
typedef struct wlan WID_WLAN;


struct configscheme
{
    enum ConfigScheme_mode confschemeid;
    enum forwarding_mode forward_mode;
    u_int8_t wlanid;
    u_int8_t roaming_flag;
    u_int8_t balance_flag;
    u_int8_t portal_flag;
    unsigned int gateway_ip;
    unsigned int gatewap_mask;
    union{
        struct portal_conf portalconf;
        struct wpae_conf  wpaeconf;
    }Au;    
};

typedef struct configscheme WID_CONFSCHEME;


struct AUTOAPINFO
{
	unsigned char*		mac;
	unsigned char*		model;
	unsigned char*		realmodel;
	unsigned char*		sn;
	unsigned int		oemoption;
	unsigned int		apcodeflag;
};
typedef struct AUTOAPINFO WIDAUTOAPINFO;


typedef enum
{
	WID_WPA_CIPHER_NONE = 1,
	WID_WPA_CIPHER_WEP40 = 2,
	WID_WPA_CIPHER_WEP104 = 4,
	WID_WPA_CIPHER_WEP128 = 8,
	WID_WPA_CIPHER_TKIP = 16,
	WID_WPA_CIPHER_CCMP = 32,
	WID_WPA_CIPHER_AES_128_CMAC = 64,
	WID_WAPI_CIPHER_SMS4 = 128
}wid_cipher;

typedef enum
{
	WID_WPA_KEY_MGMT_IEEE8021X = 1,
	WID_WPA_KEY_MGMT_PSK = 2,
	WID_WPA_KEY_MGMT_NONE = 4,
	WID_WPA_KEY_MGMT_IEEE8021X_NO_WPA = 8,
	WID_WPA_KEY_MGMT_WPA_NONE = 16,
	WID_WPA_KEY_MGMT_FT_IEEE8021X = 32,
	WID_WPA_KEY_MGMT_FT_PSK = 64,
	WID_WPA_KEY_MGMT_SHARED = 128,	
	WID_WPA2_KEY_MGMT_IEEE8021X = 256,
	WID_WPA2_KEY_MGMT_PSK = 512,
	WID_WPA2_KEY_MGMT_FT_IEEE8021X = 1024,
	WID_WPA2_KEY_MGMT_FT_PSK = 2048,
	WID_WAPI_KEY_MGMT_PSK = 4096, 
	WID_WAPI_KEY_MGMT_CER = 8192
}wid_wpa_key_mgmt;

typedef enum{
	WID_WPA_PROTO_WPA = 1,
	WID_WPA_PROTO_RSN = 2
}wid_wpa_proto;

typedef enum{
	NO_INTERFACE = 0,
	WLAN_INTERFACE = 1,
	BSS_INTERFACE = 2
}wid_if_policy;

#if 0
typedef struct {	
	unsigned char elem_id;
	unsigned char len;
	unsigned char oui[3];
	unsigned char oui_type;
	unsigned char version[2]; /* little endian */
	int proto;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	unsigned int num_pmkid;
	const unsigned char *pmkid;
	int mgmt_group_cipher;
}WPA_IE;
#endif

typedef struct {
	u_int8_t	WlanID;
	u_int8_t	WTPID;
	u_int8_t	Radio_G_ID;
	u_int8_t	BSSIndex;
	u_int8_t	*mac;
}ASD_STA;


typedef struct{
	struct sockaddr_un addr;
	int 	addrlen;
}unixAddr;
typedef struct{
	struct sockaddr_tipc addr;
	int addrlen;
}unixTipc;

struct white_mac{
	unsigned char elem_mac[6];
	struct white_mac *next;
};

typedef struct{
	int imaccount;
	struct white_mac *list_mac;
}white_mac_list;


/*xm add////////////////*/
struct oui_node
{
	unsigned char oui[3];
	struct oui_node *next;
};


typedef struct
{
	unsigned int list_len;
	struct oui_node *oui_list;
}OUI_LIST_S;


struct essid_node
{
	char *essid;
	unsigned int len;
	struct essid_node *next;
};


typedef struct
{
	unsigned int list_len;
	struct essid_node *essid_list;
}ESSID_LIST_S;


struct attack_mac_node
{
	unsigned char mac[MAC_LEN];
	struct attack_mac_node *next;
};


typedef struct
{
	unsigned int  list_len;
	struct attack_mac_node*attack_mac_list;
}ATTACK_MAC_LIST_S;

typedef struct
{	
	ATTACK_MAC_LIST_S	*dcli_attack_mac_list;
	ESSID_LIST_S		*dcli_essid_list;
	OUI_LIST_S		 *dcli_oui_list;
	
}DCLI_AC_API_GROUP_ONE;

/* coverity-CID: 10822,10818,10819, Out-of-bounds access */
/* coverity-CID: 10820,10821 Out-of-bounds read*/
#define H_CHANNEL_LIST_NUM	5
#define L_CHANNEL_LIST_NUM	5


typedef struct
{
	unsigned int WTPID;
	unsigned char flags;
	unsigned char channel;
	unsigned short txpower;	
	unsigned char H_channel_list[H_CHANNEL_LIST_NUM];   
    unsigned char N_channel_list[L_CHANNEL_LIST_NUM];
	unsigned int WTPID_List[4][2];
}WTP_RRM_INFO;

typedef struct
{
	unsigned int wtpid;
	unsigned char neighbor_rssi[4];
	unsigned char txpower;
	unsigned char pre_txpower;
	unsigned char wtp_cnt;
}transmit_power_control;

extern WID_WLAN		*AC_WLAN[WLAN_NUM];
extern WID_WTP		**AC_WTP;
extern WID_WTP_RADIO	**AC_RADIO;
extern WID_BSS		**AC_BSS;
extern WID_WLAN		*ASD_WLAN[WLAN_NUM];
extern WID_WTP		**ASD_WTP;
extern WID_WTP_RADIO	**ASD_RADIO;
extern WID_BSS		**ASD_BSS;
extern WID_ACCESS	*AC_WTP_ACC;
extern Neighbor_AP_INFOS *gRogueAPInfos;/*added by weiay 2008/11/11*/
extern APScanningSetting gapscanset;
extern int wids_judge_policy;
extern white_mac_list *pwhite_mac_list;
extern white_mac_list *pblack_mac_list;
extern struct sta_static_info *STA_STATIC_TABLE[STA_HASH_SIZE];
extern unsigned int	sta_static_num;

extern int TableSock;
extern int TableSend;
extern int DataSend;
extern int DataSock;		//mahz add 2011.10.17
extern int TipcSend;
#ifdef ASD_MULTI_THREAD_MODE
extern int 		LoopSend;
extern unixAddr ASD_LOOP;
#endif

extern unixAddr toWSM;
extern unixAddr toWID;
extern unixAddr toCHILL;
extern unixAddr toDHCP;			//WEICHAO  add 2011.11.11
extern unixTipc toFASTFWD;
extern unsigned char wirelessdata_switch;
extern unsigned char wireddata_switch;
extern unsigned char apstatistics;
extern unsigned int apstatisticsinterval;
extern wid_wids_set gwids;
extern unsigned char gwidsinterval;
extern unsigned char gprobethreshold;
extern unsigned char gotherthreshold;
extern unsigned int glasttimeinblack;
extern update_wtp_list *updatewtplist;
extern update_wtp_list *updatefailwtplist;
extern unsigned int checkwtpcount;
extern int gtrapflag;
extern int gtrap_ap_run_quit_trap_switch;
extern int gtrap_ap_cpu_trap_switch;
extern int gtrap_ap_mem_trap_switch;
extern int gtrap_rrm_change_trap_switch;
extern int gtrap_flash_write_fail_trap_switch;

extern int gtrap_channel_device_ap_switch;/*zhaoruijia,translate  neighbor_channel_interference to 1.3,start*/

extern int gtrap_channel_device_interference_switch;
extern int gtrap_channel_terminal_interference_switch;
extern int gtrap_rogue_ap_threshold_switch;
extern int gtrap_wireless_interface_down_switch;
extern int gtrap_channel_count_minor_switch;
extern int gtrap_channel_change_switch;

extern int gtrap_ap_run_quit_switch;
extern unsigned char updatemaxfailcount;
extern unsigned char aphotreboot;
extern unsigned char gwidspolicy;
extern wid_wids_device *wids_ignore_list;
extern unsigned char gessidfilterflag;
extern unsigned char gmacfilterflag;
extern unsigned char sta_deauth_message_reportswitch;
extern unsigned char sta_flow_information_reportswitch;

extern OUI_LIST_S 			g_oui_list;
extern ESSID_LIST_S 		g_essid_list;
extern ATTACK_MAC_LIST_S 	g_attack_mac_list;  /*xm add */
/*fengwenchao add 20120117 for onlinebug-96*/
extern int gwtpstate_mb;
extern int gwtpstate_mb_timer;
/*fengwenchao add end*/


/*country code area*/
enum country_code_result{
	COUNTRY_CHINA_CN,/* 0*/
	COUNTRY_EUROPE_EU,/* 1*/
	COUNTRY_USA_US,/* 2*/
	COUNTRY_JAPAN_JP,/* 3*/
	COUNTRY_FRANCE_FR,/* 4*/
	COUNTRY_SPAIN_ES,/* 5*/
	COUNTRY_CODE_SUCCESS,
	COUNTRY_CODE_NO_CHANGE,
	COUNTRY_CODE_ERROR_SMALL_LETTERS,
	COUNTRY_CODE_ERROR
};
/**/
/* 5G Hz channel for China */
#define WAI_RADIO5G_CHANN_CN(c) \
			((149 == c) ||(153 == c) ||(157 == c) ||(161 == c)|| (165 ==c))
#define WAI_RADIO5G_DEFAULT_CHANN_CN	149
#define WAI_RADIO2G_DEFAULT_CHANN_CN	1

/* 5G Hz channel for Europe */
#define WAI_RADIO5G_CHANN_EU(c) \
			((36 == c) ||(40 == c) ||(44 == c) ||(48 == c)|| (52 ==c) || \
			 (56 == c) ||(60 == c) ||(64 == c) ||(100 == c)|| (104 ==c) || \
			 (108 == c) ||(112 == c) ||(116 == c) ||(120 == c)|| (124 ==c) || \
			 (128 == c) ||(132 == c) ||(136 == c) ||(140 == c)) 
#define WAI_RADIO5G_DEFAULT_CHANN_EU	36
#define WAI_RADIO2G_DEFAULT_CHANN_EU	WAI_RADIO2G_DEFAULT_CHANN_CN

/* 5G Hz channel for USA */
#define WAI_RADIO5G_CHANN_US(c) \
			((36 == c) ||(40 == c) ||(44 == c) ||(48 == c)|| (52 ==c) || \
			 (56 == c) ||(60 == c) ||(64 == c) ||(100 == c)|| (104 ==c) || \
			 (108 == c) ||(112 == c) ||(116 == c) ||(120 == c)|| (124 ==c) || \
			 (128 == c) ||(132 == c) ||(136 == c) ||(140 == c) || (149 ==c) || \
			 (153 == c) ||(157 == c) ||(161 == c) ||(165 == c)) 
#define WAI_RADIO5G_DEFAULT_CHANN_US	WAI_RADIO5G_DEFAULT_CHANN_EU
#define WAI_RADIO2G_DEFAULT_CHANN_US	WAI_RADIO2G_DEFAULT_CHANN_CN

/* 5G Hz channel for Japan */
#define WAI_RADIO5G_CHANN_JP(c) \
			((36 == c) ||(40 == c) ||(44 == c) ||(48 == c)|| (52 ==c) || \
			 (56 == c) ||(60 == c) ||(64 == c) ||(100 == c)|| (104 ==c) || \
			 (108 == c) ||(112 == c) ||(116 == c) ||(120 == c)|| (124 ==c) || \
			 (128 == c) ||(132 == c) ||(136 == c) ||(140 == c) || (184 ==c) || \
			 (188 == c) ||(192 == c) ||(196 == c)) 
#define WAI_RADIO5G_DEFAULT_CHANN_JP	WAI_RADIO5G_DEFAULT_CHANN_EU
#define WAI_RADIO2G_DEFAULT_CHANN_JP	WAI_RADIO2G_DEFAULT_CHANN_CN

/* 5G Hz channel for France */
#define WAI_RADIO5G_CHANN_FR(c)  WAI_RADIO5G_CHANN_EU(c)
#define WAI_RADIO5G_DEFAULT_CHANN_FR	WAI_RADIO5G_DEFAULT_CHANN_EU
#define WAI_RADIO2G_DEFAULT_CHANN_FR	WAI_RADIO2G_DEFAULT_CHANN_CN

/* 5G Hz channel for Spain */
#define WAI_RADIO5G_CHANN_SPAIN(c)	WAI_RADIO5G_CHANN_EU(c)
#define WAI_RADIO5G_DEFAULT_CHANN_SPAIN	WAI_RADIO5G_DEFAULT_CHANN_EU
#define WAI_RADIO2G_DEFAULT_CHANN_SPAIN	WAI_RADIO2G_DEFAULT_CHANN_CN

#define WAI_RADIO5G_DEFAULT_CHANN_INVALID	0
#define WAI_RADIO2G_DEFAULT_CHANN_INVALID	0

/*Get default channel via country-code */
extern int wai_radio5g_ch_default_arr[] ;
extern int wai_radio2g_ch_default_arr[];
#define WAI_RADIO5G_CH_DEFAULT(c) \
	((c <= COUNTRY_SPAIN_ES) ? wai_radio5g_ch_default_arr[c] : WAI_RADIO5G_DEFAULT_CHANN_INVALID)
#define WAI_RADIO2G_CH_DEFAULT(c) \
	((c <= COUNTRY_SPAIN_ES) ? wai_radio2g_ch_default_arr[c] : WAI_RADIO2G_DEFAULT_CHANN_INVALID)

enum channel_cwmode{
	CHANNEL_CWMODE_SUCCESS,/* 0*/
	CHANNEL_CWMODE_HT40,/* 1*/
	CHANNEL_CWMODE_HT20,
	CHANNEL_CWMODE_ERROR,
	CHANNEL_CROSS_THE_BORDER      /*fengwenchao add 20110323  信道越界*/
};
enum dcli_sn{
	FIRST = 1,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH,
	SIXTH,
	SEVENTH,
	EIGHTH,
	NINTH,
	TENTH,
	ELEVENTH,
	TWELFTH,
	THIRTEENTH,
	FOURTEENTH,
	FIFTEENTH
};

/*qos area*/
#define WID_QOS_ARITHMETIC_NAME_LEN 20


struct qos
{
	u_int8_t	QueueDepth;
	u_int16_t	CWMin;
	u_int16_t	CWMax;
	u_int8_t	AIFS;
	u_int16_t	TXOPlimit;
	u_int8_t	Dot1PTag;
	u_int8_t	DSCPTag;
	u_int8_t	ACK;
	u_int8_t	mapstate;
	u_int8_t	wmm_map_dot1p;
	u_int8_t	dot1p_map_wmm_num;
	u_int8_t	dot1p_map_wmm[8];
	u_int8_t	qos_average_rate;
	u_int8_t	qos_max_degree;
	u_int8_t	qos_policy_pri;
	u_int8_t	qos_res_shove_pri;
	u_int8_t	qos_res_grab_pri;
	u_int8_t	qos_max_parallel;
	u_int8_t	qos_bandwidth;
	u_int8_t	qos_bandwidth_scale;
	u_int8_t	qos_use_wred;
	u_int8_t	qos_use_traffic_shaping;
	u_int8_t	qos_use_flow_eq_queue;
	u_int8_t	qos_flow_average_rate;
	u_int8_t	qos_flow_max_degree;
	u_int8_t	qos_flow_max_queuedepth;
};
typedef struct qos qos_profile;

struct wid_qos
{
	unsigned int	QosID;
	char		*name;
	qos_profile	*radio_qos[4];
	qos_profile *client_qos[4];
	unsigned char	qos_total_bandwidth;
	unsigned char	qos_res_scale;
	unsigned char	qos_share_bandwidth;
	unsigned char	qos_res_share_scale;
	char			qos_manage_arithmetic[WID_QOS_ARITHMETIC_NAME_LEN];
	char			qos_res_grab_arithmetic[WID_QOS_ARITHMETIC_NAME_LEN];
	char			qos_res_shove_arithmetic[WID_QOS_ARITHMETIC_NAME_LEN];
	unsigned char	qos_use_res_grab;
	unsigned char	qos_use_res_shove;
	struct wid_qos *next;
};
typedef struct wid_qos AC_QOS;
typedef struct
{
	unsigned int  qos_num;
	AC_QOS **qos;
}DCLI_WQOS;

#define WID_QOS_CWMIN_DEFAULT	15
#define WID_QOS_CWMAX_DEFAULT	15
#define WID_QOS_AIFS_DEFAULT	15

extern AC_QOS *WID_QOS[QOS_NUM];

enum wid_qos_type
{
	WID_BESTEFFORT = 0,
	WID_BACKGTOUND,
	WID_VIDEO,
	WID_VOICE
};
typedef enum
{
	unkonwn_type = 0,
	averagerate_type,
	max_burstiness_type,
	manage_priority_type,
	shove_priority_type,
	grab_priority_type,
	max_parallel_type,
	bandwidth_type,
	bandwidth_percentage_type,
	flowqueuelenth_type,
	flowaveragerate_type,
	flowmaxburstiness_type
}flow_parameter_type;

typedef enum
{
	qos_unkonwn_type = 0,
	totalbandwidth_type,
	resourcescale_type,
	sharebandwidth_type,
	resourcesharescale_type
}qos_parameter_type;

struct model_info
{
 char	*model;
 unsigned short	ap_eth_num;
 unsigned short	ap_wifi_num;
 unsigned short	ap_11a_antenna_gain;
 unsigned short	ap_11bg_antenna_gain;
 unsigned int	ap_if_mtu;
 unsigned int	ap_if_rate;
 char	*hw_version;
 char	*sw_name;
 char	*sw_version;
 char	*sw_supplier;
 char	*supplier;
};
typedef struct model_info model_infomation;

struct model_code_info
{
	char	*code;
	unsigned char	cpu_type;
	unsigned char	mem_type;
	unsigned char	ap_eth_num;
	unsigned char	ap_wifi_num;
	unsigned char	ap_antenna_gain;
	unsigned char	support_mode[L_RADIO_NUM];
	unsigned int	ap_if_mtu;
	unsigned int	ap_if_rate;
	unsigned int	card_capacity;
	unsigned int	flash_capacity;
	char	*hw_version;
	char	*sw_name;
	char	*sw_version;
	char	*sw_supplier;
	char	*supplier;
};
typedef struct model_code_info wid_code_infomation;

typedef struct
{
	unsigned char state; 				/*0-disable, 1-num balance, 2-flow balance*/
	unsigned int num_balance_para;	/*default : 1*/
	unsigned int flow_balance_para;	/*default : 1 (Mbps)*/
}ac_balance_flag;

struct sample_info
{
	unsigned char	monitor_time;
	unsigned char	sample_time;
	unsigned char	monitor_switch;
	unsigned char	sample_switch;
};
typedef struct sample_info wid_sample_info;
/*ethereal bridge area*/
struct WID_EBR_IF
{
	char	*ifname;	
	struct WID_EBR_IF  *ifnext;	
};
typedef struct WID_EBR_IF EBR_IF_LIST;

struct wid_ebr
{
	unsigned int	EBRID;
	char			*name;
	unsigned char	state;						/* (1 enable /0 disable)*/
	unsigned char	isolation_policy;			/* (1 enable /0 disable)*/
	unsigned char	multicast_isolation_policy;	/* (1 enable /0 disable)*/
	unsigned char	sameportswitch;		/* (1 enable /0 disable)*/
	unsigned char	bridge_ucast_solicit_stat;	/* (1 enable/ 0 disable) */		/* Huang Leilei add 2012-11-13 */
	unsigned char	bridge_mcast_solicit_stat;	/* (1 enable/ 0 disable) */		/* Huang Leilei add 2012-11-13 */
	unsigned char  multicast_fdb_learn;		/* (1 enable /0 disable)*/
	unsigned int	r_num; 
	unsigned int	eth_num;
	EBR_IF_LIST	*iflist;
	EBR_IF_LIST	*uplinklist;
	struct wid_ebr *next;
};
typedef struct wid_ebr ETHEREAL_BRIDGE;

extern ETHEREAL_BRIDGE *WID_EBR[EBR_NUM];
/*auto ap area*/
struct auto_ap_if
{
	unsigned char	wlannum;
	unsigned char	wlanid[L_BSS_NUM];
	unsigned int ifindex;
	char		ifname[ETH_IF_NAME_LEN];  
	struct auto_ap_if  *ifnext;	
};
typedef struct auto_ap_if wid_auto_ap_if;

typedef struct
{
    unsigned char	auto_ap_switch;
    unsigned char	save_switch;
    unsigned char	ifnum;
    unsigned int	list_len;
    char *ifname;
    wid_auto_ap_if	*auto_ap_if;
}wid_auto_ap_info;

struct wid_ac_ip
{
	char *ip;
	unsigned int wtpcount;
	unsigned int threshold;
	unsigned char priority;
	struct wid_ac_ip *next;
};

/*fengwenchao add 20120323*/
typedef struct
{
	unsigned int policy;
	char arp_ifname[ETH_IF_NAME_LEN];
}sta_static_arp;

typedef struct
{
	unsigned int unicast_policy;
	unsigned int multicast_broadcast_policy;
	unsigned int wifi_policy;
	unsigned int rate;
}ap_uni_muti_bro_cast;
/*fengwenchao add end*/

typedef struct
{
	unsigned char GroupID;
	unsigned char load_banlance; //added by weianying 2010/0323
	unsigned int diff_count;
	int isock;
	unsigned char *ifname;
	unsigned char *ipaddr;
	unsigned int ipnum;
	struct wid_ac_ip *ip_list;
}wid_ac_ip_group;

typedef struct
{
	unsigned int count;
	wlan_stats_info *ap_statics_ele;
}Ap_statics_INFOS;

typedef struct
{
	char * WTPIP;
	unsigned long wtpip;
	Ap_statics_INFOS *ap_statics_list;
	unsigned char txpowr;
	unsigned char rssi[4];

	unsigned int floodingcount;
	unsigned int sproofcount;
	unsigned int weakivcount;

	unsigned int ap_mask_new;
	unsigned int ap_gateway;
	unsigned int ap_dnsfirst;
	unsigned int ap_dnssecend;

}DCLI_AC_API_GROUP_THREE;
/*
struct bak_sock{
	int sock;
	unsigned int ip;
	struct bak_sock *next;
};*/
typedef struct
{
	int list_len;
	struct bak_sock *b_sock_node;
}Bak_Sock_INFO;

struct CWConfigVersionInfo_dcli
{
	char *str_ap_model; //for oem change
	char *str_ap_version_name;
	char *str_ap_version_path;
	unsigned char radio_num;
	unsigned char bss_num;
	CWBool_DCLI ischanged;
	CWBool_DCLI ismodelchanged;
	struct radio_info_type_dcli radio_info[4];
	char *str_ap_code;// for model match
	char *str_oem_version;
	int	apcodeflag;
	struct CWConfigVersionInfo_dcli *next;
};

typedef struct CWConfigVersionInfo_dcli CWConfigVersionInfo_dcli;

typedef struct
{
	int list_len;
	CWConfigVersionInfo_dcli *config_ver_node;
}Config_Ver_Info;

typedef struct
{
	wid_code_infomation *code_info;
	model_infomation *model_info;
	Config_Ver_Info *config_ver_info;

	Bak_Sock_INFO *bak_sock;	
}DCLI_AC_API_GROUP_FOUR;

typedef struct
{
	int result;
	int hw_version;
	int sw_version;
	char *hw_version_char;
	char *sw_version_char;
	char *ac_name;
	int sta_count;
	int max_wtp;
	int static_wtp;
	int force_mtu;
	int log_switch;
	int log__level;
	char loglevel[100];	
//	char clog_swith[2][4];/* init it : = {"OFF","ON"},when use it*/
	int log_size;
	unsigned char uclev3_protocol;
//	char caauth_security[2][20];/* init it : = {"CW_PRESHARED","CW_X509_CERTIFICATE"},when use it*/
	unsigned char auth_security;
//	char calev3_protocol[2][8];/* init it :  = {"CW_IPv6","CW_IPv4"},when use it*/
	int trapflag;
	unsigned char apstaticstate;
	unsigned char macfiltrflag;
	unsigned char essidfiltrflag;
	int g_ac_all_extention_information_switch;
	unsigned int apstatisticsinterval;
	/*xiaodawei add, 20110115*/
	unsigned char radioresmgmt;
	int tranpowerctrl;
	unsigned char tranpwrctrlscope;
	unsigned char autoaploginswitch;
	unsigned char autoaplogin_saveconfigswitch;
	unsigned char wirelessctrlmonitor;
	unsigned char wirelessctrlsample;
	unsigned char widwatchdog;
	unsigned char ac_balance_method;
	unsigned char ap_hotreboot;
	unsigned int ap_acc_through_nat;
	unsigned char wtp_wids_policy;
	unsigned char radio_src_mgmt_countermeasures;
	unsigned char radio_src_mgmt_countermeasures_mode;
	int wireless_interface_vmac;
	unsigned char wtp_link_detect;
	unsigned int wsm_switch;
	unsigned int service_tftp;
	unsigned int service_ftp;
	/*xiaodawei add for trap switch, 20110115*/
	int ap_run_quit;
	int ap_cpu_threshold;
	int ap_mem_threshold;
	int ap_update_fail;
	int rrm_change;
	int rogue_ap_threshold;
	int rogue_terminal_threshold;
	int rogue_device;
	int wireless_interface_down;
	int channel_count_minor;
	int channel_change;
	int rogue_ap;
	int country_code; /*wcl add*/
	/*end of trap switch*/
}wireless_config;

typedef struct
{
	int state;
	unsigned char scope;
	unsigned char th1;
	unsigned char th2;
	unsigned char constant;
	unsigned char max;
}txpower_control;

typedef struct
{
	int flag,vrid,master_uplinkip,master_downlinkip,bak_uplinkip,bak_downlinkip,vir_uplinkip,vir_downlinkip,global_ht_ip,global_ht_opposite_ip;
	char *vir_uplinkname;
	char *vir_downlinkname;
	char *global_ht_ifname;
}wid_vrrp_state;

typedef struct
{
	int num;
	/*ap  threadhold*/
	unsigned int cpu ;
	unsigned int memoryuse ;
	unsigned int temperature ;

	unsigned char rrm_state;
	unsigned int d_channel_state;
	unsigned short report_interval;
	unsigned char flag;

	int neighbordead_interval;
	unsigned int timer;

	/*balance  configuration*/
	unsigned int state;
	unsigned int number;
	unsigned int flow;
	
	wireless_config *wireless_control;
	wid_auto_ap_info *auto_login;
	txpower_control *tx_control;
	wid_sample_info *sample_info;
	wid_vrrp_state * wid_vrrp;
	unsigned char countermeasures_mode;
	unsigned char countermeasures_switch;
}DCLI_AC_API_GROUP_FIVE;

typedef struct
{
	int list_len;
	WID_WTP *WTP_LIST;
}WID_WTP_INFO;

typedef struct
{
	unsigned int num ;
	unsigned int TotalNum;

	unsigned int join_num;
	unsigned int configure_num;
	unsigned int datacheck_num;
	unsigned int run_num;
	unsigned int quit_num;
	unsigned int imagedata_num;
	unsigned int bak_run_num;
	unsigned int wtp_model_type;      /*fengwenchao add 20110307 WTP MODEL 的种类*/
	unsigned int wtp_version_type;    /*fengwenchao add 20110314 WTP VERSION 的种类*/

	WID_WTP_INFO *WTP_INFO;

	struct WTP_MODEL_VERSION **WTP_M_V;    /*fengwenchao add 20110314*/
	WID_WTP **WTP;
	struct AP_VERSION **AP_VERSION;
}DCLI_WTP_API_GROUP_ONE;

typedef struct
{
	wid_sample_rate_info *sample_info;
	WID_BSS **bssstatistics;
	WID_WTP **WTP;
}DCLI_WTP_API_GROUP_TWO;

struct dcli_wtp_api
{
	int echotimer;
	int checktimer;
	unsigned int ElectrifyRegisterCircle;
	time_t addtime;
	time_t quittime;
	time_t starttime;
	time_t imagadata_time;
	time_t config_update_time;
	char *wtp_location;
	char *netid;
	unsigned int wlan_num;
	WID_WLAN **WLAN;
	
	wid_wids_set wids;	
	unsigned char interval;
	unsigned char probethreshold;	
	unsigned char otherthreshold;	
	unsigned int lasttime;

	unsigned int txpower;	
	char *model;
	
	char *versionname;
	char *versionpath;
	char *apcode;
	unsigned char Count_onetimeupdt;
	struct dcli_wtp_api *next;

	int old_ap_img_state;
	unsigned int checktimes;
};
typedef struct dcli_wtp_api	DCLI_WTP_API_GROUP_THREE;

typedef struct
{
	unsigned int radio_num;
	unsigned char bss_num;
	int bss_num_int;
	unsigned int wlan_num;
	unsigned int qos_num;
	unsigned short interval;
	WID_WTP_RADIO **RADIO;
	WID_WTP **WTP;
	WID_BSS **BSS;
}DCLI_RADIO_API_GROUP_ONE;

typedef struct
{
	unsigned char wlan_num;
	unsigned int enable_num;
	WID_WLAN **WLAN;
}DCLI_WLAN_API_GROUP;

typedef struct
{
//	unsigned char ebr_num;
	unsigned int ebr_num;
	ETHEREAL_BRIDGE **EBR;
}DCLI_EBR_API_GROUP;

typedef struct
{
	unsigned int ip_list_num;
	wid_ac_ip_group **AC_IP_LIST;
}DCLI_AC_IP_LIST_API_GROUP;

enum dcli_method
{	
	WID_DBUS_QOS_METHOD_SHOW_QOS_LIST_CASE=1,
	WID_DBUS_QOS_METHOD_SHOW_QOS_CASE,
	WID_DBUS_QOS_METHOD_SHOW_QOS_EXTENSION_INFO_CASE,
	WID_DBUS_QOS_METHOD_SHOW_RADIO_QOS_INFO_CASE
};

typedef struct
{
	unsigned int dcli_ap_run_quit_trap_switch;
	unsigned int dcli_ap_cpu_trap_switch;
	unsigned int dcli_ap_mem_trap_switch;
	unsigned int dcli_flash_write_fail_trap_switch;
	unsigned int dcli_rrm_change_trap_switch;
	unsigned int dcli_rogue_ap_threshold_switch;
	unsigned int dcli_channel_terminal_interference_switch;
	unsigned int dcli_channel_device_interference_switch;
	unsigned int dcli_wireless_interface_down_switch;
	unsigned int dcli_channel_count_minor_switch;
	unsigned int dcli_channel_change_switch;
	unsigned int dcli_rogue_ap_switch;
}WID_TRAP_SWITCH;

typedef struct
{
	unsigned int cpu;
	unsigned int memoryuse;
	unsigned int rogue_ap_threshold;
	unsigned int rogue_termi_threshold;
	unsigned int wtpid;
	unsigned int trap_switch;
	unsigned int collecttime;
	int samechannelrssi_theshold;
    int neighborchannelrssi_theshold;
}WID_TRAP_THRESHOLD;

struct WTP_GROUP_MEMBER
{	
	struct WTP_GROUP_MEMBER *next;
	struct WTP_GROUP_MEMBER *hnext;
	unsigned int WTPID;
};

typedef struct
{
	unsigned int GID;
	unsigned char *GNAME;
	unsigned int WTP_COUNT;
	struct WTP_GROUP_MEMBER *WTP_M;
	struct WTP_GROUP_MEMBER *WTP_HASH[256];	
	WID_WTP	WTP_CONFIG;
}WID_WTP_GROUP;
extern WID_WTP_GROUP *WTP_GROUP[WTP_GROUP_NUM];

typedef struct
{
	unsigned char grpid;
	unsigned char grpnmame[NAME_LEN];
	unsigned char used;
	unsigned char apply_wlan_num;
	unsigned char apply_wlanid[WLAN_NUM];/*binding wlan id store apply wlan id*/
	unsigned int send_traffic_limit;
	unsigned int traffic_limit;
}WID_USER_GROUP;
extern WID_USER_GROUP *USER_GROUP[USER_GROUP_NUM];
extern WID_USER_GROUP SOFT_USERGRP[USER_GROUP_NUM];

struct FLASHDISCONN_STAINFO
{
	struct FLASHDISCONN_STAINFO *next;
	struct FLASHDISCONN_STAINFO *hnext;
	unsigned char addr[MAC_LEN];
	unsigned int wtpid;
	unsigned int bssindex;
	unsigned char wlanid;
	unsigned int ipv4Address;
	unsigned long long  rxbytes;
	unsigned long long  txbytes;
	unsigned long long  rxpackets;
	unsigned long long  txpackets;
};

typedef struct{
	struct FLASHDISCONN_STAINFO *fd_sta_list;		
	struct FLASHDISCONN_STAINFO *fd_sta_hash[256];	
	unsigned int fd_num_sta;	
}asd_sta_flash_disconn;

//fengwenchao add 20101222 for  show wtp config of all wtp

struct WLAN_INFO
{
	char *wlanname;
	unsigned char Wlanid;
	unsigned char bifnum;                               //wlan绑定的接口数量
	
	unsigned char wlanBindSecurity;           			//wlan绑定的安全策略的ID
	unsigned char wlanHideEssid;              			//是否隐藏ESSID 1--yes 0--no
	unsigned char wlanServiceEnable;          			//WLAN Status 1(default disable)/0(enable)
	unsigned int wlanMaxConnectUsr;          			//wlan最大接入用户数
	unsigned char wlanLoadBalanceStatus;      			//负载均衡状态
	unsigned char wlanLoadBalanceStatusBaseOnFlow;    	//wlan基于流量的负载均衡
	unsigned char wlanLoadBalanceStatusBaseOnUsr;       //wlan基于用户的负载均衡
	unsigned int wlanLoadBalanceTrafficDiffThreshhd;    //流量差阈值
	unsigned int wlanLoadBalanceUsersDiffThreshhd;      //用户差阈值
	unsigned int  wlanStaOnlineNum;                     //wlan在线用户数
	 float wlanUsrWirelessResoUseRate;           //用户侧（无线侧）可用资源利用率  wlanStaOnlineNum占wlanMaxConnectUsr的百分比
	unsigned int wlanBindSecType;                       //wlan绑定的安全策略的安全类型
	unsigned int wlanBindEncryType;                     //wlan绑定的安全策略的加密类型
	unsigned char wlanLoadBalanceFunction;               //balance switch 开关
	unsigned int indorpPkts;                        //SSID总入丢包数
	unsigned int intotlePkts;                       //SSID总入包数
	unsigned int outdorpPkts;                       //SSID总出丢包数
	unsigned int outtotlePkts;                      //SSID总出包数
	unsigned int SSIDDownBandWidthRate;                 //SSID下行带宽利用率
	unsigned int SSIDUpBandWidthRate;                   //SSID上行带宽利用率

	/*fengwenchao add 20110401 for dot11WlanDataPktsTable*/
	/*-----------------------------------------------*/
	unsigned int ethernetRecvCorrectFrames;        //网络侧（有线侧）MAC层接收到的正确数据帧数目
	unsigned long long ethernetRecvCorrectBytes;   //网络侧（有线侧）MAC层接收到的数据正确数据包字节数
	unsigned long long ethernetSendCorrectBytes;   //网络侧（有线侧）MAC层接发送出去的数据正确数据包字节数
	/*-----------------------------------------------*/
	
	struct WLAN_INFO *wlan_info_list;
	struct WLAN_INFO *wlan_info_last;
	struct WLAN_INFO *next;
	struct ifi *ifi_head;
};

struct WTP_CONFIG_INFORMATION
{
	struct WTP_CONFIG_INFORMATION *wtp_config_list;
	struct WTP_CONFIG_INFORMATION *wtp_config_last;
	struct WTP_CONFIG_INFORMATION *next;
	struct WLAN_INFO *wlan_info_head;

	int WTPID;
	char *wtpBindPort;   //绑定的端口
	unsigned char apply_wlan_num;   //绑定的wlan数量
	unsigned char *wtpMacAddr;

	unsigned char wtpused;   //wtp的状态  1--used; 0--unused
	unsigned int wtpMaxStaNum;  //wtp允许的最大sta num
	unsigned int wtpLoadBalanceTrigerBaseUsr;  //基于用户的负载平衡 wtp number triger  <1-64>
	unsigned int wtpLoadBalanceTrigerBaseFlow;  //基于流量的负载平衡 wtp flow triger  <0-1024>
};

//fengwenchao add 20101222 for  show wtp config of all wtp end

//fengwenchao add 20110307 for REQUIREMENTS-144 
struct WTP_MODEL_VERSION
{
	char* wtp_model;
	unsigned int wtp_model_num;
	char* wtp_version;
	unsigned int wtp_version_num;
	//int wtpid_group[2048];
	//int wtpid_flag;
};

//fengwenchao add end


struct res_node
{
	union
	{
		unsigned int wtpid;
		unsigned int radioid;
		wlan_t wlanid;
	}u;
	unsigned int res;	/* result */
};
struct res_head
{
	unsigned int num;
	struct res_node *node;
};

struct ap_update_config
{
	unsigned int ipaddr;    /*FTP/TFTP :server ip*/
	char user[64];
	char passwd[128];
	unsigned char encrypt_type;
};

struct Listenning_IF{
	char ifname[ETH_IF_NAME_LEN];
	unsigned int ipaddr;
	struct Listenning_IF *if_next;
	LISTEN_FLAG lic_flag;
};


typedef struct{
    int count;
	int a;
	struct Listenning_IF *interface;
}Listen_IF; 

typedef struct _tagNEIGHBOR_LIST
{
	unsigned char mac[MAC_LEN];			//neighbor's mac
	unsigned char channel;				//neighbor's channel
	char neighbor_flag;					//neighbor's flag >=0:local AC, <0:other AC
	char rssi;							//neighbor's RSSI

	struct _tagNEIGHBOR_LIST *next;
}NEIGHBOR_LIST_OBJ, *NEIGHBOR_LIST_HANDLE;

typedef struct _tagCLIENT_INFO
{
	int fd;								//AP's socket描述符
	int active;							//socket状态
	char name[32];						//AP's essid
	struct sockaddr_in addr;			//AP's ip address 
	unsigned char type;					//2.4G 0, 5G, 1
	unsigned char channel;
	unsigned char ap_mac[MAC_LEN];		//client's MAC
	NEIGHBOR_LIST_HANDLE neighbor_link_head;	//neighbors list
	int neighbor_cnt;					//neighbors' count
}CLIENT_INFO_OBJ, *CLIENT_INFO_HANDLE;


/* AP EXTERNTION COMMAND MACRO */
#define AP_EXT_CMD_NOTIFY_STA_PORTAL_AUTH "autelan tunnel_ctl ath.%d-%d setniflag %02X:%02X:%02X:%02X:%02X:%02X %d %u %u %u %u"
#define AP_EXT_CMD_SET_AP_MAX_STA_NUM_V1 "echo %d > /proc/sys/net/ath.%d-%d/maxaid"
#define AP_EXT_CMD_SET_AP_MAX_STA_NUM_V2 "ifconfig ath.%d-%d down;iwpriv ath.%d-%d maxsta %d;echo %d > /proc/sys/net/ath.%d-%d/maxaid;ifconfig ath.%d-%d up"
#define AP_EXT_CMD_SET_RADIO_WLAN_PKTPWR "iwpriv ath.%d-%d PktPowEnable %d"


/* REQUIREMENTS-536/ENR4-2 for 5G-FIRST */
#define WTP_JOIN5G_SWITCH_DISABLE        (0)
#define WTP_JOIN5G_SWITCH_ENABLE         (1)
#define WTP_JOIN5G_AGING_TIME_DEF        (10000)
#define WTP_JOIN5G_DISCARD_COUNT_DEF     (6)
#define WTP_JOIN5G_SCAN_TIME_DEF         (200)
#define WTP_JOIN5G_STA_COUNT_DEF         (30)
#define WTP_JOIN5G_SIGNAL_STRENGTH_DEF   (45)

#endif/*_WID_DEFINE_H*/
