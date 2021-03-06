#ifndef _HMD_DBUS_DEF_H
#define _HMD_DBUS_DEF_H

#ifndef PATH_LEN
#define PATH_LEN 64
#endif

enum hmd_dbus_result_no_e {
	HMD_DBUS_SUCCESS,
	HMD_DBUS_ERROR, /* general use, no detail error information*/
	HMD_DBUS_ID_NO_EXIST,
	HMD_DBUS_UNKNOWN_ID_FORMAT,
	HMD_DBUS_LOCAL_HANSI_ID_NOT_EXIST,
	HMD_DBUS_HANSI_ID_NOT_EXIST,	
	HMD_DBUS_SLOT_ID_NOT_EXIST,
	HMD_DBUS_NUM_OVER_MAX,
	HMD_DBUS_WARNNING,
	HMD_DBUS_NOCONFIG_UPLINK,
	HMD_DBUS_NOCONFIG_DOWNLINK,
	HMD_DBUS_COMMAND_NOT_SUPPORT,
	HMD_DBUS_LICENSE_TYPE_NOT_EXIST,	
	HMD_DBUS_LICENSE_NUM_NOT_ENOUGH,
	HMD_DBUS_PERMISSION_DENIAL,
	HMD_DBUS_FEMTO_SERVICE_CONFLICT,
	HMD_DBUS_FEMTO_SERVICE_ERROR,
	HMD_DBUS_IU_NOT_IN_SERVICE,
	HMD_DBUS_IUH_NOT_IN_SERVICE,
	HMD_DBUS_INVALID_SLOT_NUM,
	HMD_DBUS_TIMER_CONFIG_SAVE_STATE_ENABLE,  //fengwenchao add 20130412 for hmd timer config save 
	HMD_DBUS_IS_NOT_MASTER, //fengwenchao add 20130412 for hmd timer config save 
	HMD_DBUS_SET_NUM_MORE_THAN_SPECEFICATION,  //fengwenchao add for read gMaxWTPs from  /dbm/local_board/board_ap_max_counter
	HMD_DBUS_DELETING_HANSI,
	HMD_DBUS_DHCP_RESTART_ALREADY_ENABLE, //DHCP RESTART already open supf add 20130730
	HMD_DBUS_DHCP_RESTART_ALREADY_DISABLE
};
#define DCLI_HMD_CHECK_FAILED	(-1)
#define DCLI_HMD_CREATED		(1)		/* HMD have created.		*/
#define DCLI_HMD_NO_CREATED	(0)		/* HMD have not created.	*/


#endif
