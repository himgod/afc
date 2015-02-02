#ifndef ACMSGQ_H
#define ACMSGQ_H

typedef enum
{
	CONTROL_TYPE = 1,
	DATA_TYPE = 2 
}MQType;

typedef enum
{
	WLAN_S_TYPE = 1,
	WTP_S_TYPE = 2,
	Radio_S_TYPE = 3,
	STA_S_TYPE = 4,
	WDS_S_TYPE = 5
}MQSubType;

#define WAI_MQSTYPE_STR(sq) 	\
		((WLAN_S_TYPE == sq) ? "WLAN_S_TYPE" : \
		  (WTP_S_TYPE == sq) ? "WTP_S_TYPE": \
		  (Radio_S_TYPE == sq) ? "Radio_S_TYPE": \
		  (STA_S_TYPE == sq)? "STA_S_TYPE": \
		  (WDS_S_TYPE== sq) ? "WDS_S_TYPE":"ERR")
		  
#define WAI_MQTYPE_STR(q,sq)		\
			((DATA_TYPE == q) ? "PKT": \
			(CONTROL_TYPE == q) ? WAI_MQSTYPE_STR(sq):"ERR")

typedef struct
{
	unsigned int len;
	char Data[4096];
}MQ_DATA;

typedef enum{
	WLAN_ADD = 0,
	WLAN_DEL = 1,
	WLAN_HIDE_ESSID = 2,
	WLAN_WDS_ENABLE = 3,
	WLAN_WDS_DISABLE = 4,
	WLAN_CHANGE_TUNNEL = 5,
}WlanOP;

typedef struct{
	WlanOP Wlan_Op;
	unsigned char WLANID;
	unsigned char LEVEL;
	unsigned char Radio_L_ID;
	u_int8_t	HideESSid; /* 1  hide  0 not hode*/
	unsigned char WlanEssid[ESSID_LENGTH];
	unsigned int bssindex;
	char	WlanKey[DEFAULT_LEN];
	unsigned int KeyLen;
	int  wpa_group_rekey;
	unsigned int SecurityType;
	//unsigned int EncryptionType;
	unsigned char SecurityIndex;
	unsigned char asic_hex;/* 0 asic; 1 hex*/
	unsigned char Roaming_Policy;			/*Roaming (1 enable /0 disable)*/
	unsigned short flow_check;
	unsigned int no_flow_time;
	unsigned int limit_flow;
	unsigned char multi_user_optimize_switch;
	unsigned int wlan_send_traffic_limit;
	unsigned int wlan_traffic_limit;
}MQ_WLAN;

typedef enum
{
	WTP_REBOOT = 0,
	WTP_EXTEND_CMD = 1,
	WTP_SCANNING_OP = 2,
	WTP_STATISTICS_REPORT = 3,
	WTP_SET_IP = 4,
	WTP_TIMESTAMP = 5,
	WTP_EXTEND_INFO_GET = 6,
	WTP_SAMPLE_INFO_SET = 7,
	WTP_STA_INFO_SET = 8,
	WTP_IF_INFO_SET = 9,
	WTP_RESEND = 10,
	WTP_WIDS_SET = 11,
	WTP_NEIGHBORDEAD_INTERVAL = 12,
	WTP_SET_IP_DNS = 13,
	WTP_DHCP_SNOOPING = 14,
	WTP_STA_INFO_REPORT = 15,
	WTP_STA_WAPI_INFO_SET = 16,
	WTP_STATISTICS_REPORT_INTERVAL = 17,
	WTP_NTP_SET = 18,
	WTP_TERMINAL_DISTRUB_INFO = 19,
	WTP_IF_ETH_MTU = 20,          /* fengwenchao add 20110126 for XJDEV-32  from 2.0 */
	WTP_FLOW_CHECK = 21,	
	WTP_SET_NAME_PASSWD = 22,
	WTP_MULTI_USER_OPTIMIZE =23,
	WTP_STA_DEAUTH_SWITCH = 24,
	WTP_STA_FLOW_INFORMATION_SWITCH = 25,
	WTP_OPTION60_PARAM =26,
	WTP_LAN_VLAN = 27,			/* ENR-23 */
	WTP_BLACK_WHITE_LIST = 28,
	WTP_ELECTRONIC_MENU = 29,
	WTP_SET_AP_5G_FIRST = 30,
	WTP_STA_ACL_SWITCH = 31,	/* station acl group */
	WTP_STA_ACL_LIST = 32,		/* station acl group */
	WTP_SPECTRUM_ANALYSIS = 33,	
	WTP_QOS_MAP_INFO = 34,
	WTP_QOS_MAP_SWITCH = 35, 
	WLAN_LOAD_BALANCE = 36,
	WTP_WIFI_LOCATE_PUBLIC_CONFIG = 37,
	WTP_ROGUEAP_BLACK_LIST = 38,
	WTP_LOCATE = 39,
	WLAN_TRAFFIC_LIMIT = 40,
	WTP_OP_INVALID = 41,
}WtpOP;

#define WAI_MQWTPOP_STR(op) 	\
	((WTP_REBOOT == op) ? "WTP_REBOOT" : \
	(WTP_EXTEND_CMD == op) ? "WTP_EXTEND_CMD": \
	(WTP_SCANNING_OP == op) ? "WTP_SCANNING_OP": \
	(WTP_STATISTICS_REPORT == op)? "WTP_STATISTICS_REPORT": \
	(WTP_SET_IP == op)? "WTP_SET_IP": \
	(WTP_TIMESTAMP == op)? "WTP_TIMESTAMP": \
	(WTP_EXTEND_INFO_GET == op)? "WTP_EXTEND_INFO_GET": \
	(WTP_SAMPLE_INFO_SET == op)? "WTP_SAMPLE_INFO_SET": \
	(WTP_STA_INFO_SET == op)? "WTP_STA_INFO_SET": \
	(WTP_IF_INFO_SET == op)? "WTP_IF_INFO_SET": \
	(WTP_RESEND == op)? "WTP_RESEND": \
	(WTP_WIDS_SET == op)? "WTP_WIDS_SET": \
	(WTP_NEIGHBORDEAD_INTERVAL == op)? "WTP_NEIGHBORDEAD_INTERVAL": \
	(WTP_SET_IP_DNS == op)? "WTP_SET_IP_DNS": \
	(WTP_DHCP_SNOOPING == op)? "WTP_DHCP_SNOOPING": \
	(WTP_STA_INFO_REPORT == op)? "WTP_STA_INFO_REPORT": \
	(WTP_STA_WAPI_INFO_SET == op)? "WTP_STA_WAPI_INFO_SET": \
	(WTP_STATISTICS_REPORT_INTERVAL == op)? "WTP_STATISTICS_REPORT_INTERVAL": \
	(WTP_NTP_SET == op)? "WTP_NTP_SET": \
	(WTP_TERMINAL_DISTRUB_INFO == op)? "WTP_TERMINAL_DISTRUB_INFO": \
	(WTP_IF_ETH_MTU == op)? "WTP_IF_ETH_MTU": \
	(WTP_FLOW_CHECK == op)? "WTP_FLOW_CHECK": \
	(WTP_SET_NAME_PASSWD == op)? "WTP_SET_NAME_PASSWD": \
	(WTP_MULTI_USER_OPTIMIZE == op)? "WTP_MULTI_USER_OPTIMIZE": \
	(WTP_STA_DEAUTH_SWITCH == op)? "WTP_STA_DEAUTH_SWITCH": \
	(WTP_STA_FLOW_INFORMATION_SWITCH == op)? "WTP_STA_FLOW_INFORMATION_SWITCH": \
	(WTP_OPTION60_PARAM == op)? "WTP_OPTION60_PARAM": \
	(WTP_LAN_VLAN == op)? "WTP_LAN_VLAN": \
	(WTP_BLACK_WHITE_LIST == op)? "WTP_BLACK_WHITE_LIST": \
	(WTP_ELECTRONIC_MENU == op)? "WTP_ELECTRONIC_MENU": \
	(WTP_SET_AP_5G_FIRST == op)? "WTP_SET_AP_5G_FIRST": \
	(WTP_STA_ACL_SWITCH == op)? "WTP_STA_ACL_SWITCH": \
	(WTP_STA_ACL_LIST == op)? "WTP_STA_ACL_LIST": \
	(WTP_SPECTRUM_ANALYSIS == op)? "WTP_SPECTRUM_ANALYSIS": \
	(WTP_QOS_MAP_INFO == op)? "WTP_QOS_MAP_INFO": \
	(WTP_QOS_MAP_SWITCH == op)? "WTP_QOS_MAP_SWITCH": \
	(WLAN_LOAD_BALANCE == op)? "WLAN_LOAD_BALANCE": \
	(WTP_WIFI_LOCATE_PUBLIC_CONFIG == op)? "WTP_WIFI_LOCATE_PUBLIC_CONFIG": \
	(WTP_ROGUEAP_BLACK_LIST == op)? "WTP_ROGUEAP_BLACK_LIST": \
	(WTP_LOCATE == op) ? "WTP_LOCATE": \
	(WLAN_TRAFFIC_LIMIT == op) ? "WLAN_TRAFFIC_LIMIT":"ERR")


typedef struct
{
	char mac[MAC_LEN];
}MACTYPE;

typedef struct{
	WtpOP Wtp_Op;
	unsigned int WTPID;	
	unsigned char radioid;
	unsigned char wlanid;
	unsigned int i1;
	unsigned int i2;
	unsigned short s1;
	unsigned short s2;
	unsigned short s3;
	unsigned char c1;
	unsigned char c2;
	unsigned char value1;
	unsigned int value2;
	unsigned short value3;
	unsigned char value4;
	unsigned char value[512];
	char username[32];
	char passwd[32];
	MACTYPE macarry[128];
}MQ_WTP;

typedef enum{
	Radio_Channel = 0,
	Radio_TXP = 1,
	Radio_Mode = 2,
	Radio_Rates = 3,
	Radio_FragThreshold = 4, 
	Radio_BeaconPeriod = 5,
	Radio_Preamble = 6,
	Radio_DTIMPeriod = 7,
	Radio_ShortRetry = 8,
	Radio_LongRetry = 9,
	Radio_rtsthreshold = 10,
	Radio_Qos = 11,
	Radio_STATUS = 12,
	Radio_Throughput = 13,
	Radio_BSS_Throughput = 14,
	Radio_TXPOF = 15,
	Radio_11N_GI_MCS_CMMODE = 16,
	Radio_ampdu_op = 17,
	Radio_11N_MCS_LIST = 18,  /*fengwenchao add 20120314 for requirements-407*/
	Radio_puren_mixed_op,
	Radio_channel_ext_offset,
	RAdio_tx_chainmask,
	RAdio_rx_chainmask,  //zhangshu add 2010-10-09
	Radio_amsdu_op,       //zhangshu add 2010-10-09
	Radio_acktimeout_distance, /*wcl add for RDIR-33*/
	Radio_NO_RESP_STA_PRO_REQ,
	Radio_UNI_MUTIBRO_CAST_ISO_SW,
	Radio_UNI_MUTIBRO_CAST_TATE,
	Radio_Countrycode,   /*wcl add for OSDEVTDPB-31*/
	Radio_set_cpe_channel,
}RadioOP;

typedef struct{
	RadioOP Radio_Op;
	unsigned char Radio_L_ID;
	unsigned int Radio_G_ID;
	unsigned char BSS_L_ID;
	unsigned int id1;
	unsigned char wlanid;
	unsigned char id_char;
	unsigned short vlan_id;
	unsigned char op;
}MQ_Radio;

typedef enum{
	Sta_ADD = 0,
	Sta_DEL = 1,
	Sta_AUTH = 2,
	Sta_UNAUTH = 3,
	Sta_SETKEY = 4,
	Sta_SETPMK = 5
}StaOP;

typedef struct{
	StaOP Sta_Op;
	unsigned int BSSIndex;
	unsigned char WLANID;
	unsigned char Radio_G_ID;
	unsigned char Radio_L_ID;
	unsigned char pmkid_len;
    unsigned char roamflag; 
	unsigned char STAMAC[MAC_LEN];
    unsigned char BSSID[MAC_LEN];
	unsigned int cipher;
	unsigned int keylen;
	unsigned char keyidx;
	char key[128];
	unsigned int traffic_limit;		/*ht add 091014*/
	unsigned int send_traffic_limit;
	unsigned int vlan_id;
	unsigned int sta_num;
	unsigned char PMK[PMK_LEN];
	unsigned char pmkid[PMKID_LEN];
}MQ_STA;

typedef struct{
	int WTPID;
	MQType type;
	MQSubType subtype;
	union{
		MQ_DATA DataInfo;
		MQ_WLAN WlanInfo;
		MQ_WTP	WtpInfo;
		MQ_Radio	RadioInfo;
		MQ_STA	StaInfo;
	}u;
}msgqdetailData;

typedef struct{
	long mqid;
	msgqdetailData mqinfo;
}msgqData;

typedef struct{
	int WTPID;
	MQType type;
	MQSubType subtype;
	union{
		MQ_WLAN WlanInfo;
		MQ_WTP	WtpInfo;
		MQ_Radio	RadioInfo;
		MQ_STA	StaInfo;
	}u;
	//qiuchen add it for AXSSZFI-1191
	int master_to_disable;
	//end
}msgqdetail;

typedef struct{
	long mqid;
	msgqdetail mqinfo;
}msgq;

struct msgqlist{
	msgqdetail mqinfo;
	struct msgqlist *next;
};
void WID_CONFIG_SAVE(unsigned int WTPIndex);
#endif
