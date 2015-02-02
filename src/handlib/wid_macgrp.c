#ifdef _D_WCPSS_

#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <dbus/dbus.h>
#include "wcpss/waw.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/ACDbusDef1.h"
#include "dbus/asd/ASDDbusDef1.h"
#include "wcpss/asd/asd.h"
#include "wid_ac.h"
#include "wid_wtp.h"
#include "wid_macgrp.h"
#include "dbus/wcpss/dcli_wid_wtp.h"
//#include "dbus/wcpss/dcli_wid_macgrp.h"

//xingqj add
int dcli_macgrp_add_mac_node(WID_MAC_GROUP **GROUP, MAC_list *list)
{
	MAC_list *tmp = NULL;
	
	/* check param */
	if (list == NULL)
	{
		return -1;
	}
	if ((NULL == GROUP) || (NULL == *GROUP))
	{
		return -1;
	}
	
	if (NULL == (*GROUP)->macList)
	{
		(*GROUP)->macList = list;
		list->next = NULL;
	}
	else
	{
		tmp = (*GROUP)->macList;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = list;
		list->next = NULL;
	}
	
	return 0;
}

void dcli_macgrp_free_fun(DCLI_MACGRP_API_GROUP *MACGRPINFO)
{
	int i = 0;
	MAC_list *list = NULL;
	MAC_list *temp = NULL;
	
	if(NULL == MACGRPINFO)
	{
		return;
	}
	if(NULL == MACGRPINFO->MACGROUP)
	{
		CW_FREE_OBJECT(MACGRPINFO);
		return;
	}
	for(i = 0; i < MACGRPINFO->macgrp_num; i++)
	{
		if(NULL == MACGRPINFO->MACGROUP[i])
			continue;			
		
		CW_FREE_OBJECT(MACGRPINFO->MACGROUP[i]->name);

		if(MACGRPINFO->MACGROUP[i]->macList)
		{
			list = MACGRPINFO->MACGROUP[i]->macList;
			while(list)
			{
				temp = list->next;
				CW_FREE_OBJECT(list);
				list = temp;
			}
		}
		CW_FREE_OBJECT(MACGRPINFO->MACGROUP[i]);
	}
	CW_FREE_OBJECT(MACGRPINFO->MACGROUP);
	CW_FREE_OBJECT(MACGRPINFO);

	return;
}


DCLI_MACGRP_API_GROUP *show_macgroup_id
(
	DBusConnection *dcli_dbus_connection,
	int index,
	int localid,
	unsigned int macgroupID,
	unsigned int *ret
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusMessageIter iter_array;
	DBusMessageIter iter_struct;
	DBusError err;
	unsigned int i = 0;	
	char *groupname = NULL;
	DCLI_MACGRP_API_GROUP *MACGRPINFO = NULL;	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	MAC_list *maclist = NULL;

	/* check param */
	if ((NULL == dcli_dbus_connection) || (NULL == ret))
	{
		return NULL;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME, OBJPATH, INTERFACE, 
											WID_DBUS_CONF_METHOD_SHOWMACGRP);
	
	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_UINT32, &macgroupID,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block (dcli_dbus_connection, query, -1, &err);

	dbus_message_unref(query);
	if (NULL == reply) 
	{
		*ret = WID_DBUS_ERROR;
		if (dbus_error_is_set(&err)) 
		{
			dbus_error_free(&err);
		}
		return NULL;
	}

	dbus_message_iter_init(reply, &iter);
	dbus_message_iter_get_basic(&iter, &(*ret));

	if (0 == *ret)
	{	
		/* malloc  MACGRPINFO */
		MACGRPINFO = (DCLI_MACGRP_API_GROUP *)malloc(sizeof(DCLI_MACGRP_API_GROUP));
		if (NULL == MACGRPINFO)
		{
			*ret = MALLOC_ERROR;
			return NULL;
		}
		memset(MACGRPINFO, 0, sizeof(DCLI_MACGRP_API_GROUP));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(MACGRPINFO->macgrp_num));		

		/* malloc  MACGRPINFO->MACGROUP  */	
		MACGRPINFO->MACGROUP = (WID_MAC_GROUP **)malloc(sizeof(WID_MAC_GROUP *));
		if (NULL == MACGRPINFO->MACGROUP)
		{
			*ret = MALLOC_ERROR;
			CW_FREE_OBJECT(MACGRPINFO);
			return NULL;
		}
		memset(MACGRPINFO->MACGROUP, 0, sizeof(WID_MAC_GROUP *));

		/* malloc  MACGRPINFO->MACGROUP[0]  */	
		MACGRPINFO->MACGROUP[0] = (WID_MAC_GROUP *)malloc(sizeof(WID_MAC_GROUP));
		if (NULL == MACGRPINFO->MACGROUP[0])
		{
			*ret = MALLOC_ERROR;
			dcli_macgrp_free_fun(MACGRPINFO);
			MACGRPINFO = NULL;
			return NULL;
		}
		memset(MACGRPINFO->MACGROUP[0], 0, sizeof(WID_MAC_GROUP));

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(MACGRPINFO->MACGROUP[0]->macgroupID));

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(groupname));		

		MACGRPINFO->MACGROUP[0]->name = (char*)malloc(strlen(groupname)+1);
		if( NULL == MACGRPINFO->MACGROUP[0]->name)
		{
			*ret = MALLOC_ERROR;
			dcli_macgrp_free_fun(MACGRPINFO);
			MACGRPINFO = NULL;
			return NULL;		
		}
		memset(MACGRPINFO->MACGROUP[0]->name, 0, strlen(groupname)+1);	
		memcpy(MACGRPINFO->MACGROUP[0]->name, groupname, strlen(groupname));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(MACGRPINFO->MACGROUP[0]->count));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_recurse(&iter, &iter_array);
		
		for (i = 0; i < MACGRPINFO->MACGROUP[0]->count; i++)
		{
			maclist = (MAC_list *)malloc(sizeof(MAC_list));
			if (NULL == maclist)
			{
				*ret = MALLOC_ERROR;
				dcli_macgrp_free_fun(MACGRPINFO);
				MACGRPINFO = NULL;
				return NULL;			
			}
			memset(maclist, 0, sizeof(MAC_list));
		
			dbus_message_iter_recurse(&iter_array, &iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(maclist->mac[0]));
		
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(maclist->mac[1]));
		
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(maclist->mac[2]));
		
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(maclist->mac[3]));
		
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(maclist->mac[4]));
		
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(maclist->mac[5]));
		
			dbus_message_iter_next(&iter_array);
			dcli_macgrp_add_mac_node(&(MACGRPINFO->MACGROUP[0]), maclist);
		}		

	}

	dbus_message_unref(reply);
	return MACGRPINFO;
}

DCLI_MACGRP_API_GROUP *show_macgroup_all
(
	DBusConnection *dcli_dbus_connection,
	int index, 
	int localid,
	unsigned int *ret
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusMessageIter	iter_array;
	DBusMessageIter iter_struct;
	DBusMessageIter iter_sub_array; 	
	DBusMessageIter iter_sub_struct;
	DBusError err;
	unsigned int i = 0, j = 0;	
	char *groupname = NULL;
	DCLI_MACGRP_API_GROUP *MACGRPINFO = NULL;	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	
	MAC_list *maclist = NULL;

	/* check param */
	if ((NULL == dcli_dbus_connection) || (NULL == ret))
	{
		return NULL;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	query = dbus_message_new_method_call(BUSNAME, OBJPATH,INTERFACE, 
											WID_DBUS_CONF_METHOD_SHOWMACGRP_ALL);	
	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection, query, DCLI_WID_DBUS_TIMEOUT, &err);

	dbus_message_unref(query);

	if (NULL == reply) 
	{
		*ret = WID_DBUS_ERROR;
		if (dbus_error_is_set(&err)) 
		{
			dbus_error_free(&err);
		}
		return NULL;
	}

	dbus_message_iter_init(reply,&iter);
	dbus_message_iter_get_basic(&iter, &(*ret));

	if(*ret == 0 )
	{	
		MACGRPINFO = (DCLI_MACGRP_API_GROUP *)malloc(sizeof(DCLI_MACGRP_API_GROUP));
		if(NULL == MACGRPINFO)
		{
			*ret = MALLOC_ERROR;
			return NULL;
		}
		memset(MACGRPINFO, 0, sizeof(DCLI_MACGRP_API_GROUP));

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(MACGRPINFO->macgrp_num));		
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_recurse(&iter, &iter_array);
		
		MACGRPINFO->MACGROUP = (WID_MAC_GROUP *)malloc((MACGRPINFO->macgrp_num)*(sizeof(WID_MAC_GROUP *)));
		if (NULL == MACGRPINFO->MACGROUP)
		{
			*ret = MALLOC_ERROR;
			CW_FREE_OBJECT(MACGRPINFO);
			return NULL;
		}
		memset(MACGRPINFO->MACGROUP, 0, sizeof(WID_MAC_GROUP *));
		
		for(j = 0; j < MACGRPINFO->macgrp_num; j++)
		{
			MACGRPINFO->MACGROUP[j] = (WID_MAC_GROUP *)malloc(sizeof(WID_MAC_GROUP));
			if (NULL == MACGRPINFO->MACGROUP[j])
			{
				*ret = MALLOC_ERROR;
				dcli_macgrp_free_fun(MACGRPINFO);
				MACGRPINFO = NULL;
				return NULL;
			}			
			memset(MACGRPINFO->MACGROUP[j], 0, sizeof(WID_MAC_GROUP));
			
			dbus_message_iter_recurse(&iter_array,&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(MACGRPINFO->MACGROUP[j]->macgroupID));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(groupname));
			
			MACGRPINFO->MACGROUP[j]->name = (char*)malloc(strlen(groupname)+1);
			if( NULL == MACGRPINFO->MACGROUP[j]->name)
			{
				*ret = MALLOC_ERROR;
				dcli_macgrp_free_fun(MACGRPINFO);
				MACGRPINFO = NULL;
				return NULL;		
			}			
			memset(MACGRPINFO->MACGROUP[j]->name, 0, strlen(groupname)+1);	
			memcpy(MACGRPINFO->MACGROUP[j]->name, groupname, strlen(groupname));
			
			dbus_message_iter_next(&iter_struct);	
			dbus_message_iter_get_basic(&iter_struct, &(MACGRPINFO->MACGROUP[j]->count));
			
			dbus_message_iter_next(&iter_struct);	
			dbus_message_iter_recurse(&iter_struct, &iter_sub_array);

			for (i = 0; i < MACGRPINFO->MACGROUP[j]->count; i++)
			{
				maclist = (MAC_list *)malloc(sizeof(MAC_list));
				if( NULL == maclist)
				{
					*ret = MALLOC_ERROR;
					dcli_macgrp_free_fun(MACGRPINFO);
					MACGRPINFO = NULL;
					return NULL;		
				}				
				memset(maclist, 0, sizeof(MAC_list));
			
				dbus_message_iter_recurse(&iter_sub_array,&iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(maclist->mac[0]));
			
				dbus_message_iter_next(&iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(maclist->mac[1]));
			
				dbus_message_iter_next(&iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(maclist->mac[2]));
			
				dbus_message_iter_next(&iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(maclist->mac[3]));
			
				dbus_message_iter_next(&iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(maclist->mac[4]));
			
				dbus_message_iter_next(&iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(maclist->mac[5]));
				
				dbus_message_iter_next(&iter_sub_struct);			
				dbus_message_iter_next(&iter_sub_array);
				
				dcli_macgrp_add_mac_node(&(MACGRPINFO->MACGROUP[j]),maclist);
			}
			
			dbus_message_iter_next(&iter_array);
		}
	}

	dbus_message_unref(reply);
	return MACGRPINFO;
}

#endif
