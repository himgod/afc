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
* ACReset.c
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
#include "CWAC.h"
#include "ACImageData.h"
#include "wcpss/waw.h"

#include "wcpss/wid/WID.h"
#include "ACDbus.h"
#include "dbop.h"
#include "AC.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
extern CWBool CWAssembleMsgElemImageIdentifier(CWProtocolMessage *msgPtr, 
	CWImageIdentifier_cw *ImageIdentifier);
CWBool CWAssembleResetRequestMessage
(
	CWProtocolMessage **messagesPtr,
	int *fragmentsNumPtr,
	int PMTU,
	int seqNum,
	CWImageIdentifier *resPtr,
	int WTPIndex
)
{
	CWProtocolMessage *msgElems = NULL;
	const int MsgElemCount = 1;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	int k = -1;
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	wid_syslog_debug_debug(WID_WTPINFO,"Assembling Reset...\n");
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemImageIdentifierAC(&(msgElems[++k]), resPtr))))
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	
	if(!(CWAssembleMessage(messagesPtr,
							fragmentsNumPtr,
							PMTU,
							seqNum,
							CW_MSG_TYPE_VALUE_RESET_REQUEST,
							msgElems,
							MsgElemCount,
							msgElemsBinding,
							msgElemBindingCount,
							CW_PACKET_CRYPT,
							AC_WTP[WTPIndex]->cap_pro_ver)))
	{
		return CW_FALSE;
	}
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"assembled %s msg seq %u capprover %u with %u elements\n",
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_RESET_REQUEST), seqNum, AC_WTP[WTPIndex]->cap_pro_ver, MsgElemCount);
	return CW_TRUE;
}


CWBool CWAssembleResetRequestMessage_cw
(
	CWProtocolMessage **messagesPtr, 
	int *fragmentsNumPtr, 
	int PMTU, 
	unsigned char  seqNum,
	CWImageIdentifier_cw *resPtr,
	int WTPIndex
)
{
	CWProtocolMessage *msgElems = NULL;
	CWProtocolMessage *msgElemsBinding = NULL;
	const int MsgElemCount = 1;
	int msgElemBindingCount = 0;
	int k = -1;
	int i = 0;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	/*Assemble Message Elements*/
	if (!(CWAssembleMsgElemImageIdentifier(&(msgElems[++k]), resPtr)))
	{
		for(i = 0; i <= k; i++) 
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; 
	}
			
	if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_RESET_REQUEST, msgElems, MsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n",
					 WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,
					 WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_RESET_REQUEST), seqNum, AC_WTP[WTPIndex]->cap_pro_ver, MsgElemCount);
	return CW_TRUE;
}

CWBool ACEnterReset(int WTPIndex, CWProtocolMessage *msgPtr)
{
    int ret = 0;
	CWControlHeaderValues controlVal;
	wid_syslog_debug_debug(WID_WTPINFO,"######### WTP %d Enter Reset State #########\n",WTPIndex);
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	if (gtrapflag >= 1)
	{
#if NOSOFTAC
		wid_dbus_trap_wtp_ap_reboot(WTPIndex);
#endif
    }
	msgPtr->offset = 0;
	
	if (!(CWACParseGenericRunMessage(WTPIndex, msgPtr, &controlVal)))
	{
		//## Two possible errors: WRONG_ARG and INVALID_FORMAT
		//## In the second case we have an unexpected response: ignore the
		//## message and log the event.
		return CW_FALSE;
	}
	switch (controlVal.messageTypeValue)
	{
		case CW_MSG_TYPE_VALUE_RESET_RESPONSE:
		{
			wid_syslog_debug_debug(WID_WTPINFO,"CW_MSG_TYPE_VALUE_RESET_RESPONSE\n");
			gWTPs[WTPIndex].currentState = CW_QUIT; 
			AC_WTP[WTPIndex]->WTPStat = WID_QUIT;			
			syslog_wtp_log(WTPIndex, 0, "for update", 0);
			gWTPs[WTPIndex].isRequestClose = CW_TRUE;	
#ifdef OBJECT_ACTIVE_SUPPORT
#if OBJECT_ACTIVE_SUPPORT
			ret = active_ap_disconnect(AC_WTP[WTPIndex]->WTPMAC);
            if(0 <= ret)
            {
                wtp_add_new_alert(AC_WTP[WTPIndex]->WTPMAC, ret, CW_FALSE);
            }         
#endif
#endif
			_CWCloseThread(WTPIndex);
			break;			
		}
		default:	
			wid_syslog_debug_debug(WID_WTPINFO,"%d\n",controlVal.messageTypeValue);
			gWTPs[WTPIndex].currentState = CW_QUIT; 
			AC_WTP[WTPIndex]->WTPStat = WID_QUIT;			
			syslog_wtp_log(WTPIndex, 0, "something wrong in update", 0);
			_CWCloseThread(WTPIndex);
			break;
	}

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter reset next state %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,WAI_STATE_STR(gWTPs[WTPIndex].currentState));
	return CW_TRUE;
}



