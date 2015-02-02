#ifndef _WID_ASD_WSM_H
#include <sys/types.h>
#include <netinet/in.h>

#define	_WID_ASD_WSM_H

typedef unsigned char wlan_t;		/* WLAN ID type */
typedef unsigned char security_t;	/* security ID type */
/* dcli asd dbus max timeout : 5000 ms */
#define DCLI_ASD_DBUS_TIMEOUT		(5000)
/* dcli wid dbus max timeout : 5000 ms */
#define DCLI_WID_DBUS_TIMEOUT		(10000)

/* 127.0.0.0 */
#define WAW_LOOPBACK(x)	(((x) & htonl(0xff000000)) == htonl(0x7f000000))
/* 224.x.x.x - 239.255.255.255 */
#define WAW_MULTICAST(x)	(((x) & htonl(0xf0000000)) == htonl(0xe0000000))
/* 240.x.x.x */
#define WAW_BADCLASS(x)	(((x) & htonl(0xf0000000)) == htonl(0xf0000000))
/* 0.x.x.x */
#define WAW_ZERONET(x)	(((x) & htonl(0xff000000)) == htonl(0x00000000))
/* 169.254.x.x */
#define WAW_PRIVATE_NET(x)	(((x) & htonl(0xFFFF0000)) == htonl(0xA9FE0000))

/* global variables macro*/
#define WTP_MAX_STA_NUM 128
#define DEFAULT_WTP_MAX_STA_NUM 128
#define WLAN_MAX_STA_NUM 128
#define WLAN_SEND_TRAFFIC_LIMIT 0
#define WLAN_RECEIVE_TRAFFIC_LIMIT 0

#define AP_SCAN_COUNTERMEASURES_PTK  128

/* global variables macro*/
#define IPADDR_INVALID(ipaddr)	\
		(WAW_LOOPBACK(htonl((ipaddr)))	\
		|| WAW_MULTICAST(htonl((ipaddr)))	\
		|| WAW_BADCLASS(htonl((ipaddr))) \
		|| WAW_ZERONET(htonl((ipaddr))) \
		|| WAW_PRIVATE_NET(htonl((ipaddr))))

#define	CLI_FREE_OBJECT(obj_name) {if (obj_name) { free((obj_name)); (obj_name) = NULL;}}


#define _GROUP_POLICY 0  //fengwenchao add 
#define IPv4_LEN	4
#define IPv6_LEN	16
#define IPv4_STR_LEN	16
#define IPv6_STR_LEN	64
#define MAC_LEN	6
#define OUI_LEN 3
#define STA_USERNAME_LEN	64
#define PMK_LEN 32
#define PMKID_LEN 16
#define SHARED_KEY_LEN 256
#define VLANGRP_NAME_LEN	32
#define DEFAULT_LEN	256
#define ACIfaces_MAX_NUM 16
#define NAME_LEN	20
#define IF_NAME_MAX 16
#define NAS_PORT_ID_LEN	 32

#define CONFSCHEME_NUM		6
#define MAX_WTP_NUM		16
#define WLAN_NUM		17
#define L_RADIO_NUM		4
#define L_BSS_NUM		32	/*ht change 8 to 32,10.03.18*/
#define dot3_Max_Len	1518
#define dot11_Max_Len		2336	
#define port_max_num	2049
#define vlan_max_num	4095
#define VLAN_MAX_NUM    4095
#define QOS_NUM		16
#define TOTAL_AP_IF_NUM		16
#define TOTAL_AP_DELAY_NUM		16
#define EBR_NUM		1024
#define WTP_DEFAULT_NUM		128
#define WTP_DEFAULT_NUM_AUTELAN		3
#define WTP_DEFAULT_NUM_OEM		1
#define WTP_MAX_MAX_NUM		4096
#define THREAD_NUM	16
#define SOCK_NUM 16 /*wuwl add */
#define SOCK_BUFSIZE (8*1024*1024)
#define ACIPLIST_NUM		129
#define MIXIPLEN   16
#define ESSID_LENGTH	(32*3)
#define WTP_NAME_LEN	32
#define PATH_LEN 64
#define WID_BAK_AC_PORT		19528
#define WID_LIC_AC_PORT		19628  /*license update*/
#define ASD_BAK_AC_PORT		29527
#define G_AC_NUM	16
#define GROUP_NUM	4
#define STA_HASH_SIZE 256
#define WTP_GROUP_NUM		16
#define EVENT_RECORD_SIZRE	16
#define AP_ETH_IF_NUM		2
#define AP_WIFI_IF_NUM		2
#define AP_ATH_IF_NUM		16
#define WTP_TYPE_DEFAULT_LEN 32
#define ETH_IF_NAME_LEN 16/*Added by weiay 20080710*/
#define WTP_NETID_LEN 33 //zhangshu add
#define WID_WSM_ERROR_HANDLE_STATE_DEFAULT 1 //default enable
/* #define ASD_MULTI_THREAD_MODE */
#define ESSID_DEFAULT_LEN 32
#define ESSID_UTF8_DEFAULT_LEN (ESSID_DEFAULT_LEN*3)
#define USER_KEY_LEN 128
/*fengwenchao copy from 1318 for AXSSZFI-839*/
#define IFF_BINDING_FLAG  0x2
#define SIOCGIFUDFFLAGS	0x893d		/* get udf_flags			*/
#define SIOCSIFUDFFLAGS	0x893e		/* set udf_flags			*/
/*fengwenchao copy end*/
#define SCANNING_CHANNEL_NUM 20
#define SYN_WID_SERVER_TYPE		19858
#define SYN_ASD_SERVER_TYPE		29857
#define SYN_SERVER_BASE_INST	128
#define HASH_SIZE		4096
#define HOTSPOT_ID 4096
#define NAS_IDENTIFIER_NAME 128

#define DYNAMIC_VLAN_SUPPORT 1
#define WIFI_QOS_SUPPORT 1

#define MAX_QOS_11E_VAL 7
#define MAX_QOS_1P_VAL 7
#define MAX_QOS_DSCP_VAL 63
#define D_QoS_MAX		(128)
#define D_VLAN_MAX (1024)

#define VLANGRP_NAME_LEN	     32
#define VLAN_GROUP_NUM         1024
#define VLAN_GROUP_MAX_MEMBER  2048

extern int WTP_NUM;		
extern int gMaxWTPs;
extern int WTP_NUM_AUTELAN;		
extern int WTP_NUM_OEM;		
extern int G_RADIO_NUM;
extern int BSS_NUM;
extern int glicensecount ;
//extern struct ConflictWtp *gConflictWtp;
extern char first_conflict;
extern	u_int8_t g_WLAN_TUNNEL_POLICY;


typedef struct alert_info
{
	int active_id;
	int archived;
} alert_info_s;

typedef struct
{
    unsigned char arEther[MAC_LEN];
}MACADDR;

enum {
	VRRP_REG_IF,
	VRRP_UNREG_IF
};

typedef enum {
	NO_ROAMING = 0,
	ROAMING_AC = 1,
	RAW_AC = 2
}roam_type;

struct bak_sock{
	int sock;
	unsigned int ip;
	struct bak_sock *next;
};

typedef enum {
	NO_IF = 0,
	WLAN_IF = 1,
	BSS_IF = 2
}wAW_IF_Type;

typedef struct {
	unsigned int cipher;
	unsigned int BSSIndex;
	unsigned int WTPID;		
	unsigned int key_len;
	int wpa_group_rekey;
	char key_idx;
	char StaAddr[MAC_LEN];
	char key[DEFAULT_LEN];
	unsigned char SecurityIndex;
	unsigned char OldSecurityIndex;			/*fengwenchao add 20110309 ��¼������ȫ����֮ǰ�İ�ȫ����*/
	unsigned char OldSecurityIndex_flag;	/*fengwenchao add 20110310 ��¼��ǰ�İ�ȫ�����Ƿ�Ϊ(open||shared)&&(wep),1-->YES,2-->NO*/	
}wAW_StaKey;

typedef struct
{
	unsigned int wtpid;
	unsigned int g_radio;
	unsigned int l_radio;
	unsigned int bssindex;
	unsigned char wlanid;
	unsigned char pmkid_len;
	unsigned char roamflag;  /*1-roam 0 non roam*/
	unsigned char StaAddr[MAC_LEN];		// STA MAC
	unsigned char bssid[MAC_LEN]; 
	unsigned char pmk[PMK_LEN];
	unsigned char pmkid[PMKID_LEN];
}wASD_PMK;

typedef struct{
	unsigned char WlanID;	
	unsigned char SecurityID;
	unsigned int SecurityType;
	unsigned int EncryptionType;
	char WlanName[DEFAULT_LEN];
	char ESSID[DEFAULT_LEN];
	unsigned char WlanState;
	wAW_StaKey WlanKey;
	unsigned int   wlan_max_sta_num;
	unsigned int   balance_para;
	unsigned int   flow_balance_para;
	unsigned char  wlan_balance_triger_num;
	unsigned char  balance_switch;
	unsigned char  balance_method;
	unsigned char  Roaming_policy;
	char  	as_ip[DEFAULT_LEN];
	unsigned int as_ip_len;
	char  	cert_path[DEFAULT_LEN];
	unsigned int cert_path_len;
	
	char  	ae_cert_path[DEFAULT_LEN];
	unsigned int ae_cert_path_len;
	
	unsigned int ap_max_inactivity; 	   //weichao add
	unsigned char ascii_hex;
	unsigned int PreAuth;
	//weichao add 
	u_int16_t   flow_check;
	u_int32_t   no_flow_time;
	u_int32_t   limit_flow;
	unsigned int sp_time;		/* silence period*/	

	struct ctr_list				 /*ENR-17*/
	{
		unsigned int enable;
		int activities_effective_time;
		int session_time;
	}ctr; 						
}wASD_WLAN;	/*WID update WLAN information to ASD*/
struct mixwtpip{
	unsigned short addr_family;	
	unsigned short port;
	union {
		unsigned char ipv6_addr[MIXIPLEN];
		unsigned int ipv4_addr;
	} u;
#define m_v4addr u.ipv4_addr
#define m_v6addr u.ipv6_addr
};
typedef struct{
	//unsigned int   C_WtpID;	
	unsigned int   N_WtpID;	
	unsigned int   cur_sta_num;
	unsigned char 	WTPMAC[MAC_LEN];
}NEIGHBOR_WTP;	/*neighbor ap sta info*/

struct ap_ath_info
{
	unsigned char	radioid;
	unsigned char	wlanid;
	unsigned char	ath_updown_times;
} ;
typedef struct ap_ath_info wid_ap_ath_info;


struct wifi_info
{
	unsigned char reportswitch;
	unsigned short reportinterval;
	unsigned int cpu;
	int collect_time;/*for cpu average computing by nl*/
	unsigned int cpu_collect_average; /*for cpu average computing by nl*/
	unsigned char moment_infor_report_switch;
	unsigned int mem_collect_average;
	unsigned char temperature;
	unsigned int tx_mgmt;
	unsigned int rx_mgmt;
	unsigned int tx_packets;
	unsigned int rx_packets;
	unsigned int tx_errors;
	unsigned int rx_errors;
	unsigned int tx_retry;  
	unsigned int rx_retry;
	unsigned long long tx_bytes;
	unsigned long long rx_bytes;
	unsigned char ipmode;/*static--0,dhcp--1*/
	unsigned short memoryall;
	unsigned char memoryuse;
	unsigned short flashall;
	unsigned int flashempty;
	unsigned char wifi_snr;
	unsigned char eth_count;
	unsigned char eth_updown_time[AP_ETH_IF_NUM];
	unsigned char ath_count;
	unsigned char ath_updown_time[AP_ATH_IF_NUM];
	wid_ap_ath_info	ath_if_info[AP_ATH_IF_NUM];
	unsigned char wifi_count;
	unsigned char wifi_state[AP_WIFI_IF_NUM];/*0-not exist,1-up,2-down,3-error*/
	unsigned char cpu_trap_flag;
	unsigned char mem_trap_flag;
	unsigned char temp_trap_flag;
	unsigned char wifi_trap_flag[AP_WIFI_IF_NUM];
	unsigned int tx_unicast;
	unsigned int tx_broadcast;
	unsigned int tx_multicast;
	unsigned int tx_drop;
	unsigned int rx_unicast;
	unsigned int rx_broadcast;
	unsigned int rx_multicast;
	unsigned int rx_drop;
	unsigned int wpi_replay_error;
	unsigned int wpi_decryptable_error;
	unsigned int wpi_mic_error;	
	unsigned int disassoc_unnormal;		/*ϵͳ���������ն��쳣�Ͽ����ӵ��ܴ���*/
	unsigned int rx_assoc_norate;		/*���ն˲�֧�ֻ������ʼ�Ҫ����������ʶ�����ʧ�ܵ��ܴ���*/
	unsigned int rx_assoc_capmismatch;	/*�ɲ���802.11��׼�ƶ���Χ�ڵ�ԭ�������ʧ�ܵ��ܴ���*/
	unsigned int assoc_invaild;			/*δ֪ԭ������¹���ʧ�ܵ��ܴ���*/
	unsigned int reassoc_deny;			/*����֮ǰ�Ĺ����޷�ʶ����ת�ƶ��������¹���ʧ�ܵ��ܴ���*/
	//qiuchen copy from v1.3 f
	/*fengwenchao add 20120314 for onlinebug-162*/
	unsigned char wifi_snr_new[L_RADIO_NUM];
	unsigned short wifi_noise_new[L_RADIO_NUM];
	/*fengwenchao add end*/
	//qiuchen copy end
} ;
typedef struct wifi_info wid_wifi_info;

typedef struct{
	unsigned int WtpID;	
	unsigned int   wtp_max_sta_num;
	unsigned int   wtp_triger_num;
	unsigned int   wtp_flow_triger;
	NEIGHBOR_WTP   N_WTP;
	unsigned char	state;
	unsigned int RSSI;
	unsigned char 	WTPMAC[MAC_LEN];
	struct mixwtpip	WTPIP;
	char 			WTPSN[128];/*WTP Serial No*/
	char            WTPNAME[256];
	char            NETID[WTP_NETID_LEN]; //zhangshu modify, modify netid from point to array,2010-10-26
	wid_wifi_info	wifi_extension_info;
	char 			BindingIFName[ETH_IF_NAME_LEN];
	
}wASD_WTP;	/*WID update WLAN information to ASD*/


typedef struct
{
	unsigned char ifaceIndex;
	unsigned char ifaceIP[IPv4_LEN];
	unsigned char ifacePort;
	unsigned int iWTP_NUM;
}W_ifaces;	/*information of interface of WTP DataChannel*/

typedef struct
{
	unsigned int WTPID;
	unsigned char WTPMAC[MAC_LEN];
	char *WTPModel;
	struct mixwtpip WTPIP;
	unsigned char ACIfaceNum;
	W_ifaces ACIfaces[ACIfaces_MAX_NUM]; 
}wWSM_DataChannel;	/*WID update WTP information to WSM*/

typedef struct
{	
	unsigned int	BSSIndex;
	unsigned int	Radio_G_ID;
	unsigned int rx_pkt_data;
	unsigned int tx_pkt_data;
	unsigned long long rx_data_bytes;	//xiaodawei add rx data bytes for ASD BSSData, 20110224
	unsigned long long tx_data_bytes;	//xiaodawei add tx data bytes for ASD BSSData, 20110224
}BSSData;	

typedef struct
{	
	unsigned int	BSSIndex;
	unsigned char	Radio_L_ID;
	unsigned int	Radio_G_ID;
	unsigned char	WlanID;
	unsigned char	BSSID[MAC_LEN];
	unsigned int    bss_max_sta_num;
	unsigned int	nas_id_len;
	unsigned int	protect_type;
	wAW_IF_Type	bss_ifaces_type;
	wAW_IF_Type	wlan_ifaces_type;
	BSSData BssData[L_BSS_NUM];
	char	nas_id[128];
	unsigned int sta_static_arp_policy;
	char arp_ifname[IF_NAME_MAX];
	char br_ifname[IF_NAME_MAX];
	unsigned int	vlanid;//zhanglei add for portal M_nasid
	char nas_port_id[11];			//mahz add 2011.5.26
	unsigned char accept_mac[50][MAC_LEN];
	unsigned char deny_mac[50][MAC_LEN];
	unsigned int accept_mac_num ;
	unsigned int deny_mac_num;
	unsigned int   macaddr_acl;
	unsigned int hotspot_id;
	char master_to_disable;//qiuchen add it for AXSSZFI-1191
	unsigned char aliasSetFlag;
	char aliasESSID[ESSID_LENGTH];
	unsigned int four_way_handshake_on_ac:1;
	unsigned int reserved:31;	
}wAW_BSS;	/*WID update BSS information to ASD and WSM*/


typedef struct
{	
	unsigned int	tx_bytes;		/*trans bytes,include tx and rx*/
	unsigned int	old_tx_bytes; 	/*old trans bytes,include tx and rx*/
	unsigned int 	trans_rates; 	/*trans rates,kbps*/
	/*add new members in future.*/

}FLOW_IE;  /*xm  09/02/06*/


typedef struct
{	
	unsigned int    wtpid;
	FLOW_IE	        radio_flow[L_RADIO_NUM];

}wASD_RADIO_FLOW;	/*WID update RADIO flow information to ASD*/


/********************************
  * add for REQUIREMENTS-524
  * Internal  portal server IP	and mac 
  * external portal server	is 0
  *******************************/
typedef struct
{
	unsigned int portal_ip;
	unsigned char portal_mac[MAC_LEN];

}PORTAL_INFO;

typedef enum
{
	AUTH_FRAME = 1 ,
	AC_KICK = 2,
	AP_KICK = 3,
	AP_STA_DEAUTH = 4,
	AP_STA_DISASSOC = 5,
	WDS_CHANGE =6,
	WTPD_REBOOT = 7
}STA_LEAVE_REASON;

typedef enum
{
	AUTH_TO_ONE_WLAN = 1,
	AUTH_TO_WLAN = 2
}STA_SUB_RESON1;

typedef enum
{
	SEND_DEAUTH = 1,
	SEND_DISASSOC = 2,
	SEND_BROADCAST_DEAUTH = 3,
	VAP_DISABLE_DELETE =4
}STA_SUB_RESON2;

typedef enum
{
	STA_SIGNAL_WEAK = 1,
	RETRANSMIT_TOO_MORE =2,
	IDLE_TIME_OUT =3
}STA_SUB_RESON3;

typedef struct
{
	unsigned char StaState;
	unsigned char RoamingTag;
	unsigned char STAMAC[MAC_LEN];
	unsigned char RBSSID[MAC_LEN];
	unsigned int BSSIndex;
	unsigned int WTPID;
	unsigned int preBSSIndex;
	unsigned int count;			/*used in sta info report*/
	/*ADD IES*/
	unsigned char radioId;
	unsigned char wlanId;
	unsigned char mode;  /*11b-0x01,11a-0x02,11g-0x04,11n-0x08,*/
	unsigned char channel;
	unsigned char rssi;
	unsigned short tx_Rate;            //zhangshu modify 10-09-14
	unsigned char isPowerSave;
	unsigned char isQos;
	unsigned int rx_bytes;
	unsigned int tx_bytes;
	unsigned int traffic_limit;		/*ht add 091014*/
	unsigned int send_traffic_limit;
	unsigned int vlan_id;
	/* Inter-AC roaming member */
	roam_type ac_roam_tag;
	struct mixwtpip ac_ip;
	unsigned char length;/*mark ipv4 or ipv6*/
	unsigned int ipv4Address;
	char	arpifname[16];

	/* zhangshu append 2010-09-14 */
	unsigned long long rx_data_bytes;		//�¼�64bit��ֵ���������rx_bytes
	unsigned long long tx_data_bytes;		//�¼�64bit��ֵ���������tx_bytes
	unsigned int rx_data_frames;			//�¼�    ��AP�յ��û�������֡����
	unsigned int tx_data_frames;			//�¼�    ��AP�����û�������֡����
	unsigned int rx_frames;					//�¼�   ��AP�յ��û�����֡����
	unsigned int tx_frames;					//�¼�   ��AP�����û�����֡����
	unsigned int rx_frag_packets;			//�¼�   ��AP�յ��û��ı���Ƭ�İ�����
	unsigned int tx_frag_packets;			//�¼�   ��AP�����û��ı���Ƭ�İ���)
	unsigned short rx_Rate;					//receive rate
    
	unsigned int vrrid;
	unsigned int local;
	struct in6_addr ipv6Address;
	unsigned int delay;
	unsigned char sta_reason;
	unsigned short   sub_reason;
	unsigned int authorize_failed;
	unsigned int sta_num;	//sta has authorized
	unsigned char ptk_res;
	PORTAL_INFO portal_info;
}aWSM_STA;	/*ASD update STA information to WSM*/

typedef struct {
	unsigned char StaState;
	unsigned char STAMAC[MAC_LEN];
	unsigned int PORTID;
	unsigned int VLANID;
}WIRED_STA;	/*ASD update STA information to NPD*/

typedef struct {
	unsigned int PORTID;
	unsigned int vlan_num;
	unsigned int VLANID[vlan_max_num];
}WIRED_PORT;	/*NPD update port information to ASD*/

typedef struct {		
	unsigned int VLANID;
	unsigned int port_num;
	unsigned int PORTID[port_max_num];
}WIRED_VLAN;	/*NPD update vlan information to ASD*/

typedef struct {		
	unsigned int WTPID;
	unsigned int BufLen;
	unsigned char CMDBuf[80];
}wASD_CMD;	

typedef enum{
	WID_ADD = 0,
	WID_DEL = 1,
	WID_MODIFY = 2,
	STA_INFO = 3,
	RADIO_INFO = 4,  /* flow infomation  xm add */
	WTP_DENEY_STA = 5,
	STA_COME = 6,
	STA_LEAVE = 7,
	VERIFY_INFO = 8,
	VERIFY_FAIL_INFO = 9,
	WTP_DE_DENEY_STA = 10,
	BSS_INFO = 11,
	ASSOC_FAIL_INFO = 12,
	JIANQUAN_FAIL_INFO = 13,
	CHANNEL_CHANGE_INFO = 14,
	WID_UPDATE = 15,
	WID_CONFLICT = 16,
	WID_ONE_UPDATE = 17,
	TRAFFIC_LIMIT = 18,
	WIDS_INFO = 19,
	WIDS_SET = 20,
	WAPI_INVALID_CERT = 21,
	WAPI_CHALLENGE_REPLAY = 22,
	WAPI_MIC_JUGGLE = 23,
	WAPI_LOW_SAFE_LEVEL = 24,
	WAPI_ADDR_REDIRECTION = 25,
	OPEN_ROAM = 26,
	VRRP_IF = 27,
	STA_WAPI_INFO = 28,
	CANCEL_TRAFFIC_LIMIT = 29,
	WTP_STA_CHECK = 30,	
	WID_WIFI_INFO = 31,
	ASD_AUTH = 32,					//mahz add 2011.3.8
	ASD_DEL_AUTH = 33,				
	EAG_AUTH = 34,
	EAG_DEL_AUTH = 35,				//mahz add 2011.3.8
	BSS_UPDATE = 36,
	EAG_MAC_AUTH = 37,
	EAG_MAC_DEL_AUTH = 38,
	ASD_MAC_AUTH = 39,
	ASD_MAC_DEL_AUTH = 40,
	STA_FLOW_CHECK = 41,
	DHCP_IP	 = 42,					//weichao add 2011.11.10
	IDLE_STA_DEL = 43,
	MAC_LIST_ADD = 44,	
	RADIUS_STA_UPDATE = 45,
	EAG_NTF_ASD_STA_INFO = 46,
	STA_LEAVE_REPORT = 47,
	STA_CHECK_DEL = 48,
	STA_PORTAL_AUTH = 49,
	STA_PORTAL_DEAUTH = 50,
	WID_RSSI = 51,
	WPA_PMK = 52,
	KEY_NEGOTI = 53,
	DYNAMIC_VLAN_ENABLE = 54,
	DYNAMIC_VLAN_UPDATE = 55,
	STA_TRAFFIC_LIMIT = 56,
}Operate;

typedef enum{
	WLAN_TYPE = 0,
	WTP_TYPE = 1,
	BSS_TYPE = 2,
	STA_TYPE = 3,
	SKEY_TYPE = 4,
	PORT_TYPE = 5,
	VLAN_TYPE = 6,
	STA_PKT_TYPE = 7,
	TRAP_TYPE = 8,
	BSS_PKT_TYPE = 9,
	CMD_TYPE = 10,
	BAK_TYPE = 11,
	BSS_TRAFFIC_LIMIT_TYPE = 12,
	WIDS_TYPE = 13,
	AP_REPORT_STA_TYPE = 14,
	NEIGHBOR_AP_STA_CHECK = 15,
	LOCAL_BAK_TYPE = 16,
	DBUS_TYPE = 17,
	RSSI_TYPE = 18,
	PMK_TYPE = 19,
	DYNAMIC_VLAN_TYPE = 20
}MsgType;

typedef struct {
	unsigned int WTPID;
	unsigned int bss_cnt;
} BSS_pkt_header;

typedef struct {
	unsigned int WTPID;
	unsigned char radioid;
	unsigned char channel;
} WTP_channel_change;

typedef struct{
	unsigned int vrrid;
	unsigned int state;
	struct sockaddr_in ipaddr;
	unsigned int virip;
	unsigned int BSSIndex;
	unsigned char virname[NAME_LEN];
	unsigned int bss_count;				//mahz add 2011.6.23
	unsigned int bssindex[4096];
	unsigned int neighbor_slotid;
}wASD_BAK;
typedef struct{
	unsigned char able;
	unsigned int bssindex;
	unsigned int value;					/*����*/
	unsigned int average_value;			/*����*/
	unsigned int send_value;			/*����*/
	unsigned int send_average_value;	/*����*/
}traffic_limit_info;
typedef struct{
	unsigned int bssindex;
	unsigned char bssid[MAC_LEN]; //attack device mac
	unsigned char vapbssid[MAC_LEN]; // attack des
	unsigned char attacktype;
	unsigned char frametype;
	unsigned char channel;
	unsigned char rssi;
}wids_info;

typedef struct {
	unsigned char if_name[IF_NAME_MAX];
	struct mixwtpip	 ip;
	unsigned int op;
}vrrp_interface;

typedef struct{
	unsigned int lasttime;
	unsigned char able;
}wids_set;

typedef struct{     
	unsigned char RadioId;
	unsigned char WlanId;
	unsigned char mac[6];
	unsigned char wapi_trap_flag; /*1-addr_redirect 2-cancel_addr_redirect 3-challenge_replay 4-cancel_challenge_replay*/
	unsigned char ControlledPortStatus;
	unsigned char SelectedUnicastCipher[4];
	unsigned int WAPIVersion;
	unsigned int WPIReplayCounters;
	unsigned int WPIDecryptableErrors;
	unsigned int WPIMICErrors;
} WIDStaWapiInfo;

typedef struct{
	unsigned int WTPID;
	unsigned int sta_num;
	WIDStaWapiInfo StaWapiInfo[64];
} WIDStaWapiInfoList;

/* definition of  struct of table information communicated among WID,ASD or WSM*/
typedef struct{
	unsigned char wlanid;
	unsigned int wtpid;
	unsigned int count;
	unsigned int radioid;
	unsigned char mac[128][MAC_LEN];
}Check_Sta;

#define FREE_BSS_ALL_STA		(0xff)
#define MAX_STAINFO_NUM			(64)
#define MAX_STAINFO_MSGSIZE		(16)

typedef struct 
{
	int wlanid;
	unsigned cnt;    //vlan number in the message
	unsigned short vlan[D_VLAN_MAX];  
}d_vlan_msg;

typedef struct 
{
	int wlanid;
	unsigned int enable;     // 1:dynamic vlan enable 0: dynamic vlan  disable
}d_vlan_enable;
typedef struct {
	MsgType Type;
	Operate Op;
	union{
		wASD_WLAN WLAN;
		wAW_BSS  BSS;
		wWSM_DataChannel  DataChannel;
		aWSM_STA  STA;
		wAW_StaKey KEY;
		wASD_PMK PMK;
		wASD_WTP WTP;
		wASD_RADIO_FLOW RadioFlow;
		wASD_CMD CMDOP;
		BSS_pkt_header bss_header;
		WTP_channel_change WTP_chchange;
		wASD_BAK BAK;
		traffic_limit_info traffic_limit;
		wids_set WIDS_set;
		wids_info WIDS_info;
		vrrp_interface vrrp_if;
		WIDStaWapiInfoList StaWapi;
		aWSM_STA  STAINFO[64];
		Check_Sta bss_sta;
		d_vlan_msg  dvlan_msg;    //dynamic vlan message
		d_vlan_enable dvlan_enable;   // dynamic vlan on-off
	}u;  /*xm0723*/
}TableMsg;

typedef struct {
	MsgType wType;
	Operate wOp;
	union{
		WIRED_STA wSTA;
		WIRED_PORT wPORT;
		WIRED_VLAN wVLAN;
	}u;
}WIRED_TableMsg;


typedef enum{
	IEEE802_11_MGMT = 10,
	IEEE802_11_EAP	= 11,
	IEEE802_3_EAP	= 12,
	IEEE_OTHER	= 64
}DataType;

/*definition of  struct of data information communicated between ASD and WSM*/
typedef struct {
	DataType Type;
	unsigned int WTPID;
	unsigned int Radio_G_ID;
	unsigned int BSSIndex;
	int DataLen;
	char Data[dot11_Max_Len];
}DataMsg;

typedef struct {
	MsgType type;
	unsigned int cnt;
}STAStatisticsMsg;

typedef struct {
	unsigned int WTPID;
	unsigned int Radio_G_ID;
	unsigned int BSSIndex;
	unsigned char STAMAC[MAC_LEN];
	unsigned long long rx_unicast;
	unsigned long long tx_unicast;
	unsigned long long rx_broadcast;
	unsigned long long tx_broadcast;
	unsigned long long rx_pkt_unicast;
	unsigned long long tx_pkt_unicast;
	unsigned long long rx_pkt_broadcast;
	unsigned long long tx_pkt_broadcast;
	unsigned long long retry;
	unsigned long long retry_pkt;
	unsigned long long err;
}STAStatistics;

typedef struct {
	unsigned int BSSIndex;
	unsigned int Radio_G_ID;
	unsigned long long rx_unicast;
	unsigned long long tx_unicast;
	unsigned long long rx_broadcast;
	unsigned long long tx_broadcast;
	unsigned long long rx_pkt_unicast;
	unsigned long long tx_pkt_unicast;
	unsigned long long rx_pkt_broadcast;
	unsigned long long tx_pkt_broadcast;
	unsigned long long retry;
	unsigned long long retry_pkt;
	unsigned long long err;
}BSSStatistics;

typedef struct {
	DataType wType;
	unsigned int PORTID;
	unsigned int VLANID;
	int DataLen;
	char Data[dot11_Max_Len];
}WIRED_DataMsg;

typedef struct {
	time_t  end_time;
	time_t	begin_time;
	unsigned int sta_num;
	time_t end_time_sysrun;
	time_t begin_time_sysrun;
}CHN_TM;

struct wtp_access_info
{
	struct wtp_access_info *next;
	struct wtp_access_info *hnext;
	unsigned int ip;
	time_t time_stamp;
	unsigned char WTPNAME[WTP_NAME_LEN];
	unsigned char *WTPMAC;
	char *model;
	char *apcode;
	char *sn;
	char *version;
	char *codever;
	char *ifname;
	struct alert_info access_alert_info;
};

typedef struct{
	unsigned int num;
	struct wtp_access_info * wtp_list;
	struct wtp_access_info * wtp_list_hash[256];	
}WID_ACCESS;

/*xiaodawei add, 20101029*/
typedef struct{
	unsigned int gmax_wtp_count;
	unsigned int gmax_wtp_count_assign;
	unsigned int gcurrent_wtp_count;
	unsigned int flag;
	unsigned int isShm;
}LICENSE_TYPE;
extern LICENSE_TYPE **g_wtp_count;
extern LICENSE_TYPE **g_wtp_binding_count;
extern pthread_mutex_t ACLicense;
/*######END########*/
struct NetworkQuality
{
	double jitter;	//latency variation, in Iperf UDP test, millisecond, e.g. 0.037ms
	double datagramloss;		//datagram loss, % percent, e.g. 2.2%
};
struct ConflictWtp{
	int wtpindex;
	struct ConflictWtp *next;
};
struct wtp_con_info
{
	unsigned char wtpmac[MAC_LEN];
	unsigned int wtpindex;
	unsigned int wtpindex2;
};

struct conflict_wtp_info {
	struct conflict_wtp_info *next; /* next entry in wtp list */
	struct conflict_wtp_info *hnext; /* next entry in hash table list */
	unsigned char wtpmac[MAC_LEN];
	struct ConflictWtp *wtpindexInfo;
	unsigned int conf_num;
};
struct wid_wtp_info{
	struct conflict_wtp_info *wtp_list; /* WTP info list head */
	struct conflict_wtp_info *wtp_hash[HASH_SIZE];
	unsigned int list_len;
};
extern struct wid_wtp_info allif;

struct lic_ip_info
{
	unsigned int lic_active_ac_ip;
	unsigned int isActive;
};

typedef struct
{
	unsigned char STAMAC[MAC_LEN];
	unsigned char wtpaddr[MAC_LEN];
	unsigned char wlan_id;
	unsigned int ipv4Address;
	unsigned int radio_g_id;
	unsigned int bssindex;			
	unsigned int wtp_id;
	unsigned int vlan_id;
	unsigned int portal_info_switch;
	unsigned int initiative_leave;  /*1-sta initiative_leave 0-others*/
	unsigned int auth_type;
	char arpifname[16];
	char essid[ESSID_LENGTH];
	u_int16_t flow_check;
	u_int32_t no_flow_time;
	u_int32_t limit_flow;	
	PORTAL_INFO portal_info;
	u_int32_t bandwidthmaxup; /* maximum bandwidth for uplink stream: send from STA */
	u_int32_t bandwidthmaxdown; /* maximum bandwidth for downlink stream: send to STA */
} EAG_INFO;

typedef struct {
	MsgType Type;
	Operate Op;
	union {	
		EAG_INFO STA;
	}u;
} EagMsg;

typedef enum {
	WAW_VRRP_STATE_MASTER = 0,		/* master */
	WAW_VRRP_STATE_SECONDARY = 1,	/* secondary */
	WAW_VRRP_STATE_NONE = 2			/* disable or none */
}waw_vrrp_state;

#define WAW_CLI_DEBUG	0
#if WAW_CLI_DEBUG
#define cli_printf	printf
#else
#define cli_printf
#endif
#define WAW_DEFAULT_RSSI	(30)
#define WAW_DIFF_RSSI		(10)	/* 2.4G RSSI = 5.8G RSSI + DIFF RSSI */

void wid_hex_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer);
void wid_hex_char_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer);

#endif
