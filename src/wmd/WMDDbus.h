#ifndef _WMDDBUS_H
#define _WMDDBUS_H

#define WMD_DBUS_BUSNAME	"aw.wmd"
#define WMD_DBUS_OBJPATH	"/aw/wmd"
#define WMD_DBUS_INTERFACE	"aw.wmd"

#define IGNORE_UBSV	__attribute__((unused))

#define    PATH_LEN (64)


char WMD_DBUS_CONF_METHOD_GET_APS_INFO[PATH_LEN] =       "get_aps_list_info";

int wmd_dbus_init(void);
void *wmd_dbus_thread();

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#endif