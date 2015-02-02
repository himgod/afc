#ifndef _CHECK_ARGUMENT_H
#define _CHECK_ARGUMENT_H
#include "wcpss/asd/asd.h"

#define SSID_LEN 32

#define LOOPBACK(x)	(((x) & htonl(0xff000000)) == htonl(0x7f000000))
#define MULTICAST(x)	(((x) & htonl(0xf0000000)) == htonl(0xe0000000))
#define BADCLASS(x)	(((x) & htonl(0xf0000000)) == htonl(0xf0000000))
#define ZERONET(x)	(((x) & htonl(0xff000000)) == htonl(0x00000000))


#define SPE_IP_ADDR(s) (MULTICAST((s)) || BADCLASS((s)) || \
					LOOPBACK((s)) || ZERONET((s))) 	
					

int ssid_illegal_character_check(char *str, int len);

int wpa_key_illegal_check(char *key, int keyType);


#endif
