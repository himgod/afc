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
* ACMainLoop.c
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
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <syslog.h>
#include "wcpss/waw.h"
#include "dbus/wcpss/ACDbusDef1.h"
#include "dbus/asd/ASDDbusDef1.h"
#include "wcpss/wid/WID.h" 
#include "CWAC.h"
#include "AC.h"
#include "ACDbus.h"
#include "CWStevens.h"
#include "ACDbus_handler.h"
#include "ACImageData.h"
#include "ACBak.h"
#include "ACAccessCheck.h"
#include "ACLoadbanlance.h"
#include "AC.h"
#include "CWCommon.h"
#include "dbop.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

// index of the current thread in the global array
CWThreadSpecific gIndexSpecific;

int gCWWaitJoin = CW_WAIT_JOIN_DEFAULT;

CW_THREAD_RETURN_TYPE CWManageWTP(void *arg);
CW_THREAD_RETURN_TYPE CWManageTimers(void *arg);
void CWCriticalTimerExpiredHandler(int arg);
void CWSoftTimerExpiredHandler(int arg);

void CWACManageIncomingPacket(CWSocket sock, char *buf, int len, int incomingInterfaceIndex, int BindingSystemIndex, CWNetworkLev4Address *addrPtr,char *ifname);
void _CWCloseThread(int i);
void CWResetWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager);
__inline__ CWWTPManager *CWWTPByName(const char *addr);
__inline__ CWWTPManager *CWWTPByAddress(CWNetworkLev4Address *addressPtr, CWSocket sock, int *WTPID);
void CWDownWTP(unsigned int WTPIndex);
WIDAUTOAPINFO *parse_dynamic_wtp_login_situation(CWWTPVendorInfos * valPtr);
CWBool WidParseJoinRequestMessageForAutoApLogin(char *msg, int len, int *seqNumPtr, CWProtocolJoinRequestValues *valuesPtr);
void WidDestroyJoinRequestValuesForAutoApLogin(CWProtocolJoinRequestValues *valPtr);
int wid_parse_wtp_model_rmodel(WIDAUTOAPINFO *info);
int wid_auto_ap_mac_filter(unsigned char *mac);
int wid_auto_ap_binding(int wtpid,int ifindex);
void WidDestroyAutoApLoginInfo(WIDAUTOAPINFO *info);
int parse_dynamic_wtp_name(char *name);
int wid_dynamic_change_wtp_info(int wtpid,WIDAUTOAPINFO *info);
void wid_wtp_param_init(int WTPIndex, unsigned char flag);
CWBool AsdWsm_WTPOp(unsigned int WtpID,Operate op);
int wid_wtp_login_loadconfig(unsigned int wtpid);

int CWWTP_control_config(unsigned int wtpid);
extern CWConfigVersionXml SOFT_XMLINFO[CW_MODEL_COUNT];

typedef struct {
	int index;
	CWSocket sock;
	int interfaceIndex;
} CWACThreadArg; // argument passed to the thread func

CWBool WID_WTP_INIT(void *arg)
{
	int i = ((CWACThreadArg*)arg)->index;
	CWSocket sock = ((CWACThreadArg*)arg)->sock;
	int interfaceIndex = ((CWACThreadArg*)arg)->interfaceIndex;
	unsigned int flag = 0;
	CW_FREE_OBJECT(arg);
	if (!check_wtpid_func(i))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"init check id illegal\n", \
				WAI_FUNC_LINE_LITERAL,i);
		return CW_FALSE;
	}
		
	if (!CWErr(CWThreadMutexLock(&gActiveWTPsMutex)))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"init take thread mutex fail\n", \
				WAI_FUNC_LINE_LITERAL,i);
		return CW_FALSE;
	}
	if (CW_BAK_RUN != gWTPs[i].currentState)
	{
		gActiveWTPs++;
		CWThreadMutexLock(&ACLicense);
		flag = g_wtp_count[gWTPs[i].oemoption]->flag;
		if (0 == flag)
		{
			if (g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count >= g_wtp_count[gWTPs[i].oemoption]->gmax_wtp_count)
			{
				CWThreadMutexUnlock(&ACLicense);
				CWThreadMutexUnlock(&gActiveWTPsMutex);
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"license count %u excess spec %u deny access\n", WAI_FUNC_LINE_LITERAL, i,  \
							g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count,  g_wtp_count[gWTPs[i].oemoption]->gmax_wtp_count);
				return CW_FALSE;
			}
		}
		else
		{
			if (g_wtp_binding_count[flag]->gcurrent_wtp_count >= g_wtp_binding_count[flag]->gmax_wtp_count)
			{
				CWThreadMutexUnlock(&ACLicense);
				CWThreadMutexUnlock(&gActiveWTPsMutex);
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"oem license count %u excess spec %u deny access\n", WAI_FUNC_LINE_LITERAL, i,  \
							g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count, g_wtp_count[gWTPs[i].oemoption]->gmax_wtp_count);
				return CW_FALSE;
			}
		}
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"oem %u license count %u  match spec %u \n", WAI_FUNC_LINE_LITERAL, i,  \
					gWTPs[i].oemoption,g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count,   \
					g_wtp_count[gWTPs[i].oemoption]->gmax_wtp_count);
		(g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count)++;
		
		if (g_wtp_count[gWTPs[i].oemoption]->flag != 0)
		{
			(g_wtp_binding_count[flag]->gcurrent_wtp_count)++;
		}
		CWThreadMutexUnlock(&ACLicense);
	}
	
	if ((interfaceIndex < gMaxInterfacesCount) && (interfaceIndex >= 0))
	{
		gInterfaces[interfaceIndex].WTPCount++;
		CWUseSockNtop(((struct sockaddr*) &(gInterfaces[interfaceIndex].addr)),
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"add to interface %s now has %u afi\n", \
							WAI_FUNC_LINE_LITERAL, i, str, gInterfaces[interfaceIndex].WTPCount););
	}
	else
	{
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"add to interface with index %d illegal\n", \
					WAI_FUNC_LINE_LITERAL, i, interfaceIndex);
	}

	CWThreadMutexUnlock(&gActiveWTPsMutex);

	CWACInitBinding(i);
	
	gWTPs[i].interfaceIndex = interfaceIndex;
	gWTPs[i].socket = sock;
#ifdef CW_NO_DTLS
	AC_WTP[i]->CTR_ID = sock;
#endif
	gWTPs[i].fragmentsList = NULL;
	gWTPs[i].currentState = CW_ENTER_JOIN; // we're in the join state for this session
	AC_WTP[i]->WTPStat = WID_JOIN;
	gWTPs[i].isNotFree = CW_TRUE;
	gWTPs[i].subState = CW_DTLS_HANDSHAKE_IN_PROGRESS;
		
	/**** ACInterface ****/
	gWTPs[i].interfaceCommandProgress = CW_FALSE;
	gWTPs[i].interfaceCommand = NO_CMD;
	CWDestroyThreadMutex(&gWTPs[i].interfaceMutex); 
	CWCreateThreadMutex(&gWTPs[i].interfaceMutex);
	CWDestroyThreadMutex(&gWTPs[i].WTPThreadMutex); 
	CWCreateThreadMutex(&gWTPs[i].WTPThreadMutex);
	CWDestroyThreadMutex(&gWTPs[i].RRMThreadMutex); 
	CWCreateThreadMutex(&gWTPs[i].RRMThreadMutex);
	CWDestroyThreadMutex(&gWTPs[i].WIDSThreadMutex); 
	CWCreateThreadMutex(&gWTPs[i].WIDSThreadMutex);
	CWDestroyThreadMutex(&gWTPs[i].interfaceSingleton); 
	CWCreateThreadMutex(&gWTPs[i].interfaceSingleton);
	CWDestroyThreadCondition(&gWTPs[i].interfaceWait);	
	CWCreateThreadCondition(&gWTPs[i].interfaceWait);
	CWDestroyThreadCondition(&gWTPs[i].interfaceComplete);	
	CWCreateThreadCondition(&gWTPs[i].interfaceComplete);
	CWDestroyThreadMutex(&gWTPs[i].WTPThreadControllistMutex); 
	CWCreateThreadMutex(&gWTPs[i].WTPThreadControllistMutex);
	CWDestroyThreadMutex(&gWTPs[i].mutex_controlList);
	CWCreateThreadMutex(&gWTPs[i].mutex_controlList);
	gWTPs[i].qosValues = NULL;
	/**** ACInterface ****/

	gWTPs[i].messages = NULL;
	gWTPs[i].messagesCount = 0;
	gWTPs[i].isRetransmitting = CW_FALSE;
	gWTPs[i].retransmissionCount = 0;
	
	CWResetWTPProtocolManager(&(gWTPs[i].WTPProtocolManager));
		
	if (!CWErr(CWTimerRequest(gCWWaitJoin, &(gWTPs[i].thread), &(gWTPs[i].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,i)))  // start WaitJoin timer
	{
		_CWCloseThread(i);
	}

#ifndef CW_NO_DTLS

	if (!CWErr(CWSecurityInitSessionServer(&gWTPs[i], sock, gACSecurityContext, &((gWTPs[i]).session), &(gWTPs[i].pathMTU))))  // error joining
	{
		CWTimerCancel(&(gWTPs[i].currentTimer),1);
		_CWCloseThread(i);
	}
#endif
	(gWTPs[i]).subState = CW_WAITING_REQUEST;
	
	if (gCWForceMTU > 0)
	{
	    gWTPs[i].pathMTU = gCWForceMTU;
	}

	return CW_TRUE;
}


void STA_OP(TableMsg *msg)
{
	int WTPIndex = msg->u.STA.WTPID;
	unsigned int BSSIndex = msg->u.STA.BSSIndex;
	int auth_flag = 0;
	unsigned char WLANID = 0;
	msgq msginfo;
	
	if((AC_WTP[WTPIndex] != NULL)&&(AC_WTP[WTPIndex]->WTPStat == WID_RUN))
	{
		/*while((AC_WTP[WTPIndex]->CMD->staCMD > 0)&&(i < 10)){
			printf("please waiting seconds\n");			
			i++;
			sleep(1);
		}
		if(i == 10)
			return;*/
		if(msg->Op == CANCEL_TRAFFIC_LIMIT)
		{
			unsigned char L_Radio_ID = 0;
			unsigned char mac[MAC_LEN] = {0};
			WLANID = msg->u.STA.wlanId;
			L_Radio_ID = BSSIndex % L_RADIO_NUM;
			memset(mac, 0, MAC_LEN);
			memcpy(mac, msg->u.STA.STAMAC, MAC_LEN);	
			wid_cancel_bss_traffic_limit_sta_value(WTPIndex,L_Radio_ID,WLANID,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],0,2,0);
			return ;
		}
		else if (STA_TRAFFIC_LIMIT == msg->Op)
		{

			wid_set_sta_traffic_limit(msg->u.STA.WTPID, msg->u.STA.radioId, msg->u.STA.wlanId,
				msg->u.STA.STAMAC, msg->u.STA.traffic_limit, msg->u.STA.send_traffic_limit);
			return ;
		}		
		else if(STA_PORTAL_AUTH == msg->Op)
		{
			char cmd[WID_SYSTEM_CMD_LENTH] = {0};
			unsigned char eag_ip[4] = {0};
			
			memcpy(eag_ip, &(msg->u.STA.portal_info.portal_ip), sizeof(eag_ip));
			auth_flag = 1;
			sprintf(cmd, AP_EXT_CMD_NOTIFY_STA_PORTAL_AUTH,	((msg->u.STA.BSSIndex)/L_BSS_NUM)%L_RADIO_NUM,
							msg->u.STA.wlanId, msg->u.STA.STAMAC[0], msg->u.STA.STAMAC[1], msg->u.STA.STAMAC[2],
							msg->u.STA.STAMAC[3], msg->u.STA.STAMAC[4], msg->u.STA.STAMAC[5], auth_flag, eag_ip[0], eag_ip[1], eag_ip[2],eag_ip[3]);
			wid_syslog_debug_debug(WID_DEFAULT, "wtp %d extension command: %s\n", msg->u.STA.WTPID, cmd);
			wid_radio_set_extension_command(msg->u.STA.WTPID, cmd);
			return ;
		}
		else if(msg->Op == STA_PORTAL_DEAUTH)
		{
			char cmd[WID_SYSTEM_CMD_LENTH] = {0};
			char eag_ip[4] = {0};
			auth_flag = 0;
			
			sprintf(cmd, AP_EXT_CMD_NOTIFY_STA_PORTAL_AUTH,	((msg->u.STA.BSSIndex)/L_BSS_NUM)%L_RADIO_NUM,
							msg->u.STA.wlanId, msg->u.STA.STAMAC[0], msg->u.STA.STAMAC[1], msg->u.STA.STAMAC[2],
							msg->u.STA.STAMAC[3], msg->u.STA.STAMAC[4], msg->u.STA.STAMAC[5], auth_flag, eag_ip[0], eag_ip[1], eag_ip[2],eag_ip[3]);
			wid_syslog_debug_debug(WID_DEFAULT, "wtp %d extension command: %s\n", msg->u.STA.WTPID, cmd);
			wid_radio_set_extension_command(msg->u.STA.WTPID, cmd);
			return ;
		}
		if ((BSSIndex >= BSS_NUM) || (NULL == AC_BSS[BSSIndex]))
		{
			wid_syslog_err("%s:%d no bssindex %d\n", __func__, __LINE__, BSSIndex);
			return;
		}
		memset((char*)&msginfo, 0, sizeof(msginfo));
		msginfo.mqid = WTPIndex%THREAD_NUM + 1;		
		msginfo.mqinfo.WTPID = WTPIndex;
		msginfo.mqinfo.type = CONTROL_TYPE;
		msginfo.mqinfo.subtype = STA_S_TYPE;
		if(msg->Op == WID_ADD)
		{
			msginfo.mqinfo.u.StaInfo.Sta_Op = Sta_ADD;
			if(msg->u.STA.traffic_limit != 0)
			{
				msginfo.mqinfo.u.StaInfo.traffic_limit = msg->u.STA.traffic_limit;
			}
			if(msg->u.STA.send_traffic_limit != 0)
			{
				msginfo.mqinfo.u.StaInfo.send_traffic_limit = msg->u.STA.send_traffic_limit;
			}
			if(msg->u.STA.vlan_id > 0)
			{
				msginfo.mqinfo.u.StaInfo.vlan_id = msg->u.STA.vlan_id;
			}
			msginfo.mqinfo.u.StaInfo.sta_num = msg->u.STA.sta_num;
		}
		else if(msg->Op == WID_DEL)
		{
			msginfo.mqinfo.u.StaInfo.Sta_Op = Sta_DEL;
		}
		else if(msg->Op == RADIUS_STA_UPDATE)
		{		
			unsigned char mac[MAC_LEN] ;
			memcpy(mac, msg->u.STA.STAMAC, MAC_LEN);	
			wid_radio_set_wlan_traffic_limit_sta_value(WTPIndex,(msg->u.STA.BSSIndex/L_BSS_NUM)%L_RADIO_NUM,AC_BSS[BSSIndex]->WlanID,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], msg->u.STA.send_traffic_limit,1);
			wid_radio_set_wlan_traffic_limit_sta_value(WTPIndex,(msg->u.STA.BSSIndex/L_BSS_NUM)%L_RADIO_NUM,AC_BSS[BSSIndex]->WlanID,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], msg->u.STA.traffic_limit,0);
		}
		msginfo.mqinfo.u.StaInfo.Radio_L_ID = (msg->u.STA.BSSIndex/L_BSS_NUM)%L_RADIO_NUM;
		memcpy(msginfo.mqinfo.u.StaInfo.STAMAC, msg->u.STA.STAMAC, MAC_LEN);	
		msginfo.mqinfo.u.StaInfo.WLANID = AC_BSS[BSSIndex]->WlanID;
		if (msgsnd(ASD_WIDMSGQ, (msgq *)&msginfo, sizeof(msginfo.mqinfo), 0) == -1)
		{
			wid_syslog_info("%s msgsend %s",__func__,strerror(errno));
			perror("msgsnd");
		}

		WLANID = AC_BSS[BSSIndex]->WlanID;
		
		//AC_WTP[WTPIndex]->CMD->CMD++;
				
		printf("STA op1\n"); 		
		if((AC_WLAN[WLANID] != NULL)&&(AC_WLAN[WLANID]->Roaming_Policy == 1))
		{
			CWThreadMutexLock(&(gSTARoamingMutex));
			memset(STA_ROAM.STAMAC, 0, MAC_LEN);
			STA_ROAM.STAOP = msg->Op;
			memcpy(STA_ROAM.STAMAC, msg->u.STA.STAMAC, MAC_LEN);
			STA_ROAM.WLANDomain = AC_BSS[BSSIndex]->WlanID;
			CWSignalThreadCondition(&gSTARoamingWait);
			CWThreadMutexUnlock(&(gSTARoamingMutex));
		}
	}
}

void KEY_OP(TableMsg *msg)
{
	int WTPIndex = msg->u.KEY.WTPID;	
//	int i = 0;
	msgq msginfo;
	if(AC_WTP[WTPIndex] != NULL)
	{		
		/*while((AC_WTP[WTPIndex]->CMD->keyCMD > 0)&&(i < 10)){
			printf("please waiting seconds\n");
			i++;
			sleep(1);
		}
		if(i == 10)
			return;*/
			
		memset((char*)&msginfo, 0, sizeof(msginfo));
		msginfo.mqid = WTPIndex%THREAD_NUM + 1;	
		msginfo.mqinfo.WTPID = WTPIndex;
		msginfo.mqinfo.type = CONTROL_TYPE;
		msginfo.mqinfo.subtype = STA_S_TYPE;
		msginfo.mqinfo.u.StaInfo.Sta_Op = Sta_SETKEY;
		msginfo.mqinfo.u.StaInfo.BSSIndex = msg->u.KEY.BSSIndex;
		msginfo.mqinfo.u.StaInfo.cipher = msg->u.KEY.cipher;
		msginfo.mqinfo.u.StaInfo.keylen = msg->u.KEY.key_len;
		msginfo.mqinfo.u.StaInfo.keyidx = msg->u.KEY.key_idx;
		printf("key len %d\n", msg->u.KEY.key_len);
		memcpy((msginfo.mqinfo.u.StaInfo.STAMAC), (msg->u.KEY.StaAddr), 6);
		memcpy((msginfo.mqinfo.u.StaInfo.key), (msg->u.KEY.key), msg->u.KEY.key_len);
		
		if (msgsnd(ASD_WIDMSGQ, (msgq *)&msginfo, sizeof(msginfo.mqinfo), 0) == -1)
		{
			wid_syslog_info("%s msgsend %s",__func__,strerror(errno));
			perror("msgsnd");
		}
	}
}

void PMK_OP(TableMsg *msg)
{
	int WTPIndex = msg->u.PMK.wtpid;	
	msgq msginfo;
	
	if(AC_WTP[WTPIndex] != NULL)
	{				
		memset((char*)&msginfo, 0, sizeof(msginfo));
		msginfo.mqid = WTPIndex%THREAD_NUM + 1;	
		msginfo.mqinfo.WTPID = WTPIndex;
		msginfo.mqinfo.type = CONTROL_TYPE;
		msginfo.mqinfo.subtype = STA_S_TYPE;
		msginfo.mqinfo.u.StaInfo.Sta_Op = Sta_SETPMK;
		msginfo.mqinfo.u.StaInfo.WLANID = msg->u.PMK.wlanid;
		msginfo.mqinfo.u.StaInfo.Radio_G_ID = msg->u.PMK.g_radio;
		msginfo.mqinfo.u.StaInfo.Radio_L_ID = msg->u.PMK.l_radio;
		msginfo.mqinfo.u.StaInfo.BSSIndex = msg->u.PMK.bssindex;
		msginfo.mqinfo.u.StaInfo.roamflag = msg->u.PMK.roamflag;
		memcpy((msginfo.mqinfo.u.StaInfo.BSSID),(msg->u.PMK.bssid), MAC_LEN);
		memcpy((msginfo.mqinfo.u.StaInfo.STAMAC),(msg->u.PMK.StaAddr), MAC_LEN);
		memcpy((msginfo.mqinfo.u.StaInfo.PMK),(msg->u.PMK.pmk), PMK_LEN);
		msginfo.mqinfo.u.StaInfo.pmkid_len = msg->u.PMK.pmkid_len;
		memcpy((msginfo.mqinfo.u.StaInfo.pmkid),(msg->u.PMK.pmkid), PMKID_LEN);
		
		if (msgsnd(ASD_WIDMSGQ, (msgq *)&msginfo, sizeof(msginfo.mqinfo), 0) == -1)
		{
			perror("msgsnd");
		}
	}
}

void WLAN_OP(TableMsg *msg)
{
	int WLANID = msg->u.WLAN.WlanID;
	unsigned char oldsecid = msg->u.WLAN.WlanKey.OldSecurityIndex;
	unsigned char oldsecid_flag =  msg->u.WLAN.WlanKey.OldSecurityIndex_flag;
	int i = 0;
	int j = 0;
	char *key = NULL;
	
	switch(msg->Op)
	{
		case WID_MODIFY:
			if(AC_WLAN[WLANID] != NULL)
			{
				if(AC_WLAN[WLANID]->EncryptionType != msg->u.WLAN.EncryptionType)

								
				if(gtrapflag >= 4)
				{
			#if NOSOFTAC
					wid_dbus_trap_wlan_encryption_type_change(WLANID);
			#endif
				}

				/*fengwenchao add 20110309 通过oldsecid,oldsecid_flag判断之前绑定安全策略是否为((open||shared)&&(wep)),1-->YES清除标志位*/
				for(i = 0; i < WTP_NUM; i++)
				{
					if((AC_WTP[i] != NULL)/*&&(AC_WTP[i]->isused == 1)*/)
					{
						for(j = 0; j < AC_WTP[i]->RadioCount; j++)
						{
							if(AC_WLAN[WLANID]->S_WTP_BSS_List[i][j] != 0)
							{
								int bssindex = AC_WLAN[WLANID]->S_WTP_BSS_List[i][j];
								//int wtpid = bssindex/(L_RADIO_NUM*L_BSS_NUM);
				
								if((oldsecid <= 4) && (oldsecid >= 1) && (oldsecid_flag == 1))
								{
									AC_RADIO[AC_BSS[bssindex]->Radio_G_ID]->wep_flag[oldsecid-1] = 0;
									//AC_WTP[wtpid]->wep_flag[oldsecid-1] = 0;
								}								
							}						
						}							
					}			
				}
				
				AC_WLAN[WLANID]->ap_max_inactivity = msg->u.WLAN.ap_max_inactivity;											
				AC_WLAN[WLANID]->SecurityType = msg->u.WLAN.SecurityType;
				AC_WLAN[WLANID]->SecurityID = msg->u.WLAN.SecurityID;
				AC_WLAN[WLANID]->EncryptionType = msg->u.WLAN.EncryptionType;
				AC_WLAN[WLANID]->PreAuth = msg->u.WLAN.PreAuth;
				AC_WLAN[WLANID]->KeyLen = 0;
				AC_WLAN[WLANID]->IpLen = 0;
				AC_WLAN[WLANID]->AECerLen = 0;
				AC_WLAN[WLANID]->ASCerLen = 0;
				AC_WLAN[WLANID]->asic_hex = 0;
				memset(AC_WLAN[WLANID]->WlanKey, 0, DEFAULT_LEN);
				memset(AC_WLAN[WLANID]->AsIp, 0, DEFAULT_LEN);
				memset(AC_WLAN[WLANID]->AECerPath, 0, DEFAULT_LEN);
				memset(AC_WLAN[WLANID]->ASCerPath, 0, DEFAULT_LEN);
				
				if(((AC_WLAN[WLANID]->SecurityType == OPEN) && (AC_WLAN[WLANID]->EncryptionType == WEP))
					||(AC_WLAN[WLANID]->SecurityType == SHARED)
					||(AC_WLAN[WLANID]->SecurityType == WAPI_PSK))
				{
					AC_WLAN[WLANID]->KeyLen = msg->u.WLAN.WlanKey.key_len;
					memcpy(AC_WLAN[WLANID]->WlanKey, msg->u.WLAN.WlanKey.key, msg->u.WLAN.WlanKey.key_len);
					key = (char *)malloc(msg->u.WLAN.WlanKey.key_len+1);
					memset(key,0,(msg->u.WLAN.WlanKey.key_len+1));
					strncpy(key, msg->u.WLAN.WlanKey.key, msg->u.WLAN.WlanKey.key_len);
					AC_WLAN[WLANID]->SecurityIndex = msg->u.WLAN.WlanKey.SecurityIndex;

					if(msg->u.WLAN.ascii_hex == 1)
					{
						AC_WLAN[WLANID]->asic_hex = 0;
					}
					else if(msg->u.WLAN.ascii_hex == 2)
					{
						AC_WLAN[WLANID]->asic_hex = 1;
					}
					if(gtrapflag >= 4)
					{
			#if NOSOFTAC
						wid_dbus_trap_wlan_preshared_key_change(WLANID,key);
			#endif
					}
					
					CW_FREE_OBJECT(key);
					wid_syslog_debug_debug(WID_DEFAULT, "keylen %d %d\n", msg->u.WLAN.WlanKey.key_len, AC_WLAN[WLANID]->KeyLen);
					wid_syslog_debug_debug(WID_DEFAULT, "key %s %s\n", msg->u.WLAN.WlanKey.key, AC_WLAN[WLANID]->WlanKey);
					wid_syslog_debug_debug(WID_DEFAULT, "keytype %d %d\n", msg->u.WLAN.ascii_hex, AC_WLAN[WLANID]->asic_hex);
				}
				
				if((AC_WLAN[WLANID]->SecurityType == WPA_P)
					||(AC_WLAN[WLANID]->SecurityType == WPA2_P)
					||(AC_WLAN[WLANID]->SecurityType == IEEE8021X))
				{
					AC_WLAN[WLANID]->KeyLen = msg->u.WLAN.WlanKey.key_len;
					memcpy(AC_WLAN[WLANID]->WlanKey, msg->u.WLAN.WlanKey.key, msg->u.WLAN.WlanKey.key_len);
					
					key = (char *)malloc(msg->u.WLAN.WlanKey.key_len + 1);
					memset(key, 0, (msg->u.WLAN.WlanKey.key_len + 1));
					strncpy(key, msg->u.WLAN.WlanKey.key, msg->u.WLAN.WlanKey.key_len);
					
					AC_WLAN[WLANID]->SecurityIndex = msg->u.WLAN.WlanKey.SecurityIndex;
					AC_WLAN[WLANID]->wpa_group_rekey = msg->u.WLAN.WlanKey.wpa_group_rekey;

					if(msg->u.WLAN.ascii_hex == 1)
					{
						AC_WLAN[WLANID]->asic_hex = 0;
					}
					else if(msg->u.WLAN.ascii_hex == 2)
					{
						AC_WLAN[WLANID]->asic_hex = 1;
					}
					if(gtrapflag >= 4)
					{
			#if NOSOFTAC
						wid_dbus_trap_wlan_preshared_key_change(WLANID, key);
			#endif
					}						
						
					CW_FREE_OBJECT(key);
					wid_syslog_debug_debug(WID_DEFAULT, "keylen %d %d, key %s %s, keytype %d %d\n",
														msg->u.WLAN.WlanKey.key_len, AC_WLAN[WLANID]->KeyLen,
														msg->u.WLAN.WlanKey.key, AC_WLAN[WLANID]->WlanKey,
														msg->u.WLAN.ascii_hex, AC_WLAN[WLANID]->asic_hex);
				}
				if((AC_WLAN[WLANID]->SecurityType == WPA_E) || (AC_WLAN[WLANID]->SecurityType == WPA2_E))
				{
					AC_WLAN[WLANID]->asic_hex = 0;
					AC_WLAN[WLANID]->KeyLen = 0;
					memset(AC_WLAN[WLANID]->WlanKey, 0, DEFAULT_LEN);
					
					key = (char *)malloc(msg->u.WLAN.WlanKey.key_len + 1);
					memset(key, 0, (msg->u.WLAN.WlanKey.key_len + 1));
					strncpy(key, msg->u.WLAN.WlanKey.key, msg->u.WLAN.WlanKey.key_len);
					
					AC_WLAN[WLANID]->SecurityIndex = msg->u.WLAN.WlanKey.SecurityIndex;
					AC_WLAN[WLANID]->wpa_group_rekey = msg->u.WLAN.WlanKey.wpa_group_rekey;
					
					if(gtrapflag >= 4)
					{
			#if NOSOFTAC
						wid_dbus_trap_wlan_preshared_key_change(WLANID, key);
			#endif
					}						
						
					CW_FREE_OBJECT(key);
					wid_syslog_debug_debug(WID_DEFAULT, "keylen %d %d, key %s %s, keytype %d %d\n",
														msg->u.WLAN.WlanKey.key_len, AC_WLAN[WLANID]->KeyLen,
														msg->u.WLAN.WlanKey.key, AC_WLAN[WLANID]->WlanKey,
														msg->u.WLAN.ascii_hex, AC_WLAN[WLANID]->asic_hex);
				}
				
				if((AC_WLAN[WLANID]->SecurityType == WAPI_AUTH))					
				{
					AC_WLAN[WLANID]->AECerLen = msg->u.WLAN.ae_cert_path_len;
					memcpy(AC_WLAN[WLANID]->AECerPath, msg->u.WLAN.ae_cert_path, msg->u.WLAN.ae_cert_path_len);

					AC_WLAN[WLANID]->IpLen = msg->u.WLAN.as_ip_len;
					memcpy(AC_WLAN[WLANID]->AsIp, msg->u.WLAN.as_ip, msg->u.WLAN.as_ip_len);

					AC_WLAN[WLANID]->ASCerLen = msg->u.WLAN.cert_path_len;
					memcpy(AC_WLAN[WLANID]->ASCerPath, msg->u.WLAN.cert_path, msg->u.WLAN.cert_path_len);

					wid_syslog_debug_debug(WID_DEFAULT, "AECerLen %d %d\n", msg->u.WLAN.ae_cert_path_len, AC_WLAN[WLANID]->AECerLen);					
					wid_syslog_debug_debug(WID_DEFAULT, "IpLen %d %d\n", msg->u.WLAN.as_ip_len, AC_WLAN[WLANID]->IpLen);
					wid_syslog_debug_debug(WID_DEFAULT, "ASCerLen %d %d\n", msg->u.WLAN.cert_path_len, AC_WLAN[WLANID]->ASCerLen);
					wid_syslog_debug_debug(WID_DEFAULT, "AECerPath %s %s\n", msg->u.WLAN.ae_cert_path, AC_WLAN[WLANID]->AECerPath);
					wid_syslog_debug_debug(WID_DEFAULT, "ASCerPath %s %s\n", msg->u.WLAN.cert_path, AC_WLAN[WLANID]->ASCerPath);
					wid_syslog_debug_debug(WID_DEFAULT, "AsIp %s %s\n", msg->u.WLAN.as_ip, AC_WLAN[WLANID]->AsIp);
				}
			}
			break;
		case WID_DEL:			
			for(i = 1; i < WLAN_NUM; i++)
			{
				if((AC_WLAN[i]!=NULL)&&(AC_WLAN[i]->Status == 1) && (AC_WLAN[i]->SecurityID == msg->u.WLAN.SecurityID))
				{
					AC_WLAN[i]->SecurityID = 0;
					AC_WLAN[i]->SecurityType = 0;
					AC_WLAN[i]->KeyLen = 0;
					memset(AC_WLAN[i]->WlanKey, 0, DEFAULT_LEN);
					AC_WLAN[i]->IpLen = 0;
					AC_WLAN[i]->AECerLen = 0;
					AC_WLAN[i]->ASCerLen = 0;
					AC_WLAN[i]->asic_hex = 0;
					memset(AC_WLAN[i]->AsIp, 0, DEFAULT_LEN);
					memset(AC_WLAN[i]->AECerPath, 0, DEFAULT_LEN);
					memset(AC_WLAN[i]->ASCerPath, 0, DEFAULT_LEN);
					continue;
				}else
					continue;
			}
			break;
		case WID_ADD:	
			break;
		case STA_INFO:	
			break;
		default:
			break;		
	}
}

/*
void trap_op(TableMsg *msg)
{	
	if(gtrapflag == 1)
	{
		switch(msg->Op)
		{
			case WTP_DENEY_STA :	
				signal_wtp_deny_sta(msg->u.WTP.WtpID);
				break;
			case WTP_DE_DENEY_STA :	
				signal_de_wtp_deny_sta(msg->u.WTP.WtpID);
				break;
			case STA_COME :	
				signal_sta_come(msg->u.STA.STAMAC, msg->u.STA.WTPID, msg->u.STA.BSSIndex, msg->u.STA.StaState);
				break;
			case STA_LEAVE :	
				signal_sta_leave(msg->u.STA.STAMAC, msg->u.STA.WTPID, msg->u.STA.BSSIndex, msg->u.STA.StaState);
				break;
			case VERIFY_INFO :
				signal_sta_verify(msg->u.STA.STAMAC,msg->u.STA.WTPID);
				break;
			case VERIFY_FAIL_INFO :
				signal_sta_verify_failed(msg->u.STA.STAMAC,msg->u.STA.WTPID);
				break;
			case ASSOC_FAIL_INFO :
				signal_sta_assoc_failed(msg->u.STA.STAMAC,msg->u.STA.WTPID,msg->u.STA.nRate);
				break;
			case JIANQUAN_FAIL_INFO :
				signal_sta_jianquan_failed(msg->u.STA.STAMAC,msg->u.STA.WTPID,msg->u.STA.nRate);
				break;
			default:
				break;		
		}
	}
}
*/
void BSS_pkt_op(TableMsg *msg)
{
	int i=0;

	unsigned char *buff = (unsigned char*)msg + sizeof(MsgType);
	BSS_pkt_header *bss_header = (BSS_pkt_header *)buff;
	BSSStatistics *BSS_pkt = (BSSStatistics *)(buff + sizeof(BSS_pkt_header));
	unsigned int bssindex = 0;
	unsigned int l_radio_id = 0;
	unsigned int l_bss_id = 0;
	unsigned int wtpid = bss_header->WTPID;
	unsigned int count = bss_header->bss_cnt;
	wid_syslog_debug_debug(WID_DEFAULT,"wtpid:%d count: %d\n",wtpid,count);
	if(AC_WTP[wtpid] != NULL)
	{
		if(count != 0)
		{
			for(i=0;i<count;i++)
			{
				bssindex = BSS_pkt->BSSIndex;
				l_radio_id = (bssindex-(wtpid*L_RADIO_NUM*L_BSS_NUM))/L_BSS_NUM;
				l_bss_id = bssindex%L_BSS_NUM;
				//printf("BSS_pkt->BSSIndex %d\n",BSS_pkt->BSSIndex);
				//printf("bssindex %d l_radio_id %d l_bss_id %d\n",bssindex,l_radio_id,l_bss_id);
				if(AC_WTP[wtpid]->WTP_Radio[l_radio_id] != NULL)
				{
					if(AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id] != NULL)
					{
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.BSSIndex = BSS_pkt->BSSIndex;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.Radio_G_ID = BSS_pkt->Radio_G_ID;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_unicast = BSS_pkt->rx_unicast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_unicast = BSS_pkt->tx_unicast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_broadcast = BSS_pkt->rx_broadcast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_broadcast = BSS_pkt->tx_broadcast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_pkt_unicast = BSS_pkt->rx_pkt_unicast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_pkt_unicast = BSS_pkt->tx_pkt_unicast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_pkt_broadcast = BSS_pkt->rx_pkt_broadcast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_pkt_broadcast = BSS_pkt->tx_pkt_broadcast;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.retry = BSS_pkt->retry;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.retry_pkt = BSS_pkt->retry_pkt;
						AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.err = BSS_pkt->err;
					/*
						printf("BSSIndex: %d\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.BSSIndex);
						printf("g_radio_id: %d\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.Radio_G_ID);
						printf("rx_unicast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_unicast);
						printf("tx_unicast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_unicast);
						printf("rx_broadcast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_broadcast);
						printf("tx_broadcast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_broadcast);
						printf("rx_pkt_unicast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_pkt_unicast);
						printf("tx_pkt_unicast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_pkt_unicast);
						printf("rx_pkt_broadcast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.rx_pkt_broadcast);
						printf("tx_pkt_broadcast: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.tx_pkt_broadcast);
						printf("retry: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.retry);
						printf("retry_pkt: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.retry_pkt);
						printf("err: %lld\n",AC_WTP[wtpid]->WTP_Radio[l_radio_id]->BSS[l_bss_id]->BSS_pkt_info.err);
					*/
					}
					else
					{
						wid_syslog_debug_debug(WID_DEFAULT,"bssindex %d is not exist\n",bssindex);
					}
				}
				else
				{
					wid_syslog_debug_debug(WID_DEFAULT,"wtpid %d radioid %d is not exist\n",wtpid,l_radio_id);
				}
				BSS_pkt += 1;
			}	
		}
		else
		{
			wid_syslog_debug_debug(WID_DEFAULT,"wtp %d has not bss at all\n",wtpid);
		}
	}
	else
	{
		wid_syslog_debug_debug(WID_DEFAULT,"WID WSM WTP table not synchronous ,wtp %d is not exist\n",wtpid);
	}
}

void neighbor_ap_sta_info(TableMsg *msg)
{
	int WTPIndex = msg->u.WTP.WtpID;
	int N_wtpid = msg->u.WTP.N_WTP.N_WtpID;
	int N_sta_num  = msg->u.WTP.N_WTP.cur_sta_num;
	unsigned char mac[6] = {0};
	memcpy(mac,msg->u.WTP.N_WTP.WTPMAC,MAC_LEN);
	int IGNORE_UBSV currentchannel = 0;
	if(!check_wtpid_func(WTPIndex)){
		return;
	}else{
		if(gtrapflag>=24){
			if(AC_WTP[WTPIndex])
			{
				currentchannel = AC_WTP[WTPIndex]->WTP_Radio[0]->Radio_Chan;
				wid_syslog_debug_debug(WID_DEFAULT,"rogue_terminal,%s\n",__func__);
				wid_syslog_debug_debug(WID_DEFAULT,"rogue_terminal,current wtpid:%d,neighbor ap id:%d,neighbor ap sta num:%d\n",WTPIndex,N_wtpid,N_sta_num);
				wid_syslog_debug_debug(WID_DEFAULT,"rogue_terminal_threshold=%d\n",AC_WTP[WTPIndex]->wtp_rogue_terminal_threshold);
			}
			if((gtrap_channel_terminal_interference_switch == 1)&&(AC_WTP[WTPIndex])&&(AC_WTP[WTPIndex]->wtp_rogue_terminal_threshold <= N_sta_num))
			{
				if(AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag == 0)   //fengwenchao add 20110221
				{
					wid_syslog_debug_debug(WID_DEFAULT,"rogue_terminal,send terminal trap,%s\n",__func__);

				#if NOSOFTAC
					wid_dbus_trap_wtp_channel_terminal_interference(WTPIndex,0,currentchannel,mac);
				#endif
				
					AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag = 1;
				}
			}
			else if((AC_WTP[WTPIndex])&&(AC_WTP[WTPIndex]->wtp_rogue_terminal_threshold > N_sta_num)&&(AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag == 1))
			{
				wid_syslog_debug_debug(WID_DEFAULT,"rogue_terminal,send terminal trap clear,%s\n",__func__);

			#if NOSOFTAC
				wid_dbus_trap_wtp_channel_terminal_interference_clear(WTPIndex,0,currentchannel,NULL);
			#endif
				
				AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag = 0;      //fengwenchao add 20110221
			}	
		}
	}
}
CW_THREAD_RETURN_TYPE CWWawInforUpdate(void * arg)
{
	wid_pid_write_v2("CWWawInforUpdate", 0, vrrid);

	int len = 0;
	unsigned char buf[8192] = {0};

	wid_thread_log("info");

	while(1)
	{
		len = 0;
		memset(buf, 0, 8192);
		
		CWGetMsgQueue(&ASD_WIDMSGQ);
		len = recvfrom(wAWSocket, buf, sizeof(buf), 0, NULL, 0);
		
		if (len < 0)
		{
			wid_syslog_info("%s recv %s",__func__,strerror(errno));
			perror("recv");
			return 0;
		}		
		TableMsg *msg = (TableMsg*)buf;
		wid_syslog_debug_debug(WID_DEFAULT,"Type:%d\n",msg->Type);
		switch(msg->Type)
		{
			case WLAN_TYPE :
			{
					
				WLAN_OP(msg);
				break;
			}
			/*case BSS_TYPE :
			{
					
				BSS_OP(msg, interfaces);
				break;
			}*/
			case STA_TYPE :

				if(is_secondary != 1)
				{
					STA_OP(msg);
				}
				break;
			case SKEY_TYPE :

				if(is_secondary != 1)
				{
					KEY_OP(msg);
				}
				break;
			case PMK_TYPE :
				if (WAW_VRRP_STATE_SECONDARY != is_secondary) 
				{
					PMK_OP(msg);
				}
				break;
			case TRAP_TYPE :
				
				/*trap_op(msg);*/
				break;
			case BSS_PKT_TYPE:
				
				//BSS_pkt_op(msg);
				break;
			case NEIGHBOR_AP_STA_CHECK :
				if(is_secondary != 1)
				{
					neighbor_ap_sta_info(msg);
				}
				break;
			default:
				break;
		}
	}
}

int WTPAddrCheck(CWNetworkLev4Address *addrPtr)
{
	int i = 0;
	CWThreadMutexLock(&gWTPsMutex);
	for(i = 0; i < WTP_NUM; i++)
	{
		if(gWTPs[i].isNotFree
		    && &(gWTPs[i].address) != NULL
		    && !sock_cmp_addr((struct sockaddr*)addrPtr, (struct sockaddr*)&(gWTPs[i].address), sizeof(CWNetworkLev4Address)))  // we treat a WTP that sends packet to a different AC's interface as a new WTP
		{    
			CWThreadMutexUnlock(&gWTPsMutex);
			return i;
		}
	}
	CWThreadMutexUnlock(&gWTPsMutex);
	return -1;
}

int WTPMacCheck(unsigned char *apmac)
{
	int i = 0;
	
	for(i = 1; i < WTP_NUM; i++)
	{
		if(AC_WTP[i] != NULL
		    && AC_WTP[i]->WTPMAC != NULL
		    && !memcmp(AC_WTP[i]->WTPMAC, apmac, MAC_LEN))
		{
			return i;
		}
	}

	return -1;
}

void Delete_Interface(char *ifname, int ifindex){
	struct CWMultiHomedInterface *p,*p1;
	p1 = gACSocket.interfaces;
	if(gACSocket.interfaces == NULL)
		return;
	p = p1->if_next;
	while(p != NULL){
		if((strncmp(p->ifname,ifname,IFI_NAME)==0)&&(p->systemIndex == ifindex)){
			int gifindex;
			p1->if_next = p->if_next;
			p->if_next = NULL;
			if(p->kind == CW_PRIMARY){
				gifindex = p->gIf_Index;
				if((gifindex < gMaxInterfacesCount)&&(gifindex >= 0)){   //fengwenchao add (gifindex >= 0) 20110525
					gInterfaces[gifindex].enable = 0;
					gInterfaces[gifindex].WTPCount = 0;
				}
			}
			close(p->sock);
			free(p);
			p = NULL;
			p = p1->if_next;
			continue;
		}
		p1 = p;
		p = p1->if_next;
	}
}

void Modify_WLAN_WTP_SETTING(int index)
{	
	int i = 0;
	char ifname[IFI_NAME];
	memset(ifname, 0, IFI_NAME);
	if(AC_WTP[index])
	{
		memcpy(ifname, AC_WTP[index]->BindingIFName, IFI_NAME);
	}
	int ret = 0;
	struct ifi_info *ifi = (struct ifi_info*)calloc(1, sizeof(struct ifi_info));
	strncpy(ifi->ifi_name,ifname,sizeof(ifi->ifi_name));
	//struct CWMultiHomedInterface *p = NULL,*pbr = NULL; 
	ret = Get_Interface_Info(ifname,ifi);
	int ifindex = 0;
	if(AC_WTP[index])
	{
		ifindex = AC_WTP[index]->BindingSystemIndex;
	}
	if(ret != 0)
	{
		Delete_Interface(ifname, ifindex);
		CW_FREE_OBJECT(ifi->ifi_addr);
		CW_FREE_OBJECT(ifi->ifi_brdaddr);
		CW_FREE_OBJECT(ifi);
		return; 
	}
	else
	{
		Check_Current_Interface_and_delete(ifname,ifi);		
		if(ifi->check_brdaddr < 2)
		{
			ret = Bind_BroadAddr_For_WID(ifi,CW_CONTROL_PORT);		
	//		ret = Bind_BroadAddr_For_WID(ifi,CW_CONTROL_PORT_AU);
		}
		for(i = 0; i < ifi->addr_num; i++)
		{		
			if(ifi->addr[i] == 0)
				continue;
			//memcpy(&((struct sockaddr_in *) ifi->ifi_addr)->sin_addr,&(ifi->addr[i]),sizeof(struct in_addr));
			((struct sockaddr_in *) ifi->ifi_addr)->sin_addr.s_addr = ifi->addr[i];
			if(Lic_ip.lic_active_ac_ip == ifi->addr[i])
			{
				ret = Bind_Interface_For_WID(ifi,WID_LIC_AC_PORT,LIC_TYPE);	 
				wid_syslog_debug_debug(WID_DBUS,"%s,%d,WID_LIC_AC_PORT.\n",__func__,__LINE__);
			}
			else
			{
				ret = Bind_Interface_For_WID(ifi,CW_CONTROL_PORT,DOWN_LINK_IF_TYPE);
				if(ret == 1)
				{
					ret = 0;
					continue;
				}				
				//ret = Bind_Interface_For_WID(ifi,CW_CONTROL_PORT_AU);
				unsigned int ip = 0;
				ip = ((struct sockaddr_in *)(ifi->ifi_addr))->sin_addr.s_addr;
				WIDWsm_VRRPIFOp((unsigned char*)(ifi->ifi_name),ip,VRRP_REG_IF);
				Check_WLAN_WTP_IF_Index(ifi,ifname);	
			}
		}
	}	
	CW_FREE_OBJECT(ifi->ifi_addr);
	CW_FREE_OBJECT(ifi->ifi_brdaddr);
	CW_FREE_OBJECT(ifi);
	return;
}

int DELETE_LISTENNING_INTERFACE(char *ifname){	
	//int i = 0;
	//int ret = 0;
	int m=0,n=0;
	int IGNORE_UBSV ifindex = 0;
	int find = 0;
	struct CWMultiHomedInterface *p = NULL,*p_next = NULL; 
	p = gListenningIF.interfaces;
	if(p == NULL){  
		wid_syslog_debug_debug(WID_DEFAULT,"%s,line=%d,p=%p",__func__,__LINE__,p);
		return -1;
	}
	p_next = p->if_next;
	m = strlen(ifname);
	n= strlen(p->ifname);
	wid_syslog_debug_debug(WID_DEFAULT,"%s,name %s flag %d\n",__func__,p->ifname, p->lic_flag);
	if((m==n)&&(p->lic_flag == DOWN_LINK_IF_TYPE)&&(strcmp(ifname,p->ifname) == 0)){
		ifindex = p->systemIndex;
		gListenningIF.interfaces = p_next;
		p->if_next = NULL;
		free(p);
		p = NULL;
		find = 1;
		if(gListenningIF.count > 0)
			gListenningIF.count--;
		wid_syslog_debug_debug(WID_DEFAULT,"%s,11gListenningIF.count--;count=%d\n",__func__,gListenningIF.count);
	}else{
		while(p_next != NULL){
			n= strlen(p_next->ifname);
			if((m==n)&&(p_next->lic_flag == DOWN_LINK_IF_TYPE)&&(strcmp(ifname,p_next->ifname)==0)){
				//p_next = p_next->if_next;
				p->if_next = p_next->if_next;
				p_next->if_next = NULL;
				ifindex = p_next->systemIndex;
				free(p_next);
				p_next = NULL;
				find = 1;
				if(gListenningIF.count > 0)
					gListenningIF.count--;
				wid_syslog_debug_debug(WID_DEFAULT,"%s,22gListenningIF.count--;count=%d\n",__func__,gListenningIF.count);
				break;
			}
			p = p_next;
			p_next= p_next->if_next;
		}
	}
	if(find == 0){
		wid_syslog_debug_debug(WID_DEFAULT,"%s,not find if.line=%d,111111.\n",__func__,__LINE__);
		return INTERFACE_NOT_EXIST;
	}
	#if 0
	ret = Get_Interface_Info(ifname,ifi);
	if(ret != 0){
		Delete_Interface(ifname, ifindex);
		wid_syslog_debug_debug(WID_DEFAULT,"%s,not find if.line=%d,111111.\n",__func__,__LINE__);
	}else{
	}	
	#endif
	Delete_listenning_IF(ifname);
	
	return 0;

}

int DELETE_LISTENNING_IPADDR(unsigned int ipaddr,LISTEN_FLAG flag){	
	int IGNORE_UBSV n = 0;
	int IGNORE_UBSV ifindex = 0;
	int find = 0;
	unsigned tmp_ip = 0;
	struct CWMultiHomedInterface *p = NULL,*p_next = NULL; 
	p = gListenningIF.interfaces;
	if(p == NULL){  
		wid_syslog_debug_debug(WID_DEFAULT,"%s,line=%d,p=%p",__func__,__LINE__,p);
		return -1;
	}
	p_next = p->if_next;
	tmp_ip = ((struct sockaddr_in *)&(p->addr))->sin_addr.s_addr;
	if((ipaddr == tmp_ip)&&(p->lic_flag == flag))
	{
		ifindex = p->systemIndex;
		gListenningIF.interfaces = p_next;
		p->if_next = NULL;
		free(p);
		p = NULL;
		find = 1;
		if(gListenningIF.count > 0)
			gListenningIF.count--;
		wid_syslog_debug_debug(WID_DEFAULT,"%s,11gListenningIF.count--;count=%d\n",__func__,gListenningIF.count);
	}else{
		while(p_next != NULL){
			n= strlen(p_next->ifname);
			tmp_ip = ((struct sockaddr_in *)&(p_next->addr))->sin_addr.s_addr;
			if((ipaddr == tmp_ip)&&(p_next->lic_flag == flag))
			{
				//p_next = p_next->if_next;
				p->if_next = p_next->if_next;
				p_next->if_next = NULL;
				ifindex = p_next->systemIndex;
				free(p_next);
				p_next = NULL;
				find = 1;
				if(gListenningIF.count > 0)
					gListenningIF.count--;
				wid_syslog_debug_debug(WID_DEFAULT,"%s,22gListenningIF.count--;count=%d\n",__func__,gListenningIF.count);
				break;
			}
			p = p_next;
			p_next= p_next->if_next;
		}
	}
	if(find == 0){
		wid_syslog_debug_debug(WID_DEFAULT,"%s,not find if.line=%d,111111.\n",__func__,__LINE__);
		return INTERFACE_NOT_EXIST;
	}
	Delete_listenning_IP(ipaddr,flag);
	
	return 0;

}

void CWACEnterMainLoop()
{
	CWTimerID TimerID = 0;
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	
	// set signals
	// all the thread we spawn will inherit these settings
	
	act.sa_flags = 0;
	act.sa_handler = CWCriticalTimerExpiredHandler; // called when a timer requested by the thread has expired
	sigaction(CW_CRITICAL_TIMER_EXPIRED_SIGNAL, &act, NULL);
	
	act.sa_flags = 0;
	act.sa_handler = CWSoftTimerExpiredHandler; // called when a timer requested by the thread has expired
	sigaction(CW_SOFT_TIMER_EXPIRED_SIGNAL, &act, NULL);

	act.sa_flags = 0;
	act.sa_handler = CWCriticalTimerExpiredHandler; 
	sigaction(CW_CRITICAL_TIMER_EXPIRED_UPDATE, &act, NULL);
	
	CWThreadSetSignals(SIG_BLOCK, 3, CW_CRITICAL_TIMER_EXPIRED_SIGNAL, // this will be unblocked by the threads that needs timers
 					 CW_SOFT_TIMER_EXPIRED_SIGNAL,
 					 CW_CRITICAL_TIMER_EXPIRED_UPDATE); // this will be unblocked by the threads that needs timers

	
	if(!(CWThreadCreateSpecific(&gIndexSpecific, NULL)))
	{
		wid_syslog_crit("Critical Error With Thread Data");
		return;
	}
	
	
//	p(WidSemid);
//	wait_v(WidSemid);
	//CWThreadMutexLock(&(gACInterfaceMutex));
	//CWWaitThreadCondition(&gACInterfaceWait, &gACInterfaceMutex);
	//CWThreadMutexUnlock(&gACInterfaceMutex);
/*	CWThread thread_interface;
	if(!CWErr(CWCreateThread(&thread_interface, CWInterface1, NULL))) {
		wid_syslog_crit("Error starting Interface Thread");
		exit(1);
	}
*/
	CWThread thread_wd = 0;
	if(!CWErr(CWCreateThread(&thread_wd, CWThreadWD, NULL,0)))
	{
		wid_syslog_crit("Error starting Interface Thread");
		return;
	}
	
	/* 
		syslog indent "wid" just disappeared, I don't kown why, so reopen syslog.
		If you kown, please fix it. Thant you!
	*/
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
	
	CWThread thread_sta_roaming  = 0;
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting sta roaming handler thread\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting sta roaming handler thread\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWCreateThread(&thread_sta_roaming, CWSTARoamingOP, NULL,0)))
	{
		wid_syslog_crit("Error starting Interface Thread");
		return;
	}

	CWThread thread_channel  = 0;
	CWThread thread_channel2  = 0;
	CWThread thread_txpower  = 0;
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting 2.4G dynamic channel selection thread\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"2.4G dynamic channel selection thread\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWCreateThread(&thread_channel, CWDynamicChannelSelection, NULL,0)))
	{
		wid_syslog_crit("Error starting Interface Thread");
		return;
	}
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting 5G dynamic channel selection thread\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"5G dynamic channel selection thread\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWCreateThread(&thread_channel2, CWDynamicChannelSelection2, NULL,0)))
	{
		wid_syslog_crit("Error starting Interface Thread");
		return;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting transmission power ctrl thread\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"transmission power ctrl thread\n", WAI_FUNC_LINE_LITERAL);
	if(!CWErr(CWCreateThread(&thread_txpower, CWTransmitPowerControl, NULL,0)))
	{
		wid_syslog_crit("Error starting Interface Thread");
		return;
	}	

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting process incoming packets\n", WAI_FUNC_LINE_LITERAL);
	YLog(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"starting process incoming packets\n", WAI_FUNC_LINE_LITERAL);
	
	CWTimerRequest(MAX_SURVIVAL_TIME, NULL, &TimerID, TIMER_TYPE_AC_ACCESS, 0);
	
	CW_REPEAT_FOREVER
	{
		if((CWNetworkUnsafeMultiHomed(&gACSocket, CWACManageIncomingPacket, CW_FALSE)) == CW_FALSE) 	// CWACManageIncomingPacket will be called
		{	
			if(vrrid != 0)
			{
				wid_syslog_debug(WAI_FUNC_LINE_FMT"instance %u handle packet error on %u scokets\n", \
									WAI_FUNC_LINE_LITERAL, vrrid, gACSocket.count);
				//sleep(2);
			}
		}
	}
}

CWBool CWUpdateWTPAttachAndWTPManager(CWNetworkLev4Address * addrPtr, unsigned char *mac)
{
    int i = 0;
    
    if (NULL == mac || NULL == addrPtr)
    {
        return CW_FALSE;
    }

    CWThreadMutexLock(&AttachMutex);
    for (i = 0; i < WTP_NUM; i++)
    {
        if (AC_ATTACH[i] != NULL
		    && (0 == memcmp(AC_ATTACH[i]->wtp_mac, mac, MAC_LEN))
		    && sock_cmp_addr((struct sockaddr*)addrPtr, (struct sockaddr*)&(AC_ATTACH[i]->address), sizeof(CWNetworkLev4Address))) 
		{ 
			CWUseSockNtop(&(AC_ATTACH[i]->address),  \
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"update AC_ATTACH[%d].address from %s\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, str););
            CWUseSockNtop(addrPtr, wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"to %s\n",WAI_FUNC_LINE_LITERAL, 0,str););
            memset(&(AC_ATTACH[i]->address), 0, sizeof(CWNetworkLev4Address));
            CW_COPY_NET_ADDR_PTR(&(AC_ATTACH[i]->address), addrPtr);
			break;
		}
    }
    CWThreadMutexUnlock(&AttachMutex);

    CWThreadMutexLock(&gWTPsMutex);
	for (i = 0; i < WTP_NUM; i++) 
	{
	    if (gWTPs[i].isNotFree
	        && (0 == memcmp(gWTPs[i].wtp_mac, mac, MAC_LEN))
	        && sock_cmp_addr((struct sockaddr*)addrPtr, (struct sockaddr*)&(gWTPs[i].address), sizeof(CWNetworkLev4Address)))
	    {
			CWUseSockNtop(&(gWTPs[i].address), wid_syslog_info("update gWTPs[%d].address from %s\n", i, str););
            CWUseSockNtop(addrPtr, wid_syslog_info("to %s\n",str););
            memset(&(gWTPs[i].address), 0, sizeof(CWNetworkLev4Address));
            CW_COPY_NET_ADDR_PTR(&(gWTPs[i].address), addrPtr);
            break;
		}
	}
	CWThreadMutexUnlock(&gWTPsMutex);
    
    return CW_TRUE;
}

CWBool WidGetMACFromJoin(char *msg, CWProtocolHeaderInfo *header_info)
{
	int offsetTillMessages = 0;
	CWBool dataFlag = CW_FALSE;
	unsigned short int elemid = 0;
	unsigned short int elemlen = 0;
	CWProtocolJoinRequestValues values; 
	CWControlHeaderValues controlVal;
	CWProtocolTransportHeaderValues transportVal;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || header_info == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	memset(&values, 0, sizeof(CWProtocolJoinRequestValues));
	memset(&controlVal, 0, sizeof(CWControlHeaderValues));
	memset(&transportVal, 0, sizeof(CWProtocolTransportHeaderValues));
	memset(&completeMsg, 0, sizeof(CWProtocolMessage));
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
  	
	if(!(CWParseTransportHeader(&completeMsg, &transportVal, &dataFlag)))
	{
		return CW_FALSE;
	}
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))  // will be handled by the caller
	{
		return CW_FALSE;
	}
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
	
	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) 
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);

		switch(elemType)
		{
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
			{
				if(!(CWParseWTPBoardData(&completeMsg, elemLen, &(values.WTPBoardData))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE:
			{
				if(!(CWParseLocationData(&completeMsg, elemLen, &(values.location))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
			{
				if(!(CWParseSessionID(&completeMsg, elemLen, &values))) // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
			{
				if(!(CWParseWTPDescriptor(&completeMsg, elemLen, &(values.WTPDescriptor)))) // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE:
			{
				if(!(CWParseWTPIPv4Address(&completeMsg, elemLen, &values)))    // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_NAME_CW_TYPE:
			{
				if(!(CWParseWTPName(&completeMsg, elemLen, &(values.name))))    // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
			{
				if(!(CWParseWTPFrameTunnelMode(&completeMsg, elemLen, &(values.frameTunnelMode))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
			{
				if(!(CWParseWTPMACType(&completeMsg, elemLen, &(values.MACType))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			{
				CWParseFormatMsgElem(&completeMsg, &elemid, &elemlen);
				if(elemlen > controlVal.msgElemsLen)
				{
					wid_syslog_err("%s elemType %d elemlen:%d > controlVal.msgElemsLen:%d\n", __func__, elemType, elemlen, controlVal.msgElemsLen);
					return CW_TRUE;
				}
				if(CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE == elemid)
				{
					if(!(CWParseWTPLOGINType(&completeMsg, elemlen, &(values.ConStatus))))   // will be handled by the caller
					{
						return CW_FALSE;
					}
					
					wid_syslog_info("%s:%d spec_payload:Connect_status %d\n", __func__, __LINE__, values.ConStatus);
				}
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}		
	}
	if(0 == header_info->mac_exist)
	{
		int i = 0;
		int len = 0;
		for(i = 0; i < values.WTPBoardData.vendorInfosCount; i++)
		{
			if((values.WTPBoardData.vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
			{
				len = (values.WTPBoardData.vendorInfos)[i].length;
				if(len != MAC_LEN)
				{
					return CW_FALSE;
				}
				memcpy(header_info->wtp_mac, (values.WTPBoardData.vendorInfos)[i].mac, MAC_LEN);
				break;
			}
		}
		header_info->mac_exist = 1;
	}
	
	header_info->wtp_contype = values.ConStatus;
	
	WidDestroyJoinRequestValuesForAutoApLogin(&values);
	
	return CW_TRUE;
}

CWBool CWGetWTPMAC(char *msg, CWProtocolHeaderInfo *header_info)
{    
    if(NULL == msg || NULL == header_info)
    {
        return CW_FALSE;
    }
    
    if(CW_FALSE == CWGetProtocolHeaderInfo(msg, header_info))
    {
        wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"get protocol header info failed.\n", WAI_FUNC_LINE_LITERAL, 0);
        return CW_FALSE;
    }

	
	if(CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST == header_info->message_type)
	{
		if(CW_FALSE == CWGetMACFromDiscovery(msg, header_info))
		{
            if(0 == header_info->mac_exist)
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"try to get mac from %s payload failed.\n", \
								WAI_FUNC_LINE_LITERAL, 0,WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST));
				return CW_FALSE;
			}
		}
		
        wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv pkt %s.\n",
                        WAI_FUNC_LINE_LITERAL, 0, (header_info->wtp_contype) ? "rediscov-req" : "discov-req");
	}
	else if(CW_MSG_TYPE_VALUE_JOIN_REQUEST == header_info->message_type)
	{
		if(CW_FALSE == WidGetMACFromJoin(msg, header_info))
		{
			if(0 == header_info->mac_exist)
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"try get mac from %s payload faild.\n", \
								WAI_FUNC_LINE_LITERAL, 0, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST));
				return CW_FALSE;
			}
		}
		
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv pkt %s.\n",
						WAI_FUNC_LINE_LITERAL, 0, (header_info->wtp_contype) ? "rejoin-req" : "join-req");
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"capwap msg %s payload %s "WAI_MAC_FMT"\n", \
					WAI_FUNC_LINE_LITERAL, 0, WAI_CAPWAP_MSG_TYPE_STR(header_info->message_type), \
					"has mac", WAI_MAC_LITERAL(header_info->wtp_mac));

    return CW_TRUE;
}

// This callback function is called when there is something to read in a CWMultiHomedSocket (see ACMultiHomed.c)
// ***
// sock is the socket that can receive the packet and it can be used to reply
// ***
// buf (array of len chars) contains the packet which is ready on the socket's queue (obtained with MSG_PEEK)
// ***
// incomingInterfaceIndex is the index (different from the system index, see ACMultiHomed.c) of the interface the packet
// was sent to, in the array returned by CWNetworkGetInterfaceAddresses. If the packet was sent to a broadcast/multicast address,
// incomingInterfaceIndex is -1
void CWACManageIncomingPacket
(
	CWSocket sock,
	char *buf,
	int readBytes,
	int incomingInterfaceIndex,
	int BindingSystemIndex,
	CWNetworkLev4Address *addrPtr,
	char *ifname
)
{
	CWWTPManager *wtpPtr = NULL;
	msgqData qData;	
	int ret = 0;
	int WTPID = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int cap_pro_ver = 0;
	unsigned int flag = 0;
    WTPLoginState WTP_LOGIN = 0;
	CWProtocolHeaderInfo header_info;

	if (NULL == buf || NULL == addrPtr)
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"handle pkt on sock %u %u bytes input arg error(%p,%p)\n", \
								WAI_FUNC_LINE_LITERAL, 0, sock, readBytes, buf, addrPtr);
		return;
	}
	
	memset(&header_info, 0, sizeof(CWProtocolHeaderInfo));
	
	wid_hex_dump((unsigned char *)buf, readBytes);
	
	if (CW_FALSE == CWGetWTPMAC(buf, &header_info))
	{
		CWUseSockNtop(addrPtr,
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"pkt from %s get mac from message failed.\n", WAI_FUNC_LINE_LITERAL, 0, str););
		return;
	}

	if (0 != header_info.mac_exist)
	{
		CWUpdateWTPAttachAndWTPManager(addrPtr, header_info.wtp_mac);
	    CWUseSockNtop(addrPtr, 
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv pkt %s%s(%d) capprover %d from %s mac "MACSTR" sock %d\n",  \
								WAI_FUNC_LINE_LITERAL, 0, (header_info.wtp_contype) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), \
								header_info.message_type, header_info.protocolver, str, MAC2STR(header_info.wtp_mac), sock););
	}
	else
	{
	    CWUseSockNtop(addrPtr, 
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv pkt %s%s(%d) capprover %d from %s sock %d\n", \
								WAI_FUNC_LINE_LITERAL, 0, (header_info.wtp_contype) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), \
								header_info.message_type, header_info.protocolver, str, sock););
	}

	memset(&qData, 0, sizeof(msgq));
	WTP_LOGIN = header_info.wtp_contype;
	cap_pro_ver = header_info.protocolver;
	
	wtpPtr = CWWTPByAddress(addrPtr, sock, &WTPID);
	if ((wtpPtr != NULL) && (!check_wtpid_func(WTPID)))
	{
		CWUseSockNtop(addrPtr,
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT" from %s sock %d id error\n", WAI_FUNC_LINE_LITERAL,WTPID, str, sock););
		return;
	}
	
	if (wtpPtr != NULL)
	{
		CWUseSockNtop(addrPtr, 
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv pkt %s%s(%d) from %s match by address\n", \
						WAI_FUNC_LINE_LITERAL, WTPID, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), \
						header_info.message_type, str););
	}
	else
	{
		CWUseSockNtop(addrPtr, 		
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv pkt %s%s(%d) from %s no match by address\n", \
						WAI_FUNC_LINE_LITERAL, 0, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), \
						header_info.message_type, str););		
	}
	
	if ((AC_WTP[WTPID] != NULL) && (AC_WTP[WTPID]->BindingSystemIndex != BindingSystemIndex))
	{
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"change interface %s(%u) to %s(%u)\n", \
						WAI_FUNC_LINE_LITERAL,WTPID, AC_WTP[WTPID]->WTPNAME, AC_WTP[WTPID]->BindingIFName,  \
						AC_WTP[WTPID]->BindingSystemIndex, ifname, BindingSystemIndex);
		AC_WTP[WTPID]->BindingSystemIndex= BindingSystemIndex;
		memset(AC_WTP[WTPID]->BindingIFName, 0, ETH_IF_NAME_LEN);
		memcpy(AC_WTP[WTPID]->BindingIFName,ifname, strlen(ifname));
		
		wtp_get_ifindex_check_nas_id(WTPID);
	}
	
	struct tm *p = NULL; 
	struct sockaddr_in	*sin = (struct sockaddr_in *)addrPtr;	
	struct wtp_access_info *wtp = NULL;
	CWThreadMutexLock(&ACAccessWTP);
	wtp = get_ap(AC_WTP_ACC,sin->sin_addr.s_addr);
	if(wtp != NULL)
	{
		time(&(wtp->time_stamp));     
		p = localtime(&(wtp->time_stamp));
		wid_syslog_info("%s:%d get ap["MACSTR"] time %d:%d:%d\n",__func__, __LINE__,MAC2STR(wtp->WTPMAC),p->tm_hour,p->tm_min,p->tm_sec);
	}
	CWThreadMutexUnlock(&ACAccessWTP);
	
	if ((wtpPtr != NULL) && (wtpPtr->BAK_FLAG == 2))
	{
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"change interface index%u\n", \
						WAI_FUNC_LINE_LITERAL,WTPID,AC_WTP[WTPID]->WTPNAME,incomingInterfaceIndex);

#if NOSOFTAC
		for (j = 0; (AC_WTP[WTPID])&&(j < AC_WTP[WTPID]->RadioCount); j++)
		{
			if ((AC_WTP[WTPID]) && (AC_WTP[WTPID]->WTP_Radio[j]))
			{
				for (k = 0; k < L_BSS_NUM; k++)
				{
					if ((AC_WTP[WTPID]) && (AC_WTP[WTPID]->WTP_Radio[j]) && (AC_WTP[WTPID]->WTP_Radio[j]->BSS[k] != NULL))
					{
						wid_update_wtp_bss_infov2(WTPID,AC_WTP[WTPID]->WTP_Radio[j]->BSS[k]->BSSIndex);
					}
				}
			}
		}
#endif
		wtpPtr->interfaceIndex = incomingInterfaceIndex;	
		
		if ((incomingInterfaceIndex < gMaxInterfacesCount) && (incomingInterfaceIndex >= 0))
		{
			gInterfaces[incomingInterfaceIndex].WTPCount++;
		}
		else
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"pkt from illegal interface %#x\n", WAI_FUNC_LINE_LITERAL, WTPID, incomingInterfaceIndex);
		}
		
		wtpPtr->BAK_FLAG = 0;
		
		CWThreadMutexLock(&MasterBak);
		struct bak_sock *tmp = bak_list;
		if ((is_secondary == 0) && (bak_list != NULL))
		{
			bak_add_del_wtp(tmp->sock, B_ADD, WTPID);
			for (j = 0; (AC_WTP[WTPID])&&(j < AC_WTP[WTPID]->RadioCount); j++)
			{
				if ((AC_WTP[WTPID]) && (AC_WTP[WTPID]->WTP_Radio[j]) && (AC_WTP[WTPID]->WTP_Radio[j]->AdStat == 1))
				{
					for (k = 0; k < L_BSS_NUM; k++)
					{
						if ((AC_WTP[WTPID])&&(AC_WTP[WTPID]->WTP_Radio[j])&&(AC_WTP[WTPID]->WTP_Radio[j]->BSS[k] != NULL)&&(AC_WTP[WTPID]->WTP_Radio[j]->BSS[k]->State == 1))
						{
							bak_add_del_bss(tmp->sock, B_ADD, AC_WTP[WTPID]->WTP_Radio[j]->BSS[k]->BSSIndex);
						}
					}
				}
			}
		}
		CWThreadMutexUnlock(&MasterBak);
	}
	if (wtpPtr != NULL)  // known WTP
	{
		if ((AC_WTP[WTPID] != NULL) && (AC_WTP[WTPID]->WTPStat == WID_BAK_RUN))
		{
			CWThreadMutexLock(&(gWTPs[WTPID].WTPThreadControllistMutex));
			WID_CLEAN_CONTROL_LIST(WTPID);
			CWThreadMutexUnlock(&(gWTPs[WTPID].WTPThreadControllistMutex));
			//AC_WTP[WTPID]->channelsendtimes = 0;
			for (m = 0; ((AC_WTP[WTPID]) && (m < AC_WTP[WTPID]->RadioCount) && (m < L_RADIO_NUM)); m++)
			{
				AC_WTP[WTPID]->WTP_Radio[m]->channelsendtimes = 0;
			}
			gWTPs[WTPID].currentState = CW_ENTER_RUN;
			AC_WTP[WTPID]->WTPStat = WID_RUN;
			
			if (apstatistics == 1)                //主备切换，第一次由备变主的AC，show ap statistics相关信息没有与原主同步
			{
				wid_set_ap_statistics_v1(WTPID, 0);
				wid_set_ap_statistics_v1(WTPID, 1);
				/*send to ap interval when switch is enable*/
				wid_set_ap_statistics_interval(WTPID,AC_WTP[WTPID]->apstatisticsinterval);
			}
			AsdWsm_WTPOp(WTPID,WID_MODIFY);    //主备切换更新ASD侧相关信息
			
		}
		// Clone data packet		
		//CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { wid_syslog_crit("Out Of Memory"); return; });
		if ((WID_RELOGIN == WTP_LOGIN) && (CW_MSG_TYPE_VALUE_JOIN_REQUEST == header_info.message_type))
		{
            gWTPs[WTPID].currentState = CW_ENTER_JOIN; // we're in the join state for this session
            AC_WTP[WTPID]->WTPStat = WID_REJOIN;
		}
		AC_WTP[WTPID]->cap_pro_ver = cap_pro_ver;
		
		qData.mqid = WTPID%THREAD_NUM+1;
		qData.mqinfo.WTPID = WTPID;
		qData.mqinfo.type = DATA_TYPE;
		qData.mqinfo.u.DataInfo.len = readBytes;
		memset(qData.mqinfo.u.DataInfo.Data, 0, 4096);
		memcpy(qData.mqinfo.u.DataInfo.Data, buf, readBytes);
		CWUseSockNtop(addrPtr,wid_syslog_debug(WAI_FUNC_LINE_FMT"existing "WAI_AFI_FMT"pickup pkt %s%s from %s\n", \
						WAI_FUNC_LINE_LITERAL,	WTPID, AC_WTP[WTPID]->WTPNAME, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), str););
		if (msgsnd(WidMsgQid, (msgq *)&qData, sizeof(qData.mqinfo), 0) == -1)
		{
			wid_syslog_info(WAI_FUNC_LINE_FMT"msgsend error %s",WAI_FUNC_LINE_LITERAL,strerror(errno));
			perror("msgsnd");
		}

		//CWLockSafeList(wtpPtr->packetReceiveList);
		//CWAddElementToSafeListTail(wtpPtr->packetReceiveList, pData, readBytes);
		//CWUnlockSafeList(wtpPtr->packetReceiveList);		
	}
	else // unknown WTP
	{
		int seqNum = 0, IGNORE_UBSV tmp = 0;
		CWDiscoveryRequestValues values;
		
		if (!CWErr(CWThreadMutexLock(&gActiveWTPsMutex)))
		{
			wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"can't lock treadmutex\n", WAI_FUNC_LINE_LITERAL, 0);
			return;
		}
		tmp = gActiveWTPs;
		CWThreadMutexUnlock(&gActiveWTPsMutex);
		
		if (gActiveWTPs >= WTP_NUM)
		{
			wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"excessive wtp %u than spec %u\n", WAI_FUNC_LINE_LITERAL, 0, gActiveWTPs, WTP_NUM);
			return;
		}
		
		memset(&values, 0, sizeof(CWDiscoveryRequestValues));
		/*
		 MAURO: passa il pacchetto privato del prefisso di lunghzza 
		 del preambolo. Ritorna in seqNum il numero di sequenza 
		 presente nella richiesta.
		if(CWErr(CWParseDiscoveryRequestMessage(&buf[4], readBytes - 4, &seqNum, &values))) { // is a CAPWAP Discovery Request message without preamble
		*/
		if ((CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST == header_info.message_type)
		    && CWErr(CWParseDiscoveryRequestMessage(buf, readBytes, &seqNum, &values))) 
		{
			CWProtocolMessage *msgPtr = NULL;
			unsigned int WTPID = 0;

			if (CWDisCheckWTPBoardData(BindingSystemIndex, addrPtr, &(values.WTPBoardData), &WTPID, WTP_LOGIN))
			{
				if (!check_wtpid_func(WTPID))
				{
					CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"from %s id error\n", WAI_FUNC_LINE_LITERAL, WTPID, str););
					CWDestroyDiscoveryRequestValues(&values);
					return;
				}
				
				if (AC_WTP[WTPID] != NULL)
				{
					CWThreadMutexLock(&(gOuiMacXmlMutex));	
					if (oui_mac_filters(AC_WTP[WTPID]->WTPMAC))
					{
						CWCmpWTPAttach(addrPtr);//free AC_ATTACH
						CWThreadMutexUnlock(&(gOuiMacXmlMutex));	
						CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"" \
									WAI_DISCOVERY_LITERAL"from %s not match oui policy we give up and drop attach\n", \
									WAI_FUNC_LINE_LITERAL, WTPID,AC_WTP[WTPID]->WTPNAME, str););
						return;
					}
					CWThreadMutexUnlock(&(gOuiMacXmlMutex));
					AC_WTP[WTPID]->cap_pro_ver = cap_pro_ver;
				}
				
				if (WID_LOGIN == WTP_LOGIN)
				{
					CWThreadMutexLock(&ACAccessWTP);
					ap_add(AC_WTP_ACC,(struct sockaddr_in *)addrPtr,&(values.WTPBoardData),&(values.WTPDescriptor),ifname);
					CWThreadMutexUnlock(&ACAccessWTP);
				}
				
				int n = 0;
				for (n = 0; n < values.WTPBoardData.vendorInfosCount; n++)
				{
					if ((values.WTPBoardData.vendorInfos)[n].type == CW_WTP_MANUFACTURE_OPTION)//CW_WTP_MANUFACTURE_OPTION = 7
					{
						gWTPs[WTPID].oemoption = (values.WTPBoardData.vendorInfos)[n].manuoption;
						if (gWTPs[WTPID].oemoption > (glicensecount-1))
						{
							gWTPs[WTPID].oemoption = 0;							
						}
					}
				}
				
				CWThreadMutexLock(&ACLicense);
				if (g_wtp_count[gWTPs[WTPID].oemoption]->flag == 0)
				{
					if (g_wtp_count[gWTPs[WTPID].oemoption]->gcurrent_wtp_count >= g_wtp_count[gWTPs[WTPID].oemoption]->gmax_wtp_count)
					{
						CWDestroyDiscoveryRequestValues(&values);
						CWThreadMutexUnlock(&ACLicense);
						CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"" WAI_DISCOVERY_LITERAL \
									"from %s count %u excess spec %u we deny access\n", WAI_FUNC_LINE_LITERAL, WTPID, str, \
									g_wtp_count[gWTPs[WTPID].oemoption]->gcurrent_wtp_count,  \
									g_wtp_count[gWTPs[WTPID].oemoption]->gmax_wtp_count););
						return;
					}
				}
				else
				{
					flag = g_wtp_count[gWTPs[WTPID].oemoption]->flag;
					if (g_wtp_binding_count[flag]->gcurrent_wtp_count >= g_wtp_binding_count[flag]->gmax_wtp_count)
					{
						CWDestroyDiscoveryRequestValues(&values);
						CWThreadMutexUnlock(&ACLicense);
						CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"" WAI_DISCOVERY_LITERAL \
									"from %s oem count %u excess spec %u we deny access\n", WAI_FUNC_LINE_LITERAL, WTPID, str, \
									g_wtp_binding_count[flag]->gcurrent_wtp_count, g_wtp_binding_count[flag]->gmax_wtp_count););
						return;;
					}
				}
				CWThreadMutexUnlock(&ACLicense);
				
    			// don't add this WTP to our list to minimize DoS attacks (will be added after join)
    			// send response to WTP
				if (!CWErr(CWAssembleDiscoveryResponse(&msgPtr, seqNum, WTPID, cap_pro_ver)))  // note: we can consider reassembling only changed part AND/OR do this in a new thread
				{
				    wid_syslog_warning("Critical Error Assembling %s Response",(WID_LOGIN == WTP_LOGIN)?"Discovery":"ReDiscovery");
					//exit(1);	// note: maybe an out-of-memory memory error can be resolved without exit()-ing
					// by killing some thread or doing other funky things
					CWDestroyDiscoveryRequestValues(&values); 
					CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
					CW_FREE_OBJECT(msgPtr);
					
					return;
				}
			
				if (!CWErr(CWNetworkSendUnsafeUnconnected(sock, addrPtr, (*msgPtr).msg, (*msgPtr).offset)))
				{
					wid_syslog_warning("Critical Error Sending %s Response",(WID_LOGIN == WTP_LOGIN)?"Discovery":"ReDiscovery");
				}
				
				CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
				CW_FREE_OBJECT(msgPtr);
			}
			else
			{
				if (WID_LOGIN == WTP_LOGIN)
				{
					CWThreadMutexLock(&ACAccessWTP);
					ap_add(AC_WTP_ACC,(struct sockaddr_in *)addrPtr,&(values.WTPBoardData),&(values.WTPDescriptor),ifname);	
	 				CWThreadMutexUnlock(&ACAccessWTP);
				}
				if (g_auto_ap_login.auto_ap_switch == 0)
				{
					wid_syslog_debug("%s:%d g_auto_ap_login.auto_ap_switch == 0\n", __func__, __LINE__);
				}
				if ((g_auto_ap_login.auto_ap_switch == 1) || (WID_RELOGIN == WTP_LOGIN))
				{
					WIDAUTOAPINFO *auto_ap_info = NULL;
		
					auto_ap_info = parse_dynamic_wtp_login_situation(&(values.WTPBoardData));
					if ((auto_ap_info != NULL) && (auto_ap_info->mac != NULL))
					{
						CWThreadMutexLock(&(gOuiMacXmlMutex));	
						if (oui_mac_filters(auto_ap_info->mac))
						{
							CWDestroyDiscoveryRequestValues(&values);
							WidDestroyAutoApLoginInfo(auto_ap_info);
							CWThreadMutexUnlock(&(gOuiMacXmlMutex));	
							CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"" \
										WAI_DISCOVERY_LITERAL"from %s not match oui policy we give up and drop packet\n", \
										WAI_FUNC_LINE_LITERAL, WTPID, str););
							return;
						}
						CWThreadMutexUnlock(&(gOuiMacXmlMutex));	
					}
					int n = 0;
					int oemoption = 0;
					for (n = 0; n < values.WTPBoardData.vendorInfosCount; n++)
					{
						if ((values.WTPBoardData.vendorInfos)[n].type == CW_WTP_MANUFACTURE_OPTION)//CW_WTP_MANUFACTURE_OPTION = 7
						{
							oemoption = (values.WTPBoardData.vendorInfos)[n].manuoption;
							if (oemoption > (glicensecount-1))
							{
								CWUseSockNtop(addrPtr, wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"" WAI_DISCOVERY_LITERAL \
											"from %s board data%u manufacture option %u unkown we use 0\n", WAI_FUNC_LINE_LITERAL, 0, str,oemoption););
								oemoption = 0; 						
							}
						}
					}
					
					CWThreadMutexLock(&ACLicense);
					if (g_wtp_count[oemoption]->flag == 0)
					{
						if (g_wtp_count[oemoption]->gcurrent_wtp_count >= g_wtp_count[oemoption]->gmax_wtp_count)
						{
							CWDestroyDiscoveryRequestValues(&values);
							WidDestroyAutoApLoginInfo(auto_ap_info);
							CWThreadMutexUnlock(&ACLicense);
							CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"" WAI_DISCOVERY_LITERAL \
										"from %s count %u excess spec %u we deny access\n", WAI_FUNC_LINE_LITERAL, 0, str, \
										g_wtp_count[oemoption]->gcurrent_wtp_count, g_wtp_count[oemoption]->gmax_wtp_count););
							return;
						}
					}
					else
					{
						flag = g_wtp_count[oemoption]->flag;
						if (g_wtp_binding_count[flag]->gcurrent_wtp_count >=g_wtp_binding_count[flag]->gmax_wtp_count)
						{
							CWDestroyDiscoveryRequestValues(&values);
							WidDestroyAutoApLoginInfo(auto_ap_info);
							CWThreadMutexUnlock(&ACLicense);
							CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"" WAI_DISCOVERY_LITERAL \
										"from %s oem count %u excess spec %u we deny access\n", WAI_FUNC_LINE_LITERAL, 0, str, \
										g_wtp_binding_count[flag]->gcurrent_wtp_count, g_wtp_binding_count[flag]->gmax_wtp_count););
							return;;
						}
					}
					CWThreadMutexUnlock(&ACLicense);
					
					if (auto_ap_info != NULL)
					{
						if (!CWErr(CWAssembleDiscoveryResponse(&msgPtr, seqNum, 0, cap_pro_ver)))
						{
							wid_syslog_warning("Critical Error Assembling %s Response",(WID_LOGIN == WTP_LOGIN)?"Discovery":"ReDiscovery");							
							WidDestroyAutoApLoginInfo(auto_ap_info);
							CWDestroyDiscoveryRequestValues(&values); 
							return;
						}
					
						if (!CWErr(CWNetworkSendUnsafeUnconnected(sock, addrPtr, (*msgPtr).msg, (*msgPtr).offset))) 
						{
							wid_syslog_warning("Critical Error Sending %s Response",(WID_LOGIN == WTP_LOGIN)?"Discovery":"ReDiscovery");
						}
					
						CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
						CW_FREE_OBJECT(msgPtr);
					}
					WidDestroyAutoApLoginInfo(auto_ap_info);
				}
			}
			
			CWDestroyDiscoveryRequestValues(&values); // destroy useless values
		}
		else  // this isn't a Discovery Request
		{
			int i = 0;
			CWACThreadArg *argPtr = NULL;
			char __str[128] = {0};
			char *ip = NULL;
			CWUseSockNtop(addrPtr,wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"pkt %s%s from %s\n", \
							WAI_FUNC_LINE_LITERAL, 0, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), str););
			CWUseSockNtop(addrPtr,YLog(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"pkt %s%s from %s\n", \
							WAI_FUNC_LINE_LITERAL, 0, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), str););
			ip = sock_ntop_r(((struct sockaddr*)(addrPtr)), __str);

			if (((i = CWCmpWTPAttach(addrPtr)) > 0)
				&& (AC_WTP[i] != NULL)
				&& (AC_WTP[i]->isused == 1)/*&&(CWWTPMatchBindingInterface(i,BindingSystemIndex))*/)
			{
				if (!CWErr(CWThreadMutexLock(&gWTPsMutex)))
				{
					wid_syslog_crit(WAI_FUNC_LINE_FMT"take lock failed", WAI_FUNC_LINE_LITERAL);
					return;
				}
	    			
				CW_COPY_NET_ADDR_PTR(&(gWTPs[i].address), addrPtr);
	    	
				if (AC_WTP[i])
				{
					memset(AC_WTP[i]->WTPIP, 0, DEFAULT_LEN);
					if (strlen(ip) > DEFAULT_LEN)
					{
						memcpy(AC_WTP[i]->WTPIP, ip, DEFAULT_LEN);
					}
					else
					{
						memcpy(AC_WTP[i]->WTPIP, ip, strlen(ip));
					}
				}
				AC_WTP[i]->cap_pro_ver = cap_pro_ver;
				AC_WTP[i]->BindingSystemIndex = BindingSystemIndex;
				memset(AC_WTP[i]->BindingIFName, 0, ETH_IF_NAME_LEN);
				memcpy(AC_WTP[i]->BindingIFName, ifname, strlen(ifname));
	    			
				wtp_get_ifindex_check_nas_id(i);

    			gWTPs[i].isRequestClose = CW_FALSE;
    			CWThreadMutexUnlock(&gWTPsMutex);
#ifndef CW_NO_DTLS
    			// Capwap receive packets list
    			if (!CWErr(CWCreateSafeList(&gWTPs[i].packetReceiveList)))
    			{
					if (!CWErr(CWThreadMutexLock(&gWTPsMutex)))
					{
						wid_syslog_crit(WAI_FUNC_LINE_FMT"take lock failed", WAI_FUNC_LINE_LITERAL);
						return;
					}
					gWTPs[i].isNotFree = CW_FALSE;
				    CWThreadMutexUnlock(&gWTPsMutex);
					
					wid_syslog_crit(WAI_FUNC_LINE_FMT"create packetReceiveList failed", WAI_FUNC_LINE_LITERAL);
    			    return;
    			}
	    		
    			CWSetMutexSafeList(gWTPs[i].packetReceiveList, &gWTPs[i].interfaceMutex);
    			CWSetConditionSafeList(gWTPs[i].packetReceiveList, &gWTPs[i].interfaceWait);
#endif		
    			CW_CREATE_OBJECT_ERR(argPtr, CWACThreadArg,  \
    							{ wid_syslog_crit(WAI_FUNC_LINE_FMT"Out Of Memory\n", WAI_FUNC_LINE_LITERAL); return; });

				argPtr->index = i;
				argPtr->sock = sock;
				argPtr->interfaceIndex = incomingInterfaceIndex;
				if (WID_WTP_INIT((void *)argPtr) == CW_FALSE)
				{
					return;
				}	
    			/* coverity-CID:10970 Read from pointer after free */
    			/*
    			if (argPtr->interfaceIndex < 0) argPtr->interfaceIndex = 0; // if the packet was addressed to a broadcast address,
    			*/
    			// just choose an interface we like (note: we can consider
    			// a bit load balancing instead of hard-coding 0-indexed interface).
    			// Btw, Join Request should not really be accepted if addressed to a
    			// broadcast address, so we could simply discard the packet
    			// and go on.
    			// If you leave this code, the WTP Count will increase for the interface
    			// we hard-code here, even if it is not necessary the interface we use to
    			// send packets to that WTP.
    			// If we really want to accept Join Request from broadcast address,
    			// we can consider asking to the kernel which interface will be used
    			// to send the packet to a specific address (if it remains the same)
    			// and than increment WTPCount for that interface instead of 0-indexed one.
    			
    			// create the thread that will manage this WTP
    			/*if(!CWErr(CWCreateThread(&(gWTPs[i].thread), CWManageWTP, argPtr,1))) {
    				CW_FREE_OBJECT(argPtr);
    				if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) {
    					wid_syslog_crit("can't lock threadmutex");
    					exit(1);
    					}
    					CWDestroySafeList(&gWTPs[i].packetReceiveList);
    					gWTPs[i].isNotFree = CW_FALSE;
    				CWThreadMutexUnlock(&gWTPsMutex);
    				return;
    			}*/
    	
    			// Clone data packet		
    /*			CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, {wid_syslog_crit("Out Of Memory"); return; });
    			memcpy(pData, buf, readBytes);

    			CWLockSafeList(gWTPs[i].packetReceiveList);
    			CWAddElementToSafeListTail(gWTPs[i].packetReceiveList, pData, readBytes);
    			CWUnlockSafeList(gWTPs[i].packetReceiveList);
    */
				if ((WID_RELOGIN == WTP_LOGIN) && (CW_MSG_TYPE_VALUE_JOIN_REQUEST == header_info.message_type))
				{
					gWTPs[i].currentState = CW_ENTER_JOIN; // we're in the join state for this session
					AC_WTP[i]->WTPStat = WID_REJOIN;
				}
				qData.mqid = i%THREAD_NUM+1;
				qData.mqinfo.WTPID = i;
				qData.mqinfo.type = DATA_TYPE;
				qData.mqinfo.u.DataInfo.len = readBytes;
				memset(qData.mqinfo.u.DataInfo.Data, 0, 4096);
				memcpy(qData.mqinfo.u.DataInfo.Data, buf, readBytes);
				CWUseSockNtop(addrPtr,wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"pickup pkt %s%s from %s\n",
							  WAI_FUNC_LINE_LITERAL, i, (WTP_LOGIN) ? "re":"",
							  WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), str););
				
				if (msgsnd(WidMsgQid, (msgq *)&qData, sizeof(qData.mqinfo), 0) == -1)
				{
					wid_syslog_info(WAI_FUNC_LINE_FMT"msgsend error %s",WAI_FUNC_LINE_LITERAL,strerror(errno));
					perror("msgsnd");
				}
			}
			else
			{
				if (CW_MSG_TYPE_VALUE_JOIN_REQUEST == header_info.message_type)
				{
					int IGNORE_UBSV index = 0;
					index = WTPAddrCheck(addrPtr);
					
					CWUseSockNtop(addrPtr,wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"pickup pkt %s%s from %s\n",
								  WAI_FUNC_LINE_LITERAL, (-1 == i) ? 0:i, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), str););
					if ((g_auto_ap_login.auto_ap_switch == 1) || (WID_RELOGIN == WTP_LOGIN))
					{	
						int id = 0;
						unsigned int wtp_index = 0;
						CWProtocolJoinRequestValues joinRequest;
						memset(&joinRequest, 0, sizeof(CWProtocolJoinRequestValues));
						
						if (!(WidParseJoinRequestMessageForAutoApLogin(buf, readBytes, &seqNum, &joinRequest))) 
						{
							CWUseSockNtop(addrPtr, 
										wid_syslog_debug(WAI_FUNC_LINE_FMT"msg %s%s possible of "WAI_AFI0_FMT"from %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), index, str););
							WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
							return;
						}
					
						//check dynamic wtp index
						id = parse_dynamic_wtp_name(joinRequest.name);
					
						if (id == -1)
						{
							CWUseSockNtop(addrPtr,
										wid_syslog_err(WAI_FUNC_LINE_FMT"msg %s%s possible of"WAI_AFI0_FMT"from %s by name %s has no legal index,drop it\n", \
										WAI_FUNC_LINE_LITERAL, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST),  \
										index, str, joinRequest.name););
							WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
							return;
						}
						else if (id != 0)
						{
							wtp_index = id;
						}
						else
						{
							if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) 
							{
								CWUseSockNtop(addrPtr,
									wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"msg %s%s from %s can't lock threadmutex\n", \
												WAI_FUNC_LINE_LITERAL, id, (WTP_LOGIN) ? "re" : "",
												WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), str););
								return;
							}
							for (i = 1; i < WTP_NUM; i++) 
							{	
								if ((!(gWTPs[i].isNotFree)) && (AC_WTP[i] == NULL))
								{
									wtp_index = i;
									CWUseSockNtop(addrPtr,
												wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"msg %s%s from %s name '%s' got a random assigned id\n", \
												WAI_FUNC_LINE_LITERAL, wtp_index, (WTP_LOGIN) ? "re" : "",
												WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), str, joinRequest.name););
									break;
								}
							}
							CWThreadMutexUnlock(&gWTPsMutex);
						}
					
						if (wtp_index == 0)
						{
							CWUseSockNtop(addrPtr,
												wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"msg %s%s from %s can't login because count limit %d\n", \
												WAI_FUNC_LINE_LITERAL, 0, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), str, WTP_NUM-1););
							WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
							return;
						}
					
						WIDAUTOAPINFO *auto_ap_info2 = NULL;
						auto_ap_info2 = parse_dynamic_wtp_login_situation(&(joinRequest.WTPBoardData));
						if (auto_ap_info2 != NULL)
						{
							if (id != 0)
							{
								CWUseSockNtop(addrPtr,
											wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"msg %s%s from %s change basic login info we wait for next login\n",\
											WAI_FUNC_LINE_LITERAL, id, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), str););
								wid_dynamic_change_wtp_info(id,auto_ap_info2);
								WidDestroyAutoApLoginInfo(auto_ap_info2);
								WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
								return;
							}
							char name[32] = {0};
							//char *name = "autoap";

							if (ret != -1)
							{
								int result = 0;
								result = wid_parse_wtp_model_rmodel(auto_ap_info2);
								if (result == -1)
								{
									CWUseSockNtop(addrPtr,
												wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"msg %s%s from %s real model match failed we drop pkt\n", \
												WAI_FUNC_LINE_LITERAL, wtp_index, (WTP_LOGIN) ? "re" : "",
												WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), str););
									WidDestroyAutoApLoginInfo(auto_ap_info2);	
									WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
									return;
								}
								char *sn = (char *)(auto_ap_info2->sn);
								char *model = (char *)(auto_ap_info2->realmodel);
								unsigned char mac[7] = {0};
								//int apcodeflag = auto_ap_info2->apcodeflag;
								char *code = (char*)(auto_ap_info2->model);

								memcpy(mac, auto_ap_info2->mac, MAC_LEN);
								snprintf(name, 31, "AFi_%02X%02X%02X", mac[3], mac[4], mac[5]);
								/*check whether mac exist*/
								for (i = 1; i < WTP_NUM; i++)
								{
									if ((AC_WTP[i] != NULL)
										&& (AC_WTP[i]->WTPMAC != NULL)
										&& (memcmp(AC_WTP[i]->WTPMAC, mac, MAC_LEN) == 0)
										&& (memcmp(AC_WTP[i]->WTPMAC, gdefaultMac, MAC_LEN) != 0))
									{
										CWAddAC_ATTACH_For_Auto(addrPtr, i, (unsigned char*)mac);
										CWUseSockNtop(addrPtr,
													wid_syslog_err(WAI_FUNC_LINE_FMT" "WAI_AFI_FMT"msg %s%s from %s mac "WAI_MAC_FMT \
																	"already used by "WAI_AFI0_FMT" and drop pkt\n", WAI_FUNC_LINE_LITERAL,  \
																	wtp_index, AC_WTP[wtp_index]->WTPNAME, (WTP_LOGIN) ? "re" : "",
																	WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST),
																	str, WAI_MAC_LITERAL(mac), i););
										WidDestroyAutoApLoginInfo(auto_ap_info2);	
										WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
										return;
									}
								}
							
								if (!check_wtpid_func(wtp_index))
								{
									CWUseSockNtop(addrPtr,
											wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg %s%s from %s auto login found id illegal\n", \
														WAI_FUNC_LINE_LITERAL, wtp_index, AC_WTP[wtp_index]->WTPNAME, (WTP_LOGIN) ? "re":"",
														WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST),str););
									WidDestroyAutoApLoginInfo(auto_ap_info2);	
									WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
									return ;
								}
							
								CWThreadMutexLock(&(gWTPs[WTPID].WTPThreadMutex));//fengwenchao add 20121123 for AXSSZFI-1050
								ret = WID_CREATE_NEW_WTP(name, wtp_index, mac, model, 0, 0, code);
								CWThreadMutexUnlock(&(gWTPs[WTPID].WTPThreadMutex));
							
								if (ret == 0)
								{
									AC_WTP[wtp_index]->wtp_login_mode = 1;
									AC_WTP[wtp_index]->cap_pro_ver = cap_pro_ver;
									memset(AC_WTP[wtp_index]->WTPSN, 0, NAS_IDENTIFIER_NAME);
									if (wid_illegal_character_check((char*)sn,strlen(sn),0) == 1)
									{
										memcpy(AC_WTP[wtp_index]->WTPSN, sn, strlen(sn));
									}
									else
									{
										CWUseSockNtop(addrPtr,
											wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg %s%s from %s sn has non readable char\n",\
													WAI_FUNC_LINE_LITERAL,wtp_index,  AC_WTP[wtp_index]->WTPNAME, (WTP_LOGIN) ? "re":"",
													WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), str););
									}
								
									if (wid_auto_ap_binding(wtp_index,BindingSystemIndex) == 0)
									{
										//send to asd
									
										CWAddAC_ATTACH_For_Auto(addrPtr, wtp_index, (unsigned char*)mac);
							#if NOSOFTAC			
										AsdWsm_WTPOp(wtp_index,WID_ADD);
							#endif			
										int n = 0;
										for (n = 0; n < joinRequest.WTPBoardData.vendorInfosCount; n++)
										{
											if ((joinRequest.WTPBoardData.vendorInfos)[n].type == CW_WTP_MANUFACTURE_OPTION)//CW_WTP_MANUFACTURE_OPTION = 7
											{
												gWTPs[wtp_index].oemoption = (joinRequest.WTPBoardData.vendorInfos)[n].manuoption;
												if (gWTPs[wtp_index].oemoption > (glicensecount-1))
												{
													gWTPs[wtp_index].oemoption = 0;
												}
											}
										}
										
										CWUseSockNtop(addrPtr,wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"%s from %s%s create and bind success\n",
														WAI_FUNC_LINE_LITERAL, wtp_index, (WTP_LOGIN) ? "re":"", "join-req", str););
									}
									else
									{
										CWThreadMutexLock(&(gWTPs[WTPID].WTPThreadMutex));
										if (AC_WTP[wtp_index] != NULL)
										{
											WID_DELETE_WTP(wtp_index);
										}
										CWThreadMutexUnlock(&(gWTPs[WTPID].WTPThreadMutex));
									}
								}
							}
						}

	    				WidDestroyAutoApLoginInfo(auto_ap_info2);	
	    				WidDestroyJoinRequestValuesForAutoApLogin(&joinRequest);
					}
				
					return;	
				}
				else
				{
					CWUseSockNtop(addrPtr,wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"no one pickup pkt %s%s from %s we drop\n", \
								WAI_FUNC_LINE_LITERAL, 0, (WTP_LOGIN) ? "re":"", WAI_CAPWAP_MSG_TYPE_STR(header_info.message_type), str););
				}
			}
		}
	}// unknown wtp
}

// simple job: see if we have a thread that is serving address *addressPtr
__inline__ CWWTPManager *CWWTPByAddress(CWNetworkLev4Address *addressPtr, CWSocket sock, int *WTPID)
{
	int i = 0;
	if(addressPtr == NULL)
	{
	    return NULL;
	}
	CWThreadMutexLock(&gWTPsMutex);
	for(i = 0; i < WTP_NUM; i++)
    {
		if(gWTPs[i].isNotFree
		    && &(gWTPs[i].address) != NULL
 		    && !sock_cmp_addr((struct sockaddr*)addressPtr, (struct sockaddr*)&(gWTPs[i].address), sizeof(CWNetworkLev4Address))
 		    /*&& gWTPs[i].socket == sock*/)   //we treat a WTP that sends packet to a different AC's interface as a new WTP
		{ 
			if(gWTPs[i].BAK_FLAG == 1)
			{
				wid_syslog_info("%s: change wtp %d sock %d\n",__func__,i,sock);
				gWTPs[i].socket = sock;
				gWTPs[i].BAK_FLAG = 2;
			}
			else if(gWTPs[i].socket != sock)
			{
				continue;
			}
			*WTPID = i;
			CWThreadMutexUnlock(&gWTPsMutex);
			return &(gWTPs[i]);
		}
	}
	CWThreadMutexUnlock(&gWTPsMutex);
	
	return NULL;
}

CWBool CWGetMsgQueue(int *msgqid)
{
	key_t key;

	if ((key = ftok(MSGQ_PATH, 'W')) == -1)
	{		
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"ftok %s failed we quit the process\n", WAI_FUNC_LINE_LITERAL, MSGQ_PATH);
		perror("ftok");
		exit(1);
	}
	if ((*msgqid = msgget(key, 0644)) == -1)
	{
		wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"get msq qid on %s failed we quit the process\n", WAI_FUNC_LINE_LITERAL, MSGQ_PATH);
		perror("msgget");
		exit(1);
	}
	return CW_TRUE;
}

int wid_internal_control_message(msgq *msg)
{
	unsigned int wtpid = 0;	
	struct msgqlist *elem = NULL;

	if (!msg)
	{
		return -1;
	}
	
	if (CONTROL_TYPE != msg->mqinfo.type)
	{
		return -1;
	}
	
	wtpid = msg->mqinfo.WTPID; 
	if (wtpid >= WTP_NUM)
	{
		wid_syslog_err("%s wtp %d not exist\n", __func__, wtpid);
		return -1;
	}

	if ((msg->mqinfo.subtype == WTP_S_TYPE) && (msg->mqinfo.u.WtpInfo.Wtp_Op == WTP_REBOOT))
	{					
		if ((gWTPs[wtpid].currentState != CW_QUIT) && (gWTPs[wtpid].isRequestClose))
		{
			wid_syslog_debug_debug(WID_DEFAULT, "wtp%d Request close thread", wtpid);
			_CWCloseThread(wtpid);
		}
	}
	else if ((msg->mqinfo.subtype == WTP_S_TYPE) && (msg->mqinfo.u.WtpInfo.Wtp_Op == WTP_RESEND))
	{
		if (!CWErr(CWACResendAcknowledgedPacket(wtpid)))
		{
			_CWCloseThread(wtpid);
		}
	}
	else
	{
		elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
		if (NULL == elem)
		{			
			WID_MALLOC_ERR();			
			return -1;
		}			
		memset(elem, 0, sizeof(struct msgqlist));
		elem->mqinfo.WTPID = msg->mqinfo.WTPID;
		elem->mqinfo.type = msg->mqinfo.type;
		elem->mqinfo.subtype = msg->mqinfo.subtype;
		
		if (WLAN_S_TYPE == msg->mqinfo.subtype)
		{
			memcpy((char*)&(elem->mqinfo.u.WlanInfo), (char*)&(msg->mqinfo.u.WlanInfo), sizeof(msg->mqinfo.u.WlanInfo));
		}
		else if (WTP_S_TYPE == msg->mqinfo.subtype)
		{
			memcpy((char*)&(elem->mqinfo.u.WtpInfo), (char*)&(msg->mqinfo.u.WtpInfo), sizeof(msg->mqinfo.u.WtpInfo));
		}
		else if (Radio_S_TYPE == msg->mqinfo.subtype)
		{
			memcpy((char*)&(elem->mqinfo.u.RadioInfo), (char*)&(msg->mqinfo.u.RadioInfo), sizeof(msg->mqinfo.u.RadioInfo));
		}
		else if (STA_S_TYPE == msg->mqinfo.subtype)
		{
			memcpy((char*)&(elem->mqinfo.u.StaInfo), (char*)&(msg->mqinfo.u.StaInfo), sizeof(msg->mqinfo.u.StaInfo));
		}
		else if (WDS_S_TYPE == msg->mqinfo.subtype)
		{
			memcpy((char*)&(elem->mqinfo.u.WlanInfo), (char*)&(msg->mqinfo.u.WlanInfo), sizeof(msg->mqinfo.u.WlanInfo));
		}
		
		WID_INSERT_CONTROL_LIST(wtpid, elem);
	}

	return 0;
}


int CreateWlan = 1;
// session's thread function: each thread will manage a single session with one WTP
CW_THREAD_RETURN_TYPE CWManageWTP(void *arg)
{
	int QID = 0;
	int WTPMsgqID = 0;
	int WTPFirst = 0;
	int WTPEnd = WTP_NUM/THREAD_NUM + 1;
	char indent[WID_LOG_INDENT_LEN] = {0};

	if (!arg)
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT"arg nil!\n",WAI_FUNC_LINE_LITERAL);
		return NULL;
	}
	
	QID = ((CWACThreadArg*)arg)->index;
	wid_pid_write_v2("CWManageWTP",QID,vrrid);
	CW_FREE_OBJECT(arg);
	CWGetMsgQueue(&WTPMsgqID);

	snprintf(indent, WID_LOG_INDENT_LEN-1, "mgt%d", QID);
	wid_thread_log(indent);

	CW_REPEAT_FOREVER
	{
		int wait = 1;
		int i = 0;
		int GoBack = 1;
		CWProtocolMessage msg;
		CWBool dataFlag = CW_FALSE;
		msgq WTPMsgq;
		msgqData WTPMsgqData;
		msg.msg = NULL;
		msg.offset = 0;
		struct msgqlist *elem = NULL;
		unsigned int wtpid = 0;
		// Wait WTP action		
		memset((char*)&WTPMsgq, 0, sizeof(WTPMsgq));
		memset((char*)&WTPMsgqData, 0, sizeof(WTPMsgqData));
		
		for (WTPFirst = 0; WTPFirst <= WTPEnd; WTPFirst++)
		{
			i = WTPFirst*THREAD_NUM + QID - 1;
			if ((i < WTP_NUM) 
				&& (NULL != AC_WTP[i])
				&& (WID_RUN == AC_WTP[i]->WTPStat)
				&& (NULL != AC_WTP[i]->ControlList)
				&& (NULL == AC_WTP[i]->ControlWait))
			{
				CWThreadSetSignals(SIG_BLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

				CWWTP_control_config(i);
				
				CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
			}
		}
		
		while (wait)
		{
			// TODO: Check system
			memset((char*)&WTPMsgq, 0, sizeof(WTPMsgq));
			memset((char*)&WTPMsgqData, 0, sizeof(WTPMsgqData));
			if (msgrcv(WTPMsgqID, (msgq*)&WTPMsgqData, sizeof(WTPMsgqData.mqinfo), QID, 0) == -1)
			{
				perror("msgrcv");
				//continue;
				break;
				//exit(1);
			}
			memcpy(&WTPMsgq, &WTPMsgqData, sizeof(WTPMsgq));
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv %s msg", \
									WAI_FUNC_LINE_LITERAL,WTPMsgq.mqinfo.WTPID, \
									WAI_MQTYPE_STR((WTPMsgq.mqinfo.type), (WTPMsgq.mqinfo.subtype)));
			if (WTPMsgq.mqinfo.type == DATA_TYPE)
			{
				i = WTPMsgq.mqinfo.WTPID;
				if (!check_wtpid_func(i))
				{
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv packet with id error", WAI_FUNC_LINE_LITERAL, i);
					return CW_FALSE;
				}
				
				if (AC_WTP[i] == NULL)
				{
					wid_syslog_notice(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"rcv %s msg but we known nothing about "WAI_AFI0_FMT"drop it\n", \
											WAI_FUNC_LINE_LITERAL, i, WAI_MQTYPE_STR((WTPMsgq.mqinfo.type), (WTPMsgq.mqinfo.subtype)), i);
					continue;
				}
				GoBack = 0;
				break;
			}
			else if (WTPMsgq.mqinfo.type == CONTROL_TYPE)
			{				
				wtpid = WTPMsgq.mqinfo.WTPID;
				
				wid_internal_control_message(&WTPMsgq); 
				
				CWThreadSetSignals(SIG_BLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
				
				CWWTP_control_config(wtpid);
				
				CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
			}
			else
			{
				continue;
			}
			//CWWaitThreadCondition(&gWTPs[i].interfaceWait, &gWTPs[i].interfaceMutex);
		}

		if (GoBack)
		{
			continue;
		}		
		if (is_secondary == 1)
		{
			continue;
		}
	/*	if (gWTPs[i].isRequestClose)
		{
			wid_syslog_notice("Request close thread");
			_CWCloseThread(i);
		}*/

		CWThreadSetSignals(SIG_BLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

		//if (CWGetCountElementFromSafeList(gWTPs[i].packetReceiveList) > 0)
		if (WTPMsgq.mqinfo.type == DATA_TYPE)
		{
			CWBool bCrypt = CW_FALSE;
//			char* pBuffer;

			//CWThreadMutexLock(&gWTPs[i].interfaceMutex);

			//pBuffer = (char*)CWGetHeadElementFromSafeList(gWTPs[i].packetReceiveList, NULL);
#ifdef CW_NO_DTLS
			bCrypt = CW_FALSE;
#else
			if ((pBuffer[0] & 0x0f) == CW_PACKET_CRYPT)
				bCrypt = CW_TRUE;
#endif
			//CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);

			if (bCrypt)
			{
                /*if(!CWErr(CWSecurityReceive(gWTPs[i].session, gWTPs[i].buf, CW_BUFFER_SIZE - 1, &readBytes))) 
				{
					wid_syslog_debug_debug("Error during security receive");		// error
					CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
					continue;
				}*/
			}
			else
			{
			//	CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			//	pBuffer = (char*)CWRemoveHeadElementFromSafeList(gWTPs[i].packetReceiveList, &readBytes);
			//	CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);

			//	memcpy(gWTPs[i].buf, pBuffer, readBytes);
				//memcpy(gWTPs[i].buf, WTPMsgq.mqinfo.u.DataInfo.Data, WTPMsgq.mqinfo.u.DataInfo.len);
				//readBytes = WTPMsgq.mqinfo.u.DataInfo.len;
				//memset((char*)&WTPMsgq, 0, sizeof(WTPMsgq));
			//	CW_FREE_OBJECT(pBuffer);
			}

			if (!CWProtocolParseFragment(WTPMsgqData.mqinfo.u.DataInfo.Data, WTPMsgqData.mqinfo.u.DataInfo.len, &(gWTPs[i].fragmentsList), &msg, &dataFlag)) 
			{
				if (CWErrorGetLastErrorCode() == CW_ERROR_NEED_RESOURCE) 
				{
					wid_syslog_debug_debug(WID_DEFAULT,"Need At Least One More Fragment");
				} 
				else 
				{
					CWErrorHandleLast();
				}

				CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
				memset((char*)&WTPMsgqData, 0, sizeof(WTPMsgqData));
				continue;
			}
			memset((char*)&WTPMsgqData, 0, sizeof(WTPMsgqData));

			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"current state enter %s\n", \
								WAI_FUNC_LINE_LITERAL, i, WAI_STATE_STR(gWTPs[i].currentState));
			
			switch (gWTPs[i].currentState) 
			{
				case CW_ENTER_JOIN: 	// we're inside the join state
				{					
					syslog_wtp_log(i, 1, "none", 1);
					if (!ACEnterJoin(i, &msg)) 
					{
						if (CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
						{
							// Log and ignore other messages
							CWErrorHandleLast();
							wid_syslog_warning("Received something different from a Join Request");
						} 
						else 
						{
							// critical error, close session
							wid_syslog_crit("--> wtp%d Join State Critical Error... closing thread", i);
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							_CWCloseThread(i);
						}
					}
	
					break;
				}

				case CW_ENTER_IMAGE_DATA: 	// we're inside the join state
				{
					if (!ACEnterImageData(i, &msg))
					{
						if (CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT)
						{
							// Log and ignore other messages
							CWErrorHandleLast();
							wid_syslog_warning("Received something different from a Join Request");
						} 
						else 
						{
							// critical error, close session
							wid_syslog_crit("--> wtp%d Imagedata State Critical Error... closing thread", i);
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							_CWCloseThread(i);
						}
					}
	
					break;
				}
		
				case CW_ENTER_RESET:	// we're inside the reset state
				{
					if (!ACEnterReset(i, &msg))
					{
						if (CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT)
						{
							// Log and ignore other messages
							CWErrorHandleLast();
							wid_syslog_warning("Received something different from a Reset Response");
						} 
						else 
						{
							// critical error, close session
							wid_syslog_crit("--> wtp%d Reset State Critical Error... closing thread", i);
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							_CWCloseThread(i);
						}
					}
		
					break;
				}
				
				case CW_ENTER_CONFIGURE:
				{
					if (!ACEnterConfigure(i, &msg))
					{
						if (CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT)
						{
							// Log and ignore other messages
							CWErrorHandleLast();
							wid_syslog_warning("Received something different from a Configure Request");
						} 
						else 
						{
							// critical error, close session
							wid_syslog_crit("--> wtp%d Configure State Critical Error... closing thread", i);
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							_CWCloseThread(i);
						}
					}
					break;
				}

				case CW_ENTER_DATA_CHECK:
				{
					if (!ACEnterDataCheck(i, &msg))
					{
						if (CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT)
						{
							// Log and ignore other messages
							CWErrorHandleLast();
							wid_syslog_warning("Received something different from a Change State Event Request");
						} 
						else 
						{
							// critical error, close session
							wid_syslog_crit("--> wtp%d Datacheck State Critical Error... closing thread", i);
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							_CWCloseThread(i);
						}
					}
					break;
				}	

				case CW_ENTER_RUN:
				{
					if (!ACEnterRun(i, &msg, dataFlag))
					{
						if (CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT)
						{
							// Log and ignore other messages
							CWErrorHandleLast();
							wid_syslog_warning("--> WTP %d Received something different from a valid Run Message\n",i);
						} 
						else 
						{
							// critical error, close session
							wid_syslog_crit("--> wtp%d Run State Critical Error... closing thread", i);
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							_CWCloseThread(i);
						}
					}
	
					break;
				}

				default:
				{
					wid_syslog_debug_debug(WID_DEFAULT,WAI_FUNC_LINE_FMT"state %u not handled",WAI_FUNC_LINE_LITERAL,gWTPs[i].currentState);
					break;
				}
			}

			CW_FREE_PROTOCOL_MESSAGE(msg);
		}
		else
		{
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			elem = NULL;
			elem = WID_GET_CONTROL_LIST_ELEM(i);
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
			if (elem == NULL)
			{
				continue;
			}
			if ((elem->mqinfo.subtype == WTP_S_TYPE) && (elem->mqinfo.u.WtpInfo.Wtp_Op == WTP_REBOOT))
			{					
				if ((gWTPs[i].currentState != CW_QUIT) && (gWTPs[i].isRequestClose))
				{
					wid_syslog_info("reboot wtp %d for table update\n",i);
					_CWCloseThread(i);
				}
				CW_FREE_OBJECT(elem);
				continue;
			}

			if ((AC_WTP[i]) && (AC_WTP[i]->ControlList == NULL))
			{
				if ((AC_WTP[i]) && (AC_WTP[i]->sendsysstart == 0))
				{
					if (elem->mqinfo.subtype == WTP_S_TYPE)
					{
						if ((elem->mqinfo.u.WtpInfo.Wtp_Op == WTP_EXTEND_CMD) && (memcmp(elem->mqinfo.u.WtpInfo.value,"sysstart",8) == 0))
						{
						}
						else
						{
							wid_syslog_debug_debug(WID_DEFAULT,"wlan cmd expand :sysstart (before)");
							char wlan_cmd_expand[DEFAULT_LEN] = "sysstart";
							msgq msg1;
							
							struct msgqlist *elem1 = NULL;
							memset((char*)&msg1, 0, sizeof(msg1));
							msg1.mqid = i%THREAD_NUM+1;
							msg1.mqinfo.WTPID = i;
							msg1.mqinfo.type = CONTROL_TYPE;
							msg1.mqinfo.subtype = WTP_S_TYPE;
							msg1.mqinfo.u.WtpInfo.Wtp_Op = WTP_EXTEND_CMD;
							memcpy(msg1.mqinfo.u.WtpInfo.value, wlan_cmd_expand, strlen(wlan_cmd_expand));
							
							elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
        					if (elem1 == NULL)
							{
								perror("malloc");
								return 0;
							}
							
							memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));
							elem1->next = NULL;
							memcpy((char*)&(elem1->mqinfo),(char*)&(msg1.mqinfo),sizeof(msg1.mqinfo));
							
							WID_INSERT_CONTROL_LIST(i, elem1);
							//wid_radio_set_extension_command(i,wlan_cmd_expand);
							wid_syslog_debug_debug(WID_DEFAULT,"wlan cmd expand :sysstart (after)");
							AC_WTP[i]->sendsysstart = 1;
						}
					}
					else
					{
						wid_syslog_debug_debug(WID_DEFAULT,"wlan cmd expand :sysstart (before)");
						char wlan_cmd_expand[DEFAULT_LEN] = "sysstart";
						//wid_radio_set_extension_command(i,wlan_cmd_expand);
						msgq msg1;
						struct msgqlist *elem1 = NULL;
						memset((char*)&msg1, 0, sizeof(msg1));
						
						msg1.mqid = i%THREAD_NUM+1;
						msg1.mqinfo.WTPID = i;
						msg1.mqinfo.type = CONTROL_TYPE;
						msg1.mqinfo.subtype = WTP_S_TYPE;
						msg1.mqinfo.u.WtpInfo.Wtp_Op = WTP_EXTEND_CMD;
						memcpy(msg1.mqinfo.u.WtpInfo.value, wlan_cmd_expand, strlen(wlan_cmd_expand));
						
						elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
						if (elem1 == NULL)
						{
							perror("malloc");
							return 0;
						}
						
						memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));
						elem1->next = NULL;
						memcpy((char*)&(elem1->mqinfo),(char*)&(msg1.mqinfo),sizeof(msg1.mqinfo));
						
						WID_INSERT_CONTROL_LIST(i, elem1);
						wid_syslog_debug_debug(WID_DEFAULT,"wlan cmd expand :sysstart (after)");
						AC_WTP[i]->sendsysstart = 1;
					}
				}
			}
			if (elem->mqinfo.subtype == WLAN_S_TYPE)
			{
    			wid_syslog_debug_debug(WID_DEFAULT,"***** WTP %d IEEE80211_WLAN_CONFIG_REQUEST*****\n",i);
    			
    			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
    			
    			CWBool bResult = CW_FALSE;
				int seqNum = CWGetSeqNum();
				
				if (CWBindingAssembleWlanConfigurationRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
				{
					if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_RESPONSE, seqNum))
					{
						bResult = CW_TRUE;
					}	
					else
					{
						CWACStopRetransmission(i);
					}	
				}
    			
    			gWTPs[i].interfaceCommand = NO_CMD;
				
				if (bResult)
				{
					CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
					if ((AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
					{
						wid_syslog_err("wtp%d WLAN op something wrong (subtype)%d\n", i, AC_WTP[i]->ControlWait->mqinfo.subtype);
						CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
					}
					AC_WTP[i]->ControlWait = elem;
					CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
				}
				else
				{
    				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
    				//WID_INSERT_CONTROL_LIST(i, elem);
					if ((AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
					{
						wid_syslog_debug_debug(WID_WTPINFO,"wtp%d WLAN op something wrong (subtype)%d\n",i,AC_WTP[i]->ControlWait->mqinfo.subtype);

						CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
					}
					CW_FREE_OBJECT(elem);
					wid_syslog_err("wtp%d Error sending command.(WLAN_S_TYPE)\n", i);
					CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
				}
				
				CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
			}
			else if (elem->mqinfo.subtype == Radio_S_TYPE)
    		{	
    			wid_syslog_debug_debug(WID_DEFAULT,"***** WTP %d CONFIGURE_UPDATE_RESPONSE_Radio*****\n",i);
				
    			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
    			int seqNum = CWGetSeqNum();
				CWBool bResult = CW_FALSE;
    	
				if (CWAssembleConfigurationUpdateRequest_Radio(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
				{
					if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum))
					{
						bResult = CW_TRUE;
					}
					else
					{
						CWACStopRetransmission(i);
					}
				}
    					
				if (bResult)
				{
					CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
					if ((AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
					{
						wid_syslog_err("wtp%d radio op something wrong (subtype)%d\n",i,AC_WTP[i]->ControlWait->mqinfo.subtype);
						CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
					}
					AC_WTP[i]->ControlWait = elem;
					CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
				}
				else
				{					
					CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
					//WID_INSERT_CONTROL_LIST(i, elem);
					if (AC_WTP[i] != NULL)
					{
						CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
					}
					
					CW_FREE_OBJECT(elem);
					CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
					wid_syslog_err("wtp%d Error sending command.(Radio_S_TYPE)\n", i);
				}
				
				CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
			}
			else if (elem->mqinfo.subtype == STA_S_TYPE)
	 		{
				if (elem->mqinfo.u.StaInfo.Sta_Op != Sta_SETKEY)
				{
					wid_syslog_debug_debug(WID_DEFAULT,"***** WTP %d STATION_CONFIGURATION_RESPONSE*****\n",i);
					CWThreadMutexLock(&gWTPs[i].interfaceMutex);
					int seqNum = CWGetSeqNum();
					
					CWBool bResult = CW_FALSE;
			
					if (CWAssembleStaConfigurationRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
					{
						if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE, seqNum))
						{
							bResult = CW_TRUE;
						}
						else
						{
							CWACStopRetransmission(i);
						}
					}
					
					if (bResult)
					{
						CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
						wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d sta op successful\n", __func__, __LINE__, i);
						if ((AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
						{
							wid_syslog_err("wtp%d sta op something wrong (subtype)%d\n",i,AC_WTP[i]->ControlWait->mqinfo.subtype);
							CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
						}
						AC_WTP[i]->ControlWait = elem;
						CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
					}
					else
					{					
						CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
						//WID_INSERT_CONTROL_LIST(i, elem);
						if (AC_WTP[i] != NULL)
						{
							CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
						}
						
						CW_FREE_OBJECT(elem);
						CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
						wid_syslog_err("Error sending command.(STA_S_TYPE:Sta_SETKEY)");
					}
			
					CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
				}
				else
				{
					wid_syslog_debug_debug(WID_DEFAULT,"***** WTP %d STATION_KEY_CONFIGURATION_RESPONSE*****\n",i);
					CWThreadMutexLock(&gWTPs[i].interfaceMutex);
					int seqNum = CWGetSeqNum();
					
					CWBool bResult = CW_FALSE;
			
					if (CWAssembleStaConfigurationRequest_key(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
					{
						if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE, seqNum))
						{
							bResult = CW_TRUE;
						}
						else
						{
	   						CWACStopRetransmission(i);
						}
					}
					
					if (bResult)
					{
						wid_syslog_debug_debug(WID_DEFAULT,"wtp%d sta key op successful", i);
						CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
						if ((AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
						{
							wid_syslog_debug_debug(WID_DEFAULT,"wtp%d sta op something wrong (subtype)%d\n",i,AC_WTP[i]->ControlWait->mqinfo.subtype);
							CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
						}
						AC_WTP[i]->ControlWait = elem;
						CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
					}
					else
					{					
						CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
						//WID_INSERT_CONTROL_LIST(i, elem);
						if (AC_WTP[i] != NULL)
						{
							CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
						}
						
						CW_FREE_OBJECT(elem);
						CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
						wid_syslog_err("wtp%d Error sending command.(STA_S_TYPE)\n", i);
					}
					
					CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
				}
			}
			else if (elem->mqinfo.subtype == WTP_S_TYPE)
			{
				wid_syslog_debug_debug(WID_DEFAULT,"***** WTP %d CONFIGURE_UPDATE_RESPONSE_WTP*****\n",i);
				CWThreadMutexLock(&gWTPs[i].interfaceMutex);
				int seqNum = CWGetSeqNum();
	//			int j = 0;
    			//for(j=0;	j<AC_WTP[i]->RadioCount; j++)
    			//	if (AC_WTP[i]->WTP_Radio[j]->CMD != 0x0)
    			//	{
						CWBool bResult = CW_FALSE;
		
						if (CWAssembleConfigurationUpdateRequest_WTP(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
						{
							if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum))
							{
								bResult = CW_TRUE;
							}
							else
							{
								CWACStopRetransmission(i);
							}
						}
						
						//AC_WTP[i]->CMD->setCMD = 0;
						//AC_WTP[i]->WTP_Radio[j]->CMD = 0x0;				
						//gWTPs[i].interfaceCommand = NO_CMD;
						
						if (bResult)
						{
							CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
							if ((AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
							{
								wid_syslog_debug_debug(WID_DEFAULT,"wtp%d op something wrong (subtype)%d\n",i,AC_WTP[i]->ControlWait->mqinfo.subtype);
								CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
							}
							AC_WTP[i]->ControlWait = elem;
							CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
						}
						else
						{
							CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
							//WID_INSERT_CONTROL_LIST(i, elem);
							if (NULL != AC_WTP[i])
							{
								CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
							}
							
							CW_FREE_OBJECT(elem);
							CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
							wid_syslog_err("Error sending command.(WTP_S_TYPE)");
						}
				//}
					
				CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
			}
			else if (elem->mqinfo.subtype == WDS_S_TYPE)
			{
				CWThreadMutexLock(&gWTPs[i].interfaceMutex);
				wid_set_wds_state(i,elem->mqinfo.u.WlanInfo.Radio_L_ID,elem->mqinfo.u.WlanInfo.WLANID,elem->mqinfo.u.WlanInfo.Wlan_Op);
				CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
				CW_FREE_OBJECT(elem);
			}
		}
		CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
	}
}

void _CWCloseThread(int i)
{
	unsigned int flag = 0;				/*xiaodawei add for binding flag, 20101125*/

	WID_CHECK_WTP_STANDARD(i);
	
	if ((AC_WTP[i] != NULL) && (AC_WTP[i]->isused == 1) && (AC_WTP[i]->unused_flag == 1))
	{
		AC_WTP[i]->isused = 0;
		AC_WTP[i]->unused_flag = 0;
	}
	
	CWStateTransition state = CW_QUIT;
	state = gWTPs[i].currentState;
 //	CWThreadSetSignals(SIG_BLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
#if NOSOFTAC
	AsdWsm_DataChannelOp(i, WID_DEL);
#endif	
//**** ACInterface ****
	gWTPs[i].qosValues = NULL;
	CWThreadMutexUnlock(&(gWTPs[i].interfaceMutex));
//**** ACInterface ****

	if (!CWErr(CWThreadMutexLock(&gActiveWTPsMutex)))
	{
		wid_syslog_crit("can't lock threadmutex");
		exit(1);
	}
	
	if ((gWTPs[i].interfaceIndex < gMaxInterfacesCount) && (gWTPs[i].interfaceIndex >= 0))
	{
		gInterfaces[gWTPs[i].interfaceIndex].WTPCount--;
	}
	else
	{
		wid_syslog_err("%s:%d gWTPs[%d].interfaceIndex = %d, is not legal\n", __func__, __LINE__, i, gWTPs[i].interfaceIndex);
	}
	
	gActiveWTPs--;
	CWThreadMutexLock(&ACLicense);
	if (g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count > 0)
	{
		(g_wtp_count[gWTPs[i].oemoption]->gcurrent_wtp_count)--;
		if (g_wtp_count[gWTPs[i].oemoption]->flag != 0)
		{
			flag = g_wtp_count[gWTPs[i].oemoption]->flag;
			if (flag < glicensecount)
			{
				if (g_wtp_binding_count[flag] != NULL)
				{
					if (g_wtp_binding_count[flag]->gcurrent_wtp_count > 0)
					{
						(g_wtp_binding_count[flag]->gcurrent_wtp_count)--;
					}
					wid_syslog_debug_debug(WID_DEFAULT,"%s,flag=%u,g_wtp_binding_count->gcurrent_wtp_count=%u",__func__,flag,g_wtp_binding_count[flag]->gcurrent_wtp_count);
				}
			}
		}
	}
	CWThreadMutexUnlock(&ACLicense);
	
	/*if (g_wtp_count[gWTPs[i].oemoption]->flag != 0)
	{
		(g_wtp_binding_count[(g_wtp_count[gWTPs[i].oemoption]->flag)]->gcurrent_wtp_count)--;
	}
	*/
	//gWTPs[i].oemoption = 0;
	if (gloadbanlance >= 1)
	{
		wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d quit send active wtp count is %d\n", __func__, __LINE__, i, gActiveWTPs);
		CWThreadMutexLock(&ACIPLISTMutex);
		SendActiveWTPCount(gActiveWTPs);
		CWThreadMutexUnlock(&ACIPLISTMutex);
	}
	
	if ((gWTPs[i].interfaceIndex < gMaxInterfacesCount) && (gWTPs[i].interfaceIndex >= 0))
	{
		CWUseSockNtop(((struct sockaddr*) &(gInterfaces[gWTPs[i].interfaceIndex].addr)),
			wid_syslog_debug_debug(WID_DEFAULT,"Remove wtp%d on Interface %s (%d)\n", i, str, gWTPs[i].interfaceIndex);
		);
	}
	else
	{
		wid_syslog_err("gWTPs[%d].interfaceIndex = %d, is not legal\n",i,gWTPs[i].interfaceIndex);
	}
	
	CWThreadMutexUnlock(&gActiveWTPsMutex);
	
	wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d Close Thread: %08x", __func__, __LINE__, i, (unsigned int)CWThreadSelf());
	
	if (gWTPs[i].subState != CW_DTLS_HANDSHAKE_IN_PROGRESS)
	{
		//CWSecurityDestroySession(gWTPs[i].session);
	}
	
	CWTimerCancel(&(gWTPs[i].currentTimer), 0); // this will do nothing if the timer isn't active
	CWTimerCancel(&(gWTPs[i].updateTimer), 0);

	if (find_in_wtp_list(i) == CW_TRUE)
	{
		//printf("_CWCloseThread ap mangement\n");
		if ((AC_WTP[i]) && (AC_WTP[i]->isused))
		{
			if (!CWErr(CWTimerRequest(CW_REACCESS_INTERVAL_DEFAULT, &(gWTPs[i].thread), &(gWTPs[i].updateTimer), CW_CRITICAL_TIMER_EXPIRED_UPDATE,i)))
			{
				//printf("error _CWCloseThread ap mangement\n");
				//_CWCloseThread(WTPIndex);	
			}
		}
		else
		{
			delete_wtp_list(i);
		}
		// start Change State Pending timer
	}
	
	CWThreadMutexLock(&gWTPs[i].interfaceMutex);
	CWACStopRetransmission(i);

	if (gWTPs[i].interfaceCommandProgress == CW_TRUE)
	{
		gWTPs[i].currentState = CW_QUIT;
		gWTPs[i].interfaceResult = 1;
		gWTPs[i].interfaceCommandProgress = CW_FALSE;
//		CWSignalThreadCondition(&gWTPs[i].interfaceComplete);
	}
	CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
	//gWTPs[i].session = NULL;
	gWTPs[i].subState = CW_DTLS_HANDSHAKE_IN_PROGRESS;
	//CWDeleteList(&(gWTPs[i].fragmentsList), CWProtocolDestroyFragment);
	
	//CW_FREE_OBJECT(gWTPs[i].configureReqValuesPtr);
	
	//CWCleanSafeList(gWTPs[i].packetReceiveList, free);
	//CWDestroySafeList(gWTPs[i].packetReceiveList);
	CWThreadMutexLock(&(gWTPs[i].WTPThreadMutex));	
	wid_syslog_debug_debug(WID_DEFAULT, "%s:%d down wtp%d\n", __func__, __LINE__, i);
	CWDownWTP(i);	
	wid_wtp_param_init(i, 0);
	CWThreadMutexUnlock(&(gWTPs[i].WTPThreadMutex));
	
	CWThreadMutexLock(&gWTPsMutex);
	gWTPs[i].isNotFree = CW_FALSE;	
	//gWTPs[i].oemoption = 0;
	gWTPs[i].currentState = CW_QUIT;
	CWThreadMutexUnlock(&gWTPsMutex);	
//	CWDownWTP(i);
	
	if (WID_WATCH_DOG_OPEN == 1)
	{
		if ((gWTPs[i].interfaceIndex < gMaxInterfacesCount) && (gWTPs[i].interfaceIndex >= 0))	
		{
			if (gInterfaces[gWTPs[i].interfaceIndex].WTPCount == 0)
			{
				gInterfaces[gWTPs[i].interfaceIndex].tcpdumpflag = 1;
				gInterfaces[gWTPs[i].interfaceIndex].datacount = 0;
				gInterfaces[gWTPs[i].interfaceIndex].times = 0;
				memset(gInterfaces[gWTPs[i].interfaceIndex].ip,0,128);
				sock_ntop_r1(((struct sockaddr*)&(gInterfaces[gWTPs[i].interfaceIndex].addr)), gInterfaces[gWTPs[i].interfaceIndex].ip);
				memset(gInterfaces[gWTPs[i].interfaceIndex].ifname,0,16);
				memcpy(gInterfaces[gWTPs[i].interfaceIndex].ifname,AC_WTP[i]->BindingIFName,ETH_IF_NAME_LEN);
			}
		}
		else
		{
			wid_syslog_err("gWTPs[%d].interfaceIndex =  %d  ,is not legal \n",i,gWTPs[i].interfaceIndex);
		}
	}
	
	/*if(gInterfaces[gWTPs[i].interfaceIndex].WTPCount == 0){
		Modify_WLAN_WTP_SETTING(i);		
		gInterfacesCount = CWNetworkCountInterfaceAddresses(&gACSocket);
		//printf("gInterfacesCount %d\n",gInterfacesCount);
	}*/

	if (state != CW_ENTER_RUN)
	{
		if (g_auto_ap_login.save_switch != 1)
		{
			if ((AC_WTP[i]) && (AC_WTP[i]->wtp_login_mode == 1))
			{
				AC_WTP[i]->isused = 0;
				//printf("ap is auto login,delete\n");
				CWThreadMutexLock(&(gWTPs[i].WTPThreadMutex));
				WID_DELETE_WTP(i);
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadMutex));
			}
		}
	}
//	CWExitThread();
}

void CWCloseThread()
{
	int *iPtr = NULL;
	if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL)
	{
		wid_syslog_crit("Error Closing Thread");
		return;
	}
	
	_CWCloseThread(*iPtr);
}

void CWCriticalTimerExpiredHandler(int arg)
{
	int *iPtr = NULL;
	msgq msg;
	CWThreadSetSignals(SIG_BLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
 	
 	wid_syslog_debug_debug(WID_DEFAULT,"Critical Timer Expired for Thread: %08x", (unsigned int)CWThreadSelf());
	wid_syslog_debug_debug(WID_DEFAULT,"Abort Session");
	//CWCloseThread();

	if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL) 
	{
		wid_syslog_debug_debug(WID_DEFAULT,"Error Handling Critical timer");
		CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL, CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
		return;
	}

	// Request close thread
	gWTPs[*iPtr].isRequestClose = CW_TRUE;	
	syslog_wtp_log(*iPtr, 0, "Critical Timer Expired", 0);
	memset((char*)&msg, 0, sizeof(msg));
	msg.mqid = (*iPtr)%THREAD_NUM+1;
	msg.mqinfo.WTPID = *iPtr;
	msg.mqinfo.type = CONTROL_TYPE;
	msg.mqinfo.subtype = WTP_S_TYPE;
	msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
	msg.mqinfo.u.WtpInfo.WTPID = *iPtr;
	if (msgsnd(ACDBUS_MSGQ, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
		perror("msgsnd");
	
//	CWDownWTP(*iPtr);
//	CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
}

void CWSoftTimerExpiredHandler(int arg)
{
    int ret = 0;
	int *iPtr = NULL;
	CWThreadSetSignals(SIG_BLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL,CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

	wid_syslog_debug_debug(WID_DEFAULT,"Soft Timer Expired for Thread: %08x", (unsigned int)CWThreadSelf());
	
	if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL)
	{
		wid_syslog_debug_debug(WID_DEFAULT,"Error Handling Soft timer");
		CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL,CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
		return;
	}
	
	if((!gWTPs[*iPtr].isRetransmitting) || (gWTPs[*iPtr].messages == NULL))
	{
		wid_syslog_debug_debug(WID_DEFAULT,"Soft timer expired but we are not retransmitting");
		CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL,CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
		CWThreadMutexLock(&(gWTPs[*iPtr].WTPThreadControllistMutex));
		if((AC_WTP[*iPtr] != NULL)&&(AC_WTP[*iPtr]->ControlWait != NULL))
		{
			CW_FREE_OBJECT(AC_WTP[*iPtr]->ControlWait);
		}
		CWThreadMutexUnlock(&(gWTPs[*iPtr].WTPThreadControllistMutex));
		return;
	}

	(gWTPs[*iPtr].retransmissionCount)++;
	
	wid_syslog_debug_debug(WID_DEFAULT,"wtp%d Retransmission Count increases to %d", *iPtr, gWTPs[*iPtr].retransmissionCount);
	
	if(gWTPs[*iPtr].retransmissionCount >= gCWMaxRetransmit) 
	{
	    /*
		wid_syslog_debug_debug(WID_DEFAULT,"Peer is Dead");
		//?? _CWCloseThread(*iPtr);
		// Request close thread
		gWTPs[*iPtr].isRequestClose = CW_TRUE;
//		CWDownWTP(*iPtr);
		syslog_wtp_log(*iPtr, 0, "Soft Timer Expired", 0);		
        msgq msg;
		memset((char*)&msg, 0, sizeof(msg));
		msg.mqid = (*iPtr)%THREAD_NUM+1;
		msg.mqinfo.WTPID = *iPtr;
		msg.mqinfo.type = CONTROL_TYPE;
		msg.mqinfo.subtype = WTP_S_TYPE;
		msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
		msg.mqinfo.u.WtpInfo.WTPID = *iPtr;
		if (msgsnd(ACDBUS_MSGQ, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
			perror("msgsnd");
//		CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
        */
    	CWACStopRetransmission(*iPtr);
		if(AC_WTP[*iPtr] != NULL)
		{
            AC_WTP[*iPtr]->WTPStat = WID_OFFLINE;
            ret = active_ap_offlined(AC_WTP[*iPtr]->WTPMAC);
            if(ret >= 0)
            {
                wtp_add_new_alert(AC_WTP[*iPtr]->WTPMAC, ret, CW_FALSE);
            } 
		}
		return;
	}

	if(!CWErr(CWACResendAcknowledgedPacket(*iPtr)))
	{
		_CWCloseThread(*iPtr);
	}
	
	CWThreadSetSignals(SIG_UNBLOCK, 2, CW_SOFT_TIMER_EXPIRED_SIGNAL,CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
}

void CWResetWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager)
{
	int i = 0;
	CW_FREE_OBJECT(WTPProtocolManager->locationData);
	CW_FREE_OBJECT(WTPProtocolManager->name);
	WTPProtocolManager->sessionID = 0;
	WTPProtocolManager->descriptor.maxRadios = 0;
	WTPProtocolManager->descriptor.radiosInUse = 0;
	WTPProtocolManager->descriptor.encCapabilities = 0;
	
	for(i = 0; i < (WTPProtocolManager->descriptor.vendorInfos).vendorInfosCount; i++)
	{
		if((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_HARDWARE_VERSION)
		{
			CW_FREE_OBJECT(((WTPProtocolManager->descriptor.vendorInfos).vendorInfos)[i].sysver);
		}
		else if((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
		{
			CW_FREE_OBJECT(((WTPProtocolManager->descriptor.vendorInfos).vendorInfos)[i].ver);
		}
		else if((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].type == CW_BOOT_VERSION)
		{
			CW_FREE_OBJECT(WTPProtocolManager->descriptor.vendorInfos.vendorInfos[i].bootver);
		}
		else
		{
			CW_FREE_OBJECT(((WTPProtocolManager->descriptor.vendorInfos).vendorInfos)[i].valuePtr);
		}
	}
	WTPProtocolManager->descriptor.vendorInfos.vendorInfosCount = 0;
	CW_FREE_OBJECT(WTPProtocolManager->descriptor.vendorInfos.vendorInfos);
	
	WTPProtocolManager->radiosInfo.radioCount = 0;
	CW_FREE_OBJECT(WTPProtocolManager->radiosInfo.radiosInfo);
	CW_FREE_OBJECT(WTPProtocolManager->ACName);
	(WTPProtocolManager->ACNameIndex).count = 0;
	CW_FREE_OBJECT((WTPProtocolManager->ACNameIndex).ACNameIndex);
	(WTPProtocolManager->radioAdminInfo).radiosCount = 0;
	CW_FREE_OBJECT((WTPProtocolManager->radioAdminInfo).radios);
	WTPProtocolManager->StatisticsTimer = 0;
	
	for(i = 0; i < WTPProtocolManager->WTPBoardData.vendorInfosCount; i++)
	{
		if((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].model);
		}
		else if((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].SN);
		}
		else if((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].mac);
		}
		else if((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].Rmodel);
		}
		else
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].valuePtr);
		}
	}
	(WTPProtocolManager->WTPBoardData).vendorInfosCount = 0;
	CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData).vendorInfos);
	CW_FREE_OBJECT(WTPProtocolManager->WTPRebootStatistics);

	//CWWTPResetRebootStatistics(&(WTPProtocolManager->WTPRebootStatistics));

	/*
		**AUTELEN.SOFTWARE.WID:**
		CWNetworkLev4Address address;
		int pathMTU;
		struct sockaddr_in ipv4Address;
		CWProtocolConfigureRequestValues *configureReqValuesPtr;
		CWTimerID currentPacketTimer;
	*/
}

void syslog_wid_wtp_log(char *format,...)
{
	char *ident = "WTP_LOG";
	char buf[2048]; 
	int level = LOG_INFO;
	memset(buf,0,2048);
	openlog(ident, 0, LOG_DAEMON);    
	va_list ptr;
	va_start(ptr, format);
	vsprintf(buf,format,ptr);
	va_end(ptr);
	syslog(level,"%s", buf);
	closelog();
}

//use the wtplog
void syslog_wtp_log(int WTPIndex, int login, char *note, unsigned char flag)//xiaodawei add flag, 0--ap quit, 1--ap run, 20110121
{
	if (AC_WTP[WTPIndex] == NULL)
	{
		return;
	}
	
	if (login)
	{
		wid_syslog_notice_local7(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"login ip %s mac "WAI_MAC_FMT"[note:%s]\n", \
						WAI_FUNC_LINE_LITERAL,WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
						AC_WTP[WTPIndex]->WTPIP, WAI_MAC_LITERAL(AC_WTP[WTPIndex]->WTPMAC),note);
	}
	else
	{
		wid_syslog_notice_local7(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"logout ip %s mac "WAI_MAC_FMT"[note:%s]\n", \
						WAI_FUNC_LINE_LITERAL,WTPIndex,AC_WTP[WTPIndex]->WTPNAME, \
						AC_WTP[WTPIndex]->WTPIP, WAI_MAC_LITERAL(AC_WTP[WTPIndex]->WTPMAC),note);
	}
}


void wid_wtp_param_init(int WTPIndex, unsigned char flag)//xiaodawei add flag, 0--ap quit, 1--ap run, 20110121
{
	time_t now;
	int i = 0;
	char * IGNORE_UBSV nowReadable = NULL;
	now = time(NULL);
	//int len;
	nowReadable = ctime(&now);
	//len = strlen(nowReadable);
	//nowReadable[len-1] = '\0';
	//printf("%s",nowReadable);
	if(AC_WTP[WTPIndex] == NULL)
		return;
	//process some switch
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->mib_info.dos_def_switch = 0;
			AC_WTP[WTPIndex]->mib_info.igmp_snoop_switch = 0;
		}
		for(i=0;i<L_BSS_NUM;i++)
		{
			if((AC_WTP[WTPIndex])&&(AC_WTP[WTPIndex]->mib_info.wlan_l2isolation[i].wlanid != 0))
			{
				AC_WTP[WTPIndex]->mib_info.wlan_l2isolation[i].wlanid = 0;
				AC_WTP[WTPIndex]->mib_info.wlan_l2isolation[i].l2_isolation_switch = 0;
			}
		}

		//AC_WTP[WTPIndex]->wifi_extension_info.reportswitch = g_AC_ALL_EXTENTION_INFORMATION_SWITCH;
	}
	if((AC_WTP[WTPIndex])&&(AC_WTP[WTPIndex]->add_time != NULL))
	{
		*(AC_WTP[WTPIndex]->add_time) = 0;
	}
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->wifi_extension_info.cpu_trap_flag = 0;
			AC_WTP[WTPIndex]->wifi_extension_info.mem_trap_flag = 0;
			AC_WTP[WTPIndex]->wifi_extension_info.temp_trap_flag = 0;
			//memset(AC_WTP[WTPIndex]->wifi_extension_info.wifi_trap_flag,0,AP_WIFI_IF_NUM);  fengwenchao  modify 20110302 此处初始化会导致wifi故障清除告警无法发送
		}
	}
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->apcminfo.cpu_average = 0;
			AC_WTP[WTPIndex]->apcminfo.cpu_peak_value = 0;
			AC_WTP[WTPIndex]->apcminfo.cpu_times = 0;
			AC_WTP[WTPIndex]->apcminfo.mem_average = 0;
			AC_WTP[WTPIndex]->apcminfo.mem_peak_value = 0;
			AC_WTP[WTPIndex]->apcminfo.mem_times = 0;
			memset(AC_WTP[WTPIndex]->apcminfo.cpu_value,0,10);
			memset(AC_WTP[WTPIndex]->apcminfo.mem_value,0,10);
		}
	}
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->wtp_wifi_snr_stats.snr_average= 0;
			AC_WTP[WTPIndex]->wtp_wifi_snr_stats.snr_max= 0;
			AC_WTP[WTPIndex]->wtp_wifi_snr_stats.snr_min= 100;
			memset(AC_WTP[WTPIndex]->wtp_wifi_snr_stats.snr,0,10);
		}
	}
	/*fengwenchao add 20111118 for GM-3*/
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->heart_time.heart_lose_pkt = 0;
			AC_WTP[WTPIndex]->heart_time.heart_time_avarge = 0;
			AC_WTP[WTPIndex]->heart_time.heart_time_delay = 0;
			AC_WTP[WTPIndex]->heart_time.heart_transfer_pkt = 0;
		}
	}
	/*fengwenchao add end*/
	if(1 == flag)
	{
		if(AC_WTP[WTPIndex])
		{
			//AC_WTP[WTPIndex]->apifinfo.report_switch = 0;
			AC_WTP[WTPIndex]->apifinfo.eth_num = 1;
			AC_WTP[WTPIndex]->apifinfo.wifi_num = 1;
			time_t t;
			time(&t);
			//for(i=0;i<2;i++)
			//{
				AC_WTP[WTPIndex]->apifinfo.eth[0].type = 0;
				AC_WTP[WTPIndex]->apifinfo.eth[0].ifindex = 0;
				AC_WTP[WTPIndex]->apifinfo.eth[0].state = 1;
				AC_WTP[WTPIndex]->apifinfo.eth[0].state_time = t;
				AC_WTP[WTPIndex]->apifinfo.wifi[0].type = 0;
				AC_WTP[WTPIndex]->apifinfo.wifi[0].ifindex = 0;
				AC_WTP[WTPIndex]->apifinfo.wifi[0].state = 1;
				AC_WTP[WTPIndex]->apifinfo.wifi[0].state_time = t;
		//	}
		}
	}
	else
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->apifinfo.eth_num = 1;
			AC_WTP[WTPIndex]->apifinfo.wifi_num = 1;
			time_t t;
			time(&t);
			AC_WTP[WTPIndex]->apifinfo.eth[0].type = 0;
			AC_WTP[WTPIndex]->apifinfo.eth[0].ifindex = 0;
			AC_WTP[WTPIndex]->apifinfo.eth[0].state = 2;
			AC_WTP[WTPIndex]->apifinfo.eth[0].state_time = t;
			AC_WTP[WTPIndex]->apifinfo.wifi[0].type = 0;
			AC_WTP[WTPIndex]->apifinfo.wifi[0].ifindex = 0;
			AC_WTP[WTPIndex]->apifinfo.wifi[0].state = 2;
			AC_WTP[WTPIndex]->apifinfo.wifi[0].state_time = t;
		}
	}
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->ElectrifyRegisterCircle = 0;
		}
	}
	{
		if(AC_WTP[WTPIndex])
		{
			AC_WTP[WTPIndex]->wid_sample_throughput.current_downlink_throughput = 0;
			AC_WTP[WTPIndex]->wid_sample_throughput.current_uplink_throughput = 0;
			AC_WTP[WTPIndex]->wid_sample_throughput.past_downlink_throughput = 0;
			AC_WTP[WTPIndex]->wid_sample_throughput.past_uplink_throughput = 0;
			AC_WTP[WTPIndex]->wid_sample_throughput.uplink_rate = 0;
			AC_WTP[WTPIndex]->wid_sample_throughput.downlink_rate = 0;
		}
	}
}

void CWDownWTP(unsigned int WTPIndex)
{
	int i = 0, j = 0;
	char buf[DEFAULT_LEN] = {0};
	char apcmd[WID_SYSTEM_CMD_LENTH] = {0};
	char out[] = "timeout";
	char normal[] = "normal";
	char *command = NULL;
	char *ath_str = NULL;
	struct wds_bssid *wds = NULL;
	unsigned int bssindex = 0;
	unsigned int groupid = 0;
	unsigned char policy = 0;
	unsigned char IGNORE_UBSV aclgroupid = 0;
	int apbind[WLAN_NUM] = {0};
	msgq msg, msg2, msg4;
	struct msgqlist *elem = NULL;
	struct msgqlist *elem2 = NULL;
	struct msgqlist *elem4 = NULL;
	memset(&msg, 0, sizeof(msgq));
	memset(&msg2, 0, sizeof(msgq));
	memset(&msg4, 0, sizeof(msgq));
	
	WID_CHECK_WTP_STANDARD(WTPIndex);
	
	if (AC_WTP[WTPIndex] != NULL)
	{
		if (AC_WTP[WTPIndex]->sendsysstart != 2)
		{
			AC_WTP[WTPIndex]->sendsysstart = 0;
		}
		AC_WTP[WTPIndex]->WTPStat = WID_QUIT;
		AC_WTP[WTPIndex]->CTR_ID = 0;
		AC_WTP[WTPIndex]->DAT_ID = 0;
		AC_WTP[WTPIndex]->CMD->keyCMD = 0;
		AC_WTP[WTPIndex]->CMD->staCMD = 0;
		AC_WTP[WTPIndex]->CMD->CMD = 0;
		AC_WTP[WTPIndex]->neighbordeatimes = 0;
		AC_WTP[WTPIndex]->sysver = NULL;
		AC_WTP[WTPIndex]->ver = NULL;
		CW_FREE_OBJECT(AC_WTP[WTPIndex]->codever);
		//memset(AC_WTP[WTPIndex]->WTPIP, 0, 128);
		//memset(AC_WTP[WTPIndex]->WTPMAC, 0, 6);
		//release memory
		/*initialization for apstatsinfo when wtp quit*/
		
		wid_syslog_debug(WAI_FUNC_LINE_FMT"free wtp%d codever\n", WAI_FUNC_LINE_LITERAL, WTPIndex);
		wid_apstatsinfo_init(WTPIndex);
		
		wtp_upgrade_reinit(&(AC_WTP[WTPIndex]->upgrade));
	}
	
	CWThreadMutexLock(&(gWTPs[WTPIndex].RRMThreadMutex));
	for (i = 0; ((AC_WTP[WTPIndex]) && (i < AC_WTP[WTPIndex]->RadioCount) && (i < L_RADIO_NUM)); i++)
	{
		if ((AC_WTP[WTPIndex]) && ((AC_WTP[WTPIndex]->NeighborAPInfos[i]) != NULL))
		{
			destroy_ap_info_list(&(AC_WTP[WTPIndex]->NeighborAPInfos[i]));
		}
		if ((AC_WTP[WTPIndex]) && ((AC_WTP[WTPIndex]->rouge_ap_infos[i]) != NULL))
		{
			destroy_ap_info_list(&(AC_WTP[WTPIndex]->rouge_ap_infos[i]));
		}
	}
	CWThreadMutexUnlock(&(gWTPs[WTPIndex].RRMThreadMutex));
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT"free wtp%d neighbor ap and rouge ap info\n", WAI_FUNC_LINE_LITERAL, WTPIndex);
	//make sure quit state and reason
	if ((AC_WTP[WTPIndex]) && (AC_WTP[WTPIndex]->isused == 1))
	{
		WTPQUITREASON quitreason = WTP_INIT;
		Check_Interface_Config(AC_WTP[WTPIndex]->BindingIFName,&quitreason);
		if (quitreason == WTP_INIT)
		{
			AC_WTP[WTPIndex]->quitreason = WTP_TIMEOUT;
		}
		else
		{
			AC_WTP[WTPIndex]->quitreason = quitreason;
		}
	}
	
	if ((AC_WTP[WTPIndex]) && (AC_WTP[WTPIndex]->quitreason == WTP_TIMEOUT))
	{
		if (gtrapflag >= 4)
		{
#if NOSOFTAC
			wid_dbus_trap_wtp_divorce_networwok(WTPIndex);
#endif
		}
	}
	//AC_WTP[WTPIndex]->channelsendtimes = 1;
	for (j = 0; ((AC_WTP[WTPIndex]) && (j < AC_WTP[WTPIndex]->RadioCount) && (j < L_RADIO_NUM)); j++)
	{
		if (AC_WTP[WTPIndex]->WTP_Radio[j] != NULL)
		{
			if (AC_WTP[WTPIndex]->WTP_Radio[j]->auto_channel_cont == 0)
			{
				AC_WTP[WTPIndex]->WTP_Radio[j]->channelsendtimes = 1;	
			}
			//AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_TXP = 0	;
			//AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_Chan = 0;
			AC_WTP[WTPIndex]->WTP_Radio[j]->channelchangetime = 0;
			AC_WTP[WTPIndex]->WTP_Radio[j]->CMD = 0x0;
			AC_WTP[WTPIndex]->WTP_Radio[j]->AdStat = 2;
			AC_WTP[WTPIndex]->WTP_Radio[j]->OpStat = 2;
			AC_WTP[WTPIndex]->WTP_Radio[j]->wifi_state = 1;
		}
	}
	
	//ap quit time
	if (AC_WTP[WTPIndex]->quit_time == NULL)
	{
		AC_WTP[WTPIndex]->quit_time = (time_t *)malloc(sizeof(time_t));
		time(AC_WTP[WTPIndex]->quit_time);
	}
	else
	{
		time(AC_WTP[WTPIndex]->quit_time);
	}
	unsigned char pcy = 0;
	time_t now;
	time(&now);
	syslog(LOG_INFO|LOG_LOCAL7, "WTP %d down, WTP MAC:"MACSTR", WTP IP:%s Down Time:%s\n",
                                WTPIndex,MAC2STR(AC_WTP[WTPIndex]->WTPMAC),AC_WTP[WTPIndex]->WTPIP,ctime(&now));
	
	unsigned int rand_num = rand() * now % 100;	
	if (gtrapflag >= 1 && AC_WTP[WTPIndex]->wid_trap.ignore_percent <= rand_num)
	{
#if NOSOFTAC
    	wid_dbus_trap_ap_run_quit(WTPIndex,state);
#endif
    }
	else
	{
	    AC_WTP[WTPIndex]->wid_trap.ignore_switch = 1;
	}
	wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" afi%d quit reason: %s\n", WAI_FUNC_LINE_LITERAL, WTPIndex, (AC_WTP[WTPIndex]->quitreason == 7)?out:normal);
	//end
	CWThreadMutexLock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
	WID_CLEAN_CONTROL_LIST(WTPIndex);
	CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
	
	AC_WTP[WTPIndex]->CMD->wlanCMD = 0;
	
	wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" afi%d index is %d\n",WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->BindingSystemIndex);
	
	for (i = 0; (i < L_RADIO_NUM) && ((AC_WTP[WTPIndex]) && (i < AC_WTP[WTPIndex]->RadioCount)); i++)
	{
		int k = 0, m = 0;
		m = i;
		for (k = 0; k < WLAN_NUM; k++)
		{
			//for(m = 0;(AC_WTP[WTPIndex])&&(m < AC_WTP[WTPIndex]->RadioCount); m++)
			{	
			AC_WTP[WTPIndex]->CMD->radiowlanid[m][k] = 0;
			if ((AC_WLAN[k] != NULL) && (AC_WLAN[k]->Status == 0))
			{	
				if (AC_WLAN[k]->Wlan_Ifi != NULL)
				{
					struct ifi *wlan_ifi = AC_WLAN[k]->Wlan_Ifi;
					while (wlan_ifi != NULL)
					{
						wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT " wlan%d index is %d\n", WAI_FUNC_LINE_LITERAL, k, wlan_ifi->ifi_index);
						if ((AC_WTP[WTPIndex]) && (AC_WTP[WTPIndex]->BindingSystemIndex == wlan_ifi->ifi_index))
						{
							break;
						}
						wlan_ifi = wlan_ifi->ifi_next;
					}
					
					if (wlan_ifi == NULL)
					{
						wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" wtp%d binding interface doesn't match with wlan%d\n",WAI_FUNC_LINE_LITERAL, WTPIndex, k);
						continue;
					}
					else
					{
						int ret = 0;
						ret = CONFIG_WLAN_CHECK_APPLY_WTP(WTPIndex, k);
						if (ret == 0)
						{
							wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" wtp%d binding wlan%d match success\n",WAI_FUNC_LINE_LITERAL, WTPIndex, k);
							apbind[k] = 1;
							issued_wlan_enable(WTPIndex, 0, k, LEVEL_AP);
						}
						if ((AC_WTP[WTPIndex])
							&& (AC_WTP[WTPIndex]->WTP_Radio[m])
							&& (AC_WTP[WTPIndex]->WTP_Radio[m]->isBinddingWlan == 1))
						{
							struct wlanid *wlan_id = AC_WTP[WTPIndex]->WTP_Radio[m]->Wlan_Id;
							while (wlan_id != NULL)
							{
								if (wlan_id->wlanid == k)
								{
									break;
								}
								wlan_id = wlan_id->next;
							}
							if ((wlan_id != NULL) && (AC_WTP[WTPIndex]) && (AC_WTP[WTPIndex]->isused == 1))
							{
								wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" wtp%d radio%d binding wlan%d match success\n", WAI_FUNC_LINE_LITERAL, WTPIndex, m, k);
								if (0 == apbind[k])
								{
									issued_wlan_enable(WTPIndex, m, k, LEVEL_RADIO);
								}
								if ((AC_WLAN[wlan_id->wlanid] != NULL) && (AC_WLAN[wlan_id->wlanid]->balance_switch == 1))
								{
									char *command = NULL;
									command = (char *)malloc(sizeof(char)*50);
									memset(command, 0, 50);
									strncpy(command,"echo 1 > /proc/sys/dev/wifi0/traffic_balance",44);
									set_balance_probe_extension_command(WTPIndex,command);
									CW_FREE_OBJECT(command);
								}
								//wds state 
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
								    && (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->WDSStat == WDS_ANY))
								{
									msg2.mqid = WTPIndex%THREAD_NUM +1;
									msg2.mqinfo.WTPID = WTPIndex;
									msg2.mqinfo.type = CONTROL_TYPE;
									msg2.mqinfo.subtype = WDS_S_TYPE;
									msg2.mqinfo.u.WlanInfo.Wlan_Op = WLAN_WDS_ENABLE;
									msg2.mqinfo.u.WlanInfo.WLANID = k;
									msg2.mqinfo.u.WlanInfo.Radio_L_ID = m;
									
									elem2 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
									if (elem2 == NULL)
									{
										WID_MALLOC_ERR();			
										return;
									}
									memset((char*)&(elem2->mqinfo), 0, sizeof(msgqdetail));
									elem2->next = NULL;
									memcpy((char*)&(elem2->mqinfo),(char*)&(msg2.mqinfo),sizeof(msg2.mqinfo));
									WID_INSERT_CONTROL_LIST(WTPIndex, elem2);
								}	
								//wds state end
								else
								{
									bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m];
									if ((check_bssid_func(bssindex))
										&& (AC_BSS[bssindex]!=NULL) 
										&& (AC_BSS[bssindex]->WDSStat == WDS_SOME))
									{
										wds = AC_BSS[bssindex]->wds_bss_list;
										while (wds != NULL)
										{
											memset(buf, 0, DEFAULT_LEN);
											sprintf(buf,"/usr/sbin/wds_add ath.%d-%d %02X:%02X:%02X:%02X:%02X:%02X\n",
														AC_BSS[bssindex]->Radio_L_ID, k, MAC2STR(wds->BSSID));
											wid_radio_set_extension_command(WTPIndex,buf);
											wds = wds->next;
										}
									}
								}
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
									&& (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->ath_l2_isolation == 1))
								{
									bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m];
									memset(buf, 0, DEFAULT_LEN);
									sprintf(buf, "iwpriv ath.%d-%d ap_bridge 0\n", AC_BSS[bssindex]->Radio_L_ID, AC_BSS[bssindex]->WlanID);
									wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"iwpriv ath.%d-%d ap_bridge 0\n",
																		WAI_FUNC_LINE_LITERAL, WTPIndex, AC_BSS[bssindex]->Radio_L_ID, AC_BSS[bssindex]->WlanID);
									wid_radio_set_extension_command(WTPIndex,buf);
								}
								/*traffic limit*/
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
								    && (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->traffic_limit_able == 1))
								{
//									unsigned int value = 0;
									bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m];
									if (!check_bssid_func(bssindex))
									{
										wid_syslog_err("<error> %s\n",__func__);
										//return ;
									}
									else
									{
										WID_Save_Traffic_Limit(bssindex, WTPIndex);
										if ((AC_BSS[bssindex]) && (AC_BSS[bssindex]->multi_user_optimize_switch == 1))
										{
											char wlanid =AC_BSS[bssindex]->WlanID;
											int radioid = AC_BSS[bssindex]->Radio_G_ID;
											muti_user_optimize_switch(wlanid,radioid,1);
											
										}
									#if 0
										if(AC_BSS[bssindex]->traffic_limit != 0)
										{
											value = AC_BSS[bssindex]->traffic_limit;
											memset(buf,0,DEFAULT_LEN);
											sprintf(buf,"autelan traffic_limit ath.%d-%d set_vap_flag 1;autelan traffic_limit ath.%d-%d set_vap %d",AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,value);
											wid_syslog_debug_debug(WID_DEFAULT,"wid_set_bss_traffic_limit_value buf %s\n",buf);
											wid_radio_set_extension_command(WTPIndex,buf);
										}
					
										if(AC_BSS[bssindex]->average_rate != 0)
										{
											value = AC_BSS[bssindex]->average_rate;
											memset(buf,0,DEFAULT_LEN);
											sprintf(buf,"autelan traffic_limit ath.%d-%d set_every_node_flag 1;autelan traffic_limit ath.%d-%d set_every_node %d",AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,value);
											wid_syslog_debug_debug(WID_DEFAULT,"wid_set_bss_traffic_limit_average_value buf %s\n",buf);
											wid_radio_set_extension_command(WTPIndex,buf);
										}

										if(AC_BSS[bssindex]->send_traffic_limit != 0)
										{
											value = AC_BSS[bssindex]->send_traffic_limit;
											memset(buf,0,DEFAULT_LEN);
											sprintf(buf,"autelan traffic_limit ath.%d-%d set_vap_flag 1;autelan traffic_limit ath.%d-%d set_vap_send %d",AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,value);
											wid_syslog_debug_debug(WID_DEFAULT,"wid_set_bss_traffic_limit_send_value buf %s\n",buf);
											wid_radio_set_extension_command(WTPIndex,buf);
										}

										if(AC_BSS[bssindex]->send_average_rate != 0)
										{
											value = AC_BSS[bssindex]->send_average_rate;
											memset(buf,0,DEFAULT_LEN);
											sprintf(buf,"autelan traffic_limit ath.%d-%d set_every_node_flag 1;autelan traffic_limit ath.%d-%d set_every_node_send %d",AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,AC_BSS[bssindex]->Radio_L_ID
																																							,AC_BSS[bssindex]->WlanID
																																							,value);
											wid_syslog_debug_debug(WID_DEFAULT,"wid_set_bss_traffic_limit_average_value buf %s\n",buf);
											wid_radio_set_extension_command(WTPIndex,buf);
										}
									#endif
									}
								}

								/*ip mac binding*/
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
								    && (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->ip_mac_binding == 1))
								{
									//unsigned int value = 0;
									
									memset(buf, 0, DEFAULT_LEN);
									bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]; //fengwenchao add 20120517 for autelan-2970
									sprintf(buf,"/usr/sbin/set_ip_enable ath.%d-%d %d",AC_BSS[bssindex]->Radio_L_ID,AC_BSS[bssindex]->WlanID,1);
									
									wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"wid_set_sta_ip_mac_binding apcmd %s\n", WAI_FUNC_LINE_LITERAL, WTPIndex, buf);
									wid_radio_set_extension_command(WTPIndex,buf);
								}
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
								    && ((AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->unicast_sw == 1)
    									|| (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->muti_bro_cast_sw == 1)
    									|| (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->muti_rate != 10)))
								{
									if ((1 == AC_BSS[bssindex]->muti_bro_cast_sw) && (1 == AC_BSS[bssindex]->unicast_sw))
									{
										pcy = 3;
									}
									else if (1 == AC_BSS[bssindex]->muti_bro_cast_sw)
									{
										pcy = 2;
									}
									else if (1 == AC_BSS[bssindex]->unicast_sw)
									{
										pcy = 1;
									}
									else
									{
										pcy = 0;
									}
									if(AC_BSS[bssindex]->wifi_sw == 1)
									{
										pcy = pcy|0x4;
									}
									else
									{
										pcy = pcy&~0x4;
									}
									setWtpUniMutiBroCastIsolation(WTPIndex,m,k,pcy);
									wid_syslog_debug_debug(WID_DBUS, WAI_FUNC_LINE_FMT" radio%d-%d wlan%d bssindex=%d.config save.\n", WAI_FUNC_LINE_LITERAL, WTPIndex, m, k, (AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]));
								}
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
								    && (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->noResToStaProReqSW == 1))
							    {
									setWtpNoRespToStaProReq(WTPIndex,m,k,AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->noResToStaProReqSW);
									wid_syslog_debug_debug(WID_DBUS, WAI_FUNC_LINE_FMT" radio%d-%d wlan%d bssindex=%d.config save.\n", WAI_FUNC_LINE_LITERAL, WTPIndex, m, k, (AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]));
								}
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
								    && (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->muti_rate != 10))
							    {
									setWtpUniMutiBroCastRate(WTPIndex,m,k,AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->muti_rate);
									wid_syslog_debug_debug(WID_DBUS, WAI_FUNC_LINE_FMT" radio%d-%d wlan%d bssindex=%d.config save.\n", WAI_FUNC_LINE_LITERAL, WTPIndex, m, k, (AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]));
								}
								if (gtrapflag >= 4)
								{
						#if NOSOFTAC
									int type = 2;//auto
									int flag = 0;//disable
									wid_dbus_trap_ap_ath_error(WTPIndex, m, k, type, flag);
                        #endif
								}
								//wei chao add flow check 2011.11.03
								
								if ((AC_WLAN[k]->SecurityType == IEEE8021X)
								    || (AC_WLAN[k]->SecurityType == WPA_E)
								    || (AC_WLAN[k]->SecurityType == WPA2_E)
								    || (AC_WLAN[k]->SecurityType == MD5))
								{
									msgq msg3;
									struct msgqlist *elem3 = NULL;
									
									msg2.mqinfo.u.WlanInfo.WLANID = k;
									msg2.mqinfo.u.WlanInfo.Radio_L_ID = m;

									msg3.mqid = WTPIndex%THREAD_NUM +1;
									msg3.mqinfo.WTPID = WTPIndex;			
									msg3.mqinfo.type = CONTROL_TYPE;
									msg3.mqinfo.subtype = WTP_S_TYPE;
									msg3.mqinfo.u.WtpInfo.Wtp_Op = WTP_FLOW_CHECK;
									msg3.mqinfo.u.WlanInfo.WLANID = k;
									msg3.mqinfo.u.WlanInfo.Radio_L_ID = m;
									msg3.mqinfo.u.WlanInfo.flow_check = AC_WLAN[k]->flow_check;
									msg3.mqinfo.u.WlanInfo.no_flow_time = AC_WLAN[k]->no_flow_time;
									msg3.mqinfo.u.WlanInfo.limit_flow = AC_WLAN[k]->limit_flow;
								 
    								elem3 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
									if (elem3 == NULL)
									{			
										wid_syslog_crit(WAI_FUNC_LINE_FMT" malloc %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										perror("malloc");
										return ;
									}
									
									memset((char*)&(elem3->mqinfo), 0, sizeof(msgqdetail));
									elem3->next = NULL;
									memcpy((char*)&(elem3->mqinfo),(char*)&(msg3.mqinfo),sizeof(msg3.mqinfo));
									WID_INSERT_CONTROL_LIST(WTPIndex, elem3);
								}
								
								if (AC_WLAN[k]->Status == 0)
								{
									command = (char *)malloc(sizeof(char)*100);
									if (NULL == command)
									{
										wid_syslog_crit(WAI_FUNC_LINE_FMT" malloc %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										return ;
									}
									ath_str = (char *)malloc(sizeof(char)*20);
									if (NULL == ath_str)
									{
										wid_syslog_crit(WAI_FUNC_LINE_FMT" malloc %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										CW_FREE_OBJECT(command);
										return ;
									}
									memset(command,0,100);
									memset(ath_str,0,20);
									memset(&msg4,0,sizeof(msg4));
									msg4.mqid = WTPIndex%THREAD_NUM +1;
									msg4.mqinfo.WTPID = WTPIndex;			
									msg4.mqinfo.type = CONTROL_TYPE;
									msg4.mqinfo.subtype = WTP_S_TYPE;
									msg4.mqinfo.u.WtpInfo.Wtp_Op = WTP_EXTEND_CMD;
									
									sprintf(ath_str, "ath.%d-%d", m, k);
									sprintf(command, "ifconfig %s down;iwpriv %s inact %u;ifconfig %s up", ath_str, ath_str, AC_WLAN[k]->ap_max_inactivity,ath_str);
									memcpy(msg4.mqinfo.u.WtpInfo.value, command, strlen(command));
									
									CW_FREE_OBJECT(command);
									CW_FREE_OBJECT(ath_str);
									elem4 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
    								if (elem4 == NULL)
									{
										wid_syslog_crit(WAI_FUNC_LINE_FMT" malloc %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										perror("malloc");
										return ;
									}
								
									memset((char*)&(elem4->mqinfo), 0, sizeof(msgqdetail));
									elem4->next = NULL;
									memcpy((char*)&(elem4->mqinfo), (char*)&(msg4.mqinfo), sizeof(msg4.mqinfo));
									WID_INSERT_CONTROL_LIST(WTPIndex, elem4);
									/*coverity-CID: 10860 Resource leak , move up */
								}
								/* send eap switch & mac to ap,zhangshu add 2010-10-22 */
								bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m];
								
								memset(apcmd, 0, WID_SYSTEM_CMD_LENTH);
								
                            	if ((AC_WLAN[k]->eap_mac_switch == 1)
									&& (AC_WLAN[k]->wlan_if_policy == NO_INTERFACE)
									&& (AC_BSS[bssindex] != NULL)
									&& (AC_BSS[bssindex]->BSS_IF_POLICY == NO_INTERFACE))
								{
									sprintf(apcmd,"set_eap_mac ath.%d-%d %s", m, k, AC_WLAN[k]->eap_mac);
									wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" wtp%d enable wlan%d: set eap mac cmd %s\n", WAI_FUNC_LINE_LITERAL, WTPIndex, k, apcmd);
									wid_radio_set_extension_command(WTPIndex,apcmd);
								}
								else
								{
									// sprintf(apcmd,"set_eap_mac ath.%d-%d 0",m,k);
								}
								
								int bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m];
								if ((AC_BSS[bssindex])
								    && (AC_BSS[bssindex]->BSS_IF_POLICY != NO_INTERFACE)
								    && (AC_BSS[bssindex]->BSS_TUNNEL_POLICY != CW_802_DOT_11_TUNNEL))
								{
									msgq msg;
									struct msgqlist *elem = NULL;
									memset((char*)&msg, 0, sizeof(msg));
									msg.mqid = WTPIndex%THREAD_NUM+1;
									msg.mqinfo.WTPID = WTPIndex;
									msg.mqinfo.type = CONTROL_TYPE;
									msg.mqinfo.subtype = WLAN_S_TYPE;
									msg.mqinfo.u.WlanInfo.Wlan_Op = WLAN_CHANGE_TUNNEL;
									msg.mqinfo.u.WlanInfo.WLANID = k;
									msg.mqinfo.u.WlanInfo.Radio_L_ID = m;
									msg.mqinfo.u.WlanInfo.bssindex = AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m];
									
									if (AC_WTP[WTPIndex]->WTPStat == WID_RUN)
									{ 
										if (msgsnd(ACDBUS_MSGQ, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
										{
											wid_syslog_crit(WAI_FUNC_LINE_FMT" msgsend %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
											perror("msgsnd");
										}
									}
									else
									{
										elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
										if (elem == NULL)
										{
											wid_syslog_crit(WAI_FUNC_LINE_FMT" malloc %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
											perror("malloc");
											return ;
										}
										memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
										elem->next = NULL;
										memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
										WID_INSERT_CONTROL_LIST(WTPIndex, elem);
									}
								}
								/* wlan tunnel switch func for REQUIREMENTS-264 */
								memset(apcmd,0,WID_SYSTEM_CMD_LENTH);
								
								if (1 == AC_WLAN[k]->wlan_tunnel_switch)
								{
									//sprintf(apcmd, "echo 3 > /proc/sys/net/ath.%d-%d/vap_splitmac", m, k);
									sprintf(apcmd,TUNNEL_SWITCH_CMD";"AP_OPEN_IPFORWARD";"AP_SPFAST_DOWM";"AP_SPFAST_UP,m, k);
									wid_radio_set_extension_command(WTPIndex,apcmd);
								}
								/*acl-group*/
								if ((0 != AC_WLAN[k]->aclgrp_id) && (NULL != STA_ACL_GROUP[AC_WLAN[k]->aclgrp_id]))
								{
									aclgroupid = AC_WLAN[k]->aclgrp_id;
									memset((char*)&msg, 0, sizeof(msg));
									msg.mqid = WTPIndex%THREAD_NUM+1;
									msg.mqinfo.WTPID = WTPIndex;
									msg.mqinfo.type = CONTROL_TYPE;
									msg.mqinfo.subtype = WTP_S_TYPE;
									msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_ACL_LIST;
									msg.mqinfo.u.WtpInfo.value[0] = k;
									msg.mqinfo.u.WtpInfo.value[1] = m;
									msg.mqinfo.u.WtpInfo.value[2] = OVERWRITE;
									msg.mqinfo.u.WtpInfo.value[3] = LEVEL_WLAN;
									msg.mqinfo.u.WtpInfo.value[4] = AC_WLAN[k]->aclgrp_id;

									/*
									msg.mqinfo.u.WtpInfo.value3 = STA_ACL_GROUP[aclgroupid]->rule_cnt;
									rulelist = STA_ACL_GROUP[aclgroupid]->rule_list;
									for (i = 0; i < STA_ACL_GROUP[aclgroupid]->rule_cnt; i++)
									{
										msg.mqinfo.u.WtpInfo.value[i] = rulelist->rule_id;
										rulelist = rulelist->next;
									}
									*/
									if (AC_WTP[WTPIndex]->WTPStat == WID_RUN)
									{ 
										if (msgsnd(ACDBUS_MSGQ, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
										{
											wid_syslog_crit(WAI_FUNC_LINE_FMT" msgsend %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										}
									}
									else
									{
										elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
										if (NULL == elem)
										{			
											WID_MALLOC_ERR();			
											return ;
										}			
										memset(elem, 0, sizeof(struct msgqlist));
										elem->mqinfo.WTPID = WTPIndex;
										elem->mqinfo.type = CONTROL_TYPE;
										elem->mqinfo.subtype = WTP_S_TYPE;
										elem->mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_ACL_LIST;
										elem->mqinfo.u.WtpInfo.value[0] = k;
										elem->mqinfo.u.WtpInfo.value[1] = m;
										elem->mqinfo.u.WtpInfo.value[2] = OVERWRITE;
										elem->mqinfo.u.WtpInfo.value[3] = LEVEL_WLAN;
										elem->mqinfo.u.WtpInfo.value[4] = AC_WLAN[k]->aclgrp_id;
										
										WID_INSERT_CONTROL_LIST(WTPIndex, elem);
										elem = NULL;
									}
								}
                            	/* end */
								if (0 != AC_WLAN[k]->electronic_menu)
								{																	
									memset((char*)&msg, 0, sizeof(msg));
									msg.mqid = WTPIndex%THREAD_NUM+1;
									msg.mqinfo.WTPID = WTPIndex;
									msg.mqinfo.type = CONTROL_TYPE;
									msg.mqinfo.subtype = WTP_S_TYPE;
									msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_ELECTRONIC_MENU;
									msg.mqinfo.u.WtpInfo.value[0] = k;
									msg.mqinfo.u.WtpInfo.value[1] = m;
									msg.mqinfo.u.WtpInfo.value[2] = LEVEL_WLAN;
									msg.mqinfo.u.WtpInfo.value[3] = AC_WLAN[k]->electronic_menu;
										
									if (WID_RUN == AC_WTP[WTPIndex]->WTPStat)
									{	
										if (-1 == msgsnd(ACDBUS_MSGQ, (msgq *)&msg, sizeof(msg.mqinfo), 0))
										{
											wid_syslog_info(WAI_FUNC_LINE_FMT" msgsend %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										}
									}
									else
									{
										elem = NULL;
										elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
										if (NULL == elem)
										{
											WID_MALLOC_ERR();
											return;
										}
										memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
										elem->next = NULL;
										memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
										WID_INSERT_CONTROL_LIST(WTPIndex, elem);									
									}
									wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" AC_WLAN[%d] local-survival switch %d\n",
																		WAI_FUNC_LINE_LITERAL, k, AC_WLAN[k]->electronic_menu);
								}
								if (0 != AC_WLAN[k]->macgroupID)
								{									
									groupid = AC_WLAN[k]->macgroupID;		
									policy = AC_WLAN[k]->mac_grp_policy;									
									elem = NULL;								
									memset((char*)&msg, 0, sizeof(msg));
									msg.mqid = WTPIndex%THREAD_NUM+1;
									msg.mqinfo.WTPID = WTPIndex;
									msg.mqinfo.type = CONTROL_TYPE;
									msg.mqinfo.subtype = WTP_S_TYPE;
									msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_BLACK_WHITE_LIST;
									msg.mqinfo.u.WtpInfo.value[0] = k;
									msg.mqinfo.u.WtpInfo.value[1] = m;
									msg.mqinfo.u.WtpInfo.value[2] = policy;
									msg.mqinfo.u.WtpInfo.value[3] = LEVEL_WLAN;
									msg.mqinfo.u.WtpInfo.value[4] = OVERWRITE;
									CWThreadMutexLock(&MAC_GROUP[groupid]->macListMutex);
									msg.mqinfo.u.WtpInfo.value[5] = MAC_GROUP[groupid]->count;
									MAC_list *maclist = MAC_GROUP[groupid]->macList;
									for (i = 0; (i < MAC_GROUP[groupid]->count) && (i < GROUP_MAC_COUNT); i++)
									{
										memcpy(msg.mqinfo.u.WtpInfo.macarry[i].mac, maclist->mac, sizeof(maclist->mac));
										maclist = maclist->next;
									}
									CWThreadMutexUnlock(&MAC_GROUP[groupid]->macListMutex);
										
									if (WID_RUN == AC_WTP[WTPIndex]->WTPStat)
									{	
										if (-1 == msgsnd(ACDBUS_MSGQ, (msgq *)&msg, sizeof(msg.mqinfo), 0))
										{
											wid_syslog_info(WAI_FUNC_LINE_FMT" msgsend %s", WAI_FUNC_LINE_LITERAL, strerror(errno));
										}
									}
									else
									{
										elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
										if(NULL == elem)
										{
											WID_MALLOC_ERR();
											return;
										}
										memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
										elem->next = NULL;
										memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
										WID_INSERT_CONTROL_LIST(WTPIndex, elem);									
									}
									wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" AC_WLAN[%d]->macgroupID %d\n",
																		WAI_FUNC_LINE_LITERAL, k, AC_WLAN[k]->macgroupID);
								}
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m]))
									&& (AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->packet_power != 0))
								{
									WID_RADIO_WLAN_PACKET_POWER(WTPIndex,m,k,AC_BSS[(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])]->packet_power);
								}
								if ((check_bssid_func(AC_WLAN[k]->S_WTP_BSS_List[WTPIndex][m])) 
									&& (0 != AC_WLAN[k]->balance_switch))
								{
									wid_bss_balance_switch(WTPIndex, m, k);										
								}
							}
							else
							{
								continue;
							}
						}
					}
				}
			}
			}
		}
		int j = 0;
		for (j = 0; j < L_BSS_NUM; j++)
		{
			if ((AC_WTP[WTPIndex])
				&& (i < L_RADIO_NUM)
				&& (AC_WTP[WTPIndex]->WTP_Radio[i])
				&& (AC_WTP[WTPIndex]->WTP_Radio[i]->BSS[j] != NULL))
			{
				unsigned int BssIndex = AC_WTP[WTPIndex]->WTP_Radio[i]->BSS[j]->BSSIndex;
    			AC_BSS[BssIndex]->master_to_disable = AC_WTP[WTPIndex]->master_to_disable;//qiuchen add it for AXSSZFI-1191
#if NOSOFTAC
    			AsdWsm_BSSOp(BssIndex, WID_DEL, 0);			
    			wid_update_bss_to_wifi(BssIndex,WTPIndex,0);
#endif
				asd_free_stas(BssIndex);
				
				AC_WTP[WTPIndex]->WTP_Radio[i]->BSS[j]->State = 0;
				memset(AC_WTP[WTPIndex]->WTP_Radio[i]->BSS[j]->BSSID, 0, MAC_LEN);
    		}
    	}
		
		if ((AC_WTP[WTPIndex])&& (i < L_RADIO_NUM) && (AC_WTP[WTPIndex]->WTP_Radio[i] != NULL))
		{
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->auto_channel != 0)
    		{
				memset(buf, 0, DEFAULT_LEN);
				sprintf(buf, "echo 1 > /proc/sys/dev/wifi%d/nonoverlapping", i);
				wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT" wtp%d radio%d cmd buf %s\n",WAI_FUNC_LINE_LITERAL, WTPIndex, i, buf);
				wid_radio_set_extension_command(WTPIndex,buf);
			}
		}
	}
	
	if (AC_WTP[WTPIndex])
	{
		AC_WTP[WTPIndex]->rx_bytes_before = AC_WTP[WTPIndex]->rx_bytes;
		AC_WTP[WTPIndex]->tx_bytes_before = AC_WTP[WTPIndex]->tx_bytes;
	}
	/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,start*/
	if ((AC_WTP[WTPIndex]) && (AC_WTP[WTPIndex]->ntp_state != 1))
	{
		wid_set_wtp_ntp(WTPIndex);
	}
	/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,end*/
 
	AC_WTP[WTPIndex]->master_to_disable = 0;//qiuchen add it for AXSSZFI-1191
	WID_CONFIG_SAVE(WTPIndex);
}

//dynamic area
CWBool WidParseJoinRequestMessageForAutoApLogin
(
	char *msg,
	int len,
	int *seqNumPtr,
	CWProtocolJoinRequestValues *valuesPtr
)
{
	unsigned short int elemid = 0;
	unsigned short int elemlen = 0;
	CWBool dataFlag = CW_FALSE;
	int offsetTillMessages = 0;
	CWProtocolMessage completeMsg;
	CWControlHeaderValues controlVal;
	CWProtocolTransportHeaderValues transportVal;

	if(msg == NULL || seqNumPtr == NULL || valuesPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
  	
	if(!(CWParseTransportHeader(&completeMsg, &transportVal, &dataFlag)))
	{
	    return CW_FALSE;	
	}
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
	{
	    return CW_FALSE; // will be handled by the caller
	}

	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_JOIN_REQUEST)
	{
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Join Request as Expected");
	}
	
	*seqNumPtr = controlVal.seqNum;
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
	
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen)
	{
		unsigned short int elemType = 0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen = 0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
		
		//wid_syslog_debug_debug("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
		switch(elemType) 
		{
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
			{
				if(!(CWParseWTPBoardData(&completeMsg, elemLen, &(valuesPtr->WTPBoardData))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break; 
			}
			case CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE:
			{
				if(!(CWParseLocationData(&completeMsg, elemLen, &(valuesPtr->location))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
			{
				if(!(CWParseSessionID(&completeMsg, elemLen, valuesPtr)))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
			{
				if(!(CWParseWTPDescriptor(&completeMsg, elemLen, &(valuesPtr->WTPDescriptor)))) // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE:
			{
				if(!(CWParseWTPIPv4Address(&completeMsg, elemLen, valuesPtr)))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_NAME_CW_TYPE:
			{
				if(!(CWParseWTPName(&completeMsg, elemLen, &(valuesPtr->name))))    // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
			{
				if(!(CWParseWTPFrameTunnelMode(&completeMsg, elemLen, &(valuesPtr->frameTunnelMode))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
			{
				if(!(CWParseWTPMACType(&completeMsg, elemLen, &(valuesPtr->MACType))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			{
				CWParseFormatMsgElem(&completeMsg, &elemid, &elemlen);
				if(elemlen > controlVal.msgElemsLen)
				{
					wid_syslog_err("%s elemType %d elemlen:%d > controlVal.msgElemsLen:%d\n", __func__, elemType, elemlen, controlVal.msgElemsLen);
					return CW_TRUE;
				}
				if(CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE == elemid)
				{
					if(!(CWParseWTPLOGINType(&completeMsg, elemlen, &(valuesPtr->ConStatus))))   // will be handled by the caller
					{
						return CW_FALSE;
					}
				}
				break;
			}
			
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}
	
	if(completeMsg.offset != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
    }
    
	return CW_TRUE;
}

int parse_dynamic_wtp_name(char *name)
{
	unsigned int wtpid = 0;
	int ret = 0;
	ret = parse_int_ID(name, &wtpid);
	if(ret == 0)
	{
		if(wtpid == 0)
		{
			return 0;//use any id
		}
		else
		{
			if(AC_WTP[wtpid] == NULL)
			{
				return 0;
			}
			else if(gWTPs[wtpid].currentState == CW_ENTER_RUN)
			{
				return -1;//drop
			}
			else
			{
				return wtpid;//change info
			}
		}
	}
	else
	{
		return 0;
	}
}

WIDAUTOAPINFO *parse_dynamic_wtp_login_situation(CWWTPVendorInfos *valPtr)
{
	int i = 0;
	int IGNORE_UBSV rmodel_state = 0;
	int model_state = 0;
	int sn_state = 0;
	int mac_state = 0;
	int len = 0;
	//WIDAUTOAPINFO *return_autoapinfo = NULL;
	//save mac parameter
	unsigned char *WTP_MAC = NULL;
	
	WIDAUTOAPINFO *autoapinfo = NULL;
	autoapinfo = (WIDAUTOAPINFO *)malloc(sizeof(WIDAUTOAPINFO));
	if (autoapinfo == NULL)
	{
		return NULL;
	}
	
	autoapinfo->mac = NULL;
	autoapinfo->model = NULL;
	autoapinfo->sn = NULL;
	autoapinfo->realmodel = NULL;
	autoapinfo->apcodeflag = 0;
	
	for (i = 0; i < valPtr->vendorInfosCount; i++)
	{
		if ((valPtr->vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			len = (valPtr->vendorInfos)[i].length;
			autoapinfo->model = (unsigned char*)malloc(len+1);
			if (autoapinfo->model == NULL)
			{
				perror("malloc");
				CW_FREE_OBJECT(WTP_MAC);
				WidDestroyAutoApLoginInfo(autoapinfo);
				return NULL;
			}
			memset(autoapinfo->model, 0, len+1);
			memcpy(autoapinfo->model, (valPtr->vendorInfos)[i].model, len);
			model_state = 1; 
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			len = (valPtr->vendorInfos)[i].length;
			char *sn = (char *)((valPtr->vendorInfos)[i].SN);
			str2higher(&sn);
			(valPtr->vendorInfos)[i].SN = (unsigned char *)sn;

			autoapinfo->sn = (unsigned char*)malloc(len+1);
			if (autoapinfo->sn == NULL)
			{
				perror("malloc");
				CW_FREE_OBJECT(WTP_MAC);
				WidDestroyAutoApLoginInfo(autoapinfo);
				return NULL;
			}
			memset(autoapinfo->sn, 0, len+1);
			memcpy(autoapinfo->sn, (valPtr->vendorInfos)[i].SN, len);
			sn_state = 1;
		}
		if ((valPtr->vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			len = (valPtr->vendorInfos)[i].length; 
			autoapinfo->realmodel = (unsigned char*)malloc(len+1);
			if (autoapinfo->realmodel == NULL)
			{
				perror("malloc");
				CW_FREE_OBJECT(WTP_MAC);
				WidDestroyAutoApLoginInfo(autoapinfo);
				return NULL;
			}
			memset(autoapinfo->realmodel, 0, len+1);
			memcpy(autoapinfo->realmodel, (valPtr->vendorInfos)[i].Rmodel, len);
			rmodel_state = 1;
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_ID)
		{
			continue;
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_REVISION)
		{
			continue;
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			len = (valPtr->vendorInfos)[i].length;
			if (len != MAC_LEN)
			{
				//printf("wtp mac error\n");
			}
			CW_FREE_OBJECT(WTP_MAC);
			unsigned char muti_brc_mac[MAC_LEN] = {0};
			memcpy(muti_brc_mac,(valPtr->vendorInfos)[i].mac,MAC_LEN);
			if (muti_brc_mac[0] & 0x1)
			{
				WidDestroyAutoApLoginInfo(autoapinfo);
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"access with multicast mac"WAI_MAC_FMT"\n", \
								WAI_FUNC_LINE_LITERAL, 0, WAI_MAC_LITERAL(muti_brc_mac));
				return NULL;
			}

			WTP_MAC = (unsigned char*)malloc(MAC_LEN+1);
			if (WTP_MAC == NULL)
			{
				perror("malloc");
				WidDestroyAutoApLoginInfo(autoapinfo);
				return NULL;
			}
			memset(WTP_MAC, 0, MAC_LEN+1);
			
			autoapinfo->mac = (unsigned char*)malloc(MAC_LEN+1);
			if (autoapinfo->mac == NULL)
			{
				perror("malloc");
				CW_FREE_OBJECT(WTP_MAC);
				WidDestroyAutoApLoginInfo(autoapinfo);
				return NULL;
			}
			memset(autoapinfo->mac, 0, MAC_LEN+1);
			
			memcpy(WTP_MAC, (valPtr->vendorInfos)[i].mac, MAC_LEN);
			memcpy(autoapinfo->mac, (valPtr->vendorInfos)[i].mac, MAC_LEN);
			if (wid_auto_ap_mac_filter(WTP_MAC) == 0)
			{
				mac_state = 1;
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"mac "WAI_MAC_FMT"match mac oui check policy\n", \
								 WAI_FUNC_LINE_LITERAL, 0, WAI_MAC_LITERAL(WTP_MAC));		
			}
			else
			{
				mac_state = 1;//change to 1 because ap mac change
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"mac "WAI_MAC_FMT"mismatch mac oui check policy need drop\n", \
								 WAI_FUNC_LINE_LITERAL, 0, WAI_MAC_LITERAL(WTP_MAC));		
			}
		}
		else 
		{
		//not process yet
		}
	}
	//decide return value
	/* Coverity: CID: 10194  Error-Type: Resource leak  */
	if (!WTP_MAC)
	{
		WidDestroyAutoApLoginInfo(autoapinfo);
		return NULL;
	}
	if ((model_state == 1) && (mac_state == 1) && (sn_state == 1))
	{
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_LOGIN_PHASE_FMT""WAI_AFI0_FMT"mac "WAI_MAC_FMT"model %s sn %s\n",  \
						WAI_FUNC_LINE_LITERAL, WAI_AFI_LOGIN_P1_LITERAL, "1", 0, WAI_MAC_LITERAL(WTP_MAC), autoapinfo->model, autoapinfo->sn);
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_LOGIN_PHASE_FMT""WAI_AFI0_FMT"full match dynamic access policy so will send a response\n", \
						WAI_FUNC_LINE_LITERAL, WAI_AFI_LOGIN_P1_LITERAL, "2", 0);
		
		CW_FREE_OBJECT(WTP_MAC);

		return autoapinfo;
	}
	else
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_LOGIN_PHASE_FMT""WAI_AFI0_FMT"%s%s%s mismatch dynamic access policy so no response\n", \
						WAI_FUNC_LINE_LITERAL, WAI_AFI_LOGIN_P1_LITERAL, 1, 0, model_state ? "":"model ", mac_state ? "":"mac ", sn_state ? "":"sn ");
		CW_FREE_OBJECT(WTP_MAC);
		CW_FREE_OBJECT(autoapinfo->mac);
		CW_FREE_OBJECT(autoapinfo->model);
		CW_FREE_OBJECT(autoapinfo->sn);
		CW_FREE_OBJECT(autoapinfo->realmodel);		
		CW_FREE_OBJECT(autoapinfo);
		return NULL;
	}
}
/*
int dynamicWTPaddrbinding(CWNetworkLev4Address *addrPtr)
{
	int i = 0;
	CWThreadMutexLock(&gWTPsMutex);
	for(i = 1; i < CW_MAX_WTP; i++) 
	{	
		if((!(gWTPs[i].isNotFree))
			//&& (&(gWTPs[i].address) == NULL)
			&&(AC_WTP[i] == NULL))
		{ 
			//printf("sock_cpy_addr_port\n");
			if((sock_cpy_addr_port((struct sockaddr*)&(gWTPs[i].address),(struct sockaddr*)addrPtr)) == 0)
			{	
				CWThreadMutexUnlock(&gWTPsMutex);
				//printf("i %d\n",i);
				return i;
			}
		}
	}
	CWThreadMutexUnlock(&gWTPsMutex);
	return -1;
}*/
int wid_auto_ap_mac_filter(unsigned char *mac)
{
	return 0;
	//check auto ap whether AUTELAN oui : 00-1F-64
	if(((*(mac)) == 0)&&((*(mac+1)) == 31)&&((*(mac+2)) == 100))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int wid_auto_ap_binding(int wtpid,int ifindex)
{
	int interface_state = 0;
	int state = 0;
	int used_state = 0;
	int ret = 0;
	int ret1 = 0;
	int i = 0, j = 0;
	char *name = NULL;
	
	WID_CHECK_WTP_STANDARD_RET(wtpid, WTP_ID_NOT_EXIST);
	
	name = (char *)malloc(ETH_IF_NAME_LEN+1);
	if (name == NULL)
	{
		perror("malloc");
		return -1;
	}
	memset(name,0,ETH_IF_NAME_LEN+1);

	//find ifindex in the auto ap login iflist
	wid_auto_ap_if *iflist = NULL;
	iflist = g_auto_ap_login.auto_ap_if;
	int wlannum = 0;
	unsigned char wlan[L_BSS_NUM];
	memset(wlan, 0, L_BSS_NUM);

	while (iflist != NULL)
	{
		if (iflist->ifindex == ifindex)
		{
			memset(name,0,ETH_IF_NAME_LEN+1);
			memcpy(name,iflist->ifname,ETH_IF_NAME_LEN);
			wlannum = iflist->wlannum;
			for (i = 0; i < L_BSS_NUM; i++)
			{
				if (iflist->wlanid[i] != 0)
				{
					wlan[j] = iflist->wlanid[i];
					j++;
				}
			}
			interface_state = 1;
			break;
		}
		iflist = iflist->ifnext;
	}

	if (interface_state != 1)
	{
		CW_FREE_OBJECT(name);
		return -1;
	}
	else//find ifindex in the list,then binding
	{
		ret = WID_BINDING_IF_APPLY_WTP(wtpid,name);
		if (ret == 0)
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bind interface %s success\n", \
						WAI_FUNC_LINE_LITERAL, wtpid,AC_WTP[wtpid]->WTPNAME,name);
			state = 1;
			for (i = 0; i < wlannum; i++)
			{
				if (wlan[i] == 0)
				{
					continue;
				}
				//ret1 = WID_ADD_WLAN_APPLY_RADIO(radioid,wlan[i]);
				ret1 = WID_BINDING_WLAN_APPLY_WTP(wtpid,wlan[i]);
				if (ret1 == 0)
				{
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bind %u "WAI_WLAN_FMT"success\n", \
								WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, i, wlan[i]);
				}
				else
				{ 
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bind %u "WAI_WLAN_FMT"error %d\n", \
								WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, i, wlan[i], ret1);
				}
			}		
		}
		else
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bind interface %s error %d\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, name, ret);
			state = 0;
		}
	}

	if (state == 1)
	{
		used_state = WID_USED_WTP(wtpid);
		if (used_state == 0)
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"config take effect success\n", \
						WAI_FUNC_LINE_LITERAL, wtpid,AC_WTP[wtpid]->WTPNAME);
			CW_FREE_OBJECT(name);
			return 0;
		}
		else
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"config take effect failed\n", \
						WAI_FUNC_LINE_LITERAL, wtpid,AC_WTP[wtpid]->WTPNAME);
			CW_FREE_OBJECT(name);
			return -1;
		}
	}
	CW_FREE_OBJECT(name);
	
	return -1;
}

int wid_dynamic_change_wtp_info(int wtpid,WIDAUTOAPINFO *info)
{
	int len = 0;
	
	WID_CHECK_WTP_STANDARD_RET(wtpid, WTP_ID_NOT_EXIST);
	
	if ((AC_WTP[wtpid] == NULL) || (info == NULL))
	{
		return -1;
	}
	
	if (info->mac != NULL)
	{
		if (memcmp(AC_WTP[wtpid]->WTPMAC, info->mac, MAC_LEN) != 0)
		{
			memset(AC_WTP[wtpid]->WTPMAC,0,MAC_LEN);
			memcpy(AC_WTP[wtpid]->WTPMAC,info->mac,MAC_LEN);		
		}
	}
	if (info->sn != NULL)
	{
		len = strlen((char *)info->sn);
		if ((AC_WTP[wtpid]) && (AC_WTP[wtpid]->WTPSN) && (strncmp(AC_WTP[wtpid]->WTPSN,(char *)info->sn,len) != 0))
		{
			/*default code
			CW_FREE_OBJECT(AC_WTP[wtpid]->WTPSN);
			AC_WTP[wtpid]->WTPSN = (char *)malloc(len+1);
			memset(AC_WTP[wtpid]->WTPSN,0,len+1);
			memcpy(AC_WTP[wtpid]->WTPSN,info->sn,len);
			*/
			memset(AC_WTP[wtpid]->WTPSN, 0, NAS_IDENTIFIER_NAME);
			if (wid_illegal_character_check((char*)info->sn, len, 0) == 1)
			{
				memcpy(AC_WTP[wtpid]->WTPSN,info->sn,len);
			}
			else
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"with illegal sn\n", WAI_FUNC_LINE_LITERAL,wtpid, AC_WTP[wtpid]->WTPNAME);
			}
		}
	}
	if (info->model != NULL)
	{
		len = strlen((char *)info->model);
		if ((AC_WTP[wtpid]) && (AC_WTP[wtpid]->APCode) && (strncmp(AC_WTP[wtpid]->APCode,(char *)info->model,len) != 0))
		{
			CW_FREE_OBJECT(AC_WTP[wtpid]->APCode);
			AC_WTP[wtpid]->APCode = (char *)malloc(len+1);
			memset(AC_WTP[wtpid]->APCode, 0, len+1);
			memcpy(AC_WTP[wtpid]->APCode, info->model, len);
		}
	}
	if (info->realmodel != NULL)
	{
		len = strlen((char *)info->realmodel);
		if ((AC_WTP[wtpid]) && (AC_WTP[wtpid]->WTPModel) && (strncmp(AC_WTP[wtpid]->WTPModel,(char *)info->realmodel,len) != 0))
		{
			CW_FREE_OBJECT(AC_WTP[wtpid]->WTPModel);
			AC_WTP[wtpid]->WTPModel= (char *)malloc(len+1);
			memset(AC_WTP[wtpid]->WTPModel, 0, len+1);
			memcpy(AC_WTP[wtpid]->WTPModel, info->realmodel, len);
		}
	}
	return 0;
}
/* book modify,2011-11-15 */
int wid_parse_wtp_model_rmodel(WIDAUTOAPINFO *info)
{
	int ret = 0;
	int len1 = 0;/*WTPModel lenth*/
	int len2 = 0;/*str model lenth*/
	int i = 0;

	if (info == NULL)
	{
		return -1;
	}
	if (info->realmodel == NULL)
	{
	    wid_syslog_err(WAI_FUNC_LINE_FMT"model nil\n",WAI_FUNC_LINE_LITERAL);
		return -1;
	}

	char *RealModel = (char *)info->realmodel;

	if (RealModel  != NULL)
	{
		len1 = strlen(RealModel);
	}
	for (i = 0; i < ((sizeof(SOFT_XMLINFO))/sizeof(SOFT_XMLINFO[0])); i++)
	{
		if (SOFT_XMLINFO[i].str_ap_model != NULL)
		{
			len2 = strlen(SOFT_XMLINFO[i].str_ap_model);
		}
		if ((SOFT_XMLINFO[i].str_ap_model != NULL)
			&& (RealModel != NULL)
			&& (len2 == len1)
			&& (strcmp(SOFT_XMLINFO[i].str_ap_model,RealModel) == 0)) 
		{
			ret = 1;
			break;
		}
	}
	
	if (ret == 1)
	{
    	return 0;
	}
	
	return -1;
}

void WidDestroyJoinRequestValuesForAutoApLogin(CWProtocolJoinRequestValues *valPtr)
{
	int i = 0;
	
	if (valPtr == NULL)
	{
		return;
	}
	
	for (i = 0; i < valPtr->WTPBoardData.vendorInfosCount; i++)
	{
		
		if ((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].model);
		}
		else if ((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].SN);
		}
		else if ((valPtr->WTPBoardData.vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].mac);
		}
		else if ((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].Rmodel);
		}
		else if ((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_CODE_VERSION)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].codever);
		}
		else
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].valuePtr);
		}
	}
	
	CW_FREE_OBJECT(valPtr->WTPBoardData.vendorInfos);

	for (i = 0; i < (valPtr->WTPDescriptor.vendorInfos).vendorInfosCount; i++)
	{
		if ((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_HARDWARE_VERSION)
		{
			CW_FREE_OBJECT(((valPtr->WTPDescriptor.vendorInfos).vendorInfos)[i].sysver);
		}
		else if ((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
		{
			CW_FREE_OBJECT(((valPtr->WTPDescriptor.vendorInfos).vendorInfos)[i].ver);
		}
		else if ((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].type == CW_BOOT_VERSION)
		{
			CW_FREE_OBJECT(valPtr->WTPDescriptor.vendorInfos.vendorInfos[i].bootver);
		}
		else
		{
			CW_FREE_OBJECT((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].valuePtr);
		}
	}
	CW_FREE_OBJECT(valPtr->WTPDescriptor.vendorInfos.vendorInfos);
}

void WidDestroyAutoApLoginInfo(WIDAUTOAPINFO *info)
{
	if (info == NULL)
	{
	    return;
    }
	CW_FREE_OBJECT(info->mac);
	CW_FREE_OBJECT(info->sn);
	CW_FREE_OBJECT(info->model);
	CW_FREE_OBJECT(info->realmodel);
	
	CW_FREE_OBJECT(info);

	return ;
}


int wid_wtp_login_loadconfig(unsigned int wtpid)
{
	wlan_t wlanid = 0;
	unsigned char l_radioid = 0;
	struct wlanid *wlan_id = NULL;
	unsigned int bssindex = 0;
	unsigned int IGNORE_UBSV radioid = 0;
	unsigned int bind_wlan = 0;
	
	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"reload config...\n", WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME);	

	for (l_radioid = 0; (l_radioid < AC_WTP[wtpid]->RadioCount); l_radioid++)
	{
		radioid = wtpid * L_RADIO_NUM + l_radioid;
		
		/* list radio config */
		if (NULL == AC_WTP[wtpid]->WTP_Radio[l_radioid])
		{
			continue;
		}
		if (0 == AC_WTP[wtpid]->WTP_Radio[l_radioid]->isBinddingWlan)
		{
			continue;
		}

		/* reload RADIO's wlan config */
		wlan_id = AC_WTP[wtpid]->WTP_Radio[l_radioid]->Wlan_Id;
		for (; NULL != wlan_id; wlan_id = wlan_id->next)
		{
			wlanid = wlan_id->wlanid;
			if ((wlanid >= WLAN_NUM)
				|| (NULL == AC_WLAN[wlanid])
				|| (0 != AC_WLAN[wlanid]->Status)) /* is wlan disable */
			{
				continue;
			}

			/* wlan config */
			


			/* bss config */
			bssindex = AC_WLAN[wlanid]->S_WTP_BSS_List[wtpid][l_radioid];
			if (0 == bssindex)
			{
				continue;
			}
			if (0 != AC_WLAN[wlanid]->balance_switch)
			{
				//wtp_balance_handle(wtpid, wlanid, AC_WLAN[wlanid]->balance_switch);
				wid_bss_balance_switch(wtpid, l_radioid, wlanid);
			}
			/* qos config */
			if (0 != memcmp(QOS_AP_11E_TO_1P_DEFAULT, AC_WLAN[wlanid]->qos_ap_11e_to_1p, MAX_QOS_11E_VAL+1))
			{
				set_ap_bss_qos_map_default(wtpid, l_radioid, QOS_MAP_11E_TO_1P, wlanid);
			}
			if (0 != memcmp(QOS_AP_11E_TO_DSCP_DEFAULT, AC_WLAN[wlanid]->qos_ap_11e_to_dscp, MAX_QOS_11E_VAL+1))
			{
				set_ap_bss_qos_map_default(wtpid, l_radioid, QOS_MAP_11E_TO_DSCP, wlanid);
			}
			if (0 != memcmp(QOS_AP_DSCP_TO_11E_DEFAULT, AC_WLAN[wlanid]->qos_ap_dscp_to_11e, MAX_QOS_DSCP_VAL+1))
			{
				set_ap_bss_qos_map_default(wtpid, l_radioid, QOS_MAP_DSCP_TO_11E, wlanid);
			}
			if (0 != memcmp(QOS_AP_1P_TO_11E_DEFAULT, AC_WLAN[wlanid]->qos_ap_1p_to_11e, MAX_QOS_1P_VAL+1))
			{
				set_ap_bss_qos_map_default(wtpid, l_radioid, QOS_MAP_1P_TO_11E, wlanid);
			}
			if (0 != AC_WLAN[wlanid]->qos_ap_11e_to_1p_switch)
			{
				set_ap_bss_qos_map_switch(wtpid, l_radioid, wlanid, QOS_MAP_11E_TO_1P, AC_WLAN[wlanid]->qos_ap_11e_to_1p_switch);
			}
			if (0 != AC_WLAN[wlanid]->qos_ap_11e_to_dscp_switch)
			{
				set_ap_bss_qos_map_switch(wtpid, l_radioid, wlanid, QOS_MAP_11E_TO_DSCP, AC_WLAN[wlanid]->qos_ap_11e_to_dscp_switch);
			}
			for(bind_wlan = 0; bind_wlan < 8; bind_wlan++)
    		{
    			if(AC_WTP[wtpid]->WTP_Radio[l_radioid]->cpe_intf[bind_wlan].vlan_count != 0)
    			{
    				wid_radio_set_cpe_channel(wtpid,l_radioid,bind_wlan);
    			}
    		}
		}

		/* reload RADIO's config */
		if(AC_WTP[wtpid]->WTP_Radio[l_radioid]->spec_analysis.enalbe)
		{
		//	set_ap_spectrum_analysis(radioid);
		}
		if(((AC_WTP[wtpid]->WTP_Radio[l_radioid]->Radio_Type&IEEE80211_11N)||(AC_WTP[wtpid]->WTP_Radio[l_radioid]->Radio_Type&IEEE80211_11AC))
    	    &&(AC_WTP[wtpid]->WTP_Radio[l_radioid]->cwmode != 0))
    	{
    		wid_radio_set_cmmode(wtpid*L_RADIO_NUM+l_radioid);
		}
	}

	/* WTP CONFIG */
	if(0 != AC_WTP[wtpid]->macgroupID)
	{
        set_ap_bss_black_white_list(wtpid, AC_WTP[wtpid]->macgroupID);
	}
	if (0 != AC_WTP[wtpid]->electronic_menu)
	{
	    set_ap_bss_local_survival_switch(wtpid, AC_WTP[wtpid]->electronic_menu);
	}

//	AC_WTP[wtpid]->load_config_flag = 1;	/* DONE */

	return 0;
}


int CWWTP_control_config(unsigned int wtpid)
{
	unsigned int i = 0;	
	struct msgqlist *elem = NULL;

	i = wtpid;

	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);

	if ((WID_RUN != AC_WTP[i]->WTPStat)
		|| (NULL == AC_WTP[i]->ControlList)
		|| (NULL != AC_WTP[i]->ControlWait))
	{
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"state %s ControlWait %p ControlList %p\n",
							WAI_FUNC_LINE_LITERAL, i, AC_WTP[i]->WTPNAME, \
							WAI_STATE_STR(AC_WTP[i]->WTPStat) , \
							AC_WTP[i]->ControlWait, AC_WTP[i]->ControlList);
		return 0;
	}

	/* wtp at run state and have config msg and no config request on the fly */	
	CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
	elem = NULL;
	elem = WID_GET_CONTROL_LIST_ELEM(i);
	CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
	
	if (elem == NULL)
	{
		return 0;
	}
	if ((elem->mqinfo.subtype == WTP_S_TYPE) && (elem->mqinfo.u.WtpInfo.Wtp_Op == WTP_REBOOT))
	{					
		if ((gWTPs[i].currentState != CW_QUIT) && (gWTPs[i].isRequestClose))
		{
			wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"be requested to reboot for table update\n", WAI_FUNC_LINE_LITERAL, i);
			_CWCloseThread(i);
		}
		CW_FREE_OBJECT(elem);
		return 0;
	}

	if ((AC_WTP[i]) && (AC_WTP[i]->ControlList == NULL))
	{
		 if ((AC_WTP[i]) && (AC_WTP[i]->sendsysstart == 0))
		 {
			if (elem->mqinfo.subtype == WTP_S_TYPE)
			{
				if ((elem->mqinfo.u.WtpInfo.Wtp_Op == WTP_EXTEND_CMD) && (memcmp(elem->mqinfo.u.WtpInfo.value,"sysstart",8) == 0))
				{
				}
				else
				{
					wid_syslog_debug_debug(WID_DEFAULT, "wlan cmd expand :sysstart (before)");
					char wlan_cmd_expand[DEFAULT_LEN] = "sysstart";
					struct msgqlist *elem1 = NULL;
					
					elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
					if (NULL == elem1)
					{			
						WID_MALLOC_ERR();			
						return -1;
					}			
					memset(elem1, 0, sizeof(struct msgqlist));
					elem1->mqinfo.WTPID = i;
					elem1->mqinfo.type = CONTROL_TYPE;
					elem1->mqinfo.subtype = WTP_S_TYPE;
					elem1->mqinfo.u.WtpInfo.Wtp_Op = WTP_EXTEND_CMD;
					memcpy(elem1->mqinfo.u.WtpInfo.value, wlan_cmd_expand, strlen(wlan_cmd_expand));
					
					WID_INSERT_CONTROL_LIST(i, elem1);
					wid_syslog_debug_debug(WID_DEFAULT,"wlan cmd expand :sysstart (after)");
					AC_WTP[i]->sendsysstart = 1;
				}
			}
			else
			{
				wid_syslog_debug_debug(WID_DEFAULT, "wlan cmd expand :sysstart (before)");
				char wlan_cmd_expand[DEFAULT_LEN] = "sysstart";
				struct msgqlist *elem1 = NULL;
								
				elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
				if (NULL == elem1)
				{			
					WID_MALLOC_ERR();			
					return -1;
				}			
				memset(elem1, 0, sizeof(struct msgqlist));
				elem1->mqinfo.WTPID = i;
				elem1->mqinfo.type = CONTROL_TYPE;
				elem1->mqinfo.subtype = WTP_S_TYPE;
				elem1->mqinfo.u.WtpInfo.Wtp_Op = WTP_EXTEND_CMD;
				memcpy(elem1->mqinfo.u.WtpInfo.value, wlan_cmd_expand, strlen(wlan_cmd_expand));
				
				WID_INSERT_CONTROL_LIST(i, elem1);
				wid_syslog_debug_debug(WID_DEFAULT,"wlan cmd expand :sysstart (after)");
				AC_WTP[i]->sendsysstart = 1;
			}
		}
	}
	
	if (elem->mqinfo.subtype == WLAN_S_TYPE)
	{			
		CWThreadMutexLock(&gWTPs[i].interfaceMutex);
		int seqNum = CWGetSeqNum();
		
		CWBool bResult = CW_FALSE;

		if (CWBindingAssembleWlanConfigurationRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
		{
			if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_RESPONSE, seqNum)) 
			{
				bResult = CW_TRUE;
			}
			else
			{
				CWACStopRetransmission(i);
			}
		}
		
		gWTPs[i].interfaceCommand = NO_CMD;
		
		if (bResult)
		{
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			if (NULL != AC_WTP[i])
			{
				CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
			}
			AC_WTP[i]->ControlWait = elem;
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
		}
		else
		{					
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			//WID_INSERT_CONTROL_LIST(i, elem);
			if ((AC_WTP[i])&&(AC_WTP[i]->ControlWait != NULL))
			{
				wid_syslog_debug_debug(WID_WTPINFO,"wtp%d WLAN op something wrong (subtype)%d\n",i,AC_WTP[i]->ControlWait->mqinfo.subtype);

				CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
			}
			CW_FREE_OBJECT(elem);
			
			wid_syslog_err("%s-%d wtp%d Error sending command.(WLAN_S_TYPE)\n", __func__, __LINE__, i);
			
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
		}		
		CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
	}
	else if ((elem->mqinfo.subtype == Radio_S_TYPE))
	{	
		wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"***** WTP %d CONFIGURE_UPDATE_RESPONSE_Radio*****\n",WAI_FUNC_LINE_LITERAL,i);
		CWThreadMutexLock(&gWTPs[i].interfaceMutex);
		int seqNum = CWGetSeqNum();
		CWBool bResult = CW_FALSE;

		if (CWAssembleConfigurationUpdateRequest_Radio(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
		{
			if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum))
			{
				bResult = CW_TRUE;
			}
			else
			{
				CWACStopRetransmission(i);
			}
		}				
		if (bResult)
		{
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			if (NULL != AC_WTP[i])
			{
				CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
			}
			AC_WTP[i]->ControlWait = elem;
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
		}
		else
		{					
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			//WID_INSERT_CONTROL_LIST(i, elem);
			if (NULL != AC_WTP[i])
			{
				CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
			}
			CW_FREE_OBJECT(elem);
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
			
			wid_syslog_err("%s:%d wtp%d Error sending command.(Radio_S_TYPE)\n", __func__, __LINE__, i);
		}
		CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
	}
	else if ((elem->mqinfo.subtype == STA_S_TYPE))
	{
		if (elem->mqinfo.u.StaInfo.Sta_Op == Sta_SETKEY)
		{
			wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"***** WTP %d STATION_KEY_CONFIGURATION_RESPONSE*****\n",WAI_FUNC_LINE_LITERAL,i);
			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			int seqNum = CWGetSeqNum(); 			
			CWBool bResult = CW_FALSE;
	
			if (CWAssembleStaConfigurationRequest_key(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
			{
				if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE, seqNum)) 
				{
					bResult = CW_TRUE;
				}
				else
				{
					CWACStopRetransmission(i);
				}
			}							
			if (bResult)
			{
				wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"wtp%d sta key op successful", WAI_FUNC_LINE_LITERAL, i);
				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
				if ((NULL != AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
				{
					wid_syslog_debug_debug(WID_DEFAULT,WAI_FUNC_LINE_FMT"wtp%d sta key op something wrong (subtype)%d\n", WAI_FUNC_LINE_LITERAL, i, AC_WTP[i]->ControlWait->mqinfo.subtype);
					CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
				}
				AC_WTP[i]->ControlWait = elem;
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
			}
			else
			{					
				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
				//WID_INSERT_CONTROL_LIST(i, elem);
				if ((NULL != AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
				{
					AsdWsm_StationOpNew(i,elem->mqinfo.u.StaInfo.STAMAC,STA_LEAVE,1); //weichao add
					CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
				}
				CW_FREE_OBJECT(elem);
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
				
				wid_syslog_err("%s-%d wtp%d Error sending command.(STA_S_TYPE:Sta_SETKEY)\n", __func__, __LINE__, i);
			}						
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
		}				
		else if (elem->mqinfo.u.StaInfo.Sta_Op == Sta_SETPMK)
		{
			wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"***** WTP %d STATION_PMK_CONFIGURATION_RESPONSE*****\n",WAI_FUNC_LINE_LITERAL,i);
			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			int seqNum = CWGetSeqNum();
			
			CWBool bResult = CW_FALSE;
	
			if (CWAssembleStaConfigurationRequest_pmk(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
			{
				if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE, seqNum)) 
				{
					bResult = CW_TRUE;
				}
				else
				{
					CWACStopRetransmission(i);
				}
			}			
			if (bResult)
			{
				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
				if ((NULL != AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
				{
					wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"wtp%d sta pmk op something wrong (subtype)%d\n", WAI_FUNC_LINE_LITERAL,i,AC_WTP[i]->ControlWait->mqinfo.subtype);
					CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
				}
				AC_WTP[i]->ControlWait = elem;
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
			}
			else
			{					
				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
				if (NULL != AC_WTP[i])
				{
					CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
				}
				CW_FREE_OBJECT(elem);
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
				
				wid_syslog_err("%s-%d wtp%d Error sending command.(STA_S_TYPE:Sta_SETPMK)\n", __func__, __LINE__, i);
			}						
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
		}
		else
		{
			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			int seqNum = CWGetSeqNum();
			
			CWBool bResult = CW_FALSE;
	
			if (CWAssembleStaConfigurationRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
			{
				if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE, seqNum)) 
				{
					bResult = CW_TRUE;
				}
				else
				{
					CWACStopRetransmission(i);
				}
			}			
			if (bResult)
			{
				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));

				if ((NULL != AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
				{
					wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"wtp%d sta op something wrong (subtype)%d\n",WAI_FUNC_LINE_LITERAL,i,AC_WTP[i]->ControlWait->mqinfo.subtype);
					free(AC_WTP[i]->ControlWait);
					AC_WTP[i]->ControlWait = NULL;
				}
				AC_WTP[i]->ControlWait = elem;
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
			}
			else
			{					
				CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
				//WID_INSERT_CONTROL_LIST(i, elem);
				if ((NULL != AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
				{
					AsdWsm_StationOpNew(i,elem->mqinfo.u.StaInfo.STAMAC, STA_LEAVE, 1);
					free(AC_WTP[i]->ControlWait);
					AC_WTP[i]->ControlWait = NULL;
				}
				CW_FREE_OBJECT(elem);
				CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
				wid_syslog_err("%s-%d wtp%d Error sending command.(STA_S_TYPE)\n", __func__, __LINE__, i);
			}						
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
		}
	}
	else if (elem->mqinfo.subtype == WTP_S_TYPE)
	{	
		CWThreadMutexLock(&gWTPs[i].interfaceMutex);
		int seqNum = CWGetSeqNum();
		CWBool bResult = CW_FALSE;

		if (CWAssembleConfigurationUpdateRequest_WTP(&(gWTPs[i].messages), &(gWTPs[i].messagesCount), gWTPs[i].pathMTU, seqNum, i, elem))
		{
			if (CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum)) 
			{
				bResult = CW_TRUE;
			}
			else
			{
				CWACStopRetransmission(i);
			}
		}
		
		//AC_WTP[i]->CMD->setCMD = 0;
		//AC_WTP[i]->WTP_Radio[j]->CMD = 0x0;				
		//gWTPs[i].interfaceCommand = NO_CMD;

		if (bResult)
		{
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			if ((NULL != AC_WTP[i]) && (AC_WTP[i]->ControlWait != NULL))
			{
				wid_syslog_debug_debug(WID_DEFAULT,WAI_FUNC_LINE_FMT"wtp%d op something wrong (subtype)%d\n",WAI_FUNC_LINE_LITERAL,i,AC_WTP[i]->ControlWait->mqinfo.subtype);
				free(AC_WTP[i]->ControlWait);
				AC_WTP[i]->ControlWait = NULL;
			}
			AC_WTP[i]->ControlWait = elem;
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
		}
		else
		{					
			CWThreadMutexLock(&(gWTPs[i].WTPThreadControllistMutex));
			//WID_INSERT_CONTROL_LIST(i, elem);
			if (NULL != AC_WTP[i])
			{
				CW_FREE_OBJECT(AC_WTP[i]->ControlWait);
			}
			CW_FREE_OBJECT(elem);
			CWThreadMutexUnlock(&(gWTPs[i].WTPThreadControllistMutex));
			wid_syslog_err("%s-%d wtp%d Error sending command.(WTP_S_TYPE)\n", __func__, __LINE__, i);
		}
		//}
		CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
	}
	else if ((elem->mqinfo.subtype == WDS_S_TYPE))
	{
		CWThreadMutexLock(&gWTPs[i].interfaceMutex);
		wid_set_wds_state(i,elem->mqinfo.u.WlanInfo.Radio_L_ID,elem->mqinfo.u.WlanInfo.WLANID,elem->mqinfo.u.WlanInfo.Wlan_Op);
		CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
		CW_FREE_OBJECT(elem);
	}

	return 0;
}

