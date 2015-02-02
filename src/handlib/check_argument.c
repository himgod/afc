#include<string.h>
#include"check_argument.h"

#include "dbus/wcpss/ACDbusDef1.h"



int ssid_illegal_character_check(char *str, int len)//xiaodawei add for checking ssid illegal character, 20110509
{
	int m = 0;
	char *tmp = NULL;
	tmp = str;
	if (!str)
	{
		return WID_SSID_ERROR;
	}
	if (strlen(str) > len)
	{
		return WID_SSID_ERROR;	//-1 means ssid is too long , out of limit 
	}
	for (m = 0; m < len; m++)
	{
		if (tmp[m] >= 32 && tmp[m] <= 126 && tmp[m] != '\\' && tmp[m] != '"' && tmp[m] != '&' && tmp[m] != '`' && tmp[m] != '(' && tmp[m] != ')' &&tmp[m] != '*') /*wcl modify*/
			continue;
		else
			break;
	}
	if (m == len)
	{
		return 0;
	}
	else
	{
		return WID_SSID_ERROR;	//-2 means illegal character
	}
}


int CheckHex(char *p)
{
	int i=0,len=0;
	
	if(p==NULL)
		return -1;

	len=strlen(p);
	
	for(i=0;i<len;i++){
		if(!((*p>='0'&&*p<='9')||(*p>='a'&&*p<='f')||(*p>='A'&&*p<='F')))
			return -1;
		p++;
	}
	return 0;
			
}


int wpa_key_illegal_check(char *key, int keyType)
{	
	int len = 0;
	
	if(!key)
	{
		return WID_SECURITY_WPA_KEY_ERROR;
	}

	len = strlen(key);
	
	switch(keyType)
	{
		case ASCII :
		{
		 	if((len < 8)||(len > 63))
				return WID_SECURITY_WPA_KEY_ERROR;
			break;
		}
		case HEX :
		{
			if(len != 64 || 0 != CheckHex(key))
			{
				return WID_SECURITY_WPA_KEY_ERROR;
			}

			break;
		}
	}

	
	return 0;
}

int wep_key_illegal_check(char *key, int keyType)
{
	
	int len = 0;
		
	if(!key)
	{
		return WID_SECURITY_WEP_KEY_ERROR;
	}
	len = strlen(key);


	switch(keyType)
	{
		case ASCII :
		{
		 	if((len != 5)&&(len != 13)&&(len != 16))
				return WID_SECURITY_WEP_KEY_ERROR;
			break;
		}
		case HEX :
		{
			if(0!=CheckHex(key))
				return WID_SECURITY_WEP_KEY_ERROR;
			else if((len != 10)&&(len != 26)&&(len != 32))
				return WID_SECURITY_WEP_KEY_ERROR;
			break;
		}
	}
	return 0;
}

int check_user_vlanid(int vlanid)
{
	if(vlanid < 0 || vlanid > 4096)
	{
		return 1;
	}
	return 0;
}
