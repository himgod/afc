/*******************************************************************************
Copyright (C) Autelan Technology


This software file is owned and distributed by Autelan Technology 
********************************************************************************


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
* AC.c
*
*
* CREATOR:
* autelan.software.wireless-control. team
*
* DESCRIPTION:
* wid module
*
*
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>

#include <ctype.h>
#include <syslog.h>
#include <time.h>
#include <dirent.h>
#include "CWAC.h"
#include "wcpss/waw.h"
#include "wcpss/wid/WID.h"
#include "wcpss/asd/asd.h"
#include "CWCommon.h"
#include "ACDbus.h"
#include "dbus/wcpss/ACDBusPath.h"
#include "dbus/asd/ASDDbusDef1.h"
#include "ACNetlink.h"
#include "ACBak.h"
#include "ACDbus_handler.h"
#include "AC.h"
#include "os.h"
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

#define SOFTACLICENSE	5

/*_________________________________________________________*/
/*  *******************___VARIABLES___*******************  */
CWThreadMutex gCreateIDMutex;

CWWTPManager *gWTPs; 	//stores info about our WTPs
CWThreadMutex gWTPsMutex;
CWThreadMutex AttachMutex;
CWThreadMutex MasterBak;
CWThreadMutex ACAccessWTP;
CWThreadMutex ACLicense;	//add for license control


int gEnabledLog;
int gMaxLogFileSize;


int gActiveWTPs = 0;			//number of active WTPs
int gActiveWTPs_Autelan = 0;
int gActiveWTPs_OEM = 0;
int gStaticWTPs = 0;
int scanningWTPs = 0;
int scanningWTPs1 = 0;
unsigned int isNoCheck = 0;

CWThreadMutex gActiveWTPsMutex;
CWThreadMutex gAllThreadMutex;//zhanglei add for wlan op
CWThreadMutex gACInterfaceMutex;//zhanglei add for wlan op
CWThreadCondition gACInterfaceWait;//zhanglei add for wlan op
CWThreadCondition gInterfaceComplete;//zhanglei add for wlan op
CWThreadMutex gSTARoamingMutex;//zhanglei add for sta roaming op
CWThreadCondition gSTARoamingWait;//zhanglei add for sta roaming op

CWThreadMutex gACChannelMutex;//zhanglei for dynamic channel selection
CWThreadCondition gACChannelWait;//zhanglei for dynamic channel selection
CWThreadCondition gChannelComplete;//zhanglei for dynamic channel selection

CWThreadMutex gACChannelMutex2;//zhanglei for dynamic channel selection
CWThreadCondition gACChannelWait2;//zhanglei for dynamic channel selection
CWThreadCondition gChannelComplete2;//zhanglei for dynamic channel selection

CWThreadMutex gOuiMacXmlMutex;//zhaoruijia add for black&whiteouilist.xml

CWThreadMutex gACTxpowerMutex;
CWThreadCondition gACTxpowerWait;
CWThreadCondition gTxpowerComplete;

int gMaxWTPs = 1024;  				//max WTPs
int gRMACField = 0;			//The Radio MAC Field of the discovery response
int gWirelessField = 0;			//The Wireless Field of the discovery response
int gDTLSPolicy=DTLS_ENABLED_DATA;	//DTLS Policy for data channel
int wAWSocket = 0;
int sockPerThread[SOCK_NUM] ={0,0}; /*wuwl add */
int WidMsgQid;
int WidAllQid;
int ASD_WIDMSGQ;
unixAddr toASD;
unixAddr toASD_STA;
unixAddr toWSM;
CWMultiHomedSocket gACSocket;		//special socket to handle multiple network interfaces
CWProtocolNetworkInterface *gInterfaces = NULL;// AC's network interfaces
CWMultiHomedSocket gListenningIF;		//special socket to handle multiple network interfaces
int gInterfacesCount = 0;
int gInterfacesCountIpv4 = 0;
int gInterfacesCountIpv6 = 0;
struct wid_wtp_info allif;
char first_conflict = 1;
int gMaxInterfacesCount = 72;
#ifndef CW_NO_DTLS
CWSecurityContext gACSecurityContext;	// DTLS Context
#endif
int gActiveStations = 0;
int gLimit;  				// max stations
char **gMulticastGroups;
int gMulticastGroupsCount;
CWAuthSecurity gACDescriptorSecurity;
int gACHWVersion;
int gACSWVersion;
char gdefaultsn[DEFAULT_SN_LENTH] = "00000000000000000000";
char gdefaultMac[MAC_LEN] = {0x00,0x00,0x00,0x00,0x00,0x00};
char *gACName = NULL;
char *gACHWVersion_char = NULL;
char *gACSWVersion_char = NULL;
/*fengwenchao add 20120117 for onlinebug-96*/
int gwtpstate_mb = 300;
int gwtpstate_mb_timer = 0;  //the timer's name
/*fengwenchao add end*/
//int gCOUNTRYCODE = COUNTRY_USA_US;//now,system default is US
int gCOUNTRYCODE = COUNTRY_CHINA_CN;//wuwl change default country code to CN 
/****************globle variable*****************/

/* REQUIREMENTS-536/ENR4-2 for 5G-FIRST */
int gWTP_JOIN5G_SWITCH          = WTP_JOIN5G_SWITCH_DISABLE;
int gWTP_JOIN5G_AGING_TIME      = WTP_JOIN5G_AGING_TIME_DEF;
int gWTP_JOIN5G_DISCARD_COUNT   = WTP_JOIN5G_DISCARD_COUNT_DEF;
int gWTP_JOIN5G_SCAN_TIME       = WTP_JOIN5G_SCAN_TIME_DEF;
int gWTP_JOIN5G_STA_COUNT       = WTP_JOIN5G_STA_COUNT_DEF;
int gWTP_JOIN5G_SIGNAL_STRENGTH = WTP_JOIN5G_SIGNAL_STRENGTH_DEF;

int gINFOREPORTSWITCH = 0;
unsigned short gWIFIEXTENSIONREPORTINTERVAL = 1800;
unsigned short gINFOREPORTINTERVAL = 1800;
int gSTAINFOREPORT = 0;
int gSTAREPORTSWITCH = 0;
unsigned short gSTAREPORTINTERVAL = 1800;
int gAPIFINFOETH_MTU[AP_ETH_IF_NUM] ={1500,1500};
int gAPIFINFOETH_RATE[AP_ETH_IF_NUM] ={100,100};
int gBANDWIDTH = 108;
int gAP_STA_WAPI_REPORT_SWITCH = 0;
unsigned int gAP_STA_WAPI_REPORT_INTERVAL = 1800;
int gTER_DIS_INFOREPORTPKT = 0;
int gTER_DIS_INFOSTA_TRAP_COUNT = 2;
int gTER_DIS_INFOREPORTSWITCH = 0;
int gNEIGHBORCHANNELRSSITHOLD = -70;
int gSAMECHANNELRSSITHOLD = -80;
int gWTP_ROGUE_AP_THRESHOLD = 10;
int gWTP_ROGUE_TERMINAL_THRESHOLD = 1;
int gWTP_CPU_USE_THRESHOLD = 10;
int gWTP_MEM_USE_THRESHOLD = 30;
int gDHCP_SNOOPING = 0;
int gWTP_FLOW_TRIGER = 0;
int gWTP_MAX_STA = DEFAULT_WTP_MAX_STA_NUM;
int gNTP_STATE = 1;
int gNTP_INTERVAL = 3600;

char gFlow_balance_para = 20;

int gWLAN_SEND_TRAFFIC_LIMIT = 0;
int gWLAN_STATION_AVERAGE_TRAFFIC_LIMIT = 0;
int gWLAN_STATION_AVERAGE_SEND_TRAFFIC_LIMIT = 0;
int gWLAN_TRAFFIC_LIMIT = 0;
int gWLAN_MAX_ALLOWED_STA_NUM = 65536;
int gWLAN_MAX_ALLOWED_STA_NUM_FOR_BSS = 128;
unsigned char  gWLAN_ATH_L2_ISOLATION = 0;
sta_static_arp gWLAN_STA_STATIC_ARP_POLICY;
unsigned char gWLAN_LIMIT_STA_RSSI = 0;
ap_uni_muti_bro_cast gWLAN_UNI_MUTI_BRO_CAST;
u_int8_t	g_WLAN_TUNNEL_POLICY = CW_802_DOT_11_TUNNEL;

STA_ACL *STA_ACL_RULE[ACL_RULE_NUM];			/*ENR-17*/
STA_ACLGRP *STA_ACL_GROUP[ACL_GROUP_NUM];	/*ENR-17*/

/****************globle variable*****************/


int gDiscoveryTimer = 20;
int gEchoRequestTimer = CW_ECHO_INTERVAL_DEFAULT;
int gCheckRequestTimer = CW_ECHO_INTERVAL_DEFAULT;
int gIdleTimeout = 10; 	//PROVVISORIO: Il valore è scelto a caso

WID_CONFSCHEME *AC_CONFSCHME[CONFSCHEME_NUM];
WID_WLAN	*AC_WLAN[WLAN_NUM];
WID_WTP		**AC_WTP;
WID_WTP_RADIO	**AC_RADIO;
WID_BSS		**AC_BSS;
CWWTPAttach **AC_ATTACH;
WID_ACCESS	*AC_WTP_ACC;
WID_USER_GROUP *USER_GROUP[USER_GROUP_NUM];
WID_WTP_GROUP *WTP_GROUP[WTP_GROUP_NUM];
WID_MAC_GROUP *MAC_GROUP[MAC_GROUP_NUM];	// add for electronic-menu
int WTP_NUM = 16;	
int WTP_NUM_AUTELAN = 0;
int WTP_NUM_OEM = 0;
int G_RADIO_NUM	= 0;
int BSS_NUM	= 0;
LICENSE_TYPE **g_wtp_count = NULL;
LICENSE_TYPE **g_wtp_binding_count = NULL;
int glicensecount = 9;

int WidSemid;
int channel_state = 0;
int txpower_state = 0;
unsigned char channelRange[4] = {1,6,11,0};

int tx_wtpid = 0;
unsigned char control_scope = 0;
unsigned char coverage_threshold = 10;
unsigned char tx_power_threshold = 30;
unsigned char neighborrogueapcount = 10;
unsigned char power_constant = 17;
unsigned char tx_power_max = 23;
int g_AUTO_AP_LOGIN_SWITCH = 0;
int g_AC_ALL_EXTENTION_INFORMATION_SWITCH ;//nl add
char* g_AUTO_AP_LOGIN_BINDING_L3_INTERFACE = NULL;
int g_interface_state = 0;
int g_AUTO_AP_LOGIN_BINDING_WLANID = 0;
int g_AUTO_AP_LOGIN_SAVE_CONFIG_SWITCH = 0;
wid_auto_ap_info	g_auto_ap_login;
ac_balance_flag		ac_flow_num_balance_flag;
AC_QOS *WID_QOS[QOS_NUM];
WID_STA STA_ROAM;
unsigned int receiver_signal_level = 29;
wid_sample_info WID_SAMPLE_INFORMATION = {200,2,0,0};
ETHEREAL_BRIDGE *WID_EBR[EBR_NUM];
unsigned int g_ap_cpu_threshold = 10;
unsigned int g_ap_memuse_threshold = 30;
unsigned int g_ap_temp_threshold = 40;
CWThread WidThread[THREAD_NUM];
struct ifi *WID_IF = NULL;
struct ifi *WID_IF_V6 = NULL;
wid_ac_ip_group *AC_IP_GROUP[ACIPLIST_NUM];
CWThreadMutex	ACIPLISTMutex;
pthread_t WID_BAK;
pthread_t WID_MASTER;
struct sockaddr M_addr;
struct sockaddr B_addr;
struct sockaddr Lic_Active_addr;
struct sockaddr Lic_bak_addr;
struct lic_ip_info Lic_ip;
int lic_bak_fd = 0;
int lic_active_fd = 0;
struct bak_sock *bak_list = NULL;
unsigned int neighbor_slotid = 0;
char is_secondary = 2;
unsigned int vrrid = 0;
unsigned int slotid = 0;
unsigned int local = 1;
char secondary_AC_cmd[80]={0};
CWACThreadArg_clone BakArgPtr;
unsigned int img_now = 0;
unsigned int G_LocalHost_num;
unsigned char WID_WATCH_DOG_OPEN = 0;
int gloadbanlance = 0;
int havecreatethread = 0;
unsigned char wtp_link_detect = 0;
unsigned int wsmswitch = 0;
unsigned char vlanSwitch = 0;
unsigned char DhcpOption82Switch = 0;
int multicast_listen_state = 0;
//unsigned int vrrid;
int wid_bak_sock = 0;
CWTimerID	bak_check_timer;
CWTimerID	Lic_bak_req_timer;
int BakCheckInterval = 60;
int LicBakReqInterval = 60;
unsigned int g_ap_auto_update_service_tftp = 0;//service tftp (0--disable;1--enable)
unsigned int g_service_ftp_state = 0;//service ftp  (0--disable;1--enable)
enum wtp_upgrade_mode g_wtp_upgrade_mode = WTP_UPGRADE_MODE_HTTP;
struct ap_update_config  g_ap_udapte_config;

int rrm_rid = 0;
/************************Qos mapping***********************/
unsigned char QOS_AC_11E_TO_1P_DEFAULT[MAX_QOS_11E_VAL+1] = {0,1,2,3,4,5,6,7};
unsigned char QOS_AC_DSCP_TO_1P_DEFAULT[MAX_QOS_DSCP_VAL+1] = {0,0,0,0,0,0,0,0,0,0,\
															   1,1,1,1,1,1,2,2,2,2,\
															   2,2,2,2,3,3,3,3,3,3,\
															   3,3,4,4,4,4,4,4,4,4,\
															   4,4,4,4,4,4,5,5,6,6,\
															   6,6,6,6,6,6,7,7,7,7,\
															   7,7,7,7};

unsigned char QOS_AC_DSCP_TO_DSCP_DEFAULT[MAX_QOS_DSCP_VAL+1] = {0,1,2,3,4,5,6,7,8,9,\
															   10,11,12,13,14,15,16,17,18,19,\
															   20,21,22,23,24,25,26,27,28,29,\
															   30,31,32,33,34,35,36,37,38,39,
															   40,41,42,43,44,45,46,47,48,49,
															   50,51,52,53,54,55,56,57,58,59,
															   60,61,62,63};
unsigned char QOS_AP_11E_TO_1P_DEFAULT[MAX_QOS_11E_VAL+1] = {0,1,2,3,4,5,6,7};
unsigned char QOS_AP_11E_TO_DSCP_DEFAULT[MAX_QOS_11E_VAL+1] = {0,8,10,16,24,32,46,48};
unsigned char QOS_AP_DSCP_TO_11E_DEFAULT[MAX_QOS_DSCP_VAL+1] = {0,0,0,0,0,0,0,0,1,1,\
															   2,2,2,2,2,2,3,3,3,3,\
															   3,3,3,3,4,4,4,4,4,4,\
															   4,4,5,5,5,5,5,5,5,5,\
															   5,5,5,5,5,5,6,6,7,7,\
															   7,7,7,7,7,7,7,7,7,7,\
															   7,7,7,7};
unsigned char QOS_AP_1P_TO_11E_DEFAULT[MAX_QOS_1P_VAL+1] = {0,1,2,3,4,5,6,7};
/**********************end Qos mapping *********************/
/******************************dynamic vlan *****************/
WID_VLAN_GROUP	*VLAN_GROUP[VLAN_GROUP_NUM+1] = {NULL};
/*******************************end ************************/

/*_________________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */

char* wai_radio_type_str[] = {
	"nil","b","a","nil","g","b/g","nil","nil","n", /* 0-8 */
	"nil","a/n","nil","g/n","b/g/n","nil","nil", "nil"/* 9-16 */
};

int wai_radio5g_ch_default_arr[] =  {
	WAI_RADIO5G_DEFAULT_CHANN_CN,
	WAI_RADIO5G_DEFAULT_CHANN_EU,
	WAI_RADIO5G_DEFAULT_CHANN_US,
	WAI_RADIO5G_DEFAULT_CHANN_JP,
	WAI_RADIO5G_DEFAULT_CHANN_FR,
	WAI_RADIO5G_DEFAULT_CHANN_SPAIN
};
int wai_radio2g_ch_default_arr[] =  {
	WAI_RADIO2G_DEFAULT_CHANN_CN,
	WAI_RADIO2G_DEFAULT_CHANN_EU,
	WAI_RADIO2G_DEFAULT_CHANN_US,
	WAI_RADIO2G_DEFAULT_CHANN_JP,
	WAI_RADIO2G_DEFAULT_CHANN_FR,
	WAI_RADIO2G_DEFAULT_CHANN_SPAIN
};

int main (int argc, const char * argv[])
{
	pthread_key_create(&thread_log_key, wid_close_thread_log);
	wid_thread_log("main");

	if(argc > 2)
	{
		local =  atoi(argv[1]);
		vrrid =  atoi(argv[2]);
	}
	else if(argc != 1)
	{
		wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"argc %d, something wrong\n", \
					WAI_FUNC_LINE_LITERAL, argc);
		return 0;
	}
	
	wid_pid_write(vrrid);
	wid_pid_write_v2("main",0,vrrid);
	CWACInit();
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init interface listening for auth login\n", WAI_FUNC_LINE_LITERAL);
	set_auto_ap_login_listen_interface();

	/*test */
//	char uSSID[] = "AFSUNUSER";
//	char gSSID[] = "AFSUNGUEST";
//	char oSSID[] = "OPENTEST";
//	char wSSID[] = "WEPTEST";
//	char wpSSID[] = "WPATEST";
	
//	char uKey[] = "12345678";
//	create_user_wlan(uSSID, uKey);
//	create_guest_wlan(gSSID);
//	add_open_wireless_network(oSSID, 0, 1, 0, 0, 0);
//	add_wep_wireless_network(wSSID, uKey, 0, 1, 0, 0, 0);
//	unsigned int wid = 3;
//	del_wpae_wireless_network(wid);
//	add_wep_wireless_network(wSSID, uKey, 0, 1, 0, 0, 0);
//	add_wpap_wireless_network(wpSSID, uKey,0, 1, 0, 0, 0, WPA_P, AES);
	
	CWACEnterMainLoop(); 
	
	CWACDestroy();  
	
	return 0;
}

int CWACSemPostForOpenSSLHack(void *s)
{
	CWThreadTimedSem *semPtr = (CWThreadTimedSem*) s;
	
	if(!CWThreadTimedSemIsZero(semPtr))
	{
		wid_syslog_err("This Semaphore's Value should really be 0");
		// note: we can consider setting the value to 0 and going on,
		// that is what we do here
		if(!CWErr(CWThreadTimedSemSetValue(semPtr, 0))) return 0;
	}
	
	if(!CWErr(CWThreadTimedSemPost(semPtr)))
	{
		return 0;
	}
	 
	return 1;
}

void license_binding_init(char *string)
{
	unsigned char type = 1;
	int i = 0;
	int j = 0;
	unsigned int flag = 0;
	//char *string = NULL;
	int strlength = 0;
	int *licenselist = NULL;

	strlength = strlen(string);
	/*process license type list*/
	licenselist = (int *)malloc((glicensecount+1)*sizeof(int));
	for(i = 0, j = 0; i < strlength;i++)
	{
		if(string[i]!=',')
		{
			licenselist[j] = string[i]-'0';
			for(; string[i+1]!=','&&string[i+1]!='\0'; i++)
			{
				licenselist[j] = licenselist[j]*10+string[i+1]-'0';
			}
			j++;
		}
	}
	/*set license LICENSELIST binding*/
	if(type == 1)
	{
		//binding license type
		for(flag = 1; flag < glicensecount+1; flag++)
		{
			if(g_wtp_binding_count[flag] == NULL)
			{
				g_wtp_binding_count[flag] = malloc(sizeof(LICENSE_TYPE));
				break;
			}
		}
		for(i = 0; i < j; i++)
		{
			g_wtp_count[licenselist[i]-1]->flag = flag;
		}
		g_wtp_binding_count[flag]->flag = flag;
		g_wtp_binding_count[flag]->gcurrent_wtp_count = 0;
		g_wtp_binding_count[flag]->gmax_wtp_count = 0;
		for(i = 0; i < j; i++)
		{
			g_wtp_binding_count[flag]->gcurrent_wtp_count += g_wtp_count[licenselist[i]-1]->gcurrent_wtp_count;
			g_wtp_binding_count[flag]->gmax_wtp_count += g_wtp_count[licenselist[i]-1]->gmax_wtp_count;
		}
	}
	CW_FREE_OBJECT(licenselist);
}

void license_binding_Reinit(char *string)
{
	unsigned char type = 1;
	int i = 0, j = 0;
	unsigned int flag = 0;
	//char *string = NULL;
	int strlength = 0;
	int *licenselist = NULL;

	strlength = strlen(string);
	/*process license type list*/
	licenselist = (int *)malloc((glicensecount+1)*sizeof(int));
	for(i = 0, j = 0; i < strlength; i++)
	{
		if(string[i]!=',')
		{
			licenselist[j] = string[i]-'0';
			for(; string[i+1]!=','&&string[i+1]!='\0'; i++)
			{
				licenselist[j] = licenselist[j]*10+string[i+1]-'0';
			}
			j++;
		}
	}
	/*set license LICENSELIST binding*/
	if(type == 1)
	{
		//binding license type
		for(flag = 1; flag < glicensecount+1; flag++)
		{
			if(g_wtp_binding_count[flag] == NULL)
			{
				//g_wtp_binding_count[flag] = malloc(sizeof(LICENSE_TYPE));
				break;
			}
		}
		for(i = 0; i < j; i++)
		{
			g_wtp_count[licenselist[i]-1]->flag = flag;
		}
		g_wtp_binding_count[flag]->flag = flag;
		g_wtp_binding_count[flag]->gcurrent_wtp_count = 0;
		g_wtp_binding_count[flag]->gmax_wtp_count = 0;
		for(i = 0; i < j; i++)
		{
			g_wtp_binding_count[flag]->gcurrent_wtp_count+=g_wtp_count[licenselist[i]-1]->gcurrent_wtp_count;
			g_wtp_binding_count[flag]->gmax_wtp_count+=g_wtp_count[licenselist[i]-1]->gmax_wtp_count;
		}
	}
	
	CW_FREE_OBJECT(licenselist);
	wid_syslog_debug_debug(WID_DEFAULT,"set license binding reinit.\n");
}

void CWACInit() 
{
	char indent[WID_LOG_INDENT_LEN];
	memset(indent, 0, sizeof(indent));
	snprintf(indent, WID_LOG_INDENT_LEN-1, "wid%d", vrrid);

	if(local)
	{	
		snprintf(indent, WID_LOG_INDENT_LEN-1, "wid%d-%d", slotid,vrrid);
	}
	else
	{
		snprintf(indent, WID_LOG_INDENT_LEN-1, "widR%d-%d", slotid,vrrid);	
	}
	openlog(indent, 0, LOG_DAEMON);

	CWWIDInit();	
	CWWIDDbusPathInit();
	//CWLogInitFile(LOG_FILE_NAME);
	#ifndef CW_SINGLE_THREAD
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"found system use single threads\n", \
				WAI_FUNC_LINE_LITERAL);
	#else
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"found system use multi threads\n", \
				WAI_FUNC_LINE_LITERAL);
	#endif
	
	CWErrorHandlingInitLib();
	
	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);
	if (timer_init() == 0)
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"threads malfunction when init timer we quit the process\n", \
					WAI_FUNC_LINE_LITERAL);
		exit(1);
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"controller function...\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"controller function...\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWParseConfigFile1())	||
		//!CWErr(CWParseConfigVersionInfo())||
		//!CWErr(CWParseConfigVersionInfoXML(&gConfigVersionInfo))||
		//!CWErr(CWParseBlackOuiInfoXML(&gBlackOuiInfoList))||
		//!CWErr(CWParseWhiteOuiInfoXML(&gWhiteOuiInfoList))||
#ifndef CW_NO_DTLS
		!CWErr(CWSecurityInitLib())	||
#endif
		!CWErr(CWwAWInitSocket(&wAWSocket))   ||
		!CWErr(CWCreateThreadMutex(&gWTPsMutex))	||
		!CWErr(CWCreateThreadMutex(&AttachMutex))	||
		!CWErr(CWCreateThreadMutex(&MasterBak))		||
		!CWErr(CWCreateThreadMutex(&ACAccessWTP))	||
		!CWErr(CWCreateThreadMutex(&ACLicense)) 	||
		!CWErr(CWCreateThreadMutex(&gActiveWTPsMutex))	  ||
		!CWErr(CWCreateThreadMutex(&gAllThreadMutex))	||
		!CWErr(CWCreateThreadMutex(&gACInterfaceMutex))||
		!CWErr(CWCreateThreadMutex(&gACChannelMutex))||//zhanglei for dynamic channel selection
		!CWErr(CWCreateThreadMutex(&gACChannelMutex2))||//zhanglei for dynamic channel selection
		!CWErr(CWCreateThreadMutex(&gSTARoamingMutex))||
		!CWErr(CWInitMsgQueue(&WidMsgQid))||
		!CWErr(CWGetMsgQueue(&WidAllQid))||
		!CWErr(CWCreateThreadMutex(&ACIPLISTMutex)) ||//zhanglei add for ac ip list
		!CWErr(CWCreateThreadMutex(&gACTxpowerMutex))||
		!CWErr(CWCreateThreadMutex(&gOuiMacXmlMutex))
	)  // error starting
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"start controller function failed we quit the process\n", \
						WAI_FUNC_LINE_LITERAL);
		exit(1);
	}

	/* book add loading 11n rates xml, 2011-11-08 */
    if(!CWErr(CWInit11nRateTable()))
    {
    	wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"load 802.11n rate table failed\n", \
						WAI_FUNC_LINE_LITERAL);
    }
    
	wid_sock = init_wid_bak_socket();
	CWCreateThreadCondition(&gACInterfaceWait);
	CWCreateThreadCondition(&gInterfaceComplete);	
	CWCreateThreadCondition(&gSTARoamingWait);
	CWCreateThreadCondition(&gACChannelWait);//zhanglei for dynamic channel selection
	CWCreateThreadCondition(&gChannelComplete);//zhanglei for dynamic channel selection
	CWCreateThreadCondition(&gACChannelWait2);//zhanglei for dynamic channel selection
	CWCreateThreadCondition(&gChannelComplete2);//zhanglei for dynamic channel selection
	
	CWCreateThreadCondition(&gACTxpowerWait);
	CWCreateThreadCondition(&gTxpowerComplete);

#ifndef CW_NO_DTLS
	if(gACDescriptorSecurity == CW_X509_CERTIFICATE)
	{
		if(!CWErr(CWSecurityInitContext(&gACSecurityContext, "root.pem", "server.pem", "prova", CW_FALSE, CWACSemPostForOpenSSLHack)))
		{
			wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init x509 cert security context failed we quit the process\n", \
						WAI_FUNC_LINE_LITERAL);
			exit(1);
		}
	}
	else
	{ // preshared
		if(!CWErr(CWSecurityInitContext(&gACSecurityContext, NULL, NULL, NULL, CW_FALSE, CWACSemPostForOpenSSLHack)))
		{
			wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init non x509 cert security context failed we quit the process\n", \
						WAI_FUNC_LINE_LITERAL);
			exit(1);
		}
	}
#endif

	CWThread thread_dbus;	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init dbus thread\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init dbus thread\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWCreateThread(&thread_dbus, wid_dbus_thread, NULL, 0)))
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init dbus thread failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL);
		exit(1);
	}

	CWThread thread_waw;
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init cross-process message handler thread\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init cross-process message handler thread\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWCreateThread(&thread_waw, CWWawInforUpdate, NULL, 0)))
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init cross-process message handler thread failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL);
		exit(1);
	}
	
	int i = 0;	
	gACSocket.count = 0;//zhanglei mv it out
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init capwap control channel sockets\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init capwap control channel sockets\n", WAI_FUNC_LINE_LITERAL);
	if(	!CWErr(CWNetworkInitSocketServerMultiHomed(&gACSocket, CW_CONTROL_PORT, gMulticastGroups, gMulticastGroupsCount)))
	{ // error starting
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init capwap control channel sockets failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL);
		exit(1);
	}	
/*	if(	!CWErr(CWNetworkInitSocketServerMultiHomed(&gACSocket, CW_CONTROL_PORT_AU, gMulticastGroups, gMulticastGroupsCount))//	||
	){ //zhang lei binding 1234 and 5246
		wid_syslog_crit("Can't start AC");
		exit(1);
	}	
*/
	for(i = 0; i < WTP_NUM; i++)
	{
		gWTPs[i].isNotFree = CW_FALSE;
		gWTPs[i].oemoption = 0;
	}	

	// store network interface's addresses
	gInterfacesCount = CWNetworkCountInterfaceAddresses(&gACSocket);
	gInterfacesCountIpv4 = CWNetworkCountInterfaceAddressesIpv4(&gACSocket);
	gInterfacesCountIpv6 = CWNetworkCountInterfaceAddressesIpv6(&gACSocket);
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"capwap control channel take effect on %u interfaces(%u ipv4 + %u ipv6)\n", \
				WAI_FUNC_LINE_LITERAL, gInterfacesCount, gInterfacesCountIpv4, gInterfacesCountIpv6);
	CW_CREATE_ARRAY_ERR(gInterfaces, gMaxInterfacesCount, CWProtocolNetworkInterface, wid_syslog_crit("Out of Memory"); return;);
	
	for(i = 0; i < gMaxInterfacesCount; i++)
	{
		gInterfaces[i].WTPCount = 0;
		gInterfaces[i].enable = 0;
		gInterfaces[i].tcpdumpflag = 0;
		gInterfaces[i].datacount = 0;
		gInterfaces[i].times = 0;
		memset(gInterfaces[i].ip, 0, 128);
		memset(gInterfaces[i].ifname, 0, 16);
	}

	if(!CWErr(CWCreateThreadMutex(&gCreateIDMutex)))
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init thread mutex failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL);
		exit(1);
	}
	
	WID_Interface_Listen_init();

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init packet and control message process threads\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init packet and control message process threads\n", WAI_FUNC_LINE_LITERAL);
	for(i = 0; i < THREAD_NUM; i++)
	{	
		CWACThreadArg_clone *argPtr = NULL;		
		CW_CREATE_OBJECT_ERR(argPtr, CWACThreadArg_clone, { wid_syslog_crit("Out Of Memory"); return; });
		argPtr->index = i + 1;
		if(!CWErr(CWCreateThread(&(WidThread[i]), CWManageWTP, argPtr, 0)))
		{
			CW_FREE_OBJECT(argPtr);
			return;
		}
	}
	//auto ap area
	{
		g_auto_ap_login.auto_ap_switch = 0;
		g_auto_ap_login.save_switch = 1;
		g_auto_ap_login.ifnum = 0;
		g_auto_ap_login.auto_ap_if = NULL;
	}
	/*ac flow or num balance flag*/	
	ac_flow_num_balance_flag.state = 0;
	ac_flow_num_balance_flag.num_balance_para = 1;
	ac_flow_num_balance_flag.flow_balance_para = gFlow_balance_para;
	
	gapscanset.opstate = 1;
	
	g_AC_ALL_EXTENTION_INFORMATION_SWITCH = 0;
	
	gWLAN_STA_STATIC_ARP_POLICY.policy = 0;
	memset(gWLAN_STA_STATIC_ARP_POLICY.arp_ifname,0,ETH_IF_NAME_LEN);
	gWLAN_UNI_MUTI_BRO_CAST.multicast_broadcast_policy = 0;
	gWLAN_UNI_MUTI_BRO_CAST.rate = 10;
	gWLAN_UNI_MUTI_BRO_CAST.unicast_policy = 0;
	gWLAN_UNI_MUTI_BRO_CAST.wifi_policy = 0;

	/*ap_update_config*/
	memset(&g_ap_udapte_config, 0, sizeof(struct ap_update_config));
	memcpy(g_ap_udapte_config.user, "wtp", strlen("wtp"));
	memcpy(g_ap_udapte_config.passwd, "w1t23p", strlen("w1t23p"));
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init license bindings\n", WAI_FUNC_LINE_LITERAL);
	license_binding_init("2,3");
	license_binding_init("5,6");
	LISTEN_IF_INIT();
}

int read_file_info(char *FILENAME,char *buff)
{
	int len,fd;
	
	fd = open(FILENAME,O_RDONLY);
	if (fd < 0) {
		return 1;
	}	
	len = read(fd,buff,DEFAULT_LEN);	
	
	if (len < 0)
	{
		close(fd);
		return 1;
	}
	if(len != 0)
	{
		if(buff[len-1] == '\n')
		{
			buff[len-1] = '\0';
		}
	}
	close(fd);
	return 0;
}


int get_dir_wild_file_count(char *dir, char *wildfile)
{
	DIR *dp = NULL;
	struct dirent *dirp;
	int wildfilecount = 0;
	dp = opendir(dir);
	if(dp == NULL)
	{
		return wildfilecount;
	}
	while((dirp = readdir(dp)) != NULL)
	{
		if((memcmp(dirp->d_name,wildfile,strlen(wildfile))) ==  0)
		{
			wildfilecount++;
		}
	}
	closedir(dp);
	return wildfilecount;
}


void CWWIDInit()
{	
	int i = 0;
	int licensecount = 0;
	char buf_base[DEFAULT_LEN] = {0};
	char strdir[DEFAULT_LEN] = {0};	

	char Board_Slot_Path[] = "/dbm/local_board/slot_id";
	unsigned int slot_id = 0;
	char tmpbuf[DEFAULT_LEN] = {0}; 
	char newlicense[128] = {0};
	memset(tmpbuf, 0, DEFAULT_LEN);
	if(read_file_info(Board_Slot_Path,tmpbuf) == 0)
	{
		if(parse_int_ID(tmpbuf,&slot_id) == 0)
		{
			slotid = slot_id;
		}	
	}
	sprintf(newlicense,"wtplicense%d-%d",local,vrrid);
	glicensecount = get_dir_wild_file_count(AFC_RUN_PAH,newlicense);
 
	if(licensecount < 9)
	{
		glicensecount = 9;	
	}
	else
	{
		glicensecount = licensecount;
	}

//	glicensecount = SOFTACLICENSE;

	/*xiaodawei add, 20101104, initialization for g_wtp_binding_count*/
	g_wtp_binding_count = malloc((glicensecount + 1)*sizeof(LICENSE_TYPE *));
	for(i = 0; i < glicensecount + 1; i++)
	{
		g_wtp_binding_count[i] = NULL;
	}

	if(glicensecount == 0)
	{
		g_wtp_count = malloc(sizeof(LICENSE_TYPE *));
		g_wtp_count[0] = malloc(sizeof(LICENSE_TYPE));
		g_wtp_count[0]->gcurrent_wtp_count = 0;
		g_wtp_count[0]->gmax_wtp_count = WTP_DEFAULT_NUM_AUTELAN;
		g_wtp_count[0]->gmax_wtp_count_assign = WTP_DEFAULT_NUM_AUTELAN;
		g_wtp_count[0]->flag = 0;
		g_wtp_count[0]->isShm = 0;
		glicensecount = 1;
	}
	else
	{
		g_wtp_count = malloc(glicensecount*(sizeof(LICENSE_TYPE *)));
		
		for(i = 0; i < glicensecount; i++)
		{
			g_wtp_count[i] = malloc(sizeof(LICENSE_TYPE));
			g_wtp_count[i]->gcurrent_wtp_count = 0;
			g_wtp_count[i]->gmax_wtp_count_assign = 0;
			g_wtp_count[i]->gmax_wtp_count = 0;
			g_wtp_count[i]->flag = 0;
			g_wtp_count[i]->isShm = 0;
			memset(strdir, 0, DEFAULT_LEN);
			memset(buf_base, 0, DEFAULT_LEN);	
			{
				sprintf(strdir,AFC_RUN_PAH"/wtplicense%d-%d-%d",local,vrrid,i+1);
				if(read_ac_info(strdir,buf_base) == 0)
				{
					if(parse_int_ID(buf_base, &g_wtp_count[i]->gmax_wtp_count)==-1)
					g_wtp_count[i]->gmax_wtp_count = WTP_DEFAULT_NUM_OEM;
				}
				else
				{
					g_wtp_count[i]->gmax_wtp_count = /*WTP_DEFAULT_NUM_OEM*/SOFTACLICENSE;
				}
			}
			g_wtp_count[i]->gmax_wtp_count_assign = g_wtp_count[i]->gmax_wtp_count;
		}
		
	}
#if NOSOFTAC
	g_wtp_count = malloc(sizeof(LICENSE_TYPE *));
	g_wtp_count[0] = malloc(sizeof(LICENSE_TYPE));
	g_wtp_count[0]->gcurrent_wtp_count = 0;
	g_wtp_count[0]->gmax_wtp_count = SOFTACLICENSE;
	g_wtp_count[0]->gmax_wtp_count_assign = SOFTACLICENSE;
	g_wtp_count[0]->flag = 0;
	g_wtp_count[0]->isShm = 0;
	glicensecount = SOFTACLICENSE;
#endif 

	WTP_NUM += 1;
	G_RADIO_NUM = WTP_NUM*L_RADIO_NUM;
	BSS_NUM = G_RADIO_NUM*L_BSS_NUM;
	AC_WTP = malloc(WTP_NUM*(sizeof(WID_WTP *)));
	memset(AC_WTP,0,WTP_NUM*(sizeof(WID_WTP *)));
	AC_RADIO = malloc(G_RADIO_NUM*(sizeof(WID_WTP_RADIO *)));
	memset(AC_RADIO,0,G_RADIO_NUM*(sizeof(WID_WTP_RADIO *)));
	AC_BSS = malloc(BSS_NUM*(sizeof(WID_BSS *)));
	memset(AC_BSS,0,BSS_NUM*(sizeof(WID_BSS *)));
	AC_ATTACH = malloc(WTP_NUM*(sizeof(CWWTPAttach *)));
	memset(AC_ATTACH,0,WTP_NUM*(sizeof(CWWTPAttach *)));
	gWTPs = malloc(WTP_NUM*(sizeof(CWWTPManager)));
	memset(gWTPs,0,WTP_NUM*(sizeof(CWWTPManager)));
	AC_WTP_ACC = malloc(sizeof(WID_ACCESS));
	memset(AC_WTP_ACC, 0, sizeof(WID_ACCESS));
	memset(AC_WTP_ACC->wtp_list_hash, 0, 256*sizeof(struct wtp_access_info *));
	USER_GROUP[USER_GROUP_DEFAUID] = &SOFT_USERGRP[USER_GROUP_DEFAUID];
	/*wireless-qos-original-config*/
    const int QosID = QOS_NUM-1;
	WID_ADD_QOS_PROFILE("default-wqos",QosID);
	WID_QOS_SET_QOS_WMM_MAP(QosID,1);
}
void LISTEN_IF_INIT(){
	gListenningIF.count = 0;
	gListenningIF.interfaces = NULL;
}

void CWACDestroy() {
	wid_syslog_debug_debug(WID_DEFAULT,"CWACDestroy start");
	CWNetworkCloseMultiHomedSocket(&gACSocket);
	int j=0;
	for(j=0;j<SOCK_NUM;j++){
		close(sockPerThread[j]);
	}
	
	CWDestroyThreadMutex(&gWTPsMutex);
	CWDestroyThreadMutex(&gCreateIDMutex);
	CWDestroyThreadMutex(&gActiveWTPsMutex);
	CW_FREE_OBJECT(gACName);
	CW_FREE_OBJECT(gACHWVersion_char);
	CW_FREE_OBJECT(gACSWVersion_char);
	CW_FREE_OBJECT(gInterfaces);
	wid_syslog_info("AC Destroyed");
	wid_syslog_debug_debug(WID_DEFAULT,"CWACDestroy end");
}


void ACInterfaceReInit()
{	
	int i = 0;
	wid_syslog_debug_debug(WID_DEFAULT,"ACInterfaceReInit start");
	CWNetworkLev4Address *addresses = NULL;
	struct sockaddr_in *IPv4Addresses = NULL;
	CWNetworkCloseMultiHomedSocket(&gACSocket);
	CW_FREE_OBJECT(gInterfaces);
	gACSocket.count = 0;
	if(	!CWErr(CWNetworkInitSocketServerMultiHomed(&gACSocket, CW_CONTROL_PORT, gMulticastGroups, gMulticastGroupsCount))
	    //|| !CWErr(CWNetworkInitSocketServerMultiHomed(&gACSocket, CW_CONTROL_PORT_AU, gMulticastGroups, gMulticastGroupsCount))
		|| !CWErr(CWNetworkGetInterfaceAddresses(&gACSocket, &addresses, &IPv4Addresses))
	)   // error starting
	{
		wid_syslog_crit("Can't start AC");
		exit(1);
	}
	
	gInterfacesCount = CWNetworkCountInterfaceAddresses(&gACSocket);
	gInterfacesCountIpv4 = CWNetworkCountInterfaceAddressesIpv4(&gACSocket);
	gInterfacesCountIpv6 = CWNetworkCountInterfaceAddressesIpv6(&gACSocket);
	wid_syslog_debug_debug(WID_DEFAULT,"Found %d Network Interface(s)", gInterfacesCount);
	
	if (gInterfacesCount <= 0)
	{
		wid_syslog_crit("Can't start AC");
	}
	
	/*coverity 10143 memory leak*/
	CW_CREATE_ARRAY_ERR(gInterfaces, gInterfacesCount, CWProtocolNetworkInterface, 
				CW_FREE_OBJECT(addresses); wid_syslog_crit("Out of Memory"); return;);
	
	for(i = 0; i < gInterfacesCount; i++)
	{
		gInterfaces[i].WTPCount = 0;
		CW_COPY_NET_ADDR_PTR(&(gInterfaces[i].addr), ((CWNetworkLev4Address*)&((addresses)[i])));
		if(IPv4Addresses != NULL)
		{
			CW_COPY_NET_ADDR_PTR(&(gInterfaces[i].addrIPv4), &((IPv4Addresses)[i]));
		}
	}
	
	CW_FREE_OBJECT(addresses);
	CW_FREE_OBJECT(IPv4Addresses);
	LISTEN_IF_INIT();
	wid_syslog_debug_debug(WID_DEFAULT,"ACInterfaceReInit end");
}

__inline__ unsigned int CWGetSeqNum()
{
	static unsigned int seqNum = 0;
	unsigned int r = 0;
	
	if(!CWThreadMutexLock(&gCreateIDMutex)) 
	{
		wid_syslog_crit("Error Locking a mutex");
	}
	r = seqNum;
	if (seqNum == CW_MAX_SEQ_NUM)
	{
	    seqNum = 0;
	}
	else
	{
	    seqNum++;
	}
	CWThreadMutexUnlock(&gCreateIDMutex);
	wid_syslog_debug(WAI_FUNC_LINE_FMT"pkt sequence %u\n",  WAI_FUNC_LINE_LITERAL, r);
	return r;
}


__inline__ int CWGetFragmentID()
{
	static int fragID = 0;
	int r = 0;

	if(!CWThreadMutexLock(&gCreateIDMutex))
	{
		wid_syslog_crit("Error Locking a mutex");
	}
	r = fragID;
	if (fragID == CW_MAX_FRAGMENT_ID)
	{
	    fragID = 0;
	}
	else
	{
	    fragID++;
	}
	CWThreadMutexUnlock(&gCreateIDMutex);

	return r;
}


CWBool CWwAWInitSocket(int *sock)
{
	struct sockaddr_un local;
	char WID_PATH[PATH_LEN] = AFC_RUN_PAH"/wid";
	char ASD_PATH[PATH_LEN] = AFC_RUN_PAH"/asd_table";
	char ASD_PATH1[PATH_LEN] = AFC_RUN_PAH"/asd_sta";
	char WSM_TABLE_PATH[PATH_LEN] = AFC_RUN_PAH"/wsm_table_socket";
	int len = 0;//,t,rlen;
	//int ret;
	InitPath(vrrid,WID_PATH);
	InitPath(vrrid,ASD_PATH);
	InitPath(vrrid,ASD_PATH1);
	InitPath(vrrid,WSM_TABLE_PATH);
	
	//int rcvbuf = SOCK_BUFSIZE;
	int sndbuf = SOCK_BUFSIZE;
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init global socket\n", WAI_FUNC_LINE_LITERAL);
	*sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (*sock < 0)
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"create socket error %s",WAI_FUNC_LINE_LITERAL,strerror(errno));
		perror("socket[PF_UNIX,SOCK_STREAM]");
		return -1;
	}
	local.sun_family = PF_UNIX;
	strcpy(local.sun_path,WID_PATH);
	unlink(local.sun_path); 
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(*sock, (struct sockaddr *) &local, len) < 0)
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"socket bind on sock %u error %s",WAI_FUNC_LINE_LITERAL,*sock, strerror(errno));
		perror("bind(PF_UNIX)");
		return -1;
	}
	int i = 0;
	for(i = 0; i < SOCK_NUM; i++)
	{
		sockPerThread[i]= socket(PF_UNIX, SOCK_DGRAM, 0);
		if ((setsockopt(sockPerThread[i],SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf))) < 0)
		{	
			wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"set per thread socket option of t%u sock %u error %s",\
						WAI_FUNC_LINE_LITERAL, i, sockPerThread[i], strerror(errno));
			perror("setsockopt");
			return -1;
		}
		fcntl(sockPerThread[i], F_SETFL, O_NONBLOCK);
	}
	
	toASD.addr.sun_family = PF_UNIX;
	strcpy(toASD.addr.sun_path,ASD_PATH); 
	toASD.addrlen = strlen(toASD.addr.sun_path) + sizeof(toASD.addr.sun_family);
	toASD_STA.addr.sun_family = PF_UNIX;
	strcpy(toASD_STA.addr.sun_path,ASD_PATH1);
	toASD_STA.addrlen = strlen(toASD_STA.addr.sun_path) + sizeof(toASD_STA.addr.sun_family);
	
	toWSM.addr.sun_family = PF_UNIX;
	strcpy(toWSM.addr.sun_path,WSM_TABLE_PATH);
	toWSM.addrlen = strlen(toWSM.addr.sun_path) + sizeof(toWSM.addr.sun_family);

	return CW_TRUE;
}

CWBool CWInitMsgQueue(int *msgqid)
{
	key_t key;
	FILE *fp = NULL;
	InitPath(vrrid,MSGQ_PATH);
	

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"init msg queue\n", WAI_FUNC_LINE_LITERAL);
	/*coverity 10145 memory leak*/
	fp = fopen(MSGQ_PATH,"w");
	if(fp == NULL)
	{		
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"open %s failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL, MSGQ_PATH);
		perror("fopen");
		exit(1);	
	}
		
	if ((key = ftok(MSGQ_PATH, 'W')) == -1) {
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"ftok %s failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL, MSGQ_PATH);
		perror("ftok");
		fclose(fp);
		exit(1);
	}
	if ((*msgqid = msgget(key, 0666 | IPC_CREAT)) == -1) {		
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"get msq id on %s failed we quit the process\n", \
					WAI_FUNC_LINE_LITERAL, MSGQ_PATH);
		perror("msgget");
		fclose(fp);
		exit(1);
	}
	fclose(fp);
	return CW_TRUE;
}

CWBool AsdWsm_RadioOp(unsigned int WTPID, Operate op)
{
	int i = 0;
	int j = 0;
	TableMsg wASD;
	int len;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}

	memset(&wASD, 0, sizeof(wASD));
	wASD.Op = op;
	wASD.Type = WTP_TYPE;
	
	wASD.u.RadioFlow.wtpid = WTPID;
	
	 /* coverity-CID: 10124 Out-of-bounds write */ 
	for(i=0; i<L_RADIO_NUM; i++)
	{
		wASD.u.RadioFlow.radio_flow[i].tx_bytes = 0;
	}
	
	for(i=0; i<L_RADIO_NUM; i++)
	{
		for(j=0; j<TOTAL_AP_IF_NUM; j++)
		{
			if((AC_WTP[WTPID]->apstatsinfo[j].radioId == i)&&(AC_WTP[WTPID]->apstatsinfo[j].type == 0))
			{
				wASD.u.RadioFlow.radio_flow[i].tx_bytes += (unsigned int)AC_WTP[WTPID]->apstatsinfo[j].tx_bytes;
				wASD.u.RadioFlow.radio_flow[i].tx_bytes += (unsigned int)AC_WTP[WTPID]->apstatsinfo[j].rx_bytes;
				AC_WTP[WTPID]->rate = (wASD.u.RadioFlow.radio_flow[i].tx_bytes-AC_WTP[WTPID]->old_bytes)/30;
				AC_WTP[WTPID]->old_bytes = wASD.u.RadioFlow.radio_flow[i].tx_bytes;
			}
		}
	}
	
	len = sizeof(wASD);
	int sock_index = WTPID%SOCK_NUM;
	int sock = sockPerThread[sock_index];
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		return CW_FALSE;
	}
	return CW_TRUE;

}

CWBool AsdWsm_bytes_info(unsigned int WTPID, Operate op)
{
	int i = 0;
	int j = 0;
	int jj = 0;
	TableMsg wASD;
	int len = 0;
	int l_radioId = 0;
	u_int8_t wlanID = 0;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}

	memset(&wASD, 0, sizeof(wASD));
	
	wASD.Op = op;
	wASD.Type = BSS_TYPE;
	for(jj=0;jj<L_BSS_NUM;jj++){
			wASD.u.BSS.BssData[jj].BSSIndex = 0;
			wASD.u.BSS.BssData[jj].Radio_G_ID = 0;
			wASD.u.BSS.BssData[jj].rx_pkt_data = 0;
			wASD.u.BSS.BssData[jj].tx_pkt_data = 0;
	}
	for(i=0; (i<TOTAL_AP_IF_NUM); i++){
		l_radioId = AC_WTP[WTPID]->apstatsinfo[i].radioId;
		wlanID = AC_WTP[WTPID]->apstatsinfo[i].wlanId;
		if(AC_WTP[WTPID]->apstatsinfo[i].type == 0){
			for(j=0;j<L_BSS_NUM;j++){
				if((l_radioId < L_RADIO_NUM)&&(AC_WTP[WTPID]->WTP_Radio[l_radioId] != NULL))
					if((AC_WTP[WTPID]->WTP_Radio[l_radioId]->BSS[j] != NULL)&&(wlanID == AC_WTP[WTPID]->WTP_Radio[l_radioId]->BSS[j]->WlanID))
					{
						wASD.u.BSS.BssData[j].BSSIndex = AC_WTP[WTPID]->WTP_Radio[l_radioId]->BSS[j]->BSSIndex;
						wASD.u.BSS.BssData[j].Radio_G_ID = AC_WTP[WTPID]->WTP_Radio[l_radioId]->BSS[j]->BSSIndex/L_BSS_NUM;	//xiaodawei modify, 20110224
						wASD.u.BSS.BssData[j].rx_pkt_data = AC_WTP[WTPID]->apstatsinfo[i].rx_pkt_data;
						wASD.u.BSS.BssData[j].tx_pkt_data = AC_WTP[WTPID]->apstatsinfo[i].tx_pkt_data;
						wASD.u.BSS.BssData[j].rx_data_bytes = AC_WTP[WTPID]->apstatsinfo[i].rx_bytes;	//xiaodawei add rx_data_bytes, 20110224
						wASD.u.BSS.BssData[j].tx_data_bytes = AC_WTP[WTPID]->apstatsinfo[i].tx_bytes;	//xiaodawei add tx_data_bytes, 20110224
						
						wid_syslog_debug_debug(WID_DEFAULT,"BssData[%d].BSSIndex: %d = BSS[%d]->BSSIndex: %d\n", j, wASD.u.BSS.BssData[j].BSSIndex,j ,AC_WTP[WTPID]->WTP_Radio[l_radioId]->BSS[j]->BSSIndex);
						wid_syslog_debug_debug(WID_DEFAULT,"BssData[%d].rx_pkt_data: %u = apstatsinfo[%d].rx_pkt_data: %u\n",j, wASD.u.BSS.BssData[j].rx_pkt_data, i, AC_WTP[WTPID]->apstatsinfo[i].rx_pkt_data);
						wid_syslog_debug_debug(WID_DEFAULT,"BssData[%d].tx_pkt_data: %u = apstatsinfo[%d].tx_pkt_data: %u\n",j, wASD.u.BSS.BssData[j].tx_pkt_data, i, AC_WTP[WTPID]->apstatsinfo[i].tx_pkt_data);
						wid_syslog_debug_debug(WID_DEFAULT,"BssData[%d].rx_data_bytes: %llu = apstatsinfo[%d].rx_bytes: %llu\n",j, wASD.u.BSS.BssData[j].rx_data_bytes, i, AC_WTP[WTPID]->apstatsinfo[i].rx_bytes);
						wid_syslog_debug_debug(WID_DEFAULT,"BssData[%d].tx_data_bytes: %llu = apstatsinfo[%d].tx_bytes: %llu\n",j, wASD.u.BSS.BssData[j].tx_data_bytes, i, AC_WTP[WTPID]->apstatsinfo[i].tx_bytes);
					}
			}
		}
	}
	
	len = sizeof(wASD);
	int sock_index = WTPID%SOCK_NUM;
	int sock = sockPerThread[sock_index];
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		return CW_FALSE;
	}
	return CW_TRUE;

}
int copymixwtpip(struct mixwtpip *wtpip, const struct sockaddr *sa2)
{
	switch (sa2->sa_family) {
	case AF_INET: {
		 wtpip->addr_family = AF_INET;
		 #if 0
		(memcpy(wtpip->addr,
					   &((struct sockaddr_in *) sa2)->sin_addr,
					   sizeof(struct in_addr)));
		#endif
		wtpip->m_v4addr = ((struct sockaddr_in *)sa2)->sin_addr.s_addr;
		wtpip->port = ((struct sockaddr_in *)sa2)->sin_port + 1;
		return 0;
	}

#ifdef	IPV6
	case AF_INET6: {
		 wtpip->addr_family = AF_INET6;
		(memcpy( wtpip->m_v6addr,
					   &((struct sockaddr_in6 *) sa2)->sin6_addr,
					   sizeof(struct in6_addr)));
		wtpip->port = ((struct sockaddr_in6 *)sa2)->sin6_port + 1; 
		return 0;
	}
#endif
	default:
	   wtpip->addr_family = 0;
	}

	return 0;
}

//xm add 08/12/04
CWBool AsdWsm_WTPOp(unsigned int WtpID,Operate op)
{
	int len = 0;
	TableMsg wASD;
	memset(&wASD, 0, sizeof(TableMsg));	
	if(!check_wtpid_func(WtpID))
	{
		wid_syslog_err("%s invalid wtpid:%d\n", __func__, WtpID);
		return CW_FALSE;
	}

	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = WtpID%SOCK_NUM;
	int sock = sockPerThread[i];
	wASD.Op = op;
	wASD.Type = WTP_TYPE;
	wASD.u.WTP.WtpID= WtpID;
	wASD.u.WTP.wtp_max_sta_num= AC_WTP[WtpID]->wtp_allowed_max_sta_num;
	wASD.u.WTP.wtp_triger_num= AC_WTP[WtpID]->wtp_triger_num;
	wASD.u.WTP.wtp_flow_triger= AC_WTP[WtpID]->wtp_flow_triger;
	wASD.u.WTP.RSSI = AC_WTP[WtpID]->RSSI;	
	wASD.u.WTP.state = AC_WTP[WtpID]->WTPStat;
	memset(wASD.u.WTP.BindingIFName,0,ETH_IF_NAME_LEN);
	memcpy(wASD.u.WTP.BindingIFName,AC_WTP[WtpID]->BindingIFName,ETH_IF_NAME_LEN);
    /* zhangshu modify for netid , 2010-10-26 */
	memset(wASD.u.WTP.NETID, 0, WTP_NETID_LEN);
	memcpy(wASD.u.WTP.NETID,AC_WTP[WtpID]->netid,strlen(AC_WTP[WtpID]->netid));
	
	//xm add 08/12/04
	memset(wASD.u.WTP.WTPMAC,0,MAC_LEN);
	memcpy(wASD.u.WTP.WTPMAC,AC_WTP[WtpID]->WTPMAC,MAC_LEN);

	//struct sockaddr_in	*sin = (struct sockaddr_in *)&(gWTPs[WtpID].address);
	//wASD.u.WTP.WTPIP= sin->sin_addr.s_addr;

	copymixwtpip(&(wASD.u.WTP.WTPIP),(struct sockaddr *)&gWTPs[WtpID].address);


	memset(wASD.u.WTP.WTPSN,0,128);
	memcpy(wASD.u.WTP.WTPSN,AC_WTP[WtpID]->WTPSN,strlen(AC_WTP[WtpID]->WTPSN));
	//zhaoruijia,20100916,add
	memset(wASD.u.WTP.WTPNAME,0,256);
	memcpy(wASD.u.WTP.WTPNAME,AC_WTP[WtpID]->WTPNAME,strlen(AC_WTP[WtpID]->WTPNAME));
	
	if(op == WID_WIFI_INFO)
	{
		memset(&(wASD.u.WTP.wifi_extension_info),0,sizeof(wid_wifi_info));
		memcpy(&(wASD.u.WTP.wifi_extension_info),&(AC_WTP[WtpID]->wifi_extension_info),sizeof(wid_wifi_info));			
	}
		
	len = sizeof(wASD);
	
	//wid_syslog_info("AsdWsm_WTPOp1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_crit("%s sendto %s AsdWsm_WTPOp2 WTPID [%d]\n", __func__, strerror(errno), WtpID);	
	//	close(sock);
		return CW_FALSE;
	}	
	//wid_syslog_info("AsdWsm_WTPOp3\n");
	//close(sock);
	return CW_TRUE;
}

//wuwl add 20100910
CWBool Asd_neighbor_ap_sta_check_op(unsigned int WtpID,unsigned int N_WtpID,unsigned char mac[6],Operate op){

	TableMsg wASD;
	int len;	
	
	if(!check_wtpid_func(WtpID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = WtpID%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wASD, 0, sizeof(wASD));
	
	wASD.Op = op;
	wASD.Type = NEIGHBOR_AP_STA_CHECK;
	wASD.u.WTP.WtpID= WtpID;
	wASD.u.WTP.N_WTP.N_WtpID = N_WtpID;
	memset(wASD.u.WTP.N_WTP.WTPMAC,0,MAC_LEN);
	memcpy(wASD.u.WTP.N_WTP.WTPMAC,mac,MAC_LEN);
	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"rogue_terminal,%s\n",__func__);
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		
		wid_syslog_info("AsdWsm_WTPOp2 WTPID [%d]\n",WtpID);
		return CW_FALSE;
	}
	return CW_TRUE;
}

CWBool AsdWsm_WTP_Channelchange_Op(unsigned int WtpID,unsigned int radioid,Operate op){

	TableMsg wASD;
	int len;	
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if(!check_wtpid_func(WtpID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = WtpID%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wASD, 0, sizeof(wASD));
	
	wASD.Op = op;
	wASD.Type = WTP_TYPE;
	wASD.u.WTP_chchange.WTPID = WtpID;
	wASD.u.WTP_chchange.radioid = radioid;
	if(!check_l_radioid_func(radioid)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	wASD.u.WTP_chchange.channel = AC_WTP[WtpID]->WTP_Radio[radioid]->Radio_Chan;
	len = sizeof(wASD);
	//wid_syslog_info("AsdWsm_WTP_Channelchange_Op1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		
		wid_syslog_info("AsdWsm_WTP_Channelchange_Op2 WTPID [%d]\n",WtpID);
	//	close(sock);
		return CW_FALSE;
	}
	//wid_syslog_info("AsdWsm_WTP_Channelchange_Op3\n");
	//close(sock);
	return CW_TRUE;
}


CWBool AsdWsm_WLANOp(unsigned char WlanID, Operate op, int both)
{
	if (!check_wlanid_func(WlanID))
	{
		return CW_FALSE;
	}
	
	if(AC_WLAN[WlanID] == NULL)
	{
		return CW_FALSE;
	}
	
	TableMsg wASD;
	int len = 0;
	int sndbuf = SOCK_BUFSIZE;
	
	int sock = socket(PF_UNIX, SOCK_DGRAM, 0);	
	if (sock < 0) 
	{
		wid_syslog_err("open socket error when %s(line %d)\n", __func__, __LINE__);
		return CW_FALSE;
	}
	if ((setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf))) < 0)
	{	
		wid_syslog_err("%s setsockopt %s",__func__,strerror(errno));
		perror("setsockopt");	
		/*coverity 10144 memory leak*/
		close(sock);
		return CW_FALSE;
	}

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
	{
		wid_syslog_err("%s:%d, wlan %d set sock nonblock failed %s\n",
			            __func__, __LINE__, WlanID, strerror(errno));
	}

	memset(&wASD, 0, sizeof(wASD));
	wASD.Op = op;
	wASD.Type = WLAN_TYPE;
	wASD.u.WLAN.WlanID = WlanID;
	wASD.u.WLAN.WlanState = AC_WLAN[WlanID]->Status;
	strcpy(wASD.u.WLAN.WlanName, AC_WLAN[WlanID]->WlanName);
	if ((AC_WLAN[WlanID]->ESSID) && (strlen((char *)AC_WLAN[WlanID]->ESSID) < ESSID_LENGTH))
	{
		strcpy(wASD.u.WLAN.ESSID, (char *)AC_WLAN[WlanID]->ESSID);
	}
	else
	{
		memcpy(wASD.u.WLAN.ESSID, AC_WLAN[WlanID]->ESSID, ESSID_LENGTH);
	}
	wASD.u.WLAN.wlan_max_sta_num = AC_WLAN[WlanID]->wlan_max_allowed_sta_num;//xm add 08/12/04
	wASD.u.WLAN.wlan_balance_triger_num = AC_WLAN[WlanID]->wlan_balance_triger_num;
	wASD.u.WLAN.balance_para = AC_WLAN[WlanID]->balance_para;
	wASD.u.WLAN.flow_balance_para = AC_WLAN[WlanID]->flow_balance_para;
	wASD.u.WLAN.balance_switch = AC_WLAN[WlanID]->balance_switch;
	wASD.u.WLAN.balance_method = AC_WLAN[WlanID]->balance_method;
	wASD.u.WLAN.Roaming_policy = AC_WLAN[WlanID]->Roaming_Policy;
	wASD.u.WLAN.flow_check = AC_WLAN[WlanID]->flow_check;
	wASD.u.WLAN.limit_flow = AC_WLAN[WlanID]->limit_flow;
	wASD.u.WLAN.no_flow_time = AC_WLAN[WlanID]->no_flow_time;

	/*ENR-17*/
	wASD.u.WLAN.ctr.enable = AC_WLAN[WlanID]->ctr_head.enable;
	wASD.u.WLAN.ctr.activities_effective_time = AC_WLAN[WlanID]->ctr_head.activities_effective_time;
	wASD.u.WLAN.ctr.session_time = AC_WLAN[WlanID]->ctr_head.session_time;
	wASD.u.WLAN.sp_time = AC_WLAN[WlanID]->sp_head.sp_time;
	
	len = sizeof(wASD);
	//wid_syslog_info("AsdWsm_WLANOp1\n");
	if (sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_err("%s sendtoASD %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		
		//wid_syslog_info("AsdWsm_WLANOp2\n");
		close(sock);
		return CW_FALSE;
	}
	else if(both)
	{
		if (sendto(sock, &wASD, len, 0, (struct sockaddr *) &toWSM.addr, toWSM.addrlen) < 0)
		{ 	
			wid_syslog_err("%s sendtoWSM %s",__func__,strerror(errno));
			perror("send(wWSMSocket)");
			
			close(sock);
			return CW_FALSE;		
		}
	}
	
	close(sock);
	return CW_TRUE;
}

CWBool AsdWsm_BSSOp(unsigned int BSSIndex, Operate op, int both)
{
	TableMsg wASD;
	int len = 0;
	memset(&wASD, 0, sizeof(wASD));
	
	if(!check_bssid_func(BSSIndex))
	{
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = (BSSIndex/L_BSS_NUM/L_RADIO_NUM)%SOCK_NUM;
	int sock = sockPerThread[i];
	int wlan_id = AC_BSS[BSSIndex]->WlanID;
	int WTPID = BSSIndex/L_BSS_NUM/L_RADIO_NUM;
	
	if(!check_wlanid_func(wlan_id))
	{
		return CW_FALSE;
	}
	
	if(AC_WLAN[wlan_id] == NULL)
	{		
		wid_syslog_info("%s wlan %d null\n",__func__,wlan_id);
		return CW_FALSE;
	}
	if(!check_wtpid_func(WTPID))
	{
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}
	
	CWThreadMutexLock(&MasterBak);
	struct bak_sock *tmp = bak_list;
	if((is_secondary == 0)&&(bak_list!=NULL))
	{
		if(op == WID_ADD)
		{
			bak_add_del_bss(tmp->sock,B_ADD,BSSIndex);
		}
		else if(op == WID_DEL)
		{
			bak_add_del_bss(tmp->sock,B_DEL,BSSIndex);
		}			
	}
	CWThreadMutexUnlock(&MasterBak);
	wASD.Op = op;
	wASD.Type = BSS_TYPE;
	wASD.u.BSS.BSSIndex = BSSIndex;
	wASD.u.BSS.Radio_L_ID = AC_BSS[BSSIndex]->Radio_L_ID;
	wASD.u.BSS.Radio_G_ID = AC_BSS[BSSIndex]->Radio_G_ID;
	wASD.u.BSS.WlanID = AC_BSS[BSSIndex]->WlanID;
	wASD.u.BSS.vlanid = AC_BSS[BSSIndex]->vlanid;
	memcpy(wASD.u.BSS.BSSID,AC_BSS[BSSIndex]->BSSID, MAC_LEN);
	wASD.u.BSS.hotspot_id= AC_BSS[BSSIndex]->hotspot_id;
	if(!check_g_radioid_func(wASD.u.BSS.Radio_G_ID))
	{
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}
	if(!check_l_radioid_func(wASD.u.BSS.Radio_L_ID))
	{
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}

	//mahz add 2011.5.26,msg �е�nas_port_id �ݶ�����Ϊ11
	if(op == WID_ADD)
	{
		if(AC_BSS[BSSIndex]->nas_port_id[0] != 0)
		{
			memset(wASD.u.BSS.nas_port_id,0,sizeof(wASD.u.BSS.nas_port_id));
			memcpy(wASD.u.BSS.nas_port_id,AC_BSS[BSSIndex]->nas_port_id, strlen(AC_BSS[BSSIndex]->nas_port_id));
		}
		else if((AC_BSS[BSSIndex]->nas_port_id[0] == 0) && (AC_WLAN[wlan_id]->nas_port_id[0] != 0))
		{
			memset(wASD.u.BSS.nas_port_id,0,sizeof(wASD.u.BSS.nas_port_id));
			memcpy(wASD.u.BSS.nas_port_id,AC_WLAN[wlan_id]->nas_port_id, strlen(AC_WLAN[wlan_id]->nas_port_id));
		}		
		else
		{
			memset(wASD.u.BSS.nas_port_id,0,sizeof(wASD.u.BSS.nas_port_id));
			//memcpy(wASD.u.BSS.nas_port_id,"00000001",sizeof("00000001"));
		}
		wid_syslog_debug_info("wASD.u.BSS.nas_port_id: %s\n",wASD.u.BSS.nas_port_id);	//for test
	}

	/* for REQUIREMENTS-489 */
	if (op == WID_ADD)
	{
		wASD.u.BSS.aliasSetFlag = AC_BSS[BSSIndex]->aliasSetFlag;
		memset(wASD.u.BSS.aliasESSID, 0, ESSID_LENGTH);
		memcpy(wASD.u.BSS.aliasESSID, AC_BSS[BSSIndex]->aliasESSID, ESSID_LENGTH);
		wid_syslog_debug_debug(WID_DEFAULT, "wASD.u.BSS.aliasSetFlag: %d\n", wASD.u.BSS.aliasSetFlag);
	}	

	if((AC_WLAN[wlan_id] != NULL)&&(AC_WLAN[wlan_id]->SecurityType == OPEN)&&(AC_WLAN[wlan_id]->EncryptionType == NONE))
	{
		wASD.u.BSS.protect_type = 0;
	}
	else
	{
		wASD.u.BSS.protect_type = 1;
	}
	if((AC_WLAN[wlan_id]->wlan_if_policy == NO_INTERFACE)&&(AC_BSS[BSSIndex]->BSS_IF_POLICY == WLAN_INTERFACE))
	{
		return CW_FALSE;
	}
	else
	{
		//wASD.u.BSS.wlan_ifaces_type = AC_WLAN[wlan_id]->wlan_if_policy;
		//wASD.u.BSS.bss_ifaces_type = AC_BSS[BSSIndex]->BSS_IF_POLICY;

		wASD.u.BSS.wlan_ifaces_type = BSS_INTERFACE;
		if(AC_BSS[BSSIndex]->BSS_IF_POLICY == NO_INTERFACE)
		{
			wASD.u.BSS.bss_ifaces_type = NO_INTERFACE;
		}
		else
		{
			wASD.u.BSS.bss_ifaces_type = BSS_INTERFACE;
		}
	}
	wid_syslog_debug_debug(WID_DEFAULT,"%s:AC_RADIO[%d]->br_ifname[%d] %s",
            	            __func__,wASD.u.BSS.Radio_G_ID,wASD.u.BSS.WlanID,
            	            AC_RADIO[wASD.u.BSS.Radio_G_ID]->br_ifname[wASD.u.BSS.WlanID]);

	if(AC_RADIO[wASD.u.BSS.Radio_G_ID] != NULL)
	{
	    memset(wASD.u.BSS.br_ifname, 0, IF_NAME_MAX);
		memcpy(wASD.u.BSS.br_ifname,AC_RADIO[wASD.u.BSS.Radio_G_ID]->br_ifname[wASD.u.BSS.WlanID],IF_NAME_MAX);
		wid_syslog_debug_debug(WID_DEFAULT, "%s: wASD.u.BSS.br_ifname %s AC_RADIO[%d]->br_ifname[%d] [%s]\n",
		                    __func__,
							wASD.u.BSS.br_ifname,
							wASD.u.BSS.Radio_G_ID,
							wASD.u.BSS.WlanID,
							AC_RADIO[wASD.u.BSS.Radio_G_ID]->br_ifname[wASD.u.BSS.WlanID]);
	}

	wid_syslog_debug_debug(WID_DEFAULT,"%s:br_ifname %s.",__func__,wASD.u.BSS.br_ifname);
	wASD.u.BSS.nas_id_len = 0;
	if((op == WID_ADD) && (AC_BSS[BSSIndex]->nas_id_len > 0))
	{
		wASD.u.BSS.nas_id_len = AC_BSS[BSSIndex]->nas_id_len;
		memcpy(wASD.u.BSS.nas_id, AC_BSS[BSSIndex]->nas_id, NAS_IDENTIFIER_NAME);
	}
	//if(op==WID_MODIFY)
	wASD.u.BSS.bss_max_sta_num = AC_BSS[BSSIndex]->bss_max_allowed_sta_num;//xm add 08/12/04
	wASD.u.BSS.sta_static_arp_policy = AC_BSS[BSSIndex]->sta_static_arp_policy;
	memcpy(wASD.u.BSS.arp_ifname, AC_BSS[BSSIndex]->arp_ifname, ETH_IF_NAME_LEN);
	
	wid_syslog_debug_debug(WID_DEFAULT,"BSSID:%02x:%02x:%02x:%02x:%02x:%02x",
							wASD.u.BSS.BSSID[0],wASD.u.BSS.BSSID[1],wASD.u.BSS.BSSID[2],
							wASD.u.BSS.BSSID[3],wASD.u.BSS.BSSID[4],wASD.u.BSS.BSSID[5]);
	len = sizeof(wASD);
	//wid_syslog_info("AsdWsm_BSSOp1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_info("%s sendtoASD %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
	//	close(sock);
		return CW_FALSE;
		if(op == WID_ADD)
		{
			msgq msg1;			
			struct msgqlist *elem1 = NULL;
			gWTPs[WTPID].isRequestClose = CW_TRUE;	
			memset((char*)&msg1, 0, sizeof(msg1));
			msg1.mqid = WTPID%THREAD_NUM+1;
			msg1.mqinfo.WTPID = WTPID;
			msg1.mqinfo.type = CONTROL_TYPE;
			msg1.mqinfo.subtype = WTP_S_TYPE;
			msg1.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
						
			elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if(elem1 == NULL)
			{			
				perror("malloc");
				return 0;
			}
			
			memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));
			elem1->next = NULL;
			memcpy((char*)&(elem1->mqinfo),(char*)&(msg1.mqinfo),sizeof(msg1.mqinfo));			
			WID_INSERT_CONTROL_LIST(WTPID, elem1);
		}
		wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_BSSOp2 WTPID [%d]\n",BSSIndex/L_BSS_NUM/L_RADIO_NUM);
		return CW_FALSE;
	}
	else if(both)
	{
		if (sendto(sock, &wASD, len, 0, (struct sockaddr *) &toWSM.addr, toWSM.addrlen) < 0)
		{ 	
			wid_syslog_info("%s sendtoWSM %s",__func__,strerror(errno));
			perror("send(wWSMSocket)");
	//		close(sock);
			return CW_FALSE;
			if(op == WID_ADD)
			{
				msgq msg1;			
				struct msgqlist *elem1 = NULL;
				gWTPs[WTPID].isRequestClose = CW_TRUE;	
				memset((char*)&msg1, 0, sizeof(msg1));
				msg1.mqid = WTPID%THREAD_NUM+1;
				msg1.mqinfo.WTPID = WTPID;
				msg1.mqinfo.type = CONTROL_TYPE;
				msg1.mqinfo.subtype = WTP_S_TYPE;
				msg1.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
							
				elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
				if(elem1 == NULL)
				{				
					perror("malloc");
					return 0;				
				}				
				memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));				
				elem1->next = NULL;				
				memcpy((char*)&(elem1->mqinfo),(char*)&(msg1.mqinfo),sizeof(msg1.mqinfo));
				WID_INSERT_CONTROL_LIST(WTPID, elem1);
			}
			//wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_BSSOp3\n");
			return CW_FALSE;		
		}
	}
	//wid_syslog_info("AsdWsm_BSSOp4\n");
//	close(sock);
	if(WID_ADD == op)	
		AsdWsm_BssMacOp(BSSIndex,MAC_LIST_ADD);
	return CW_TRUE;
}

//weichao add
CWBool AsdWsm_BssMacOp(unsigned int BSSIndex, Operate op){
	wid_syslog_debug_debug(WID_DEFAULT,"func %s : \n",__func__);
	TableMsg wASD;
	int len;
	if(!check_bssid_func(BSSIndex)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = (BSSIndex/L_BSS_NUM/L_RADIO_NUM)%SOCK_NUM;
	int sock = sockPerThread[i];
	int wlan_id = AC_BSS[BSSIndex]->WlanID;
	int WTPID = BSSIndex/L_BSS_NUM/L_RADIO_NUM;
	int m = 0,m1 = 0; 
	struct acl_config *conf = NULL;	
	struct maclist * accept_tmp = NULL;
	struct maclist * deny_tmp = NULL;
	if(!check_wlanid_func(wlan_id)){
		return CW_FALSE;
	}
	
	if(AC_WLAN[wlan_id] == NULL){		
		wid_syslog_info("%s wlan %d null\n",__func__,wlan_id);
		return CW_FALSE;
	}
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	if(AC_BSS[BSSIndex]->acl_conf ==  NULL)
		return CW_FALSE;
	conf = AC_BSS[BSSIndex]->acl_conf;
	if(conf == NULL)
		return CW_FALSE;
	accept_tmp = conf->accept_mac;
	deny_tmp = conf->deny_mac;
	memset(&wASD,0,sizeof(wASD));
	wid_syslog_debug_debug(WID_DEFAULT,"the conf->num_accept_mac  is %d\n",conf->num_accept_mac);
	wid_syslog_debug_debug(WID_DEFAULT,"the conf->num_deny_mac  is %d\n",conf->num_deny_mac);
	
	if((conf->num_accept_mac == 0)&&(conf->num_deny_mac == 0))
		return CW_TRUE;
	for( m = 0 ;  ((m <= (conf->num_accept_mac/50))||(m<=(conf->num_deny_mac/50))); m++)
	{
		memset(wASD.u.BSS.accept_mac,0 ,50*MAC_LEN);
		memset(wASD.u.BSS.deny_mac,0 ,50*MAC_LEN);
		wASD.Op = op;
		wASD.Type = BSS_TYPE;
		wASD.u.BSS.BSSIndex = BSSIndex;
		wASD.u.BSS.macaddr_acl = conf->macaddr_acl;
		wid_syslog_debug_debug(WID_DEFAULT,"the msg deny_mac_num = %d\n",wASD.u.BSS.deny_mac_num);
		wid_syslog_debug_debug(WID_DEFAULT,"the conf->macaddr_acl  is %d\n",conf->macaddr_acl);
		wid_syslog_debug_debug(WID_DEFAULT,"the wASD.u.BSS.macaddr_acl  is %d\n",wASD.u.BSS.macaddr_acl );
		for(m1 = 0 ; m1 < 50 ; m1++)
		{
			if(accept_tmp == NULL)
				break;
			wASD.u.BSS.accept_mac[m1][0] = accept_tmp->addr[0];
			wASD.u.BSS.accept_mac[m1][1] = accept_tmp->addr[1];
			wASD.u.BSS.accept_mac[m1][2] = accept_tmp->addr[2];
			wASD.u.BSS.accept_mac[m1][3] = accept_tmp->addr[3];
			wASD.u.BSS.accept_mac[m1][4] = accept_tmp->addr[4];
			wASD.u.BSS.accept_mac[m1][5] = accept_tmp->addr[5];
			wASD.u.BSS.accept_mac_num = m1+1;
			if(accept_tmp->next == NULL)
				break;
			accept_tmp = accept_tmp->next;			
		}
		for(m1 = 0 ; m1 < 50 ; m1++)
		{
			if(deny_tmp == NULL)
				break;
			wASD.u.BSS.deny_mac[m1][0] = deny_tmp->addr[0];
			wASD.u.BSS.deny_mac[m1][1] = deny_tmp->addr[1];
			wASD.u.BSS.deny_mac[m1][2] = deny_tmp->addr[2];
			wASD.u.BSS.deny_mac[m1][3] = deny_tmp->addr[3];
			wASD.u.BSS.deny_mac[m1][4] = deny_tmp->addr[4];
			wASD.u.BSS.deny_mac[m1][5] = deny_tmp->addr[5];
			
			wid_syslog_debug_debug(WID_DEFAULT,"the %dst mac is : "MACSTR"\n",m1,deny_tmp->addr);
			wid_syslog_debug_debug(WID_DEFAULT,"the %dst mac is : "MACSTR"\n",m1,wASD.u.BSS.deny_mac[m1]);

			wASD.u.BSS.deny_mac_num = m1+1;
			if(deny_tmp->next == NULL)
				break;
			deny_tmp = deny_tmp->next;
		}
		len = sizeof(wASD);
		if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
		{
			wid_syslog_info("%s sendtoASD %s",__func__,strerror(errno));
			perror("send(wASDSocket)");
			return CW_FALSE;
			
			msgq msg1;			
			struct msgqlist *elem1;
			gWTPs[WTPID].isRequestClose = CW_TRUE;	
			memset((char*)&msg1, 0, sizeof(msg1));
			msg1.mqid = WTPID%THREAD_NUM+1;
			msg1.mqinfo.WTPID = WTPID;
			msg1.mqinfo.type = CONTROL_TYPE;
			msg1.mqinfo.subtype = WTP_S_TYPE;
			msg1.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
						
			elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if(elem1 == NULL)
			{				
				perror("malloc");				
				return 0;				
			}
			memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));
			elem1->next = NULL;
			memcpy((char*)&(elem1->mqinfo),(char*)&(msg1.mqinfo),sizeof(msg1.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPID, elem1);
			wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_BSSOp2 WTPID [%d]\n",BSSIndex/L_BSS_NUM/L_RADIO_NUM);
			return CW_FALSE;
		}
	}	
	return CW_TRUE;
}


CWBool AsdWsm_DataChannelOp(unsigned int WTPID, Operate op)
{
	if(!check_wtpid_func(WTPID))
	{
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}
	TableMsg wWsm;
	int len = 0;
	int i = 0;
	i = WTPID % SOCK_NUM;
	
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int sock = sockPerThread[i];
	CWThreadMutexLock(&MasterBak);
	struct bak_sock *tmp = bak_list;
	if((is_secondary == 0) && (bak_list != NULL))
	{
		if(op == WID_ADD)
		{
			bak_add_del_wtp(tmp->sock,B_ADD,WTPID);
		}
		else if(op == WID_DEL)
		{
			bak_add_del_wtp(tmp->sock,B_DEL,WTPID);
		}
	}
	CWThreadMutexUnlock(&MasterBak);

	memset(&wWsm, 0, sizeof(wWsm));

	wWsm.Op = op;
	wWsm.Type = WTP_TYPE;
	wWsm.u.DataChannel.WTPID = WTPID;
	wWsm.u.DataChannel.WTPModel = AC_WTP[WTPID]->WTPModel;
	
	//struct sockaddr_in	*sin = (struct sockaddr_in *)&(gWTPs[WTPID].address);
	//wWsm.u.DataChannel.WTPIP = sin->sin_addr.s_addr;

	copymixwtpip(&(wWsm.u.DataChannel.WTPIP),(struct sockaddr *)&gWTPs[WTPID].address);

	//char dst[20];
	//inet_ntop(AF_INET,(void *)wWsm.u.DataChannel.WTPIP.m_v4addr,dst,20); 
	
	len = sizeof(wWsm);
	//wid_syslog_info("AsdWsm_DataChannelOp1\n");
	if (sendto(sock, &wWsm, len, 0, (struct sockaddr *) &toWSM.addr, toWSM.addrlen) < 0)
	{	
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wWSMSocket)");
		wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_DataChannelOp2 WTPID [%d]\n",WTPID);
////		close(sock);
		return CW_TRUE;
		if(op == WID_ADD)
		{
			msgq msg1;			
			struct msgqlist *elem1 = NULL;
			memset((char*)&msg1, 0, sizeof(msg1));
			
			gWTPs[WTPID].isRequestClose = CW_TRUE;	
			msg1.mqid = WTPID%THREAD_NUM+1;
			msg1.mqinfo.WTPID = WTPID;
			msg1.mqinfo.type = CONTROL_TYPE;
			msg1.mqinfo.subtype = WTP_S_TYPE;
			msg1.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
						
			elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if(elem1 == NULL)
			{			
				perror("malloc");
				return 0;
			}
			
			memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));			
			elem1->next = NULL;
			memcpy((char*)&(elem1->mqinfo),(char*)&(msg1.mqinfo),sizeof(msg1.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPID, elem1);
		}
		return CW_FALSE;		
	}
	//wid_syslog_info("AsdWsm_DataChannelOp3\n");
//	close(sock);
	return CW_TRUE;
}

CWBool WIDWsm_VRRPIFOp(unsigned char* name,unsigned int ip, unsigned int op){

	TableMsg wWsm;
	int len;
	int i=0;	
	int sock = sockPerThread[i];
	wWsm.Op = VRRP_IF;
	wWsm.u.vrrp_if.op = op;
	wWsm.u.vrrp_if.ip.addr_family = AF_INET;
	wWsm.u.vrrp_if.ip.u.ipv4_addr = ip;
	memcpy(wWsm.u.vrrp_if.if_name, name ,strlen((char *)name));
		
	len = sizeof(wWsm);
	if (sendto(sock, &wWsm, len, 0, (struct sockaddr *) &toWSM.addr, toWSM.addrlen) < 0){	
		perror("send(wWSMSocket)");
		return CW_TRUE;
	}
	return CW_TRUE;
}

CWBool WIDWsm_VRRPIFOp_IPv6(struct ifi_info *ifi, unsigned int op)
{
	TableMsg wWsm;
	int len;
	int i=0;	
	int sock = sockPerThread[i];
	wWsm.Op = VRRP_IF;
	wWsm.u.vrrp_if.op = op;
	//wWsm.u.vrrp_if.ip.addr_family = AF_INET;
	//wWsm.u.vrrp_if.ip.u.ipv4_addr = ip;
	copymixwtpip(&wWsm.u.vrrp_if.ip,ifi->ifi_addr6);
	memcpy(wWsm.u.vrrp_if.if_name, ifi->ifi_name, IFI_NAME);
		
	len = sizeof(wWsm);
	if (sendto(sock, &wWsm, len, 0, (struct sockaddr *) &toWSM.addr, toWSM.addrlen) < 0){	
		perror("send(wWSMSocket)");
		return CW_TRUE;
	}
	return CW_TRUE;

}

int WIDBAKInfoToASD(unsigned int state,struct sockaddr_in *ipaddr, unsigned int virIP,unsigned int vrrid,char *name, Operate op){

	TableMsg AsdCMD;
	int len = 0;
	//int ret;	
	int sndbuf = SOCK_BUFSIZE;
	int sock = socket(PF_UNIX, SOCK_DGRAM, 0);	
	if(sock < 0) 
	{
		wid_syslog_err("open socket error when %s(line %d)\n", __func__, __LINE__);
		return CW_FALSE;
	}
	if ((setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf))) < 0) 
	{	
		wid_syslog_crit("%s setsockopt %s",__func__,strerror(errno));
		perror("setsockopt");

		/*coverity 10147 memory leak*/
		close(sock);
		
		return CW_FALSE;
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);
	AsdCMD.Type = BAK_TYPE;
	AsdCMD.Op = op;
	AsdCMD.u.BAK.vrrid = vrrid;
	AsdCMD.u.BAK.state = state;
	AsdCMD.u.BAK.virip = virIP;
	memcpy(&AsdCMD.u.BAK.ipaddr,ipaddr,sizeof(struct sockaddr_in));
	memcpy(AsdCMD.u.BAK.virname,name,strlen((char *)name));
	len = sizeof(AsdCMD);
	wid_syslog_debug_debug(WID_MB,"WIDBAKInfoToASD1\n");
	if(sendto(sock, &AsdCMD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("WIDBAKInfoToASD2\n");
		close(sock);
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_MB,"WIDBAKInfoToASD3\n");
	close(sock);
	return 0;
}

int WIDLocalBAKInfoToASD(unsigned int state,int slotID,unsigned int vrrid, Operate op){

	TableMsg AsdCMD;
	int len;
	//int ret;	
	int sndbuf = SOCK_BUFSIZE;
	int sock = -1;
	sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		return -1;
	}
	
	if ((setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf))) < 0) {	
		wid_syslog_crit("%s setsockopt %s",__func__,strerror(errno));
		/* coverity-CID: 10202 10933 Resource leak */ 
		close(sock);
		return -1;
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);
	AsdCMD.Type = LOCAL_BAK_TYPE;
	AsdCMD.Op = op;
	AsdCMD.u.BAK.vrrid = vrrid;
	AsdCMD.u.BAK.state = state;
	AsdCMD.u.BAK.neighbor_slotid = slotID;
	len = sizeof(AsdCMD);
	wid_syslog_debug_debug(WID_MB,"WIDBAKInfoToASD1\n");
	if(sendto(sock, &AsdCMD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("WIDBAKInfoToASD2\n");
		close(sock);
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_MB,"WIDBAKInfoToASD3\n");
	close(sock);

	return 0;
}


int WIDBAKOneBSSInfoToASD(unsigned int vrrid,unsigned int BSSIndex,Operate op){

	TableMsg AsdCMD;
	int len;
	//int ret;	
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if(!check_bssid_func(BSSIndex)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}
	int i = (BSSIndex/L_BSS_NUM/L_RADIO_NUM)%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&AsdCMD, 0, sizeof(AsdCMD));
	AsdCMD.Type = BAK_TYPE;
	AsdCMD.Op = op;
	AsdCMD.u.BAK.vrrid = vrrid;
	AsdCMD.u.BAK.BSSIndex = BSSIndex;
	len = sizeof(AsdCMD);
	wid_syslog_debug_debug(WID_MB,"WIDBAKOneBSSInfoToASD1\n");
	if(sendto(sock, &AsdCMD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("WIDBAKOneBSSInfoToASD2\n");
	//	close(sock);
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_MB,"WIDBAKOneBSSInfoToASD3\n");
	return 0;
	//close(sock);
}

//mahz add 2011.6.23
int WIDBAKBSSInfoToASD(unsigned int vrrid,unsigned int *bssindex,unsigned int count,Operate op){

	TableMsg AsdCMD;
	int len = 0;
	int j = 0;
	int sndbuf = SOCK_BUFSIZE;
	
	int sock = socket(PF_UNIX, SOCK_DGRAM, 0);	
	if(sock < 0) 
	{
		wid_syslog_err("open socket error when %s(line %d)\n", __func__, __LINE__);
		return CW_FALSE;
	}
	if ((setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf))) < 0) 
	{	
		wid_syslog_err("%s setsockopt %s",__func__,strerror(errno));
		perror("setsockopt");

		/*coverity 10146 memory leak*/
		close(sock);
		
		return CW_FALSE;
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);

	memset(&AsdCMD, 0, sizeof(AsdCMD));

	for(j=0; j<count; j++){
		AsdCMD.u.BAK.bssindex[j] = bssindex[j];
	}
	AsdCMD.Type = BAK_TYPE;
	AsdCMD.Op = op;
	AsdCMD.u.BAK.vrrid = vrrid;
	AsdCMD.u.BAK.bss_count = count;
	len = sizeof(AsdCMD);
	
	if(sendto(sock, &AsdCMD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_err("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		close(sock);
		return CW_FALSE;
	}
	
	wid_syslog_debug_debug(WID_MB,"$s success\n",__func__);
	close(sock);

	return 0;
}

CWBool wid_to_wsm_bss_pkt_info_get(unsigned int wtpindex)
{

	TableMsg wWsm;
	int len;	
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = wtpindex%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wWsm, 0, sizeof(wWsm));
	
	wWsm.Op = BSS_INFO;
	wWsm.Type = BSS_PKT_TYPE;
	wWsm.u.bss_header.WTPID = wtpindex;
	wWsm.u.bss_header.bss_cnt = 0;
	len = sizeof(wWsm);
	wid_syslog_debug_debug(WID_DEFAULT,"wid_to_wsm_bss_pkt_info_get1\n");
	if (sendto(sock, &wWsm, len, 0, (struct sockaddr *) &toWSM.addr, toWSM.addrlen) < 0){	
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wWSMSocket)");
////		close(sock);
		wid_syslog_info("wid_to_wsm_bss_pkt_info_get2\n");
		return CW_FALSE;		
	}
	wid_syslog_debug_debug(WID_DEFAULT,"wid_to_wsm_bss_pkt_info_get3\n");
////	close(sock);
	return CW_TRUE;
}
//added by weiay 20080702
CWBool AsdWsm_StationOp(unsigned int WTPID,CWStationInfoValues *valuesPtr, Operate op)
{
	TableMsg wASD;
	int len;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wASD, 0, sizeof(wASD));
	
	wASD.Op = op;
	wASD.Type = STA_TYPE;
	wASD.u.STA.StaState = 0;
	wASD.u.STA.BSSIndex = 0;
	wASD.u.STA.WTPID = WTPID;
	//Copy mac address;
	CW_COPY_MEMORY(wASD.u.STA.STAMAC,valuesPtr->mac_addr,MAC_LEN);
	
	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_StationOp1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
	//	close(sock);
		wid_syslog_info("AsdWsm_StationOp2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_StationOp end***\n");
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_StationOp3\n");
	//close(sock);
	return CW_TRUE;	
}
//weichao add 2011.11.03
CWBool WidAsd_StationInfoUpdate1(unsigned int WTPID,WIDStationInfo*valuesPtr)
{
	TableMsg wASD;
	int len;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];
	wASD.Op = STA_FLOW_CHECK;
	wASD.Type = STA_TYPE;
	wASD.u.STA.WTPID = WTPID;	
 	 wASD.u.STA.rx_data_bytes = valuesPtr->rx_data_bytes;
  	 wASD.u.STA.tx_data_bytes = valuesPtr->tx_data_bytes;
   	 wASD.u.STA.rx_frames=  valuesPtr->rx_frames;
   	 wASD.u.STA.tx_frames=  valuesPtr->tx_frames;	
	 
	CW_COPY_MEMORY(wASD.u.STA.STAMAC,valuesPtr->mac,MAC_LEN);

	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"WidAsd_StationInfoUpdate1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("WidAsd_StationInfoUpdate2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_StationOp end***\n");
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_DEFAULT,"WidAsd_StationInfoUpdate  weichao\n");
	
	return CW_TRUE;	

}
//weichao add
CWBool AsdWsm_StationOpNew(unsigned int WTPID,unsigned char *mac, Operate op,unsigned short reason)
{
	TableMsg wASD;
	int len;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];
	memset(&wASD,0,sizeof(wASD));
	if(AC_WTP[WTPID]&&AC_WTP[WTPID]->ControlWait)
	{
		wASD.u.STAINFO[0].wlanId = AC_WTP[WTPID]->ControlWait->mqinfo.u.StaInfo.WLANID;
		wASD.u.STAINFO[0].radioId= AC_WTP[WTPID]->ControlWait->mqinfo.u.StaInfo.Radio_L_ID;		
		memcpy(wASD.u.STA.STAMAC,AC_WTP[WTPID]->ControlWait->mqinfo.u.StaInfo.STAMAC,MAC_LEN);
	}
	else 
		return CW_FALSE;
	wASD.u.STAINFO[0].WTPID = WTPID;	
	wASD.u.STAINFO[0].count = 1 ; 	
	wASD.Op = op;
	wASD.Type = STA_TYPE;
	wASD.u.STAINFO[0].authorize_failed = reason;
	wid_syslog_debug_debug(WID_WTPINFO,"in func %s :sta(%2X:%2X:%2X:%2X:%2X:%2X) authorize wrong!\n",__func__,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	if(mac !=NULL){
		memcpy(wASD.u.STA.STAMAC,mac,MAC_LEN);
	}
	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_StationOp**********\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("AsdWsm_StationOp2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_StationOp end***\n");
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_StationOp3\n");
	return CW_TRUE;	
}
CWBool AsdWsm_ap_report_sta_info(unsigned int WTPID,CWStationReportInfo *valuesPtr, Operate op)
{
	TableMsg wASD;
	int len;
	
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wASD, 0, sizeof(wASD));	
	wASD.Op = op;
	wASD.Type = AP_REPORT_STA_TYPE;
	wASD.u.STA.StaState = 0;
	wASD.u.STA.BSSIndex = 0;
	wASD.u.STA.WTPID = WTPID;
	wASD.u.STA.radioId = valuesPtr->radioId;
	wASD.u.STA.wlanId = valuesPtr->wlanId;
	//Copy mac address;
	CW_COPY_MEMORY(wASD.u.STA.STAMAC,valuesPtr->mac,MAC_LEN);
	if(valuesPtr->length == 16){
		wASD.u.STA.ipv6Address.s6_addr[i] = (valuesPtr->ipv6Address).s6_addr[i];
	}else{
		wASD.u.STA.ipv4Address = valuesPtr->ipv4Address;
	}
	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_StationOp_stain_reporet1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
	//	close(sock);
		wid_syslog_info("AsdWsm_StationOp_stain_reporet2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_ap_report_sta_info end***\n");
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_DEFAULT,"AsdWsm_StationOp_stain_reporet3\n");
	//close(sock);
	return CW_TRUE;	
}
CWBool WidAsd_StationInfoUpdate(unsigned int WTPID,unsigned int count,WIDStationInfo valuesPtr[64])
{
	TableMsg wASD;
	int len;
	int j = 0;
	if(!check_wtpid_func(WTPID))
	{
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];
	memset(&wASD, 0, sizeof(wASD));
	wASD.Op = STA_INFO;
	wASD.Type = STA_TYPE;
	for(j=0;j<count;j++){
		wASD.u.STAINFO[j].StaState = 0;
		wASD.u.STAINFO[j].BSSIndex = 0;
		wASD.u.STAINFO[j].WTPID = WTPID;
		wASD.u.STAINFO[j].count = count;
		wASD.u.STAINFO[j].radioId = valuesPtr[j].radioId;
		wASD.u.STAINFO[j].wlanId = valuesPtr[j].wlanId;
		wASD.u.STAINFO[j].mode = valuesPtr[j].mode;
		wASD.u.STAINFO[j].channel = valuesPtr[j].channel;
		wASD.u.STAINFO[j].rssi = valuesPtr[j].rssi;
		wASD.u.STAINFO[j].tx_Rate = valuesPtr[j].tx_Rate;
		wASD.u.STAINFO[j].isPowerSave = valuesPtr[j].isPowerSave;
		wASD.u.STAINFO[j].isQos = valuesPtr[j].isQos;
		
		wASD.u.STAINFO[j].rx_bytes = valuesPtr[j].rx_bytes;
		wASD.u.STAINFO[j].tx_bytes = valuesPtr[j].tx_bytes;

	    wASD.u.STAINFO[j].rx_data_bytes = valuesPtr[j].rx_data_bytes;
	    wASD.u.STAINFO[j].tx_data_bytes = valuesPtr[j].tx_data_bytes;
	    wASD.u.STAINFO[j].rx_data_frames = valuesPtr[j].rx_data_frames;
	    wASD.u.STAINFO[j].tx_data_frames = valuesPtr[j].tx_data_frames;
	    wASD.u.STAINFO[j].rx_frames=  valuesPtr[j].rx_frames;
	    wASD.u.STAINFO[j].tx_frames=  valuesPtr[j].tx_frames;
	    wASD.u.STAINFO[j].rx_frag_packets = valuesPtr[j].rx_frag_packets;
	    wASD.u.STAINFO[j].tx_frag_packets = valuesPtr[j].tx_frag_packets;
	    wASD.u.STAINFO[j].rx_Rate = valuesPtr[j].rx_Rate;
		
		CW_COPY_MEMORY(wASD.u.STAINFO[j].STAMAC,valuesPtr[j].mac,MAC_LEN);
	}

	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"WidAsd_StationInfoUpdate1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		//close(sock);
		wid_syslog_info("WidAsd_StationInfoUpdate2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_StationOp end***\n");
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_DEFAULT,"WidAsd_StationInfoUpdate3\n");
	//close(sock);
	
	return CW_TRUE;	
}

CWBool WidAsd_StationKeyNegotiReport(unsigned int WTPID, WIDStationInfo *valuesPtr)
{
	TableMsg wASD;
	int len = 0;
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];
	
	if(NULL == valuesPtr)
	{
		return CW_FALSE;
	}
	if(!check_wtpid_func(WTPID))
	{
		wid_syslog_err("%s invalid wtpid:%d\n", __func__, WTPID);
		return CW_FALSE;
	}

	memset(&wASD, 0, sizeof(wASD));
	wASD.Op = KEY_NEGOTI;
	wASD.Type = STA_TYPE;
	wASD.u.STA.WTPID = WTPID;	
 	wASD.u.STA.radioId = valuesPtr->radioId;
  	wASD.u.STA.wlanId = valuesPtr->wlanId;
   	wASD.u.STA.ptk_res = valuesPtr->ptk_res;		 
	CW_COPY_MEMORY(wASD.u.STA.STAMAC, valuesPtr->mac, MAC_LEN);

	if ((valuesPtr->wlanId < WLAN_NUM) && (NULL != AC_WLAN[valuesPtr->wlanId]))
	{
		wASD.u.STA.BSSIndex = AC_WLAN[valuesPtr->wlanId]->S_WTP_BSS_List[WTPID][valuesPtr->radioId];
	}
	
	len = sizeof(wASD);
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_info("%s sendto %s", __func__, strerror(errno));
		return CW_FALSE;
	}
	else
	{
		wid_syslog_info("%s sendto ASD sta %02X:%02X:%02X:%02X:%02X:%02X"
			" radio %d-%d wlan %d bssindex %d ptk result %d\n", __func__,
			valuesPtr->mac[0],valuesPtr->mac[1],valuesPtr->mac[2],
			valuesPtr->mac[3],valuesPtr->mac[4],valuesPtr->mac[5],
			WTPID, valuesPtr->radioId, valuesPtr->wlanId, wASD.u.STA.BSSIndex, valuesPtr->ptk_res);
	}
	return CW_TRUE; 
}


//weichao add
CWBool WidAsd_StationLeaveReport(unsigned int WTPID,unsigned int count,WIDStationInfo*valuesPtr)
{
	TableMsg wASD;
	int len;
	unsigned int j =0;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wASD,0,sizeof(wASD));
	
	wASD.Op = STA_LEAVE_REPORT;
	wASD.Type = STA_TYPE;
	wid_syslog_debug_debug(WID_DEFAULT,"in func %s:count  = %d\n",__func__,count);
	if(count != 255){
		for( j = 0 ; (j < count)&&(j < 20) ; j++){
			wASD.u.STAINFO[j].WTPID = WTPID;	
			wASD.u.STAINFO[j].count = count ; 
			
			wASD.u.STAINFO[j].wlanId= valuesPtr[j].wlanId;
			wASD.u.STAINFO[j].radioId = valuesPtr[j].radioId;
		 	 wASD.u.STAINFO[j].rx_data_bytes = valuesPtr[j].rx_data_bytes;
		  	 wASD.u.STAINFO[j].tx_data_bytes = valuesPtr[j].tx_data_bytes;
			 
		 	 wASD.u.STAINFO[j].rx_data_frames = valuesPtr[j].rx_data_frames;
		  	 wASD.u.STAINFO[j].tx_data_frames = valuesPtr[j].tx_data_frames;
			 
		 	 wASD.u.STAINFO[j].rx_frames = valuesPtr[j].rx_frames;
		  	 wASD.u.STAINFO[j].tx_frames = valuesPtr[j].tx_frames;
			 
		 	 wASD.u.STAINFO[j].rx_frag_packets = valuesPtr[j].rx_frag_packets;
		  	 wASD.u.STAINFO[j].tx_frag_packets = valuesPtr[j].tx_frag_packets;
			 
			 
			 wASD.u.STAINFO[j].sta_reason = valuesPtr[j].sta_reason;
			 wASD.u.STAINFO[j].sub_reason = valuesPtr[j].sub_reason;
			CW_COPY_MEMORY(wASD.u.STAINFO[j].STAMAC,valuesPtr[j].mac,MAC_LEN);
		}
	}
	else{
		wASD.u.STAINFO[0].count = count ; 
		wASD.u.STAINFO[0].WTPID = WTPID;	
		wASD.u.STAINFO[0].wlanId= valuesPtr[j].wlanId;
		wASD.u.STAINFO[0].radioId = valuesPtr[j].radioId;
		wASD.u.STAINFO[0].sta_reason = valuesPtr[j].sta_reason;
		wASD.u.STAINFO[0].sub_reason = valuesPtr[j].sub_reason;
	}
	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"WidAsd_StationInfoUpdateLEAVE\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("WidAsd_StationInfoUpdate2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_StationOp end***\n");
		return CW_FALSE;
	}
	
	return CW_TRUE;	

}
CWBool WidAsdStaWapiInfoUpdate(unsigned int WTPID,WIDStaWapiInfoList*valuesPtr)
{
	TableMsg wASD;
	int len;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];

	memset(&wASD, 0, sizeof(wASD));
	
	wASD.Op = STA_WAPI_INFO;
	wASD.Type = STA_TYPE;
	memcpy(&(wASD.u.StaWapi), valuesPtr, sizeof(WIDStaWapiInfoList));

	len = sizeof(wASD);
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		return CW_FALSE;
	}
	
	return CW_TRUE;	
}
CWBool wid_asd_bss_traffic_limit(unsigned int bssindex)
{
	TableMsg wASD;
	int len;
	if(!check_bssid_func(bssindex)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int i = (bssindex/L_BSS_NUM/L_RADIO_NUM)%SOCK_NUM;
	//int sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	int sock = sockPerThread[i];

	memset(&wASD, 0, sizeof(wASD));
	wASD.Op = TRAFFIC_LIMIT;
	wASD.Type = BSS_TRAFFIC_LIMIT_TYPE;
	wASD.u.traffic_limit.able = AC_BSS[bssindex]->traffic_limit_able;
	wASD.u.traffic_limit.bssindex = bssindex;
	wASD.u.traffic_limit.value = AC_BSS[bssindex]->traffic_limit;
	wASD.u.traffic_limit.average_value = AC_BSS[bssindex]->average_rate;
	wASD.u.traffic_limit.send_value = AC_BSS[bssindex]->send_traffic_limit;
	wASD.u.traffic_limit.send_average_value = AC_BSS[bssindex]->send_average_rate;

	len = sizeof(wASD);
	wid_syslog_debug_debug(WID_DEFAULT,"wid_asd_bss_traffic_limit1\n");
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
////		close(sock);
		wid_syslog_info("wid_asd_bss_traffic_limit2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error wid_asd_bss_traffic_limit end***\n");
		return CW_FALSE;
	}
	
	wid_syslog_debug_debug(WID_DEFAULT,"wid_asd_bss_traffic_limit3\n");
//	close(sock);
	return CW_TRUE;	
}/*sz xm0723*/

CWBool wid_asd_send_wids_able(unsigned int able)
{
	wid_syslog_debug_debug(WID_DEFAULT,"wid_asd_send_wids_able\n");
	//printf("wid_asd_send_wids_able\n");
	TableMsg wASD;
	int len;
	int sock = sockPerThread[0];
	
	memset(&wASD, 0, sizeof(wASD));
	wASD.Op = WIDS_SET;
	wASD.Type = WIDS_TYPE;
	wASD.u.WIDS_set.lasttime = glasttimeinblack;
	wASD.u.WIDS_set.able = able;
	len = sizeof(wASD);
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error wid_asd_send_wids_able end***\n");
		return CW_FALSE;
	}
	
	return CW_TRUE;	
}

CWBool wid_asd_send_wids_info(struct tag_wids_device_ele *info,unsigned int WTPID)
{
	wid_syslog_debug_debug(WID_DEFAULT,"wid_asd_send_wids_info\n");
	TableMsg wASD;
	int len;
	char BSSID[MAC_LEN];
	int i,j;
	unsigned int bssindex = 0;
	if(!check_wtpid_func(WTPID)){
		wid_syslog_err("%s\n",__func__);
		return CW_FALSE;
	}else{
	}
	int m = WTPID%SOCK_NUM;
	int sock = sockPerThread[m];
	
	memset(&wASD, 0, sizeof(wASD));
	
	memset(BSSID,0,MAC_LEN);
	memcpy(BSSID,info->vapbssid,MAC_LEN);
	for(i=0;i<L_RADIO_NUM;i++)
	{
		if(AC_WTP[WTPID]->WTP_Radio[i] != NULL)
		{
			for(j=0;j<L_BSS_NUM;j++)
			{
				if(AC_WTP[WTPID]->WTP_Radio[i]->BSS[j] != NULL)
				{
					if(memcmp(AC_WTP[WTPID]->WTP_Radio[i]->BSS[j]->BSSID,BSSID,MAC_LEN) == 0)
					{
						bssindex = AC_WTP[WTPID]->WTP_Radio[i]->BSS[j]->BSSIndex;
						break;
					}
				}
			}
		}
	}

	if(bssindex == 0)
	{
		wid_syslog_debug_debug(WID_DEFAULT,"wid_asd_send_wids_info bssindex 0\n");
		return CW_FALSE;
	}
	
	wASD.Op = WIDS_INFO;
	wASD.Type = WIDS_TYPE;
	wASD.u.WIDS_info.bssindex = bssindex;
	wASD.u.WIDS_info.attacktype = info->attacktype;
	wASD.u.WIDS_info.frametype = info->frametype;
	wASD.u.WIDS_info.channel = info->channel;
	wASD.u.WIDS_info.rssi = info->rssi;

	memset(wASD.u.WIDS_info.bssid,0,MAC_LEN);
	memcpy(wASD.u.WIDS_info.bssid,info->bssid,MAC_LEN);
	memset(wASD.u.WIDS_info.vapbssid,0,MAC_LEN);
	memcpy(wASD.u.WIDS_info.vapbssid,info->vapbssid,MAC_LEN);
	len = sizeof(wASD);
	if(sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error wid_asd_send_wids_info end***\n");
		return CW_FALSE;
	}
	
	return CW_TRUE;	
}


void CWCaptrue(int n ,unsigned char *buffer){
		int t=0;
		while((n-t)>=16)
		{
			int i;
			printf("[");
			for(i=0;i<16;i++)
				printf("%02x ",buffer[t+i]);
			printf("]\t[");
			for(i=0;i<16;i++)
			{
				char ch=buffer[t+i];
				if(isalnum(ch))
					printf("%c",ch);
				else
					printf(".");
			}
			printf("]\n");
			t+=16;
		}

		if(n>t)
		{
			int i=t;
			printf("[");
			while(i<n)
				printf("%02x ",buffer[i++]);
			printf("]");
			i=n-t;
			i=16-i;
			while(i--)
				printf("   ");
			printf("\t[");
			i=t;
			while(i<n)
			{
				char ch=buffer[i++];
				if(isalnum(ch))
					printf("%c",ch);
				else
					printf(".");
			}
			printf("]\n");
		}
		printf("\n\n");
}


void str2higher(char **str) {  
	int i,len;
	char *ptr;

	len = strlen(*str);
	ptr = *str;

	for(i=0; i<len ; i++) {
		if((ptr[i]<='z')&&(ptr[i]>='a'))  
			ptr[i] = ptr[i]-'a'+'A';  
	}
	
	return;
}

CWBool get_sock_descriper(int isystemindex, int* sockdes)
{
	//CWMultiHomedSocket tmp;
	*sockdes = -1;
	
	struct CWMultiHomedInterface *elist = gACSocket.interfaces;

	for( ; elist != NULL; elist = elist->if_next)
	{
		wid_syslog_debug_debug(WID_DEFAULT,"***get_sock_descriper::systemIndex is %d \n",elist->systemIndex);
		if(elist->systemIndex == isystemindex)//find sock descriper according to system index
		{
			*sockdes = elist->sock;
			return CW_TRUE;
		}
	}

	return CW_FALSE;
}

int file_check(char *dir)
{
	FILE *dp = NULL;
	dp = fopen(dir,"rb");
	if(dp == NULL)
	{
		return 0;
	}
	fclose(dp);
	return 1;
}


int wid_illegal_character_check(char *str , int len, int modify){
	int m = 0;
	char * tmp;
	int ret = 1;
	tmp = str;
	if(str == NULL){
		return -1;
	}
	if(strlen(str) != len){
		return -1;
	}
	for(m = 0; m < len; m++){
		if((tmp[m] < 32)||(tmp[m] > 126)){
			if(modify){
				tmp[m] = 42;
				ret = 2;
				continue;
			}else{
				break;
			}
		}
	}
	if(m == len){
		return ret;
	}else{
		return 0;
	}
}

#define WTP_MAC_HASH(wtp) (wtp[2]+wtp[3]+wtp[4]+wtp[5])

struct conflict_wtp_info * wid_get_wtp(struct wid_wtp_info *if_wid, unsigned char *wtp)
{
	struct conflict_wtp_info *s;

	s = if_wid->wtp_hash[WTP_MAC_HASH(wtp)];
	while (s != NULL && memcmp(s->wtpmac, wtp, 6) != 0)
		s = s->hnext;
	return s;
}
struct conflict_wtp_info * wid_update_wtp(struct conflict_wtp_info * tmp,struct wtp_con_info * con_info)
{
	struct ConflictWtp *idinfo = NULL;
	struct ConflictWtp *tmpInfo = NULL;
	if(tmp == NULL)
		return NULL;
	
	idinfo = (struct ConflictWtp*)malloc(sizeof(struct ConflictWtp));
	if(NULL == idinfo)
	{
		return NULL;
	}
	memset(idinfo,0,sizeof(struct ConflictWtp));
	
	tmpInfo = tmp->wtpindexInfo;
	if(tmpInfo == NULL)
	{
		/* coverity-CID: 10934 Resource leak */ 
		
		free(idinfo);
		return NULL;
	}
	wid_syslog_info("%s,%d,con_info->wtpindex=%d,con_info->wtpindex2=%d.\n",__func__,__LINE__,con_info->wtpindex,con_info->wtpindex2);
	while(tmpInfo != NULL){
		wid_syslog_info("%s,%d,tmpInfo->wtpindex=%d.\n",__func__,__LINE__,tmpInfo->wtpindex);
		if((tmpInfo->wtpindex == con_info->wtpindex)){
			idinfo->wtpindex = con_info->wtpindex2;
			wid_syslog_info("%s,%d,con_info->wtpindex=%d,con_info->wtpindex2=%d.\n",__func__,__LINE__,con_info->wtpindex,con_info->wtpindex2);
			break;
		}else if((tmpInfo->wtpindex == con_info->wtpindex2)){
			idinfo->wtpindex = con_info->wtpindex;
			wid_syslog_info("%s,%d,con_info->wtpindex=%d,con_info->wtpindex2=%d.\n",__func__,__LINE__,con_info->wtpindex,con_info->wtpindex2);
			break;
		}
		tmpInfo = tmpInfo->next;
	}
	idinfo->next = tmp->wtpindexInfo;
	tmp->wtpindexInfo = idinfo;
	tmp->conf_num ++;
	return tmp;
}

struct conflict_wtp_info * wid_del_conflict_wtpinfo(struct wtp_con_info * con_info)
{
	struct ConflictWtp *tmpInfo = NULL;
	struct ConflictWtp *tmpInfo_next = NULL;
	struct conflict_wtp_info * tmp = NULL;
	tmp = wid_get_wtp(&allif, con_info->wtpmac);
	wid_syslog_debug_debug(WID_DEFAULT,"%s,%d.\n",__func__,__LINE__);
	if(tmp == NULL)
		return NULL;
	tmpInfo = tmp->wtpindexInfo;
	if(tmpInfo == NULL)
		return NULL;
	tmpInfo_next = tmpInfo->next;
	if((tmpInfo->wtpindex == con_info->wtpindex)){
		tmp->wtpindexInfo = tmpInfo->next;
		free(tmpInfo);
		tmpInfo = NULL;
		tmp->conf_num -- ;
		//return;
	}else if((tmpInfo->wtpindex == con_info->wtpindex2)){
		tmp->wtpindexInfo = tmpInfo->next;
		free(tmpInfo);
		tmpInfo = NULL;
		tmp->conf_num -- ;
		//return;
	}else{
		wid_syslog_debug_debug(WID_DEFAULT,"%s,%d,tmp->conf_num=%d.\n",__func__,__LINE__,tmp->conf_num);
		while((tmpInfo_next != NULL)&&(tmp->conf_num > 0)){
			wid_syslog_debug_debug(WID_DEFAULT,"%s,%d,tmpInfo_next=%p,tmpInfo_next->wtpindex=%d,con_info->wtpindex=%d.\n",__func__,__LINE__,tmpInfo_next,tmpInfo_next->wtpindex,con_info->wtpindex);
			if((tmpInfo_next->wtpindex == con_info->wtpindex)){
				break;
			}else if((tmpInfo_next->wtpindex == con_info->wtpindex2)){
				break;
			}
			tmpInfo_next = tmpInfo_next->next;
			tmpInfo = tmpInfo->next;
		}
		wid_syslog_debug_debug(WID_DEFAULT,"%s,%d,tmpInfo_next=%p.\n",__func__,__LINE__,tmpInfo_next);
		if(tmpInfo_next != NULL){
			tmpInfo->next = tmpInfo_next->next;
			tmpInfo_next->next = NULL;
			free(tmpInfo_next);
			tmpInfo_next = NULL;
			tmp->conf_num -- ;
			wid_syslog_debug_debug(WID_DEFAULT,"%s,%d.\n",__func__,__LINE__);
		}
	}
	wid_syslog_debug_debug(WID_DEFAULT,"%s,%d,tmp->wtpindexInfo=%p,tmp->conf_num=%d.\n",__func__,__LINE__,tmp->wtpindexInfo,tmp->conf_num);
	if((tmp->wtpindexInfo == NULL)||(tmp->conf_num == 0)||(tmp->conf_num == 1)){
		wid_syslog_debug_debug(WID_DEFAULT,"%s,%d.\n",__func__,__LINE__);
		wid_del_wtp(con_info);
	}
	return tmp;
}

static void if_wtp_list_del(struct wid_wtp_info *if_wid, struct conflict_wtp_info *wtp)
{
	struct conflict_wtp_info *tmp;

	if (if_wid->wtp_list == wtp) {
		if_wid->wtp_list = wtp->next;
		return;
	}

	tmp = if_wid->wtp_list;
	while (tmp != NULL && tmp->next != wtp)
		tmp = tmp->next;
	if (tmp == NULL) {

	} else
		tmp->next = wtp->next;
}


void if_wtp_hash_add(struct wid_wtp_info *if_wid, struct conflict_wtp_info *wtp)
{
	wtp->hnext = if_wid->wtp_hash[WTP_MAC_HASH(wtp->wtpmac)];
	if_wid->wtp_hash[WTP_MAC_HASH(wtp->wtpmac)] = wtp;
}


static void if_wtp_hash_del(struct wid_wtp_info *if_wid, struct conflict_wtp_info *wtp)
{
	struct conflict_wtp_info *s;

	s = if_wid->wtp_hash[WTP_MAC_HASH(wtp->wtpmac)];
	if (s == NULL) return;
	if (memcmp(s->wtpmac, wtp->wtpmac, 6) == 0) {
		if_wid->wtp_hash[WTP_MAC_HASH(wtp->wtpmac)] = s->hnext;
		return;
	}

	while (s->hnext != NULL &&
	       memcmp(s->hnext->wtpmac, wtp->wtpmac, ETH_ALEN) != 0)
		s = s->hnext;
	if (s->hnext != NULL)
		s->hnext = s->hnext->hnext;
}

struct conflict_wtp_info* wid_add_wtp(struct wtp_con_info * con_info)
{
	struct conflict_wtp_info * tmp = NULL;
	struct ConflictWtp *node1=NULL,*node2 = NULL;
	tmp = wid_get_wtp(&allif, con_info->wtpmac);
	if(tmp != NULL)
	{
		wid_update_wtp(tmp,con_info);
		return tmp;
	}
	tmp =(struct conflict_wtp_info*)malloc(sizeof(struct conflict_wtp_info));
	memset(tmp, 0, sizeof(struct conflict_wtp_info));
	tmp->next = allif.wtp_list;
	allif.wtp_list = tmp;
	memcpy(tmp->wtpmac, con_info->wtpmac, MAC_LEN);	
	
	if(tmp->wtpindexInfo == NULL)
	{		
		node1 = malloc(sizeof(struct ConflictWtp));
		memset(node1,0,sizeof(struct ConflictWtp));
		tmp->wtpindexInfo = node1;
		node1->wtpindex = con_info->wtpindex;
		tmp->conf_num ++;
		if((con_info->wtpindex != con_info->wtpindex2)
		    && (con_info->wtpindex > 0)
		    && (con_info->wtpindex2 > 0))
	    {
			node2 = malloc(sizeof(struct ConflictWtp));
			memset(node2,0,sizeof(struct ConflictWtp));
			node2->next = tmp->wtpindexInfo;
			tmp->wtpindexInfo = node2;
			node2->wtpindex = con_info->wtpindex2;
			tmp->conf_num ++;
		}
	}

	wid_syslog_info("%s,%d,tmp->tmp->wtpmac:%2X:%2X:%2X:%2X:%2X:%2X,con_info->wtpindex=%d.\n",__func__,__LINE__,tmp->wtpmac[0],tmp->wtpmac[1],tmp->wtpmac[2],tmp->wtpmac[3],tmp->wtpmac[4],tmp->wtpmac[5],con_info->wtpindex);
	if_wtp_hash_add(&allif, tmp);
	allif.list_len ++;
	return tmp;
}

int wid_del_wtp(struct wtp_con_info * con_info)
{
	struct conflict_wtp_info * tmp = NULL;
	tmp = wid_get_wtp(&allif, con_info->wtpmac);
	if(tmp != NULL)
	{
		if_wtp_list_del(&allif, tmp);
		if_wtp_hash_del(&allif, tmp);
		wid_syslog_info("%s,%d,del wtp.\n",__func__,__LINE__);
		free(tmp);
		tmp = NULL;
	}
	allif.list_len --;
	return 0;
}


__inline__ unsigned int CWGetWtpSeqNum(unsigned int wtpid) 
{
	unsigned int r;

	r = gWTPs[wtpid].SeqNum;
	if (CW_MAX_SEQ_NUM == gWTPs[wtpid].SeqNum)
	{
		gWTPs[wtpid].SeqNum = 0;
	}
	else
	{
		gWTPs[wtpid].SeqNum++;
	}
	
	return r;
}


/*
	parameter: enable  support 0 :disable or 1 : enable
*/
unsigned int wid_to_asd_dynamic_vlan_state(unsigned int wlanid,unsigned int enable)
{
	TableMsg wASD;
	int len = 0;
	int sock = -1;
	
	if (!check_wlanid_func(wlanid) || NULL == AC_WLAN[wlanid])
	{
		return 1;
	}
	
	sock = sockPerThread[1];
	if(sock < 0)
	{   
		wid_syslog_crit("%s: create sock failed: %s\n", __func__, strerror(errno));
		return 1;
    }     

	memset(&wASD, 0, sizeof(TableMsg));
	wASD.Type = DYNAMIC_VLAN_TYPE;
	wASD.Op = DYNAMIC_VLAN_ENABLE;
	wASD.u.dvlan_enable.wlanid = wlanid;
	wASD.u.dvlan_enable.enable= enable;
	len = sizeof(wASD);
	wid_syslog_info("wlan: %d dynamic vlan: %s to asd\n",
				 wASD.u.dvlan_enable.wlanid, wASD.u.dvlan_enable.enable? "enable":"disable");
				 
	if (sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_crit("%s sendto %s",__func__,strerror(errno));
		return 1;
	}
	
	return 0;	
}

unsigned int wid_to_asd_dynamic_vlan_list(unsigned int wlanid)
{
	TableMsg wASD;
	int len = 0;
	int sock = sockPerThread[1];
	if(sock < 0)
	{   
		wid_syslog_crit("%s: create sock failed: %s\n", __func__, strerror(errno));
    	return 1;   
	}     
	
	if (!check_wlanid_func(wlanid) ||NULL == AC_WLAN[wlanid])
	{
		return 1;
	}
	
	memset(&wASD, 0, sizeof(TableMsg));
	wASD.Type = DYNAMIC_VLAN_TYPE;
	wASD.Op = DYNAMIC_VLAN_UPDATE;
	wASD.u.dvlan_msg.wlanid = wlanid;
	wASD.u.dvlan_msg.cnt = AC_WLAN[wlanid]->dvlan.cnt;
	memcpy(wASD.u.dvlan_msg.vlan, AC_WLAN[wlanid]->dvlan.vlan, AC_WLAN[wlanid]->dvlan.cnt*sizeof(unsigned short));
	len = sizeof(wASD);
	wid_syslog_info("wlan:%d dynamic-vlan list cnt: %d to asd\n",wASD.u.dvlan_msg.wlanid, wASD.u.dvlan_msg.cnt);
	
	if (sendto(sock, &wASD, len, 0, (struct sockaddr *) &toASD.addr, toASD.addrlen) < 0)
	{
		wid_syslog_crit("%s sendto %s",__func__,strerror(errno));
		return 1;
	}
	
	return 0;	
}

struct sta_info *ap_get_sta(unsigned int bssindex, const u8 *sta)
{
	struct sta_info *s = NULL;
    if(NULL == AC_BSS[bssindex])
    {
        return NULL;
    }
	s = AC_BSS[bssindex]->sta_hash[STA_HASH(sta)];
	while (s != NULL && os_memcmp(s->addr, sta, MAC_LEN) != 0)
	{
	    s = s->hnext;
	}
	return s;
}

void ap_sta_hash_add(unsigned int bssindex, struct sta_info *sta)
{
    if(AC_BSS[bssindex])
    {
    	sta->hnext = AC_BSS[bssindex]->sta_hash[STA_HASH(sta->addr)];
    	AC_BSS[bssindex]->sta_hash[STA_HASH(sta->addr)] = sta;
    }
    else
    {
        wid_syslog_err("%s:%d AC_BSS[bssindex] = NULL\n",__func__,__LINE__,bssindex);
    }
}


void ap_sta_hash_del(unsigned int bssindex, struct sta_info *sta)
{
	struct sta_info *s = NULL;
    if(NULL == AC_BSS[bssindex])
    {
        return;
    }

	s = AC_BSS[bssindex]->sta_hash[STA_HASH(sta->addr)];
	if (s == NULL)
	{
	    return;
	}
	if (os_memcmp(s->addr, sta->addr, 6) == 0)
	{
		AC_BSS[bssindex]->sta_hash[STA_HASH(sta->addr)] = s->hnext;
		return;
	}

	while (s->hnext != NULL && os_memcmp(s->hnext->addr, sta->addr, ETH_ALEN) != 0)
	{
	    s = s->hnext;
	}
	if (s->hnext != NULL)
	{
	    s->hnext = s->hnext->hnext;
	}
	else
		wid_syslog_err("%s:%d AP: could not remove STA " MACSTR " from hash table", MAC2STR(sta->addr));
}

void ap_sta_list_del(unsigned int bssindex, struct sta_info *sta)
{
	struct sta_info *tmp = NULL;
    if(NULL == AC_BSS[bssindex])
    {
        return;
    }

	if (AC_BSS[bssindex]->sta_list == sta)
	{
		AC_BSS[bssindex]->sta_list = sta->next;
		return;
	}

	tmp = AC_BSS[bssindex]->sta_list;
	while (tmp != NULL && tmp->next != sta)
	{
	    tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		wid_syslog_err("Could not remove STA " MACSTR " from list.", MAC2STR(sta->addr));
	}
	else
	{
	    tmp->next = sta->next;
	}    
}

void ap_free_sta(unsigned int bssindex, struct sta_info *sta)
{
	unsigned int wtpid = 0;
	
	if(sta == NULL)
	{
		wid_syslog_err("%s:%d sta is NULL\n",__func__,__LINE__);
		return ;
	}
	
	WID_CHECK_BSS_STANDARD(bssindex);
	WID_CHECK_BSS_EXIST(bssindex);
	wtpid = AC_BSS[bssindex]->Radio_G_ID/L_RADIO_NUM;
	
	ap_sta_hash_del(bssindex, sta);
	ap_sta_list_del(bssindex, sta);
	destroy_sta_info(sta);
	
	AC_BSS[bssindex]->bss_accessed_sta_num--;
	AC_WLAN[AC_BSS[bssindex]->WlanID]->wlan_accessed_sta_num--;
	AC_WTP[wtpid]->wtp_accessed_sta_num--;   
}


void asd_free_stas(unsigned int bssindex)
{
	unsigned int wtpid = 0;
	struct sta_info *sta = NULL, *prev = NULL;
	
	WID_CHECK_BSS_STANDARD(bssindex);
	WID_CHECK_BSS_EXIST(bssindex);
	
	wtpid = AC_BSS[bssindex]->Radio_G_ID/L_RADIO_NUM;
	sta = AC_BSS[bssindex]->sta_list;
	while (sta)
	{
		prev = sta;
		sta = sta->next;
		wid_syslog_info("%s:%d afi%d bssindex%d removing sta["MACSTR"]\n",__func__,__LINE__,wtpid,bssindex,MAC2STR(prev->addr));
		
		ap_free_sta(bssindex, prev);
	}
}


struct asd_stainfo *wid_check_sta_roam(WID_STAINFO *sta)
{
	int i = 0;	
	int j = 0;
	int k = 0;
	unsigned int bssindex = 0;
	unsigned int roamnum = 0;
	unsigned int min_inactive_time = 0;
	unsigned int smin_inactive_time = 0;
	WID_STAINFO *sta_temp = NULL;
	struct asd_stainfo *stainfo = NULL;
	struct asd_stalist *phead = NULL;
	struct asd_stalist *ptemp = NULL;
	struct asd_stalist *prear = NULL;
	
	if (NULL == sta)
	{
		return NULL;
	}
	
	phead = (struct asd_stalist *)os_zalloc(sizeof(struct asd_stalist));
	if (phead == NULL)
	{
		return NULL;
	}
	memset(phead, 0, sizeof(struct asd_stalist));
	
	phead->next = NULL;
	stainfo = (struct asd_stainfo *)os_zalloc(sizeof(struct asd_stainfo));
	if (stainfo == NULL)
	{
		CW_FREE_OBJECT(phead);
		return NULL;
	}
	memset(stainfo, 0, sizeof(struct asd_stainfo));
	stainfo->sta = sta;
	stainfo->bss = AC_BSS[sta->BssIndex];
	phead->stainfo = stainfo;
	stainfo = NULL;
	prear = phead;
	min_inactive_time = sta->inactive_time;
	
	for (i = 0; i < WTP_NUM; i++)
	{
		if ((AC_WTP[i] != NULL) && (AC_WTP[i]->isused == 1))
		{
			for (j = 0; (j < AC_WTP[i]->RadioCount) && (j < L_RADIO_NUM); j++)
			{
				for (k = 0; k < L_BSS_NUM; k++)
				{
					if (AC_WTP[i]->WTP_Radio[j]->BSS[k] != NULL)
					{
						if (AC_WTP[i]->WTP_Radio[j]->BSS[k]->WlanID == 0)
						{
							continue;
						}
						
						bssindex = AC_WTP[i]->WTP_Radio[j]->BSS[k]->BSSIndex;
						if (sta->BssIndex == bssindex)
						{
							continue;
						}
						
						sta_temp = NULL;
						sta_temp = ap_get_sta(bssindex, sta->addr);
						if (sta_temp == NULL)
						{
							continue;
						}
						
						sta_temp->roam_type = STA_ROAM_TYPE_LEAVE;
						roamnum++;
						if (1 == roamnum)
						{
							sta->roam_type = STA_ROAM_TYPE_LEAVE;
							
							ptemp = (struct asd_stalist *)os_zalloc(sizeof(struct asd_stalist));
							if (ptemp == NULL)
							{
								CW_FREE_OBJECT(phead->stainfo);
								CW_FREE_OBJECT(phead);
								return NULL;
							}
							memset(ptemp, 0, sizeof(struct asd_stalist));
							ptemp->next = NULL;
							
							stainfo = (struct asd_stainfo *)os_zalloc(sizeof(struct asd_stainfo));
							if (stainfo == NULL)
							{
								CW_FREE_OBJECT(phead->stainfo);
								CW_FREE_OBJECT(phead);
								CW_FREE_OBJECT(ptemp);
								return NULL;
							}
							memset(stainfo, 0, sizeof(struct asd_stainfo));
							
							stainfo->sta = sta_temp;
							stainfo->bss = AC_WTP[i]->WTP_Radio[j]->BSS[k];
							ptemp->stainfo = stainfo;
							stainfo = NULL;
							
							if (sta_temp->inactive_time < min_inactive_time)
							{
								smin_inactive_time = min_inactive_time;
								min_inactive_time = sta_temp->inactive_time;
								phead = ptemp;
							}
							else
							{
								smin_inactive_time = sta_temp->inactive_time;
								prear = ptemp;
							}
							
							ptemp = NULL;
							phead->next = prear;
							
							continue;
						}
						
						if (sta_temp->inactive_time < min_inactive_time)
						{
							smin_inactive_time = min_inactive_time;
							min_inactive_time = sta_temp->inactive_time;
							prear->stainfo->sta = phead->stainfo->sta;
							prear->stainfo->bss = phead->stainfo->bss;
							phead->stainfo->sta = sta_temp;
							phead->stainfo->bss = AC_WTP[i]->WTP_Radio[j]->BSS[k];
						}
						else if (sta_temp->inactive_time < smin_inactive_time)
						{
							smin_inactive_time = sta_temp->inactive_time;
							
							prear->stainfo->sta = sta_temp;
							prear->stainfo->bss = AC_WTP[i]->WTP_Radio[j]->BSS[k];
						}
					}
				}
			}
		}
	}
	
	if ((NULL != phead) && (NULL != phead->next))
	{
		prear->stainfo->sta->roam_type = STA_ROAM_TYPE_LEAVE;
		phead->stainfo->sta->roam_type = STA_ROAM_TYPE_ACCESS;
		phead->stainfo->sta->PreBssIndex = prear->stainfo->sta->BssIndex;
		memcpy(phead->stainfo->sta->PreBSSID, prear->stainfo->sta->bssid, MAC_LEN);
		wid_syslog_info("%s:%d sta(%s)["MACSTR"] roam from bss%d["MACSTR"] to bss%d["MACSTR"]\n",
						__func__, __LINE__, WAI_STA_DEVTYPE_STR(phead->stainfo->sta->dev_type), MAC2STR(sta->addr),
						prear->stainfo->sta->BssIndex, MAC2STR(prear->stainfo->sta->bssid),
						phead->stainfo->sta->BssIndex, MAC2STR(phead->stainfo->sta->bssid));
		
		CW_FREE_OBJECT(prear->stainfo);
		CW_FREE_OBJECT(prear);
	}
	
	CW_FREE_OBJECT(phead->stainfo);
	CW_FREE_OBJECT(phead);
	
	return NULL;
}


void wid_check_sta_type(unsigned int bssindex, unsigned int *sta_type_num)
{
	unsigned int i = 0;
	struct sta_info *sta = NULL;
	
	WID_CHECK_BSS_STANDARD(bssindex);
	WID_CHECK_BSS_EXIST(bssindex);
	
	sta = AC_BSS[bssindex]->sta_list;
	while (sta)
	{
		for (i = 0; i < Sta_Type_Num; i++)
		{
			if (sta->dev_type == i)
			{
				sta_type_num[i]++;
			}
		}
		sta = sta->next;
	}
}

void get_sysruntime(time_t *sysruntime)
{
	struct timespec *sysrunt = NULL;;
	sysrunt = (struct timespec *)os_zalloc(sizeof(*sysrunt));
	if(sysrunt == NULL)
	{
		wid_syslog_err("%s,%d malloc error.\n",__func__,__LINE__);
		return;
	}
	clock_gettime(CLOCK_MONOTONIC,sysrunt);
	*sysruntime = sysrunt->tv_sec;
	CW_FREE_OBJECT(sysrunt);
}

