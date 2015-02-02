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
* ACRetransmission.c
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

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

CWBool CWACSendFragments(int WTPIndex)
{
	int i = 0;
	if (gWTPs[WTPIndex].messages == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	for (i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
	{
#ifdef CW_NO_DTLS
		if (!CWNetworkSendUnsafeUnconnected(	gWTPs[WTPIndex].socket, 
							&gWTPs[WTPIndex].address, 
							gWTPs[WTPIndex].messages[i].msg, 
							gWTPs[WTPIndex].messages[i].offset))
		{
#else
		if (!(CWSecuritySend(gWTPs[WTPIndex].session, gWTPs[WTPIndex].messages[i].msg, gWTPs[WTPIndex].messages[i].offset)))
		{
#endif
			return CW_FALSE;
		}
	}
	
	return CW_TRUE;
}


CWBool CWACResendAcknowledgedPacket(int WTPIndex)
{
	if (!CWACSendFragments(WTPIndex))
	{
		return CW_FALSE;
	}
	
	CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
	if (!(CWTimerRequest(gCWRetransmitTimer, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentPacketTimer), CW_SOFT_TIMER_EXPIRED_SIGNAL, WTPIndex)))
	{
		return CW_FALSE;
	}
	CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
	
	return CW_TRUE;
}


__inline__ CWBool CWACSendAcknowledgedPacket(int WTPIndex, int msgType, int seqNum)
{
	gWTPs[WTPIndex].retransmissionCount = 0;
	gWTPs[WTPIndex].isRetransmitting = CW_TRUE;
	gWTPs[WTPIndex].responseType = msgType;
	gWTPs[WTPIndex].responseSeqNum = seqNum;
	
	return CWACResendAcknowledgedPacket(WTPIndex);
}


void CWACStopRetransmission(int WTPIndex)
{
	if (gWTPs[WTPIndex].isRetransmitting)
	{
		int i = 0;
		wid_syslog_debug_debug(WID_DEFAULT, "%s:%d gWTPs[%d].isRetransmitting = %d\n", __func__, __LINE__, WTPIndex, gWTPs[WTPIndex].isRetransmitting);
		gWTPs[WTPIndex].isRetransmitting = CW_FALSE;
		CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
		if (!CWTimerCancel(&(gWTPs[WTPIndex].currentPacketTimer),1))
		{
			wid_syslog_err("%s:%d wtp%d Error Cancelling a Timer... possible error!", __func__, __LINE__, WTPIndex);
		}
		CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
		gWTPs[WTPIndex].responseType = UNUSED_MSG_TYPE;
		gWTPs[WTPIndex].responseSeqNum = 0;
		wid_syslog_debug_debug(WID_DEFAULT, "%s:%d gWTPs[%d].messagesCount = %d\n", __func__, __LINE__, WTPIndex, gWTPs[WTPIndex].messagesCount);
		for (i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
		}
		
		CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
		wid_syslog_debug_debug(WID_DEFAULT, "%s:%d gWTPs[%d].messages free end\n", __func__, __LINE__, WTPIndex);
	}
}

