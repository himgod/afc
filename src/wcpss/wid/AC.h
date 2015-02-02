#ifndef AC_AC_H
#define AC_AC_H

#define WID_WIFI_PATH		"/dev/wifi0"

void LISTEN_IF_INIT();

#define MAX_DATE_SIZE 1024

/*---for AC_ATTACH cache time---*/
#define MAX_SURVIVAL_TIME  60

#define WID_SECURITY_TYPE_GTK(security_type)	\
		((IEEE8021X == (security_type)) \
		|| (WPA_P == (security_type))	\
		|| (WPA2_P== (security_type))   \
		|| (WPA_E == (security_type))	\
		|| (WPA2_E== (security_type)))

#define WID_SECURITY_TYPE_PMK(security_type)	\
		((IEEE8021X == (security_type)) \
		|| (WPA_E == (security_type))	\
		|| (WPA2_E== (security_type)))

/* check pointer without return value */
#define WID_CHECK_POINTER(ptr)	\
	do {	\
		if (NULL == (ptr))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"parameter NULL\n",WAI_FUNC_LINE_LITERAL);	\
			return;	\
		}	\
	} while (0);
/* check pointer with return value ret */
#define WID_CHECK_POINTER_RET(ptr, ret)	\
	do {	\
		if (NULL == (ptr))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT" parameter NULL\n",WAI_FUNC_LINE_LITERAL); \
			return (ret); \
		}	\
	} while (0);



/* check WTP standard without return value */
#define WID_CHECK_WTP_STANDARD(_wtpid_)	\
	do {	\
		if (((_wtpid_) >= WTP_NUM) || ((_wtpid_) <= 0) || (NULL == AC_WTP[(_wtpid_)]))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"not exist\n",WAI_FUNC_LINE_LITERAL, (_wtpid_));	\
			return;	\
		}	\
	} while (0);
/* check WTP standard with return value */
#define WID_CHECK_WTP_STANDARD_RET(_wtpid_, ret)	\
	do {	\
		if (((_wtpid_) >= WTP_NUM) || ((_wtpid_) <= 0) || (NULL == AC_WTP[(_wtpid_)]))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"not exist\n",WAI_FUNC_LINE_LITERAL, (_wtpid_));	\
			return (ret);	\
		}	\
	} while (0);

/* check WTP standard with goto out */
#define WID_CHECK_WTP_STANDARD_GOTO(_wtpid_, ret, out) do {	\
		if (((_wtpid_) >= WTP_NUM) || ((_wtpid_) <= 0) || (NULL == AC_WTP[(_wtpid_)]))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"not exist\n",WAI_FUNC_LINE_LITERAL, (_wtpid_));	\
			ret = WTP_ID_NOT_EXIST; \
			{out;}	\
		}	\
	} while (0)


/* check wlan exist; without return value */
#define WID_CHECK_WLAN_EXIST(wlanid)	\
	do {\
		if (((wlanid) >= WLAN_NUM) || ((wlanid) <= 0) || (NULL == AC_WLAN[(wlanid)])||(0 == AC_WLAN[(wlanid)]->used))\
		{\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_WLAN_FMT"not exist\n",WAI_FUNC_LINE_LITERAL, (wlanid));\
			return;	\
		}\
	} while (0);

/* check wlan exist; with return value */
#define WID_CHECK_WLAN_EXIST_RET(wlanid, ret)	\
	do {	\
		if (((wlanid) >= WLAN_NUM) || ((wlanid) <= 0) || (NULL == AC_WLAN[(wlanid)]) || (0 == AC_WLAN[(wlanid)]->used))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_WLAN_FMT"not exist\n",WAI_FUNC_LINE_LITERAL, (wlanid));	\
			return (ret);	\
		}	\
	} while (0);

/* check wlan exist; with goto out */
#define WID_CHECK_WLAN_EXIST_GOTO(wlanid, ret, out)	\
	do {	\
		if (((wlanid) >= WLAN_NUM) || ((wlanid) <= 0) || (NULL == AC_WLAN[(wlanid)])||(0 == AC_WLAN[(wlanid)]->used))   \
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_WLAN_FMT"not exist\n",WAI_FUNC_LINE_LITERAL, (wlanid));   \
			ret = WLAN_ID_NOT_EXIST;    \
			{out;}   \
		}	\
	} while (0);

/* check bss standard without return value */
#define WID_CHECK_BSS_STANDARD(bssid)	\
	do {	\
		if (((bssid) >= BSS_NUM) || ((bssid) < L_RADIO_NUM * L_BSS_NUM))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max bss id %d, bssid %d\n", WAI_FUNC_LINE_LITERAL, (BSS_NUM-1), (bssid));	\
			return;	\
		}	\
	} while (0);
/* check bss standard with return value ret */
#define WID_CHECK_BSS_STANDARD_RET(bssid, ret)	\
	do {	\
		if (((bssid) >= BSS_NUM) || ((bssid) < L_RADIO_NUM * L_BSS_NUM))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max bss id %d, bssid %d\n", WAI_FUNC_LINE_LITERAL, (BSS_NUM-1), (bssid));	\
			return (ret);	\
		}	\
	} while (0);

/* check bss exist; without return value */
#define WID_CHECK_BSS_EXIST(bssid)	\
	do {	\
		if (NULL == AC_BSS[(bssid)])	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"no bss %d\n", WAI_FUNC_LINE_LITERAL, (bssid));	\
			return;	\
		}	\
	} while (0);
/* check bss exist; with return value */
#define WID_CHECK_BSS_EXIST_RET(bssid, ret)	\
	do {	\
		if (NULL == AC_BSS[(bssid)])	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"no bss %d\n", WAI_FUNC_LINE_LITERAL, (bssid));	\
			return (ret);	\
		}	\
	} while (0);



/* check g_radioid standard without return value */
#define WID_CHECK_G_RADIOID_STANDARD(g_radioid)	\
	do {	\
		if (((g_radioid) > G_RADIO_NUM) || ((g_radioid) < L_RADIO_NUM) || (NULL == AC_RADIO[(g_radioid)]))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max g_radio id %d, g_radioid %d\n", WAI_FUNC_LINE_LITERAL, G_RADIO_NUM, (g_radioid));	\
			return;	\
		}	\
	} while (0);

/* check g_radioid standard with return value ret */
#define WID_CHECK_G_RADIOID_STANDARD_RET(g_radioid, ret)	\
	do {	\
		if (((g_radioid) > G_RADIO_NUM) || ((g_radioid) < L_RADIO_NUM) || (NULL == AC_RADIO[(g_radioid)]))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max g_radio id %d, g_radioid %d\n", WAI_FUNC_LINE_LITERAL, G_RADIO_NUM, (g_radioid));	\
			return (ret);	\
		}	\
	} while (0);

/* check g_radioid standard with return goto out */
#define WID_CHECK_G_RADIOID_STANDARD_GOTO(g_radioid, ret, out)	\
	do {	\
		if (((g_radioid) > G_RADIO_NUM) || ((g_radioid) < L_RADIO_NUM) || (NULL == AC_RADIO[(g_radioid)]))   \
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max g_radio id %d, g_radioid %d\n", WAI_FUNC_LINE_LITERAL, G_RADIO_NUM, (g_radioid));  \
			ret = RADIO_ID_NOT_EXIST;   \
			{out;}	\
		}	\
	} while (0);

/* check l_radioid standard without return value */
#define WID_CHECK_L_RADIOID_STANDARD(l_radioid)	\
	do {	\
		if (((l_radioid) >= L_RADIO_NUM))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max l_radio id %d, l_radioid %d\n", WAI_FUNC_LINE_LITERAL, (L_RADIO_NUM - 1), (l_radioid));	\
			return;	\
		}	\
	} while (0);

/* check l_radioid standard with return value ret */
#define WID_CHECK_L_RADIOID_STANDARD_RET(l_radioid, ret)	\
	do {	\
		if (((l_radioid) >= L_RADIO_NUM))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max l_radio id %d, l_radioid %d\n", WAI_FUNC_LINE_LITERAL, (L_RADIO_NUM - 1), (l_radioid));	\
			return (ret);	\
		}	\
	} while (0);

/* check l_radioid standard with goto out */
#define WID_CHECK_L_RADIOID_STANDARD_GOTO(l_radioid, ret, out)	\
	do {	\
		if (((l_radioid) >= L_RADIO_NUM))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"max l_radio id %d, l_radioid %d\n", WAI_FUNC_LINE_LITERAL, (L_RADIO_NUM - 1), (l_radioid));	\
			ret = RADIO_ID_NOT_EXIST;	\
			{out;}	\
		}	\
	} while (0);

/* check user group exist; without return value */
#define WID_CHECK_USER_GROUP_EXIST(usergrpid)	\
	do {\
		if (((usergrpid) >= USER_GROUP_NUM) || ((usergrpid) <= 0) || (NULL == USER_GROUP[(usergrpid)]) || (0 == USER_GROUP[(usergrpid)]->used))\
		{\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_USERGRP_FMT"not exist\n", WAI_FUNC_LINE_LITERAL, (usergrpid));\
			return;	\
		}\
	} while (0);

/* check user group exist; with return value */
#define WID_CHECK_USER_GROUP_EXIST_RET(usergrpid, ret)	\
	do {	\
		if (((usergrpid) >= USER_GROUP_NUM) || ((usergrpid) <= 0) || (NULL == USER_GROUP[(usergrpid)]) || (0 == USER_GROUP[(usergrpid)]->used))	\
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_USERGRP_FMT"not exist\n", WAI_FUNC_LINE_LITERAL, (usergrpid));	\
			return (ret);	\
		}	\
	} while (0);

/* check user group exist; with goto out */
#define WID_CHECK_USER_GROUP_EXIST_GOTO(usergrpid, ret, out)	\
	do {	\
		if (((usergrpid) >= USER_GROUP_NUM) || ((usergrpid) <= 0) || (NULL == USER_GROUP[(usergrpid)]) || (0 == USER_GROUP[(usergrpid)]->used))   \
		{	\
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_USERGRP_FMT"not exist\n", WAI_FUNC_LINE_LITERAL, (usergrpid));   \
			ret = GROUP_ID_NOT_EXIST;    \
			{out;}   \
		}	\
	} while (0);


int get_dir_wild_file_count(char *dir, char *wildfile);
int wid_illegal_character_check(char *str , int len, int modify);


#define WID_MALLOC_ERR()	\
	do {	\
			wid_syslog_err(WAI_FUNC_LINE_FMT"malloc failed: %s\n", WAI_FUNC_LINE_LITERAL, strerror(errno));	\
	} while (0);

#define LOGOUT_MEMORY()	\
	do {	\
		char __cmd[128];	\
		char __buf[256];	\
		FILE *fp = NULL;	\
		memset(__buf, 0, sizeof(__buf));	\
		memset(__cmd, 0, sizeof(__cmd));	\
		if (vrrid)	\
			sprintf(__cmd, "ps aux | grep \"wid %d\" | grep -v grep | tail -1", vrrid);	\
		else	\
			sprintf(__cmd, "ps aux | grep \"wid\" | grep -v grep | tail -1");	\
			\
		fp = popen(__cmd, "r");	\
		if (fp)	\
		{	\
			fgets(__buf, 256, fp);	\
			__buf[255] = '\0';	\
			wid_syslog_info(WAI_FUNC_LINE_FMT"%s\n", WAI_FUNC_LINE_LITERAL, __buf);	\
			pclose(fp);	\
		}	\
	} while (0);

#define destroy_sta_info(sta)	\
	do {	\
        CW_FREE_OBJECT(sta->in_addr);   \
        CW_FREE_OBJECT(sta->add_time);  \
        CW_FREE_OBJECT(sta->challenge);  \
        CW_FREE_OBJECT(sta);  \
	} while (0);


#define WID_INVALID_MODEL_WTP	"invalid-model-wtp"

CWBool AsdWsm_StationOpNew(unsigned int WTPID,unsigned char *mac, Operate op,unsigned short reason);
CWBool AsdWsm_WTPOp(unsigned int WtpID,Operate op);
CWBool AsdWsm_RSSIOp(Operate op);
CWBool AsdWsm_BssMacOp(unsigned int BSSIndex, Operate op);
CWBool AsdWsm_BSSOp(unsigned int BSSIndex, Operate op, int both);
CWBool AsdWsm_bytes_info(unsigned int WTPID, Operate op);
CWBool WidAsd_StationInfoUpdate1(unsigned int WTPID, WIDStationInfo *valuesPtr);
CWBool WidAsd_StationLeaveReport(unsigned int WTPID, unsigned int count, WIDStationInfo *valuesPtr);
CWBool WidAsd_StationKeyNegotiReport(unsigned int WTPID, WIDStationInfo *valuesPtr);
int WIDBAKInfoToASD(unsigned int state,struct sockaddr_in *ipaddr, unsigned int virIP,unsigned int vrrid,char *name, Operate op);
int WIDLocalBAKInfoToASD(unsigned int state,int slotID,unsigned int vrrid, Operate op);
CWBool Asd_neighbor_ap_sta_check_op(unsigned int WtpID,unsigned int N_WtpID,unsigned char mac[6],Operate op);
int wid_del_wtp(struct wtp_con_info * con_info);
CWBool AsdWsm_BssMacOp(unsigned int BSSIndex, Operate op);
struct conflict_wtp_info* wid_add_wtp(struct wtp_con_info * con_info);
struct conflict_wtp_info * wid_del_conflict_wtpinfo(struct wtp_con_info * con_info);
void CWResetWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager);
int WIDBAKBSSInfoToASD(unsigned int vrrid,unsigned int *bssindex,unsigned int count,Operate op);
int file_check(char *dir);
extern unixAddr toASD_STA;

extern WID_VLAN_GROUP  *VLAN_GROUP[VLAN_GROUP_NUM+1];

unsigned int get_file_size(const char *path);
int md5_hash(char *path, unsigned char *md5);
unsigned long int HexToDec(unsigned char *hex);
__inline__ unsigned int CWGetWtpSeqNum(unsigned int wtpid);

unsigned int wid_to_asd_dynamic_vlan_state(unsigned int wlanid,unsigned int enable);
unsigned int wid_to_asd_dynamic_vlan_list(unsigned int wlanid);
struct sta_info *ap_get_sta(unsigned int bssindex, const u8 *sta);
void ap_sta_hash_add(unsigned int bssindex, struct sta_info *sta);
void ap_sta_hash_del(unsigned int bssindex, struct sta_info *sta);
void ap_sta_list_del(unsigned int bssindex, struct sta_info *sta);
void ap_free_sta(unsigned int bssindex, struct sta_info *sta);
void asd_free_stas(unsigned int bssindex);
struct asd_stainfo *wid_check_sta_roam(WID_STAINFO *sta);
void wid_check_sta_type(unsigned int bssindex, unsigned int *sta_type_num);
void get_sysruntime(time_t *sysruntime);

#endif
