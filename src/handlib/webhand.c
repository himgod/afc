#include<stdio.h>
#include<string.h>
#include <dbus/dbus.h>

#include "wid_ac.h"



int main()
{
    DBusError dbus_error;

    DBusConnection *dcli_dbus_connection = NULL;

    WLANWEBINFO webInfo;

    memset(&webInfo, 0 , sizeof(WLANWEBINFO));

    webInfo.WlanId = 1;

    dbus_error_init (&dbus_error);
    dcli_dbus_connection = dbus_bus_get (DBUS_BUS_SYSTEM, &dbus_error);
    if (dcli_dbus_connection == NULL) {
        printf ("dbus_bus_get(): %s", dbus_error.message);
        return FALSE;
    }

    dbus_bus_request_name(dcli_dbus_connection,"aw.new",0,&dbus_error);

    hand_add_softAc_network("SUNTEST", "12345678", 3, 0,  0 , 1, 0, 0, 0, 2, 0, 0, dcli_dbus_connection);

    show_network_wlan_config(&webInfo, dcli_dbus_connection);

    return 0 ;
}
