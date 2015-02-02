#ifndef _WID_MACGRP_H
#define _WID_MACGRP_H

void dcli_macgrp_init(void);
DCLI_MACGRP_API_GROUP *show_macgroup_id(DBusConnection *dcli_dbus_connection,int index,int localid, unsigned int macgroupID, unsigned int *ret);
void dcli_macgrp_free_fun(DCLI_MACGRP_API_GROUP *MACGRPINFO);
int dcli_macgrp_add_mac_node(WID_MAC_GROUP **GROUP, MAC_list *list);



#endif


