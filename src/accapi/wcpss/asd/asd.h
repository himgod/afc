#ifndef _ASD_H_
#define	_ASD_H_
#include "wcpss/waw.h"
#define DEVINFO_LOCAL_MAC "/devinfo/local_mac"
typedef struct {
	unsigned int ACIP;
	unsigned char ACID;
	unsigned int GroupID;
}Mobility_AC_Info_T;

typedef struct {
	unsigned char GroupID;
	unsigned char WLANID;
	unsigned char *ESSID;
	unsigned char *name;
	unsigned int host_ip;
	Mobility_AC_Info_T *Mobility_AC[G_AC_NUM];
}Inter_AC_R_Group_T;



enum key_input_type{
	HEX=2,
	ASCII=1
};


typedef struct {
 char *acct_ip;
 char *acct_shared_secret;
 unsigned int acct_port;
 char *secondary_acct_ip;               
 char *secondary_acct_shared_secret;
 unsigned int secondary_acct_port;     /*xm 08/09/02*/
}acct_server;


enum cert_type{
	WAPI_X509=1,
	WAPI_GBW=2
};

enum wapi_trap_type{
	ATTACK_INVALID_CERT=1,
	ATTACK_CHALLENGE_REPLAY=2,
	ATTACK_MIC_JUGGLE=3,
	ATTACK_LOW_SAFE_LEVEL=4,
	ATTACK_ADDR_REDIRECTION=5
};

enum asd_debug{ 
	ASD_DEFAULT = 0x1,
	ASD_DBUS = 0x2, 
	ASD_80211 = 0x4, 
	ASD_1X = 0x8, 
	ASD_WPA = 0x10, 
	ASD_WAPI = 0x20,
	ASD_LEAVE =0x40,
	ASD_ALL = 0x7f
};

typedef struct {
 char *as_ip;
 unsigned int as_ip_len;
 unsigned int as_port;
 unsigned char multi_cert;	/*0-disable,1-enable*/
 char *certification_path;
 unsigned int certification_path_len;

 char *ae_cert_path;
 unsigned int ae_cert_path_len;
 char *ca_cert_path;
 unsigned int ca_cert_path_len;
 char *unite_cert_path;
 unsigned int unite_cert_path_len; 
 enum cert_type certification_type; 
}WAPI_AS;


typedef struct {
char *auth_ip;
char *auth_shared_secret;
unsigned int auth_port;
char *secondary_auth_ip;
char *secondary_auth_shared_secret;
unsigned int secondary_auth_port;     /*xm 08/09/02*/
}auth_server;

struct wapi_sub_security{
	unsigned char 	WapiPreauth;			/*default 0,no*/
	unsigned char   MulticaseRekeyStrict;   /*default 0,no*/
	unsigned int 	CertificateUpdateCount;	/*default 3*/
	unsigned int 	MulticastUpdateCount;	/*default 3*/
	unsigned int 	UnicastUpdateCount;	/*default 3*/
	unsigned int 	BKLifetime;	/*default 43200*/
	unsigned int 	BKReauthThreshold;	/*default 70*/
	unsigned int 	SATimeout;	/*default 60*/
	unsigned char 	UnicastCipherEnabled;			/*default 1,yes*/
	unsigned char   AuthenticationSuiteEnable;   /*default 1,yes*/
	unsigned char	MulticastCipher[4];			/*default 00-14-72-01*/
};

enum auth_policy
{
	AUTH_POLICY_INVALID = 0,		/* invalid auth policy */
	AUTH_POLICY_NONE = 1,			/* NONE */
	AUTH_POLICY_MAC = 2,			/* MAC */
	AUTH_POLICY_MAC_WEB = 3,		/* MAC+WEB */
	AUTH_POLICY_MAC_WPA = 4,		/* MAC+WPA */
	AUTH_POLICY_WPA_WEB = 5,		/* WPA+WEB */
	AUTH_POLICY_MAC_WPA_WEB = 6,	/* MAC +WPA+Web */
	AUTH_POLICY_MAX = 7
};

enum authpolicy_mac
{
	ASD_USE_LOCAL_MAC_AUTH = 0,
	ASD_USE_EXTERNAL_RADIUS_AUTH = 1
/*
	ACCEPT_UNLESS_DENIED = 0,
	DENY_UNLESS_ACCEPTED = 1,
	USE_EXTERNAL_RADIUS_AUTH = 2
	*/
};

#define ASD_MAC_AUTH_MAX_DAMAIN			(64)
#define MACAUTH_DEFAULT_EXPIRE			(3600)
struct security {
 unsigned int securityType;
 unsigned int encryptionType; 
	enum auth_policy authpolicy;
	enum authpolicy_mac authpolicy_mac;
	unsigned int webauth_pass;
	unsigned int macauth_expire_time;
	
 unsigned char SecurityID;
 unsigned char RadiusID;
 char *name;
 char *host_ip;
 enum key_input_type keyInputType;
 char *SecurityKey;
 unsigned int keyLen;
 auth_server	auth;
 acct_server	acct;
 int eap_reauth_priod;  /*xm 08/09/02*/
 char *macauth_domain;
 int acct_interim_interval;	/*ht 090205*/
 unsigned int quiet_Period; /*ht 090727*/
 unsigned int extensible_auth;
 unsigned int mac_auth;
 unsigned int accounting_on_disable;
 unsigned int radius_extend_attr;
 unsigned int pre_auth;
 unsigned int pmk_alive_period;
 unsigned int wired_radius;
 WAPI_AS wapi_as;
 unsigned char index;
 int eap_alive_period;	//weichao add 2011.09.22
 int account_after_authorize;	//weichao add 2011.09.22
 int account_after_dhcp;	//weichao add 2011.09.22

 unsigned int wapi_radius_auth;			//mahz add 2010.11.24
 char *user_passwd;						//mahz add 2010.12.9
 unsigned int hybrid_auth;				//mahz add 2011.2.18
 unsigned int fast_auth;				//mahz add 2011.7.8
 unsigned int ap_max_inactivity;
 
 unsigned char wapi_ucast_rekey_method;
 unsigned int wapi_ucast_rekey_para_t;
 unsigned int wapi_ucast_rekey_para_p;

 unsigned char wapi_mcast_rekey_method;
 unsigned int wapi_mcast_rekey_para_t;
 unsigned int wapi_mcast_rekey_para_p;	/*	xm0701*/
 int wpa_group_rekey;
 int wpa_keyupdate_timeout;
 int wpa_once_group_rekey_time;		//mahz add 2011.1.3
 unsigned int distribute_off;		//mahz add 2011.10.25
 unsigned int slot_value;			//mahz add 2011.10.25
 unsigned int inst_value;			//mahz add 2011.10.25
 unsigned char traffic_limite_radius;
 
 struct wapi_sub_security *wapi_config;/*nl 091102*/
 struct security *next;
};
typedef struct security security_profile;
#define ASD_WIFI_L3_ROAM	(2)		/* TODO */

#define ASD_BSSINDEX_TO_WTPID(_bssindex_)	((_bssindex_) / (L_BSS_NUM * L_RADIO_NUM))


#ifdef __ASD_DEBUG_TIME__
#define ASD_DEBUG_TIME()	\
	do {	\
		struct timeval now;	\
		gettimeofday(&now, (struct timezone *)0);\
		asd_printf(ASD_DEFAULT,MSG_INFO,"%s: cur time second %d usec %d\n", __func__, now.tv_sec, now.tv_usec);\
	} while (0);
#else
#define ASD_DEBUG_TIME()
#endif


/* ASD balance */
#define ASD_BALANCE_MAX_ASSOC_CNT		(3)
#define ASD_BALANCE_MIN_AP_NUM			(2)
#define ASD_BALANCE_REJECT				(0)
#define ASD_BALANCE_ACCEPT				(1) 	

#define CTR_LIST_PERMIT		(0)
#define CTR_LIST_DENY		(1)


/* silence period (SP)when kick sta */
#define SP_PERMIT			(0)
#define SP_DENY				(1)
#define SP_AGING_TIMER		(1800)
//#define SP_HASH_SIZE 		(64)
#define SP_HASH(haddr) 		((haddr[5]) % SP_HASH_SIZE)

/***********************************************/

typedef struct  {
       unsigned char slot;
       unsigned char port;
	   unsigned char stat;
}SLOT_PORT_ENABLE_S;/*xm 08/08/27*/

typedef struct  {
       unsigned int port_index;
	   unsigned int vlan_id;
	   unsigned char security;
}PORTINDEX_VLANID_S;	/*xm 08/08/27*/
/***********************************************/
/*sz20080825*/ 
typedef struct  {
	   unsigned int port;
	   unsigned int vlanid;
       unsigned int stat;
}VLAN_PORT_ENABLE;

typedef struct  {
       unsigned int slot;
	   unsigned int port;
	   unsigned int portindex;
	   unsigned int vlanid;
	   unsigned char securityid;
}SLOT_PORT_VLAN_SECURITY;

typedef struct  {
	   unsigned int slot;
	   unsigned int port;
	   unsigned int portindex;
	   unsigned int vlanid;
       unsigned int stat;
}SLOT_PORT_VLAN_ENABLE;

typedef struct  {
       unsigned int vlanid;
       unsigned int port;
	   unsigned char securityid;
}VLAN_PORT_SECURITY;
/*sz20080825 */

struct _wtp_asd {
	unsigned int	WTPID;
	/*unsigned int	WTPModel;*/
    unsigned int	wtp_deny_sta_flag;		//mahz add 2011.4.13

 	unsigned int ap_accessed_sta_num;
 	unsigned int ap_max_allowed_sta_num;
	unsigned int wtp_triger_num;
	unsigned int RSSI;
	unsigned int wtp_flow_triger;
	struct acl_config *acl_conf;
	FLOW_IE      radio_flow_info[L_RADIO_NUM];   /*xm add*/

	unsigned char ra_ch[4];

	unsigned char 	WTPMAC[MAC_LEN];
	unsigned int 	WTPIP;
	char 			WTPSN[128];/*WTP Serial No*/
	char           WTPNAME[256];

	unsigned char	state;
	char           NETID[WTP_NETID_LEN];    //zhangshu add ,2010-10-19 modify netid from point to array,2010-10-26
	char			Binding_IF_NAME[ETH_IF_NAME_LEN];
	wid_wifi_info	wifi_extension_info;
};
typedef struct _wtp_asd ASD_WTP_ST;  /*xm add  08/12/01*/

//mahz add 2011.4.30
struct _wtp_asd_history {
	unsigned int	WTPID;
	unsigned int num_assoc, num_reassoc, num_assoc_failure, num_reassoc_failure;
	unsigned int assoc_success, reassoc_success, assoc_req, assoc_resp;
	unsigned int auth_success, auth_fail,usr_auth_tms,ac_rspauth_tms;

	FLOW_IE      radio_flow_info[L_RADIO_NUM];
	unsigned long long total_ap_flow;
	unsigned long long total_ap_flow_record;
};
typedef struct _wtp_asd_history  ASD_WTP_ST_HISTORY;
//

typedef struct  {
		/*fengwenchao modify 20110224 for long change to long  long   begin*/
	   unsigned long long wl_up_flow;		/*	�������ж˿ڵ�������*/
	   unsigned long long wl_dw_flow;		/*	�������ж˿ڵ�������*/
	   
	   unsigned long long ch_dw_pck;			/*	�ŵ������ܵİ���*/
	   unsigned long long ch_dw_los_pck;		/*	�ŵ������ܵĶ�����*/
	   unsigned long long ch_dw_mac_err_pck;	/*	�ŵ������ܵ�MAC�����*/
	   unsigned long long ch_dw_resend_pck;	/*	�ŵ������ܵ��ش�����*/
	   
	   unsigned long long ch_up_frm;			/*	�ŵ������ܵ�֡��*/
	   unsigned long long ch_dw_frm;			/*	�ŵ������ܵ�֡��*/
	   unsigned long long ch_dw_err_frm;		/*	�ŵ������ܵĴ�֡��*/
	   unsigned long long ch_dw_los_frm;		/*	�ŵ������ܵĶ�֡��*/
	   unsigned long long ch_dw_resend_frm;	/*	�ŵ������ܵ��ش�֡��*/

	   
	   unsigned long long ch_up_los_frm;		/*�ŵ������ܵĶ�֡��*/
	   unsigned long long ch_up_resend_frm;

/*�ŵ������ܵ��ش�֡��*/
	   /*fengwenchao modify 20110224 for long change to long  long  end*/
	   unsigned long long send_bytes;		/*���͵�����֡�ֽ���*/
	   
}BSS_MIB_INFO_ST;	/*	xm0616*/


typedef struct  {

	unsigned char	bssid[MAC_LEN];				/*	xm0630*/
	
	unsigned char	ControlledAuthControl;		/*	�Ƿ����ü�Ȩ(����)*/
	unsigned char	ControlledPortControl;		/*	�˿ڵĿ�������*/
	unsigned char	wapiEnabled;				/*	�Ƿ�����WAPI*/
	
	unsigned long long	CertificateUpdateCount;		/*	��Ȩ���ֵ����Դ���*/
	unsigned long long	MulticastUpdateCount;		/*	MSK���ֵ����Դ���*/
	unsigned long long	UnicastUpdateCount;			/*	������Կ���ֵ����Դ���*/
	
	unsigned char	AuthenticationSuite;			/*	ѡ���AKM�׼�*/
	unsigned char	AuthSuiteSelected;				/*	ѡ���AKM*/
	   
}BSS_WAPI_MIB_INFO_ST;	/*	xm0623*/


typedef struct  {
	
	unsigned long	UnicastRekeyTime;		/*	������Կ��Чʱ��*/
	unsigned int	UnicastRekeyPackets;	/*	������Կ��Ч�����ݰ�����*/
	
	unsigned long	MulticastRekeyTime;		/*	�鲥��Կ��Чʱ��*/
	unsigned int	MulticastRekeyPackets;	/*	�鲥��Կ��Ч�����ݰ�����*/
	
	unsigned char	ControlledPortStatus;	/*	��Ȩ���ƶ˿ڵ�״̬*/

	unsigned char	BKIDUsed[16];			/*	��һ��ʹ�õ�BKID	//	xm0626*/

}STA_WAPI_MIB_INFO_ST;	/*	xm0623*/

struct maclist {
	unsigned char addr[MAC_LEN];
	unsigned char add_reason;	//0 for static,1 for dynamic
	unsigned char vapbssid[MAC_LEN];
	unsigned char attacktype;
	unsigned char frametype;
	unsigned char channel;
	unsigned char rssi;
	time_t add_time;
	time_t add_time_sysruntime;//qiuchen add it
	struct maclist *next;
};


struct dcli_acl_config {
	unsigned int  	macaddr_acl;		/*0-none;1-black;2-white*/	
	unsigned char	wids_set;
	unsigned int 	wids_last_time;
	unsigned int 	num_deny_mac;
	unsigned int 	num_wids_mac;
	struct maclist *deny_mac;
	unsigned int 	num_accept_mac;
	struct maclist *accept_mac;
};

#define BIT(x) (1 << (x))

/* dcli_sta_info sta_flags copy from STA flags */
#define WLAN_STA_AUTH 				BIT(0)
#define WLAN_STA_ASSOC 				BIT(1)
#define WLAN_STA_PS 				BIT(2)
#define WLAN_STA_TIM 				BIT(3)
#define WLAN_STA_PERM 				BIT(4)
#define WLAN_STA_AUTHORIZED 		BIT(5)
#define WLAN_STA_PENDING_POLL 		BIT(6) /* pending activity poll not ACKed */
#define WLAN_STA_SHORT_PREAMBLE 	BIT(7)
#define WLAN_STA_PREAUTH 			BIT(8)
#define WLAN_STA_WME 				BIT(9)
#define WLAN_STA_MFP 				BIT(10)
#define WLAN_STA_HT 				BIT(11)
#define WLAN_STA_WPS 				BIT(12)
#define WLAN_STA_MAYBE_WPS 			BIT(13)
#define WLAN_STA_ROAMOUT			BIT(14)
#define WLAN_STA_ROAMING_L2			BIT(16)	/* L2 ROAM */  
#define WLAN_STA_ROAMING_L3			BIT(17) /* L3 ROAM */
#define WLAN_STA_REAUTH				BIT(18)	/* recv double auth */
#define WLAN_STA_BALANCE_MULASSOC	BIT(19)	/* wlan balance assoc more then 3 times */
#define WLAN_STA_BALANCE_NOAP		BIT(20)	/* no ap list to balance */
#define WLAN_STA_DEL 				BIT(27) /*set when ap told sta is leave*/
#define WLAN_STA_ROAMING 			BIT(28)
#define WLAN_STA_FREE 				BIT(29) 
#define WLAN_STA_AUTH_ACK 			BIT(30)
#define WLAN_STA_NONERP 			BIT(31)


struct dcli_sta_info
{
	struct dcli_sta_info *next; /* next entry in sta list */
	unsigned char addr[6];
	unsigned char *ip;	
	
	unsigned char 	wlan_id;
	unsigned char 	security_id;
	unsigned char	radio_l_id;
	unsigned int radio_g_id;
	unsigned int bssindex;
	unsigned int wtp_id;
	unsigned int	security_type;		//mahz add 2011.3.1

	unsigned int port_id;
	unsigned int vlan_id;
	
	unsigned int sta_flags;
	unsigned int pae_state;
	unsigned int backend_state;
	
	unsigned int sta_traffic_limit;		 /*����*//*ָstation �ϴ���AP�Ĵ������� */
	unsigned int sta_send_traffic_limit; /*����*//*ָstation ��AP���صĴ������� */

	time_t StaTime;		/*Access time*/
	
	unsigned int snr;
	unsigned long long rr;  
	unsigned long long tr;   //receive rate    transmit rate   byte
	unsigned long long tp;   //throughput
	unsigned long long rxbytes;
	unsigned long long txbytes;
	unsigned long long rxpackets;
	unsigned long long txpackets;
	unsigned long long retrybytes;
	unsigned long long retrypackets;
	unsigned long long errpackets;
	
	unsigned long long rxsuccpackets;	/*packets ap send to sta successfully*/

	double flux ;

	/*sta info single intensity etc.*/
	unsigned char mode;  //11b-0x01,11a-0x02,11g-0x04,11n-0x08,
	unsigned char channel;
	unsigned char rssi;
	unsigned short nRate;
	unsigned char isPowerSave;
	unsigned char isQos;
	unsigned char info_channel;

	unsigned long	UnicastRekeyTime;		/*	������Կ��Чʱ��*/
	unsigned int	UnicastRekeyPackets;	/*	������Կ��Ч�����ݰ�����*/
	
	unsigned long	MulticastRekeyTime; 	/*	�鲥��Կ��Чʱ��*/
	unsigned int	MulticastRekeyPackets;	/*	�鲥��Կ��Ч�����ݰ�����*/
	
	unsigned char	ControlledPortStatus;	/*	��Ȩ���ƶ˿ڵ�״̬*/
	unsigned char	BKIDUsed[16];			/*	��һ��ʹ�õ�BK?ID*/
	
	/*packets*/
	unsigned long long rx_pkts ;
	unsigned long long tx_pkts ;
	unsigned long long rtx ; 
	unsigned long long rtx_pkts ;
	unsigned long long err_pkts ;

	unsigned int	wapi_version;
	unsigned char	controlled_port_status;
	unsigned char	selected_unicast_cipher[4];

	unsigned int	wai_sign_errors;
	unsigned int	wai_hmac_errors;
	unsigned int	wpi_mic_errors; 	
	unsigned int	wpi_replay_counters;
	unsigned int	wpi_decryptable_errors;

	unsigned int	wai_auth_res_fail;
	unsigned int	wai_discard;
	unsigned int	wai_timeout;
	unsigned int	wai_format_errors;
	unsigned int	wai_cert_handshake_fail;
	unsigned int	wai_unicast_handshake_fail;
	unsigned int	wai_multi_handshake_fail;
	unsigned char *identify;
	char eap_type;
	time_t sta_online_time;//qiuchen add it

	unsigned int realip;
};


struct dcli_sta_info_v2
{
	struct dcli_sta_info *next; /* next entry in sta list */
	unsigned char addr[6];
	unsigned char 	wlan_id;
	unsigned char 	security_id;
	unsigned char	radio_l_id;
	unsigned int radio_g_id;
	unsigned int bssindex;
	unsigned int wtp_id;
	unsigned int vlan_id;
	char * essid;
	//weichao add 
	u_int16_t   flow_check;
	u_int32_t   no_flow_time;
	u_int32_t   limit_flow;
	unsigned int	auth_type;		//weichao add
	unsigned int realip;	/* TUNNEL AUTH LOCAL FORWAD  STA real IP addr */
};

struct dcli_sta_base_info
{
	struct dcli_sta_info *next; /* next entry in sta list */
	unsigned char addr[6];
	unsigned char wtp_addr[6];
	unsigned char wlan_id;
	unsigned char radio_l_id;
	unsigned int radio_g_id;
	unsigned int wtp_id;
	unsigned char * essid;
	unsigned char *ip;	
};

struct dcli_bss_info {
	struct dcli_bss_info *next;

	struct dcli_acl_config acl_conf;
	
	unsigned char 	mac[6];
	unsigned char 	WlanID;
	unsigned char 	SecurityID;
	unsigned char 	Radio_L_ID;
	unsigned int 	Radio_G_ID;
	unsigned int 	WtpID;
	unsigned int 	BSSIndex;
	unsigned char 	bssid[6];
	char			Binding_IF_NAME[ETH_IF_NAME_LEN];

	unsigned int 	PortID;		/*for wired bss*/
	unsigned int 	VlanID;

	unsigned int 	num_sta; 		/* number of entries in sta_list */
	struct dcli_sta_info *sta_list; /* STA info list head */
	struct dcli_sta_info *sta_last; /* STA info last head */

	unsigned int num_assoc;
	unsigned int num_reassoc;
	unsigned int num_assoc_failure;
	unsigned int assoc_success;
	unsigned int reassoc_success;

	/*statics info*/
	unsigned int access_sta_num ; 
	unsigned int auth_req_num ; 
	unsigned int auth_resp_num ; 
	unsigned int total_deny_num;
	
	unsigned int auth_success_num ; //5
	unsigned int auth_fail_num ; 
	unsigned int auth_invalid_num ; 
	unsigned int auth_timeout_num ; 
	unsigned int auth_refused_num ; 
	unsigned int auth_others_num ; //10
	unsigned int assoc_req_num ; 
	unsigned int assoc_resp_num ; 
	unsigned int assoc_invalid_num ; 
	unsigned int assoc_timeout_num ; 
	unsigned int assoc_refused_num ; //15
	unsigned int assoc_others_num ; 
	unsigned int reassoc_request_num ; 
	unsigned int reassoc_success_num ; 
	unsigned int reassoc_invalid_num ; 
	unsigned int reassoc_timeout_num ; //20
	unsigned int reassoc_refused_num ; 
	unsigned int reassoc_others_num ; 
	unsigned int identify_request_num ; 
	unsigned int identify_success_num ; 
	unsigned int abort_key_error_num ; //25
	unsigned int abort_invalid_num ; 
	unsigned int abort_timeout_num ; 
	unsigned int abort_refused_num ; 
	unsigned int abort_others_num ; 
	unsigned int deauth_request_num ; //30
	unsigned int deauth_user_leave_num ; 
	unsigned int deauth_ap_unable_num ; 
	unsigned int deauth_abnormal_num ; 
	unsigned int deauth_others_num ; 
	unsigned int disassoc_request_num ; //35
	unsigned int disassoc_user_leave_num ; 
	unsigned int disassoc_ap_unable_num ; 
	unsigned int disassoc_abnormal_num ; 
	unsigned int disassoc_others_num ; 

	unsigned int rx_mgmt_pkts ;//40
	unsigned int tx_mgmt_pkts ;
	unsigned int rx_ctrl_pkts ;
	unsigned int tx_ctrl_pkts ;
	unsigned int rx_data_pkts ;
	unsigned int tx_data_pkts ;//45
	unsigned int rx_auth_pkts ;
	unsigned int tx_auth_pkts ;
	unsigned long long total_past_online_time;	
	unsigned long long total_present_online_time;	

	unsigned int	traffic_limit;
	unsigned int	sta_average_traffic_limit;
	unsigned int	send_traffic_limit;	/*����,ht add 090902*/
	unsigned int	sta_average_send_traffic_limit;//51

	unsigned int	acc_tms;
	unsigned int	auth_tms;
	unsigned int	repauth_tms;

	unsigned int	wai_sign_errors;
	unsigned int	wai_hmac_errors;
	unsigned int	wai_auth_res_fail;
	unsigned int	wai_discard;
	unsigned int	wai_timeout;
	unsigned int	wai_format_errors;
	unsigned int	wai_cert_handshake_fail;
	unsigned int	wai_unicast_handshake_fail;
	unsigned int	wai_multi_handshake_fail;
	unsigned int	wpi_mic_errors;		// D8.2.3
	unsigned int	wpi_replay_counters;
	unsigned int	wpi_decryptable_errors;
	unsigned char 	WapiEnabled;

	unsigned long long wl_up_flow;			/*	�������ж˿ڵ�������*/
	unsigned long long wl_dw_flow;			/*	�������ж˿ڵ�������*/
	   
	unsigned long long ch_dw_pck;			/*	�ŵ������ܵİ���*/
	unsigned long long ch_dw_los_pck;		/*	�ŵ������ܵĶ�����*/
	unsigned long long ch_dw_mac_err_pck;	/*	�ŵ������ܵ�MAC�����*/
	unsigned long long ch_dw_resend_pck; 	/*	�ŵ������ܵ��ش�����*/
	   
	unsigned long long ch_up_frm;			/*	�ŵ������ܵ�֡��*/
	unsigned long long ch_dw_frm;			/*	�ŵ������ܵ�֡��*/
	unsigned long long ch_dw_err_frm;		/*	�ŵ������ܵĴ�֡��*/
	unsigned long long ch_dw_los_frm;		/*	�ŵ������ܵĶ�֡��*/
	unsigned long long ch_dw_resend_frm; 	/*	�ŵ������ܵ��ش�֡*/

	unsigned long long ch_up_los_frm;		/*�ŵ������ܵĶ�֡��*/
	unsigned long long ch_up_resend_frm; 	/*�ŵ������ܵ��ش�֡��*/		
	unsigned long long send_bytes;		/*���͵�����֡�ֽ���*/

	unsigned char	ControlledAuthControl;		/*	�Ƿ����ü�Ȩ(����)*/
	unsigned char	ControlledPortControl;		/*	�˿ڵĿ�������*/
	unsigned char	wapiEnabled;				/*	�Ƿ�����WAPI*/
	
	unsigned long long	CertificateUpdateCount; 	/*	��Ȩ���ֵ����Դ���*/
	unsigned long long	MulticastUpdateCount;		/*	MSK���ֵ����Դ���*/
	unsigned long long	UnicastUpdateCount; 		/*	������Կ���ֵ����Դ���*/
	
	unsigned char	AuthenticationSuite;			/*	ѡ���AKM�׼�*/
	unsigned char	AuthSuiteSelected;				/*	ѡ���AKM*/

	unsigned char if_policy;
};

struct dcli_base_bss_info {
	struct dcli_bss_info *next;
	
	unsigned char 	mac[6];
	unsigned char 	WlanID;
	unsigned char 	SecurityID;
	unsigned char 	Radio_L_ID;
	unsigned int 	Radio_G_ID;
	unsigned int 	WtpID;
	unsigned int 	BSSIndex;
	unsigned char 	bssid[6];

	unsigned int 	num_sta; 		/* number of entries in sta_list */
	struct dcli_sta_base_info *sta_list; /* STA info list head  , mahz add for mib request*/
	struct dcli_sta_base_info *sta_last; /* STA info last head , mahz add for mib request*/
};

struct dcli_wtp_info{
	struct dcli_wtp_info *next;

	struct dcli_bss_info *bss_list;
	struct dcli_bss_info *bss_last;
	
	struct dcli_radio_info *radio_list;
	struct dcli_radio_info *radio_last;
	
	struct dcli_acl_config acl_conf;
	
	unsigned int 	WtpID;
	unsigned char  	mac[6];

	unsigned int 	num_sta;
	unsigned int 	num_bss;
	unsigned int    num_radio;

	unsigned int 	num_assoc;
	unsigned int 	num_reassoc;
	unsigned int 	num_assoc_failure;
	unsigned int 	num_normal_sta_down;
	unsigned int	num_abnormal_sta_down;
	unsigned int	num_accessed_sta;	//ht 100630

	unsigned long long	wtp_total_past_online_time;	//	xm0703	
	unsigned int		acc_tms;
	unsigned int		auth_tms;
	unsigned int		repauth_tms;
	unsigned int		auth_success_num;
	unsigned int		auth_fail_num;
	unsigned int		auth_invalid_num;
	unsigned int		auth_timeout_num;
	unsigned int		auth_refused_num;		//10
	unsigned int		auth_others_num;
	unsigned int		assoc_req_num;
	unsigned int		assoc_resp_num;
	unsigned int		assoc_invalid_num;
	unsigned int		assoc_timeout_num;		//15
	unsigned int		assoc_refused_num;
	unsigned int		assoc_others_num;
	unsigned int		reassoc_request_num;
	unsigned int		reassoc_success_num;
	unsigned int		reassoc_invalid_num;		//20
	unsigned int		reassoc_timeout_num;
	unsigned int		reassoc_refused_num;
	unsigned int		reassoc_others_num;
	unsigned int		identify_request_num;
	unsigned int		identify_success_num;		//25
	unsigned int		abort_key_error_num;
	unsigned int		abort_invalid_num;
	unsigned int		abort_timeout_num;
	unsigned int		abort_refused_num;
	unsigned int		abort_others_num;		//30											
	unsigned int		deauth_request_num;
	unsigned int		deauth_user_leave_num;
	unsigned int		deauth_ap_unable_num;
	unsigned int		deauth_abnormal_num;
	unsigned int		deauth_others_num;		//35
	unsigned int		disassoc_request_num;
	unsigned int		disassoc_user_leave_num;
	unsigned int		disassoc_ap_unable_num;
	unsigned int		disassoc_abnormal_num;
	unsigned int		disassoc_others_num;		//40	
	unsigned int		rx_mgmt_pkts;
	unsigned int		tx_mgmt_pkts;
	unsigned int		rx_ctrl_pkts;
	unsigned int		tx_ctrl_pkts;
	unsigned int		rx_data_pkts;	//45
	unsigned int		tx_data_pkts;
	unsigned int		rx_auth_pkts;
	unsigned int		tx_auth_pkts;	//48
		
	unsigned int 	ap_wapi_version ;
	unsigned int 	wai_sign_errors ;
	unsigned int 	wai_hmac_errors ;
	unsigned int 	wai_auth_res_fail ;
	unsigned int 	wai_discard ;	//5
	unsigned int 	wai_timeout ;
	unsigned int 	wai_format_errors ;
	unsigned int 	wai_cert_handshake_fail ;
	unsigned int 	wai_unicast_handshake_fail ;
	unsigned int 	wai_multi_handshake_fail ; //10
	unsigned int 	wpi_mic_errors ;
	unsigned int 	wpi_replay_counters ;
	unsigned int 	wpi_decryptable_errors ;//13

	//mahz add 2011.5.3
	unsigned int num_assoc_record, num_reassoc_record, num_assoc_failure_record, num_reassoc_failure_record;
	unsigned int assoc_success_record, reassoc_success_record, assoc_req_record, assoc_resp_record;
	unsigned int auth_success_record, auth_fail_record,usr_auth_tms_record,ac_rspauth_tms_record;
	unsigned long long total_ap_flow_record;
	//
	
#if 1	
/*for extern show wtp sta*/
	unsigned int deny_num ;
	unsigned int bss_deny_num ;


#endif
};

struct dcli_radio_info{
	struct dcli_radio_info *next;
	unsigned int radioid;			
	unsigned int acc_tms;
	unsigned int auth_tms;
	unsigned int repauth_tms;						
	unsigned int auth_success_num;//5
	unsigned int auth_fail_num;
	unsigned int auth_invalid_num;
	unsigned int auth_timeout_num;
	unsigned int auth_refused_num;
	unsigned int auth_others_num;//10
						
	unsigned int assoc_req_num;
	unsigned int assoc_resp_num;
				
	unsigned int assoc_invalid_num;
	unsigned int assoc_timeout_num;
	unsigned int assoc_refused_num;//15
	unsigned int assoc_others_num;
						
	unsigned int reassoc_request_num;
	unsigned int reassoc_success_num;
	unsigned int reassoc_invalid_num;
	unsigned int reassoc_timeout_num;//20
	unsigned int reassoc_refused_num;
	unsigned int reassoc_others_num;
						
	unsigned int identify_request_num;
	unsigned int identify_success_num;
	unsigned int abort_key_error_num;//25
	unsigned int abort_invalid_num;
	unsigned int abort_timeout_num;
	unsigned int abort_refused_num;
	unsigned int abort_others_num;
	
	unsigned int deauth_request_num;//30
	unsigned int deauth_user_leave_num;
	unsigned int deauth_ap_unable_num;
	unsigned int deauth_abnormal_num;
	unsigned int deauth_others_num;
	
	unsigned int disassoc_request_num;//35
	unsigned int disassoc_user_leave_num;
	unsigned int disassoc_ap_unable_num;
	unsigned int disassoc_abnormal_num;
	unsigned int disassoc_others_num;

	unsigned int rx_mgmt_pkts;//40
	unsigned int tx_mgmt_pkts;
	unsigned int rx_ctrl_pkts;
	unsigned int tx_ctrl_pkts;
	unsigned int rx_data_pkts;
	unsigned int tx_data_pkts;//45
	unsigned int rx_auth_pkts;
	unsigned int tx_auth_pkts;//47

	unsigned int num_bss;			
	struct dcli_bss_info *bss_list;
	struct dcli_bss_info *bss_last;

};

struct dcli_wlan_info{
	struct dcli_wlan_info *next;
	struct dcli_wlan_info *wlan_list;   //add fengwenchao 20101221
	struct dcli_wlan_info *wlan_last;   //add fengwenchao 20101221
	unsigned char 	WlanID;

	unsigned int 	num_assoc;
	unsigned int 	num_reassoc;
	unsigned int 	num_assoc_failure;
	unsigned int 	num_normal_sta_down;
	unsigned int	num_abnormal_sta_down;
	
	unsigned int rx_pkts ;	
	unsigned int tx_pkts ;	
	unsigned int assoc_req_num ; 
	unsigned int assoc_resp_num ; 
	unsigned long long rx_bytes ;
	unsigned long long tx_bytes ;

	unsigned int assoc_fail_num ;
	unsigned int sta_assoced_num;
	unsigned int reassoc_num ; 
	unsigned int reassoc_success_num ; 
	unsigned int sta_accessed_num;

	unsigned int assoc_req_interim ;
	unsigned int assoc_resp_interim ;
	unsigned int assoc_success_interim ;

	unsigned int 	num_sta;
	unsigned int 	num_bss;
	struct dcli_acl_config	 acl_conf;
	struct dcli_bss_info 	*bss_list;
	struct dcli_bss_info 	*bss_last;
};

struct dcli_channel_info {
	
	struct dcli_channel_info *next ;

	struct dcli_channel_info *channel_list ;
	struct dcli_channel_info *channel_last ;
	
	unsigned char channel_id ;
	unsigned int sta_num ;
	time_t StaTime ;
	time_t statime_sysrun;//qiuchen add it
	
} ;

struct dcli_iface_info{
	struct dcli_iface_info *next;
	char 					IF_NAME[ETH_IF_NAME_LEN];
	unsigned int 			num_sta;
	unsigned int 			num_bss;
	struct dcli_bss_info 	*bss_list;
	struct dcli_bss_info 	*bss_last;
};

struct dcli_ac_info{
	unsigned int 	num_auth;
	unsigned int 	num_auth_fail;
	unsigned int 	num_assoc;
	unsigned int 	num_reassoc;
	unsigned int 	num_assoc_failure;
	unsigned int 	num_reassoc_failure;
	unsigned int 	num_normal_sta_down;
	unsigned int	num_abnormal_sta_down;

	unsigned int 	num_accessed_sta;
	unsigned int 	max_sta_num;
	unsigned int 	num_sta;
	unsigned int 	num_sta_wired;
	unsigned int 	num_sta_all;
	unsigned int 	num_local_roam;
	unsigned int	num_unconnect_sta;
	unsigned int 	num_bss_wireless;
	unsigned int 	num_bss_wired;	
	unsigned int 	num_bss;
	
	unsigned int 	num_wtp;
	unsigned int 	num_wlan;
	unsigned int	num_iface;
	struct dcli_bss_info *bss_list;
	struct dcli_bss_info *bss_last;
	struct dcli_wtp_info *wtp_list;
	struct dcli_wtp_info *wtp_last;
	struct dcli_wlan_info *wlan_list;
	struct dcli_wlan_info *wlan_last;
	struct dcli_iface_info *iface_list;
};


struct dcli_security {
	struct dcli_security 	*next;
	unsigned int 	SecurityType;
	unsigned int 	EncryptionType; 
	unsigned char 	SecurityID;
	char 			*name;
	char 			*host_ip;
	char 			*SecurityKey;
	unsigned int	keyInputType;
	unsigned int 	keyLen;
	auth_server		auth;
	acct_server		acct;
	int 			eap_reauth_period;  
	int 			acct_interim_interval;	
	unsigned int 	quiet_period;
	unsigned int 	extensible_auth;
	unsigned int 	accounting_on_disable;
	unsigned int 	radius_extend_attr;
	unsigned int 	pre_auth;
	unsigned int 	wired_radius;
	WAPI_AS 	 	wapi_as; 
	unsigned char	 security_index;

	unsigned int    wapi_radius_auth;		//mahz add 2010.11.24
	int 			eap_alive_period;  		//weichao add
	char *user_passwd;						//mahz add 2010.12.9
	unsigned int    hybrid_auth;			//mahz add 2011.2.28
	unsigned int slot_value;				//mahz add 2011.11.18
	unsigned int inst_value;
	unsigned int  mac_auth;
	unsigned int	webauth_pass;
	unsigned char 	wapi_ucast_rekey_method;
	unsigned int 	wapi_ucast_rekey_para_t;
	unsigned int 	wapi_ucast_rekey_para_p;
	unsigned char 	wapi_mcast_rekey_method;
	unsigned int 	wapi_mcast_rekey_para_t;
	unsigned int 	wapi_mcast_rekey_para_p;	
	unsigned char 	WapiPreauth;			/*default 0,no*/
	unsigned char   MulticaseRekeyStrict;   /*default 0,no*/
	unsigned int 	CertificateUpdateCount;	/*default 3*/
	unsigned int 	MulticastUpdateCount;	/*default 3*/
	unsigned int 	UnicastUpdateCount;	/*default 3*/
	unsigned int 	BKLifetime;	/*default 43200*/
	unsigned int 	BKReauthThreshold;	/*default 70*/
	unsigned int 	SATimeout;	/*default 60*/
	unsigned char 	UnicastCipherEnabled;			/*default 1,yes*/
	unsigned char   AuthenticationSuiteEnable;   /*default 1,yes*/
	unsigned char	MulticastCipher[4];	
	unsigned char 	RadiusID;


	unsigned char	AuthenticationSuiteEnabled;
	unsigned char	IsInstalledCer;
	char			RekeyMethod[30];
	unsigned char	ControlledAuthControl;
	unsigned int  	ConfigVersion;
	unsigned char	UnicastKeysSupported;
	unsigned char	WapiSupported;
	unsigned int  	MulticastCipherSize ;
	unsigned int  	UnicastCipherSize ;
	unsigned int 	MulticastRekeyPackets ;	
	unsigned int 	UnicastRekeyPackets ;
	unsigned char 	UnicastCipher[4];
	unsigned char 	AuthenticationSuite[4];
	unsigned char 	UnicastCipherSelected[4];
	unsigned char 	MulticastCipherSelected[4];
	unsigned char 	UnicastCipherRequested[4];
	unsigned char 	MulticastCipherRequested[4];
	unsigned char 	AuthSuitSelected_Auth[4];	
	unsigned char 	AuthSuitRequested_Psk[4];
	unsigned char 	MulticastRekeyStrict;
	unsigned char	WapiPreauthEnabled;
	enum auth_policy authpolicy;
};

struct sta_static_info {
	struct sta_static_info *next; 
	struct sta_static_info *hnext; 
	unsigned char	addr[6];
	unsigned int 	vlan_id;
	unsigned int	sta_traffic_limit;		/*����*//*ָstation �ϴ���AP�Ĵ������� */
	unsigned int	sta_send_traffic_limit; /*����*//*ָstation ��AP���صĴ������� */
	unsigned char 	wlan_id;
};

extern security_profile *ASD_SECURITY[WLAN_NUM];
extern ASD_WTP_ST **ASD_WTP_AP;
extern ASD_WTP_ST_HISTORY **ASD_WTP_AP_HISTORY;  //mahz add 2011.4.30


//mahz add for mib request , 2011.1.25 , dot11StaWAPIProtocolConfigTable
struct wapi_mib_wtp_info{
	struct wapi_mib_wtp_info *next;
	unsigned int	WtpID;
	unsigned char   mac[6];
	unsigned int 	num_sta; 		/* number of entries in sta_list */
	struct dcli_sta_info *sta_list; /* STA info list head */
	struct dcli_sta_info *sta_last; /* STA info last head */
};
struct sta_nas_info{
	char nas_port_id[NAS_PORT_ID_LEN+1];
	char nas_identifier[NAS_IDENTIFIER_NAME+1];
	unsigned char nasid_len;
};
extern struct sta_nas_info *ASD_HOTSPOT[HOTSPOT_ID+1];

typedef struct{
	char MAC[MAC_LEN];
}ASDCmdSta;

typedef struct{
	char type;
}ASDCmdArpL;
typedef struct{
	unsigned char socketfd;
	unsigned char operate; 
}ASDCmdFD;
typedef enum{
	ASD_STA_TYPE = 1,
	ASD_FD_TYPE = 2,
	ASD_ARP_LISTEN = 3
}CMDType;

typedef enum{
	ASD_CMD_ADD = 1,
	ASD_CMD_DEL = 2,
	ASD_CMD_UPDATE = 3,
	ASD_CMD_UNIX_REINIT =4,
	ASD_CMD_NETLINK_REINIT =5
}CMDOp;

typedef struct {
	CMDType Type;
	CMDOp Op;
	union{
		ASDCmdSta stainfo;
		ASDCmdArpL arplisten;
		ASDCmdFD fdinit;
	}u;
}ASDCmdMsg;
//qiuchen
enum asd_auth_type {
	asd_auth_type_unkown = 1,			/* unkown */
	asd_auth_type_authfree = 2,			/* auth free */
	asd_auth_type_weppsk = 3,			/* WEP/PSK (share:wep) */	
	asd_auth_type_autoauth = 4			/* SIM/PEAP */
}asd_auth_type_t;

#define ASD_MAC_TYPE_LOCAL			(0)		/* local */
#define ASD_MAC_TYPE_TUNNEL			(1)		/* tunnel */
#define ASD_MAC_TYPE_TL				(2)		/* tunnel auth, local forward */
#endif
