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
* ACRunstate.c
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

#include <netinet/in.h>

#include "CWAC.h"
#include "CWProtocol.h"
#include "wcpss/waw.h"

#include "wcpss/wid/WID.h"
#include "dbus/asd/ASDDbusDef.h"
#include "ACDbus.h"
#include "ACDbus_handler.h"
#include "hmd/hmdpub.h"
#include "ACBak.h"
#include "AC.h"
#include "CWConfigFile.h"
#include "CWCommon.h"
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif


CWBool CWParseConfigurationUpdateResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode,CWRadioOperationalInfoValues* radioOpeinfo,int wtpindex);
CWBool CWSaveConfigurationUpdateResponseMessage(CWProtocolResultCode resultCode, int WTPIndex);

CWBool CWParseWTPEventRequestMessage(CWProtocolMessage *msgPtr, int len, CWProtocolWTPEventRequestValues *valuesPtr,int wtpindex);
CWBool CWSaveWTPEventRequestMessage (CWProtocolWTPEventRequestValues *WTPEventRequest, CWWTPProtocolManager *WTPProtocolManager);
CWBool CWAssembleWTPEventResponse (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex);

CWBool CWParseChangeStateEventRequestMessage2
(
	CWProtocolMessage *msgPtr, 
	int len, 
	CWProtocolChangeStateEventRequestValues **valuesPtr,
	int WTPIndex
);

CWBool CWParseEchoRequestMessage (CWProtocolMessage *msgPtr, int len, int WTPIndex);
CWBool CWAssembleEchoResponse (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex);

CWBool CWStartNeighborDeadTimer(int WTPIndex);
CWBool CWStopNeighborDeadTimer(int WTPIndex);
CWBool CWRestartNeighborDeadTimer(int WTPIndex);
CWBool CWRestartNeighborDeadTimerForEcho(int WTPIndex);
CWBool CWParseWlanConfigurationResponseMessage
(
    CWProtocolMessage* msgPtr,
    int len,
    CWProtocolResultCode *resultCode,
    WID_BSS *BSS,
    unsigned char *bss_num
);

CWBool CWParseStaConfigurationResponseMessage(CWProtocolMessage* msgPtr, int len, int WTPIndex,CWProtocolResultCode* resultCode);
CWBool CWParseDiscoveryRequestMessageInRun
(
	CWProtocolMessage* msgPtr,
	int len,
	CWDiscoveryRequestValues *valuesPtr,
	WTPLoginState *wtp_constat
);
void CWDestroyDiscoveryRequestValues(CWDiscoveryRequestValues *valPtr);
void wid_hex_dump(unsigned char *buffer, int buffLen);
	//int CreateWlan = 1;
static const int gCWIANATimes256_2 = CW_IANA_ENTERPRISE_NUMBER * 256;
unsigned int RadioTXPOF;

extern     WID_CONFSCHEME *AC_CONFSCHME[CONFSCHEME_NUM];

CWBool ACEnterRun(int WTPIndex, CWProtocolMessage *msgPtr, CWBool dataFlag)
{
	CWBool toSend = CW_FALSE;
	CWBool reset = CW_FALSE;
	CWControlHeaderValues controlVal;
	CWProtocolMessage *messages = NULL;
	int messagesCount = 0;
	msgPtr->offset = 0;
	char i = 0, result = 0;
	WTPLoginState wtp_constat = WID_LOGIN;
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	if (dataFlag)
	{
		//## We have received a Data Message... now just log this event
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter run recv capwap data packet we drop it\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME);
		return CW_TRUE;
	}

	if (!(CWACParseGenericRunMessage(WTPIndex, msgPtr, &controlVal)))
	{
		//## Two possible errors: WRONG_ARG and INVALID_FORMAT
		//## In the second case we have an unexpected response: ignore the
		//## message and log the event.
		return CW_FALSE;
	}
	if (!check_wtpid_func(WTPIndex))
	{
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"enter run recv packet found id illegal we drop it\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex);
		return CW_FALSE;
	}
	
	for (i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
	{
		if (gConfigVersionUpdateInfo[i] != NULL)
		{
			result = 1;
			break;
		}
	}
	if ((result != 0) && (find_in_wtp_list(WTPIndex) == CW_TRUE))
	{
		delete_wtp_list(WTPIndex);
		update_next_wtp();
		
		if (updatewtplist == NULL)
		{
			/*mahz modified to match ap upgrade automatically*/
			for (i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
			{
				CWConfigVersionInfo *tmp_node = gConfigVersionUpdateInfo[i];
				while (tmp_node != NULL)
				{
					CWConfigVersionInfo *free_node = tmp_node;
					tmp_node = tmp_node->next;

					CW_FREE_OBJECT(free_node->str_ap_model);		
					CW_FREE_OBJECT(free_node->str_ap_version_name); 	
					CW_FREE_OBJECT(free_node->str_ap_version_path); 	
					CW_FREE_OBJECT(free_node->str_ap_code); 	
					CW_FREE_OBJECT(free_node);		
				}
				gConfigVersionUpdateInfo[i] = NULL;
			}

			gupdateControl = 0;
			checkwtpcount = 0;
			destroy_wtp_list();
		}
	}

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter run recv message %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(controlVal.messageTypeValue));
	switch (controlVal.messageTypeValue)
	{
		case CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE:
		{
			CWProtocolResultCode resultCode;
			CWRadioOperationalInfoValues radioOpeinfo;
			memset(&resultCode, 0, sizeof(CWProtocolResultCode));
			memset(&radioOpeinfo, 0, sizeof(CWRadioOperationalInfoValues));
			if (!(CWParseConfigurationUpdateResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode,&radioOpeinfo,WTPIndex)))
			{
				return CW_FALSE;
			}
			
			CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
			CWACStopRetransmission(WTPIndex);
			CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
			
			if (!CWRestartNeighborDeadTimer(WTPIndex))
			{
				_CWCloseThread(WTPIndex);
			}
			CWSaveConfigurationUpdateResponseMessage(resultCode, WTPIndex);
			
			CWThreadMutexLock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
			if ((AC_WTP[WTPIndex]->ControlWait != NULL)
				&& ((AC_WTP[WTPIndex]->ControlWait->mqinfo.subtype == Radio_S_TYPE)
					|| (AC_WTP[WTPIndex]->ControlWait->mqinfo.subtype == WTP_S_TYPE)))
			{
				if ((AC_WTP[WTPIndex]->ControlWait->mqinfo.subtype == Radio_S_TYPE))
				{
					/*resultcode parse*/
					/*0++++++7+++11+++15++++20++++++++++31*/
					/*-----type-----+-l_rd-+---------value---------*/
					/*++++++++++++++++++++++++++++++++++*/
					int radioID = 0;
					unsigned int result_l_radioid = 0;
					unsigned int txpowerof_or_channel_state = 0;
					txpowerof_or_channel_state = resultCode>>20;/*type*/

					wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d resultcode %d txpowerof_or_channel_state %d\n",
					                        __func__, __LINE__, WTPIndex, resultCode, txpowerof_or_channel_state);
					if (resultCode != 0)
					{ 
						if (((txpowerof_or_channel_state == 0)||(txpowerof_or_channel_state == 3))
						    && (AC_WTP[WTPIndex]->ControlWait->mqinfo.u.RadioInfo.Radio_Op == Radio_TXPOF))
						{
							/*txpoweroffset*/
							/*please delete (txpowerof_or_channel_state == 0) from if(), when old ap version is not used */
							/*(txpowerof_or_channel_state == 0)  old ap version use*/
							/*(txpowerof_or_channel_state == 3)  new ap verison use*/
							result_l_radioid = (resultCode>>16)&0x000f;/*l_rd*/
							radioID = WTPIndex * 4 + result_l_radioid;
							if (!check_l_radioid_func(result_l_radioid))
							{
								wid_syslog_err("%s result_l_radioid %d\n",__func__,result_l_radioid);
								CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
								return CW_FALSE;
							}
							if (AC_RADIO[radioID] != NULL)
							{
								int radio_txpof = 0x00ff&resultCode;
								wid_syslog_debug_debug(WID_DEFAULT,"%s:%d radio%u-%u result txpoweroffset= %u\n",__func__,__LINE__,WTPIndex,result_l_radioid,radio_txpof);
								AC_RADIO[radioID]->Radio_TXPOF = radio_txpof;
							}					
						}
						else if ((txpowerof_or_channel_state == 1)
						        && (AC_WTP[WTPIndex]->ControlWait->mqinfo.u.RadioInfo.Radio_Op == Radio_Channel))
						{
							int channel = 0x00ff&resultCode;
							result_l_radioid=(resultCode>>16)&0x000f;/*l_rd*/
							if (!check_l_radioid_func(result_l_radioid))
							{
								wid_syslog_err("%s:%d Radio_Op= Radio_Channel radioid= %d\n",__func__,__LINE__,result_l_radioid);
								CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
								return CW_FALSE;
							}
							
							radioID = WTPIndex * 4 + result_l_radioid;
							wid_syslog_debug_debug(WID_DEFAULT,"%s:%d radio%u-%u result channel= %u\n",__func__,__LINE__,WTPIndex,result_l_radioid,channel);
							if (AC_RADIO[radioID] != NULL)
							{
								AC_RADIO[radioID]->Radio_Chan = channel;
							}
						}
						else if ((txpowerof_or_channel_state == 2)
						        && (AC_WTP[WTPIndex]->ControlWait->mqinfo.u.RadioInfo.Radio_Op == Radio_TXP))
						{
							int txpower = 0x00ff&resultCode;
							result_l_radioid = (resultCode>>16)&0x000f;/*l_rd*/
							if (!check_l_radioid_func(result_l_radioid))
							{
								wid_syslog_err("%s:%d Radio_Op = Radio_TXP radioid= %d\n",__func__,__LINE__,result_l_radioid);
								CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
								return CW_FALSE;
							}
							radioID = WTPIndex * 4 + result_l_radioid;
							wid_syslog_debug_debug(WID_DEFAULT,"%s:%d radio%u-%u result txpower: %u\n",__func__,__LINE__,WTPIndex,result_l_radioid, txpower);
							if (AC_RADIO[radioID] != NULL)
							{
								AC_RADIO[radioID]->Radio_TXP = txpower;
							}
						}
					}
				}
				else
				{
				    if ((1 == gapscanset.opstate) && (1 == gapscanset.countermeasures_switch))
				    {
    				    int j = 0;
                        CWThreadMutexLock(&(gWTPs[WTPIndex].RRMThreadMutex));
    				    for (j = 0; j < L_RADIO_NUM && j < AC_WTP[WTPIndex]->RadioCount; j++)
                        {
                            if ((NULL != AC_WTP[WTPIndex]->rouge_ap_infos[j])
                                && (wid_check_rogue_ap_increase_state(WTPIndex,j))
                                && (0 != AC_WTP[WTPIndex]->rouge_ap_infos[j]->checkrogueapcount))
                            {
                                wid_syslog_info("%s: radio%u-%u checkrogueap %d\n",
                                                __func__,WTPIndex,j,AC_WTP[WTPIndex]->rouge_ap_infos[j]->checkrogueapcount);
                                wid_disturb_countermeasure_rogue_ap(WTPIndex,j,ROGUE_ADD);
                            }
                        }
                        
                        CWThreadMutexUnlock(&(gWTPs[WTPIndex].RRMThreadMutex));
                    }
				}
			}
			else
			{			
				wid_syslog_info("wtp%d config update response something wrong\n",WTPIndex);
			}
			
			reset = CW_FALSE;
			if (AC_WTP[WTPIndex]->resetflag == 1)
			{
				reset = CW_TRUE;
			}
			
			CW_FREE_OBJECT(AC_WTP[WTPIndex]->ControlWait);              
			CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
			break;
		}
		case CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST:
		{
			CWProtocolChangeStateEventRequestValues *valuesPtr = NULL;
			if (!(CWParseChangeStateEventRequestMessage2(msgPtr, controlVal.msgElemsLen, &valuesPtr, WTPIndex)))
			{
				CW_FREE_OBJECT(valuesPtr);
				return CW_FALSE;
			}
			if (!CWRestartNeighborDeadTimer(WTPIndex))
			{
				_CWCloseThread(WTPIndex);
			}
			if (!(CWSaveChangeStateEventRequestMessage(valuesPtr, &(gWTPs[WTPIndex].WTPProtocolManager))))
			{
				return CW_FALSE;
			}
			if (!(CWAssembleChangeStateEventResponse(&messages, &messagesCount, gWTPs[WTPIndex].pathMTU, controlVal.seqNum,WTPIndex))) 
			{
				return CW_FALSE;
			}
			toSend = CW_TRUE;
			break;
		}
		case CW_MSG_TYPE_VALUE_ECHO_REQUEST:
		{
			if (!(CWParseEchoRequestMessage(msgPtr, controlVal.msgElemsLen,WTPIndex)))
			{
				return CW_FALSE;
			}
			if (!CWRestartNeighborDeadTimer(WTPIndex))			/*fengwenchao modify begin 20111117 for GM-3*/
			{
				_CWCloseThread(WTPIndex);
			}
			if (!(CWAssembleEchoResponse(&messages, &messagesCount, gWTPs[WTPIndex].pathMTU, controlVal.seqNum, WTPIndex))) 
			{
				return CW_FALSE;
			}
			else
			{
				if ((AC_WTP[WTPIndex]) && (AC_WTP[WTPIndex]->heart_time.heart_statistics_switch == 1))
				{
					wid_prase_heart_time_avarge(WTPIndex);
				}
			}
			/*fengwenchao modify  end*/	
			toSend = CW_TRUE;
			break;
		}
		/*case CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE:
			CWACStopRetransmission(WTPIndex);
			CWRestartNeighborDeadTimer(WTPIndex);
			break;
		case CW_MSG_TYPE_VALUE_STATUS_CONFIGURATION_RESPONSE:
			CWACStopRetransmission(WTPIndex);
			CWRestartNeighborDeadTimer(WTPIndex);
			break;
		case CW_MSG_TYPE_VALUE_DATA_TRANSFER_REQUEST:
			CWRestartNeighborDeadTimer(WTPIndex);
			break;
		*/
		case CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST:
		{
			CWProtocolWTPEventRequestValues valuesPtr;
			memset(&valuesPtr, 0, sizeof(CWProtocolWTPEventRequestValues));
			int eth_num = 0;
			
			if (!(CWParseWTPEventRequestMessage(msgPtr, controlVal.msgElemsLen, &valuesPtr, WTPIndex)))
			{
				return CW_FALSE;
			}
			//trap information
			if ((valuesPtr.WTPOperationalStatisticsCount != 0) && (valuesPtr.WTPOperationalStatistics[0].ElectrifyRegisterCircle != 0))
			{
				AC_WTP[WTPIndex]->ElectrifyRegisterCircle = valuesPtr.WTPOperationalStatistics[0].ElectrifyRegisterCircle;
				
				if (gtrapflag >= 4)
				{
		#if NOSOFTAC
					wid_dbus_trap_wtp_electrify_register_circle(WTPIndex, valuesPtr.WTPOperationalStatistics[0].ElectrifyRegisterCircle);
		#endif
				}	
			}
			if (valuesPtr.WTPOperationalStatisticsCount != 0)
			{
				if ((gtrapflag >= 1) && (valuesPtr.WTPOperationalStatistics[0].ColdStart == 1))
				{
				#if NOSOFTAC
					wid_dbus_trap_wtp_code_start(WTPIndex);
				#endif
				}
				AC_WTP[WTPIndex]->ap_mask_new = valuesPtr.WTPOperationalStatistics[0].ipmask;	
				AC_WTP[WTPIndex]->ap_gateway = valuesPtr.WTPOperationalStatistics[0].ipgateway;
				AC_WTP[WTPIndex]->ap_dnsfirst = valuesPtr.WTPOperationalStatistics[0].ipdnsfirst;
				AC_WTP[WTPIndex]->ap_dnssecend = valuesPtr.WTPOperationalStatistics[0].ipdnssecend;
			 	memcpy(AC_WTP[WTPIndex]->cpuType,valuesPtr.WTPOperationalStatistics[0].cpuType,WTP_TYPE_DEFAULT_LEN);
			 	memcpy(AC_WTP[WTPIndex]->flashType,valuesPtr.WTPOperationalStatistics[0].flashType,WTP_TYPE_DEFAULT_LEN);
			 	memcpy(AC_WTP[WTPIndex]->memType,valuesPtr.WTPOperationalStatistics[0].memType,WTP_TYPE_DEFAULT_LEN);				
				wid_syslog_debug_debug(WID_DEFAULT,"%s: wtp%d apifinfo eth_rate %d\n",
							__func__,WTPIndex, valuesPtr.WTPOperationalStatistics[0].eth_rate);
				if ((10 == valuesPtr.WTPOperationalStatistics[0].eth_rate)
					|| (100 == valuesPtr.WTPOperationalStatistics[0].eth_rate)
					|| (1000 == valuesPtr.WTPOperationalStatistics[0].eth_rate))
				{
					for (eth_num = 0; eth_num < valuesPtr.WTPOperationalStatistics[0].eth_count; eth_num++)
					{
						AC_WTP[WTPIndex]->apifinfo.eth[eth_num].eth_rate = valuesPtr.WTPOperationalStatistics[0].eth_rate;
					}
				}
			}

			if (!CWRestartNeighborDeadTimer(WTPIndex))
			{
				_CWCloseThread(WTPIndex);
			}
			if (!(CWSaveWTPEventRequestMessage(&valuesPtr, &(gWTPs[WTPIndex].WTPProtocolManager))))
			{
				return CW_FALSE;
			}
			if (!(CWAssembleWTPEventResponse(&messages, &messagesCount, gWTPs[WTPIndex].pathMTU, controlVal.seqNum, WTPIndex)))
			{
 				return CW_FALSE;
 			}
 			
			toSend = CW_TRUE;	
			
			if (valuesPtr.CWStationInfo != NULL)
			{
				AsdWsm_StationOp(WTPIndex,(valuesPtr.CWStationInfo),WID_DEL);
				CW_FREE_OBJECT(valuesPtr.CWStationInfo->mac_addr);
				CW_FREE_OBJECT(valuesPtr.CWStationInfo);
			}
			
			if (valuesPtr.wids_device_infos != NULL)
			{
				if (AC_WTP[WTPIndex]->wids_device_list == NULL)
				{
					CWThreadMutexLock(&(gWTPs[WTPIndex].WIDSThreadMutex));
					AC_WTP[WTPIndex]->wids_device_list = valuesPtr.wids_device_infos;
					
					valuesPtr.wids_device_infos = NULL;
					CWThreadMutexUnlock(&(gWTPs[WTPIndex].WIDSThreadMutex));
				}
				else
				{
					CWThreadMutexLock(&(gWTPs[WTPIndex].WIDSThreadMutex));
					merge_wids_list((AC_WTP[WTPIndex]->wids_device_list),&(valuesPtr.wids_device_infos),WTPIndex);
					CWThreadMutexUnlock(&(gWTPs[WTPIndex].WIDSThreadMutex));
				}
			}
			
			if (valuesPtr.ApReportStaInfo != NULL)
            {
				AsdWsm_ap_report_sta_info(WTPIndex,(valuesPtr.ApReportStaInfo),valuesPtr.ApReportStaInfo->op);
				//CW_FREE_OBJECT(valuesPtr.ApReportStaInfo->mac);
				CW_FREE_OBJECT(valuesPtr.ApReportStaInfo);
			}
			break;
		}
		case CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_RESPONSE:
		{
			wid_syslog_debug_debug(WID_DEFAULT,"wtp%d receive WLAN CONFIGURATION RESPONSE\n", WTPIndex);
			int j = 0;
			unsigned char radio_num = 0;
			CWProtocolResultCode resultCode[L_RADIO_NUM];
			WID_BSS BSS[L_RADIO_NUM];
            memset(resultCode, 0, L_RADIO_NUM*(sizeof(CWProtocolResultCode)));
            memset(BSS, 0, L_RADIO_NUM*(sizeof(WID_BSS)));
			
			if (!(CWParseWlanConfigurationResponseMessage(msgPtr,controlVal.msgElemsLen,resultCode,BSS,&radio_num)))
			{
			    return CW_FALSE;
			}
			
			for (j = 0; j < radio_num; j++)
			{
    			unsigned char BSSID[MAC_LEN];
    			memset(BSSID, 0, MAC_LEN);
    			unsigned int bssindex = 0;
    			int zero = memcmp(BSS[j].BSSID, BSSID, MAC_LEN);
    			
    			if (zero != 0)
    			{
    				wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d radio%d wlan%d BSSID:"MACSTR"\n",
    				                        __func__,__LINE__,WTPIndex,BSS[j].Radio_L_ID,BSS[j].WlanID,MAC2STR(BSS[j].BSSID));
    				int i1 = 0;
    				if (!check_wlanid_func(BSS[j].WlanID))
    				{
    					wid_syslog_err("%s BSS[%d].WlanID = %d\n",__func__,j,BSS[j].WlanID);
    					//return CW_FALSE;
    				}
    				if (!check_l_radioid_func(BSS[j].Radio_L_ID))
    				{
    					wid_syslog_err("%s BSS[%d].Radio_L_ID = %d\n",__func__,j,BSS[j].Radio_L_ID);
    					//return CW_FALSE;
    				}
    				if ((BSS[j].Radio_L_ID < L_RADIO_NUM)
    				    && (AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID] != NULL)
    				    && (check_wlanid_func(BSS[j].WlanID)))
    				{
    				    for (i1 = 0; i1 < L_BSS_NUM; i1++)
    				    {
    				 	    if ((AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1] == NULL))
    						{
    						    continue;
    					    }
    					    else if (AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->WlanID == BSS[j].WlanID)
    					    {
        						memcpy(AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSID, BSS[j].BSSID, MAC_LEN);
        						AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->State = 1;
								//AsdWsm_BSSOp(AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSIndex, WID_ADD, 1);
        						bssindex = AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSIndex;
        						if (!check_bssid_func(bssindex))
        						{
        							continue;
        						}
#if NOSOFTAC
        						wid_asd_bss_traffic_limit(bssindex);
#endif
    							if (AC_BSS[bssindex]->BSS_TUNNEL_POLICY == CW_802_IPIP_TUNNEL)
        						{
        							add_ipip_tunnel(bssindex);
        						}
        						wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d radio%d wlan%d BSSID:"MACSTR"\n",
                    						            __func__,__LINE__,WTPIndex,BSS[j].Radio_L_ID,BSS[j].WlanID,
                    						            MAC2STR(AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSID));
        						CWThreadMutexLock(&MasterBak);
        						/*ht changed for update bss info in local mode*/
        						if (1)//((AC_BSS[bssindex]->BSS_IF_POLICY == BSS_INTERFACE)||(AC_BSS[bssindex]->BSS_IF_POLICY == WLAN_INTERFACE))
        						{
        							struct sockaddr *sa = NULL;
        							struct sockaddr* sa2 = NULL;
        							//int ret = -1;	
        							unsigned char wlan_id = 0;
        							IF_info ifinfo;
#if NOSOFTAC
        							int fd = open("/dev/wifi0", O_RDWR);
        							wid_syslog_debug_debug(WID_DEFAULT,"***%s:%d fd:%d ***\n",__func__,__LINE__,fd);

        							if (fd < 0)
        							{
        								CWThreadMutexUnlock(&MasterBak);
        								int k = 0;
										for (k = 0; k < radio_num; k++)
                            			{
                                            CW_FREE_OBJECT(BSS[k].BSSID);
                            			}
        								return -1;//create failure
        							}
#endif			
        							memset(&ifinfo, 0, sizeof(IF_info));
        							sa = (struct sockaddr *)&gWTPs[WTPIndex].address;
        							sa2 = (struct sockaddr*)&(gInterfaces[gWTPs[WTPIndex].interfaceIndex].addr);
        							ifinfo.acport = htons(5247);
        							ifinfo.BSSIndex = bssindex;
        							ifinfo.WLANID = AC_BSS[bssindex]->WlanID;
        							wlan_id = AC_BSS[bssindex]->WlanID; 
        							if ((AC_WLAN[wlan_id] != NULL)
        							    && (AC_WLAN[wlan_id]->SecurityType == OPEN)
        							    && (AC_WLAN[wlan_id]->EncryptionType == NONE))
        							{
        							    ifinfo.protect_type = 0;
        							}
        							else
        							{
        							    ifinfo.protect_type = 1;
        							}
        							ifinfo.vrid = local*MAX_INSTANCE+ vrrid;
        							if (AC_BSS[bssindex]->BSS_TUNNEL_POLICY == CW_802_DOT_3_TUNNEL)
        							{
        								ifinfo.f802_3 = 1;
        								wid_syslog_info("%s: ifinfo.f802_3 = %d.\n",__func__,ifinfo.f802_3);
        							}
        							else
        							{
        								ifinfo.f802_3 = 0;
        								wid_syslog_info("%s: AC_BSS[%d]->BSS_TUNNEL_POLICY =%d, not dot3,set ifinfo.f802_3 = %d.\n",
        								                __func__,bssindex,AC_BSS[bssindex]->BSS_TUNNEL_POLICY,ifinfo.f802_3);
        							}
        							ifinfo.wsmswitch = wsmswitch;
        							ifinfo.vlanSwitch = vlanSwitch;
        							if ((AC_BSS[bssindex]->BSS_IF_POLICY == BSS_INTERFACE)
        							    || (AC_BSS[bssindex]->BSS_IF_POLICY == WLAN_INTERFACE))
        							{
        							    ifinfo.if_policy = 1;
        							}
        							else
        							{
        							    ifinfo.if_policy = 0;
        							}
        							
        							if (AC_BSS[bssindex]->vlanid != 0)
        							{
        								ifinfo.vlanid = AC_BSS[bssindex]->vlanid;
        							}
        							else if (AC_BSS[bssindex]->wlan_vlanid != 0)
        							{
        								ifinfo.vlanid = AC_BSS[bssindex]->wlan_vlanid;
        							}
        							else
        							{
        								ifinfo.vlanid = 0;
        							}
#if DYNAMIC_VLAN_SUPPORT
        							ifinfo.dyvlanid = AC_BSS[bssindex]->dyvlanid;
        							wid_syslog_debug_debug(WID_DEFAULT, "%s:%d bss%d "MACSTR" dyvlanid %d to ifinfo.dyvlanid %d", 
											__func__,__LINE__,bssindex, MAC2STR(AC_BSS[bssindex]->BSSID),AC_BSS[bssindex]->dyvlanid, ifinfo.dyvlanid);
#endif
        							memcpy(ifinfo.apmac, AC_WTP[WTPIndex]->WTPMAC, MAC_LEN);
        							memcpy(ifinfo.bssid, AC_BSS[bssindex]->BSSID, MAC_LEN);
        							memcpy(ifinfo.ifname, AC_WTP[WTPIndex]->BindingIFName, strlen(AC_WTP[WTPIndex]->BindingIFName));
        							memcpy(ifinfo.apname, AC_WTP[WTPIndex]->WTPNAME, strlen(AC_WTP[WTPIndex]->WTPNAME));
        							if (AC_WLAN[wlan_id] != NULL)
        							{
                                        if (NULL != AC_WLAN[wlan_id]->ESSID)
        								{
											memcpy(ifinfo.essid, AC_WLAN[wlan_id]->ESSID, strlen((char *)AC_WLAN[wlan_id]->ESSID));
        								}
        								ifinfo.Eap1XServerSwitch = AC_WLAN[wlan_id]->eap_mac_switch;
        								memset(ifinfo.Eap1XServerMac, 0, MAC_LEN);
        								memcpy(ifinfo.Eap1XServerMac, AC_WLAN[wlan_id]->eap_mac2, MAC_LEN);
        								wid_syslog_debug_debug(WID_DBUS,"%s:%d AC_WLAN[%d]->eap_mac2= "MACSTR"\n",__func__,__LINE__,wlan_id,MAC2STR(AC_WLAN[wlan_id]->eap_mac2));
#if WIFI_QOS_SUPPORT
        								memcpy(ifinfo.qos_11e_to_1p, AC_WLAN[wlan_id]->qos_ac_11e_to_1p, MAX_QOS_11E_VAL+1);
        								memcpy(ifinfo.qos_dscp_to_1p, AC_WLAN[wlan_id]->qos_ac_dscp_to_1p, MAX_QOS_DSCP_VAL+1);
        								memcpy(ifinfo.qos_dscp_to_dscp, AC_WLAN[wlan_id]->qos_ac_dscp_to_dscp, MAX_QOS_DSCP_VAL+1);
#endif	
        							}
        							
        							char __str[128];			
        							memset(__str, 0, 128);
        							char *str = "lo";
        							str = sock_ntop_r(((struct sockaddr*)&(gInterfaces[gWTPs[WTPIndex].interfaceIndex].addr)), __str);
        							wid_syslog_info("%s:%d wtp%d on Interface %s(%d)\n", __func__, __LINE__, WTPIndex, str, gWTPs[WTPIndex].interfaceIndex);

        							if (sa->sa_family != AF_INET6)
        							{										
        								ifinfo.isIPv6 = 0;
        								ifinfo.apip = ((struct sockaddr_in *)sa)->sin_addr.s_addr;
        								ifinfo.apport = ((struct sockaddr_in *)sa)->sin_port +1;
        								struct sockaddr_in	*sin = (struct sockaddr_in *) sa2;
        								unsigned int v_ip = sin->sin_addr.s_addr;
        								wid_syslog_info("%s: v_ip %d %d.%d.%d.%d.\n",
        								                __func__, v_ip, (v_ip >> 24) & 0xFF, (v_ip >> 16) & 0xFF,(v_ip >> 8) & 0xFF, v_ip & 0xFF);
        								ifinfo.acip = v_ip;
        							}
        							else
        							{
        								ifinfo.isIPv6 = 1;
        								memcpy(ifinfo.apipv6,&((struct sockaddr_in6 *) sa)->sin6_addr,sizeof(struct in6_addr));
        								ifinfo.apport = ((struct sockaddr_in6 *)sa)->sin6_port +1;
        								memcpy(ifinfo.acipv6,&((struct sockaddr_in6 *) sa2)->sin6_addr,sizeof(struct in6_addr));
        							}
#if NOSOFTAC
        							ret = ioctl(fd, WIFI_IOC_IF_UPDATE, &ifinfo);
        							
        							wid_syslog_debug_debug(WID_DEFAULT,"%s:%d WIFI_IOC_IF_UPDATE ret:%d\n",__func__,__LINE__,ret);
        							close(fd);
        							if (ret < 0)
        							{
        								CWThreadMutexUnlock(&MasterBak);
                                        int k = 0;
                                        for (k = 0; k < radio_num; k++)
                                        {
                                            CW_FREE_OBJECT(BSS[k].BSSID);
                                        }
        								return -1;
        							}
#endif
        						}
        						struct bak_sock *tmp = bak_list;
        						if (tmp != NULL)
        						{
        							bak_add_del_bss(tmp->sock,B_ADD,AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSIndex);
        						}
        						CWThreadMutexUnlock(&MasterBak);

        						if (gtrapflag >= 4)
    							{
#if NOSOFTAC
                                    int type = 2;//auto
                                    int flag = 1;//enable
    							    wid_dbus_trap_ap_ath_error(WTPIndex,BSS[j].Radio_L_ID,BSS[j].WlanID,type,flag);
#endif
    							}
    					    }
    				    }
    				}
    	        }
    		}
    		
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d Stop Retransmission Wlan Configuration Request",__func__,__LINE__,WTPIndex);
			CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
			CWACStopRetransmission(WTPIndex);
			CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
			
			if (!CWRestartNeighborDeadTimer(WTPIndex))
			{
				_CWCloseThread(WTPIndex);
			}
			if ((AC_WTP[WTPIndex]->ControlWait != NULL)
				&& (AC_WTP[WTPIndex]->ControlWait->mqinfo.subtype == WLAN_S_TYPE))
			{
				CWThreadMutexLock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
				CW_FREE_OBJECT(AC_WTP[WTPIndex]->ControlWait);
				CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
				wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d wlan response\n",__func__,__LINE__,WTPIndex);
			}
			else
			{		
				CWThreadMutexLock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
				CW_FREE_OBJECT(AC_WTP[WTPIndex]->ControlWait);
				CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
				wid_syslog_info("%s:%d wtp%d wlan response something wrong\n",__func__,__LINE__,WTPIndex);
			}
			
			for (j = 0; j < radio_num; j++)
    		{
    			unsigned char BSSID[MAC_LEN];
    			memset(BSSID, 0, MAC_LEN);
    			int zero = memcmp(BSS[j].BSSID, BSSID, MAC_LEN);
			
    			if (resultCode[j] == CW_PROTOCOL_SUCCESS)
    			{	
    				if ((zero != 0) && (BSS[j].Radio_L_ID < L_RADIO_NUM))
    				{
    					if ((check_l_radioid_func(BSS[j].Radio_L_ID))
    						&& (AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID] != NULL))
    					{
    						AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->AdStat = 1;
    						AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->OpStat = 1;
    						wid_syslog_debug_debug(WID_DEFAULT,"%s:%d radio%d-%d create wlan success\n",__func__,__LINE__,WTPIndex,BSS[j].Radio_L_ID);
    						wid_syslog_debug_debug(WID_DEFAULT,"wtp%d Enter Run State#####\n",WTPIndex);
    						
    						if (AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->channelsendtimes == 1)
    						{	
								int RadioID = 0;
								msgq msg;				
								struct msgqlist *elem1 = NULL;

								RadioID = WTPIndex*L_RADIO_NUM + BSS[j].Radio_L_ID ;	
								
								if (AC_RADIO[RadioID]->auto_channel_cont == 0)
								{
									AC_RADIO[RadioID]->Radio_Chan = 0;
									
									memset((char*)&msg, 0, sizeof(msg));
									msg.mqid = WTPIndex%THREAD_NUM+1;
									msg.mqinfo.WTPID = WTPIndex;
									msg.mqinfo.type = CONTROL_TYPE;
									msg.mqinfo.subtype = Radio_S_TYPE;
									msg.mqinfo.u.RadioInfo.Radio_Op = Radio_Channel;
									msg.mqinfo.u.RadioInfo.Radio_L_ID = RadioID%L_RADIO_NUM;
									msg.mqinfo.u.RadioInfo.Radio_G_ID = RadioID;

									elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
									if (elem1 == NULL)
									{
										wid_syslog_crit("%s malloc %s",__func__,strerror(errno));
										CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
										CWACStopRetransmission(WTPIndex);
										CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
										
                            			if (!CWRestartNeighborDeadTimer(WTPIndex))
                            			{
                            				_CWCloseThread(WTPIndex);
                            			}
                                        int k = 0;
                                        for(k = 0; k < radio_num; k++)
                                        {
                                            CW_FREE_OBJECT(BSS[k].BSSID);
                                        }
										return 0;
									}
									memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));
									elem1->next = NULL;
									memcpy((char*)&(elem1->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
									WID_INSERT_CONTROL_LIST(WTPIndex, elem1);
								}
							
    							AC_WTP[WTPIndex]->WTP_Radio[BSS[j].Radio_L_ID]->channelsendtimes--;
    						}
    					}
    				}
    			}
    			
                CW_FREE_OBJECT(BSS[j].BSSID);
    		}
			
			break;
		}
		case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE:
		{			
			wid_syslog_debug_debug(WID_DEFAULT,"CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE\n");
			CWProtocolResultCode resultCode;
			if (!(CWParseStaConfigurationResponseMessage(msgPtr, controlVal.msgElemsLen,WTPIndex, &resultCode)))
			{
				return CW_FALSE;
			}
			if (resultCode == CW_PROTOCOL_SUCCESS)
			{
				wid_syslog_debug_debug(WID_DEFAULT,"wtp%d sta config success\n",WTPIndex);
			}
			else
			{
				wid_syslog_debug_debug(WID_DEFAULT,"wtp%d sta config failed\n",WTPIndex);
				AsdWsm_StationOpNew(WTPIndex,AC_WTP[WTPIndex]->ControlWait->mqinfo.u.StaInfo.STAMAC,STA_LEAVE_REPORT,1);
			}
			
			CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
			CWACStopRetransmission(WTPIndex);
			CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
			
			if (!CWRestartNeighborDeadTimer(WTPIndex))
			{
				_CWCloseThread(WTPIndex);
			}	
			
			CWThreadMutexLock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));
			if ((AC_WTP[WTPIndex]->ControlWait != NULL)
				&& (AC_WTP[WTPIndex]->ControlWait->mqinfo.subtype == STA_S_TYPE))
			{
				CW_FREE_OBJECT(AC_WTP[WTPIndex]->ControlWait);
				wid_syslog_debug_debug(WID_DEFAULT,"wtp%d sta response\n",WTPIndex);
			}
			else
			{				
				CW_FREE_OBJECT(AC_WTP[WTPIndex]->ControlWait);
				wid_syslog_info("wtp%d sta response something wrong\n",WTPIndex);
			}
			CWThreadMutexUnlock(&(gWTPs[WTPIndex].WTPThreadControllistMutex));

			break;
		}
		case CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST:
		{				
			CWDiscoveryRequestValues values;		
			CWWTPVendorInfos *valPtr = NULL;
			int i = 0;
			memset(&values, 0, sizeof(CWDiscoveryRequestValues));
			
			if (!(CWParseDiscoveryRequestMessageInRun(msgPtr, controlVal.msgElemsLen, &values, &wtp_constat)))
			{
				CWDestroyDiscoveryRequestValues(&values); 
				return CW_FALSE;
			}
			
			valPtr = &(values.WTPBoardData);
			for (i = 0; i < valPtr->vendorInfosCount; i++)
			{
				if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
				{
					char * sn = (char *)((valPtr->vendorInfos)[i].SN);
					str2higher(&sn);
					(valPtr->vendorInfos)[i].SN = (unsigned char *)sn;
				
					if (memcmp((valPtr->vendorInfos)[i].SN, AC_WTP[WTPIndex]->WTPSN, strlen(AC_WTP[WTPIndex]->WTPSN)) == 0)
					{
						continue;
					}
					else
					{					
						CWDestroyDiscoveryRequestValues(&values); 
						return CW_FALSE;
					}						
				}
				else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
				{
					if (((valPtr->vendorInfos)[i].mac[0] == AC_WTP[WTPIndex]->WTPMAC[0])
						&& ((valPtr->vendorInfos)[i].mac[1] == AC_WTP[WTPIndex]->WTPMAC[1])
						&& ((valPtr->vendorInfos)[i].mac[2] == AC_WTP[WTPIndex]->WTPMAC[2])
						&& ((valPtr->vendorInfos)[i].mac[3] == AC_WTP[WTPIndex]->WTPMAC[3])
						&& ((valPtr->vendorInfos)[i].mac[4] == AC_WTP[WTPIndex]->WTPMAC[4])
						&& ((valPtr->vendorInfos)[i].mac[5] == AC_WTP[WTPIndex]->WTPMAC[5]))
					{
						continue;
					}
					else
					{
						CWDestroyDiscoveryRequestValues(&values); 
						return CW_FALSE;
					}
				}
				else
				{
					continue;
				}
			}
			
			if (WID_LOGIN == wtp_constat)
			{
				CWDestroyDiscoveryRequestValues(&values); 
				wid_syslog_err("%s:%d check discovery, and wtp%d "MACSTR" quit",__func__,__LINE__,WTPIndex,MAC2STR(AC_WTP[WTPIndex]->WTPMAC));
				gWTPs[WTPIndex].isRequestClose = CW_TRUE;
				_CWCloseThread(WTPIndex);
				return CW_TRUE;
			}
			else
			{
				if (!CWRestartNeighborDeadTimer(WTPIndex))
				{
					_CWCloseThread(WTPIndex);
				}

				if (!(CWAssembleDiscoveryResponse(&messages, controlVal.seqNum, WTPIndex, AC_WTP[WTPIndex]->cap_pro_ver))) 
				{
					wid_syslog_err("%s:%d wtp%d Assemble Discovery Response failed",__func__,__LINE__,WTPIndex);
					CWDestroyDiscoveryRequestValues(&values); 
					return CW_FALSE;
				}
    			
				CWDestroyDiscoveryRequestValues(&values);
				messagesCount = 1;			
				toSend = CW_TRUE;
				break;
			}
		}
		default: 
			//## We have an unexpected request and we have to send
			//## a corresponding response containing a failure result code
			wid_syslog_err("%s:%d wtp%d --> Not valid Request in Run State... we send a failure Response",__func__,__LINE__,WTPIndex);
			if (!CWRestartNeighborDeadTimer(WTPIndex))
			{
				_CWCloseThread(WTPIndex);
			}
			if (!(CWAssembleUnrecognizedMessageResponse(&messages, &messagesCount, gWTPs[WTPIndex].pathMTU, controlVal.seqNum, controlVal.messageTypeValue+1, WTPIndex))) 
 			{
				return CW_FALSE;
			}
			toSend = CW_TRUE;
//			return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message not valid in Run State");
	}
	
	if (toSend)
	{
		int i = 0;
	
		if (messages == NULL)
		{
			return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		}
		for (i = 0; i < messagesCount; i++)
		{
			//if(controlVal.messageTypeValue == CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST)
			//{
				//printf("way send message wtpid=%d sendcount i=%d\n",WTPIndex,i);
			//}
#ifdef CW_NO_DTLS
			if (!CWNetworkSendUnsafeUnconnected(gWTPs[WTPIndex].socket, 
								&gWTPs[WTPIndex].address, 
								messages[i].msg, 
								messages[i].offset))
			{
#else
			if (!(CWSecuritySend(gWTPs[WTPIndex].session, messages[i].msg, messages[i].offset)))
			{
#endif
				CWFreeMessageFragments(messages, messagesCount);
				CW_FREE_OBJECT(messages);
				return CW_FALSE;
			}
		}
		CWFreeMessageFragments(messages, messagesCount);
		CW_FREE_OBJECT(messages);
	}

	gWTPs[WTPIndex].currentState = CW_ENTER_RUN;	
	if (AC_WTP[WTPIndex] == NULL)
	{
        return CW_FALSE;
	}
	if (WID_RELOGIN != wtp_constat)
    {
        AC_WTP[WTPIndex]->WTPStat = WID_RUN;
	}
	gWTPs[WTPIndex].subState = CW_WAITING_REQUEST;
	
	if ((reset) && (AC_WTP[WTPIndex] != NULL) && (AC_WTP[WTPIndex]->resetflag == 1))
	{
		reset = CW_FALSE;
		if (AC_WTP[WTPIndex] != NULL)
		{
			AC_WTP[WTPIndex]->resetflag = 0;
		}
		wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d change static ip close thread",__func__,__LINE__,WTPIndex);
		_CWCloseThread(WTPIndex);		
		
	}

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter run next state %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_STATE_STR(gWTPs[WTPIndex].currentState));
	return CW_TRUE;
}

CWBool CWACParseGenericRunMessage(int WTPIndex, CWProtocolMessage *msg, CWControlHeaderValues* controlVal)
{
	if (msg == NULL || controlVal == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	if (!(CWParseControlHeader(msg, controlVal)))
	{
	    return CW_FALSE; // will be handled by the caller
    }
    
	controlVal->msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp

	if ((controlVal->messageTypeValue % 2 == 1) && (controlVal->messageTypeValue != CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_RESPONSE))			//Check if it is a request
	{
	    return CW_TRUE;	
	}
	
	if (AC_WTP[WTPIndex]->wtp_seqnum_switch == 1)
	{
		if ((gWTPs[WTPIndex].responseSeqNum != controlVal->seqNum) || (gWTPs[WTPIndex].responseType != controlVal->messageTypeValue))
		{
			if (gWTPs[WTPIndex].responseType || gWTPs[WTPIndex].responseSeqNum)
			{
				wid_syslog_err("wtp%d wait response type %d seq %d rcv msg type %d seq %d not match\n", 
            					WTPIndex,gWTPs[WTPIndex].responseType, gWTPs[WTPIndex].responseSeqNum,
            					controlVal->messageTypeValue, controlVal->seqNum);
			}
			CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Seq Num or Msg Type not valid!");
			return CW_FALSE;
		}
	}

	return CW_TRUE;	
}

CWBool CWParseConfigurationUpdateResponseMessage
(
    CWProtocolMessage* msgPtr,
    int len,
    CWProtocolResultCode* resultCode,
    CWRadioOperationalInfoValues* radioOpeinfo,
    int wtpindex
)
{
	int offsetTillMessages = 0;

	if (msgPtr == NULL || resultCode == NULL || ((msgPtr->msg) == NULL))
    {
        return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	offsetTillMessages = msgPtr->offset;

	// parse message elements
	while ((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType)
		{
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode = CWProtocolRetrieve32(msgPtr);
				break;	
				
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				if (!(CWParseWTPRadioOperationalState(msgPtr, elemLen, radioOpeinfo)))
				{
					//printf("****ERROR CWParseWTPRadioOperationalState**\n");
				}
				else
				{
					//printf("****SUCCESS CWParseWTPRadioOperationalState**\n");
				}
				break;

			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				/*
				if(!(CWParseWTPextensioninfomation(msgPtr, elemLen, wtpindex)))
				{
					wid_syslog_debug_debug(WID_DEFAULT,"****ERROR CWParseWTPextensioninfomation**\n");
				}
				else
				{
					wid_syslog_debug_debug(WID_DEFAULT,"****SUCCESS CWParseWTPextensioninfomation**\n");
				}*/
				break;
				
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				break;	
		}
	}
	
	if ((msgPtr->offset - offsetTillMessages) != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}

	return CW_TRUE;	
}

CWBool CWParseDiscoveryRequestMessageInRun
(
	CWProtocolMessage *msgPtr,
	int len,
	CWDiscoveryRequestValues *valuesPtr,
	WTPLoginState *wtp_constat
)
{
	int offsetTillMessages = 0;
	
	if (msgPtr == NULL || (msgPtr->msg) == NULL || valuesPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}	
	wid_syslog_debug_debug(WID_DEFAULT,"%s:%d Discovery Request InRun",__func__,__LINE__);
	
	offsetTillMessages = msgPtr->offset;
	
	while ((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		unsigned short int subelemid = 0;
		unsigned short int subelemlen = 0;
		
		CWParseFormatMsgElem(msgPtr,&elemType,&elemLen);		

//		wid_syslog_debug_debug(WID_DEFAULT,"Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
									
		switch(elemType)
		{
			case CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE:
			{
				if (!(CWParseDiscoveryType(msgPtr, elemLen, valuesPtr))) // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
			{
				if (!(CWParseWTPBoardData(msgPtr, elemLen, &(valuesPtr->WTPBoardData)))) // will be handled by the caller
				{
					return CW_FALSE;
				}
				break; 
			}
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
			{
				if (!(CWParseWTPDescriptor(msgPtr, elemLen, &(valuesPtr->WTPDescriptor))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
			{
				if (!(CWParseWTPFrameTunnelMode(msgPtr, elemLen, &(valuesPtr->frameTunnelMode))))    // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
			{
				if (!(CWParseWTPMACType(msgPtr, elemLen, &(valuesPtr->MACType))))    // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			{
				CWParseFormatMsgElem(msgPtr, &subelemid, &subelemlen);
				if (subelemlen > len)
				{
					wid_syslog_err("%s elemType %d elemlen:%d > len:%d\n", __func__, elemType, subelemlen, len);
					return CW_TRUE;
				}
				if (CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE == subelemid)
				{
					wid_syslog_info("%s:%d subelemid:49 subelemlen:%d Connect_status %d \n", __func__, __LINE__, subelemlen, valuesPtr->Connect_status);
					
					if (!(CWParseWTPLOGINType(msgPtr, subelemlen, &(valuesPtr->Connect_status))))   // will be handled by the caller
					{
						return CW_FALSE;
					}
					
					wid_syslog_info("%s:%d spec_payload:Connect_status %d \n", __func__, __LINE__, valuesPtr->Connect_status);
				}
				break;
			}
			/*case CW_MSG_ELEMENT_WTP_RADIO_INFO_CW_TYPE:
				(*valuesPtr).radios.radiosCount++; // just count how many radios we have, so we can allocate the array
				completeMsg.offset += elemLen;
				break;
			*/
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,"Unrecognized Message Element");
		}		
		//wid_syslog_debug_debug(WID_DEFAULT,"bytes: %d/%d", (completeMsg.offset-offsetTillMessages), controlVal.msgElemsLen);
	}
	
	*wtp_constat = valuesPtr->Connect_status;
	
	if ((msgPtr->offset - offsetTillMessages) != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	return CW_TRUE;
}

CWBool CWParseStaNumResponseMessage(CWProtocolMessage *msgPtr,int WTPID)
{
	TableMsg msg; 
	unsigned char radioid = 0;
	unsigned char  wlanid = 0;
	unsigned short num = 0;
	char *mac = NULL;
	int len;
	int i = WTPID%SOCK_NUM;
	int sock = sockPerThread[i];
	memset(&msg,0,sizeof(msg));

	msg.Type = STA_TYPE;
	msg.Op = STA_CHECK_DEL;
	radioid = CWProtocolRetrieve8(msgPtr);
	wlanid = CWProtocolRetrieve8(msgPtr);
	num = CWProtocolRetrieve16(msgPtr);
	msg.u.bss_sta.radioid = radioid;
	msg.u.bss_sta.wlanid = wlanid;
	msg.u.bss_sta.wtpid = WTPID;
	len = sizeof(msg);	
	wid_syslog_debug_debug(WID_DEFAULT,"CWParseStaNumResponseMessage,num = %d\n",num);
//again:
	msg.u.bss_sta.count = num;
	memset(&msg.u.bss_sta.mac,0,sizeof(msg.u.bss_sta.mac));
	for(i = 0 ; (i < 128)&&(i < num);i++)
	{
		/* coverity-CID:10950 Resource leak */
		mac = CWProtocolRetrieveStr(msgPtr,MAC_LEN);
		if (NULL != mac)
		{
			CW_COPY_MEMORY(msg.u.bss_sta.mac[i], mac, MAC_LEN);
			CW_FREE_OBJECT(mac);
		}
	}
	wid_syslog_debug_debug(WID_DEFAULT,"CWParseStaNumResponseMessage\n");
	if(sendto(sock, &msg, len, 0, (struct sockaddr *) &toASD_STA.addr, toASD_STA.addrlen) < 0){
		wid_syslog_info("%s sendto %s",__func__,strerror(errno));
		perror("send(wASDSocket)");
		wid_syslog_info("WidAsd_StationInfoUpdate2\n");
		wid_syslog_debug_debug(WID_DEFAULT,"*** error AsdWsm_StationOp end***\n");
		return CW_FALSE;
	}
	/*if(i >= 64)
	{
		num = num-i;
		goto again;
	}*/
	return CW_TRUE;
}

CWBool CWParseStaConfigurationResponseMessage(CWProtocolMessage* msgPtr, int len, int WTPID,CWProtocolResultCode* resultCode)
{
	int offsetTillMessages = 0;
	unsigned short value = 0, valuelen = 0;
	unsigned int IGNORE_UBSV vendorid = 0;
	
	if (msgPtr == NULL || (msgPtr->msg) == NULL || resultCode == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	offsetTillMessages = msgPtr->offset;
	wid_syslog_debug_debug(WID_DEFAULT,"Parsing Configuration Update Response...");

	// parse message elements
	while ((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		wid_syslog_debug_debug(WID_DEFAULT,"elemType = %d\n",elemType);
		switch (elemType)
		{
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode = CWProtocolRetrieve32(msgPtr);
				 break;	
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				vendorid = CWProtocolRetrieve32(msgPtr);
				
				CWParseFormatMsgElem(msgPtr, &value, &valuelen);
				wid_syslog_debug_debug(WID_DEFAULT,"value = %d\n",value);
				if (16 == value)			
					CWParseStaNumResponseMessage(msgPtr,WTPID);
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
		}
	}
	
	if ((msgPtr->offset - offsetTillMessages) != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	wid_syslog_debug_debug(WID_DEFAULT,"Sta Configuration Response Parsed");

	return CW_TRUE;	
}


CWBool CWSaveConfigurationUpdateResponseMessage(CWProtocolResultCode resultCode, int WTPIndex)
{
	if (!CWBindingSaveConfigurationUpdateResponse(resultCode, WTPIndex))
	{
		return CW_FALSE;
	}

	return CW_TRUE;
}

void wid_hex_dump(unsigned char *buffer, int buffLen)
{
	unsigned int i = 0, j = 0;
	unsigned int num = 0;
	unsigned int curLen = 0;
	unsigned char lineBuffer[255] = {0}, *bufPtr = NULL;
	
	if(!buffer)
	{
		return;
	}
	wid_syslog_debug_debug(WID_DEFAULT, ".......................RX.......................%d\n", buffLen);
	
	bufPtr = lineBuffer;
	num = buffLen / 16;
	for(i = 0; i < num; i++)
	{
		wid_syslog_debug_debug(WID_DEFAULT, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
								buffer[i * 16 + 0],buffer[i * 16 + 1],buffer[i * 16 + 2],buffer[i * 16 + 3],			
								buffer[i * 16 + 4],buffer[i * 16 + 5],buffer[i * 16 + 6],buffer[i * 16 + 7],		
								buffer[i * 16 + 8],buffer[i * 16 + 9],buffer[i * 16 + 10],buffer[i * 16 + 11],
								buffer[i * 16 + 12],buffer[i * 16 + 13],buffer[i * 16 + 14],buffer[i * 16 + 15]);	
	}	
	if((buffLen%16) != 0)
	{
		j = num * 16;
		for(i = j; i < buffLen; i++)
		{
			curLen += sprintf((char*)bufPtr, "%02x ", buffer[i]);
			bufPtr = lineBuffer + curLen;
		}
		wid_syslog_debug_debug(WID_DEFAULT, "%s\n", lineBuffer);
	}

	wid_syslog_debug_debug(WID_DEFAULT, ".......................RX.......................\n");
}


void wid_hex_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer)
{
	unsigned int i = 0;
	unsigned int curLen = 0;
	unsigned char *bufPtr = NULL;
	
	if (!buffer || !ssidbuffer)
	{
		return;
	}
	
	bufPtr = ssidbuffer;
	for (i = 0; i < buffLen; i++)
	{
		curLen += sprintf((char*)bufPtr, "%02x ", buffer[i]);
		bufPtr = ssidbuffer + curLen;
	}
}

void wid_hex_char_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer)
{
	unsigned int i = 0;
	unsigned int num = 0;
	unsigned int curLen = 0;
	unsigned char *bufPtr = NULL;
	
	if (!buffer || !ssidbuffer)
	{
		return;
	}
	
	bufPtr = ssidbuffer;
	for (i = 0; i < buffLen; i++)
	{
		if (buffer[i] > 128)
		{
			num += 1;
			if ((num%2) != 0)
			{
				curLen += sprintf((char*)bufPtr, "[%02x ", buffer[i]);
			}
			else
			{
				curLen += sprintf((char*)bufPtr, "%02x] ", buffer[i]);
			}
		}
		else
		{
			curLen += sprintf((char*)bufPtr, "%c ", buffer[i]);
		}
		
		bufPtr = ssidbuffer + curLen;
	}
	
	wid_syslog_debug_debug(WID_DEFAULT, WAI_FUNC_LINE_FMT"essid:%s\n", WAI_FUNC_LINE_LITERAL, ssidbuffer);
}

CWBool CWParseWTPEventRequestMessage
(
	CWProtocolMessage *msgPtr,
	int len,
	CWProtocolWTPEventRequestValues *valuesPtr,
	int wtpindex
)
{
	int offsetTillMessages = 0;
	int i = 0, k = 0;
	int vendorvalue = 0;
	int flag = 0;           //Custom manufacturer new format, 0 Old format and element id can't equal to 128, 1 The new format
	int IGNORE_UBSV venderid = 0;
	char report_type = 0;
	unsigned short int elemid = 0;
	unsigned short int elemlen = 0;

	if (msgPtr == NULL || (msgPtr->msg) == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}

	WID_CHECK_WTP_STANDARD_RET(wtpindex, CW_FALSE);

	//CW_CREATE_OBJECT_ERR(valuesPtr, CWProtocolWTPEventRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	offsetTillMessages = msgPtr->offset;
	
	valuesPtr->errorReportCount = 0;
	valuesPtr->errorReport = NULL;
	valuesPtr->duplicateIPv4 = NULL;
	valuesPtr->duplicateIPv6 = NULL;
	valuesPtr->WTPOperationalStatisticsCount = 0;
	valuesPtr->WTPOperationalStatistics = NULL;
	valuesPtr->WTPRadioStatisticsCount = 0;
	valuesPtr->WTPRadioStatistics = NULL;
	valuesPtr->WTPRebootStatistics = NULL;
	valuesPtr->CWStationInfo = NULL;
	valuesPtr->neighbor_ap_infos = NULL;
	valuesPtr->ap_sta_info = NULL;
	valuesPtr->wids_device_infos = NULL;
	valuesPtr->ApReportStaInfo = NULL;
	// parse message elements
	while ((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		if (elemLen > len)
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s len %u too long to %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  \
						elemType, WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), elemLen, len);
			//return CW_FALSE;
			return CW_TRUE;
		}

		switch (elemType)
		{
			case CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->errorReport, CWDecryptErrorReportValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
				if (!(CWParseMsgElemDecryptErrorReport(msgPtr, elemLen, valuesPtr->errorReport)))
				{
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s parse error\n", \
								WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME, elemType, \
								WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
					return CW_FALSE;
				}
				break;	
			case CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->duplicateIPv4, WTPDuplicateIPv4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
				CW_CREATE_ARRAY_ERR((valuesPtr->duplicateIPv4)->MACoffendingDevice_forIpv4, 6, unsigned char, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				if (!(CWParseMsgElemDuplicateIPv4Address(msgPtr, elemLen, valuesPtr->duplicateIPv4)))
				{
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s parse error\n", \
								WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
								WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
					return CW_FALSE;
				}
				break;
			case CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->duplicateIPv6, WTPDuplicateIPv6, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
				CW_CREATE_ARRAY_ERR((valuesPtr->duplicateIPv6)->MACoffendingDevice_forIpv6, 6, unsigned char, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				if (!(CWParseMsgElemDuplicateIPv6Address(msgPtr, elemLen, valuesPtr->duplicateIPv6)))
				{
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s parse error\n", \
								WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
								WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
					return CW_FALSE;
				}
				break;
			case CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE:
				valuesPtr->WTPOperationalStatisticsCount++;
				msgPtr->offset += elemLen;
				break;
			case CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE:
				valuesPtr->WTPRadioStatisticsCount++;
				msgPtr->offset += elemLen;
				break;
			case CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->WTPRebootStatistics, WTPRebootStatisticsInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				if (!(CWParseWTPRebootStatistics(msgPtr, elemLen, valuesPtr->WTPRebootStatistics)))
				{
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s parse error\n", \
								WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
								WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
					return CW_FALSE;	
				}
				break;
			case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->CWStationInfo, CWStationInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
				if (!(CWParseMsgElemCWStationInfoValue(msgPtr, elemLen, valuesPtr->CWStationInfo)))
				{
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s parse error\n", \
								WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
								WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
					return CW_FALSE;
				}
				break;
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
                vendorvalue = CWProtocolPeep8(msgPtr);
                flag = vendorvalue & 0x80;
				 
				if (0 == flag)      //element id can't  equal to 128
				{
					vendorvalue = CWProtocolRetrieve8(msgPtr);				 	
					if (vendorvalue == 1)
					{
						CW_CREATE_OBJECT_ERR(valuesPtr->neighbor_ap_infos, Neighbor_AP_INFOS, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
						if (!(CWParseMsgElemAPNeighborAPInfos(msgPtr, (elemLen-1), valuesPtr->neighbor_ap_infos)))
						{
							//return CW_FALSE;
						}
						if ((valuesPtr->neighbor_ap_infos) && (valuesPtr->neighbor_ap_infos->neighborapInfosCount == 0))
						{
							CW_FREE_OBJECT(valuesPtr->neighbor_ap_infos);
						}

						if ((valuesPtr->neighbor_ap_infos != NULL) && (valuesPtr->neighbor_ap_infos->DeviceInterference == 1))
						{
							if (gtrapflag >= 3)
							{
								wid_dbus_trap_wtp_device_interference(wtpindex);
							}
						}
						
						if ((gapscanset.opstate == 0) && ((valuesPtr->neighbor_ap_infos) != NULL))
						{
							destroy_ap_info_list(&(valuesPtr->neighbor_ap_infos));
						}

						if (gapscanset.opstate == 1)
						{
							wid_mark_rogue_ap(valuesPtr->neighbor_ap_infos);

							CWThreadMutexLock(&(gWTPs[wtpindex].RRMThreadMutex));                            
        					
							if (AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid] == NULL)
							{
								AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid] = valuesPtr->neighbor_ap_infos;
								AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid] = wid_check_rogue_ap_mac(wtpindex,rrm_rid);
							}
							else
							{
								if (NULL == AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid])
								{
									merge_ap_list((AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid]),&(valuesPtr->neighbor_ap_infos),wtpindex);
									AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid] = wid_check_rogue_ap_mac(wtpindex,rrm_rid);
								}
								else
								{
									merge_rogue_list((AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid]),&(valuesPtr->neighbor_ap_infos),rrm_rid);
									merge_ap_list((AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid]),&(valuesPtr->neighbor_ap_infos),wtpindex);
								}

								if (AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid]->neighborapInfosCount == 0)
								{
									CW_FREE_OBJECT(AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid]);
								}
							}

							if (gtrapflag >= 25)
							{
								wid_count_rogue_ap((valuesPtr->neighbor_ap_infos),wtpindex);
								wid_parse_neighbor_ap_list(wtpindex);
							}					
							if ((gtrapflag >= 24) && (AC_WTP[wtpindex]->NeighborAPInfos[0] != NULL))
							{
								channel_interference_detected(wtpindex);
							}
							
							CWThreadMutexUnlock(&(gWTPs[wtpindex].RRMThreadMutex));
							
							valuesPtr->neighbor_ap_infos = NULL;

							if (channel_state == 1)
							{			
								if (!CWErr(CWThreadMutexLock(&gActiveWTPsMutex)))
								{
									wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s take thread lock failed\n", \
													WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,
													elemType, WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "1");
									return CW_FALSE;	
								}
								if (rrm_rid == 0)
								{
									scanningWTPs++;
									if (scanningWTPs >= gActiveWTPs)
									{						
										CWThreadMutexLock(&(gACChannelMutex));
										CWSignalThreadCondition(&gACChannelWait);
										CWThreadMutexUnlock(&gACChannelMutex);
										scanningWTPs = 0;
									}				
								}
								else
								{
									scanningWTPs1++;
							
									wid_syslog_debug_debug(WID_DEFAULT,"%s: notice channel gActiveWTPs %d scanningWTPs1 %d\n",__func__,gActiveWTPs,scanningWTPs1);
									if (scanningWTPs1 >= gActiveWTPs)
									{						
										CWThreadMutexLock(&(gACChannelMutex2));
										CWSignalThreadCondition(&gACChannelWait2);
										CWThreadMutexUnlock(&gACChannelMutex2);
										scanningWTPs1 = 0;
									}				
								}		
								CWThreadMutexUnlock(&gActiveWTPsMutex);
							}

							if (txpower_state == 1)
							{
								CWThreadMutexLock(&(gACTxpowerMutex));
								tx_wtpid = wtpindex;
								CWSignalThreadCondition(&gACTxpowerWait);
								CWThreadMutexUnlock(&gACTxpowerMutex);
							}
							
							if (gapscanset.countermeasures_switch == 1)
							{
    							wid_count_countermeasure_rogue_ap((AC_WTP[wtpindex]->NeighborAPInfos[rrm_rid]),wtpindex);	

								CWThreadMutexLock(&(gWTPs[wtpindex].RRMThreadMutex));
								if (gmacfilterflag == 1)
								{
									delete_rouge_ap_list_by_whitelist(&(AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid]));
								}
								if (gessidfilterflag == 1)
								{
									delete_rouge_ap_list_by_essidlist(&(AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid]));
								}
								delete_rouge_ap_list_by_ouilist(&(AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid]));
								delsame_rogue_ap_from_list(&(AC_WTP[wtpindex]->rouge_ap_infos[rrm_rid]));

								if (wid_check_rogue_ap_increase_state(wtpindex,rrm_rid))
								{
									wid_disturb_countermeasure_rogue_ap(wtpindex,rrm_rid,ROGUE_ADD);
								}
								CWThreadMutexUnlock(&(gWTPs[wtpindex].RRMThreadMutex));
							}
						}
					}
					else if (vendorvalue == 2)
				 	{
				 	    if (!(CWParseMsgElemAPInterfaceInfo(msgPtr, (elemLen-1), &valuesPtr->wid_sample_throughput)))
						{
							wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,
										elemType, WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "2");
						}
						save_sample_throughput_info(wtpindex, valuesPtr->wid_sample_throughput);
				 	}
					else if (vendorvalue == 3)
				 	{
				 		if (!(CWParseMsgElemAPExtensionInfo(msgPtr, (elemLen-1), &valuesPtr->ap_wifi_info)))
						{
							wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,
										elemType, WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "3");
						}
						else
						{
							save_extension_info(wtpindex, valuesPtr->ap_wifi_info);
						}
				 	}
					else if (vendorvalue == 4)
				 	{
				 		CW_CREATE_OBJECT_ERR(valuesPtr->ap_sta_info, WIDStationInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				 		if (!(CWParseMsgElemAPStaInfoReport(msgPtr, (elemLen-1), valuesPtr->ap_sta_info,wtpindex)))
						{
							wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
										WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "4");
						}
						CW_FREE_OBJECT(valuesPtr->ap_sta_info)
				 	}
					else if (vendorvalue == 5)
				 	{
				 		//CW_CREATE_OBJECT_ERR(valuesPtr->ap_if_info, wid_ap_if_state_time, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				 		if (!(CWParseMsgElemAPIfInfoReport(msgPtr, (elemLen-1), &valuesPtr->ap_if_info,wtpindex)))
						{
							wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
										WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "5");
						}
				 	}
					else if (vendorvalue == 6)
					{
						CW_CREATE_OBJECT_ERR(valuesPtr->wids_device_infos, wid_wids_device, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
						if (!(CWParseMsgElemAPWidsInfos(msgPtr, (elemLen-1), valuesPtr->wids_device_infos,wtpindex)))
						{
							wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
										WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "6");
						}
						if (valuesPtr->wids_device_infos->count == 0)
						{
							CW_FREE_OBJECT(valuesPtr->wids_device_infos);
						}				
				    }				
					else if (vendorvalue == 7)
					{
						CW_CREATE_OBJECT_ERR(valuesPtr->wid_sta_wapi_infos, WIDStaWapiInfoList, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
						memset(valuesPtr->wid_sta_wapi_infos, 0, sizeof(WIDStaWapiInfoList));
						if (!(CWParseMsgElemAPStaWapiInfos(msgPtr, (elemLen-1), valuesPtr->wid_sta_wapi_infos,wtpindex)))
						{
							wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s sub type %s parse error\n", \
										WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
										WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), "7");
						}
						CW_FREE_OBJECT(valuesPtr->wid_sta_wapi_infos);
					}
					else if (vendorvalue == 8)
					{
						char tmp[1] = {0};
						CWParseAPStatisInfo_v2(msgPtr, elemLen-1,tmp,wtpindex);

						if (apstatistics == 1)
						{
							int radio_id = 0;
							int IGNORE_UBSV local_radioid = 0 ;
							struct wlanid *wlanid_list = NULL;
							
							for (i = 0; i < AC_WTP[wtpindex]->RadioCount; i++)
							{
								if (AC_WTP[wtpindex]->WTP_Radio[i])
								{
									radio_id = AC_WTP[wtpindex]->WTP_Radio[i]->Radio_G_ID;
									local_radioid = AC_WTP[wtpindex]->WTP_Radio[i]->Radio_L_ID;
									if (AC_RADIO[radio_id])
									{
										wlanid_list = AC_RADIO[radio_id]->Wlan_Id;
										while (wlanid_list)
										{
											if ((AC_WLAN[wlanid_list->wlanid] != NULL)&&(AC_WLAN[wlanid_list->wlanid]->balance_switch == 1)&&(AC_WLAN[wlanid_list->wlanid]->balance_method == 2))
											{
												AsdWsm_RadioOp(wtpindex,RADIO_INFO);
												AsdWsm_bytes_info(wtpindex,BSS_INFO);
												break;
											}
											wlanid_list = wlanid_list->next;
										}
									}
								}
							}
						}
					}
					else if (vendorvalue == 9)//ntp resultcode
					{
						char tmp[1] = {0};
						CWParseAP_Ntp_resultcode(msgPtr, elemLen-1,tmp,wtpindex);
					}
				    /*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,end*/
					else if (vendorvalue == 10)//attack addr redirect
					{
						 CWParseAttack_addr_Redirect(msgPtr, elemLen-1,wtpindex);
					}
					else if (vendorvalue == 11)//challenge replay
					{
						CWParseAP_challenge_replay(msgPtr, elemLen-1,wtpindex);
					}
					else if (vendorvalue == 12)
					{
						char tmp[1] = {0};
					    CWParseWtp_Sta_Terminal_Disturb_Report(msgPtr, elemLen-1,tmp,wtpindex);
					}
					else if (13 == vendorvalue)
					{
				 		CW_CREATE_OBJECT_ERR(valuesPtr->ap_sta_info, WIDStationInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						if (!(CWPareseWtp_Sta_Flow_Check_Report(msgPtr, (elemLen-1), valuesPtr->ap_sta_info,wtpindex)))
						{
						
						}	
						CW_FREE_OBJECT(valuesPtr->ap_sta_info)
							
					}
					else if (15 == vendorvalue)
					{
				 		CW_CREATE_OBJECT_ERR(valuesPtr->ap_sta_info, WIDStationInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						if (!(CWPareseWtp_Sta_leave_Report(msgPtr, (elemLen-1), valuesPtr->ap_sta_info,wtpindex)))
						{
						
						}	
						CW_FREE_OBJECT(valuesPtr->ap_sta_info)			
					}
				}
				else                     //The new format
				{
				 	venderid = CWProtocolRetrieve32(msgPtr);
					venderid &= 0x7fffffff; /* ignore left-most bit */
					CWParseFormatMsgElem(msgPtr, &elemid, &elemlen);
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s vendor %u element(%u) %s len %u\n", \
								WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME, elemType, \
								WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType), venderid, \
								elemid, WAI_VENDOR_MSG_ELEMENT_TYPE_STR(elemid), elemlen);
					if (elemlen > len)
					{
						wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s vendor %u element %u len %u too long %u\n", \
									WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME, elemType,\
									WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType),  \
									venderid, elemid, elemlen, len);
						return CW_TRUE;
					}

					if (CW_VENDOR_EVENT_ELEMENT_SPECTRUM_ANALYSIS == elemid)
					{
						if (!(CWParseSpectrumAnalysis_report(msgPtr, elemlen, wtpindex)))
						{						
						}										
					}
				 	else if (CW_VENDOR_EVENT_ELEMENT_CWPTK_RESULT == elemid)
					{
				 		CW_CREATE_OBJECT_ERR(valuesPtr->ap_sta_info, WIDStationInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						if (!(CWParseWtp_Sta_Ptk_Report(msgPtr, (elemLen-1), valuesPtr->ap_sta_info, wtpindex)))
						{						
						}	
						CW_FREE_OBJECT(valuesPtr->ap_sta_info)
					}
					if (CW_VENDOR_EVENT_ELEMENT_STA_INFO == elemid)
					{
						if (!(CWParseStaInfo(msgPtr, elemlen, wtpindex)))
						{						
						}
					}
					if (CW_VENDOR_EVENT_ELEMENT_BSSID == elemid)
					{
						unsigned char radio_num = 0;
						CWProtocolResultCode resultCode[L_RADIO_NUM];
						WID_BSS BSS[L_RADIO_NUM];
						memset(resultCode, 0, L_RADIO_NUM*(sizeof(CWProtocolResultCode)));
						memset(BSS, 0, L_RADIO_NUM*(sizeof(WID_BSS)));
						
						if (!(CWParseWlanConfigurationBssInfo(msgPtr, elemlen, resultCode, BSS, &radio_num)))
						{
							return CW_FALSE;
						}
						
						int j = 0;
						for (j = 0; j < radio_num; j++)
						{
							unsigned char BSSID[MAC_LEN];
							memset(BSSID, 0, MAC_LEN);
							unsigned int bssindex = 0;
							int zero = memcmp(BSS[j].BSSID, BSSID, MAC_LEN);
							
							if (zero != 0)
							{
								wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d radio%d wlan%d BSSID:"MACSTR"\n",
														__func__,__LINE__,wtpindex,BSS[j].Radio_L_ID,BSS[j].WlanID,MAC2STR(BSS[j].BSSID));
								int i1 = 0;
								if (!check_wlanid_func(BSS[j].WlanID))
								{
									wid_syslog_err("%s BSS[%d].WlanID = %d\n",__func__,j,BSS[j].WlanID);
									//return CW_FALSE;
								}
								if (!check_l_radioid_func(BSS[j].Radio_L_ID))
								{
									wid_syslog_err("%s BSS[%d].Radio_L_ID = %d\n",__func__,j,BSS[j].Radio_L_ID);
									//return CW_FALSE;
								}
								
								if ((BSS[j].Radio_L_ID < L_RADIO_NUM)
									&& (AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID] != NULL)
									&& (check_wlanid_func(BSS[j].WlanID)))
								{
									for (i1 = 0; i1 < L_BSS_NUM; i1++)
									{
										if (AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1] == NULL)
										{
											continue;
										}
										else if (AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->WlanID == BSS[j].WlanID)
										{
											memcpy(AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSID, BSS[j].BSSID, MAC_LEN);
											AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->State = 1;
											//AsdWsm_BSSOp(AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSIndex, WID_ADD, 1);
											bssindex = AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSIndex;
											if (!check_bssid_func(bssindex))
											{
												continue;
											}
#if NOSOFTAC
											wid_asd_bss_traffic_limit(bssindex);
#endif
											if (AC_BSS[bssindex]->BSS_TUNNEL_POLICY == CW_802_IPIP_TUNNEL)
											{
												add_ipip_tunnel(bssindex);
											}
											wid_syslog_debug_debug(WID_DEFAULT,"%s:%d wtp%d radio%d wlan%d BSSID:"MACSTR"\n",
																	__func__,__LINE__,wtpindex,BSS[j].Radio_L_ID,BSS[j].WlanID,
																	MAC2STR(AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSID));
											CWThreadMutexLock(&MasterBak);
											/*ht changed for update bss info in local mode*/
											if (1)//((AC_BSS[bssindex]->BSS_IF_POLICY == BSS_INTERFACE)||(AC_BSS[bssindex]->BSS_IF_POLICY == WLAN_INTERFACE))
											{
												struct sockaddr *sa = NULL;
												struct sockaddr *sa2 = NULL;
												//int ret = -1; 
												unsigned char wlan_id = 0;
												IF_info ifinfo;
#if NOSOFTAC
												int fd = open("/dev/wifi0", O_RDWR);
												wid_syslog_debug_debug(WID_DEFAULT,"***%s:%d fd:%d ***\n",__func__,__LINE__,fd);
												
												if (fd < 0)
												{
													CWThreadMutexUnlock(&MasterBak);
													int k = 0;
													for (k = 0; k < radio_num; k++)
													{
														CW_FREE_OBJECT(BSS[k].BSSID);
													}
													return -1;//create failure
												}
#endif
												memset(&ifinfo, 0, sizeof(IF_info));
												sa = (struct sockaddr *)&gWTPs[wtpindex].address;
												sa2 = (struct sockaddr*)&(gInterfaces[gWTPs[wtpindex].interfaceIndex].addr);
												ifinfo.acport = htons(5247);
												ifinfo.BSSIndex = bssindex;
												ifinfo.WLANID = AC_BSS[bssindex]->WlanID;
												wlan_id = AC_BSS[bssindex]->WlanID; 
												if ((AC_WLAN[wlan_id] != NULL)
													&& (AC_WLAN[wlan_id]->SecurityType == OPEN)
													&& (AC_WLAN[wlan_id]->EncryptionType == NONE))
												{
													ifinfo.protect_type = 0;
												}
												else
												{
													ifinfo.protect_type = 1;
												}
												ifinfo.vrid = local*MAX_INSTANCE+ vrrid;
												if (AC_BSS[bssindex]->BSS_TUNNEL_POLICY == CW_802_DOT_3_TUNNEL)
												{
													ifinfo.f802_3 = 1;
													wid_syslog_info("%s: ifinfo.f802_3 = %d.\n",__func__,ifinfo.f802_3);
												}
												else
												{
													ifinfo.f802_3 = 0;
													wid_syslog_info("%s: AC_BSS[%d]->BSS_TUNNEL_POLICY =%d, not dot3, set ifinfo.f802_3 = %d.\n",
																	__func__,bssindex,AC_BSS[bssindex]->BSS_TUNNEL_POLICY,ifinfo.f802_3);
												}
												ifinfo.wsmswitch = wsmswitch;
												ifinfo.vlanSwitch = vlanSwitch;
												if ((AC_BSS[bssindex]->BSS_IF_POLICY == BSS_INTERFACE)
													|| (AC_BSS[bssindex]->BSS_IF_POLICY == WLAN_INTERFACE))
												{
													ifinfo.if_policy = 1;
												}
												else
												{
													ifinfo.if_policy = 0;
												}
												
												if (AC_BSS[bssindex]->vlanid != 0)
												{
													ifinfo.vlanid = AC_BSS[bssindex]->vlanid;
												}
												else if (AC_BSS[bssindex]->wlan_vlanid != 0)
												{
													ifinfo.vlanid = AC_BSS[bssindex]->wlan_vlanid;
												}
												else
												{
													ifinfo.vlanid = 0;
												}
#if DYNAMIC_VLAN_SUPPORT
												ifinfo.dyvlanid = AC_BSS[bssindex]->dyvlanid;
												wid_syslog_debug_debug(WID_DEFAULT, "%s:%d bss%d "MACSTR" dyvlanid %d to ifinfo.dyvlanid %d", 
																		__func__,__LINE__,bssindex, MAC2STR(AC_BSS[bssindex]->BSSID),AC_BSS[bssindex]->dyvlanid, ifinfo.dyvlanid);
#endif
												memcpy(ifinfo.apmac, AC_WTP[wtpindex]->WTPMAC, MAC_LEN);
												memcpy(ifinfo.bssid, AC_BSS[bssindex]->BSSID, MAC_LEN);
												memcpy(ifinfo.ifname, AC_WTP[wtpindex]->BindingIFName, strlen(AC_WTP[wtpindex]->BindingIFName));
												memcpy(ifinfo.apname, AC_WTP[wtpindex]->WTPNAME, strlen(AC_WTP[wtpindex]->WTPNAME));
												if (AC_WLAN[wlan_id] != NULL)
												{
													if (NULL != AC_WLAN[wlan_id]->ESSID)
													{
														memcpy(ifinfo.essid, AC_WLAN[wlan_id]->ESSID, strlen((char *)AC_WLAN[wlan_id]->ESSID));
													}
													ifinfo.Eap1XServerSwitch = AC_WLAN[wlan_id]->eap_mac_switch;
													memset(ifinfo.Eap1XServerMac, 0, MAC_LEN);
													memcpy(ifinfo.Eap1XServerMac, AC_WLAN[wlan_id]->eap_mac2, MAC_LEN);
													wid_syslog_debug_debug(WID_DBUS,"%s:%d AC_WLAN[%d]->eap_mac2= "MACSTR"\n",__func__,__LINE__,wlan_id,MAC2STR(AC_WLAN[wlan_id]->eap_mac2));
#if WIFI_QOS_SUPPORT
													memcpy(ifinfo.qos_11e_to_1p, AC_WLAN[wlan_id]->qos_ac_11e_to_1p, MAX_QOS_11E_VAL+1);
													memcpy(ifinfo.qos_dscp_to_1p, AC_WLAN[wlan_id]->qos_ac_dscp_to_1p, MAX_QOS_DSCP_VAL+1);
													memcpy(ifinfo.qos_dscp_to_dscp, AC_WLAN[wlan_id]->qos_ac_dscp_to_dscp, MAX_QOS_DSCP_VAL+1);
#endif	
												}
												
												char __str[128];
												memset(__str, 0, 128);
												char *str = "lo";
												str = sock_ntop_r(((struct sockaddr*)&(gInterfaces[gWTPs[wtpindex].interfaceIndex].addr)), __str);
												wid_syslog_info("%s:%d wtp%d on Interface %s(%d)\n", __func__, __LINE__, wtpindex, str, gWTPs[wtpindex].interfaceIndex);
												
												if (sa->sa_family != AF_INET6)
												{
													ifinfo.isIPv6 = 0;
													ifinfo.apip = ((struct sockaddr_in *)sa)->sin_addr.s_addr;
													ifinfo.apport = ((struct sockaddr_in *)sa)->sin_port +1;
													struct sockaddr_in  *sin = (struct sockaddr_in *) sa2;
													unsigned int v_ip = sin->sin_addr.s_addr;
													wid_syslog_info("%s: v_ip %d %d.%d.%d.%d.\n",
																	__func__, v_ip, (v_ip >> 24) & 0xFF, (v_ip >> 16) & 0xFF,(v_ip >> 8) & 0xFF, v_ip & 0xFF);
													ifinfo.acip = v_ip;
                                                }
												else
												{
													ifinfo.isIPv6 = 1;
													memcpy(ifinfo.apipv6,&((struct sockaddr_in6 *) sa)->sin6_addr,sizeof(struct in6_addr));
													ifinfo.apport = ((struct sockaddr_in6 *)sa)->sin6_port +1;
													memcpy(ifinfo.acipv6,&((struct sockaddr_in6 *) sa2)->sin6_addr,sizeof(struct in6_addr));
												}
#if NOSOFTAC
												int ret = 0;
												ret = ioctl(fd, WIFI_IOC_IF_UPDATE, &ifinfo);
												
												wid_syslog_debug_debug(WID_DEFAULT,"%s:%d WIFI_IOC_IF_UPDATE ret:%d\n",__func__,__LINE__,ret);
												close(fd);
												if (ret < 0)
												{
													CWThreadMutexUnlock(&MasterBak);
													int k = 0;
													for (k = 0; k < radio_num; k++)
													{
														CW_FREE_OBJECT(BSS[k].BSSID);
													}
													return -1;
												}
#endif
											}
											struct bak_sock *tmp = bak_list;
											if (tmp != NULL)
											{
												bak_add_del_bss(tmp->sock,B_ADD,AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->BSS[i1]->BSSIndex);
											}
											CWThreadMutexUnlock(&MasterBak);
											
											if (gtrapflag >= 4)
											{
#if NOSOFTAC
												int type = 2;//auto
												int flag = 1;//enable
												wid_dbus_trap_ap_ath_error(wtpindex,BSS[j].Radio_L_ID,BSS[j].WlanID,type,flag);
#endif
											}
										}
									}
								}
							}
							
							if (resultCode[j] == CW_PROTOCOL_SUCCESS)
							{
								if ((zero != 0) && (BSS[j].Radio_L_ID < L_RADIO_NUM))
								{
									if ((check_l_radioid_func(BSS[j].Radio_L_ID))
										&& (AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID] != NULL))
									{
										AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->AdStat = 1;
										AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->OpStat = 1;
										wid_syslog_debug_debug(WID_DEFAULT,"%s:%d radio%d-%d create wlan%d success\n",__func__,__LINE__,wtpindex,BSS[j].Radio_L_ID,BSS[j].WlanID);
										
										if (AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->channelsendtimes == 1)
										{
											int RadioID = 0;
											msgq msg;
											struct msgqlist *elem1 = NULL;
											
											RadioID = wtpindex*L_RADIO_NUM + BSS[j].Radio_L_ID ;    
											
											if (AC_RADIO[RadioID]->auto_channel_cont == 0)
											{
												AC_RADIO[RadioID]->Radio_Chan = 0;
												
												memset((char*)&msg, 0, sizeof(msg));
												msg.mqid = wtpindex%THREAD_NUM+1;
												msg.mqinfo.WTPID = wtpindex;
												msg.mqinfo.type = CONTROL_TYPE;
												msg.mqinfo.subtype = Radio_S_TYPE;
												msg.mqinfo.u.RadioInfo.Radio_Op = Radio_Channel;
												msg.mqinfo.u.RadioInfo.Radio_L_ID = RadioID%L_RADIO_NUM;
												msg.mqinfo.u.RadioInfo.Radio_G_ID = RadioID;
												
												elem1 = (struct msgqlist*)malloc(sizeof(struct msgqlist));
												if (elem1 == NULL)
												{
													wid_syslog_crit("%s malloc %s",__func__,strerror(errno));
													int k = 0;
													for (k = 0; k < radio_num; k++)
													{
														CW_FREE_OBJECT(BSS[k].BSSID);
													}
													return 0;
												}
												memset((char*)&(elem1->mqinfo), 0, sizeof(msgqdetail));
												elem1->next = NULL;
												memcpy((char*)&(elem1->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
												WID_INSERT_CONTROL_LIST(wtpindex, elem1);
											}
											
											AC_WTP[wtpindex]->WTP_Radio[BSS[j].Radio_L_ID]->channelsendtimes--;
										}
									}
								}
							}
							
							CW_FREE_OBJECT(BSS[j].BSSID);
						}
					}
					if (CW_VENDOR_EVENT_ELEMENT_AFIINFO == elemid)
					{
						if (!(CWParseAfiStatisticsInfo(msgPtr, elemlen, wtpindex)))
						{
						}
					}
				}
			 	break;
			case BINDING_MSG_ELEMENT_TYPE_WTP_SNOOPING:
				break;
			case BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_REPORT:
				//malloc memory
				report_type = CWProtocolRetrieve8(msgPtr);
				if (report_type == 1)
				{
					CW_CREATE_OBJECT_ERR(valuesPtr->ApReportStaInfo, CWStationReportInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
					if (!(CWParseMsgElemCWWtpStaIpMacReportInfo(msgPtr, elemLen-1, valuesPtr->ApReportStaInfo)))
					{
						wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s parse error\n", \
									WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME,  elemType, \
									WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
						return CW_FALSE;
					}
				}
				break;
				
			default:
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"msg element type(%u) %s unrecognized\n", \
							WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME, elemType, \
							WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(elemType));
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in WTP Event Request");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len)
	{
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	CW_CREATE_ARRAY_ERR(valuesPtr->WTPOperationalStatistics, valuesPtr->WTPOperationalStatisticsCount, WTPOperationalStatisticsValues,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->WTPRadioStatistics, valuesPtr->WTPRadioStatisticsCount, WTPRadioStatisticsValues,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	

	msgPtr->offset = offsetTillMessages;
	while ((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch (elemType)
		{
			case CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE:
				if (!(CWParseWTPOperationalStatistics(msgPtr, elemLen, &(valuesPtr->WTPOperationalStatistics[k++]), wtpindex)))
				{
					return CW_FALSE;				
				}
				break;
			case CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE:
				if (!(CWParseWTPRadioStatistics(msgPtr, elemLen, &(valuesPtr->WTPRadioStatistics[i++]), wtpindex)))
				{
					return CW_FALSE;
				}
				break;
			default:
				msgPtr->offset += elemLen;
				break;
		}
	}

	return CW_TRUE;	
}

CWBool CWSaveWTPEventRequestMessage (CWProtocolWTPEventRequestValues *WTPEventRequest, CWWTPProtocolManager *WTPProtocolManager)
{
	if (WTPEventRequest == NULL || WTPProtocolManager == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
    }
    
	if (WTPEventRequest->WTPRebootStatistics)
	{	
		CW_FREE_OBJECT(WTPProtocolManager->WTPRebootStatistics);
		WTPProtocolManager->WTPRebootStatistics = WTPEventRequest->WTPRebootStatistics;
	}

	if ((WTPEventRequest->WTPOperationalStatisticsCount) > 0)
	{
		int i = 0, k = 0;
		CWBool IGNORE_UBSV found = CW_FALSE;

		for (i = 0; i < (WTPEventRequest->WTPOperationalStatisticsCount); i++)
		{
			found = CW_FALSE;
			for (k = 0; k < (WTPProtocolManager->radiosInfo).radioCount; k++)
			{
				if ((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == (WTPEventRequest->WTPOperationalStatistics[i]).radioID) 
				{
					found = CW_TRUE;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].TxQueueLevel = (WTPEventRequest->WTPOperationalStatistics[i]).TxQueueLevel;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].wirelessLinkFramesPerSec = (WTPEventRequest->WTPOperationalStatistics[i]).wirelessLinkFramesPerSec;
				}
			}
			/*if(!found)
			{
				for(k=0; k<(WTPProtocolManager->radiosInfo).radioCount; k++)
				{
					if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == UNUSED_RADIO_ID); 
					{
						(WTPProtocolManager->radiosInfo).radiosInfo[k].radioID = (WTPEventRequest->WTPOperationalStatistics[i]).radioID;
						(WTPProtocolManager->radiosInfo).radiosInfo[k].TxQueueLevel = (WTPEventRequest->WTPOperationalStatistics[i]).TxQueueLevel;
						(WTPProtocolManager->radiosInfo).radiosInfo[k].wirelessLinkFramesPerSec = (WTPEventRequest->WTPOperationalStatistics[i]).wirelessLinkFramesPerSec;
					}
				}	
			}*/
		}
	}

	if ((WTPEventRequest->WTPRadioStatisticsCount) > 0)
	{
		int i = 0, k = 0;
		CWBool IGNORE_UBSV found;

		for (i = 0; i < (WTPEventRequest->WTPRadioStatisticsCount); i++)
		{
			found = CW_FALSE;
			for (k = 0; k < (WTPProtocolManager->radiosInfo).radioCount; k++) 
			{
				if ((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == (WTPEventRequest->WTPOperationalStatistics[i]).radioID) 
				{
					found = CW_TRUE;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].statistics = (WTPEventRequest->WTPRadioStatistics[i]).WTPRadioStatistics;
				}
			}
			/*if(!found)
			{
				for(k=0; k<(WTPProtocolManager->radiosInfo).radioCount; k++) 
				{
					if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == UNUSED_RADIO_ID);
					{
						(WTPProtocolManager->radiosInfo).radiosInfo[k].radioID = (WTPEventRequest->WTPOperationalStatistics[i]).radioID;
						(WTPProtocolManager->radiosInfo).radiosInfo[k].statistics = (WTPEventRequest->WTPRadioStatistics[i]).WTPRadioStatistics;
					}
				}	
			}*/
		}
	}

	//CW_FREE_OBJECT((WTPEventRequest->WTPOperationalStatistics), (WTPEventRequest->WTPOperationalStatisticsCount));
	//CW_FREE_OBJECTS_ARRAY((WTPEventRequest->WTPRadioStatistics), (WTPEventRequest->WTPRadioStatisticsCount));
	//Da controllare!!!!!!!
	CW_FREE_OBJECT(WTPEventRequest->WTPOperationalStatistics);
	CW_FREE_OBJECT(WTPEventRequest->WTPRadioStatistics);
	//CW_FREE_OBJECT(WTPEventRequest);

	return CW_TRUE;
}


CWBool CWAssembleWTPEventResponse (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex) 
{
	CWProtocolMessage *msgElems = NULL;
	const int msgElemCount = 0;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	if (!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE, msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
					WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE),seqNum, msgElemCount);

	return CW_TRUE;
}

CWBool CWParseChangeStateEventRequestMessage2 
(
	CWProtocolMessage *msgPtr,
	int len, 
	CWProtocolChangeStateEventRequestValues **valuesPtr,
	int WTPIndex
) 
{
	int i = 0;
	int offsetTillMessages = 0;
	
	if ((msgPtr == NULL) || ((msgPtr->msg) == NULL))
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	CW_CREATE_OBJECT_ERR(*valuesPtr, CWProtocolChangeStateEventRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	offsetTillMessages = msgPtr->offset;
	wid_syslog_debug_debug(WID_WTPINFO,"#________ WTP Change State Event (Run) ________#");
	
	(*valuesPtr)->radioOperationalInfo.radiosCount = 0;
	(*valuesPtr)->radioOperationalInfo.radios = NULL;
	
	// parse message elements
	while ((msgPtr->offset-offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen = 0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(msgPtr,&elemType,&elemLen);		

		wid_syslog_debug_debug(WID_WTPINFO,"Parsing Message Element: %u, elemLen: %u", elemType, elemLen);

		switch (elemType)
		{
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				((*valuesPtr)->radioOperationalInfo.radiosCount)++; // just count how many radios we have, so we can allocate the array
				msgPtr->offset += elemLen;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE: 
				if (!(CWParseResultCode(msgPtr, elemLen, &((*valuesPtr)->resultCode)))) return CW_FALSE;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Change State Event Request");
		}
	}
	
	if ((msgPtr->offset - offsetTillMessages) != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	CW_CREATE_ARRAY_ERR((*valuesPtr)->radioOperationalInfo.radios, (*valuesPtr)->radioOperationalInfo.radiosCount, CWRadioOperationalInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	msgPtr->offset = offsetTillMessages;
	i = 0;
	while (msgPtr->offset-offsetTillMessages < len)
	{
		unsigned short int type = 0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len = 0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(msgPtr,&type,&len);		

		switch (type)
		{
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				if (!(CWParseWTPRadioOperationalState(msgPtr, len, &((*valuesPtr)->radioOperationalInfo.radios[i]))))
				{
    				return CW_FALSE; // will be handled by the caller
    			}	
				i++;
				break;
			default:
				msgPtr->offset += len;
				break;
		}
	}
	
	return CW_TRUE;
}


CWBool CWSaveChangeStateEventRequestMessage
(
    CWProtocolChangeStateEventRequestValues *valuesPtr,
    CWWTPProtocolManager *WTPProtocolManager
)
{
    int i = 0, k = 0;
	CWBool found = CW_FALSE;
	CWBool ret = CW_TRUE;

	if(valuesPtr == NULL || WTPProtocolManager == NULL) 
	{
    	return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
    }
    
	if((valuesPtr->radioOperationalInfo.radiosCount) > 0)
	{
		for(i = 0; i < (valuesPtr->radioOperationalInfo.radiosCount); i++)
		{
			found = CW_FALSE;
			for(k = 0; k < (WTPProtocolManager->radiosInfo).radioCount; k++)
			{
				if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == (valuesPtr->radioOperationalInfo.radios[i]).ID) 
				{
					found = CW_TRUE;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].operationalState = (valuesPtr->radioOperationalInfo.radios[i]).state;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].operationalCause = (valuesPtr->radioOperationalInfo.radios[i]).cause;
					break;
				}	
			}
			if(!found)
			{
				ret = CW_FALSE;
				break;
			}
		}
	}
	
	CW_FREE_OBJECT(valuesPtr->radioOperationalInfo.radios)
	CW_FREE_OBJECT(valuesPtr);	

	return ret;
}


CWBool CWParseEchoRequestMessage (CWProtocolMessage *msgPtr, int len, int WTPIndex) 
{
	
	int offsetTillMessages;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;
	
	// parse message elements
	while((msgPtr->offset-offsetTillMessages) < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(msgPtr,&elemType,&elemLen);		

		//wid_syslog_debug_debug("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
		
		/*fengwenchao modify begin for GM-3,20111121*/
		unsigned char vendorvalue = 0;  //fengwenchao add 20111121 for GM-3
		switch(elemType) {
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				vendorvalue = CWProtocolRetrieve8(msgPtr);
				if(vendorvalue == 111)
				{	
					if((AC_WTP[WTPIndex] != NULL)&&(AC_WTP[WTPIndex]->heart_time.heart_statistics_switch == 1))
						CWParselinkquality(msgPtr, (elemLen-1),WTPIndex);
					else
						msgPtr->offset = msgPtr->offset +15;
				}
				else
				{	msgPtr->offset = msgPtr->offset -1;
					CWParseAPStatisInfo(msgPtr, elemLen,WTPIndex);
				}
				break;

			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Echo Request must carry no message elements");
		}
		/*fengwenchao modify end*/
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	return CW_TRUE;
}

CWBool CWAssembleEchoResponse (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex) 
{
	CWProtocolMessage *msgElems = NULL;
	const int msgElemCount = 0;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_ECHO_RESPONSE, msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
					WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_ECHO_RESPONSE), seqNum, msgElemCount);
	return CW_TRUE;
}

CWBool CWAssembleConfigurationUpdateRequest_Radio(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem) 
{	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	CWProtocolMessage *msgElems = NULL;
	int MsgElemCount = 0;
	int k = -1, i = 0;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if (!check_wtpid_func(WTPIndex))
	{
		wid_syslog_err("%s wtpid %d illegal\n", __func__, WTPIndex);
		return CW_FALSE;
	}
	
	if (!CWBindingAssembleConfigurationUpdateRequest2(&msgElemsBinding, &msgElemBindingCount, WTPIndex, elem))
	{
		return CW_FALSE;
	}
	
	if ((elem->mqinfo.u.RadioInfo.Radio_Op == Radio_Throughput) || (elem->mqinfo.u.RadioInfo.Radio_Op == Radio_BSS_Throughput))
	{
		MsgElemCount = 1;
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, 
					{CW_FREE_OBJECT(msgElemsBinding); return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});

		if (!(CWAssembleMsgElemAPThroughoutSet(&(msgElems[++k]),WTPIndex))) 
		{
			for (i = 0; i <= k; i++) 
			{ 
				CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
			CW_FREE_OBJECT(msgElems);
			CW_FREE_OBJECT(msgElemsBinding); 
			return CW_FALSE; // error will be handled by the caller
		}
	}		
	
	if (elem->mqinfo.u.RadioInfo.Radio_Op == Radio_Qos)
	{
		wid_syslog_debug_debug(WID_WTPINFO,"## wtp id = %d enable qos##\n",WTPIndex);
		MsgElemCount = 1;
		/* Coverity: CID: 10198  Error-Type: Resource leak */
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, 
													{CW_FREE_OBJECT(msgElemsBinding);
													return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		int tagpacket = 0;
		int g_radio_id = elem->mqinfo.u.RadioInfo.Radio_G_ID;
		unsigned int qosid = elem->mqinfo.u.RadioInfo.id1;
		if (!check_g_radioid_func(g_radio_id))
		{
			wid_syslog_err("%s\n",__func__);
			CW_FREE_OBJECT(msgElems);
			CW_FREE_OBJECT(msgElemsBinding); 
			return CW_FALSE;
		}
		if (!(CWAssembleWTPQoS2(&(msgElems[++k]),g_radio_id,tagpacket,qosid))) 
		{
			for (i = 0; i <= k; i++) 
			{
				CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
			CW_FREE_OBJECT(msgElems);
			CW_FREE_OBJECT(msgElemsBinding); 
			return CW_FALSE; // error will be handled by the caller
		}
	}

	if (!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST, msgElems, MsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
					WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST), seqNum, MsgElemCount);

	return CW_TRUE;
}


CWBool CWAssembleConfigurationUpdateRequest_WTP(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem) 
{	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	CWProtocolMessage *msgElems = NULL;
	int MsgElemCount = 0;
	int k = -1;
	int i = 0;
	int level = 0;
	unsigned char l_radioid = 0;
	unsigned char wlanid = 0;
	unsigned int send_traffic_limit = 0;
	unsigned int traffic_limit = 0;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	switch (elem->mqinfo.u.WtpInfo.Wtp_Op)
	{
		case WTP_SCANNING_OP:
			//printf("##003 wtp id = %d ##\n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

			if (!(CWAssembleMsgElemAPScanningSet(&(msgElems[++k])))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
	
		case WTP_STATISTICS_REPORT:
			//printf("##statistics wtp id = %d ##\n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

			if (!(CWAssembleMsgElemAPStatisticsSet(&(msgElems[++k]),elem->mqinfo.u.WtpInfo.value2)))
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_STATISTICS_REPORT_INTERVAL:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAPStatistics_Interval_Set(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
		/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3*/
		case WTP_NTP_SET:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAP_NTP_Set(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_EXTEND_CMD:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

			if (!(CWAssembleMsgElemAPExtensinCommandSet(&(msgElems[++k]),WTPIndex,(char*)&(elem->mqinfo.u.WtpInfo.value)))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;	
	
		case WTP_SET_IP:
			//printf("## wtp id = %d config ap ip \n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleStaticAPIP(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;	
	
		case WTP_TIMESTAMP:
			//printf("## wtp id = %d synchronization ac timestamp \n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleTimestamp(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_EXTEND_INFO_GET:
			//printf("## wtp id = %d extension infomation\n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssemblewtpextensioninfomation(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_SAMPLE_INFO_SET:
			//printf("## wtp id = %d ##\n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

			if (!(CWAssembleMsgElemAPInterfaceInfo(&(msgElems[++k])))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_STA_INFO_SET:
			//printf("## wtp id = %d sta infomation\n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssemblewtpstainfomationreport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_IF_INFO_SET:
			//printf("## wtp id = %d if infomation\n",WTPIndex);
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssemblewtpifinforeport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_WIDS_SET:
			
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWidsSet(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_NEIGHBORDEAD_INTERVAL:
			
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleneighbordead_interval(&(msgElems[++k])))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_SET_IP_DNS:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleStaticAPIPDNS(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			
			wid_syslog_debug_debug(WID_WTPINFO,"wtp%d assemble static IP DNS\n",WTPIndex);
			break;
			
		case WTP_STA_WAPI_INFO_SET:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWtpStaWapiInfoReport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
			
		case WTP_DHCP_SNOOPING:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleSnoopingAble(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_STA_INFO_REPORT:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleIpMacReport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
			
		case WTP_FLOW_CHECK:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
			if (!(CWAssembleMsgElemAPFlowCheck(&(msgElems[++k]),elem->mqinfo.u.WlanInfo.Radio_L_ID,elem->mqinfo.u.WlanInfo.WLANID,elem->mqinfo.u.WlanInfo.flow_check,elem->mqinfo.u.WlanInfo.no_flow_time,elem->mqinfo.u.WlanInfo.limit_flow))) 
			{	
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		/* zhangshu add for Terminal Disturb Info Report, 2010-10-07 */
		case WTP_TERMINAL_DISTRUB_INFO:
		    MsgElemCount = 1;
		    CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		    if (!(CWAssembleTerminalDisturbInfoReport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
		    break;
			
		/*fengwenchao add 20110126 for XJDEX-32  from 2.0 */
		case WTP_IF_ETH_MTU:
			MsgElemCount = 1;
			char eth_index = elem->mqinfo.u.WtpInfo.value1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWtpEthMtu(&(msgElems[++k]),WTPIndex,eth_index))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
			
		case WTP_SET_NAME_PASSWD:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAPPasswd(&(msgElems[++k]),elem->mqinfo.u.WtpInfo.username,elem->mqinfo.u.WtpInfo.passwd))) 
			{	
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_MULTI_USER_OPTIMIZE:
			MsgElemCount =1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAPMultiUserOptimize(&(msgElems[++k]),elem->mqinfo.u.WlanInfo.WLANID,elem->mqinfo.u.WlanInfo.Radio_L_ID,elem->mqinfo.u.WlanInfo.multi_user_optimize_switch))) 
			{			
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_STA_DEAUTH_SWITCH:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWtpStaDeauthreport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_STA_FLOW_INFORMATION_SWITCH:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWtpStaFlowInformationreport(&(msgElems[++k]),WTPIndex))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;

		case WTP_OPTION60_PARAM:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems,MsgElemCount,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
			if (!(CWAssembleMsgElemAPOption60ParameterSet(&(msgElems[++k]),WTPIndex,(char*)&(elem->mqinfo.u.WtpInfo.value)))) 
			{
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;

		case WTP_SPECTRUM_ANALYSIS:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAPspectrum_analysis(&(msgElems[++k]),
														elem->mqinfo.u.WtpInfo.c1,
														elem->mqinfo.u.WtpInfo.c2,
														elem->mqinfo.u.WtpInfo.s1,
														elem->mqinfo.u.WtpInfo.s2,
														elem->mqinfo.u.WtpInfo.s3
														))) 
			{
				for(i = 0; i <= k; i++) 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_LAN_VLAN:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemWTPlanvlan(&(msgElems[++k]),
														elem->mqinfo.u.WtpInfo.value1,	
														elem->mqinfo.u.WtpInfo.value3))) 
			{
				for(i = 0; i <= k; i++) 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;

		case WTP_BLACK_WHITE_LIST:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	

			if (!(CWAssembleMsgElemAPBlackWhiteList(&(msgElems[++k]),
										elem->mqinfo.u.WtpInfo.value[0],
										elem->mqinfo.u.WtpInfo.value[1],
										elem->mqinfo.u.WtpInfo.value[2],
										elem->mqinfo.u.WtpInfo.value[3],
										elem->mqinfo.u.WtpInfo.value[4],
										elem->mqinfo.u.WtpInfo.value[5],
										elem->mqinfo.u.WtpInfo.macarry))) 
			{													
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_ELECTRONIC_MENU:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			
			if (!(CWAssembleMsgElemAPElectronicMenu(&(msgElems[++k]),
											elem->mqinfo.u.WtpInfo.value[0],
											elem->mqinfo.u.WtpInfo.value[1],
											elem->mqinfo.u.WtpInfo.value[2],
											elem->mqinfo.u.WtpInfo.value[3]))) 
			{													
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_SET_AP_5G_FIRST:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAP5gJoinFirst(&(msgElems[++k]),
                                                 elem->mqinfo.u.WtpInfo.value[0],
                                                 elem->mqinfo.u.WtpInfo.value[1],
                                                 elem->mqinfo.u.WtpInfo.value3,
                                                 elem->mqinfo.u.WtpInfo.value2,
                                                 elem->mqinfo.u.WtpInfo.value[2],
                                                 elem->mqinfo.u.WtpInfo.value[3])))
			{
				for (i = 0; i <= k; i++) 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_STA_ACL_LIST:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWtpStaAclList(&(msgElems[++k]),
										elem->mqinfo.u.WtpInfo.value[0],
										elem->mqinfo.u.WtpInfo.value[1],
										elem->mqinfo.u.WtpInfo.value[2],
										elem->mqinfo.u.WtpInfo.value[3],										
										elem->mqinfo.u.WtpInfo.value[4]))) 
			{
				for(i = 0; i <= k; i++)
				{
				    CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_QOS_MAP_INFO:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAPQosMapInfo(&(msgElems[++k]),
														elem->mqinfo.u.WtpInfo.value1,
														elem->mqinfo.u.WtpInfo.value4,
														elem->mqinfo.u.WtpInfo.value3,
														elem->mqinfo.u.WtpInfo.value2,
														elem->mqinfo.u.WtpInfo.value))) 
			{
				for(i = 0; i <= k; i++) 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_QOS_MAP_SWITCH:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemAPQosMapSwitch(&(msgElems[++k]),
														elem->mqinfo.u.WtpInfo.value1,
														elem->mqinfo.u.WtpInfo.value4,
														elem->mqinfo.u.WtpInfo.value3,
														elem->mqinfo.u.WtpInfo.value2))) 
			{
				for(i = 0; i <= k; i++) 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
				
		case WLAN_LOAD_BALANCE:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemWLANloadbalance(&(msgElems[++k]),
														elem->mqinfo.u.WtpInfo.c1,	
														elem->mqinfo.u.WtpInfo.c2,												
														elem->mqinfo.u.WtpInfo.value1)))										
			{
				for(i = 0; i <= k; i++) 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WTP_ROGUEAP_BLACK_LIST:
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleWtpRougeApList(&(msgElems[++k]),
			                            WTPIndex,
										elem->mqinfo.u.WtpInfo.i1,
										elem->mqinfo.u.WtpInfo.c1)))
			{
				for (i = 0; i <= k; i++)
				{
				    CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		        }
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
			
		case WLAN_TRAFFIC_LIMIT:
			level = elem->mqinfo.u.WtpInfo.c1;
			wlanid = elem->mqinfo.u.WtpInfo.wlanid;
			l_radioid = elem->mqinfo.u.WtpInfo.radioid;
			send_traffic_limit = elem->mqinfo.u.WtpInfo.i1;
			traffic_limit = elem->mqinfo.u.WtpInfo.i2;
			
			if (level)
			{
				WID_CHECK_L_RADIOID_STANDARD_RET(l_radioid, CW_FALSE);
			}
			else
			{
				l_radioid |= 0x80;
			}
			
			MsgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemTrafficLimit(&(msgElems[++k]),
					WTPIndex, l_radioid, wlanid, send_traffic_limit, traffic_limit))) 
			{
				for (i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; 
			}
			break;
		default:
			return CW_FALSE;
	}
	if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST, msgElems, MsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
	    return CW_FALSE;
	}
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
						WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST), seqNum, MsgElemCount);
	return CW_TRUE;
}

CWBool CWAssembleConfigurationUpdateRequest
(
	CWProtocolMessage **messagesPtr, 
	int *fragmentsNumPtr, 
	int PMTU, 
	int seqNum,
	int WTPIndex
) 
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 0;
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}

	if(!CWBindingAssembleConfigurationUpdateRequest(&msgElemsBinding, &msgElemBindingCount))
	{
		return CW_FALSE;
	}

	if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST, msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
						WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST), seqNum, msgElemCount);	
	return CW_TRUE;
}

CWBool CWStartNeighborDeadTimer(int WTPIndex)
{
	if(!CWErr(CWTimerRequest(gCWNeighborDeadInterval/3, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex)))  // start NeighborDeadInterval timer
	{
		return CW_FALSE;
	}
	return CW_TRUE;
}

CWBool CWStartNeighborDeadTimerForEcho(int WTPIndex)
{
	int echoInterval = 0;

	CWACGetEchoRequestTimer(&echoInterval);
	if (!CWErr(CWTimerRequest(echoInterval, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex)))  // start NeighborDeadInterval timer
	{
		return CW_FALSE;
	}
	return CW_TRUE;
}

CWBool CWStopNeighborDeadTimer(int WTPIndex)
{
	AC_WTP[WTPIndex]->neighbordeatimes = 0;
	if (!CWTimerCancel(&(gWTPs[WTPIndex].currentTimer),1))
	{
		return CW_FALSE;
	}
	return CW_TRUE;
}

CWBool CWRestartNeighborDeadTimer(int WTPIndex)
{
	CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
	if (!CWStopNeighborDeadTimer(WTPIndex))
	{
		return CW_FALSE;
	}
	if (!CWStartNeighborDeadTimer(WTPIndex))
	{
		return CW_FALSE;
	}
	CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"timer neighbor-dead restarted", \
					WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME);
	return CW_TRUE;
}

CWBool CWRestartNeighborDeadTimerForEcho(int WTPIndex)
{
	CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
	if (!CWStopNeighborDeadTimer(WTPIndex))
	{
		return CW_FALSE;
	}
	if (!CWStartNeighborDeadTimerForEcho(WTPIndex))
	{
		return CW_FALSE;
	}
	CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"timer neighbor-dead restarted for echo", \
					WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME);
	return CW_TRUE;
}

CWBool CWAssembleMsgElemIEEE80211Info(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int wlanid, struct msgqlist *elem)
{
	unsigned int key_mgmt = 0;
	unsigned int cipher = 0;
	unsigned int BSSIndex = 0;	
	unsigned int pre_auth = 0;
	int size = 39;
	int id = wlanid;
	
	if(elem->mqinfo.subtype == STA_S_TYPE)
	{
		cipher = elem->mqinfo.u.StaInfo.cipher;
		BSSIndex = elem->mqinfo.u.StaInfo.BSSIndex;
		RadioID = (BSSIndex/L_BSS_NUM)%L_RADIO_NUM;
	}
		
	wid_syslog_debug_debug(WID_WTPINFO,"%s:%d radio%d-%d wlan%d cipher %d\n",__func__,__LINE__,WTPIndex,RadioID,wlanid,cipher);
	
	if((wlanid > 0)&&(AC_WLAN[wlanid] != NULL))
	{
		switch(AC_WLAN[wlanid]->SecurityType)
		{
			case OPEN :
			{
				if(AC_WLAN[wlanid]->EncryptionType == WEP)
				{
					if((AC_WLAN[wlanid]->KeyLen == 5)||(AC_WLAN[wlanid]->KeyLen == 10))
					{
						key_mgmt = WID_WPA_KEY_MGMT_NONE;
						cipher = WID_WPA_CIPHER_WEP40;
					}
					else if((AC_WLAN[wlanid]->KeyLen == 13)||(AC_WLAN[wlanid]->KeyLen == 26))
					{
						key_mgmt = WID_WPA_KEY_MGMT_NONE;
						cipher = WID_WPA_CIPHER_WEP104;
					}
					else if((AC_WLAN[wlanid]->KeyLen == 16)||(AC_WLAN[wlanid]->KeyLen == 32))
					{
						key_mgmt = WID_WPA_KEY_MGMT_NONE;
						cipher = WID_WPA_CIPHER_WEP128;
					}else
						return CW_FALSE;
				}
				else
				{
					key_mgmt = WID_WPA_KEY_MGMT_NONE;
					cipher = WID_WPA_CIPHER_NONE;
				}
				break;
			}
			case SHARED:
			{
				if((AC_WLAN[wlanid]->KeyLen == 5)||(AC_WLAN[wlanid]->KeyLen == 10))
				{
					key_mgmt = WID_WPA_KEY_MGMT_SHARED;
					cipher = WID_WPA_CIPHER_WEP40;
				}
				else if((AC_WLAN[wlanid]->KeyLen == 13)||(AC_WLAN[wlanid]->KeyLen == 26))
				{
					key_mgmt = WID_WPA_KEY_MGMT_SHARED;
					cipher = WID_WPA_CIPHER_WEP104;
				}
				else if((AC_WLAN[wlanid]->KeyLen == 16)||(AC_WLAN[wlanid]->KeyLen == 32))
				{
					key_mgmt = WID_WPA_KEY_MGMT_SHARED;
					cipher = WID_WPA_CIPHER_WEP128;
				}
				else
				{
				    return CW_FALSE;
				}    
				break;
			}	
			case IEEE8021X :
			{
				key_mgmt = WID_WPA_KEY_MGMT_IEEE8021X_NO_WPA;
				cipher = WID_WPA_CIPHER_WEP40;
				break;
			}	
			case WPA_P :
			{
				key_mgmt = WID_WPA_KEY_MGMT_PSK;
				if(AC_WLAN[wlanid]->EncryptionType == TKIP)
				{
				    cipher = WID_WPA_CIPHER_TKIP;
				}
				else if(AC_WLAN[wlanid]->EncryptionType == AES)
				{
				    cipher = WID_WPA_CIPHER_CCMP;
				}
				break;
			}	
			case WPA2_P :
			{
				key_mgmt = WID_WPA2_KEY_MGMT_PSK;
				if(AC_WLAN[wlanid]->EncryptionType == TKIP)
				{
				    cipher = WID_WPA_CIPHER_TKIP;
				}
				else if(AC_WLAN[wlanid]->EncryptionType == AES)
				{
				    cipher = WID_WPA_CIPHER_CCMP;
				}
				break;
			}
			case WPA_E :
			{
				key_mgmt = WID_WPA_KEY_MGMT_IEEE8021X;
				if(AC_WLAN[wlanid]->EncryptionType == TKIP)
				{
				    cipher = WID_WPA_CIPHER_TKIP;
				}
				else if(AC_WLAN[wlanid]->EncryptionType == AES)
				{
				    cipher = WID_WPA_CIPHER_CCMP;
				}
				break;
			}
			case WPA2_E :
			{
				pre_auth = AC_WLAN[wlanid]->PreAuth;
				key_mgmt = WID_WPA2_KEY_MGMT_IEEE8021X;
				if(AC_WLAN[wlanid]->EncryptionType == TKIP)
				{
				    cipher = WID_WPA_CIPHER_TKIP;
				}
				else if(AC_WLAN[wlanid]->EncryptionType == AES)
				{
				    cipher = WID_WPA_CIPHER_CCMP;
				}
				break;
			}
			//add for WAPI
			case WAPI_PSK:
			{
			    key_mgmt = WID_WAPI_KEY_MGMT_PSK;
				cipher = WID_WAPI_CIPHER_SMS4;
				break;
			}
			case WAPI_AUTH:
			{
			    key_mgmt = WID_WAPI_KEY_MGMT_CER;
				cipher = WID_WAPI_CIPHER_SMS4;
				break;
			}
			default : break;
		}
	}
	else if(elem->mqinfo.subtype == STA_S_TYPE && AC_BSS[BSSIndex] != NULL)
	{
		id = AC_BSS[BSSIndex]->WlanID;
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CWProtocolStore8(msgPtr, RadioID);
	CWProtocolStore8(msgPtr, id);	
	CWProtocolStore8(msgPtr, 0);
	
	CWProtocolStore8(msgPtr, 221);
	CWProtocolStore8(msgPtr, 34);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);	
	CWProtocolStore32(msgPtr, 0);	
	CWProtocolStore32(msgPtr, cipher);
	CWProtocolStore32(msgPtr, cipher);
	CWProtocolStore32(msgPtr, key_mgmt);
	CWProtocolStore32(msgPtr, pre_auth);
	CWProtocolStore32(msgPtr, 0);
	CWProtocolStore32(msgPtr, 0);	
//	CWCaptrue(msgPtr->offset,msgPtr->msg);

	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_IEEE80211_INFO_ELEMENT);
}

CWBool CWAssembleMsgElemAddWlan(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int wlanid, int op, struct msgqlist *elem)
{
	unsigned int BSSIndex = 0;
	if (!check_wlanid_func(wlanid))
	{
		wid_syslog_err("%s-%d wlanid= %d\n", __func__, __LINE__, wlanid);
		return CW_FALSE;
	}
	
	MQ_WLAN *wlaninfo = (MQ_WLAN *)malloc(sizeof(MQ_WLAN));
	if (!wlaninfo)
	{
		wid_syslog_err("%s-%d malloc for wlaninfo failed!\n", __func__, __LINE__);
		return CW_FALSE;
	}
	memset(wlaninfo, 0, sizeof(MQ_WLAN));
	memcpy(wlaninfo, &(elem->mqinfo.u.WlanInfo), sizeof(MQ_WLAN));
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s-%d radio%d-%d bssindex%d HideESSid:%d SecurityType:%d SecurityIndex:%d\n",
										__func__, __LINE__, WTPIndex, RadioID, wlaninfo->bssindex,
										wlaninfo->HideESSid, wlaninfo->SecurityType, wlaninfo->SecurityIndex);
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s-%d asic_hex:%d KeyLen:%d Roaming_Policy:%d essid:%s\n",
										__func__, __LINE__, wlaninfo->asic_hex, wlaninfo->KeyLen, wlaninfo->Roaming_Policy, wlaninfo->WlanEssid);
	
	if (AC_WLAN[wlanid] == NULL)
	{
		AC_WTP[WTPIndex]->CMD->wlanCMD -= 1;
		if (AC_WTP[WTPIndex]->CMD->wlanCMD < 0)
		{
			AC_WTP[WTPIndex]->CMD->wlanCMD = 0;
		}
		
		CW_FREE_OBJECT(wlaninfo);	
		return CW_FALSE;
	}
	if (wlaninfo->LEVEL)
	{
		BSSIndex = wlaninfo->bssindex;
		if ((0 == BSSIndex) || (!check_bssid_func(BSSIndex)))
		{
			wid_syslog_err("<error> %s-%d radio%d-%d wlan%d bssindex=%d\n",__func__, __LINE__, WTPIndex, RadioID, wlanid, BSSIndex);
			CW_FREE_OBJECT(wlaninfo);
			return CW_FALSE;
		}
	}
	if (op == WLAN_ADD)
	{
		int size = 19;
		unsigned char policy = 0;
		unsigned char tunnel = 0;
		wid_syslog_debug_debug(WID_WTPINFO,"%s-%d wtp%d create wlan%d\n", __func__, __LINE__, WTPIndex, wlanid);
		
		AC_WTP[WTPIndex]->CMD->wlanCMD -= 1;
		
		if (wlaninfo->LEVEL)
		{
			AC_WTP[WTPIndex]->CMD->radiowlanid[RadioID][wlanid] = 2;
			
			int k1 = 0;
			for (k1 = 0; k1 < L_BSS_NUM; k1++)
			{
				if ((AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1] != NULL)
					&& (AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1]->WlanID == wlanid))
				{
					policy = AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1]->BSS_IF_POLICY;
					tunnel = AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1]->BSS_TUNNEL_POLICY;
					break;
				}
			}
		}
		else
		{
			int j = 0;
			for (j = 0; ((j < L_RADIO_NUM) && (AC_WTP[WTPIndex]) && (j < AC_WTP[WTPIndex]->RadioCount)); j++)
			{
				AC_WTP[WTPIndex]->CMD->radiowlanid[j][wlanid] = 2;
			}
		}
		char essid[ESSID_UTF8_DEFAULT_LEN*2+3] = {0};
		memcpy(essid, wlaninfo->WlanEssid, ESSID_LENGTH);
		unsigned char ssidbuffer[DEFAULT_LEN] = {0}; 
		wid_hex_dump_essid((unsigned char *)essid, strlen(essid), ssidbuffer);
		
		wid_syslog_info("%s-%d wlan%d cn flag: %d cn utf8 essid: %s %s\n", __func__, __LINE__, wlanid, AC_WLAN[wlanid]->chinaEssid, ssidbuffer, essid);
		
		size += wlaninfo->KeyLen;
		size += strlen(essid);
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, {CW_FREE_OBJECT(wlaninfo);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
		CWProtocolStore8(msgPtr, RadioID);
		CWProtocolStore8(msgPtr, wlanid);
		if (wlaninfo->Roaming_Policy == 1)		 //wuwl change for muti thread op
		{
			CWProtocolStore16(msgPtr, 1);
		}
		else
		{
			CWProtocolStore16(msgPtr, 0);
		}
		if (wlaninfo->LEVEL)
		{
			unsigned int l_bss_id = BSSIndex%L_BSS_NUM+1;
			if (AC_BSS[BSSIndex] == NULL)
			{
				wid_syslog_debug_debug(WID_WTPINFO, WAI_FUNC_LINE_FMT"bssindex %d\n", WAI_FUNC_LINE_LITERAL, BSSIndex);
				CWProtocolStore8(msgPtr,0);
			}
			else
			{
				AC_BSS[BSSIndex]->keyindex = wlaninfo->SecurityIndex;
				CWProtocolStore8(msgPtr, wlaninfo->SecurityIndex);//sz change l_bss_id to keyindex 05-20
			}
			wid_syslog_debug_debug(WID_WTPINFO, WAI_FUNC_LINE_FMT"wid send l_bss_id %d\n", WAI_FUNC_LINE_LITERAL, l_bss_id);
		}
		else
		{
			CWProtocolStore8(msgPtr, wlaninfo->SecurityIndex);//sz change l_bss_id to keyindex 05-20
		}
		
		if (wlaninfo->SecurityType == WID_WAPI_KEY_MGMT_PSK)
		{
			if (wlaninfo->asic_hex == 1)
			{
				CWProtocolStore8(msgPtr, 1);
			}
			else
			{
				CWProtocolStore8(msgPtr, 0);
			}
		}
		else
		{
			CWProtocolStore8(msgPtr, 0);
		}
		CWProtocolStore16(msgPtr, wlaninfo->KeyLen);
		if (wlaninfo->KeyLen != 0)
		{
			CWProtocolStoreRawBytes(msgPtr,wlaninfo->WlanKey,wlaninfo->KeyLen);
		}
		CWProtocolStore32(msgPtr, 0);
		CWProtocolStore16(msgPtr, 0);
		CWProtocolStore8(msgPtr, 0);
		CWProtocolStore8(msgPtr, 0);
		
		unsigned int wlan_tunnel_switch = AC_WLAN[wlanid]->wlan_tunnel_switch;
		if ((policy == NO_INTERFACE) || (wlan_tunnel_switch == 1))
		{
			CWProtocolStore8(msgPtr, CW_LOCAL_MAC);	
			CWProtocolStore8(msgPtr, CW_LOCAL_BRIDGING);
		}
		else
		{
			CWProtocolStore8(msgPtr, CW_SPLIT_MAC);	
			if (tunnel == CW_802_IPIP_TUNNEL)
			{
				CWProtocolStore8(msgPtr, CW_802_IPIP_TUNNEL);
			}
			else if (tunnel == CW_802_DOT_3_TUNNEL)
			{
				CWProtocolStore8(msgPtr,CW_802_DOT_3_TUNNEL);/*802.3*/
				wid_syslog_info(WAI_FUNC_LINE_FMT" 802.3.\n", WAI_FUNC_LINE_LITERAL);
			}
			else
			{
				CWProtocolStore8(msgPtr, CW_802_DOT_11_TUNNEL);
			}
		}
		CWProtocolStore8(msgPtr, wlaninfo->HideESSid);
		CWProtocolStoreStr(msgPtr, essid);
		
		wid_syslog_info(WAI_FUNC_LINE_FMT"essid: %s\n", WAI_FUNC_LINE_LITERAL, essid);
		
		CW_FREE_OBJECT(wlaninfo);
		
		return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_ADD_WLAN);
	}
	else if (op == WLAN_DEL)
	{
		wid_syslog_debug_debug(WID_WTPINFO,WAI_FUNC_LINE_FMT"wtp%d disable wlan%d\n", WAI_FUNC_LINE_LITERAL, WTPIndex, wlanid);
		int size = 2;
		
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, {CW_FREE_OBJECT(wlaninfo);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		
		CWProtocolStore8(msgPtr, RadioID);
		CWProtocolStore8(msgPtr, wlanid);
		
		CW_FREE_OBJECT(wlaninfo);
		
		return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_DELETE_WLAN);
	}
	else if (op == WLAN_CHANGE_TUNNEL)
	{
		int size = 4;
		wid_syslog_debug_debug(WID_WTPINFO,"%s: wtp%d update wlan%d\n",__func__,WTPIndex,wlanid);
		
		int k1 = 0;
		unsigned char policy = 0;
		unsigned char tunnel = 0;
		for (k1 = 0; k1 < L_BSS_NUM; k1++)
		{
			if ((AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1] != NULL) && (AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1]->WlanID == wlanid))
			{
				policy = AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1]->BSS_IF_POLICY;
				tunnel = AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[k1]->BSS_TUNNEL_POLICY;
				break;
			}
		}
		
		if (policy == NO_INTERFACE)
		{
			CW_FREE_OBJECT(wlaninfo);
			return CW_FALSE;
		}
		
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, {CW_FREE_OBJECT(wlaninfo);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		
		CWProtocolStore8(msgPtr, RadioID);
		CWProtocolStore8(msgPtr, wlanid);
		
		CWProtocolStore8(msgPtr, CW_SPLIT_MAC);	
		if (tunnel == CW_802_IPIP_TUNNEL)
		{
			CWProtocolStore8(msgPtr, CW_802_IPIP_TUNNEL);
		}
		else if (tunnel == CW_802_DOT_3_TUNNEL)
		{
			CWProtocolStore8(msgPtr,CW_802_DOT_3_TUNNEL);/*802.3*/
			wid_syslog_info("%s,%d,802.3.\n",__func__,__LINE__);
		}
		else
		{
			CWProtocolStore8(msgPtr, CW_802_DOT_11_TUNNEL);
		}
		
		CW_FREE_OBJECT(wlaninfo);
		
		return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_CHANGE_TUNNEL_MODE);
	}
	
	CW_FREE_OBJECT(wlaninfo);
	
	return CW_FALSE;
}


CWBool CWAssembleMsgElemWlanVlanPriority(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int wlanid, int level)
{
	int size = 6;
	unsigned int val = 0;
	//parse bss vlan info	
	int i = 0;
	unsigned char priority = 0;
	unsigned int vlan_id = 0;
	
	if (level)
	{
    	for (i = 0; i < L_BSS_NUM; i++)
    	{
    		if (AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[i] != NULL)
    		{
    			if (AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[i]->WlanID == wlanid)
    			{
    				if (AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[i]->BSS_IF_POLICY != NO_INTERFACE)
    				{
    					if ((AC_WLAN[wlanid] != NULL) && (AC_WLAN[wlanid]->wlan_tunnel_switch == 0))
    					{
    						break;
    					}	
    				}
    				if (AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[i]->vlanid != 0)
    				{
    					vlan_id = AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[i]->vlanid;
    				}
    				else
    				{
    					vlan_id = AC_WTP[WTPIndex]->WTP_Radio[RadioID]->BSS[i]->wlan_vlanid;
    				}
    				priority = AC_WLAN[wlanid]->wlan_1p_priority;
    				break;
    			}
    		}
    	}
    }
    else
    {
        if (AC_WLAN[wlanid] != NULL)
        {
            vlan_id = AC_WLAN[wlanid]->vlanid;
            priority = AC_WLAN[wlanid]->wlan_1p_priority;
        }
    }
	wid_syslog_debug_debug(WID_WTPINFO,"%s:%d radio%d-%d wlan%d vlan %d wlan_1p_priority %d\n",__func__,__LINE__,WTPIndex,RadioID,wlanid,vlan_id,priority);

	if (AC_WLAN[wlanid] == NULL)
	{
		return CW_FALSE;
	}

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, RadioID);
	CWProtocolStore8(msgPtr, wlanid);
	CWSetField32(val,
			 0,
			 16,
			 0x8100);
	CWSetField32(val,
			 16,
			 3,
			 priority);
	CWSetField32(val,
			 19,
			 1,
			 0);
	CWSetField32(val,
			 20,
			 12,
			 vlan_id);
	CWProtocolStore32(msgPtr, val);	

	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_IEEE80211_WLAN_VLAN_INFO);
}

CWBool CWAssembleMsgElemWlanWapiCerInfo(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int id)
{
	int size = 0;
	unsigned int lenth = 0;
	
	if (AC_WLAN[id] == NULL)
	{
		return CW_FALSE;
	}
	
	lenth = (AC_WLAN[id]->AECerLen + AC_WLAN[id]->ASCerLen + AC_WLAN[id]->IpLen)+12;
	size = lenth;
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s radio%d-%d wlan%d lenth %d\n",__func__,WTPIndex,RadioID,id,lenth);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	//CWProtocolStore16(msgPtr, BINDING_MSG_ELEMENT_TYPE_WAPI_CER_INFO_ELEMENT);
	//CWProtocolStore16(msgPtr, lenth);
	CWProtocolStore16(msgPtr, 0);//as path 
	CWProtocolStore16(msgPtr, AC_WLAN[id]->ASCerLen);
	CWProtocolStoreRawBytes(msgPtr,AC_WLAN[id]->ASCerPath,AC_WLAN[id]->ASCerLen);
	//printf("AC_WLAN[id]->ASCerLen %d AC_WLAN[id]->ASCerPath %s\n",AC_WLAN[id]->ASCerLen,AC_WLAN[id]->ASCerPath);
	
	CWProtocolStore16(msgPtr, 1); //ae path
	CWProtocolStore16(msgPtr, AC_WLAN[id]->AECerLen);
	CWProtocolStoreRawBytes(msgPtr,AC_WLAN[id]->AECerPath,AC_WLAN[id]->AECerLen);
	//printf("AC_WLAN[id]->AECerLen %d AC_WLAN[id]->AECerPath %s\n",AC_WLAN[id]->AECerLen,AC_WLAN[id]->AECerPath);
	
	CWProtocolStore16(msgPtr, 2); //as ip
	CWProtocolStore16(msgPtr, AC_WLAN[id]->IpLen);
	CWProtocolStoreRawBytes(msgPtr,AC_WLAN[id]->AsIp,AC_WLAN[id]->IpLen);
	//printf("AC_WLAN[id]->asip %d AC_WLAN[id]->asip %s\n",AC_WLAN[id]->IpLen,AC_WLAN[id]->AsIp);
	
	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_WAPI_CER_INFO_ELEMENT);
}

CWBool CWAssembleMsgElemMulticastSecretKey(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int id, int op, struct msgqlist *elem)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Element ID         |          Element Length       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    radio id   |    wlan id    |          passwd length       	|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                   		passwd 								|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       			    group rekey time						|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
	short int length = 0;
	short int elementid = CWMULKEY_UPDATE_CYCLE; 
	short int passwd_length = 0;
	char passwd[DEFAULT_LEN] = {0};
	int group_rekey_time = 0;

	if (op != WLAN_ADD)
	{
		return CW_FALSE;
	}

	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}		

	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(id, CW_FALSE);
	if (elem->mqinfo.u.WlanInfo.LEVEL)
	{
	    WID_CHECK_L_RADIOID_STANDARD_RET(RadioID, CW_FALSE);
	}
	unsigned int type = elem->mqinfo.u.WlanInfo.SecurityType;
	passwd_length = elem->mqinfo.u.WlanInfo.KeyLen;
	memcpy(passwd, elem->mqinfo.u.WlanInfo.WlanKey, elem->mqinfo.u.WlanInfo.KeyLen);
	
	if ((WPA_E == type) || (WPA2_E == type))
	{
		passwd_length = 0;
	}
	group_rekey_time = elem->mqinfo.u.WlanInfo.wpa_group_rekey;
	length = passwd_length + 12;
	wid_syslog_debug_debug(WID_WTPINFO, "%s radioid %d wtpindex %d wlanid %d\n",__func__, RadioID, WTPIndex, id);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length - 4));	
	CWProtocolStore8(msgPtr, RadioID); 
	CWProtocolStore8(msgPtr, id);
	CWProtocolStore16(msgPtr, passwd_length);
	if (0 != passwd_length)
	{
		CWProtocolStoreRawBytes(msgPtr, passwd, passwd_length); 
	}
	CWProtocolStore32(msgPtr, group_rekey_time); 	
	
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);	
}


CWBool CWAssembleMsgElemTrafficLimit
(
	CWProtocolMessage *msgPtr,
	int WTPIndex,
	unsigned char RadioID,
	int wlanid,
	unsigned int send_traffic_limit,
	unsigned int traffic_limit
)
{
	/*
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|            Element ID         |         Element Length        |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|    operation  |    radio id   |    wlan id    |    reserved   |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                       send_traffic_limit                      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                         traffic_limit                         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	short int length = 16;
	short int elementid = CW_VENDOR_SPEC_ELEMENT_WLAN_TRAFFIC_LIMIT; 
	
	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid);
	CWProtocolStore16(msgPtr, (length - 4));
	CWProtocolStore8(msgPtr, LEVEL_WLAN);
	CWProtocolStore8(msgPtr, RadioID);
	CWProtocolStore8(msgPtr, wlanid);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore32(msgPtr, send_traffic_limit);
	CWProtocolStore32(msgPtr, traffic_limit);
	
	wid_syslog_debug_debug(WID_WTPINFO, "%s-%d wtp%d radio%d wlan%d send_traffic_limit %d traffic_limit %d\n",
							__func__, __LINE__, WTPIndex, RadioID, wlanid, send_traffic_limit, traffic_limit);
	
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE); 
}


CWBool CWAssembleMsgElemConfTemplate(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int wlanid, struct msgqlist *elem)
{
	/*
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|            Element ID         |         Element Length        |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|    radio id   |    wlan id    |         forwarding_mode       |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                          gateway-ip                           |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                          gateway-mask                         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	short int length = 16;
	short int elementid = CW_VENDOR_SPEC_ELEMENT_WLAN_CONFIG_SCHEME; 
	short int confschemeid = 0;
	short int forward_mode = 0;
	
	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}       
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);
	
	confschemeid = AC_WLAN[wlanid]->confschemeid;
	if (confschemeid)
	{
		forward_mode = AC_CONFSCHME[confschemeid]->forward_mode;
	}
	else
	{
		forward_mode = CW_FWMODE_BRIDGE_UNTAGGED;
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length - 4));    
	CWProtocolStore8(msgPtr, RadioID); 
	CWProtocolStore8(msgPtr, wlanid);
	CWProtocolStore16(msgPtr, forward_mode);
	
	if ((confschemeid) && (CW_FWMODE_ROUTER != AC_CONFSCHME[confschemeid]->forward_mode))
	{
		CWProtocolStore32(msgPtr, 0);
		
		CWProtocolStore32(msgPtr, 0);
	}
	else
	{
		CWProtocolStore32(msgPtr, AC_WLAN[wlanid]->gateway_ip);
		
	 	CWProtocolStore32(msgPtr, AC_WLAN[wlanid]->gateway_mask);
	}
	
	wid_syslog_debug_debug(WID_WTPINFO, "%s-%d radio%d wtp%d wlan%d confschemeid %d forward_mode %d\n",
							WAI_FUNC_LINE_LITERAL,RadioID,WTPIndex,wlanid,confschemeid,forward_mode);
	
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE); 
}

CWBool CWAssembleMsgElemWlanPortalInfo(CWProtocolMessage *msgPtr, int WTPIndex, unsigned char RadioID, int wlanid)
{
	/*
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|            Element ID         |          Element Length       |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|    radio id   |    wlan id    |      method   |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	short int elementid = CW_VENDOR_SPEC_ELEMENT_WLAN_PORTAL_INFO; 
	short int length = 8;
	
	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length - 4));
	CWProtocolStore8(msgPtr, RadioID); 
	CWProtocolStore8(msgPtr, wlanid); 
	CWProtocolStore8(msgPtr, AC_WLAN[wlanid]->guest_flag);
	CWProtocolStore8(msgPtr, 0);
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s len%d wtp%d radio%d wlan%d %s guest network\n",
			__func__, length, WTPIndex, RadioID, wlanid, AC_WLAN[wlanid]->guest_flag ? "enable":"disable");
	
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);	
}

CWBool CWAssembleMsgElemStaOp(CWProtocolMessage *msgPtr, int WTPIndex, struct msgqlist *elem)
{
	int size = 23;
	unsigned char tmpval = 6;
	int tmpvalint = 6;
	CWProtocolMessage msgPtr2;
	unsigned short  len = 8;
	unsigned short vendorvalue ;
	unsigned short sta_num = 0;
	CWBool  ret = CW_FALSE; 
	vendorvalue = 30;
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.Radio_L_ID);
	CWProtocolStore8(msgPtr, tmpval);
	CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.STAMAC), tmpvalint);
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.WLANID);
	if(elem->mqinfo.u.StaInfo.Sta_Op == Sta_ADD)
	{
		unsigned int	wtpid;
		unsigned int 	l_radioid;
		unsigned char 	wlanid;
		unsigned char 	mac[MAC_LEN];
		unsigned int 	value, send_value, vlan_id;
		wtpid = WTPIndex;
		l_radioid = elem->mqinfo.u.StaInfo.Radio_L_ID;
		wlanid = elem->mqinfo.u.StaInfo.WLANID;
		memcpy(mac,elem->mqinfo.u.StaInfo.STAMAC,MAC_LEN);
		value = elem->mqinfo.u.StaInfo.traffic_limit;
		send_value = elem->mqinfo.u.StaInfo.send_traffic_limit;
		vlan_id = elem->mqinfo.u.StaInfo.vlan_id;
		wid_syslog_debug_debug(WID_DEFAULT,"\n %s\n",__func__);					
		wid_syslog_debug_debug(WID_DEFAULT,"traffic_limit %d\n",value); 				
		wid_syslog_debug_debug(WID_DEFAULT,"send_traffic_limit %d\n",send_value);					
		wid_syslog_debug_debug(WID_DEFAULT,"vlan_id %d\n",vlan_id); 				
		if(value != 0)
			wid_set_bss_traffic_limit_sta_value(wtpid,l_radioid,wlanid,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],value,0);
		if(send_value != 0)
			wid_set_bss_traffic_limit_sta_value(wtpid,l_radioid,wlanid,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],send_value,1);
		if(vlan_id > 0)
			wid_set_sta_vlan_id(wtpid,l_radioid,wlanid,mac,vlan_id);
		//weichao add for check sta num
		wid_syslog_debug_debug(WID_WTPINFO,"\nmsg ofset = %d!\n",__func__,msgPtr->offset);							
		wid_syslog_debug_debug(WID_WTPINFO,"\n %s:now go go go!\n",__func__);					
		
		sta_num = elem->mqinfo.u.StaInfo.sta_num;
		
		wid_syslog_debug_debug(WID_WTPINFO,"\n %s:sta_num :%d!\n",__func__,elem->mqinfo.u.StaInfo.sta_num);					
		wid_syslog_debug_debug(WID_WTPINFO,"\n %s:sta_num :%d!\n",__func__,sta_num);					
		CW_CREATE_PROTOCOL_MESSAGE(msgPtr2, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		wid_syslog_debug_debug(WID_WTPINFO,"1111111111111\n");
		CWProtocolStore16(&msgPtr2,vendorvalue);
		CWProtocolStore16(&msgPtr2,len-4);
		CWProtocolStore16(&msgPtr2, sta_num);
		CWProtocolStore16(&msgPtr2,0);//reserved
		CWAssembleMsgElemVendor(&msgPtr2,CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
		CWProtocolStoreMessage(msgPtr,&msgPtr2);
		CW_FREE_PROTOCOL_MESSAGE(msgPtr2);
		//end
		
		wid_syslog_debug_debug(WID_WTPINFO,"\n %s:now go end!\n",__func__);					
		wid_syslog_debug_debug(WID_WTPINFO,"\nmsg ofset = %d!\n",__func__,msgPtr->offset);							
		ret = CWAssembleMsgElemAddVendor(msgPtr, CW_MSG_ELEMENT_ADD_STATION_CW_TYPE);
		return ret;
	}
	else if(elem->mqinfo.u.StaInfo.Sta_Op == Sta_DEL)
	{
		return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE);
	}
	else
	{
		return CW_FALSE;
	}
}
/*
CWBool CWAssembleMsgElemDeleteSta(CWProtocolMessage *msgPtr, int WTPIndex) {
	int size = 9;
	unsigned char tmpval = 6;
	int tmpvalint = 6;
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, AC_WTP[WTPIndex]->CMD->StaInf[0]);
	CWProtocolStore8(msgPtr, tmpval);
	CWProtocolStoreRawBytes(msgPtr, &(AC_WTP[WTPIndex]->CMD->StaInf[1]), tmpvalint);
	CWProtocolStore8(msgPtr, AC_WTP[WTPIndex]->CMD->StaInf[7]);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE);
}
*/

CWBool CWAssembleMsgElemSetStaPMK(CWProtocolMessage *msgPtr, int WTPIndex, struct msgqlist *elem)
{
	short int elementid = CW_VENDOR_CONFIG_ELEMENT_PMK; 
	short int length = 52;  /* 52bytes = element id (2bytes) + length(2bytes)
	                                   + radioid(1bytes) + WLANID(1bytes) + STA-MAC(6bytes)
	                                   + roamflag(1bytes) + BSSID(6bytes) + PMK(32bytes) 
							           + pmkid_len(1bytes) */
	unsigned char l_radioid = 0;
	unsigned char wlanid = 0;

	if ((NULL == msgPtr) || (NULL == elem))
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	} 

	if(elem->mqinfo.u.StaInfo.Sta_Op != Sta_SETPMK)
	{
		return CW_FALSE;
	}
	
	l_radioid = elem->mqinfo.u.StaInfo.Radio_L_ID;
	wlanid = elem->mqinfo.u.StaInfo.WLANID;
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);
	WID_CHECK_L_RADIOID_STANDARD_RET(l_radioid, CW_FALSE);	

	if (PMKID_LEN == elem->mqinfo.u.StaInfo.pmkid_len)
	{
		length += PMKID_LEN;
	}

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length - 4));
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.Radio_L_ID);
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.WLANID);
	CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.STAMAC), MAC_LEN);
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.roamflag);
	CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.BSSID), MAC_LEN);
	CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.PMK), PMK_LEN); 


	if (PMKID_LEN == elem->mqinfo.u.StaInfo.pmkid_len)
	{
		CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.pmkid_len);
		CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.pmkid), PMKID_LEN); 
	}
	else
	{
		CWProtocolStore8(msgPtr, 0);
	}

	wid_syslog_debug_debug(WID_DEFAULT,
		"ele %d len %d radio %d-%d wlan %d sta %02X:%02X:%02X:%02X:%02X:%02X\n",
		elementid, (length-4), WTPIndex, l_radioid, wlanid,
		elem->mqinfo.u.StaInfo.STAMAC[0], elem->mqinfo.u.StaInfo.STAMAC[1],
		elem->mqinfo.u.StaInfo.STAMAC[2], elem->mqinfo.u.StaInfo.STAMAC[3],
		elem->mqinfo.u.StaInfo.STAMAC[4], elem->mqinfo.u.StaInfo.STAMAC[5]);
	
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

#if 0
CWBool CWAssembleMsgElemSetStaPMKID(CWProtocolMessage *msgPtr, int WTPIndex, struct msgqlist *elem)
{
	unsigned char l_radioid = 0;
	unsigned char wlanid = 0;
	short elementid = CW_VENDOR_CONFIG_ELEMENT_PMKID; 
	short length = 13;  

	if ((NULL == msgPtr) || (NULL == elem))
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	} 
	
	if(Sta_SETPMK != elem->mqinfo.u.StaInfo.Sta_Op)
	{
		return CW_FALSE;
	}
	
	l_radioid = elem->mqinfo.u.StaInfo.Radio_L_ID;
	wlanid = elem->mqinfo.u.StaInfo.WLANID;
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);
	WID_CHECK_L_RADIOID_STANDARD_RET(l_radioid, CW_FALSE);	

	if (PMKID_LEN == elem->mqinfo.u.StaInfo.pmkid_len)
	{
		length += PMKID_LEN;
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length - 4));
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.Radio_L_ID);
	CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.WLANID);
	CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.STAMAC), MAC_LEN);

	if (PMKID_LEN == elem->mqinfo.u.StaInfo.pmkid_len)
	{
		CWProtocolStore8(msgPtr, elem->mqinfo.u.StaInfo.pmkid_len);
		CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.pmkid), PMKID_LEN); 
	}
	else
	{
		CWProtocolStore8(msgPtr, 0);
	}
	
	wid_syslog_debug_debug(WID_DEFAULT,
		"elem %d len %d radio %d-%d wlan %d sta %02X:%02X:%02X:%02X:%02X:%02X\n",
		elementid, (length-4), WTPIndex, l_radioid, wlanid,
		elem->mqinfo.u.StaInfo.STAMAC[0], elem->mqinfo.u.StaInfo.STAMAC[1],
		elem->mqinfo.u.StaInfo.STAMAC[2], elem->mqinfo.u.StaInfo.STAMAC[3],
		elem->mqinfo.u.StaInfo.STAMAC[4], elem->mqinfo.u.StaInfo.STAMAC[5]);
	
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

#endif
CWBool CWAssembleMsgElemSetStaKey(CWProtocolMessage *msgPtr, int WTPIndex, struct msgqlist *elem)
{
	int size = 20;
	int storesize = 6;
	size+= elem->mqinfo.u.StaInfo.keylen;
	//printf("key len %d\n",AC_WTP[WTPIndex]->CMD->key.key_len);
	unsigned int val = 0;
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreRawBytes(msgPtr, (char *)(elem->mqinfo.u.StaInfo.STAMAC), storesize);	
	CWSetField32(val,
			 0,
			 1,
			 1);	
	CWSetField32(val,
			 1,
			 1,
			 0);
	CWSetField32(val,
			 2,
			 2,
			 elem->mqinfo.u.StaInfo.keyidx);	
	CWSetField32(val,
			 4,
			 8,
			 elem->mqinfo.u.StaInfo.keylen);	
	CWSetField32(val,
			 12,
			 20,
			 0);
	CWProtocolStore32(msgPtr, val);	
	CWProtocolStore16(msgPtr, 0);
	CWProtocolStore32(msgPtr, 0);	
	CWProtocolStore32(msgPtr, 0);
	CWProtocolStoreRawBytes(msgPtr, elem->mqinfo.u.StaInfo.key,elem->mqinfo.u.StaInfo.keylen);
//	CWCaptrue(msgPtr->offset, msgPtr->msg);
	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_STATION_SESSION_KEY);
}


int RadioNumCheck(int WTPIndex){
	if(AC_WTP[WTPIndex] == NULL)
	{
		return 0;
	}
	return AC_WTP[WTPIndex]->RadioCount;

}

CWBool CWAssembleStaConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem)
{
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 1;	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	int k = -1;	
//	unsigned char m = 0;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	// Assemble Message Elements
	if((!(CWAssembleMsgElemStaOp(&(msgElems[++k]), WTPIndex, elem))))		
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	if(!CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST,  \
							msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
						WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST), seqNum, msgElemCount);
	return CW_TRUE;
}


CWBool CWAssembleStaConfigurationRequest_pmk(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem)
{
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 2;	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	int k = -1;	
	wlan_t wlanid = 0;
		
	if (messagesPtr == NULL || fragmentsNumPtr == NULL || NULL == elem)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}	

	if (Sta_SETPMK != elem->mqinfo.u.StaInfo.Sta_Op)
	{
		return CW_FALSE;
	}
	
	wlanid = elem->mqinfo.u.StaInfo.WLANID;
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);

	if (!(WID_SECURITY_TYPE_PMK(AC_WLAN[wlanid]->SecurityType)))
	{
		return CW_FALSE;
	}

	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	

	if (!CWAssembleMsgElemSetStaPMK(&(msgElems[++k]), WTPIndex, elem))
//		|| (!CWAssembleMsgElemSetStaPMKID(&(msgElems[++k]), WTPIndex, elem)))
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if(!CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST, \
						msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))
	{
		return CW_FALSE;
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
						WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST), seqNum, msgElemCount);
	return CW_TRUE;
}


CWBool CWAssembleStaConfigurationRequest_key(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem)
{
	CWProtocolMessage *msgElems= NULL;
	int msgElemCount = 2;	
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount = 0;
	int k = -1;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););		

	if ((!(CWAssembleMsgElemSetStaKey(&(msgElems[++k]), WTPIndex, elem)))
	    ||(!(CWAssembleMsgElemIEEE80211Info(&(msgElems[++k]), WTPIndex, 0, -1, elem))))
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST, \
						msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))
	{
		return CW_FALSE;
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", WAI_FUNC_LINE_LITERAL, \
						WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST), seqNum, msgElemCount);
	return CW_TRUE;
}

CWBool CWBindingAssembleWlanConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem)
{
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 4;	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	int k = -1, wlanid = 0, op = 0, level = 0;
	unsigned char l_radioid = 0;
	unsigned int type = 0;
	unsigned int send_traffic_limit = 0;
	unsigned int traffic_limit = 0;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	wlanid = elem->mqinfo.u.WlanInfo.WLANID;
	l_radioid = elem->mqinfo.u.WlanInfo.Radio_L_ID;
	op = elem->mqinfo.u.WlanInfo.Wlan_Op;
	type = elem->mqinfo.u.WlanInfo.SecurityType;
	level = elem->mqinfo.u.WlanInfo.LEVEL;
	send_traffic_limit = elem->mqinfo.u.WlanInfo.wlan_send_traffic_limit;
	traffic_limit = elem->mqinfo.u.WlanInfo.wlan_traffic_limit;
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET(wlanid, CW_FALSE);
	
	if (level)
	{
	    WID_CHECK_L_RADIOID_STANDARD_RET(l_radioid, CW_FALSE);
	}
	else
	{
    	l_radioid |= 0x80;
	}

	if (WLAN_ADD == op)
	{
		msgElemCount += 3;
		if (WID_SECURITY_TYPE_GTK(type))
		{
			msgElemCount++;
		}
	}

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	if ((!(CWAssembleMsgElemAddWlan(&(msgElems[++k]), WTPIndex, l_radioid, wlanid, op, elem)))
		|| (!(CWAssembleMsgElemIEEE80211Info(&(msgElems[++k]), WTPIndex, l_radioid, wlanid, elem)))
		|| (!(CWAssembleMsgElemWlanVlanPriority(&(msgElems[++k]), WTPIndex, l_radioid, wlanid, level)))
		|| (!(CWAssembleMsgElemWlanWapiCerInfo(&(msgElems[++k]), WTPIndex, l_radioid, wlanid)))
		|| ((WLAN_ADD == op) && (!(CWAssembleMsgElemConfTemplate(&(msgElems[++k]), WTPIndex, l_radioid, wlanid, elem))))
		|| ((WLAN_ADD == op) && (!(CWAssembleMsgElemTrafficLimit(&(msgElems[++k]), WTPIndex, l_radioid, wlanid, send_traffic_limit, traffic_limit))))
		|| ((WLAN_ADD == op)
			&& (WID_SECURITY_TYPE_GTK(type))
			&& (!(CWAssembleMsgElemMulticastSecretKey(&(msgElems[++k]), WTPIndex, l_radioid, wlanid, op, elem))))
		|| ((WLAN_ADD == op)
			&& (!(CWAssembleMsgElemWlanPortalInfo(&(msgElems[++k]), WTPIndex, l_radioid, wlanid)))))
	{
		int j = 0;
		for (j = 0; j <= k; j++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[j]);
		}
		CW_FREE_OBJECT(msgElems);		
		return CW_FALSE; // error will be handled by the caller
	}
	
	if (!CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum,
							CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_REQUEST,
							msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount,
							CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))
	{
		return CW_FALSE;
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex,  AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_REQUEST), seqNum, msgElemCount);
	return CW_TRUE;
}


CWBool CWParseMsgElemAssignedWTPBSSID(CWProtocolMessage *msgPtr, int len, WID_BSS *valPtr) 
{
	CWParseMessageElementStart();
	
	valPtr[0].Radio_L_ID = CWProtocolRetrieve8(msgPtr);
	valPtr[0].WlanID = CWProtocolRetrieve8(msgPtr);
	valPtr[0].BSSID = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr, 6);
	
    wid_syslog_debug_debug(WID_WTPINFO,"elemid 1024 radio %d wlan %d bssid "MACSTR"\n",valPtr[0].Radio_L_ID,valPtr[0].WlanID,MAC2STR(valPtr[0].BSSID));

	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemAssignedWTPBSSLIST
(
    CWProtocolMessage *msgPtr,
    int len,
    WID_BSS *valPtr,
    CWProtocolResultCode *resultCode,
    unsigned char *bss_num
) 
{
    int i = 0;
	CWParseMessageElementStart();
	
    *bss_num = CWProtocolRetrieve16(msgPtr);
    
    wid_syslog_debug_debug(WID_WTPINFO,"%s: elemid 1025 bssnum %d\n",__func__,*bss_num);
    for(i = 0; i < *bss_num; i++)
    {
    	valPtr[i].Radio_L_ID = CWProtocolRetrieve8(msgPtr);
    	valPtr[i].WlanID = CWProtocolRetrieve8(msgPtr);
    	valPtr[i].BSSID = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr, MAC_LEN);
    	resultCode[i] = CWProtocolRetrieve32(msgPtr);
    	
        wid_syslog_debug_debug(WID_WTPINFO,"radio %d wlan %d bssid "MACSTR" resultcode %d\n",
                                valPtr[i].Radio_L_ID,valPtr[i].WlanID,MAC2STR(valPtr[i].BSSID),resultCode[i]);
    }
	CWParseMessageElementEnd();
}

CWBool CWParseWlanConfigurationResponseMessage
(
    CWProtocolMessage* msgPtr,
    int len,
    CWProtocolResultCode *resultCode,
    WID_BSS *BSS,
    unsigned char *bss_num
)
{
	int offsetTillMessages = 0;
	//WID_BSS BSS;
	if(msgPtr == NULL || resultCode == NULL || bss_num == NULL || (msgPtr->msg) == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	offsetTillMessages = msgPtr->offset;
	wid_syslog_debug_debug(WID_WTPINFO,"Parsing WLAN Configuration Response...");

	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType)
		{
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				resultCode[0] = CWProtocolRetrieve32(msgPtr);
				
                wid_syslog_debug_debug(WID_WTPINFO,"elemid 33 result code %d",resultCode[0]);
                *bss_num = 1;
				break;
			case BINDING_MSG_ELEMENT_TYPE_ASSIGNED_WTP_BSSID:
				if(CWParseMsgElemAssignedWTPBSSID(msgPtr, elemLen, BSS))
				{
				    *bss_num = 1;
                    /*int i=0;
					for(i=0;i<4;i++)
					{
                        if((AC_RADIO[BSS.Radio_G_ID]->BSS[i] == NULL))
    						break;
                        else if(AC_RADIO[BSS.Radio_G_ID]->BSS[i]->WlanID == BSS.WlanID)
                        {
    						memcpy(AC_RADIO[BSS.Radio_G_ID]->BSS[i]->BSSID,BSS.BSSID,6);
    						printf("BSSID:%02x:%02x:%02x:%02x:%02x:%02x\n",BSS.BSSID[0],BSS.BSSID[1],BSS.BSSID[2],BSS.BSSID[3],BSS.BSSID[4],BSS.BSSID[5]);
                        }
                        else
                            i++;
                    }*/
				}
				break;
			
            case BINDING_MSG_ELEMENT_TYPE_ASSIGNED_WTP_BSSLIST:
                if(CWParseMsgElemAssignedWTPBSSLIST(msgPtr, elemLen, BSS, resultCode, bss_num))
                {
                }
                break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
    }
	wid_syslog_debug_debug(WID_WTPINFO,"WLAN Configuration Response Parsed");

	return CW_TRUE;	
}
/*
static int to_asd_or_not(unsigned int wtpid){
//	1	---	to asd
//	0	---	not
//	-1	---	error
	int i=0 , j=0;
	unsigned char wlanid=0;
	
	if(AC_WTP[wtpid] == NULL){
		return -1;
	}

	for(i=0;i<L_RADIO_NUM;i++){
		for(j=0;j<L_BSS_NUM;j++){
			if((AC_WTP[wtpid]->WTP_Radio[i]!=NULL)
				&&(AC_WTP[wtpid]->WTP_Radio[i]->BSS[j]!=NULL)){
				wlanid = AC_WTP[wtpid]->WTP_Radio[i]->BSS[j]->WlanID;

				if(	(AC_WLAN[wlanid]!=NULL)
					&&(AC_WLAN[wlanid]->balance_switch==1)
					&&(AC_WLAN[wlanid]->balance_method==2)){
					return 1;
				}
			}
		}
	}

	return 0;

}
*/

