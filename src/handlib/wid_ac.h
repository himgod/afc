#ifndef _WID_AC_H
#define _WID_AC_H

#include <dbus/dbus.h>
#include "wcpss/wid/WID.h"

#define MAX_WLANID 16

typedef struct
{
	int 	WTPID;
	char* 	WTPSN;/*WTP Serial No*/
	char*	WTPNAME;
	char*	WTPModel;/*model of wtp which determin the number of Radios and rates supported in WTP*/
	unsigned char WTPMAC[MAC_LEN];
	char * adopt_amdin;/*adopt by or NULL*/
	char * adopt_ip;/*adopt from or NULL*/
	int apcodeflag;
} NEW_WTP_ARG;

typedef struct
{
	u_int32_t	WTPID;
	unsigned char RADIOMAC[MAC_LEN];
	u_int8_t	Radio_Chan;/*Channel*/
	u_int8_t	Radio_L_ID;/*Radio Local ID*/
	u_int32_t	Radio_G_ID;/*Radio Global ID*/
	u_int32_t	Radio_Type;/*a/b/g/n*/
	u_int16_t   cwmode;
	u_int16_t	Radio_TXP;/*TX power*/
	u_int16_t	Radio_TXPOF;/*TX  power offset*/
	u_int16_t	BeaconPeriod;/*Beacon interval*/
	int 		Radio_country_code; /*wcl add for OSDEVTDPB-31*/
} NEW_RADIO_ARG;

typedef struct wlan_web_info
{
	int WlanId;
	int isExist;
	char ssid[ESSID_DEFAULT_LEN];
	char user_key[USER_KEY_LEN];
	int securType;
	int wepIndex;
	int sHideflag;
	int isEnable;
	int applyGflag;
	int vlanEnabled;
	int vlanId;
	int usrGid;
	int EncryptionType;
	unsigned int Authip;
	unsigned int AuthPort;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned int tx_packets;
	unsigned int rx_packets;
	unsigned long long tx_bytes;
	unsigned long long rx_bytes;
	wid_wlan_wtp_wifi wlan_wtp_wifi;
} WLANWEBINFO;

typedef struct usrgrp_web_info
{
	unsigned char grpid;
	unsigned char grpnmame[NAME_LEN];
	unsigned char used;
	unsigned char apply_wlan_num;
	unsigned char apply_wlanid[WLAN_NUM];/*binding wlan id store apply wlan id*/
	unsigned int send_traffic_limit;
	unsigned int traffic_limit;
} USERGRP_WEBINFO;

struct stainfo_to_web
{
    u32 wtpid;
    u8 l_radioid;
    u8 wlanid;
	u8 addr[6];
	u32 flags;
	u8 ostype[DEFAULT_LEN];
	u8 pd_model[DEFAULT_LEN];
	u8 hostname[DEFAULT_LEN];
	u8 cpu_model[DEFAULT_LEN];
	u8 sta_name[STA_INFO_NAME_LEN];
	char in_addr[STA_INFO_INADDR_LEN];
	struct in_addr ip_addr;
	u64 rxbytes_old;
	u64 txbytes_old;
	u64 rxbytes;
	u64 txbytes;
	u64 pre_rxbytes;
	u64 pre_txbytes;
	u64 rxdatapackets;  /*ap report*/ 
	u64 txdatapackets;  /*ap report*/ 
	u64 rxpackets;
	u64 txpackets;
	u64 pre_rxpackets;
	u64 pre_txpackets;
	u64 rxframepackets;  /*ap report*/ 
	u64 txframepackets;  /*ap report*/ 
	u64 errpackets;
	u64 retrybytes;
	u64 retrypackets;
	unsigned char rx_retry_per;
	unsigned char tx_retry_per;
	unsigned char pkt_loss_per;
	u16 capability;
	unsigned short roam_type;	/* 0-未漫游 1-漫游离开  2-漫游接入 */
	unsigned char account_stop_flag;
	unsigned char sta_assoc_auth_flag;
	u8 supported_rates[WLAN_SUPP_RATES_MAX];
	int supported_rates_len;
	u64 rr;  
	u64 tr;   //receive rate    transmit rate   byte
	u64 tp;   //throughput
	u32 snr;
	int vlan_id;
	unsigned int security_type;
	unsigned int nonerp_set:1;
	unsigned int no_short_slot_time_set:1;
	unsigned int no_short_preamble_set:1;
	unsigned int no_ht_gf_set:1;
	unsigned int no_ht_set:1;
	unsigned int ht_20mhz_set:1;
	unsigned int PreACIP;
	unsigned int BssIndex;
	unsigned int PreBssIndex;
	unsigned char PreBSSID[MAC_LEN];
	unsigned char bssid[MAC_LEN]; /* SSID selection based on (Re)AssocReq */
	unsigned char wtpmac[MAC_LEN];
	unsigned char roam_ap[MAC_LEN];
	unsigned char essid[ESSID_DEFAULT_LEN]; /* SSID selection based on (Re)AssocReq */
	time_t add_time;
	struct timeval start;
	struct timeval end;
	unsigned char sta_type;			/* 0-user 1-guest */
	unsigned char dev_type;
	unsigned char portal_type;		/* 0-wifidog */
	unsigned char portal_status;	/* 0-未认证 1-认证通过  2-待认证 */

	/*sta info single intensity etc.*/
	unsigned char mode;  //11b-0x01,11a-0x02,11g-0x04,11n-0x08,
	unsigned char channel;
	unsigned char rssi;
	unsigned short sta_delay;
	unsigned short nRate;   /*sta rx rate*/
	unsigned short txRate;
	unsigned char isQos;
	unsigned char info_channel;
	//struct auth_sta_info_t wapi_sta_info;
	unsigned char isPowerSave;
	unsigned char	vip_flag;	/*bit0 for up flag,bit1 for down flag*/
	unsigned int	sta_traffic_limit;		/*上行*//*指station 上传到AP的带宽限制 */
	unsigned int	sta_send_traffic_limit; /*下行*//*指station 从AP下载的带宽限制 */
	unsigned int	ipaddr;
	unsigned int	gifidx;
	char	arpifname[16];
	time_t add_time_sysruntime;
	time_t sta_online_time;
	unsigned int vir_ip;
	unsigned int realip;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	PORTAL_INFO portal_server;
};

struct sta_to_web
{
    unsigned int sta_num;
    struct stainfo_to_web *stainfo;
};
typedef struct sta_to_web STA_TO_WEB;

struct wtp_to_web
{
	u_int32_t	WTPID;
	char *WTPIP;
	u_int8_t *WTPMAC;
	char *WTPSN;					/* WTP Serial No */
	char *WTPNAME;
	char *WTPModel;					/* model of wtp which determin the number of Radios and rates supported in WTP */
	char *APCode;					/* inner develop code */
	char *codever;					/* used to recognize the version of oem production,so we can config the txantenna */
	char *updateversion;
	char *updatepath;
	u_int8_t updateStat;			//0 wait update 1 update 2 update success
	u_int8_t updatefailcount;		//0 no access 1 over max count 2 pkt error
	u_int8_t updatefailstate;		//0 wait update 1 update 2 update success
	u_int8_t ViaL3IF;				/* interface that Data of WTP come from */
	int wtp_login_mode;				/* static 0;dynamic 1 */
	u_int8_t WTPStat;				/* wtp state (online(1) /offline(0)) */
	u_int8_t BindL3IF;				/* force interface that Data of WTP come from */
	u_int8_t isused;
	u_int8_t quitreason;			/* quit reason */
	
	u_int8_t unused_flag;
	unsigned char cpu_per;
	unsigned char mem_per;
	unsigned char flash_per;
	unsigned char radio_num;
	u_int8_t	RadioCount;
	WID_WTP_RADIO WTP_Radio[L_RADIO_NUM];
	unsigned int  wtp_allowed_max_sta_num;
	unsigned int  wtp_accessed_sta_num;
	wlan_stats_info apstatsinfo[TOTAL_AP_IF_NUM]; /*ath 4 wifi 2 eth 2 total num 8*/
	ap_delay_info apdelayinfo;
	unsigned int uplink_bandwidth;
	unsigned int downlink_bandwidth;
	unsigned int tx_packets;
	unsigned int rx_packets;
	unsigned long long rx_bytes;		/*total rx byte for this ap*/
	unsigned long long tx_bytes;		/*total tx byte for this ap*/
	unsigned long long rx_bytes_before;	/*tmp total rx byte for this ap*/
	unsigned long long tx_bytes_before;	/*tmp total tx byte for this ap*/
	unsigned int ap_ipadd;
	unsigned int ap_mask_new;
	unsigned int ap_gateway;
	unsigned int ap_dnsfirst;
	unsigned int ap_dnssecend;
	unsigned int img_now_state;
	time_t	add_time;
	time_t	quit_time;
	time_t	imagedata_time;
	time_t	config_update_time;
	time_t	manual_update_time;
	unsigned int rate;
	unsigned int old_bytes;
	unsigned int RSSI;
	int apcodeflag;
	unsigned int elem_num;
	struct heart_time heart_time;   /*fengwenchao add 20111117 for GM-3*/
	struct _join5g join5g;
	struct wtp_lan_vlan lan_vlan;	/* ENR-23 */
	struct wtp_upgrade upgrade;
	wid_wtp_wlan_wifi wtp_wlan_wifi;
	WLANWEBINFO wlans[MAX_WLANID];
};
typedef struct wtp_to_web WID_WTP_TO_WEB;


typedef struct
{
	unsigned int wtpid;
	unsigned char type;
	unsigned int ip;
	unsigned int mask;
	unsigned int gateway;
	unsigned int fstdns;
	unsigned int snddns;
} IP_WIRE_ARG;

typedef struct
{
	unsigned int wtpid;
	unsigned char radioid;
	unsigned char wlanid;
	unsigned char wlan_enabled;
	unsigned char vlan_enabled;
	unsigned int vlanid;
	char security[NAME_LEN];
	char ssid[ESSID_DEFAULT_LEN];
	char user_key[USER_KEY_LEN];
} RADIO_WIRE_ARG;

int parse_country_code(char *input);
void hand_hex_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer);
void hand_hex_char_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer);

int hand_add_user_default_network
(
	char *ssid,
	char *key,
	DBusConnection *dcli_dbus_connection
);

int hand_add_guest_default_network
(
	char *ssid, 
	DBusConnection *dcli_dbus_connection
);


int hand_del_softAc_network
(
	int WlanId, 
	DBusConnection *dcli_dbus_connection
);


int hand_add_softAc_network
(
    WLANWEBINFO *wlaninfo,
	DBusConnection *dcli_dbus_connection
);


int hand_modify_softAc_network_config
(
    WLANWEBINFO *wlaninfo,
	DBusConnection *dcli_dbus_connection
);

int hand_load_conf_softAc_network_config
(
    WLANWEBINFO *wlaninfo,
	DBusConnection *dcli_dbus_connection
);

int show_network_wlan_config
(	
	WLANWEBINFO *wlanInfo,
	DBusConnection *dcli_dbus_connection
);

int show_network_sta_config
(	
	STA_TO_WEB *stawebInfo,
	DBusConnection *dcli_dbus_connection
);

int show_network_wtp_config
(
    WID_WTP_TO_WEB *wtpwebInfo,
    DBusConnection *dcli_dbus_connection
);

int show_network_wtp_ip_list
(	
	WID_WTP *wtpInfo,
	DBusConnection *dcli_dbus_connection
);

int show_network_ac_access_wtplist
(	
	WID_ACCESS *wtpaccInfo,
	DBusConnection *dcli_dbus_connection
);

int hand_add_softAc_user_group
(
	USERGRP_WEBINFO *usrgrpinfo,
	DBusConnection *dcli_dbus_connection
);


int hand_del_softAc_user_group
(
	unsigned int groupid,
	DBusConnection *dcli_dbus_connection
);


int hand_modify_softAc_user_group_config
(	
	USERGRP_WEBINFO *usrgrpinfo,
	DBusConnection *dcli_dbus_connection
);

int hand_load_conf_softAc_usrgrp_config
(
	USERGRP_WEBINFO *usrgrpinfo,
	DBusConnection *dcli_dbus_connection
);

void web_free_fun_access_wtp(WID_ACCESS *WTP_ACC_INFO);

int handlib_restart_wtp_by_id(int wtpid, DBusConnection *dcli_dbus_connection);

int handlib_delete_wtp_by_id(int wtpid, DBusConnection *dcli_dbus_connection);

int handlib_create_new_wtp(NEW_WTP_ARG * new_wtp, DBusConnection *dcli_dbus_connection);

int handlib_set_wtp_name(NEW_WTP_ARG * new_wtp, DBusConnection *dcli_dbus_connection);
int handlib_set_sta_name(struct stainfo_to_web *stainfo, DBusConnection *dcli_dbus_connection);
int handlib_dynamic_select_radio_channel(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection);
int handlib_set_radio_channel(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection);
int handlib_set_radio_txpower(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection);
int handlib_set_radio_cwmode(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection);
int handlib_set_radio_mode(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection);
int handlib_load_conf_new_wtp(NEW_WTP_ARG *new_wtp, DBusConnection *dcli_dbus_connection);
int show_network_country_code(int *country_code, DBusConnection *dcli_dbus_connection);
int set_network_country_code(unsigned int country_code, DBusConnection *dcli_dbus_connection);
int handlib_modify_ip_network_config(IP_WIRE_ARG *ip_config, DBusConnection *dcli_dbus_connection);
int handlib_overrides_radio_network_config(RADIO_WIRE_ARG *bss_config, DBusConnection *dcli_dbus_connection);

int hand_archive_alertinfo_by_activeid_apmac 
( 
	unsigned char *wtpmac, 
	int alertid, 
	DBusConnection *dcli_dbus_connection
);

int hand_get_ap_alert_ids 
( 
	struct alert_info* alert_list, 
	DBusConnection *dcli_dbus_connection
);

int hand_archive_all_alert
(
	DBusConnection *dcli_dbus_connection
);

int show_network_sta_config
(	
	STA_TO_WEB *stawebInfo,
	DBusConnection *dcli_dbus_connection
);

void *show_neighbor_ap_list_config
(
	DBusConnection *dbus_connection,
	int *wtp_num,
	int *ret
);

int record_update_status_by_apmac
(
	unsigned char *wtpmac,
	int upstat,
	DBusConnection *dcli_dbus_connection
);
void *get_neighbors_aps_info
(
	DBusConnection *dbus_connection, 
	int *ap_nums
);

void free_all_neighbor_ap
(
	CLIENT_INFO_HANDLE client_info, 
	int len
);


#endif

