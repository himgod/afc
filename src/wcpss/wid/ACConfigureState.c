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
* ACConfigureState.c
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
#include <syslog.h>
#include "CWAC.h"
#include "wcpss/waw.h"

#include "wcpss/wid/WID.h"
#include "ACDbus.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

int gCWChangeStatePendingTimer = CW_CHANGE_STATE_INTERVAL_DEFAULT;

CWBool CWAssembleConfigureResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex);
CWBool CWParseConfigureRequestMessage(char *msg, int len, int *seqNumPtr, CWProtocolConfigureRequestValues *valuesPtr);
CWBool CWSaveConfigureRequestMessage (CWProtocolConfigureRequestValues *configureRequest, CWWTPProtocolManager *WTPProtocolManager,unsigned int wtpindex);


CWBool ACEnterConfigure(int WTPIndex, CWProtocolMessage *msgPtr)
{
	int seqNum = 0;
	int i = 0;
	CWProtocolConfigureRequestValues configureRequest;
	
	if (!(CWParseConfigureRequestMessage(msgPtr->msg, msgPtr->offset, &seqNum, &configureRequest)))
	{
		return CW_FALSE; // note: we can kill our thread in case of out-of-memory error to free some space/
	 // we can see this just calling CWErrorGetLastErrorCode()
	}
	
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter configure recv message %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex,  AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST));	
	if (!CWErr(CWTimerCancel(&(gWTPs[WTPIndex].currentTimer),1)))
	{
		_CWCloseThread(WTPIndex);
	}
	
	if (!(CWSaveConfigureRequestMessage(&configureRequest, &(gWTPs[WTPIndex].WTPProtocolManager),WTPIndex)))
	{
		return CW_FALSE;
	}
	
	CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
	if (!(CWAssembleConfigureResponse(&(gWTPs[WTPIndex].messages), &(gWTPs[WTPIndex].messagesCount), gWTPs[WTPIndex].pathMTU, seqNum, WTPIndex)))
	{
		CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
		return CW_FALSE;
	}
	
	if (!CWACSendFragments(WTPIndex))
	{
	    CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
		return CW_FALSE;
	}	
	
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter configure send msg %s\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex,  AC_WTP[WTPIndex]->WTPNAME, \
						WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE)); 
	for (i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
	{
		CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
	}
	
	CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
    CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);

	if (!CWErr(CWTimerRequest(gCWChangeStatePendingTimer, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex)))  // start Change State Pending timer
	{
		_CWCloseThread(WTPIndex);
	}

	gWTPs[WTPIndex].currentState = CW_ENTER_DATA_CHECK;
	AC_WTP[WTPIndex]->WTPStat = WID_DATA_CHECK;
	
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter configure next state %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex,  AC_WTP[WTPIndex]->WTPNAME,\
					WAI_STATE_STR(gWTPs[WTPIndex].currentState));
	return CW_TRUE;
}


CWBool CWParseConfigureRequestMessage(char *msg, int len, int *seqNumPtr, CWProtocolConfigureRequestValues *valuesPtr) 
{
	CWControlHeaderValues controlVal;
	int i = 0, j = 0;
	//int k;
	int offsetTillMessages = 0;
	
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || seqNumPtr == NULL || valuesPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
	{
	    return CW_FALSE; // will be handled by the caller
    }
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST)
	{
		syslog(LOG_ERR, " %s line %d type value %d", __func__, __LINE__, controlVal.messageTypeValue);
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Configure Request (maybe it is Image Data Request)");
	}
	
	*seqNumPtr = controlVal.seqNum;
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
	
	//valuesPtr->WTPRadioInfo.radiosCount=0;
	valuesPtr->ACinWTP.count = 0;
	valuesPtr->radioAdminInfoCount = 0;
	
	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen)
	{
		unsigned short int elemType = 0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen = 0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

    	//wid_syslog_debug_debug("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
									
		switch(elemType)
		{
			case CW_MSG_ELEMENT_AC_NAME_CW_TYPE:
				if(!(CWParseACName(&completeMsg, elemLen, &(valuesPtr->ACName)))) return CW_FALSE; // will be handled by the caller
				break;
			case CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE:
				valuesPtr->ACinWTP.count++; // just count how many radios we have, so we can allocate the array
				completeMsg.offset += elemLen;
				break;			
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:
				(valuesPtr->radioAdminInfoCount)++; // just count how many radios we have, so we can allocate the array
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE:
				if(!(CWParseWTPStatisticsTimer(&completeMsg, elemLen, &(valuesPtr->StatisticsTimer)))) return CW_FALSE; // will be handled by the caller
				break;
			case CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->WTPRebootStatistics, WTPRebootStatisticsInfo, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				if(!(CWParseWTPRebootStatistics(&completeMsg, elemLen, valuesPtr->WTPRebootStatistics))) return CW_FALSE; // will be handled by the caller
				break;
			case BINDING_MSG_ELEMENT_TYPE_SET_TXP:
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_SET_CHAN:
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_INFO:
				completeMsg.offset += elemLen;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}
	
	if(completeMsg.offset != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	// actually read each radio info
	CW_CREATE_ARRAY_ERR((valuesPtr->ACinWTP).ACNameIndex, (valuesPtr->ACinWTP).count, CWACNameWithIndexValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	CW_CREATE_ARRAY_ERR(valuesPtr->radioAdminInfo, valuesPtr->radioAdminInfoCount, CWRadioAdminInfoValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	completeMsg.offset = offsetTillMessages;
	while(completeMsg.offset-offsetTillMessages < controlVal.msgElemsLen)
	{
		unsigned short int type = 0;
		unsigned short int len = 0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		switch(type)
		{
			case CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE:
				if(!(CWParseACNameWithIndex(&completeMsg, len, &(valuesPtr->ACinWTP.ACNameIndex[i])))) return CW_FALSE; // will be handled by the caller
				i++;
				break;
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:
				if(!(CWParseWTPRadioAdminState(&completeMsg, len, &(valuesPtr->radioAdminInfo[j])))) return CW_FALSE; // will be handled by the caller
				j++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}

	return CW_TRUE;
}

CWBool CWAssembleConfigureResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex) 
{
	CWProtocolMessage *msgElems = NULL;
	const int MsgElemCount = 9 + (AC_WTP[WTPIndex]->RadioCount)*6; //nine fix message  six unfix message ralated with radio
	CWProtocolMessage *msgElemsBinding = NULL;
	int k = -1, m = -1;
	int j = 0;
	int msgElemBindingCount = 0;	
	BindingRadioOperate stRadioOperate[L_RADIO_NUM] = {{0},{0},{0},{0}};
	BindingRadioConfiguration stRadioConfiguration[L_RADIO_NUM] = {{0},{0},{0},{0}};

	for(j = 0; j < AC_WTP[WTPIndex]->RadioCount; j++)
	{
		stRadioOperate[j].RadioID = j;
		stRadioOperate[j].FragThreshold = AC_WTP[WTPIndex]->WTP_Radio[j]->FragThreshold;
		stRadioOperate[j].RTSThreshold = AC_WTP[WTPIndex]->WTP_Radio[j]->rtsthreshold;
		stRadioOperate[j].Shortretry = AC_WTP[WTPIndex]->WTP_Radio[j]->ShortRetry;
		stRadioOperate[j].Longretry = AC_WTP[WTPIndex]->WTP_Radio[j]->LongRetry;
		
		stRadioConfiguration[j].RadioID = j;
		stRadioConfiguration[j].IsShortPreamble = AC_WTP[WTPIndex]->WTP_Radio[j]->IsShortPreamble;
		stRadioConfiguration[j].BeaconPeriod = AC_WTP[WTPIndex]->WTP_Radio[j]->BeaconPeriod;
		stRadioConfiguration[j].DTIMPeriod = AC_WTP[WTPIndex]->WTP_Radio[j]->DTIMPeriod;
		stRadioConfiguration[j].CountryCode = AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_country_code; /*wcl add for OSDEVTDPB-31*/
		/*fengwenchao add 20111214 for AUTELAN-2713,20111214*/
		if((AC_WTP[WTPIndex]->WTP_Radio[j] != NULL)&&(AC_WTP[WTPIndex]->WTP_Radio[j]->QOSID != 0))
		{
			msgElemBindingCount++;
		}
		/*fengwenchao add end*/
	}
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}    
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemACIPv4List(&(msgElems[++k]))))
		|| (!(CWAssembleMsgElemACIPv6List(&(msgElems[++k]))))
		|| (!(CWAssembleMsgElemCWTimer(&(msgElems[++k]), WTPIndex))) 
		//|| (!(CWAssembleneighbordead_interval(&(msgElems[++k]))))
		|| (!(CWAssembleMsgElemDecryptErrorReportPeriod(&(msgElems[++k]),WTPIndex)))
		|| (!(CWAssembleMsgElemIdleTimeout(&(msgElems[++k]))))
		|| (!(CWAssembleMsgElemWTPFallback(&(msgElems[++k]))))
		|| (!(CWAssembleMsgElemAPScanningSet(&(msgElems[++k]))))
		|| (!(CWAssembleMsgElemAPStatisticsSet(&(msgElems[++k]),-1)))
		|| (!(CWAssembleMsgElemAPThroughoutSet(&(msgElems[++k]),WTPIndex))))
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		
		return CW_FALSE; // error will be handled by the caller
	}
	
    for(j = 0; j < AC_WTP[WTPIndex]->RadioCount; j++)
	{
		if ((!(CWAssembleWTPChan(&(msgElems[++k]), j, AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_Chan)))
			|| (!(CWAssembleWTPTXP(&(msgElems[++k]), j, AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_TXP)))
			|| (!(CWAssembleWTPRadioRate1(&(msgElems[++k]), j, AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_Rate)))
			|| (!(CWAssembleWTPRadioType(&(msgElems[++k]), j, AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_Type)))
			|| (!(CWAssembleWTPMacOperate(&(msgElems[++k]),stRadioOperate[j])))
			|| (!(CWAssembleWTPRadioConfiguration(&(msgElems[++k]),stRadioConfiguration[j]))))
		{
			int i = 0;
			for(i = 0; i <= k; i++)
			{
				CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
	}

	m = k;
	k = -1;
	/*fengwenchao add 20111214 for AUTELAN-2713*/
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElemsBinding,msgElemBindingCount, \
			{for(j = 0; j <= m; j++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[j]);}CW_FREE_OBJECT(msgElems);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for (j = 0; j < AC_WTP[WTPIndex]->RadioCount; j++)
	{
		if ((AC_WTP[WTPIndex]->WTP_Radio[j] != NULL) && (AC_WTP[WTPIndex]->WTP_Radio[j]->QOSID != 0))
		{
			//int Count = 1;
			int tagpacket = 0;
			int g_radio_id = AC_WTP[WTPIndex]->WTP_Radio[j]->Radio_G_ID;
			unsigned int qosid = AC_WTP[WTPIndex]->WTP_Radio[j]->QOSID;
			if (!(CWAssembleWTPQoS2(&msgElemsBinding[++k],g_radio_id,tagpacket,qosid))) 
			{
				/*fengwenchao add 20111214 for AUTELAN-2713*/
				int i = 0;
				for(i = 0; i <= m; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				}
				CW_FREE_OBJECT(msgElems);
				for(i = 0; i <= k; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);
				}
				/*fengwenchao add end*/
				CW_FREE_OBJECT(msgElemsBinding);
				return CW_FALSE; // error will be handled by the caller
			}
		}
	}
	/*fengwenchao add end*/
	
	if (!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE, \
							msgElems, MsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n",  \
					WAI_FUNC_LINE_LITERAL,WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE), seqNum, AC_WTP[WTPIndex]->cap_pro_ver, MsgElemCount);
	return CW_TRUE;
}

CWBool CWSaveConfigureRequestMessage (CWProtocolConfigureRequestValues *configureRequest, CWWTPProtocolManager *WTPProtocolManager,unsigned int wtpindex)
{
	int i = 0;
	int l_radio_id = 0;
	
	CW_FREE_OBJECT(WTPProtocolManager->ACName);
	if((configureRequest->ACName) != NULL)
	{
	    WTPProtocolManager->ACName = configureRequest->ACName;
	}
	
	CW_FREE_OBJECT((WTPProtocolManager->ACNameIndex).ACNameIndex);
	WTPProtocolManager->ACNameIndex = configureRequest->ACinWTP;
	
	CW_FREE_OBJECT((WTPProtocolManager->radioAdminInfo).radios);
	(WTPProtocolManager->radioAdminInfo).radiosCount = configureRequest->radioAdminInfoCount;
	(WTPProtocolManager->radioAdminInfo).radios = configureRequest->radioAdminInfo;

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"config-req report %u radios info\n", \
					WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME, \
					configureRequest->radioAdminInfoCount);
	for(i=0;i<configureRequest->radioAdminInfoCount;i++)
	{
		l_radio_id = configureRequest->radioAdminInfo[i].ID;
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"radio %u state %s\n", \
						WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME, l_radio_id, \
						WAI_RADIO_ADM_STR(configureRequest->radioAdminInfo[i].state));
		if(AC_WTP[wtpindex]->WTP_Radio[l_radio_id] != NULL)
		{
			if(configureRequest->radioAdminInfo[i].state == 2)  /*1(enable), 2(disable)*/
			{
				AC_WTP[wtpindex]->WTP_Radio[l_radio_id]->wifi_state = 2;
				AC_WTP[wtpindex]->wifi_extension_info.wifi_state[l_radio_id] = 3;
				if(gtrapflag == 1)
				{
#if NOSOFTAC
					wid_dbus_trap_ap_wifi_if_error(wtpindex,l_radio_id,1);
#endif
				}
			}
		}
	}
	
	WTPProtocolManager->StatisticsTimer = configureRequest->StatisticsTimer;
		
	//CW_FREE_OBJECT((WTPProtocolManager->WTPRadioInfo).radios);
	//WTPProtocolManager->WTPRadioInfo = configureRequest->WTPRadioInfo;	
	
	CW_FREE_OBJECT(WTPProtocolManager->WTPRebootStatistics);
	WTPProtocolManager->WTPRebootStatistics = configureRequest->WTPRebootStatistics;

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"save configure request\n",  \
				WAI_FUNC_LINE_LITERAL, wtpindex, AC_WTP[wtpindex]->WTPNAME);

	return CW_TRUE;
}

