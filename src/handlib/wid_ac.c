#include <string.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include "wcpss/waw.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/ACDBusPath.h"
#include "dbus/asd/ASDDbusPath.h"
#include "dbus/wcpss/ACDbusDef1.h"
#include "dbus/asd/ASDDbusDef1.h"
#include "wcpss/asd/asd.h"
#include "wid_ac.h"
#include "check_argument.h"
#include <syslog.h>

#define LOCALID 0 
#define LOCALINDEX 1

void ReInitDbusPath(int index, char * path, char * newpath)
{
	int len = 0;
	sprintf(newpath,"%s%d",path,index);	
	if(path == ASD_DBUS_SECURITY_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","security");
	}
	else if(path == ASD_DBUS_SECURITY_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","security");			
	}
	else if(path == ASD_DBUS_STA_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","sta");
	}
	else if(path == ASD_DBUS_STA_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","sta");			
	}
	else if(path == WID_DBUS_WLAN_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","wlan");
	}
	else if(path == WID_DBUS_WLAN_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","wlan");			
	}
	else if(path == WID_DBUS_WTP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","wtp");
	}
	else if(path == WID_DBUS_WTP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","wtp");			
	}
	else if(path == WID_DBUS_MACGRP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","macgrp");
	}
	else if(path == WID_DBUS_MACGRP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","macgrp");			
	}
	else if(path == WID_DBUS_RADIO_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","radio");
	}
	else if(path == WID_DBUS_RADIO_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","radio");			
	}
	else if(path == WID_DBUS_QOS_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","qos");
	}
	else if(path == WID_DBUS_QOS_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","qos");			
	}
	else if(path == WID_DBUS_EBR_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","ebr");
	}
	else if(path == WID_DBUS_EBR_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","ebr");			
	}	
	else if(path == WID_BAK_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","bak");
	}
	else if(path == WID_BAK_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","bak");			
	}
	else if(path == WID_DBUS_ACIPLIST_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","aciplist");
	}
	else if(path == WID_DBUS_ACIPLIST_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","aciplist");			
	}
	else if(path == ASD_DBUS_AC_GROUP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","acgroup");
	}
	else if(path == ASD_DBUS_AC_GROUP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","acgroup");			
	}
	else if(path == WID_DBUS_AP_GROUP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","apgroup");
	}
	else if(path == WID_DBUS_AP_GROUP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","apgroup");			
	}
	else if(path == WID_DBUS_ACLGRP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","aclgrp");
	}
	else if(path == WID_DBUS_ACLGRP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","aclgrp");			
	}
}
void ReInitDbusPath_V2(int local,int index, char * path, char * newpath)
{
	int len = 0;
	sprintf(newpath,"%s%d_%d",path,local,index); 
	if(path == ASD_DBUS_SECURITY_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","security");
	}
	else if(path == ASD_DBUS_SECURITY_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","security");			
	}
	else if(path == ASD_DBUS_STA_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","sta");
	}
	else if(path == ASD_DBUS_STA_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","sta");			
	}
	else if(path == WID_DBUS_WLAN_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","wlan");
	}
	else if(path == WID_DBUS_WLAN_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","wlan");			
	}
	else if(path == WID_DBUS_WTP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","wtp");
	}
	else if(path == WID_DBUS_WTP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","wtp");			
	}
	else if(path == WID_DBUS_MACGRP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","macgrp");
	}
	else if(path == WID_DBUS_MACGRP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","macgrp");			
	}
	else if(path == WID_DBUS_RADIO_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","radio");
	}
	else if(path == WID_DBUS_RADIO_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","radio");			
	}
	else if(path == WID_DBUS_QOS_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","qos");
	}
	else if(path == WID_DBUS_QOS_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","qos");			
	}
	else if(path == WID_DBUS_EBR_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","ebr");
	}
	else if(path == WID_DBUS_EBR_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","ebr");			
	}	
	else if(path == WID_BAK_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","bak");
	}
	else if(path == WID_BAK_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","bak");			
	}
	else if(path == WID_DBUS_ACIPLIST_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","aciplist");
	}
	else if(path == WID_DBUS_ACIPLIST_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","aciplist");			
	}
	else if(path == ASD_DBUS_AC_GROUP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","acgroup");
	}
	else if(path == ASD_DBUS_AC_GROUP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","acgroup");			
	}
	else if(path == WID_DBUS_AP_GROUP_OBJPATH)
	{
		len = strlen(newpath);
		sprintf(newpath+len,"/%s","apgroup");
	}
	else if(path == WID_DBUS_AP_GROUP_INTERFACE)
	{	
		len = strlen(newpath);
		sprintf(newpath+len,".%s","apgroup");			
	}
	else if(path == WID_DBUS_ACLGRP_OBJPATH)
	{
			len = strlen(newpath);
			sprintf(newpath+len,"/%s","aclgrp");
	}
	else if(path == WID_DBUS_ACLGRP_INTERFACE)
	{	
			len = strlen(newpath);
			sprintf(newpath+len,".%s","aclgrp");			
	}
}
void ReInitDbusPath_Hmd(int slotid,char * path, char * newpath)
{
	sprintf(newpath,"%s%d",path,slotid); 
}

int wid_mac_format_check(char* str,int len) 
{
	int i = 0;
	unsigned int result = 0;
	char c = 0;
	
	if (17 != len)
	{
	   return -1;
	}
	for (; i < len; i++)
	{
		c = str[i];
		if ((2 == i) || (5 == i) || (8 == i) || (11 == i) || (14 == i))
		{
			if ((':' != c) && ('-' != c) && ('.' != c))
				return -1;
		}
		else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
			continue;
		else
		{
			result = -1;
			return result;
		}
    }
	if ((str[2] != str[5]) ||(str[2] != str[8]) || (str[2] != str[11])
		|| (str[2] != str[14]) || (str[5] != str[8]) || (str[5] != str[11])
		|| (str[5] != str[14]) || (str[8] != str[11])|| (str[8] != str[14]))
	{
        result = -1;
		return result;
	}
	return result;
}


int parse_country_code(char *input)
{
	if ((!strcmp(input,"cn"))||(!strcmp(input,"eu"))||(!strcmp(input,"us"))
		||(!strcmp(input,"jp"))||(!strcmp(input,"fr"))||(!strcmp(input,"es")))
	{
		return	COUNTRY_CODE_ERROR_SMALL_LETTERS;
	}
	
	if (!strcmp(input,"CN"))
	{
		return COUNTRY_CHINA_CN;
	}
	else if (!strcmp(input,"EU"))
	{
		return COUNTRY_EUROPE_EU;
	}
	else if (!strcmp(input,"US"))
	{
		return COUNTRY_USA_US;
	}
	else if (!strcmp(input,"JP"))
	{
		return COUNTRY_JAPAN_JP;
	}
	else if (!strcmp(input,"FR"))
	{
		return COUNTRY_FRANCE_FR;
	}
	else if (!strcmp(input,"ES"))
	{
		return COUNTRY_SPAIN_ES;
	}
	else 
	{
		return COUNTRY_CODE_ERROR;
	}
}


void CheckWIDSType(char *pattacktype, char* pframetype, unsigned char attacktype,unsigned char frametype)
{
	switch(attacktype)
	{
		case 1 :
		{
			strcpy(pattacktype, "flood");
			
			switch(frametype)
			{
				case 1 :
					strcpy(pframetype, "probe");
					break;
					
				case 2 :
					strcpy(pframetype, "auth");
					break;
				
				case 3 :
					strcpy(pframetype, "assoc");
					break;
					
				case 4 :
					strcpy(pframetype, "reassoc");
					break;
					
				case 5 :
					strcpy(pframetype, "deauth");
					break;
				
				case 6 :
					strcpy(pframetype, "deassoc");
						break;
				case 7 :
					strcpy(pframetype, "data");
					break;
				
				case 8 :
					strcpy(pframetype, "action");
					break;

				default:
					strcpy(pframetype, "unknown");
					break;
			}
		}
		break;
			
		case 2 :
		{
			strcpy(pattacktype, "spoof");
			switch(frametype)
			{
				case 1 :
					strcpy(pframetype, "deauth");
					break;
					
				case 2 :
					strcpy(pframetype, "deassoc");
					break;
					
				default:
					strcpy(pframetype, "unknown");
					break;
			}

		}
		break;
		
		case 3 :
		{
			strcpy(pattacktype, "weakiv");
			
			switch(frametype)
			{
				case 3 :
					strcpy(pframetype, "weakiv");
					break;
					
				default:
					strcpy(pframetype, "unknown");
					break;
			}
		}
		break;
		default:
			strcpy(pattacktype, "unknown");
			break;
	}
}

int parse_str_by_character(char* str, char *arg, char ch)
{
	int len = strlen(str);
	int i = 0;
	int c = 0;
	char *t_str = str;
	char *t_arg = arg;
	int ret = 0;
	while (len > 0)
	{
		for (i = 0; i < len; i++)
		{
			if (t_str[i] == ch)
			{
				memcpy(t_arg, t_str, i);
				len = len - i - 1;
				t_str = t_str + i + 1;
				t_arg = t_arg + i;
				memcpy(t_arg, "%d", 2);
				t_arg = t_arg +2;
				c++;
				i = 0;
				break;
			}
			else if (t_str[i] == '$')
			{		
				memcpy(t_arg,  t_str, i);
				len = len - i - 1;
				t_str = t_str + i+1;
				t_arg = t_arg + i;
				sprintf(t_arg, " \n");
				t_arg = t_arg + 2;
				i = 0 ;
				break;
			}
		}
		if (i == len)
		{
			memset(t_arg, 0, i+1);
			memcpy(t_arg, t_str, i);
			break;
		}
	}
	return c;
}


void hand_hex_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer)
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
	
	syslog(LOG_DEBUG, "%s-%d hex_essid:%s\n", __func__,__LINE__, ssidbuffer);
}


void hand_hex_char_dump_essid(unsigned char *buffer, int buffLen, unsigned char *ssidbuffer)
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
	
	syslog(LOG_DEBUG, "%s-%d hex_char essid:%s\n", __func__,__LINE__, ssidbuffer);
}

int parse_daemonlog_level(int Loglevel, char *arg)
{
	if (Loglevel == 0)
	{
		strcpy(arg, "CLOSE");
	}
	else if (Loglevel == WID_ALL)
	{
		strcpy(arg, "ALL");
	}
	else
	{ 
		if (Loglevel & WID_DEFAULT)
		{
			strncat(arg, "DEFAULT,",8);
		}
		if (Loglevel & WID_DBUS)
		{
			strncat(arg, "DBUS,",5);
		}
		if (Loglevel & WID_WTPINFO)
		{
			strncat(arg, "WTPINFO,",8);
		}
		if (Loglevel & WID_MB)
		{
			strncat(arg, "MB",2);
		}
	}
	
	return 0;
}

typedef enum
{
	check_id = 0,
	check_comma,
	check_fail,
	check_end,
	check_success,
	check_bar
}check_state;

int hand_add_softAc_user_group
(
	USERGRP_WEBINFO *usrgrpinfo,
	DBusConnection *dcli_dbus_connection
)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
    unsigned int groupid = 0;
	unsigned char *groupname = NULL;
	
	if (!usrgrpinfo || !dcli_dbus_connection)
	{
		return WID_DBUS_ERROR;
	}
	
	groupname = usrgrpinfo->grpnmame;
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_ADD_USER_GROUP);
	
	dbus_error_init(&err);
	
	if (!dbus_message_append_args(query,
							DBUS_TYPE_STRING,&groupname,
							DBUS_TYPE_UINT32,&usrgrpinfo->send_traffic_limit,
							DBUS_TYPE_UINT32,&usrgrpinfo->traffic_limit,
							DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	dbus_message_iter_next(&iter);
	dbus_message_iter_get_basic(&iter, &groupid);
	dbus_message_iter_next(&iter);

	usrgrpinfo->grpid = groupid;
	
	dbus_message_unref(reply);
	
	return ret;
}

int hand_del_softAc_user_group(unsigned int groupid, DBusConnection *dcli_dbus_connection)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	
	if (groupid < 1 || groupid > USER_GROUP_NUM)
	{
		return GROUP_ID_LARGE_THAN_MAX;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_DEL_USER_GROUP);
	dbus_error_init(&err);

	
	if(!dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &groupid,
							 DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	
	return ret;
}


int hand_modify_softAc_user_group_config
(	
    USERGRP_WEBINFO *usrgrpinfo,
	DBusConnection *dcli_dbus_connection
)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	unsigned char *groupname = NULL;
	
	if (!usrgrpinfo || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "modify usrgrpinfo %p or dcli_dbus_connection %p is NULL\n", usrgrpinfo, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	groupname = usrgrpinfo->grpnmame;
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_MODIFY_USER_GROUP_CONFIG);
	dbus_error_init(&err);
	
	if(!dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &usrgrpinfo->grpid,
							DBUS_TYPE_STRING, &groupname,
							DBUS_TYPE_UINT32, &usrgrpinfo->send_traffic_limit,
							DBUS_TYPE_UINT32, &usrgrpinfo->traffic_limit,
							DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		
		syslog(LOG_ERR, "wid return error\n");
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, &ret);

	dbus_message_unref(reply);
	
	return ret;
}


int hand_load_conf_softAc_usrgrp_config
(
	USERGRP_WEBINFO *usrgrpinfo,
	DBusConnection *dcli_dbus_connection
)
{
	DBusMessage *query = NULL, *reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	unsigned char *groupname = NULL;
	
	if (!usrgrpinfo || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "modify usrgrpinfo %p or dcli_dbus_connection %p is NULL\n", usrgrpinfo, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	groupname = usrgrpinfo->grpnmame;
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_LOAD_NETWORK_CONFIG);
	dbus_error_init(&err);
	
	if (!dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &usrgrpinfo->grpid,
							DBUS_TYPE_STRING, &groupname,
							DBUS_TYPE_UINT32, &usrgrpinfo->send_traffic_limit,
							DBUS_TYPE_UINT32, &usrgrpinfo->traffic_limit,
							DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		
		syslog(LOG_ERR, "wid return error\n");
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, &ret);
	
	dbus_message_unref(reply);
	
	return ret;
}

int hand_add_user_default_network(char *ssid,char *key, DBusConnection *dcli_dbus_connection)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;

	if(!ssid || !key || !dcli_dbus_connection)
	{
		return -1;
	}

	ret = ssid_illegal_character_check(ssid, SSID_LEN);
	if(ret) 
		return ret;

	ret = wpa_key_illegal_check(key, ASCII);
	if(ret)
		return ret;

	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_ADD_USER_NETWORK);
	dbus_error_init(&err);

	
	if(!dbus_message_append_args(query,
							 DBUS_TYPE_STRING,&ssid,
							 DBUS_TYPE_STRING,&key,
							 DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		//printf("<error> failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			//printf("%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		

		return -1;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	return ret;
}


int hand_add_guest_default_network(char *ssid, DBusConnection *dcli_dbus_connection)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	if(!ssid || !dcli_dbus_connection)
	{
		return -1;
	}

	
	ret = ssid_illegal_character_check(ssid, SSID_LEN);
	if(ret) 
		return ret;
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_ADD_GUEST_NETWORK);
	dbus_error_init(&err);

	
	if(!dbus_message_append_args(query,
							 DBUS_TYPE_STRING,&ssid,
							 DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		//printf("<error> failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			//printf("%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		

		return -1;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	return ret;
}


int hand_add_softAc_network
(
	WLANWEBINFO *wlaninfo,
	DBusConnection *dcli_dbus_connection
)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0, j = 0;
	int ret = 0;
	unsigned char cnFlag = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	int tmpwlanid = 0;
	char *ssid = NULL;
	char *user_key = NULL;
	int vlanId = 0;
	
	if (!wlaninfo || !dcli_dbus_connection)
	{
		return WID_DBUS_ERROR;
	}

	ssid = wlaninfo->ssid;
	user_key = wlaninfo->user_key;
	
	/*ret = ssid_illegal_character_check(ssid, strlen(ssid));
	if (ret)
	{
		return ret;
	}*/
	
	switch (wlaninfo->securType)
	{
		case WEP:
			ret = wep_key_illegal_check(user_key, ASCII);
			break;
		case WPA_P:
			ret = wpa_key_illegal_check(user_key, ASCII);
			break;
		case WPA_E:
			if (SPE_IP_ADDR(wlaninfo->Authip))
			{
				ret = WID_SERVER_IP_ERROR;
			}
			if (wlaninfo->AuthPort > 65535)
			{
				ret = WID_SERVER_PORT_ERROR;
			}
			if (!user_key)
			{
				ret = WID_SECURITY_KEY_ERROR;
			}
	}
	
	if (wlaninfo->vlanEnabled && wlaninfo->vlanId > 0)
	{
		vlanId = wlaninfo->vlanId;
	}
	
	if (check_user_vlanid(vlanId))
	{
		ret =  WID_USER_VLANID_ERROR ;
	}
	
	if (ret)
	{
		return ret;
	}
	
	ReInitDbusPath_V2(localid, index, WID_DBUS_BUSNAME, BUSNAME);
	ReInitDbusPath_V2(localid, index, WID_DBUS_OBJPATH, OBJPATH);
	ReInitDbusPath_V2(localid, index, WID_DBUS_INTERFACE, INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME, OBJPATH, INTERFACE, WID_DBUS_ADD_NETWORK);
	
	dbus_error_init(&err);
	
	if (!dbus_message_append_args(query,
							DBUS_TYPE_STRING,&ssid,
							DBUS_TYPE_STRING,&user_key,
							DBUS_TYPE_UINT32,&wlaninfo->securType,
							DBUS_TYPE_UINT32,&wlaninfo->sHideflag,
						 	DBUS_TYPE_UINT32,&wlaninfo->isEnable,
						 	DBUS_TYPE_UINT32,&wlaninfo->applyGflag,
						 	DBUS_TYPE_UINT32,&vlanId,
						 	DBUS_TYPE_UINT32,&wlaninfo->usrGid,
						 	DBUS_TYPE_UINT32,&wlaninfo->EncryptionType,
						 	DBUS_TYPE_UINT32,&wlaninfo->Authip,
						 	DBUS_TYPE_UINT32,&wlaninfo->AuthPort,
							DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	dbus_message_iter_next(&iter);
	dbus_message_iter_get_basic(&iter, &tmpwlanid);
	dbus_message_iter_next(&iter);

	wlaninfo->WlanId = tmpwlanid;
	
	dbus_message_unref(reply);
	
	return ret;
}

int hand_del_softAc_network(int WlanId, DBusConnection *dcli_dbus_connection)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	if(WlanId < 1 || WlanId > 16)
	{
		return WLAN_ID_ERROR;
	}
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_DEL_NETWORK);
	dbus_error_init(&err);

	
	if(!dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &WlanId,
							 DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		//printf("<error> failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			//printf("%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	
	return ret;
}


int hand_modify_softAc_network_config
(	
    WLANWEBINFO *wlaninfo,
	DBusConnection *dcli_dbus_connection
)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	char *ssid = NULL;
	char *user_key = NULL;
	int vlanId = 0;

	if(!wlaninfo || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "modify wlaninfo %p or dcli_dbus_connection %p is NULL\n", wlaninfo, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
    ssid = wlaninfo->ssid;
    user_key = wlaninfo->user_key;
    if(wlaninfo->vlanEnabled && wlaninfo->vlanId > 0)
    {
        vlanId = wlaninfo->vlanId;
    }
	/*ret = ssid_illegal_character_check(ssid, strlen(ssid));
	if(ret)
	{
		syslog(LOG_ERR, "ssid illegal check failed, ret %d!\n", ret);
		return ret;
	}*/

	switch(wlaninfo->securType)
	{
		case WEP:
			ret = wep_key_illegal_check(user_key, ASCII);
			break;
		case WPA_P:
			ret = wpa_key_illegal_check(user_key, ASCII);
			break;
		case WPA_E:
			if(SPE_IP_ADDR(wlaninfo->Authip))
				ret = WID_SERVER_IP_ERROR;
			if(wlaninfo->AuthPort > 65535)
				ret = WID_SERVER_PORT_ERROR;
			if(!user_key)
				ret = WID_SECURITY_KEY_ERROR;
			break;
		default:
			break;
	}

	if(check_user_vlanid(vlanId))
	{
		ret =  WID_USER_VLANID_ERROR ;
	}
	
	if(ret)
	{
		syslog(LOG_ERR, "wpa/wep key or vlanid check failed, ret %d!\n", ret);
		return ret;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_MODIFY_NETWORK_CONFIG);
	dbus_error_init(&err);
	
	if(!dbus_message_append_args(query,
							DBUS_TYPE_UINT32,&wlaninfo->WlanId,
							DBUS_TYPE_STRING,&ssid,
							DBUS_TYPE_STRING,&user_key,
							DBUS_TYPE_UINT32,&wlaninfo->securType,
							DBUS_TYPE_UINT32,&wlaninfo->sHideflag,
						 	DBUS_TYPE_UINT32,&wlaninfo->isEnable,
						 	DBUS_TYPE_UINT32,&wlaninfo->applyGflag,
						 	DBUS_TYPE_UINT32,&vlanId,
						 	DBUS_TYPE_UINT32,&wlaninfo->usrGid,
						 	DBUS_TYPE_UINT32,&wlaninfo->EncryptionType,
						 	DBUS_TYPE_UINT32,&wlaninfo->Authip,
						 	DBUS_TYPE_UINT32,&wlaninfo->AuthPort,
							 DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		//printf("<error> failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			//printf("%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		
		syslog(LOG_ERR, "wid return error\n");
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	
	return ret;
}


int hand_load_conf_softAc_network_config
(
    WLANWEBINFO *wlaninfo,
	DBusConnection *dcli_dbus_connection
)
{		
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	char *ssid = NULL;
	char *user_key = NULL;
	int vlanId = 0;
	
	if(!wlaninfo || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "modify ssid is NULL\n");
		return WID_DBUS_ERROR;
	}
	ssid = wlaninfo->ssid;
	user_key = wlaninfo->user_key;
	if(wlaninfo->vlanEnabled && wlaninfo->vlanId > 0)
	{
	    vlanId = wlaninfo->vlanId;
	}
	/*ret = ssid_illegal_character_check(ssid, strlen(ssid));
	if(ret)
	{
		syslog(LOG_ERR, "ssid illegal check failed, ret %d!\n", ret);
		return ret;
	}*/
	
	switch(wlaninfo->securType)
	{
		case WEP:
			ret = wep_key_illegal_check(user_key, ASCII);
			break;
		case WPA_P:
			ret = wpa_key_illegal_check(user_key, ASCII);
			break;
		case WPA_E:
			if(SPE_IP_ADDR(wlaninfo->Authip))
				ret = WID_SERVER_IP_ERROR;
			if(wlaninfo->AuthPort > 65535)
				ret = WID_SERVER_PORT_ERROR;
			if(!user_key)
				ret = WID_SECURITY_KEY_ERROR;
	}
	
	if(check_user_vlanid(vlanId))
	{
		ret =  WID_USER_VLANID_ERROR ;
	}
	
	if(ret)
	{
		syslog(LOG_ERR, "wpa/wep key or vlanid check failed, ret %d!\n", ret);
		return ret;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_LOAD_NETWORK_CONFIG);
	dbus_error_init(&err);
	
	if(!dbus_message_append_args(query,
							DBUS_TYPE_UINT32,&wlaninfo->WlanId,
							DBUS_TYPE_STRING,&ssid,
							DBUS_TYPE_STRING,&user_key,
							DBUS_TYPE_UINT32,&wlaninfo->securType,
							DBUS_TYPE_UINT32,&wlaninfo->sHideflag,
						 	DBUS_TYPE_UINT32,&wlaninfo->isEnable,
						 	DBUS_TYPE_UINT32,&wlaninfo->applyGflag,
						 	DBUS_TYPE_UINT32,&vlanId,
						 	DBUS_TYPE_UINT32,&wlaninfo->usrGid,
						 	DBUS_TYPE_UINT32,&wlaninfo->EncryptionType,
						 	DBUS_TYPE_UINT32,&wlaninfo->Authip,
						 	DBUS_TYPE_UINT32,&wlaninfo->AuthPort,
							 DBUS_TYPE_INVALID))
	{
		printf("send err append %p\n", query);
	}
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		//printf("<error> failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			//printf("%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		
		syslog(LOG_ERR, "wid return error\n");
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	dbus_message_unref(reply);
	
	return ret;
}

int show_network_wlan_config
(	
	WLANWEBINFO *wlanInfo,
	DBusConnection *dcli_dbus_connection
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0, j = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	WLANWEBINFO tmpInfo;
	memset(&tmpInfo, 0, sizeof(WLANWEBINFO));
	char *ssid = tmpInfo.ssid;
	char *user_key = tmpInfo.user_key;

	if (!wlanInfo)
	{
		return WID_DBUS_ERROR;
	}

	memset(&tmpInfo, 0, sizeof(WLANWEBINFO));

	ReInitDbusPath_V2(localid, index, WID_DBUS_BUSNAME, BUSNAME);
	ReInitDbusPath_V2(localid, index, WID_DBUS_OBJPATH, OBJPATH);
	ReInitDbusPath_V2(localid, index, WID_DBUS_INTERFACE, INTERFACE);
	query = dbus_message_new_method_call(BUSNAME, OBJPATH, INTERFACE, WID_DBUS_SHOW_NETWORK_CONFIG);

	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &wlanInfo->WlanId,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	
	dbus_message_iter_get_basic(&iter, &(tmpInfo.WlanId));
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.isExist);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &ssid);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &user_key);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.securType);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.sHideflag);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.isEnable);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.applyGflag);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.vlanId);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.usrGid);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.EncryptionType);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.Authip);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.AuthPort);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.uplink_bandwidth);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.downlink_bandwidth);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.tx_packets);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.rx_packets);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.tx_bytes);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.rx_bytes);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &tmpInfo.wlan_wtp_wifi.num);
	dbus_message_iter_next(&iter);
	
	for (i = 0; i < tmpInfo.wlan_wtp_wifi.num; i++)
	{
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].wtpid));
		dbus_message_iter_next(&iter);
		
		unsigned char *wtpname = NULL;
		dbus_message_iter_get_basic(&iter, &(wtpname));
		dbus_message_iter_next(&iter);
		
		memset(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].wtpname, 0, WTP_NAME_LEN);
		if (wtpname)
		{
			if (strlen(wtpname) < WTP_NAME_LEN)
			{
				memcpy(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].wtpname, wtpname, strlen(wtpname));
			}
			else
			{
				memcpy(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].wtpname, wtpname, WTP_NAME_LEN-1);
			}
		}
		
		for (j = 0; j < Sta_Type_Num; j++)
		{
			dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].sta_type_num[j]));
			dbus_message_iter_next(&iter);
		}
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].sta_num));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].sta_auth_per));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].sta_abn_deauth_per));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].uplink_bandwidth));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].downlink_bandwidth));
		dbus_message_iter_next(&iter);

		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].tx_packets));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].rx_packets));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].tx_bytes));
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &(tmpInfo.wlan_wtp_wifi.wtp_wifi[i].rx_bytes));
		dbus_message_iter_next(&iter);
	}
	
	dbus_message_unref(reply);
	memcpy(wlanInfo, &tmpInfo, sizeof(WLANWEBINFO));
	
	if (ssid)
	{
		memcpy(wlanInfo->ssid, ssid, ESSID_DEFAULT_LEN);
	}
	if (user_key)
	{
		memcpy(wlanInfo->user_key, user_key, USER_KEY_LEN);
	}

	return ret;
}

int show_network_sta_config
(	
	STA_TO_WEB *stawebInfo,
	DBusConnection *dcli_dbus_connection
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusError err;
	int i = 0;
	int ret = 0;
	unsigned char wlanid = 0;
	unsigned int g_radioid = 0;
	char *essid = NULL;
	char *ip_addr = NULL;
	char *sta_name = NULL;
	char *ostype = NULL;
	char *pd_model = NULL;
	char *hostname = NULL;
	char *cpu_model = NULL;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;

	if (!stawebInfo)
	{
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_SHOW_NETWORK_STA_CONFIG);

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	
	dbus_message_iter_get_basic(&iter, &stawebInfo->sta_num);
    dbus_message_iter_next(&iter);
    
	if (stawebInfo->sta_num)
	{
		stawebInfo->stainfo = (struct stainfo_to_web *)malloc((stawebInfo->sta_num) * sizeof(struct stainfo_to_web));
	       
		for (i = 0; i < stawebInfo->sta_num; i++)
		{		
			dbus_message_iter_get_basic(&iter,&g_radioid);
			dbus_message_iter_next(&iter);

			stawebInfo->stainfo[i].wtpid = g_radioid/L_RADIO_NUM;
			stawebInfo->stainfo[i].l_radioid = g_radioid%L_RADIO_NUM;
		    
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].wlanid));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&essid);
			dbus_message_iter_next(&iter);

			memset(stawebInfo->stainfo[i].essid, 0, ESSID_DEFAULT_LEN);
			if (essid)
			{
				memcpy(stawebInfo->stainfo[i].essid, essid, strlen(essid));
			}

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].add_time));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].sta_online_time));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&ip_addr);
			dbus_message_iter_next(&iter);
			
			memset(stawebInfo->stainfo[i].in_addr, 0, STA_INFO_INADDR_LEN);
			if (ip_addr)
			{
				memcpy(stawebInfo->stainfo[i].in_addr, ip_addr, strlen(ip_addr));
			}
			
			dbus_message_iter_get_basic(&iter,&sta_name);
			dbus_message_iter_next(&iter);
			
			memset(stawebInfo->stainfo[i].sta_name, 0, STA_INFO_NAME_LEN);
			if (sta_name)
			{
				memcpy(stawebInfo->stainfo[i].sta_name, sta_name, strlen(sta_name));
			}
			
			dbus_message_iter_get_basic(&iter,&ostype);
			dbus_message_iter_next(&iter);
			
			memset(stawebInfo->stainfo[i].ostype, 0, DEFAULT_LEN);
			if (ostype)
			{
				memcpy(stawebInfo->stainfo[i].ostype, ostype, strlen(ostype));
			}
			
			dbus_message_iter_get_basic(&iter,&pd_model);
			dbus_message_iter_next(&iter);
			
			memset(stawebInfo->stainfo[i].pd_model, 0, DEFAULT_LEN);
			if (pd_model)
			{
				memcpy(stawebInfo->stainfo[i].pd_model, pd_model, strlen(pd_model));
			}
			
			dbus_message_iter_get_basic(&iter,&hostname);
			dbus_message_iter_next(&iter);
			
			memset(stawebInfo->stainfo[i].hostname, 0, DEFAULT_LEN);
			if (hostname)
			{
				memcpy(stawebInfo->stainfo[i].hostname, hostname, strlen(hostname));
			}
			
			dbus_message_iter_get_basic(&iter,&cpu_model);
			dbus_message_iter_next(&iter);
			
			memset(stawebInfo->stainfo[i].cpu_model, 0, DEFAULT_LEN);
			if (cpu_model)
			{
				memcpy(stawebInfo->stainfo[i].cpu_model, cpu_model, strlen(cpu_model));
			}

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].addr[0]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].addr[1]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].addr[2]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].addr[3]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].addr[4]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].addr[5]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].bssid[0]);
			dbus_message_iter_next(&iter);  
            
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].bssid[1]);
			dbus_message_iter_next(&iter);  

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].bssid[2]);
			dbus_message_iter_next(&iter);  

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].bssid[3]);
			dbus_message_iter_next(&iter);  

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].bssid[4]);
			dbus_message_iter_next(&iter);  

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].bssid[5]);
			dbus_message_iter_next(&iter);  

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].wtpmac[0]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].wtpmac[1]);
			dbus_message_iter_next(&iter);
            
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].wtpmac[2]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].wtpmac[3]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].wtpmac[4]);
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].wtpmac[5]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].roam_ap[0]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].roam_ap[1]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].roam_ap[2]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].roam_ap[3]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].roam_ap[4]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&stawebInfo->stainfo[i].roam_ap[5]);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].sta_type));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].dev_type));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].portal_status));
			dbus_message_iter_next(&iter);
            		
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].mode));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].channel));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].rssi));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].rx_retry_per));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].tx_retry_per));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].pkt_loss_per));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].roam_type));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].sta_delay));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].nRate));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].txRate));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].rr));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].tr));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].rxbytes));
			dbus_message_iter_next(&iter);

			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].txbytes));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].rxpackets));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&(stawebInfo->stainfo[i].txpackets));
			dbus_message_iter_next(&iter);
		}
	}

	dbus_message_unref(reply);
	
	return ret;
}


int show_network_wtp_ip_list
(	
	WID_WTP *wtpInfo,
	DBusConnection *dcli_dbus_connection
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	WID_WTP tmpWtpInfo;

	if(!wtpInfo)
	{
		return WID_DBUS_ERROR;
	}

	memset(&tmpWtpInfo, 0, sizeof(WLANWEBINFO));

	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_SHOW_NETWORK_WTP_IP_LIST);

	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &wtpInfo->WTPID,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	if (NULL == reply) {
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err)) {
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, &tmpWtpInfo.isused);
	dbus_message_iter_next(&iter);
	if(tmpWtpInfo.isused)
	{
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTPIP);
		dbus_message_iter_next(&iter);
	}
	dbus_message_unref(reply);
	
	memcpy(wtpInfo, &tmpWtpInfo, sizeof(WID_WTP));

	//dbus_message_unref(reply);

	return ret;
}


int show_network_wtp_config
(	
	WID_WTP_TO_WEB *wtpwebInfo,
	DBusConnection *dcli_dbus_connection
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int j = 0;
	int k = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	WID_WTP_TO_WEB tmpWtpInfo;
	int haswtp = 0;

	if (!wtpwebInfo)
	{
		return WID_DBUS_ERROR;
	}

	memset(&tmpWtpInfo, 0, sizeof(WID_WTP_TO_WEB));

	ReInitDbusPath_V2(localid, index, WID_DBUS_BUSNAME, BUSNAME);
	ReInitDbusPath_V2(localid, index, WID_DBUS_OBJPATH, OBJPATH);
	ReInitDbusPath_V2(localid, index, WID_DBUS_INTERFACE, INTERFACE);
	query = dbus_message_new_method_call(BUSNAME, OBJPATH, INTERFACE, WID_DBUS_SHOW_NETWORK_WTP_CONFIG);

	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &wtpwebInfo->WTPID,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, &haswtp);
	dbus_message_iter_next(&iter);
	if (haswtp)
	{
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.isused);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTPIP);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTPNAME);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTPMAC);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTPModel);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTPStat);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.quitreason);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.updateStat);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.cpu_per);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.mem_per);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.flash_per);
		dbus_message_iter_next(&iter);
		
		memset(&tmpWtpInfo.apdelayinfo, 0, sizeof(ap_delay_info));
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.apdelayinfo.delay_num);
		dbus_message_iter_next(&iter);
		
		for (j = 0; j < tmpWtpInfo.apdelayinfo.delay_num; j++)
		{
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.apdelayinfo.ap_average_delay[j].delay_type);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.apdelayinfo.ap_average_delay[j].pkt_loss);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.apdelayinfo.ap_average_delay[j].delay_time);
			dbus_message_iter_next(&iter);
		}
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.uplink_bandwidth);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.downlink_bandwidth);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.tx_packets);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.rx_packets);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.tx_bytes);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.rx_bytes);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_accessed_sta_num);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.codever);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.add_time);
		dbus_message_iter_next(&iter);
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wlans[i].WlanId);
		dbus_message_iter_next(&iter);
		
		while (i < MAX_WLANID && tmpWtpInfo.wlans[i].WlanId)
		{
			unsigned char *essid = NULL;
			dbus_message_iter_get_basic(&iter, &essid);
			dbus_message_iter_next(&iter);
			strncpy(tmpWtpInfo.wlans[i].ssid, essid, ESSID_DEFAULT_LEN-1);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wlans[i].isEnable);
			dbus_message_iter_next(&iter);
			tmpWtpInfo.wlans[i].isEnable = !(tmpWtpInfo.wlans[i].isEnable);
			
			for (k = 0; k < Sta_Type_Num; k++)
			{
				dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].sta_type_num[k]);
				dbus_message_iter_next(&iter);
			}
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].sta_num);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].sta_auth_per);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].sta_abn_deauth_per);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].uplink_bandwidth);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].downlink_bandwidth);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].tx_packets);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].rx_packets);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].tx_bytes);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wtp_wlan_wifi.wlan_wifi[i].rx_bytes);
			dbus_message_iter_next(&iter);
			
			i++;
			if (i < MAX_WLANID)
			{
				dbus_message_iter_get_basic(&iter, &tmpWtpInfo.wlans[i].WlanId);
				dbus_message_iter_next(&iter);
			}
		}
		
		dbus_message_iter_get_basic(&iter, &tmpWtpInfo.RadioCount);
		dbus_message_iter_next(&iter);
		
		memset(tmpWtpInfo.WTP_Radio, 0, sizeof(WID_WTP_RADIO)*L_RADIO_NUM);
		for (i = 0; i < tmpWtpInfo.RadioCount; i++)
		{
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].Radio_L_ID);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].Radio_Type);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].Radio_Chan);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].chan_per);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].wifi_snr);
			dbus_message_iter_next(&iter);
			
			memset(&tmpWtpInfo.WTP_Radio[i].disturbinfo, 0, sizeof(radio_disturb_info));
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].disturbinfo.disturb_num);
			dbus_message_iter_next(&iter);
			
			for (k = 0; k < tmpWtpInfo.WTP_Radio[i].disturbinfo.disturb_num; k++)
			{
				dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].disturbinfo.radio_disturb[k].disturb_type);
				dbus_message_iter_next(&iter);
				
				dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].disturbinfo.radio_disturb[k].disturb_rssi);
				dbus_message_iter_next(&iter);
			}
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].Radio_TXP);
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter, &tmpWtpInfo.WTP_Radio[i].cwmode);
			dbus_message_iter_next(&iter);
		}
	}
#if 0
	if(dbus_message_get_args(reply, &err,
							DBUS_TYPE_UINT32,&tmpWtpInfo.isused,
							DBUS_TYPE_STRING,&tmpWtpInfo.WTPIP,
							DBUS_TYPE_STRING,&tmpWtpInfo.WTPNAME,
							DBUS_TYPE_STRING,&tmpWtpInfo.WTPMAC,
							DBUS_TYPE_STRING,&tmpWtpInfo.WTPModel,
							DBUS_TYPE_BYTE,&tmpWtpInfo.WTPStat,
							DBUS_TYPE_UINT32, &tmpWtpInfo.wtp_accessed_sta_num,
							DBUS_TYPE_INVALID))
	{
       ret = 0;
	} 
	else {		
		if (dbus_error_is_set(&err)) {
			printf("%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
	}
#endif
	dbus_message_unref(reply);
	
	memcpy(wtpwebInfo, &tmpWtpInfo, sizeof(WID_WTP_TO_WEB));

	return ret;
}


int show_network_ac_access_wtplist
(	
	WID_ACCESS *wtpaccInfo,
	DBusConnection *dcli_dbus_connection
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	int i = 0;
	int ret = 0;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;

	if (!wtpaccInfo)
	{
		return WID_DBUS_ERROR;
	}

	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_AC_ACCESS_WTPLIST);

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);

	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}

		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	
	dbus_message_iter_get_basic(&iter, &ret);
	dbus_message_iter_next(&iter);
	
	dbus_message_iter_get_basic(&iter, &(wtpaccInfo->num));
	dbus_message_iter_next(&iter);
	
	if (wtpaccInfo->num)
	{
		wtpaccInfo->wtp_list = (struct wtp_access_info *)malloc((wtpaccInfo->num) * sizeof(struct wtp_access_info));

		for (i = 0; i < wtpaccInfo->num; i++)
		{
			unsigned char *wtpname = NULL;
			unsigned char *wtpmac = NULL;
			unsigned char *wtpmodel = NULL;
			unsigned char *wtpcode = NULL;
			unsigned char *wtpsn = NULL;
			unsigned char *ifname = NULL;
			unsigned char *version = NULL;
			unsigned char *codever = NULL;
			
			dbus_message_iter_get_basic(&iter,&(wtpaccInfo->wtp_list[i].ip));
			dbus_message_iter_next(&iter);
			
			dbus_message_iter_get_basic(&iter,&wtpname);
			dbus_message_iter_next(&iter);
			
			memset(wtpaccInfo->wtp_list[i].WTPNAME, 0, WTP_NAME_LEN);
			memcpy(wtpaccInfo->wtp_list[i].WTPNAME, wtpname, strlen(wtpname));
			
			dbus_message_iter_get_basic(&iter,&wtpmac);
			dbus_message_iter_next(&iter);
			
			wtpaccInfo->wtp_list[i].WTPMAC = (unsigned char *)malloc(strlen(wtpmac)+1);
			memset(wtpaccInfo->wtp_list[i].WTPMAC, 0, strlen(wtpmac)+1);
			memcpy(wtpaccInfo->wtp_list[i].WTPMAC, wtpmac, strlen(wtpmac));
			
			dbus_message_iter_get_basic(&iter,&wtpmodel);
			dbus_message_iter_next(&iter);
			
			wtpaccInfo->wtp_list[i].model = (char*)malloc(strlen(wtpmodel)+1);
			memset(wtpaccInfo->wtp_list[i].model, 0, strlen(wtpmodel)+1);
			memcpy(wtpaccInfo->wtp_list[i].model, wtpmodel, strlen(wtpmodel));
			
			dbus_message_iter_get_basic(&iter,&wtpcode);
			dbus_message_iter_next(&iter);
			
			wtpaccInfo->wtp_list[i].apcode = (char*)malloc(strlen(wtpcode)+1);
			memset(wtpaccInfo->wtp_list[i].apcode, 0, strlen(wtpcode)+1);
			memcpy(wtpaccInfo->wtp_list[i].apcode, wtpcode, strlen(wtpcode));
			
			dbus_message_iter_get_basic(&iter,&wtpsn);
			dbus_message_iter_next(&iter);
			
			wtpaccInfo->wtp_list[i].sn = (char*)malloc(strlen(wtpsn)+1);
			memset(wtpaccInfo->wtp_list[i].sn, 0, strlen(wtpsn)+1);
			memcpy(wtpaccInfo->wtp_list[i].sn, wtpsn,strlen(wtpsn));
			
			dbus_message_iter_get_basic(&iter,&ifname);
			dbus_message_iter_next(&iter);
			
			wtpaccInfo->wtp_list[i].ifname = (char*)malloc(strlen(ifname)+1);
			memset(wtpaccInfo->wtp_list[i].ifname, 0, strlen(ifname)+1);
			memcpy(wtpaccInfo->wtp_list[i].ifname, ifname, strlen(ifname));
			
			dbus_message_iter_get_basic(&iter,&version);
			dbus_message_iter_next(&iter);
			
			wtpaccInfo->wtp_list[i].version = (char*)malloc(strlen(version)+1);
			memset(wtpaccInfo->wtp_list[i].version, 0, strlen(version)+1);
			memcpy(wtpaccInfo->wtp_list[i].version, version, strlen(version));
			
			dbus_message_iter_get_basic(&iter,&codever);
			dbus_message_iter_next(&iter);  
			
			wtpaccInfo->wtp_list[i].codever = (char*)malloc(strlen(codever)+1);
			memset(wtpaccInfo->wtp_list[i].codever, 0, strlen(codever)+1);
			memcpy(wtpaccInfo->wtp_list[i].codever, codever, strlen(codever));
		}
	}
	
	dbus_message_unref(reply);
	
	return ret;
}

void web_free_fun_access_wtp(WID_ACCESS *WTP_ACC_INFO)
{
	int i = 0;
	if (WTP_ACC_INFO)
	{
		if (WTP_ACC_INFO->wtp_list)
		{
			for (i = 0; i < WTP_ACC_INFO->num; i++)
			{
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].WTPMAC);
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].model);
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].apcode);
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].sn);
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].version);
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].codever);
				CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list[i].ifname);
			}
		}
		CW_FREE_OBJECT(WTP_ACC_INFO->wtp_list);    
	}
}

int handlib_restart_wtp_by_id(int wtpid, DBusConnection *dcli_dbus_connection)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	
	int ret = 0;
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_RESTART_WTP_BY_ID);

	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &wtpid,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	
	if(0 != ret)
	{
		syslog(LOG_ERR, "%s line %d restart wtp %d failed, ret %d ", __func__, __LINE__, wtpid, ret);
	}
	return ret;
}

int handlib_delete_wtp_by_id(int wtpid, DBusConnection *dcli_dbus_connection)
{	
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;
	
	if(!dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p ", 
			__func__, __LINE__, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_DEL_WTP);

	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &wtpid,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	dbus_message_unref(reply);
	
	if(0 != ret)
	{
		syslog(LOG_ERR, "%s line %d delete wtp %d failed, ret %d ", __func__, __LINE__, wtpid, ret);
	}
	
	return ret;
}


int handlib_create_new_wtp(NEW_WTP_ARG *new_wtp, DBusConnection *dcli_dbus_connection)
{	
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter  iter;	
	DBusError err;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;
	
	if(!new_wtp || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_wtp %p dbus_connection %p ", 
            			__func__, __LINE__, new_wtp, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_ADD_WTP);

	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							 DBUS_TYPE_STRING, &(new_wtp->WTPNAME),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[0]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[1]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[2]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[3]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[4]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[5]),
							 DBUS_TYPE_STRING, &(new_wtp->WTPModel),
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	    
	dbus_message_iter_next(&iter);
	dbus_message_iter_get_basic(&iter,&(new_wtp->WTPID));
	dbus_message_iter_next(&iter);
	dbus_message_iter_get_basic(&iter,&(new_wtp->WTPNAME));
	dbus_message_iter_next(&iter);
	dbus_message_iter_get_basic(&iter,&(new_wtp->WTPModel));

	dbus_message_unref(reply);
	
 	if(0 != ret)
	{
		syslog(LOG_ERR, "%s line %d create wtp[%02x:%02x:%02x:%02x:%02x:%02x] name %s model %s failed, ret %d ", 
            			__func__, __LINE__, MAC2STR(new_wtp->WTPMAC), new_wtp->WTPNAME, new_wtp->WTPModel, ret);
	}
	
	return ret;
}


int handlib_set_wtp_name(NEW_WTP_ARG *new_wtp, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter  iter;
    DBusMessage *query = NULL, *reply = NULL; 
    int localid = LOCALID;
    int index = LOCALINDEX;    
    int ret = 0;

    char BUSNAME[PATH_LEN]= {0};
    char OBJPATH[PATH_LEN] = {0};
    char INTERFACE[PATH_LEN] = {0}; 

    if(!new_wtp || !dcli_dbus_connection)
    {
        syslog(LOG_ERR, "%s line %d bad input argument new_wtp %p dbus_connection %p ", 
						__func__, __LINE__, new_wtp, dcli_dbus_connection);
        return WID_DBUS_ERROR;
    }
    
    ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
    ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
    ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);

    query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WTP_METHOD_SET_WTPNAME);

    dbus_error_init(&err);

    dbus_message_append_args(query,
                            DBUS_TYPE_UINT32, &(new_wtp->WTPID),
                            DBUS_TYPE_STRING, &(new_wtp->WTPNAME),
                            DBUS_TYPE_INVALID);

    reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

    dbus_message_unref(query);
    
    if (NULL == reply)
    {
        if (dbus_error_is_set(&err))
        {
            dbus_error_free(&err);
        }
        return WID_DBUS_ERROR;
    }
    
    dbus_message_iter_init(reply,&iter);
    dbus_message_iter_get_basic(&iter,&ret);
     
    dbus_message_unref(reply);

    return ret;
}


int handlib_dynamic_select_radio_channel(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!new_radio || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_radio %p dbus_connection %p ",
						__func__, __LINE__, new_radio, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_RADIO_METHOD_DYNAMIC_SELECT_CHAN);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_BYTE, &(new_radio->RADIOMAC[0]),
							DBUS_TYPE_BYTE, &(new_radio->RADIOMAC[1]),
							DBUS_TYPE_BYTE, &(new_radio->RADIOMAC[2]),
							DBUS_TYPE_BYTE, &(new_radio->RADIOMAC[3]),
							DBUS_TYPE_BYTE, &(new_radio->RADIOMAC[4]),
							DBUS_TYPE_BYTE, &(new_radio->RADIOMAC[5]),
							DBUS_TYPE_BYTE, &(new_radio->Radio_Chan),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	dbus_message_unref(reply);
	
	return ret;
}


int handlib_set_radio_channel(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!new_radio || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_radio %p dbus_connection %p ",
						__func__, __LINE__, new_radio, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_RADIO_METHOD_SET_CHAN);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &(new_radio->Radio_G_ID),
							DBUS_TYPE_BYTE, &(new_radio->Radio_Chan),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	dbus_message_unref(reply);
	
	return ret;
}


int handlib_set_radio_txpower(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!new_radio || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_radio %p dbus_connection %p\n",
						__func__, __LINE__, new_radio, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_RADIO_METHOD_SET_TXP);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &(new_radio->Radio_G_ID),
							DBUS_TYPE_UINT16, &(new_radio->Radio_TXP),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	dbus_message_unref(reply);
	
	return ret;
}

int handlib_set_radio_cwmode(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;	
	unsigned int max_channel = 0;
	unsigned int min_channel = 0;
	unsigned char current_channel = 0;	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!new_radio || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_radio %p dbus_connection %p\n",
						__func__, __LINE__, new_radio, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_RADIO_METHOD_SET_CMMODE);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &(new_radio->Radio_G_ID),
							DBUS_TYPE_UINT16, &(new_radio->cwmode),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);

	if(ret == CHANNEL_CROSS_THE_BORDER)
	{
		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter,&max_channel);
		dbus_message_iter_next(&iter);	    
		dbus_message_iter_get_basic(&iter,&min_channel);
		dbus_message_iter_next(&iter);	    
		dbus_message_iter_get_basic(&iter,&current_channel);		 
	}
	
	dbus_message_unref(reply);
	
	return ret;
}


int handlib_set_radio_mode(NEW_RADIO_ARG *new_radio, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!new_radio || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_radio %p dbus_connection %p\n",
						__func__, __LINE__, new_radio, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_RADIO_METHOD_SET_MODE);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &(new_radio->Radio_G_ID),
							DBUS_TYPE_UINT32, &(new_radio->Radio_Type),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);	
	
	dbus_message_unref(reply);
	
	return ret;
}

int handlib_set_sta_name(struct stainfo_to_web *stainfo, DBusConnection *dcli_dbus_connection)
{
	DBusError err;
	DBusMessageIter  iter;
	DBusMessage *query = NULL, *reply = NULL; 
	int localid = LOCALID;
	int index = LOCALINDEX;    
	int ret = 0;
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0}; 
	
	if(!stainfo || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument stainfo %p dbus_connection %p ", 
						__func__, __LINE__, stainfo, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WTP_METHOD_SET_STANAME);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_STRING, &(stainfo->sta_name),
							DBUS_TYPE_UINT32, &(stainfo->wtpid),
							DBUS_TYPE_BYTE, &(stainfo->l_radioid),
							DBUS_TYPE_BYTE, &(stainfo->wlanid),
							DBUS_TYPE_BYTE, &(stainfo->addr[0]),
							DBUS_TYPE_BYTE, &(stainfo->addr[1]),
							DBUS_TYPE_BYTE, &(stainfo->addr[2]),
							DBUS_TYPE_BYTE, &(stainfo->addr[3]),
							DBUS_TYPE_BYTE, &(stainfo->addr[4]),
							DBUS_TYPE_BYTE, &(stainfo->addr[5]),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	dbus_message_unref(reply);
	
	return ret;
}

int handlib_load_conf_new_wtp(NEW_WTP_ARG *new_wtp, DBusConnection *dcli_dbus_connection)
{	
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusError err;
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	int localid = LOCALID;
	int index = LOCALINDEX;
	int ret = 0;
	
	if(!new_wtp || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument new_wtp %p dbus_connection %p ", 
						__func__, __LINE__, new_wtp, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_LOAD_WTP);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &(new_wtp->WTPID),
							 DBUS_TYPE_STRING, &(new_wtp->WTPNAME),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[0]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[1]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[2]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[3]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[4]),
							 DBUS_TYPE_BYTE, &(new_wtp->WTPMAC[5]),
							 DBUS_TYPE_STRING, &(new_wtp->WTPModel),
							 DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		syslog(LOG_ERR,"failed get reply.\n");
		if (dbus_error_is_set(&err))
		{
			syslog(LOG_ERR,"%s raised: %s",err.name,err.message);
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	dbus_message_unref(reply);
	
 	if(0 != ret)
	{
		syslog(LOG_ERR, "%s line %d create wtp%d["MACSTR"] name %s model %s failed, ret %d\n", 
						__func__, __LINE__, new_wtp->WTPID,MAC2STR(new_wtp->WTPMAC),new_wtp->WTPNAME,new_wtp->WTPModel,ret);
	}
	
	return ret;
}

int hand_archive_alertinfo_by_activeid_apmac(unsigned char *wtpmac, int alertid, DBusConnection *dcli_dbus_connection)
{
    int ret = 0;
    int localid = LOCALID;
    int index = LOCALINDEX;    
    DBusError err;
    DBusMessageIter  iter;
    DBusMessage *query = NULL, *reply = NULL; 

    char BUSNAME[PATH_LEN]= {0};
    char OBJPATH[PATH_LEN] = {0};
    char INTERFACE[PATH_LEN] = {0}; 

    if(!dcli_dbus_connection || !wtpmac)
    {
        syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p wtpmac %p", 
                        __func__, __LINE__, dcli_dbus_connection, wtpmac);
        return WID_DBUS_ERROR;
    }
    
    ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
    ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
    ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);

    query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WTP_METHOD_ARCHIVE_ALERT);

    dbus_error_init(&err);

    dbus_message_append_args(query,
                            DBUS_TYPE_UINT32, &(alertid),
                            DBUS_TYPE_BYTE, &(wtpmac[0]),
                            DBUS_TYPE_BYTE, &(wtpmac[1]),
                            DBUS_TYPE_BYTE, &(wtpmac[2]),
                            DBUS_TYPE_BYTE, &(wtpmac[3]),
                            DBUS_TYPE_BYTE, &(wtpmac[4]),
                            DBUS_TYPE_BYTE, &(wtpmac[5]),
                            DBUS_TYPE_INVALID);

    reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

    dbus_message_unref(query);
    
    if (NULL == reply)
    {
        if (dbus_error_is_set(&err))
        {
            dbus_error_free(&err);
        }
        return WID_DBUS_ERROR;
    }
    
    dbus_message_iter_init(reply,&iter);
    dbus_message_iter_get_basic(&iter,&ret);
     
    dbus_message_unref(reply);

    return ret;
}


int hand_get_ap_alert_ids(struct alert_info *alert_list, DBusConnection *dcli_dbus_connection)
{
    int i = 0;
    int ret = 0;
    int localid = LOCALID;
    int index = LOCALINDEX;  
    int alert_num = 0;
    DBusError err;
    DBusMessageIter  iter;
    DBusMessage *query = NULL, *reply = NULL; 

    char BUSNAME[PATH_LEN]= {0};
    char OBJPATH[PATH_LEN] = {0};
    char INTERFACE[PATH_LEN] = {0}; 

    if(!dcli_dbus_connection)
    {
        syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p ", 
                        __func__, __LINE__, dcli_dbus_connection);
        return WID_DBUS_ERROR;
    }
    
    ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
    ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
    ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);

    query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WTP_METHOD_GET_WTP_ALERTIDS);

    dbus_error_init(&err);

    reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

    dbus_message_unref(query);
    
    if (NULL == reply)
    {
        if (dbus_error_is_set(&err))
        {
            dbus_error_free(&err);
        }
        return WID_DBUS_ERROR;
    }
    
    dbus_message_iter_init(reply,&iter);
    dbus_message_iter_get_basic(&iter,&ret);
    
	dbus_message_iter_next(&iter);    
    dbus_message_iter_get_basic(&iter,&alert_num);

    if(alert_num > 0)
    {
        for(i = 0; i < alert_num; i++)
        {
            dbus_message_iter_next(&iter);    
            dbus_message_iter_get_basic(&iter,&alert_list[i].active_id);

            dbus_message_iter_next(&iter);    
            dbus_message_iter_get_basic(&iter,&alert_list[i].archived);
        }
    }
    
    dbus_message_unref(reply);
	
	if(alert_num > 0)
	{
		return WID_DBUS_SUCCESS;
	}
    return -WID_DBUS_ERROR;
}


int hand_archive_all_alert(DBusConnection *dcli_dbus_connection)
{
    int i = 0;
    int ret = 0;
    int localid = LOCALID;
    int index = LOCALINDEX;  
    DBusError err;
    DBusMessageIter  iter;
    DBusMessage *query = NULL, *reply = NULL; 

    char BUSNAME[PATH_LEN]= {0};
    char OBJPATH[PATH_LEN] = {0};
    char INTERFACE[PATH_LEN] = {0}; 

    if(!dcli_dbus_connection)
    {
        syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p ", 
                        __func__, __LINE__, dcli_dbus_connection);
        return WID_DBUS_ERROR;
    }
    
    ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
    ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
    ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);

    query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WTP_METHOD_ARCHIVE_ALERTLIST);

    dbus_error_init(&err);

    reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);

    dbus_message_unref(query);
    
    if (NULL == reply)
    {
        if (dbus_error_is_set(&err))
        {
            dbus_error_free(&err);
        }
        return WID_DBUS_ERROR;
    }
    
    dbus_message_iter_init(reply,&iter);
    dbus_message_iter_get_basic(&iter,&ret);
    
    dbus_message_unref(reply);

    return ret;
}


int record_update_status_by_apmac(unsigned char *wtpmac, int upstat, DBusConnection *dcli_dbus_connection)
{
	int ret = 0;
	int localid = LOCALID;
	int index = LOCALINDEX;    
	DBusError err;
	DBusMessageIter  iter;
	DBusMessage *query = NULL, *reply = NULL; 

	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0}; 

	if(!dcli_dbus_connection || !wtpmac)
	{
		syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p wtpmac %p", 
						__func__, __LINE__, dcli_dbus_connection, wtpmac);
		return WID_DBUS_ERROR;
	}
    
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WTP_METHOD_RECORD_AP_UPDATE_STATUS);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &(upstat),
							DBUS_TYPE_BYTE, &(wtpmac[0]),
							DBUS_TYPE_BYTE, &(wtpmac[1]),
							DBUS_TYPE_BYTE, &(wtpmac[2]),
							DBUS_TYPE_BYTE, &(wtpmac[3]),
							DBUS_TYPE_BYTE, &(wtpmac[4]),
							DBUS_TYPE_BYTE, &(wtpmac[5]),
							DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	dbus_message_unref(reply);
	
	return ret;
}

static int count_link_nums(NEIGHBOR_LIST_HANDLE phead);
static NEIGHBOR_LIST_HANDLE create_node(NEIGHBOR_LIST_OBJ node);
static void create_neighbors_list(NEIGHBOR_LIST_HANDLE *pneighbor_head, NEIGHBOR_LIST_OBJ node);

void *get_neighbors_aps_info(DBusConnection *dbus_connection, int *ap_nums)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusMessageIter iter_array;
	DBusMessageIter iter_sub_array;
	DBusError err;
#define WMD_DBUS_BUSNAME	"aw.wmd"
#define WMD_DBUS_OBJPATH	"/aw/wmd"
#define WMD_DBUS_INTERFACE	"aw.wmd"
#define WMD_DBUS_CONF_METHOD_GET_APS_INFO_STR "get_aps_list_info"	
	int localid = LOCALID;
	int index = LOCALINDEX;	
#if 0
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	ReInitDbusPath_V2(localid,index,WMD_DBUS_BUSNAME, BUSNAME);
	ReInitDbusPath_V2(localid,index,WMD_DBUS_OBJPATH, OBJPATH);
	ReInitDbusPath_V2(localid,index,WMD_DBUS_INTERFACE, INTERFACE);
#endif
	CLIENT_INFO_HANDLE client_ap_info = NULL;
	NEIGHBOR_LIST_HANDLE pnew = NULL;
	NEIGHBOR_LIST_OBJ neighbor_info;
	int i = 0, j = 0, k = 0;
	int neighbor_cnt = 0;
	int need_cnt = 0;    //need neighbors' count
	
	query = dbus_message_new_method_call(WMD_DBUS_BUSNAME, WMD_DBUS_OBJPATH, WMD_DBUS_INTERFACE, WMD_DBUS_CONF_METHOD_GET_APS_INFO_STR);
	
	dbus_error_init(&err);
	
	reply = dbus_connection_send_with_reply_and_block (dbus_connection, query, DCLI_WID_DBUS_TIMEOUT, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		syslog(LOG_ERR, "%s line %d dbus_connection_send_with_reply_and_block", __func__, __LINE__);
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return NULL;
	}
	
	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, ap_nums);
	dbus_message_iter_next(&iter);
	syslog(LOG_ERR, "%s line %d ap_nums = %d!!!!!!", __func__, __LINE__, *ap_nums);
	
	client_ap_info = (CLIENT_INFO_HANDLE)malloc(sizeof(CLIENT_INFO_OBJ) * (*ap_nums));
	if (NULL == client_ap_info)
	{
		syslog(LOG_ERR, "%s line %d malloc error",	__func__, __LINE__);
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return NULL;
	}
	memset(client_ap_info, 0, sizeof(CLIENT_INFO_OBJ) * (*ap_nums));
	
	dbus_message_iter_recurse(&iter, &iter_array);
	
	for (i = 0; i < *ap_nums; i++)
	{
		DBusMessageIter iter_struct;
		dbus_message_iter_recurse(&iter_array, &iter_struct);
		for (j = 0; j < MAC_LEN; j++)	//AC's MAC
		{
			dbus_message_iter_get_basic(&iter_struct, &(client_ap_info[i].ap_mac[j]));
			dbus_message_iter_next(&iter_struct);
		}
		dbus_message_iter_get_basic(&iter_struct, &(client_ap_info[i].type));
		dbus_message_iter_next(&iter_struct);
		dbus_message_iter_get_basic(&iter_struct, &(client_ap_info[i].channel));
		dbus_message_iter_next(&iter_struct);
		dbus_message_iter_get_basic(&iter_struct, &(neighbor_cnt));
		dbus_message_iter_next(&iter_struct);
		client_ap_info[i].neighbor_cnt = neighbor_cnt;
		dbus_message_iter_recurse(&iter_struct, &iter_sub_array);
		
		client_ap_info[i].neighbor_link_head = NULL;
		for (k = 0; k < client_ap_info[i].neighbor_cnt; k++)
		{
			DBusMessageIter iter_sub_struct;
			dbus_message_iter_recurse(&iter_sub_array, &iter_sub_struct);
			
			for (j = 0; j < MAC_LEN; j++)
			{
				dbus_message_iter_get_basic(&iter_sub_struct, &(neighbor_info.mac[j]));
				dbus_message_iter_next(&iter_sub_struct);
			}
			dbus_message_iter_get_basic(&iter_sub_struct, &(neighbor_info.neighbor_flag));
			dbus_message_iter_next(&iter_sub_struct);
			dbus_message_iter_get_basic(&iter_sub_struct, &(neighbor_info.channel));
			dbus_message_iter_next(&iter_sub_struct);
			dbus_message_iter_get_basic(&iter_sub_struct, &(neighbor_info.rssi));
			dbus_message_iter_next(&iter_sub_struct);
			dbus_message_iter_next(&iter_sub_array);
			
			char tmpMacStr[32];
			memset(tmpMacStr, 0, sizeof(tmpMacStr));
			snprintf(tmpMacStr, 31, MACSTR, MAC2STR(neighbor_info.mac));
			syslog(LOG_ERR, "NO.%d: Get from dbus MAC: %s   RSSI: %d\n", k+1, tmpMacStr, neighbor_info.rssi);
			create_neighbors_list(&(client_ap_info[i].neighbor_link_head), neighbor_info);
		}
		client_ap_info[i].neighbor_cnt = count_link_nums(client_ap_info[i].neighbor_link_head);
		dbus_message_iter_next(&iter_array);
	}
	
	dbus_message_unref(reply);
	
	return client_ap_info;
}

//insert new node into link of the neighbors' list
static void create_neighbors_list(NEIGHBOR_LIST_HANDLE *pneighbor_head, NEIGHBOR_LIST_OBJ node)
{
	if (NULL == pneighbor_head)
	{
		return;
	}
	
	NEIGHBOR_LIST_HANDLE pcur = NULL, pnew = NULL;
	
	pnew = create_node(node);    //create a node
	if (NULL == *pneighbor_head)
	{
		*pneighbor_head = pnew;
	}
	else
	{
		pcur = *pneighbor_head;
		while (NULL != pcur->next)
		{
			pcur = pcur->next;
		}
		pcur->next = pnew;
	}
}

//phead: Link's head node
//sum the number of nodes
static int count_link_nums(NEIGHBOR_LIST_HANDLE phead) 
{
	if (NULL == phead)
	{
		return 0;
	}
	int cnt = 0;
	NEIGHBOR_LIST_HANDLE ptmp = phead;
	
	while (NULL != ptmp)
	{
		cnt++;
		ptmp = ptmp->next;
	}
	return cnt;
}


static NEIGHBOR_LIST_HANDLE create_node(NEIGHBOR_LIST_OBJ node)
{
	NEIGHBOR_LIST_HANDLE pnew = (NEIGHBOR_LIST_HANDLE) malloc(sizeof(NEIGHBOR_LIST_OBJ));
	if (NULL == pnew)
	{
		perror("malloc error");
		return NULL;
	}
	memcpy(pnew->mac, node.mac, MAC_LEN);
	pnew->channel = node.channel;
	pnew->neighbor_flag = node.neighbor_flag;
	pnew->rssi = node.rssi;
	pnew->next = NULL;
	
	return pnew;
}

//free all the nodes of the neighbors' link
void free_all_neighbor_ap(CLIENT_INFO_HANDLE client_info, int len)
{
	int i = 0; 

	for (i = 0; i < len; i++)
	{
		NEIGHBOR_LIST_HANDLE pcur = client_info[i].neighbor_link_head, q;
		while (pcur != NULL)
		{
			q = pcur->next;
			free(pcur);
			pcur = q;
		}
		client_info[i].neighbor_cnt = 0;
	}
	free(client_info);
}

void *show_neighbor_ap_list_config(DBusConnection *dbus_connection, int *wtp_num, int *ret)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter	 iter;
	DBusError err;
	
	int localid = LOCALID;
	int index = LOCALINDEX;
	int i = 0, j = 0, k = 0, q = 0;
	unsigned char radio_num = 0;
	struct allwtp_neighborap *all_wtp_neighbor_ap = NULL;
	struct allwtp_neighborap *all_wtp_neighbor_apNODE = NULL;
	struct allwtp_neighborap_radioinfo	*radioinfo_node = NULL;
	struct Neighbor_AP_ELE *neighborapInfos_node = NULL;
	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_NEIGHBOR_AP_LIST);
	
	dbus_error_init(&err);
	
	reply = dbus_connection_send_with_reply_and_block (dbus_connection,query,DCLI_WID_DBUS_TIMEOUT, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		*ret = -1;
		return NULL;
	}	
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,ret);
	
	dbus_message_iter_next(&iter);	
	dbus_message_iter_get_basic(&iter,wtp_num);
	
	printf("%s:%d ret = %d wtp_num = %d\n", __func__, __LINE__, *ret, *wtp_num);
	
	if (*ret == 0)
	{
		if ((all_wtp_neighbor_ap = (struct allwtp_neighborap *)malloc(sizeof(struct allwtp_neighborap))) == NULL)
		{
			*ret = MALLOC_ERROR;
			dbus_message_unref(reply);
			return NULL;
		}
		
		memset(all_wtp_neighbor_ap, 0, sizeof(struct allwtp_neighborap));
		all_wtp_neighbor_ap->allwtp_neighborap_list = NULL;
		all_wtp_neighbor_ap->allwtp_neighborap_last = NULL;
		all_wtp_neighbor_ap->radioinfo_head = NULL;
		
		dbus_message_iter_next(&iter);
		
		for (i = 0; i < (*wtp_num); i++)
		{
			if ((all_wtp_neighbor_apNODE = (struct allwtp_neighborap*)malloc(sizeof(struct allwtp_neighborap))) == NULL)
			{
				dcli_free_allwtp_neighbor_ap(all_wtp_neighbor_ap);   
				*ret = MALLOC_ERROR;
				dbus_message_unref(reply);
				return NULL;
			}
			
			memset(all_wtp_neighbor_apNODE,0,sizeof(struct allwtp_neighborap));
			all_wtp_neighbor_apNODE->next = NULL;
			all_wtp_neighbor_apNODE->radioinfo_head = NULL;
			
			if (all_wtp_neighbor_ap->allwtp_neighborap_list == NULL)
			{
				all_wtp_neighbor_ap->allwtp_neighborap_list = all_wtp_neighbor_apNODE;
			}
			else
			{
				all_wtp_neighbor_ap->allwtp_neighborap_last->next = all_wtp_neighbor_apNODE;
			}
			all_wtp_neighbor_ap->allwtp_neighborap_last = all_wtp_neighbor_apNODE;
			
			dbus_message_iter_get_basic(&iter,&(all_wtp_neighbor_apNODE->WTPmac[0]));
			
			dbus_message_iter_next(&iter);
			dbus_message_iter_get_basic(&iter,&(all_wtp_neighbor_apNODE->WTPmac[1]));
			
			dbus_message_iter_next(&iter);
			dbus_message_iter_get_basic(&iter,&(all_wtp_neighbor_apNODE->WTPmac[2]));
			
			dbus_message_iter_next(&iter);
			dbus_message_iter_get_basic(&iter,&(all_wtp_neighbor_apNODE->WTPmac[3]));
			
			dbus_message_iter_next(&iter);
			dbus_message_iter_get_basic(&iter,&(all_wtp_neighbor_apNODE->WTPmac[4]));
			
			dbus_message_iter_next(&iter);
			dbus_message_iter_get_basic(&iter,&(all_wtp_neighbor_apNODE->WTPmac[5]));
			
			dbus_message_iter_next(&iter);
			dbus_message_iter_get_basic(&iter,&all_wtp_neighbor_apNODE->radio_num);	
			
			printf("%s:%d i = %d wtp[%d]["MACSTR"] radio_num= %d\n",__func__,__LINE__,i,all_wtp_neighbor_apNODE->wtpid,MAC2STR(all_wtp_neighbor_apNODE->WTPmac),all_wtp_neighbor_apNODE->radio_num);
			
			dbus_message_iter_next(&iter);
			
			for (j = 0; j < all_wtp_neighbor_apNODE->radio_num; j++)
			{
				if ((radioinfo_node = (struct allwtp_neighborap_radioinfo*)malloc(sizeof(struct allwtp_neighborap_radioinfo))) == NULL)
				{
					dcli_free_allwtp_neighbor_ap(all_wtp_neighbor_ap); 
					*ret = MALLOC_ERROR;
					dbus_message_unref(reply);
					return NULL;
				}
				
				memset(radioinfo_node, 0, sizeof(struct allwtp_neighborap_radioinfo));
				radioinfo_node->next = NULL;
				radioinfo_node->radioinfo_list = NULL;
				radioinfo_node->radioinfo_last = NULL;
				radioinfo_node->neighborapInfos_head = NULL;
				
				if (all_wtp_neighbor_apNODE->radioinfo_head == NULL)
				{
					all_wtp_neighbor_apNODE->radioinfo_head = radioinfo_node;
				}
				else
				{
					all_wtp_neighbor_apNODE->radioinfo_head->radioinfo_last->next = radioinfo_node;
				}
				
				all_wtp_neighbor_apNODE->radioinfo_head->radioinfo_last = radioinfo_node;
				
				dbus_message_iter_get_basic(&iter,&radioinfo_node->radiomac[0]);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->radiomac[1]);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->radiomac[2]);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->radiomac[3]);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->radiomac[4]);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->radiomac[5]);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->Radio_Channel);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->Radio_TXP);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->Radio_Type);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->failreason);
				
				dbus_message_iter_next(&iter);
				dbus_message_iter_get_basic(&iter,&radioinfo_node->rouge_ap_count);
				
				printf("%s:%d radioid= %d failreason= %d rouge_ap_count= %d\n",__func__,__LINE__,radioinfo_node->l_radioid,radioinfo_node->failreason,radioinfo_node->rouge_ap_count);
				
				dbus_message_iter_next(&iter);
				
				if (radioinfo_node->failreason == WID_DBUS_SUCCESS)
				{
					for (k = 0; k < radioinfo_node->rouge_ap_count; k++)
					{
						if ((neighborapInfos_node = (struct Neighbor_AP_ELE*)malloc(sizeof(struct Neighbor_AP_ELE))) == NULL)
						{
							dcli_free_allwtp_neighbor_ap(all_wtp_neighbor_ap);  
							*ret = MALLOC_ERROR;
							dbus_message_unref(reply);
							return NULL;
						}
						
						memset(neighborapInfos_node, 0, sizeof(struct Neighbor_AP_ELE));
						
						neighborapInfos_node->next = NULL;
						neighborapInfos_node->neighborapInfos_list = NULL;
						neighborapInfos_node->neighborapInfos_last = NULL;
						
						if (radioinfo_node->neighborapInfos_head == NULL)
						{
							radioinfo_node->neighborapInfos_head = neighborapInfos_node;
							// radioinfo_node->next = neighborapInfos_node;
						}
						else
						{
							radioinfo_node->neighborapInfos_head->neighborapInfos_last->next = neighborapInfos_node;
						}
								
						radioinfo_node->neighborapInfos_head->neighborapInfos_last = neighborapInfos_node;
						
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->BSSID[0]);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->BSSID[1]);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->BSSID[2]);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->BSSID[3]);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->BSSID[4]);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->BSSID[5]);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->identity_flag);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->Rate);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->Channel);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->RSSI);
						
						dbus_message_iter_next(&iter);
						dbus_message_iter_get_basic(&iter,&neighborapInfos_node->NOISE);
						
						dbus_message_iter_next(&iter);
						
						for (q = 0; q < ESSID_DEFAULT_LEN+1; q++)
						{
							dbus_message_iter_get_basic(&iter,&neighborapInfos_node->ESSID[q]);
							dbus_message_iter_next(&iter);
						}
						
						printf("%s:%d k = %d BSSID["MACSTR"] rate= %d channel= %d rssi= %d noise= %d ESSID = %s\n",
								__func__,__LINE__,k,MAC2STR(neighborapInfos_node->BSSID),neighborapInfos_node->Rate,
								neighborapInfos_node->Channel,neighborapInfos_node->RSSI,neighborapInfos_node->NOISE,
								neighborapInfos_node->ESSID);
					}
				}
			}
		}
	}
	
	dbus_message_unref(reply);
	
	return all_wtp_neighbor_ap;
}


int show_network_country_code(int *country_code, DBusConnection *dcli_dbus_connection)
{
	int i = 0;
	int ret = 0;
	int localid = LOCALID;
	int index = LOCALINDEX;  
	DBusError err;
	DBusMessageIter  iter;
	DBusMessage *query = NULL, *reply = NULL; 
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0}; 
	
	if(!country_code || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument country_code %p dbus_connection %p\n", 
						__func__, __LINE__, country_code, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_GET_COUNTRY_CODE);
	
	dbus_error_init(&err);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
	    return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter, country_code);
	
	dbus_message_unref(reply);
	
	if(*country_code >= 0)
	{
		return WID_DBUS_SUCCESS;
	}
	return -WID_DBUS_ERROR;
}

int set_network_country_code(unsigned int country_code, DBusConnection *dcli_dbus_connection)
{
	int i = 0;
	int ret = 0;
	int localid = LOCALID;
	int index = LOCALINDEX;
	int wtpid = 0;
	int radioid = 0;
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0}; 
	
	if (!dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p\n", __func__, __LINE__, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}

	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_SET_COUNTRY_CODE);
	
	dbus_error_init(&err);
	
	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &country_code,
							 DBUS_TYPE_UINT32, &wtpid,
							 DBUS_TYPE_UINT32, &radioid,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection, query, DCLI_WID_DBUS_TIMEOUT, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, &ret);
	
	dbus_message_unref(reply);
	
	return ret;
}


int handlib_modify_ip_network_config
(
	IP_WIRE_ARG *ip_config,
	DBusConnection *dcli_dbus_connection
)
{
	int ret = 0;
	int localid = LOCALID;
	int index = LOCALINDEX;  
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	char addr_ip[IPv6_STR_LEN] = {0};
	char addr_mask[IPv6_STR_LEN] = {0};
	char addr_gateway[IPv6_STR_LEN] = {0};
	char addr_fstdns[IPv6_STR_LEN] = {0};
	char addr_snddns[IPv6_STR_LEN] = {0};
	
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0}; 
	
	if(!ip_config || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument ip_config %p dbus_connection %p\n", 
						__func__, __LINE__, ip_config, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_SET_IP_NETWORK);
	
	dbus_message_append_args(query,
								DBUS_TYPE_UINT32, &(ip_config->wtpid),
								DBUS_TYPE_BYTE, &(ip_config->type),
								DBUS_TYPE_UINT32, &(ip_config->ip),
								DBUS_TYPE_UINT32, &(ip_config->mask),
								DBUS_TYPE_UINT32, &(ip_config->gateway),
								DBUS_TYPE_UINT32, &(ip_config->fstdns),
								DBUS_TYPE_UINT32, &(ip_config->snddns),
								DBUS_TYPE_INVALID);
	dbus_error_init(&err);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
		
	syslog(LOG_DEBUG, "%s:%d set wtp%u ip: %s(%u) mask: %s(%u) gateway: %s(%u) fristdns: %s(%u) fristdns: %s(%u) ret= %d\n",
						__func__, __LINE__, (ip_config->wtpid),
						(char *)wid_parse_inet_ntoa(ip_config->ip, addr_ip), (ip_config->ip),
						(char *)wid_parse_inet_ntoa(ip_config->mask, addr_mask), (ip_config->mask),
						(char *)wid_parse_inet_ntoa(ip_config->gateway, addr_gateway), (ip_config->gateway),
						(char *)wid_parse_inet_ntoa(ip_config->fstdns, addr_fstdns), (ip_config->fstdns),
						(char *)wid_parse_inet_ntoa(ip_config->snddns, addr_snddns), (ip_config->snddns), ret);
	
	dbus_message_unref(reply);
	
	return ret;
}


int handlib_overrides_radio_network_config
(
	RADIO_WIRE_ARG *bss_config,
	DBusConnection *dcli_dbus_connection
)
{
	int ret = 0;
	int localid = LOCALID;
	int index = LOCALINDEX;  
	
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!bss_config || !dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument bss_config %p dbus_connection %p\n", 
						__func__, __LINE__, bss_config, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	char *essid = bss_config->ssid;
	char *security = bss_config->security;
	char *user_key = bss_config->user_key;
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_CONF_METHOD_COVER_RADIO_WLAN);
	
	dbus_message_append_args(query,
								DBUS_TYPE_UINT32, &(bss_config->wtpid),
								DBUS_TYPE_BYTE, &(bss_config->radioid),
								DBUS_TYPE_BYTE, &(bss_config->wlanid),
								DBUS_TYPE_BYTE, &(bss_config->wlan_enabled),
								DBUS_TYPE_BYTE, &(bss_config->vlan_enabled),
								DBUS_TYPE_UINT32, &(bss_config->vlanid),
								DBUS_TYPE_STRING, &essid,
								DBUS_TYPE_STRING, &security,
								DBUS_TYPE_STRING, &user_key,
								DBUS_TYPE_INVALID);
	dbus_error_init(&err);
	
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
	
	dbus_message_unref(query);
	
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	printf("%s:%d set radio%u-%u overrides wlan: %s(%u) key: %s %s vlan: %u %s ret= %d\n",
			__func__, __LINE__, (bss_config->wtpid), (bss_config->radioid), essid,
			bss_config->wlanid, user_key, (bss_config->wlan_enabled) ? "enable" : "disable",
			(bss_config->vlanid), (bss_config->vlan_enabled) ? "used" : "unused", ret);
	
	dbus_message_unref(reply);
	
	return ret;
}

int handlib_wlan_hideessid_config
(
	unsigned char wlanid,
	unsigned char hideessid,
	DBusConnection *dcli_dbus_connection
)
{
	int ret = 0;
	int localid = LOCALID;
	int index = LOCALINDEX;  
	
	DBusError err;
	DBusMessageIter iter;
	DBusMessage *query = NULL, *reply = NULL; 
	char BUSNAME[PATH_LEN]= {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	
	if(!dcli_dbus_connection)
	{
		syslog(LOG_ERR, "%s line %d bad input argument dbus_connection %p\n", __func__, __LINE__, dcli_dbus_connection);
		return WID_DBUS_ERROR;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME,OBJPATH,INTERFACE,WID_DBUS_WLAN_METHOD_HIDE_ESSID);
	
	dbus_message_append_args(query,
								DBUS_TYPE_BYTE,&wlanid,
								DBUS_TYPE_BYTE,&hideessid,
								DBUS_TYPE_INVALID);
	dbus_error_init(&err);
    
	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection,query,-1, &err);
    
	dbus_message_unref(query);
    
	if (NULL == reply)
	{
		if (dbus_error_is_set(&err))
		{
			dbus_error_free(&err);
		}
		return WID_DBUS_ERROR;
	}
	
	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter,&ret);
	
	printf("%s:%d set wlan%u hideessid %s(%u) ret= %d\n", __func__, __LINE__, wlanid, (hideessid) ? "yes" : "no", hideessid, ret);
	
	dbus_message_unref(reply);
	
	return ret;
}


