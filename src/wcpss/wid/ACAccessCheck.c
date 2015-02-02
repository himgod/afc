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
* ACAccessCheck.c
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
#include "wcpss/waw.h"
#include "dbus/asd/ASDDbusDef1.h"
#include "wcpss/wid/WID.h" 
#include "CWAC.h"
#include "ACDbus.h"
#include "CWStevens.h"
#include <syslog.h>
#include "dbop.h"
#include "ACDbus_handler.h"

#define WTP_IP_HASH(ip) (ip[3])

struct wtp_access_info * get_ap(WID_ACCESS *AC, unsigned int ipip)
{
	struct wtp_access_info *s = NULL;
	unsigned char *ip = (unsigned char*)&(ipip);
	s = AC->wtp_list_hash[WTP_IP_HASH(ip)];
	while ((s != NULL) && (s->ip != ipip))
	{
		s = s->hnext;
	}
	return s;
}


static void ap_list_del(WID_ACCESS *AC, struct wtp_access_info *wtp)
{
	struct wtp_access_info *tmp = NULL;

	if (AC->wtp_list == wtp)
	{
		AC->wtp_list = wtp->next;
		return;
	}

	tmp = AC->wtp_list;
	while (tmp != NULL && tmp->next != wtp)
	{
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{

	}
	else
	{
		tmp->next = wtp->next;
	}
}


void ap_hash_add(WID_ACCESS *AC, struct wtp_access_info *wtp)
{
	unsigned char *ipaddr = (unsigned char *)&(wtp->ip);
	wtp->hnext = AC->wtp_list_hash[WTP_IP_HASH(ipaddr)];
	AC->wtp_list_hash[WTP_IP_HASH(ipaddr)] = wtp;
}


static void ap_hash_del(WID_ACCESS *AC, struct wtp_access_info *wtp)
{
	struct wtp_access_info *s = NULL;
	unsigned char *ipaddr = (unsigned char *)&(wtp->ip);
	s = AC->wtp_list_hash[WTP_IP_HASH(ipaddr)];
	if (s == NULL) return;
	if (s->ip == wtp->ip)
	{
		AC->wtp_list_hash[WTP_IP_HASH(ipaddr)] = s->hnext;
		return;
	}

	while (s->hnext != NULL && (s->hnext != wtp))
		s = s->hnext;
	if (s->hnext != NULL)
		s->hnext = s->hnext->hnext;
	else
		;
}


void free_ap(WID_ACCESS *AC, struct wtp_access_info *wtp)
{
	ap_hash_del(AC, wtp);
	ap_list_del(AC, wtp);
	AC->num--;
	CW_FREE_OBJECT(wtp->apcode);
	CW_FREE_OBJECT(wtp->model);
	CW_FREE_OBJECT(wtp->sn);
	CW_FREE_OBJECT(wtp->version);
	CW_FREE_OBJECT(wtp->codever);
	CW_FREE_OBJECT(wtp->WTPMAC);
	CW_FREE_OBJECT(wtp->ifname);
	CW_FREE_OBJECT(wtp);
}


struct wtp_access_info *ap_add(WID_ACCESS *AC, struct sockaddr_in *sa, CWWTPVendorInfos *valPtr, CWWTPDescriptor *valPtr1, char *name)
{
	int i = 0;
	int ret = 0;
	char wtpname[WTP_NAME_LEN] = {0};
	struct tm *p = NULL; 
	struct wtp_access_info *wtp = NULL;
	
	wtp = get_ap(AC, sa->sin_addr.s_addr);
	if (wtp)
	{
		memset(wtp->WTPMAC, 0, MAC_LEN);		
		memset(wtp->model, 0, strlen(wtp->model));
		memset(wtp->apcode, 0, strlen(wtp->model));
		memset(wtp->sn, 0, strlen(wtp->sn));
		memset(wtp->version, 0, strlen(wtp->version));
		memset(wtp->codever, 0, strlen(wtp->codever));
		memset(wtp->ifname, 0, 16);
		for (i = 0; i < valPtr->vendorInfosCount; i++)
		{
			if ((valPtr->vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
			{
				CW_FREE_OBJECT(wtp->apcode);				
				wtp->apcode= ( char *)malloc((valPtr->vendorInfos)[i].length + 1);	
				memset(wtp->apcode, 0, (valPtr->vendorInfos)[i].length + 1);
				memcpy(wtp->apcode, (valPtr->vendorInfos)[i].model, (valPtr->vendorInfos)[i].length);
			}
			else if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
			{
				CW_FREE_OBJECT(wtp->sn);
				wtp->sn = ( char *)malloc((valPtr->vendorInfos)[i].length + 1);
				memset(wtp->sn, 0, (valPtr->vendorInfos)[i].length + 1);
				if (wid_illegal_character_check((char*)(valPtr->vendorInfos)[i].SN,(valPtr->vendorInfos)[i].length,1) > 0)
				{
					memcpy(wtp->sn, (valPtr->vendorInfos)[i].SN, (valPtr->vendorInfos)[i].length);
				}
				else
				{
					wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"sn has non readable char\n",WAI_FUNC_LINE_LITERAL, 0);
				}
			}
			else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
			{
				if (memcmp(wtp->WTPMAC, (valPtr->vendorInfos)[i].mac, (valPtr->vendorInfos)[i].length))
				{
					memcpy(wtp->WTPMAC, (valPtr->vendorInfos)[i].mac, (valPtr->vendorInfos)[i].length);
					
					snprintf(wtpname, 31, "AFi_%02X%02X%02X", wtp->WTPMAC[3], wtp->WTPMAC[4], wtp->WTPMAC[5]);
					memset(wtp->WTPNAME, 0, WTP_NAME_LEN);
					memcpy(wtp->WTPNAME, wtpname, strlen(wtpname));
					
					if ((g_auto_ap_login.auto_ap_switch == 0)
						&& (-1 == WTPMacCheck(wtp->WTPMAC))
						&& (-1 == get_access_wtp_alert(wtp->WTPMAC)))
					{
						ret = active_ap_discovery_pending(wtp->WTPMAC);
						if (ret >= 0)
						{
							access_add_new_alert(wtp->WTPMAC, ret, CW_FALSE);
						}
					}
				}
			}
			else if ((valPtr->vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
			{ 
				CW_FREE_OBJECT(wtp->model);
				wtp->model= ( char *)malloc((valPtr->vendorInfos)[i].length + 1);
				memset(wtp->model, 0, (valPtr->vendorInfos)[i].length + 1);
				memcpy(wtp->model, (valPtr->vendorInfos)[i].Rmodel, (valPtr->vendorInfos)[i].length);			
			}
			else if ((valPtr->vendorInfos)[i].type == CW_WTP_CODE_VERSION)//CW_WTP_CODE_VERSION = 6
			{
				CW_FREE_OBJECT(wtp->codever);
				wtp->codever = ( char *)malloc((valPtr->vendorInfos)[i].length + 1);
				memset(wtp->codever, 0, (valPtr->vendorInfos)[i].length + 1);
				if (wid_illegal_character_check((char*)(valPtr->vendorInfos)[i].codever,(valPtr->vendorInfos)[i].length,1) > 0)
				{
					memcpy(wtp->codever, (valPtr->vendorInfos)[i].codever, (valPtr->vendorInfos)[i].length);	
				}
				else
				{
					wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"code version has non readable char\n", \
								WAI_FUNC_LINE_LITERAL, 0);
				}
			}
			else
			{
				continue;
			}
		}
		
		for (i = 0; i < valPtr1->vendorInfos.vendorInfosCount; i++)
		{
			if ((valPtr1->vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
			{
				CW_FREE_OBJECT(wtp->version);
				wtp->version = (char *)malloc((valPtr1->vendorInfos.vendorInfos)[i].length+1);
				memset(wtp->version, 0, (valPtr1->vendorInfos.vendorInfos)[i].length+1);
				memcpy(wtp->version, (valPtr1->vendorInfos.vendorInfos)[i].ver, (valPtr1->vendorInfos.vendorInfos)[i].length);
			}
			else
			{
				continue;
			}
		}
		
		memcpy(wtp->ifname, name, strlen(name));
		
		time(&(wtp->time_stamp));	
		p = localtime(&(wtp->time_stamp));
		wid_syslog_info("%s:%d get ap["MACSTR"] time %d:%d:%d\n",__func__, __LINE__,MAC2STR(wtp->WTPMAC),p->tm_hour,p->tm_min,p->tm_sec);
		return wtp;
	}
	
	wtp = malloc(sizeof(struct wtp_access_info));
	if (wtp == NULL)
	{
		return NULL;
	}
	memset(wtp, 0, sizeof(struct wtp_access_info));	
	wtp->WTPMAC = (unsigned char *)malloc(MAC_LEN+1);
	memset(wtp->WTPMAC, 0, MAC_LEN);
	wtp->model= ( char *)malloc(1);
	memset(wtp->model, 0, 1);
	wtp->apcode= ( char *)malloc(1);	
	memset(wtp->apcode, 0, 1);
	wtp->sn = ( char *)malloc(1);
	memset(wtp->sn, 0, 1);
	wtp->version = ( char *)malloc(1);
	memset(wtp->version, 0, 1);
	wtp->codever = ( char *)malloc(1);
	memset(wtp->codever, 0, 1);
	wtp->ifname = ( char *)malloc(16);
	memset(wtp->ifname, 0, 16);

	for (i = 0; i < valPtr->vendorInfosCount; i++)
	{
		if ((valPtr->vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{	
			CW_FREE_OBJECT(wtp->apcode);
			wtp->apcode= ( char *)malloc((valPtr->vendorInfos)[i].length + 1);	
			memset(wtp->apcode, 0, (valPtr->vendorInfos)[i].length + 1);
			memcpy(wtp->apcode, (valPtr->vendorInfos)[i].model, (valPtr->vendorInfos)[i].length);
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			CW_FREE_OBJECT(wtp->sn);
			wtp->sn = ( char *)malloc((valPtr->vendorInfos)[i].length + 1);
			memset(wtp->sn, 0, (valPtr->vendorInfos)[i].length + 1);
			memcpy(wtp->sn, (valPtr->vendorInfos)[i].SN, (valPtr->vendorInfos)[i].length);
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{			
			memset(wtp->WTPMAC, 0, MAC_LEN);
			memcpy(wtp->WTPMAC, (valPtr->vendorInfos)[i].mac, (valPtr->vendorInfos)[i].length);
			snprintf(wtpname, 31, "AFi_%02X%02X%02X", wtp->WTPMAC[3], wtp->WTPMAC[4], wtp->WTPMAC[5]);
			memcpy(wtp->WTPNAME, wtpname, strlen(wtpname));
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			CW_FREE_OBJECT(wtp->model);
			wtp->model= ( char *)malloc((valPtr->vendorInfos)[i].length + 1);
			memset(wtp->model,0,(valPtr->vendorInfos)[i].length + 1);
			memcpy(wtp->model, (valPtr->vendorInfos)[i].Rmodel, (valPtr->vendorInfos)[i].length);			
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_CODE_VERSION)//CW_WTP_CODE_VERSION = 6
		{
			CW_FREE_OBJECT(wtp->codever);
			wtp->codever = ( char *)malloc((valPtr->vendorInfos)[i].length + 1);
			memset(wtp->codever, 0, (valPtr->vendorInfos)[i].length + 1);		
			if (wid_illegal_character_check((char*)(valPtr->vendorInfos)[i].codever,(valPtr->vendorInfos)[i].length,1) > 0)
			{
				memcpy(wtp->codever, (valPtr->vendorInfos)[i].codever, (valPtr->vendorInfos)[i].length);	
			}
			else
			{
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"code version has non readable char\n", WAI_FUNC_LINE_LITERAL, 0);
			}
		}		
		else
		{
			continue;
		}
	}

	for (i = 0; i < valPtr1->vendorInfos.vendorInfosCount; i++)
	{
		if ((valPtr1->vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
		{
			CW_FREE_OBJECT(wtp->version);
			wtp->version = (char *)malloc((valPtr1->vendorInfos.vendorInfos)[i].length+1);
			memset(wtp->version, 0, (valPtr1->vendorInfos.vendorInfos)[i].length+1);
			memcpy(wtp->version, (valPtr1->vendorInfos.vendorInfos)[i].ver, (valPtr1->vendorInfos.vendorInfos)[i].length);
		}
		else
		{
			continue;
		}
	}

	memcpy(wtp->ifname, name, strlen(name));
	time(&(wtp->time_stamp));
	p = localtime(&(wtp->time_stamp));
	wid_syslog_info("%s:%d get ap["MACSTR"] time %d:%d:%d\n",__func__, __LINE__,MAC2STR(wtp->WTPMAC),p->tm_hour,p->tm_min,p->tm_sec);
//	memcpy(&(wtp->ip),&(sa->sin_addr.s_addr),sizeof(int));
	wtp->ip = sa->sin_addr.s_addr;
	wtp->next = AC->wtp_list;
	AC->wtp_list = wtp;
	AC->num++;
	ap_hash_add(AC, wtp);

	if ((g_auto_ap_login.auto_ap_switch == 0)
		&& (-1 == WTPMacCheck(wtp->WTPMAC))
		&& (-1 == get_access_wtp_alert(wtp->WTPMAC)))
	{
		ret = active_ap_discovery_pending(wtp->WTPMAC);
		if (ret >= 0)
		{
			access_add_new_alert(wtp->WTPMAC, ret, CW_FALSE);
		}
	}
	
	return wtp;
}

void del_all_ap(WID_ACCESS *AC)
{
	struct wtp_access_info *wtp = NULL, *prev = NULL;
	if (AC != NULL)
	{
		wtp = AC->wtp_list;
		while (wtp)
		{
			prev = wtp;
			wtp = wtp->next;
			free_ap(AC,prev);
		}
	}
}

