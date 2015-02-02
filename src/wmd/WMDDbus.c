#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <pthread.h>
#include <stdbool.h>

#include "WMDHorstNetwork.h"
//#include "WMDThread.h"
#include "WMDDbus.h"
#include "WMDDealFunc.h"

static DBusConnection *wmd_dbus_connection = NULL;


static DBusMessage *wmd_dbus_interface_get_aps_info(DBusConnection *conn, DBusMessage *msg, void *user_data);
DBusHandlerResult wmd_dbus_filter_function (DBusConnection * connection, DBusMessage * message, void *user_data);





static DBusHandlerResult wmd_dbus_message_handler (DBusConnection *connection, DBusMessage *message, void *user_data)
{
	DBusMessage	*reply = NULL;
	char sender[20];
	//fprintf(stdout, "wmd_dbus_message_handler: helloworld!\n");
	DBusDispatchStatus IGNORE_UBSV dstatus;
	dstatus = dbus_connection_get_dispatch_status (connection);
	
	if(NULL == message) {
		fprintf(stdout, "dbus_connection_get_dispatch_status error!\n");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}
	if	(strcmp(dbus_message_get_path(message),WMD_DBUS_OBJPATH) == 0)
	{
		if (dbus_message_is_method_call(message, WMD_DBUS_INTERFACE, WMD_DBUS_CONF_METHOD_GET_APS_INFO))
		{
			reply = wmd_dbus_interface_get_aps_info(connection, message, user_data);
		}
	}
	if (reply) {
		//wid_syslog_debug_debug(WID_DBUS,"reply destination %s\n",dbus_message_get_destination(reply));
		fprintf(stdout, "reply destination %s\n", dbus_message_get_destination(reply));
		strcpy(sender,dbus_message_get_destination(reply));
		dbus_connection_send (connection, reply, NULL);
		dbus_connection_flush(connection); // TODO    Maybe we should let main loop process the flush
		dbus_message_unref (reply);
	}

//	dbus_message_unref(message); //TODO who should unref the incoming message? 
	return DBUS_HANDLER_RESULT_HANDLED ;
}


int wmd_dbus_reinit(void)
{	
	int i = 0;
	DBusError dbus_error;
	dbus_threads_init_default();
	DBusObjectPathVTable	wid_vtable = {NULL, &wmd_dbus_message_handler, NULL, NULL, NULL, NULL};	

	dbus_connection_set_change_sigpipe (TRUE);

	dbus_error_init (&dbus_error);
	
	wmd_dbus_connection = dbus_bus_get_private (DBUS_BUS_SYSTEM, &dbus_error);
	if (wmd_dbus_connection == NULL) {
		//wid_syslog_err("dbus_bus_get(): %s\n", dbus_error.message);
		fprintf(stderr, "dbus_bus_get(): %s\n", dbus_error.message);
		return FALSE;
	}
	
	// Use npd to handle subsection of NPD_DBUS_OBJPATH including slots
	if (!dbus_connection_register_fallback (wmd_dbus_connection, WMD_DBUS_OBJPATH, &wid_vtable, NULL)) {
		//wid_syslog_err("can't register D-BUS handlers (fallback NPD). cannot continue.\n");
		return FALSE;
		
	}
	
	i = dbus_bus_request_name (wmd_dbus_connection, WMD_DBUS_BUSNAME,
			       0, &dbus_error);
		
	//wid_syslog_debug_debug(WID_DBUS,"dbus_bus_request_name:%d",i);
	fprintf(stdout, "dbus_bus_request_name:%d", i);
	
	if (dbus_error_is_set (&dbus_error)) {
		//wid_syslog_debug_debug(WID_DBUS,"dbus_bus_request_name(): %s",
		//	    dbus_error.message);
		fprintf(stderr, "dbus_bus_request_name() : %s\n", dbus_error.message);
		return FALSE;
	}

	dbus_connection_add_filter (wmd_dbus_connection, wmd_dbus_filter_function, NULL, NULL);

	dbus_bus_add_match (wmd_dbus_connection,
			    "type='signal'"
					    ",interface='"DBUS_INTERFACE_DBUS"'"
					    ",sender='"DBUS_SERVICE_DBUS"'"
					    ",member='NameOwnerChanged'",
			    NULL);
	
	return TRUE;
  
}

int wmd_dbus_init(void)
{	
	int IGNORE_UBSV i = 0;
	DBusError dbus_error;
	dbus_threads_init_default();
	DBusObjectPathVTable	wmd_vtable = {NULL, &wmd_dbus_message_handler, NULL, NULL, NULL, NULL};	
	
	dbus_connection_set_change_sigpipe (TRUE);
	
	dbus_error_init (&dbus_error);

	wmd_dbus_connection = dbus_bus_get_private (DBUS_BUS_SYSTEM, &dbus_error);
	if (wmd_dbus_connection == NULL) {
		//wid_syslog_err("dbus_bus_get(): %s\n", dbus_error.message);
		fprintf(stderr, "dbus_bus_get(): %s\n", dbus_error.message);
		return FALSE;
	}

	// Use npd to handle subsection of NPD_DBUS_OBJPATH including slots
	if (!dbus_connection_register_fallback (wmd_dbus_connection, WMD_DBUS_OBJPATH, &wmd_vtable, NULL)) {
		//wid_syslog_err("can't register D-BUS handlers (fallback NPD). cannot continue.\n");
		fprintf(stderr, "can't register D-BUS handlers (fallback NPD). cannot continue.\n");
		return FALSE;
		
	}
	
	i = dbus_bus_request_name (wmd_dbus_connection, WMD_DBUS_BUSNAME,
			       0, &dbus_error);

	if (dbus_error_is_set (&dbus_error)) {
		//wid_syslog_debug_debug(WID_DBUS,"dbus_bus_request_name(): %s",
		//	    dbus_error.message);
		fprintf(stderr, "dbus_bus_request_name(): %s", dbus_error.message);
		return FALSE;
	}

	dbus_connection_add_filter (wmd_dbus_connection, wmd_dbus_filter_function, NULL, NULL);
	fprintf(stdout, "wmd_dbus_init: shanghai!\n");
	dbus_bus_add_match (wmd_dbus_connection,
			    "type='signal'"
					    ",interface='"DBUS_INTERFACE_DBUS"'"
					    ",sender='"DBUS_SERVICE_DBUS"'"
					    ",member='NameOwnerChanged'",
			    NULL);
	fprintf(stdout, "wmd_dbus_init: OK~~~!\n");
	return TRUE; 
}

void *wmd_dbus_thread()
{
	//int loop_count = 0;
	/*
	For all OAM method call, synchronous is necessary.
	Only signal/event could be asynchronous, it could be sent in other thread.
	*/
	//printf("begin\n");
	//wid_pid_write_v2("wid_dbus_thread",0,vrrid);
	
	//wid_thread_log("dbus");
	if(wmd_dbus_init()) {
		//fprintf(stdout, "wmd_dbus_thread: dbus init OK!\n");
		while (dbus_connection_read_write_dispatch(wmd_dbus_connection, 500)) {
			//wid_syslog_debug_debug("entering main loop.\n");
			//wid_syslog_debug_debug("loop %d.\n",loop_count++);
		}
	}
	//wid_syslog_err("there is something wrong in dbus handler\n");	
	fprintf(stdout, "wmd_dbus_thread: there is something wrong in dbus handler\n");

	return 0;
}

void *wmd_dbus_thread_restart()
{
	//int loop_count = 0;
	/*
	For all OAM method call, synchronous is necessary.
	Only signal/event could be asynchronous, it could be sent in other thread.
	*/
	//printf("begin\n");
	//wid_pid_write_v2("wid_dbus_thread_restart",0,vrrid);
	if(wmd_dbus_reinit()){
		while (dbus_connection_read_write_dispatch(wmd_dbus_connection, 500)) {
			fprintf(stdout, "helloworld!~~~~~~~~~~~\n");
			//wid_syslog_debug_debug(WID_DBUS,"entering main loop.\n");
			//wid_syslog_debug_debug(WID_DBUS,"loop %d.\n",loop_count++);
		}
	}
	//wid_syslog_err("there is something wrong in dbus handler\n");	
	return 0;
}


DBusHandlerResult
wmd_dbus_filter_function (DBusConnection * connection, DBusMessage * message, void *user_data)
{
//	printf("entering filter.\n");
	if (dbus_message_is_signal (message, DBUS_INTERFACE_LOCAL, "Disconnected") &&
		   strcmp (dbus_message_get_path (message), DBUS_PATH_LOCAL) == 0) {

		/* this is a local message; e.g. from libdbus in this process */

		//wid_syslog_err("Got disconnected from the system message bus; "
		//		"retrying to reconnect every 3000 ms\n");
		dbus_connection_close (wmd_dbus_connection);
		wmd_dbus_connection = NULL;
		pthread_t thread_dbus; 
		if(pthread_create(&thread_dbus, NULL, wmd_dbus_thread_restart, NULL)) {
			//wid_syslog_crit("Error starting Dbus Thread");
			exit(1);
		}

		//g_timeout_add (3000, reinit_dbus, NULL);

	} else if (dbus_message_is_signal (message,
			      DBUS_INTERFACE_DBUS,
			      "NameOwnerChanged")) {

		//if (services_with_locks != NULL)  service_deleted (message);
	} else {
		return TRUE;
	}
		//return hald_dbus_filter_handle_methods (connection, message, user_data, FALSE);

	return DBUS_HANDLER_RESULT_HANDLED;
}


extern CLIENT_INFO_OBJ horst_client[FD_MAX_SIZE];  //FD_MAX_SIZE
static DBusMessage *wmd_dbus_interface_get_aps_info(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply;	
	DBusMessageIter	 iter;
	DBusMessageIter	 iter_array;
	DBusMessageIter iter_struct;
	DBusMessageIter	 iter_sub_array;
	DBusMessageIter iter_sub_struct;
	DBusError err;
	dbus_error_init(&err);
	unsigned char mac[MAC_LEN];
	unsigned char tmp_mac[MAC_LEN] = { 0 };
	
	int i, j;
	int ap_cnt = 0;
	
	int ap_num = get_ap_nums(horst_client, FD_MAX_SIZE);
	reply = dbus_message_new_method_return(msg);
		
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32, &ap_num);
	
	dbus_message_iter_open_container (&iter,
							DBUS_TYPE_ARRAY,
							DBUS_STRUCT_BEGIN_CHAR_AS_STRING
									DBUS_TYPE_BYTE_AS_STRING		 //mac 0
									DBUS_TYPE_BYTE_AS_STRING
									DBUS_TYPE_BYTE_AS_STRING
									DBUS_TYPE_BYTE_AS_STRING
									DBUS_TYPE_BYTE_AS_STRING
									DBUS_TYPE_BYTE_AS_STRING		//mac 5
									DBUS_TYPE_BYTE_AS_STRING        //radio type
									DBUS_TYPE_BYTE_AS_STRING        //channel
									DBUS_TYPE_UINT32_AS_STRING		//neighbor_ap_count
										DBUS_TYPE_ARRAY_AS_STRING
											DBUS_STRUCT_BEGIN_CHAR_AS_STRING
												DBUS_TYPE_BYTE_AS_STRING //mac	0
												DBUS_TYPE_BYTE_AS_STRING //mac
												DBUS_TYPE_BYTE_AS_STRING //mac
												DBUS_TYPE_BYTE_AS_STRING //mac
												DBUS_TYPE_BYTE_AS_STRING //mac
												DBUS_TYPE_BYTE_AS_STRING //mac	5
												
												DBUS_TYPE_BYTE_AS_STRING //neighbor flag	
												DBUS_TYPE_BYTE_AS_STRING //channel		
												DBUS_TYPE_BYTE_AS_STRING //rssi
											DBUS_STRUCT_END_CHAR_AS_STRING
							DBUS_STRUCT_END_CHAR_AS_STRING,
							&iter_array);
	for (i = 0; i < FD_MAX_SIZE && ap_cnt < ap_num; i++) {
		if (horst_client[i].fd < 0)
			continue;
		ap_cnt++;
		dbus_message_iter_open_container (&iter_array,DBUS_TYPE_STRUCT, NULL, &iter_struct);
		memcpy(mac, horst_client[i].ap_mac, MAC_LEN);
		unsigned char radio_type = horst_client[i].type;
		int neighbor_ap_cnt = get_valid_neighbors_nums(horst_client[i]);
		fprintf(stdout, "valid---->neighbor_ap_cnt = %d\n", neighbor_ap_cnt);
		
		int k;
		fprintf(stdout, "\tNO.%d\t", i+1);
		for (k = 0; k < MAC_LEN-1; k++) {
			fprintf(stdout, "%02x:", mac[k]);
		}
		fprintf(stdout, "%02x\n", mac[k]);


		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(mac[0]));
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(mac[1]));
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(mac[2]));
		
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(mac[3]));
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(mac[4]));
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(mac[5]));
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&radio_type);
		dbus_message_iter_append_basic(&iter_struct,
										DBUS_TYPE_BYTE,
										&(horst_client[i].channel));
		dbus_message_iter_append_basic (&iter_struct,
										 DBUS_TYPE_UINT32,
										 &neighbor_ap_cnt);
		
		dbus_message_iter_open_container (&iter_struct,
				   		DBUS_TYPE_ARRAY,
								DBUS_STRUCT_BEGIN_CHAR_AS_STRING
									DBUS_TYPE_BYTE_AS_STRING //mac	0
									DBUS_TYPE_BYTE_AS_STRING //mac
									DBUS_TYPE_BYTE_AS_STRING //mac
									DBUS_TYPE_BYTE_AS_STRING //mac
									DBUS_TYPE_BYTE_AS_STRING //mac
									DBUS_TYPE_BYTE_AS_STRING //mac	5
									
									DBUS_TYPE_BYTE_AS_STRING //neighbor flag 1:by local AC 0:by other AC
									DBUS_TYPE_BYTE_AS_STRING //channel													
									DBUS_TYPE_BYTE_AS_STRING //rssi
								DBUS_STRUCT_END_CHAR_AS_STRING,
							   &iter_sub_array);
		
		NEIGHBOR_LIST_HANDLE ptmp = horst_client[i].neighbor_link_head;
		int cnt = 0;
		for (j = 0; (j < horst_client[i].neighbor_cnt)&& cnt < neighbor_ap_cnt && (NULL != ptmp); j++) {
			if (0 == memcmp(ptmp->mac, tmp_mac, MAC_LEN)) {
				ptmp = ptmp->next;
				continue;
			}
			if (0 == horst_client[i].type && ptmp->channel > 13) {
				ptmp = ptmp->next;
				continue;
			}
			if (1 == horst_client[i].type && ptmp->channel <= 14) {
				ptmp = ptmp->next;
				continue;
			}
			
			dbus_message_iter_open_container (&iter_sub_array, DBUS_TYPE_STRUCT, 
				NULL, &iter_sub_struct);
			memcpy(mac, ptmp->mac, MAC_LEN);
			char neighbor_flag = check_neighbor_flag(horst_client, ap_num, ptmp);

			//if (neighbor_flag >= 0) {
				fprintf(stdout, "NeighborFlag = %d : \t", neighbor_flag);
				fprintf(stdout, "NO.%d\t", j+1);
				int idx;
				for (idx = 0; idx < MAC_LEN-1; idx++) {
					fprintf(stdout, "%02x:", mac[idx]);
				}
				fprintf(stdout, "%02x\n", mac[idx]);
			//}

				
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(mac[0]));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(mac[1]));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(mac[2]));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(mac[3]));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(mac[4]));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(mac[5]));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &neighbor_flag);
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(ptmp->channel));
			dbus_message_iter_append_basic(&iter_sub_struct,
											  DBUS_TYPE_BYTE,
											  &(ptmp->rssi));
			ptmp = ptmp->next;
			cnt++;
			dbus_message_iter_close_container (&iter_sub_array, &iter_sub_struct);
		}
		dbus_message_iter_close_container (&iter_struct, &iter_sub_array);
		dbus_message_iter_close_container (&iter_array, &iter_struct);
		
	}
	dbus_message_iter_close_container (&iter, &iter_array);
	for (i = 0; i < ap_num; i++) {
		destroy_neighbor_list(&horst_client[i]);
	}
	return reply;
}
