#ifndef _WAI_PUBLIC_H_
#define _WAI_PUBLIC_H_
/*
  * WAI - WiFi Assistant Initiative
  * 	WAI is simplified as Y for the similar pronounciation
  * 	minor Y is the product of machine intelligence
  */
#define WAI_DEBUG_ENABLE	1 	/* should be off in release version */
#if WAI_DEBUG_ENABLE
#define WAI_FUNC_LINE_FMT		"%s-%d "
#define WAI_FUNC_LINE_LITERAL	__func__,__LINE__
#else
#define WAI_FUNC_LINE_FMT		"%s"
#define WAI_FUNC_LINE_LITERAL 	""
#endif
#define WAI_MAC_FMT	"%02x:%02x:%02x:%02x:%02x:%02x "
#define WAI_MAC_LITERAL(a)	a[0],a[1],a[2],a[3],a[4],a[5]
#define WAI_IP_FMT "%u.%u.%u.%u "
#define WAI_IP_LITERAL(a)	  ((a) >>24)&0xFF,((a) >>16)&0xFF,((a) >>8)&0xFF, (a) &0xFF
#define WAI_BOOT_FMT  "loading "
#define WAI_AFI_FMT	"afi%u %s "
#define WAI_AFI0_FMT	"afi%u "
#define WAI_GENERAL_FMT	"wai"
#define WAI_WLAN_FMT 	"wlan%u "
#define WAI_USERGRP_FMT 	"usergrp%u "
#define WAI_DISCOVERY_LITERAL	"discovery "
#define WAI_AFI_LOGIN_PHASE_FMT	 "%s-%s "
#define WAI_AFI_LOGIN_P1_LITERAL  "pkt incoming "
#define WAI_DBUS_FMT  "dbus"
#define WAI_SECONDS_ONE_MINUTE	(60)
#define WAI_MINUTES_ONE_HOUR (60)
#define WAI_HOURS_ONE_DAY  (24)
#define WAI_SECONDS_ONE_DAY	(WAI_HOURS_ONE_DAY*WAI_MINUTES_ONE_HOUR*WAI_SECONDS_ONE_MINUTE)
#define WAI_SECONDS_ONE_HOUR	(WAI_MINUTES_ONE_HOUR*WAI_SECONDS_ONE_MINUTE)
#define WAI_TIME_DAY(t)		((t)/WAI_SECONDS_ONE_DAY)
#define WAI_TIME_HOUR_INDAY(t)	(((t)%WAI_SECONDS_ONE_DAY)/WAI_SECONDS_ONE_HOUR)
#define WAI_TIME_MINUTE_INHOUR(t) (((t)%WAI_SECONDS_ONE_HOUR)/WAI_SECONDS_ONE_MINUTE)
#define WAI_TIME_SECOND_INMINUTE(t)	((t)%WAI_SECONDS_ONE_MINUTE)

#endif
