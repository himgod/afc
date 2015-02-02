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
* CWThread_wid.c
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

#include "CWCommon.h"

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/msg.h>
#include "wcpss/waw.h"
#include "wcpss/wid/WID.h"
#include "CWAC.h"
#include "dbop.h"
#include "ACDbus.h"
#include "AC.h"
#include "ACAccessCheck.h"

// timers
typedef struct {
 	CWThread *requestedThreadPtr;
 	int signalToRaise;
	int WTPID;
	int WLANID;
	int RADIOID;
	int AC_ACCTH_ID;
} CWThreadTimerArg;

void CWHandleTimer(CWTimerArg arg)
{
	msgq msg;	
	CWThreadTimerArg *a = (CWThreadTimerArg*)arg;
 //	CWThread requestedThreadPtr = *(a->requestedThreadPtr);
 	int signalToRaise = a->signalToRaise;
	int WTPID = a->WTPID;
	int WLANID = a->WLANID;
	int RADIOID = a->RADIOID;
	int AC_ACCTH_ID = a->AC_ACCTH_ID;
	int WTPMsgqID = 0;	
	time_t timep;  
	struct tm *p = NULL; 
	int times = 0;	
	int timer = 0;
	CWGetMsgQueue(&WTPMsgqID);
	char command[128] = {0};
	char *str = NULL;
	char IP[128];
	memset(IP, 0, 128);
	int ret = 0;
	CWTimerID TimerID = 0;
//	CWThreadSendSignal(requestedThreadPtr, signalToRaise);
	if (signalToRaise == SIGUSR2 )
	{
		if (AC_WTP[WTPID] == NULL)
		{
			wid_syslog_crit("%s AC_WTP[%d]= NULL!!!",__func__,WTPID);	
			return;
		}
		if (AC_WTP[WTPID])
		{
			AC_WTP[WTPID]->neighbordeatimes++;
		}
		
		if ((AC_WTP[WTPID]) && (AC_WTP[WTPID]->neighbordeatimes == 3))
		{
			syslog_wtp_log(WTPID, 0, "Critical Timer Expired", 0);
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
			
			if (WID_REJOIN == AC_WTP[WTPID]->WTPStat)
			{
				wid_syslog_info(WAI_FUNC_LINE_FMT"gWTPs[%d].currentState %d enter run\n", WAI_FUNC_LINE_LITERAL, WTPID, gWTPs[WTPID].currentState);
				AC_WTP[WTPID]->WTPStat = WID_OFFLINE;
				gWTPs[WTPID].currentState = CW_ENTER_RUN;
			}
			else
			{
				if (gWTPs[WTPID].currentState != CW_ENTER_RUN)
				{
					AC_WTP[WTPID]->neighbordeatimes = 0;
					gWTPs[WTPID].isRequestClose = CW_TRUE;
					
					wid_syslog_info(WAI_FUNC_LINE_FMT"gWTPs[%d].currentState %d close thread\n", WAI_FUNC_LINE_LITERAL, WTPID, gWTPs[WTPID].currentState); 
					_CWCloseThread(WTPID);
				}
				else
				{
					AC_WTP[WTPID]->WTPStat = WID_OFFLINE;
					
					wid_syslog_info(WAI_FUNC_LINE_FMT"gWTPs[%d].currentState ENTER_RUN WTPStat offline\n", WAI_FUNC_LINE_LITERAL, WTPID); 
				}
			}
			
			ret = active_ap_offlined(AC_WTP[WTPID]->WTPMAC);
			if (ret >= 0)
			{
				wtp_add_new_alert(AC_WTP[WTPID]->WTPMAC, ret, CW_FALSE);
			} 
		}
		else
		{
			if ((AC_WTP[WTPID]) && (AC_WTP[WTPID]->neighbordeatimes == 2) && (wtp_link_detect == 1))
			{
				str = strchr(AC_WTP[WTPID]->WTPIP,':');
				memcpy(IP,AC_WTP[WTPID]->WTPIP,str-AC_WTP[WTPID]->WTPIP);

				sprintf(command,"echo \"********************************************************************************\" >>/home/WTP%d_tracerout &",WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				sprintf(command,"echo \"WTP IP : %s\" >>/home/WTP%d_tracerout &",IP,WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				sprintf(command,"date >>/home/WTP%d_tracerout &",WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);
				sprintf(command,"traceroute -n -I -m 3 %s >>/home/WTP%d_tracerout &",IP,WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				sprintf(command,"echo \"********************************************************************************\" >>/home/WTP%d_iperf &",WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				sprintf(command,"echo \"WTP IP : %s\" >>/home/WTP%d_iperf &",IP,WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				sprintf(command,"date >>/home/WTP%d_iperf &",WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				sprintf(command,"iperf -u -c %s 2 >>/home/WTP%d_iperf &",IP,WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);

				/*sprintf(command,"/usr/bin/trace_wtp2.sh -ht %d %s /home/WTP%d_trace &", WTPID, AC_WTP[WTPID]->WTPIP,WTPID);
				system(command);
				printf("%s\n",command);
				memset(command, 0, 128);
				sprintf(command,"/usr/bin/btrace_wtp2.sh -ht b %s /home/WTP%d_btrace &", AC_WTP[WTPID]->WTPIP,WTPID);
				system(command);
				printf("%s\n",command);*/
			}
		}
		if (!CWErr(CWTimerRequest(gCWNeighborDeadInterval/3, &(gWTPs[WTPID].thread), &(gWTPs[WTPID].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPID)))  // start NeighborDeadInterval timer
		{
			return ;
		}
	}
	else if (signalToRaise == 500)
	{
		if ((gWTPs[WTPID].currentState == CW_QUIT) && (find_in_wtp_list(WTPID) == CW_TRUE) && (AC_WTP[WTPID]))
		{
			//printf("CWCriticalTimerExpiredHandler\n");
			delete_wtp_list(WTPID);
			insert_uptfail_wtp_list(WTPID);
			AC_WTP[WTPID]->updatefailstate = 0;
			update_complete_check();
		}		
	}
	else if (signalToRaise == SIGUSR1)
	{
	
		wid_syslog_debug_debug(WID_DEFAULT,"wtp%d Soft Timer Expired for Thread: %08x",WTPID, (unsigned int)CWThreadSelf());
				
		if ((!gWTPs[WTPID].isRetransmitting) || (gWTPs[WTPID].messages == NULL))
		{
			wid_syslog_info("wtp%d Soft timer expired but we are not retransmitting\n", WTPID);
			if (AC_WTP[WTPID] != NULL)
			{
				CWThreadMutexLock(&(gWTPs[WTPID].WTPThreadControllistMutex));
				CW_FREE_OBJECT(AC_WTP[WTPID]->ControlWait);
				CWThreadMutexUnlock(&(gWTPs[WTPID].WTPThreadControllistMutex));
			}
			return;
		}
	
		(gWTPs[WTPID].retransmissionCount)++;
		
		wid_syslog_info("wtp%d Retransmission Count increases to %d", WTPID, gWTPs[WTPID].retransmissionCount);
		
		if (gWTPs[WTPID].retransmissionCount >= gCWMaxRetransmit) 
		{
			wid_syslog_err("wtp%d retransmission %d times not restart ap\n",WTPID, gWTPs[WTPID].retransmissionCount);
			//?? _CWCloseThread(*iPtr);
			// Request close thread
			gWTPs[WTPID].isRequestClose = CW_TRUE;
	//		CWDownWTP(*iPtr);
	
			/*memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPID%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPID;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
			msg.mqinfo.u.WtpInfo.WTPID = WTPID;
			if (msgsnd(WTPMsgqID, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
			{
				wid_syslog_crit("%s msgsend %s",__func__,strerror(errno));
			}*/
			
	//		CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
			CWThreadMutexLock(&gWTPs[WTPID].interfaceMutex);
			CWACStopRetransmission(WTPID);			
			CWThreadMutexUnlock(&gWTPs[WTPID].interfaceMutex);
			
			AC_WTP[WTPID]->WTPStat = WID_OFFLINE;
			ret = active_ap_offlined(AC_WTP[WTPID]->WTPMAC);
			if (ret >= 0)
			{
				wtp_add_new_alert(AC_WTP[WTPID]->WTPMAC, ret, CW_FALSE);
			} 
			return;
		}
		else
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPID%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPID;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_RESEND;
			msg.mqinfo.u.WtpInfo.WTPID = WTPID;
			if (msgsnd(WTPMsgqID, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
			{
				wid_syslog_crit("%s msgsend %s",__func__,strerror(errno));
			}
		}		
	}
	else if (signalToRaise == 501)
	{		
		time(&timep);  
		p = localtime(&timep);
		printf("time %d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);
		if (AC_WLAN[WLANID]!= NULL)
		{
			if (AC_WLAN[WLANID]->StartService.TimerState == 1)
			{
				if ((AC_WLAN[WLANID]->StartService.wday[p->tm_wday] == 1) && (AC_WLAN[WLANID]->Status == 1))
				{
					WID_ENABLE_WLAN(WLANID);
				}
				if (AC_WLAN[WLANID]->StartService.is_once == 1)
				{
					AC_WLAN[WLANID]->StartService.wday[p->tm_wday] = 0;
				}
				if ((AC_WLAN[WLANID]->StartService.wday[0] == 0)
					&& (AC_WLAN[WLANID]->StartService.wday[1] == 0)
					&& (AC_WLAN[WLANID]->StartService.wday[2] == 0)
					&& (AC_WLAN[WLANID]->StartService.wday[3] == 0)
					&& (AC_WLAN[WLANID]->StartService.wday[4] == 0)
					&& (AC_WLAN[WLANID]->StartService.wday[5] == 0)
					&& (AC_WLAN[WLANID]->StartService.wday[6] == 0)
				)
				{
					AC_WLAN[WLANID]->StartService.TimerState = 0;
					printf("501 end\n");
				}
				else
				{
					time(&timep);  
					p = localtime(&timep);
					times = p->tm_hour*3600 + p->tm_min*60 + p->tm_sec;
					if (times < AC_WLAN[WLANID]->StartService.times)
					{
						timer = AC_WLAN[WLANID]->StartService.times - times; 
					}
					else
					{
						timer = 24*3600 - times + AC_WLAN[WLANID]->StartService.times;
					}
					if (!(CWTimerRequest(timer, NULL, &(AC_WLAN[WLANID]->StartService.TimerID), 501, WLANID)))
					{
						return ;
					}
					wid_syslog_info("AC_WLAN[%d]->StartService.TimerID %d\n",WLANID,AC_WLAN[WLANID]->StartService.TimerID);
				}
			}
		}
		
	}
	else if (signalToRaise == 502)
	{		
		printf("502 disable\n");
		
		time(&timep);  
		p = localtime(&timep);
		printf("time %d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);

		if (AC_WLAN[WLANID] != NULL)
		{
			if (AC_WLAN[WLANID]->StopService.TimerState == 1)
			{
				if ((AC_WLAN[WLANID]->StopService.wday[p->tm_wday] == 1) && (AC_WLAN[WLANID]->Status == 0))
				{
					WID_DISABLE_WLAN(WLANID);
				}
				if (AC_WLAN[WLANID]->StopService.is_once == 1)
				{
					AC_WLAN[WLANID]->StopService.wday[p->tm_wday] = 0;
				}
				if ((AC_WLAN[WLANID]->StopService.wday[0] == 0)
					&& (AC_WLAN[WLANID]->StopService.wday[1] == 0)
					&& (AC_WLAN[WLANID]->StopService.wday[2] == 0)
					&& (AC_WLAN[WLANID]->StopService.wday[3] == 0)
					&& (AC_WLAN[WLANID]->StopService.wday[4] == 0)
					&& (AC_WLAN[WLANID]->StopService.wday[5] == 0)
					&& (AC_WLAN[WLANID]->StopService.wday[6] == 0)
				)
				{
					AC_WLAN[WLANID]->StopService.TimerState = 0;
					printf("502 end\n");
				}
				else
				{
					time(&timep);  
					p = localtime(&timep);
					times = p->tm_hour*3600 + p->tm_min*60 + p->tm_sec;
					if (times < AC_WLAN[WLANID]->StopService.times)
					{
						timer = AC_WLAN[WLANID]->StopService.times - times; 
					}
					else
					{
						timer = 24*3600 - times + AC_WLAN[WLANID]->StopService.times;
					}
					if (!(CWTimerRequest(timer, NULL, &(AC_WLAN[WLANID]->StopService.TimerID), 502, WLANID)))
					{
						return ;
					}	
					
					wid_syslog_info("AC_WLAN[%d]->StopService.TimerID %d\n",WLANID,AC_WLAN[WLANID]->StopService.TimerID);
				}
			}
		}
	}
	else if (signalToRaise == 503)
	{
		printf("503 enable\n");
		time(&timep);  
		p = localtime(&timep);
		printf("time %d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);
		if (AC_RADIO[RADIOID] != NULL)
		{
			if (AC_RADIO[RADIOID]->StartService.TimerState == 1)
			{
				if ((AC_RADIO[RADIOID]->StartService.wday[p->tm_wday] == 1) && (AC_RADIO[RADIOID]->AdStat == 2))
				{
					WID_RADIO_SET_STATUS(RADIOID,1);
				}
				if (AC_RADIO[RADIOID]->StartService.is_once == 1)
				{
					AC_RADIO[RADIOID]->StartService.wday[p->tm_wday] = 0;
				}
				if ((AC_RADIO[RADIOID]->StartService.wday[0] == 0)
					&& (AC_RADIO[RADIOID]->StartService.wday[1] == 0)
					&& (AC_RADIO[RADIOID]->StartService.wday[2] == 0)
					&& (AC_RADIO[RADIOID]->StartService.wday[3] == 0)
					&& (AC_RADIO[RADIOID]->StartService.wday[4] == 0)
					&& (AC_RADIO[RADIOID]->StartService.wday[5] == 0)
					&& (AC_RADIO[RADIOID]->StartService.wday[6] == 0))
				{
					AC_RADIO[RADIOID]->StartService.TimerState = 0;
					printf("503 end\n");
				}
				else
				{
					time(&timep);  
					p = localtime(&timep);
					times = p->tm_hour*3600 + p->tm_min*60 + p->tm_sec;
					if (times < AC_RADIO[RADIOID]->StartService.times)
					{
						timer = AC_RADIO[RADIOID]->StartService.times - times; 
					}
					else
					{
						timer = 24*3600 - times + AC_RADIO[RADIOID]->StartService.times;
					}
					if (!(CWTimerRequest(timer, NULL, &(AC_RADIO[RADIOID]->StartService.TimerID), 503,WLANID)))
					{
						return ;
					}
					wid_syslog_info("AC_WLAN[%d]->StartService.TimerID %d\n",WLANID,AC_RADIO[RADIOID]->StartService.TimerID);
				}
			}
		}
	}
	else if (signalToRaise == 504)
	{		
		printf("504 disable\n");
		
		time(&timep);  
		p = localtime(&timep);
		printf("time %d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);

		if (AC_RADIO[RADIOID] != NULL)
		{
			if (AC_RADIO[RADIOID]->StopService.TimerState == 1)
			{
				if ((AC_RADIO[RADIOID]->StopService.wday[p->tm_wday] == 1) && (AC_RADIO[RADIOID]->AdStat == 1))
				{
					WID_RADIO_SET_STATUS(RADIOID,2);
				}
				if (AC_RADIO[RADIOID]->StopService.is_once == 1)
				{
					AC_RADIO[RADIOID]->StopService.wday[p->tm_wday] = 0;
				}
				if ((AC_RADIO[RADIOID]->StopService.wday[0] == 0)
					&& (AC_RADIO[RADIOID]->StopService.wday[1] == 0)
					&& (AC_RADIO[RADIOID]->StopService.wday[2] == 0)
					&& (AC_RADIO[RADIOID]->StopService.wday[3] == 0)
					&& (AC_RADIO[RADIOID]->StopService.wday[4] == 0)
					&& (AC_RADIO[RADIOID]->StopService.wday[5] == 0)
					&& (AC_RADIO[RADIOID]->StopService.wday[6] == 0))
				{
					AC_RADIO[RADIOID]->StopService.TimerState = 0;
					printf("504 end\n");
				}
				else
				{
					time(&timep);  
					p = localtime(&timep);
					times = p->tm_hour*3600 + p->tm_min*60 + p->tm_sec;
					if (times < AC_RADIO[RADIOID]->StopService.times)
					{
						timer = AC_RADIO[RADIOID]->StopService.times - times; 
					}
					else
					{
						timer = 24*3600 - times + AC_RADIO[RADIOID]->StopService.times;
					}
					if (!(CWTimerRequest(timer, NULL, &(AC_RADIO[RADIOID]->StopService.TimerID), 504,WLANID)))
					{
						return ;
					}
					
					wid_syslog_info("AC_WLAN[%d]->StopService.TimerID %d\n",WLANID,AC_RADIO[RADIOID]->StopService.TimerID);
				}
			}
		}
	}
	else if (signalToRaise == 900)
	{
		if (is_secondary == 1)
		{
			bak_check_req(wid_bak_sock);
			if (!CWErr(CWTimerRequest(BakCheckInterval, NULL, &(bak_check_timer), 900, 0)))
			{
				return ;
			}
		}		
	}
	else if (signalToRaise == 901)
	{
		if (Lic_ip.isActive == 2)
		{
			//lic_bak_req();
			update_license_req(lic_bak_fd , (struct sockaddr_in *)&Lic_Active_addr);
			if (!CWErr(CWTimerRequest(LicBakReqInterval, NULL, &(Lic_bak_req_timer), 901, 0)))
			{
				return ;
			}
		}
	}
	/*fengwenchao add 20120117 for onlinebug-96*/
	else if (signalToRaise == 911) 
	{
		wid_syslog_info("%s , signalToRaise = 911  \n",__func__);
		int i = 0;
		msgq qmsg;	
		if (is_secondary == 0)
		{
			for (i = 1; i < WTP_NUM;i++)
			{
				if (AC_WTP[i] != NULL)
				{
					//wid_syslog_info("1:AC_WTP[%d]->WTPStat = %d \n",i,AC_WTP[i]->WTPStat);
					if (AC_WTP[i]->WTPStat == WID_BAK_RUN)
					{					
						gWTPs[i].isRequestClose = CW_TRUE;	
						syslog_wtp_log(i, 0, "Critical Timer Expired", 0);
						memset((char*)&qmsg, 0, sizeof(qmsg));
						qmsg.mqid = i%THREAD_NUM+1;
						qmsg.mqinfo.WTPID = i;
						qmsg.mqinfo.type = CONTROL_TYPE;
						qmsg.mqinfo.subtype = WTP_S_TYPE;
						qmsg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
						qmsg.mqinfo.u.WtpInfo.WTPID = i;
						if (msgsnd(WTPMsgqID, (msgq *)&qmsg, sizeof(qmsg.mqinfo), IPC_NOWAIT) == -1)
						{
							wid_syslog_crit("%s msgsend %s",__func__,strerror(errno));
						}
					}
				}
			}
		}
	}
	/*fengwenchao add end*/
	else if (signalToRaise == TIMER_TYPE_AC_ATTCH)
	{
	    CWThreadMutexLock(&AttachMutex);
		wid_syslog_debug_debug(WID_WTPINFO,"CWHandleTimer: time is out , just free AC_ATTCH[%d] \n", AC_ACCTH_ID);
		CW_FREE_OBJECT(AC_ATTACH[AC_ACCTH_ID]);
		CWThreadMutexUnlock(&AttachMutex);
	}
	else if (signalToRaise == TIMER_TYPE_AC_ACCESS)
	{
		struct wtp_access_info *wtp = NULL, *prev = NULL;
		CWThreadMutexLock(&ACAccessWTP);
		
		if (AC_WTP_ACC != NULL)
		{
			wtp = AC_WTP_ACC->wtp_list;
			while (wtp)
			{
				time(&timep);  
				p = localtime(&timep);
				wid_syslog_info("%s:%d signalToRaise: TIMER_TYPE_AC_ACCESS["MACSTR"] time %d:%d:%d\n",__func__, __LINE__,MAC2STR(wtp->WTPMAC),p->tm_hour,p->tm_min,p->tm_sec);
				if (timep - wtp->time_stamp > MAX_SURVIVAL_TIME)
				{
					prev = wtp;
					wtp = wtp->next;
					wid_syslog_debug_debug(WID_WTPINFO,"%s:%d CWHandleTimer: time out, free AC_WTP_ACC["MACSTR"]\n",__func__,__LINE__,MAC2STR(prev->WTPMAC));
					free_ap(AC_WTP_ACC,prev);
				}
				else
				{
					wtp = wtp->next;
				}
			}
		}
		
		if (!(CWTimerRequest(MAX_SURVIVAL_TIME, NULL, &TimerID, TIMER_TYPE_AC_ACCESS, 0)))
		{
			CWThreadMutexUnlock(&ACAccessWTP);
			return ;
		}
		
		CWThreadMutexUnlock(&ACAccessWTP);
	}
 //	CWDebugLog("Timer Expired, Sent Signal(%d) to Thread: %d", signalToRaise, requestedThreadPtr);

//	CW_FREE_OBJECT(a->requestedThreadPtr);
	CW_FREE_OBJECT(a);

	return;
}

CWBool CWTimerRequest(int sec, CWThread *threadPtr, CWTimerID *idPtr, int signalToRaise, int ID)
{
	CWThreadTimerArg *arg = NULL;

	CWDebugLog("Timer Request");
	
	if(sec < 0 || idPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}    
	
	CW_CREATE_OBJECT_ERR(arg, CWThreadTimerArg, return CW_FALSE;);
//	CW_CREATE_OBJECT_ERR(arg->requestedThreadPtr, CWThread, CW_FREE_OBJECT(arg); return CW_FALSE;);
 //	CW_COPY_MEMORY(arg->requestedThreadPtr, threadPtr, sizeof(CWThread));
 	memset(arg, 0 ,sizeof(CWThreadTimerArg));
 	arg->signalToRaise = signalToRaise;
 	arg->WTPID = ID;
	arg->WLANID = ID;
	arg->RADIOID = ID;
	arg->AC_ACCTH_ID = ID;
	
//	CWDebugLog("Timer Request: thread(%d), signal(%d)", *(arg->requestedThreadPtr), arg->signalToRaise);
//	printf("CWTimerRequest3\n");
	
	if ((*idPtr = timer_add(sec, 0, &CWHandleTimer, arg)) == -1)
	{
		CW_FREE_OBJECT(arg);
		return CW_FALSE;
	}

	return CW_TRUE;
}

