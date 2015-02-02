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
* ACMsgq.c
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
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "CWAC.h"
#include "wcpss/waw.h"
#include "wcpss/wid/WID.h"
#include "CWCommon.h"
#include "ACDbus.h"
#include "dbus/asd/ASDDbusDef1.h"
#include "dbus/wcpss/ACDbusDef1.h"
#include "ACDbus_handler.h"
#include "AC.h"


void WID_INSERT_CONTROL_LIST(unsigned int WTPID, struct msgqlist *elem)
{
	struct msgqlist *tmp = NULL;

	if (AC_WTP[WTPID] == NULL)
	{
		return;
	}
	
	CWThreadMutexLock(&(gWTPs[WTPID].mutex_controlList));
	if (AC_WTP[WTPID]->elem_num >= 500)//qiuchen change it 2012.10.26
	{
		CWThreadMutexUnlock(&(gWTPs[WTPID].mutex_controlList));
		WID_CLEAN_CONTROL_LIST(WTPID);
		WID_CONFIG_SAVE(WTPID);
		CWThreadMutexLock(&(gWTPs[WTPID].mutex_controlList)); 
	}	
	if (AC_WTP[WTPID]->ControlList == NULL)
	{
		AC_WTP[WTPID]->ControlList = elem;
		elem->next = NULL;
		AC_WTP[WTPID]->elem_num++;
		goto out;
	}
	tmp = AC_WTP[WTPID]->ControlList;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	tmp->next = elem;
	elem->next = NULL;
	AC_WTP[WTPID]->elem_num++;
out:
	CWThreadMutexUnlock(&(gWTPs[WTPID].mutex_controlList));
	return;
}


void WID_CONFIG_SAVE(unsigned int WTPIndex)
{
	int i = 0;
	unsigned char eth_index = 0;
	msgq msg;
	struct msgqlist *elem = NULL;	
	int bind_wlan = 0;

	for (i = 0; i < AC_WTP[WTPIndex]->RadioCount; i++)
	{
		if (AC_WTP[WTPIndex]->WTP_Radio[i]->REFlag == 1)
		{
			wid_wtp_radio_extern_command_check(WTPIndex,i);
		}
		
		/*11n parameter set,zhangshu modify 2010-11-25*/
		if (((AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_Type & IEEE80211_11N) == IEEE80211_11N)
			|| ((AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_Type & IEEE80211_11AC) == IEEE80211_11AC))
		{
			unsigned int Radio_ID = WTPIndex * L_RADIO_NUM + i;
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->Ampdu.Able != 1)
			{
				wid_radio_set_ampdu_able(Radio_ID, 1);
			}
			if ((AC_WTP[WTPIndex]->WTP_Radio[i]->Ampdu.Able == 1)
				&& (AC_WTP[WTPIndex]->WTP_Radio[i]->Ampdu.AmpduLimit != 65535))
			{
				wid_radio_set_ampdu_limit(Radio_ID, 1);
			}
			if ((AC_WTP[WTPIndex]->WTP_Radio[i]->Ampdu.Able == 1)
				&& (AC_WTP[WTPIndex]->WTP_Radio[i]->Ampdu.subframe!= 32))
			{
				wid_radio_set_ampdu_subframe(Radio_ID, 1);
			}
    		if (AC_WTP[WTPIndex]->WTP_Radio[i]->Amsdu.Able != 0)
			{
				wid_radio_set_ampdu_able(Radio_ID, 2);
			}
			if ((AC_WTP[WTPIndex]->WTP_Radio[i]->Amsdu.Able == 1)
				&& (AC_WTP[WTPIndex]->WTP_Radio[i]->Amsdu.AmsduLimit != 4000))
			{
				wid_radio_set_ampdu_limit(Radio_ID, 2);
			}
			if ((AC_WTP[WTPIndex]->WTP_Radio[i]->Amsdu.Able == 1)
				&& (AC_WTP[WTPIndex]->WTP_Radio[i]->Amsdu.subframe!= 32))
			{
				wid_radio_set_ampdu_subframe(Radio_ID, 2);
			}
			/*if((AC_WTP[WTPIndex]->WTP_Radio[i]->MixedGreenfield.Mixed_Greenfield != 0)
				&&((AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_Type != 10)
					&&(AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_Type != 12))) //fengwenchao modify 20120716 for autelan-3057
			{
				wid_radio_set_mixed_puren_switch(Radio_ID);
			}*/
			if ((AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_Type != 10) && (AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_Type != 12))
			{
				unsigned int r = 0;
				for (r = 1; r < WLAN_NUM; r++)
				{
					if (AC_WTP[WTPIndex]->WTP_Radio[i]->MixedGreenfield.Wlan_Mixed[r] != 0)
					{
						wid_radio_set_mixed_puren_switch(Radio_ID, r);
					}
				}
			}
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->channel_offset != 0)
			{
				wid_radio_set_channel_Extoffset(Radio_ID);
			}
			/* zhangshu add for set chainmask, 2010-11-25 */
			if (((AC_WTP[WTPIndex]->WTP_Radio[i]->chainmask_num == 1)
					&& (AC_WTP[WTPIndex]->WTP_Radio[i]->tx_chainmask_state_value != 1)) 
				|| ((AC_WTP[WTPIndex]->WTP_Radio[i]->chainmask_num == 2) 
					&& (AC_WTP[WTPIndex]->WTP_Radio[i]->tx_chainmask_state_value != 3))
			 	|| ((AC_WTP[WTPIndex]->WTP_Radio[i]->chainmask_num == 3)
					&& (AC_WTP[WTPIndex]->WTP_Radio[i]->tx_chainmask_state_value != 7)))
			{
				wid_radio_set_chainmask(Radio_ID,1);
			}
			if (((AC_WTP[WTPIndex]->WTP_Radio[i]->chainmask_num == 1)
					&& (AC_WTP[WTPIndex]->WTP_Radio[i]->rx_chainmask_state_value != 1)) 
			 	|| ((AC_WTP[WTPIndex]->WTP_Radio[i]->chainmask_num == 2)
					&& (AC_WTP[WTPIndex]->WTP_Radio[i]->rx_chainmask_state_value != 3))
				|| ((AC_WTP[WTPIndex]->WTP_Radio[i]->chainmask_num == 3)
					&& (AC_WTP[WTPIndex]->WTP_Radio[i]->rx_chainmask_state_value != 7)))
			{
				wid_radio_set_chainmask(Radio_ID,2);
			}
			
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->guardinterval != 1)
			{
				wid_radio_set_guard_interval(WTPIndex*L_RADIO_NUM+i);
			}
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->cwmode != 0)
			{
				wid_radio_set_cmmode(WTPIndex*L_RADIO_NUM+i);
			}
			/*if (AC_WTP[WTPIndex]->WTP_Radio[i]->mcs != 0){
				wid_radio_set_mcs(WTPIndex*L_RADIO_NUM+i);
			}*/
			/*fengwenchao add 20120314 for requirements-407*/
			if (check_ac_whether_or_not_set_mcs_list(WTPIndex,i) == 1)
			{
				wid_radio_set_mcs_list	(WTPIndex*L_RADIO_NUM+i);
			}
			/*fengwenchao add end*/
		}
		if (AC_WTP[WTPIndex]->WTP_Radio[i]->ack.distance != 0)
		{
				wid_radio_set_acktimeout_distance(AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_G_ID);
		}/*wcl add for RDIR-33*/
		/*fengwenchao add 20110920 for radio disable config save*/
		if (AC_WTP[WTPIndex]->WTP_Radio[i]->radio_disable_flag == 1)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = Radio_S_TYPE;
			msg.mqinfo.u.RadioInfo.Radio_Op = Radio_STATUS;
			msg.mqinfo.u.RadioInfo.Radio_L_ID = i;
			msg.mqinfo.u.RadioInfo.Radio_G_ID = AC_WTP[WTPIndex]->WTP_Radio[i]->Radio_G_ID;
			struct msgqlist *elem;
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				wid_syslog_crit("%s malloc %s",__func__,strerror(errno));
				perror("malloc");
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
			elem = NULL;			
		}
		/*fengwenchao add end*/
		for (bind_wlan = 0; bind_wlan < 8; bind_wlan++)
		{
			if (AC_WTP[WTPIndex]->WTP_Radio[i]->cpe_intf[bind_wlan].vlan_count != 0)
			{
				wid_radio_set_cpe_channel(WTPIndex,i,bind_wlan);
			}
		}
		if (AC_WTP[WTPIndex]->WTP_Radio[i]->spec_analysis.enalbe)
		{
			set_ap_spectrum_analysis(WTPIndex*L_RADIO_NUM+i);
		}
	}

	if (AC_WTP[WTPIndex]->dhcp_snooping == 1)
	{
		memset((char*)&msg, 0, sizeof(msg));
		msg.mqid = WTPIndex%THREAD_NUM+1;
		msg.mqinfo.WTPID = WTPIndex;
		msg.mqinfo.type = CONTROL_TYPE;
		msg.mqinfo.subtype = WTP_S_TYPE;
		msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_DHCP_SNOOPING;
		msg.mqinfo.u.WtpInfo.value2 = AC_WTP[WTPIndex]->dhcp_snooping;
		
		elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
		if(elem == NULL)
		{
			WID_MALLOC_ERR();
			return ;
		}
		memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
		elem->next = NULL;
		memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
		WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		elem = NULL;
	}
	if(AC_WTP[WTPIndex]->sta_ip_report == 1)
	{
		memset((char*)&msg, 0, sizeof(msg));
		msg.mqid = WTPIndex%THREAD_NUM+1;
		msg.mqinfo.WTPID = WTPIndex;
		msg.mqinfo.type = CONTROL_TYPE;
		msg.mqinfo.subtype = WTP_S_TYPE;
		msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_INFO_REPORT;
		msg.mqinfo.u.WtpInfo.value2 = AC_WTP[WTPIndex]->sta_ip_report;
		
		elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
		if (elem == NULL)
		{
			WID_MALLOC_ERR();
			return ;
		}
		memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
		elem->next = NULL;
		memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
		WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		elem = NULL;
	}

	if (AC_WTP[WTPIndex]->wifi_extension_reportswitch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_EXTEND_INFO_GET;
			
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	
	if (AC_WTP[WTPIndex]->ap_sta_wapi_report_switch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_WAPI_INFO_SET;
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if(elem == NULL)
			{
				WID_MALLOC_ERR();
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
    }
	
	if (AC_WTP[WTPIndex]->ap_sta_report_switch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_INFO_SET;
			
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				wid_syslog_info("%s malloc %s",__func__,strerror(errno));
				perror("malloc");
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	//fengwenchao add 20110126 for XJDEV-32  from 2.0
	for (eth_index = 0; eth_index < AP_ETH_IF_NUM; eth_index++)
	{
		if (AC_WTP[WTPIndex]->apifinfo.eth[eth_index].eth_mtu != 1500)
		{
			wid_set_ap_eth_if_mtu(WTPIndex,eth_index);
		}
	}
	//fengwenchao add end
	if (AC_WTP[WTPIndex]->apifinfo.report_switch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_IF_INFO_SET;

			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();			
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	if (gCWNeighborDeadInterval != CW_NEIGHBORDEAD_INTERVAL_DEFAULT)
	{
		wid_set_neighbordead_intervalt(WTPIndex,gCWNeighborDeadInterval);
	}
	if ((AC_WTP[WTPIndex]->apstatisticsinterval != 1800) || (apstatistics != 0)/*switch is enable,so send interval*/)
	{
		msgq msg;
		struct msgqlist *elem = NULL;
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STATISTICS_REPORT_INTERVAL;
			msg.mqinfo.u.WtpInfo.value2 = AC_WTP[WTPIndex]->apstatisticsinterval;

			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();			
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}

	}
	 /* zhangshu add for terminal disturb information, 2010-10-08 */
    if (AC_WTP[WTPIndex]->ter_dis_info.reportswitch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_TERMINAL_DISTRUB_INFO;
			
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();			
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	if (AC_WTP[WTPIndex]->sta_deauth_message_reportswitch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_DEAUTH_SWITCH;
			
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();			
				return;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	if (AC_WTP[WTPIndex]->sta_flow_information_reportswitch == 1)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_STA_FLOW_INFORMATION_SWITCH;
			
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();			
				return ;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	if (AC_WTP[WTPIndex]->option60_param != NULL)
	{	
		memset((char*)&msg, 0, sizeof(msg));
		msg.mqid = WTPIndex%THREAD_NUM+1;
		msg.mqinfo.WTPID = WTPIndex;
		msg.mqinfo.type = CONTROL_TYPE;
		msg.mqinfo.subtype = WTP_S_TYPE;
		msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_OPTION60_PARAM;
		memcpy(msg.mqinfo.u.WtpInfo.value, AC_WTP[WTPIndex]->option60_param, strlen(AC_WTP[WTPIndex]->option60_param));
		elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
		if (elem == NULL)
		{
			WID_MALLOC_ERR();
			return ;
		}
		memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
		elem->next = NULL;
		memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
		WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		
	}
	
	if (AC_WTP[WTPIndex]->electronic_menu != 0)
	{
		if (AC_WTP[WTPIndex] != NULL)
		{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_ELECTRONIC_MENU;
			msg.mqinfo.u.WtpInfo.value[0] = 0;
			msg.mqinfo.u.WtpInfo.value[1] = 0;
			msg.mqinfo.u.WtpInfo.value[2] = LEVEL_AP;
			msg.mqinfo.u.WtpInfo.value[3] = AC_WTP[WTPIndex]->electronic_menu;

			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();
				return;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}

	if (AC_WTP[WTPIndex]->macgroupID != 0)
	{
		unsigned int groupid = AC_WTP[WTPIndex]->macgroupID;
		if (AC_WTP[WTPIndex] != NULL)
		{		
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPIndex%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPIndex;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_BLACK_WHITE_LIST;
			msg.mqinfo.u.WtpInfo.value[0] = 0;
			msg.mqinfo.u.WtpInfo.value[1] = 0;
			msg.mqinfo.u.WtpInfo.value[2] = AC_WTP[WTPIndex]->mac_grp_policy;
			msg.mqinfo.u.WtpInfo.value[3] = LEVEL_AP;
			msg.mqinfo.u.WtpInfo.value[4] = OVERWRITE;
			CWThreadMutexLock(&MAC_GROUP[groupid]->macListMutex);
			msg.mqinfo.u.WtpInfo.value[5] = MAC_GROUP[groupid]->count;
			MAC_list *maclist = MAC_GROUP[groupid]->macList;
			for (i = 0; i < MAC_GROUP[groupid]->count; i++)
			{
				memcpy(msg.mqinfo.u.WtpInfo.macarry[i].mac,maclist->mac,sizeof(maclist->mac));
				maclist = maclist->next;
			}
			CWThreadMutexUnlock(&MAC_GROUP[groupid]->macListMutex);
			elem = (struct msgqlist*)malloc(sizeof(struct msgqlist));
			if (elem == NULL)
			{
				WID_MALLOC_ERR();
				return;
			}
			memset((char*)&(elem->mqinfo), 0, sizeof(msgqdetail));
			elem->next = NULL;
			memcpy((char*)&(elem->mqinfo),(char*)&(msg.mqinfo),sizeof(msg.mqinfo));
			WID_INSERT_CONTROL_LIST(WTPIndex, elem);
		}
	}
	
	if (WTP_JOIN5G_SWITCH_ENABLE == AC_WTP[WTPIndex]->join5g.join5g_switch)
	{
		set_ap_5g_first(WTPIndex);
	}
	
	if ((0 != AC_WTP[WTPIndex]->lan_vlan.state) && (0 != AC_WTP[WTPIndex]->lan_vlan.vlanid))
	{
		set_wtp_lan_vlan(WTPIndex);	
	}
}


struct msgqlist *WID_GET_CONTROL_LIST_ELEM(unsigned int WTPID)
{
	struct msgqlist *tmp = NULL;
	
	CWThreadMutexLock(&(gWTPs[WTPID].mutex_controlList)); 
	if ((AC_WTP[WTPID] != NULL) && (AC_WTP[WTPID]->ControlList != NULL))
	{
		tmp = AC_WTP[WTPID]->ControlList;
		AC_WTP[WTPID]->ControlList = AC_WTP[WTPID]->ControlList->next;
		tmp->next = NULL;
		AC_WTP[WTPID]->elem_num --;
		CWThreadMutexUnlock(&(gWTPs[WTPID].mutex_controlList));
		
		return tmp;
	}
	CWThreadMutexUnlock(&(gWTPs[WTPID].mutex_controlList));

	return NULL;
}

void WID_CLEAN_CONTROL_LIST(unsigned int WTPID)
{
	struct msgqlist *tmp = NULL;
	struct msgqlist *tmp_t = NULL;
	
	CWThreadMutexLock(&(gWTPs[WTPID].mutex_controlList));
	CW_FREE_OBJECT(AC_WTP[WTPID]->ControlWait);
	if (AC_WTP[WTPID]->ControlList == NULL)
	{
		AC_WTP[WTPID]->elem_num = 0;
		goto out;
	}
	tmp = AC_WTP[WTPID]->ControlList;
	while (tmp != NULL)
	{
		tmp_t = tmp;		
		tmp = tmp->next;
		tmp_t->next = NULL;
		CW_FREE_OBJECT(tmp_t);
	}
	AC_WTP[WTPID]->ControlList = NULL;	
	AC_WTP[WTPID]->elem_num = 0;
out:
	CWThreadMutexUnlock(&(gWTPs[WTPID].mutex_controlList));
	
	return;
}

void WID_CLEAN_WTP_STA_LIST(unsigned int WTPID)
{
	int i = 0,  j = 0;
	unsigned int bssindex = 0;
	WID_CHECK_WTP_STANDARD(WTPID);
	
	for(i = 0; ((i < L_RADIO_NUM) && (i < AC_WTP[WTPID]->RadioCount)); i++)
	{
		for(j = 0; j < L_BSS_NUM; j++)
		{
			if((AC_WTP[WTPID])&&(AC_WTP[WTPID]->WTP_Radio[i])&&(AC_WTP[WTPID]->WTP_Radio[i]->BSS[j] != NULL))
			{
				bssindex = AC_WTP[WTPID]->WTP_Radio[i]->BSS[j]->BSSIndex;
				asd_free_stas(bssindex);
				AC_WTP[WTPID]->WTP_Radio[i]->BSS[j]->State = 0;
				memset(AC_WTP[WTPID]->WTP_Radio[i]->BSS[j]->BSSID, 0, MAC_LEN);
			}
		}
	}
	
	return;
}


