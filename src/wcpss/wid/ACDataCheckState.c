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
* ACDataCheckState.c
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
#include "AC.h"
#include "ACDbus.h"
#include "ACDbus_handler.h"
#include "ACAccessCheck.h"
#include "ACLoadbanlance.h"


#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

extern int wid_wtp_login_loadconfig(unsigned int wtpid);


CWBool ACEnterDataCheck (int WTPIndex, CWProtocolMessage *msgPtr)
{
	int i = 0;
	int seqNum = 0;
	CWProtocolChangeStateEventRequestValues *changeStateEvent = NULL;

	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);

	if (gloadbanlance == 1)
	{
		CWThreadMutexLock(&ACIPLISTMutex);
		SendActiveWTPCount(gActiveWTPs);
		CWThreadMutexUnlock(&ACIPLISTMutex);
	}
		
	CW_CREATE_OBJECT_ERR(changeStateEvent, CWProtocolChangeStateEventRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if (!(CWParseChangeStateEventRequestMessage(msgPtr->msg, msgPtr->offset, &seqNum, changeStateEvent, WTPIndex)))
	{
		CW_FREE_OBJECT(changeStateEvent);
		return CW_FALSE; // note: we can kill our thread in case of out-of-memory error to free some space/
		// we can see this just calling CWErrorGetLastErrorCode()
	}
	
	//Destroy ChangeStatePending timer
	if (!CWErr(CWTimerCancel(&(gWTPs[WTPIndex].currentTimer),1)))
	{
		_CWCloseThread(WTPIndex);
	}

	if (!(CWSaveChangeStateEventRequestMessage(changeStateEvent, &(gWTPs[WTPIndex].WTPProtocolManager))))
	{
		return CW_FALSE;
	}
	
    CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
	if (!(CWAssembleChangeStateEventResponse(&(gWTPs[WTPIndex].messages), &(gWTPs[WTPIndex].messagesCount), gWTPs[WTPIndex].pathMTU, seqNum, WTPIndex))) 
	{
		CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
		return CW_FALSE;
	}
	
	if (!CWACSendFragments(WTPIndex))
	{
		CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
		return CW_FALSE;
	}	
	
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter data-check send msg %s\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
						WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE)); 
	for (i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
	{
		CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
	}
	CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
    CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);

	/*radio admin state*/
	for (i = 0; i < AC_WTP[WTPIndex]->RadioCount; i++)
	{
		if (AC_WTP[WTPIndex]->WTP_Radio[i] != NULL)
		{
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->wifi_state != 2)
			{
				AC_WTP[WTPIndex]->WTP_Radio[i]->AdStat = 1;
				AC_WTP[WTPIndex]->WTP_Radio[i]->OpStat = 1;
			}
		}
	}
	
	for (i = 0; i < AC_WTP[WTPIndex]->RadioCount; i++)
	{
		if (AC_WTP[WTPIndex]->WTP_Radio[i] != NULL)
		{
			int result = 0;
			int isoem = 0;
			/*parse whether the ap is oem*/
			if (AC_WTP[WTPIndex]->codever == NULL)/*our ap*/
			{
				isoem = 0;/*autelan ap*/
			}
			else
			{
				isoem = 1;/*oem ap*/
			}
			/*parse set txantenna policy:oem use codever||aute ap use ver*/
			if (isoem == 0)
			{
				if (AC_WTP[WTPIndex]->ver != NULL)
				{
					if (memcmp(AC_WTP[WTPIndex]->ver,"1.1",3) == 0)//send to make a script active on the ap with the version 1.2
					{
						result = 1;
					}
				}	
			}
			else
			{
				if (AC_WTP[WTPIndex]->codever != NULL)
				{
					if (memcmp(AC_WTP[WTPIndex]->codever,"1.1",3) == 0)//send to make a script active on the ap with the version 1.2
					{
						result = 1;
					}
				}
			}
			/*append to result .send the txantenna policy*/
			if (result == 0)
			{
				wid_set_radio_diversity_txantenna_after_run_new(WTPIndex,i);
			}
			else
			{
				wid_set_radio_diversity_txantenna_after_run(WTPIndex,i);
			}
		}
	}
	
	////ap run time
	if (AC_WTP[WTPIndex]->add_time == NULL)
	{
		AC_WTP[WTPIndex]->add_time = (time_t *)malloc(sizeof(time_t));
		time(AC_WTP[WTPIndex]->add_time);
	}
	else
	{
		time(AC_WTP[WTPIndex]->add_time);
	}
	
	AC_WTP[WTPIndex]->ElectrifyRegisterCircle = 27;
#if NOSOFTAC	
	AsdWsm_DataChannelOp(WTPIndex, WID_ADD);	
	AsdWsm_WTPOp(WTPIndex,WID_MODIFY);
#endif	
	CWThreadMutexLock(&ACAccessWTP);
	struct sockaddr_in	*sin = (struct sockaddr_in *)&(gWTPs[WTPIndex].address);	
	struct wtp_access_info *wtp = NULL;
	wtp = get_ap(AC_WTP_ACC,sin->sin_addr.s_addr);
	if (wtp != NULL)
	{
		free_ap(AC_WTP_ACC, wtp);
	}
	CWThreadMutexUnlock(&ACAccessWTP);

	/* MUST before set WTPState RUN, or msgqueue maybe BLOCK */
	wid_wtp_login_loadconfig(WTPIndex);

	gWTPs[WTPIndex].currentState = CW_ENTER_RUN;	
	AC_WTP[WTPIndex]->WTPStat = WID_RUN;
	gWTPs[WTPIndex].subState = CW_WAITING_REQUEST;
#if NOSOFTAC	
	AsdWsm_WTPOp(WTPIndex,WID_MODIFY);
#endif
	if(!CWErr(CWTimerRequest(gCWNeighborDeadInterval, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex)))
	{
		_CWCloseThread(WTPIndex);
	}
	
	syslog(LOG_INFO|LOG_LOCAL7, WAI_FUNC_LINE_FMT""WAI_AFI_FMT"is up mac "WAI_MAC_FMT"ip %s at %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex,  AC_WTP[WTPIndex]->WTPNAME,\
					WAI_MAC_LITERAL(AC_WTP[WTPIndex]->WTPMAC), \
					AC_WTP[WTPIndex]->WTPIP,ctime(AC_WTP[WTPIndex]->add_time));
					
	if(gtrapflag >= 1 && AC_WTP[WTPIndex]->wid_trap.ignore_switch != 1)
	{
#if NOSOFTAC
		wid_dbus_trap_ap_run_quit(WTPIndex,state);
#endif
	}
	else
	{
	    AC_WTP[WTPIndex]->wid_trap.ignore_switch = 0;
	}

	if(AC_WTP[WTPIndex]->ApReportVer == NULL)
	{
	    /*wid_syslog_debug_debug(WID_WTPINFO,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!################################\n");*/
	}
	else
	{
		if(AC_WTP[WTPIndex]->codever == NULL)
		{
			if(memcmp(AC_WTP[WTPIndex]->ver,AC_WTP[WTPIndex]->ApReportVer,strlen(AC_WTP[WTPIndex]->ver)) == 0)
			{
				if(gtrapflag >= 4)
				{
		#if NOSOFTAC
					wid_dbus_trap_wtp_update_successful(WTPIndex);//trap successful
		#endif
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update %s success\n",\
										WAI_FUNC_LINE_LITERAL ,WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
										AC_WTP[WTPIndex]->ver);
					CW_FREE_OBJECT(AC_WTP[WTPIndex]->ApReportVer);
				}
			}
			else
			{	
				if(gtrapflag >= 4)
				{
			#if NOSOFTAC
					wid_dbus_trap_wtp_update_fail(WTPIndex);//trap fail
					wid_dbus_trap_wtp_ap_flash_write_failed(WTPIndex);
    		#endif
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update %s fail\n",\
								WAI_FUNC_LINE_LITERAL ,WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
								AC_WTP[WTPIndex]->ver);
					CW_FREE_OBJECT(AC_WTP[WTPIndex]->ApReportVer);
				}
			}		
		}
		else
		{
			if(memcmp(AC_WTP[WTPIndex]->codever,AC_WTP[WTPIndex]->ApReportVer,strlen(AC_WTP[WTPIndex]->codever)) == 0)
			{
				if(gtrapflag >= 4)
				{	
			#if NOSOFTAC
					wid_dbus_trap_wtp_update_successful(WTPIndex);//trap successful
			#endif
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update %s success\n",\
										WAI_FUNC_LINE_LITERAL ,WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
										AC_WTP[WTPIndex]->codever);
					CW_FREE_OBJECT(AC_WTP[WTPIndex]->ApReportVer);				
				}
			}
			else
			{	
				if (gtrapflag >= 4)
				{
			#if NOSOFTAC
					wid_dbus_trap_wtp_update_fail(WTPIndex);//trap fail
					wid_dbus_trap_wtp_ap_flash_write_failed(WTPIndex);
			#endif
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update %s fail\n",\
										WAI_FUNC_LINE_LITERAL ,WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
										AC_WTP[WTPIndex]->codever);
					CW_FREE_OBJECT(AC_WTP[WTPIndex]->ApReportVer);					
				}
			}	
		}
	}

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter data-check next state %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_STATE_STR(gWTPs[WTPIndex].currentState));
	return CW_TRUE;
}

CWBool CWParseChangeStateEventRequestMessage
(
	char *msg, 
	int len, 
	int *seqNumPtr, 
	CWProtocolChangeStateEventRequestValues *valuesPtr, 
	int WTPIndex
) 
{
	int i = 0;
	int offsetTillMessages = 0;
	CWProtocolMessage completeMsg;
	CWControlHeaderValues controlVal;
	
	if (msg == NULL || seqNumPtr == NULL || valuesPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
		
	if (!(CWParseControlHeader(&completeMsg, &controlVal)))
	{
	    return CW_FALSE; // will be handled by the caller
    }
	
	if (controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST)
	{
		wid_syslog_notice(WAI_FUNC_LINE_FMT""WAI_AFI_FMT" msg %s seq %u expect %s we drop\n",  \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
				WAI_CAPWAP_MSG_TYPE_STR(controlVal.messageTypeValue), \
				controlVal.seqNum, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST));
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Change State Event Request");
	}
	
	*seqNumPtr = controlVal.seqNum;
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
		
	valuesPtr->radioOperationalInfo.radiosCount = 0;
	
	while ((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen)
	{
		unsigned short int elemType = 0;    // = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen = 0;     // = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		
									
		switch (elemType)
		{
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				valuesPtr->radioOperationalInfo.radiosCount++; // just count how many radios we have, so we can allocate the array
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE: 
				if(!(CWParseResultCode(&completeMsg, elemLen, &(valuesPtr->resultCode)))) return CW_FALSE;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}
	
	if (completeMsg.offset != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}	
	CW_CREATE_ARRAY_ERR(valuesPtr->radioOperationalInfo.radios, valuesPtr->radioOperationalInfo.radiosCount, CWRadioOperationalInfoValues, 
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	completeMsg.offset = offsetTillMessages;
	i = 0;
	while (completeMsg.offset-offsetTillMessages < controlVal.msgElemsLen)
	{
		unsigned short int type = 0;    // = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len = 0;     // = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		switch (type)
		{
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				if (!(CWParseWTPRadioOperationalState(&completeMsg, len, &(valuesPtr->radioOperationalInfo.radios[i]))))
				{
				    return CW_FALSE; // will be handled by the caller
				}
				i++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}
	
	return CW_TRUE;
}

CWBool CWAssembleChangeStateEventResponse (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex) 
{
	const int msgElemCount = 0;
	int msgElemBindingCount = 0;
	CWProtocolMessage *msgElems = NULL;
	CWProtocolMessage *msgElemsBinding = NULL;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if (!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum,   \
					CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE,msgElems, msgElemCount,  \
					msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver)))
	{
	    return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE), \
					seqNum, AC_WTP[WTPIndex]->cap_pro_ver, msgElemCount);
	return CW_TRUE;
}
