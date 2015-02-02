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
#include "wid_aclgrp.h"
#include "dbus/wcpss/dcli_wid_wtp.h"
#include "sysdef/returncode.h"


//xingqj add
int dcli_aclgrp_add_rule_node(STA_ACLGRP **GROUP, ACL_RULElist *list)
{
	ACL_RULElist *tmp = NULL;
	
	/* check param */
	if (list == NULL)
	{
		return -1;
	}
	if ((NULL == GROUP) || (NULL == *GROUP))
	{
		return -1;
	}
	
	if (NULL == (*GROUP)->rule_list)
	{
		(*GROUP)->rule_list = list;
		list->next = NULL;
	}
	else
	{
		tmp = (*GROUP)->rule_list;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = list;
		list->next = NULL;
	}
	
	return 0;
}

void dcli_aclgrp_free_fun(DCLI_ACL_LIST *STAACLLIST)
{
	int i = 0;
	
	if(NULL == STAACLLIST)
	{
		return;
	}
	if(NULL == STAACLLIST->AclList)
	{
		CW_FREE_OBJECT(STAACLLIST);
		return;
	}
	for(i = 0; i < STAACLLIST->num; i++)
	{
		if(NULL == STAACLLIST->AclList[i])
		{
			continue;			
		}
		
		CW_FREE_OBJECT(STAACLLIST->AclList[i]->dom_str);
		CW_FREE_OBJECT(STAACLLIST->AclList[i]);
	}
	CW_FREE_OBJECT(STAACLLIST->AclList);
	CW_FREE_OBJECT(STAACLLIST);

	return;
}

void dcli_aclgrp_free_group_fun(DCLI_ACLGRP_SHOW *ACLGRPINFO)
{
	int i = 0;
	ACL_RULElist *list = NULL;
	ACL_RULElist *temp = NULL;
	ACL_WLANlist *wlanlist = NULL;
	ACL_WLANlist *wlantmp = NULL;
	if(NULL == ACLGRPINFO)
	{
		return;
	}
	if(NULL == ACLGRPINFO->ACLGROUP)
	{
		CW_FREE_OBJECT(ACLGRPINFO);
		return;
	}
	for(i = 0; i < ACLGRPINFO->num; i++)
	{
		if(NULL == ACLGRPINFO->ACLGROUP[i])
		{
			continue;			
		}
		
		CW_FREE_OBJECT(ACLGRPINFO->ACLGROUP[i]->groupname);

		if(ACLGRPINFO->ACLGROUP[i]->rule_list)
		{
			list = ACLGRPINFO->ACLGROUP[i]->rule_list;
			while(list)
			{
				temp = list->next;
				CW_FREE_OBJECT(list);
				list = temp;
			}
		}
		if(ACLGRPINFO->ACLGROUP[i]->WlanList)
		{
			wlanlist = ACLGRPINFO->ACLGROUP[i]->WlanList;
			while(wlanlist)
			{
				wlantmp = wlanlist->next;
				CW_FREE_OBJECT(wlanlist);
				wlanlist = wlantmp;
			}
		}

		CW_FREE_OBJECT(ACLGRPINFO->ACLGROUP[i]);
	}
	CW_FREE_OBJECT(ACLGRPINFO->ACLGROUP);
	CW_FREE_OBJECT(ACLGRPINFO);

	return;	
}

DCLI_ACLGRP_SHOW *show_aclgroup_id
(
	DBusConnection *dcli_dbus_connection,
	int index, 
	int localid,
	int slot_id, 
	unsigned char groupid, 
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
	DCLI_ACLGRP_SHOW *ACLGRPINFO = NULL;	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	ACL_RULElist *rulelist = NULL;

	/* check param */
	if (NULL == ret)
	{
		return NULL;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME, OBJPATH, INTERFACE, 
											WID_DBUS_CONF_METHOD_ACLGRP_SHOW_ACL_GROUP);
	
	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_BYTE, &groupid,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block (dcli_dbus_connection, query, -1, &err);

	dbus_message_unref(query);
	if (NULL == reply) 
	{
		*ret = WID_RETURN_CODE_DBUS_ERROR;
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
		ACLGRPINFO = (DCLI_ACLGRP_SHOW *)malloc(sizeof(DCLI_ACLGRP_SHOW));
		if (NULL == ACLGRPINFO)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			return NULL;
		}
		memset(ACLGRPINFO, 0, sizeof(DCLI_ACLGRP_SHOW));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(ACLGRPINFO->num));		

		/* malloc  MACGRPINFO->MACGROUP  */	
		ACLGRPINFO->ACLGROUP = (STA_ACLGRP **)malloc(sizeof(STA_ACLGRP *));
		if (NULL == ACLGRPINFO->ACLGROUP)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			CW_FREE_OBJECT(ACLGRPINFO);
			return NULL;
		}
		memset(ACLGRPINFO->ACLGROUP, 0, sizeof(STA_ACLGRP *));

		/* malloc  MACGRPINFO->MACGROUP[0]  */	
		ACLGRPINFO->ACLGROUP[0] = (STA_ACLGRP *)malloc(sizeof(STA_ACLGRP));
		if (NULL == ACLGRPINFO->ACLGROUP[0])
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			dcli_aclgrp_free_group_fun(ACLGRPINFO);
			ACLGRPINFO = NULL;
			return NULL;
		}
		memset(ACLGRPINFO->ACLGROUP[0], 0, sizeof(STA_ACLGRP));

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(ACLGRPINFO->ACLGROUP[0]->groupid));

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(groupname));		

		ACLGRPINFO->ACLGROUP[0]->groupname = (char*)malloc(strlen(groupname)+1);
		if(NULL == ACLGRPINFO->ACLGROUP[0]->groupname)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			dcli_aclgrp_free_group_fun(ACLGRPINFO);
			ACLGRPINFO = NULL;
			return NULL;		
		}
		memset(ACLGRPINFO->ACLGROUP[0]->groupname, 0, strlen(groupname)+1);	
		memcpy(ACLGRPINFO->ACLGROUP[0]->groupname, groupname, strlen(groupname));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(ACLGRPINFO->ACLGROUP[0]->rule_cnt));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_recurse(&iter, &iter_array);
		
		for (i = 0; i < ACLGRPINFO->ACLGROUP[0]->rule_cnt; i++)
		{
			rulelist = (ACL_RULElist *)malloc(sizeof(ACL_RULElist));
			if (NULL == rulelist)
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_group_fun(ACLGRPINFO);
				ACLGRPINFO = NULL;
				return NULL;			
			}
			memset(rulelist, 0, sizeof(ACL_RULElist));
		
			dbus_message_iter_recurse(&iter_array, &iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(rulelist->rule_id));
			
			dbus_message_iter_next(&iter_array);
			dcli_aclgrp_add_rule_node(&(ACLGRPINFO->ACLGROUP[0]), rulelist);
		}		

	}

	dbus_message_unref(reply);
	return ACLGRPINFO;
}

DCLI_ACLGRP_SHOW *show_aclgroup_all
(	
	DBusConnection *dcli_dbus_connection,
	int index, 
	int localid, 
	int slot_id,
	unsigned int *ret
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusMessageIter iter_array;
	DBusMessageIter iter_sub_array;
	DBusMessageIter iter_struct;
	DBusMessageIter iter_sub_struct;
	DBusError err;
	unsigned int i = 0, j = 0;	
	char *groupname = NULL;
	DCLI_ACLGRP_SHOW *ACLGRPINFO = NULL;	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};
	ACL_RULElist *rulelist = NULL;

	/* check param */
	if (NULL == ret)
	{
		return NULL;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME, OBJPATH, INTERFACE, 
				WID_DBUS_CONF_METHOD_ACLGRP_SHOW_ACL_GROUP_ALL);
	
	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block (dcli_dbus_connection, query, -1, &err);

	dbus_message_unref(query);
	if (NULL == reply) 
	{
		*ret = WID_RETURN_CODE_DBUS_ERROR;
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
		ACLGRPINFO = (DCLI_ACLGRP_SHOW *)malloc(sizeof(DCLI_ACLGRP_SHOW));
		if (NULL == ACLGRPINFO)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			return NULL;
		}
		memset(ACLGRPINFO, 0, sizeof(DCLI_ACLGRP_SHOW));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(ACLGRPINFO->num));		
		
		/* malloc  MACGRPINFO->MACGROUP  */	
		ACLGRPINFO->ACLGROUP = (STA_ACLGRP **)malloc(sizeof(STA_ACLGRP *) * (ACLGRPINFO->num));
		if (NULL == ACLGRPINFO->ACLGROUP)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			CW_FREE_OBJECT(ACLGRPINFO);
			return NULL;
		}
		memset(ACLGRPINFO->ACLGROUP, 0, sizeof(STA_ACLGRP *) * (ACLGRPINFO->num));
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_recurse(&iter, &iter_array);
		/* malloc  MACGRPINFO->MACGROUP[0]  */	
		for (j = 0; j < ACLGRPINFO->num; j++)
		{
			ACLGRPINFO->ACLGROUP[j] = (STA_ACLGRP *)malloc(sizeof(STA_ACLGRP));
			if (NULL == ACLGRPINFO->ACLGROUP[j])
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_group_fun(ACLGRPINFO);
				ACLGRPINFO = NULL;
				return NULL;
			}
			memset(ACLGRPINFO->ACLGROUP[j], 0, sizeof(STA_ACLGRP));

			dbus_message_iter_recurse(&iter_array,&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLGRPINFO->ACLGROUP[j]->groupid));

			dbus_message_iter_next(&iter_struct);	
			dbus_message_iter_get_basic(&iter_struct, &(groupname));		

			ACLGRPINFO->ACLGROUP[j]->groupname = (char*)malloc(strlen(groupname)+1);
			if(NULL == ACLGRPINFO->ACLGROUP[j]->groupname)
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_group_fun(ACLGRPINFO);
				ACLGRPINFO = NULL;
				return NULL;		
			}
			memset(ACLGRPINFO->ACLGROUP[j]->groupname, 0, strlen(groupname)+1);	
			memcpy(ACLGRPINFO->ACLGROUP[j]->groupname, groupname, strlen(groupname));
			
			dbus_message_iter_next(&iter_struct);	
			dbus_message_iter_get_basic(&iter_struct, &(ACLGRPINFO->ACLGROUP[j]->rule_cnt));
			
			dbus_message_iter_next(&iter_struct);	
			dbus_message_iter_recurse(&iter_struct, &iter_sub_array);
			
			for (i = 0; i < ACLGRPINFO->ACLGROUP[j]->rule_cnt; i++)
			{
				rulelist = (ACL_RULElist *)malloc(sizeof(ACL_RULElist));
				if (NULL == rulelist)
				{
					*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
					dcli_aclgrp_free_group_fun(ACLGRPINFO);
					ACLGRPINFO = NULL;
					return NULL;			
				}
				memset(rulelist, 0, sizeof(ACL_RULElist));
			
				dbus_message_iter_recurse(&iter_sub_array, &iter_sub_struct);
				dbus_message_iter_get_basic(&iter_sub_struct, &(rulelist->rule_id));
			
				dbus_message_iter_next(&iter_sub_array);
				dcli_aclgrp_add_rule_node(&(ACLGRPINFO->ACLGROUP[j]), rulelist);
			}	
			dbus_message_iter_next(&iter_array);
		}

	}

	dbus_message_unref(reply);
	return ACLGRPINFO;
}


DCLI_ACL_LIST *show_sta_acl_id
(
	DBusConnection *dcli_dbus_connection,
	int index, 
	int localid,
	int slot_id, 
	unsigned int rule_id,
	unsigned int *ret
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusMessageIter	iter_array;
	DBusMessageIter iter_struct;

	DBusError err;
	unsigned int i = 0, j = 0;	
	char *domain_str = NULL;
	DCLI_ACL_LIST *ACLLIST = NULL;	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	

	/* check param */
	if (NULL == ret)
	{
		return NULL;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);

	query = dbus_message_new_method_call(BUSNAME, OBJPATH,INTERFACE, 
											WID_DBUS_CONF_METHOD_ACLGRP_SHOW_ACL_RULE_ID);
	
	dbus_error_init(&err);

	dbus_message_append_args(query,
							DBUS_TYPE_UINT32, &rule_id,
							DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection, query, -1, &err);

	dbus_message_unref(query);
	if (NULL == reply) 
	{
		*ret = WID_RETURN_CODE_DBUS_ERROR;
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
		ACLLIST = (DCLI_ACL_LIST *)malloc(sizeof(DCLI_ACL_LIST));
		if(NULL == ACLLIST)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			return NULL;
		}
		memset(ACLLIST, 0, sizeof(DCLI_ACL_LIST));
		

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(ACLLIST->num));		
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_recurse(&iter, &iter_array);
		ACLLIST->AclList = (STA_ACL_SHOW **)malloc(sizeof(STA_ACL_SHOW *) * ACLLIST->num);
		if (NULL == ACLLIST->AclList)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			CW_FREE_OBJECT(ACLLIST);
			return NULL;
		}
		memset(ACLLIST->AclList, 0, sizeof(STA_ACL_SHOW *) * ACLLIST->num);
		
		for(j = 0; j < ACLLIST->num; j++)
		{
			ACLLIST->AclList[j] = (STA_ACL_SHOW *)malloc(sizeof(STA_ACL_SHOW));
			if (NULL == ACLLIST->AclList[j])
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_fun(ACLLIST);
				ACLLIST = NULL;
				return NULL;
			}			
			memset(ACLLIST->AclList[j], 0, sizeof(STA_ACL_SHOW));
			
			dbus_message_iter_recurse(&iter_array,&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_index));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_type));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_action));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_proto));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->direct));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->dip));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->dmask));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->sip));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->smask));


			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->dport));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->sport));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->type));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->typemask));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->code));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->codemask));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &domain_str);

			ACLLIST->AclList[j]->dom_str = (char*)malloc(strlen(domain_str)+1);
			if( NULL == ACLLIST->AclList[j]->dom_str)
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_fun(ACLLIST);
				ACLLIST = NULL;
				return NULL;		
			}			
			memset(ACLLIST->AclList[j]->dom_str, 0, strlen(domain_str)+1);	
			memcpy(ACLLIST->AclList[j]->dom_str, domain_str, strlen(domain_str));			

			dbus_message_iter_next(&iter_array);
		}

	}

	dbus_message_unref(reply);
	return ACLLIST;
}


DCLI_ACL_LIST *show_sta_acl_all
(
	DBusConnection *dcli_dbus_connection, 
	int index, 
	int localid,
	int slot_id, 
	unsigned int *ret
)
{
	DBusMessage *query = NULL, *reply = NULL; 
	DBusMessageIter iter;
	DBusMessageIter	iter_array;
	DBusMessageIter iter_struct;

	DBusError err;
	unsigned int i = 0, j = 0;	
	char *domain_str = NULL;
	DCLI_ACL_LIST *ACLLIST = NULL;	
	char BUSNAME[PATH_LEN] = {0};
	char OBJPATH[PATH_LEN] = {0};
	char INTERFACE[PATH_LEN] = {0};	

	/* check param */
	if(NULL == ret)
	{
		return NULL;
	}
	
	ReInitDbusPath_V2(localid,index,WID_DBUS_BUSNAME,BUSNAME);
	ReInitDbusPath_V2(localid,index,WID_DBUS_OBJPATH,OBJPATH);
	ReInitDbusPath_V2(localid,index,WID_DBUS_INTERFACE,INTERFACE);
	
	query = dbus_message_new_method_call(BUSNAME, OBJPATH,INTERFACE, 
					WID_DBUS_CONF_METHOD_ACLGRP_SHOW_ACL_RULE_ALL);
	
	dbus_error_init(&err);

	dbus_message_append_args(query,
							 DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(dcli_dbus_connection, query, -1, &err);

	dbus_message_unref(query);
	if (NULL == reply) 
	{
		*ret = WID_RETURN_CODE_DBUS_ERROR;
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
		ACLLIST = (DCLI_ACL_LIST *)malloc(sizeof(DCLI_ACL_LIST));
		if(NULL == ACLLIST)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			return NULL;
		}
		memset(ACLLIST, 0, sizeof(DCLI_ACL_LIST));

		dbus_message_iter_next(&iter);	
		dbus_message_iter_get_basic(&iter, &(ACLLIST->num));		
		
		dbus_message_iter_next(&iter);	
		dbus_message_iter_recurse(&iter, &iter_array);
		ACLLIST->AclList = (STA_ACL_SHOW **)malloc(sizeof(STA_ACL_SHOW *) * ACLLIST->num);
		if (NULL == ACLLIST->AclList)
		{
			*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
			CW_FREE_OBJECT(ACLLIST);
			return NULL;
		}
		memset(ACLLIST->AclList, 0, sizeof(STA_ACL_SHOW *) * ACLLIST->num);
		
		for(j = 0; j < ACLLIST->num; j++)
		{
			ACLLIST->AclList[j] = (STA_ACL_SHOW *)malloc(sizeof(STA_ACL_SHOW));
			if (NULL == ACLLIST->AclList[j])
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_fun(ACLLIST);
				ACLLIST = NULL;
				return NULL;
			}			
			memset(ACLLIST->AclList[j], 0, sizeof(STA_ACL_SHOW));
			
			dbus_message_iter_recurse(&iter_array,&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_index));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_type));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_action));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->rule_proto));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->direct));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->dip));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->dmask));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->sip));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->smask));


			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->dport));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->sport));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->type));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->typemask));
			
			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->code));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &(ACLLIST->AclList[j]->codemask));

			dbus_message_iter_next(&iter_struct);
			dbus_message_iter_get_basic(&iter_struct, &domain_str);



			ACLLIST->AclList[j]->dom_str = (char*)malloc(strlen(domain_str)+1);
			if( NULL == ACLLIST->AclList[j]->dom_str)
			{
				*ret = WID_RETURN_CODE_OUT_OF_MEMORY;
				dcli_aclgrp_free_fun(ACLLIST);
				ACLLIST = NULL;
				return NULL;		
			}			
			memset(ACLLIST->AclList[j]->dom_str, 0, strlen(domain_str)+1);	
			memcpy(ACLLIST->AclList[j]->dom_str, domain_str, strlen(domain_str));			

			dbus_message_iter_next(&iter_array);
		}

	}

	dbus_message_unref(reply);
	return ACLLIST;
}

#endif
