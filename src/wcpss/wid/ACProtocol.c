/*******************************************************************************
Copyright (C) Autelan Technology


This software file is owned and distributed by Autelan Technology 
********************************************************************************


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
* ACProtocol.c
*
*
* CREATOR:
* autelan.software.wireless-control. team
*
* DESCRIPTION:
* wid module
*
*
*******************************************************************************/
#include "wcpss/waw.h"

#include "wcpss/wid/WID.h"
#include "CWAC.h"
#include "ACDbus_handler.h"
#include "AC.h"
#include "os.h"
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

//added by weiay 20080618
CWBool CWAssembleMsgElemWTPVersion(CWProtocolMessage *msgPtr,char *version)
{

	//printf("********* CWAssembleMsgElemWTPVersion *******\n");
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if(version == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	
	char *str_Version = version;
	unsigned int usize = 8 + strlen(str_Version);
	//printf("*** version is:<%s> size is:<%d>****\n",str_Version,strlen(str_Version));
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, usize, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, 1);//AC Surport version count
	CWProtocolStore8(msgPtr, 0);//reserved
	CWProtocolStore16(msgPtr, gCWImageDataPendingTimer);//reserved

	CWProtocolStore16(msgPtr, 1000);//AC Surport wtp type
	CWProtocolStore16(msgPtr, strlen(str_Version)); 
	CWProtocolStoreStr(msgPtr, str_Version); 
		
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE);

}

CWBool CWAssembleMsgElemWTPContainMAC(CWProtocolMessage *msgPtr)
{
	short int elementid = CW_VENDOR_SPEC_ELEMENT_WTP_CONTAIN_MAC; 
	short int length = 5;
	unsigned char iscontained = 1;
	
	if(msgPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	

	CWProtocolStore8(msgPtr, iscontained);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}


/*____________________________________________________________________________*/
/*  *****************************___ASSEMBLE___*****************************  */
CWBool CWAssembleMsgElemACDescriptor(CWProtocolMessage *msgPtr) {
	CWACVendorInfos infos;
	int i=0, size=0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);;
	
	if(!CWACGetVendorInfos(&infos)) { // get infos
		return CW_FALSE;
	}

	for(i = 0; i < infos.vendorInfosCount; i++) {
		size += (8 + ((infos.vendorInfos)[i]).length);
	}
	
	size += 12; // size of message in bytes (excluding vendor infos, already counted)
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, CWACGetStations()); // Number of mobile stations associated
	CWProtocolStore16(msgPtr, CWACGetLimit()); // Maximum number of mobile stations supported	
	CWProtocolStore16(msgPtr, CWACGetActiveWTPs()); // Number of WTPs active	
	CWProtocolStore16(msgPtr, CWACGetMaxWTPs()); // Maximum number of WTPs supported	
	CWProtocolStore8(msgPtr, CWACGetSecurity());
	CWProtocolStore8(msgPtr, CWACGetRMACField());
	CWProtocolStore8(msgPtr, 0);			//Reserved
	CWProtocolStore8(msgPtr, CWACGetDTLSPolicy()); 	// DTLS Policy
	
//	wid_syslog_debug_debug("Vendor Count: %d", infos.vendorInfosCount);

	for(i=0; i<infos.vendorInfosCount; i++) {
		CWProtocolStore32(msgPtr, ((infos.vendorInfos)[i].vendorIdentifier));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].type));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].length));
		if((infos.vendorInfos)[i].length == 4) {
			*((infos.vendorInfos)[i].valuePtr) = htonl(*((infos.vendorInfos)[i].valuePtr));
		}
		CWProtocolStoreRawBytes(msgPtr, (char*) ((infos.vendorInfos)[i].valuePtr), (infos.vendorInfos)[i].length);
	}
	
	CWACDestroyVendorInfos(&infos);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE);
}

CWBool CWAssembleMsgElemACIPv4List(CWProtocolMessage *msgPtr) 
{
	int *list = NULL;
	int count = 0, i = 0;
	const int IPv4_List_length=4;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!CWACGetACIPv4List(&list, &count)) return CW_FALSE;
	
	// create message
	/* coverity-CID: 10865  Resource leak */
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, IPv4_List_length*count, {CW_FREE_OBJECT(list);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < count; i++) {
		CWProtocolStore32(msgPtr, list[i]);
//		wid_syslog_debug_debug("AC IPv4 List(%d): %d", i, list[i]);
	}
	
	CW_FREE_OBJECT(list);
				
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE);
}

CWBool CWAssembleMsgElemACIPv6List (CWProtocolMessage *msgPtr) 
{
	struct in6_addr *list = NULL;
	const int IPv6_List_length=16;
	int count = 0, i = 0;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!CWACGetACIPv6List(&list, &count)) return CW_FALSE;
	
	// create message
	/* coverity-CID: 10866  Resource leak */
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, IPv6_List_length*count, {CW_FREE_OBJECT(list);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	/*--- ATTENZIONE! l'indirizzo ipv6 forse deve essere girato ---*/
	for(i = 0; i < count; i++) {
		CWProtocolStoreRawBytes(msgPtr, (char*)list[i].s6_addr, 16);
	}
	
	CW_FREE_OBJECT(list);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE);
}

CWBool CWAssembleMsgElemACName(CWProtocolMessage *msgPtr)
{
	char *name = NULL;
	if(msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	name = CWACGetName();
	if(NULL == name)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	}
	
	// create message
    /* coverity-CID: 10301 Out-of-bounds access */
    /* coverity-CID: 10342 Allocation too small for string */
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(name)+1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreStr(msgPtr, name);
	
//	wid_syslog_debug_debug("AC Name: %s", name);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_AC_NAME_CW_TYPE);
}
/*
CWBool CWAssembleMsgElemCWControlIPv4Addresses(CWProtocolMessage *msgPtr) {
	int count, i;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	count = CWACGetInterfacesCount();
	
	if(count <= 0) {
		return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "No Interfaces Configured");
	}
	
	for(i = 0; i < count; i++) { // one Message Element for each interface
		CWProtocolMessage temp;
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(temp, 6, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStore32(&temp, CWACGetInterfaceIPv4AddressAtIndex(i));
		CWProtocolStore16(&temp, CWACGetInterfaceWTPCountAtIndex(i));
		
		CWAssembleMsgElem(&temp, CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE);
		
		if(i == 0) {
			CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, (temp.offset)*count, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		}
		
		CWProtocolStoreMessage(msgPtr, &temp);
		CW_FREE_PROTOCOL_MESSAGE(temp);
	}
	
	return CW_TRUE;
}
*/
CWBool CWAssembleMsgElemCWControlIPv4Addresses(CWProtocolMessage *msgPtr, unsigned int WTPID)
{
	int count = 0, i = 0;
	int j = 0; 
	if(msgPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if(AC_WTP[WTPID] != NULL)
	{
		CWThreadMutexLock(&ACIPLISTMutex);
		for(j = 0; j < ACIPLIST_NUM; j++)
		{
			if((AC_IP_GROUP[j] != NULL)&&(strcmp(AC_WTP[WTPID]->BindingIFName, (char*)AC_IP_GROUP[j]->ifname) == 0))
			{
				break;
			}
		}
		if((j != ACIPLIST_NUM)&&(AC_IP_GROUP[j]->ipnum != 0))
		{
			count = AC_IP_GROUP[j]->ipnum;
			struct wid_ac_ip *tmp = AC_IP_GROUP[j]->ip_list;
			for(i = 0; i < count; i++) // one Message Element for each interface
			{
				CWProtocolMessage temp;
				// create message
				CW_CREATE_PROTOCOL_MESSAGE(temp, 6, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				
				CWProtocolStore32(&temp, inet_addr(tmp->ip));
				CWProtocolStore16(&temp, 9999);
				
				CWAssembleMsgElem(&temp, CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE);
				
				if(i == 0)
				{
					CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, (temp.offset)*count, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				}
				
				CWProtocolStoreMessage(msgPtr, &temp);
				CW_FREE_PROTOCOL_MESSAGE(temp);
				tmp = tmp->next;
			}			
			CWThreadMutexUnlock(&ACIPLISTMutex);
			return CW_TRUE;
		}		
		CWThreadMutexUnlock(&ACIPLISTMutex);
	}
	
	count = CWACGetInterfacesIpv4Count();
	
	if(count <= 0)
	{
		return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "No IPv4 Interfaces Configured");
	}
	
	for(i = 0; i < count; i++)  // one Message Element for each interface
	{
		CWProtocolMessage temp;
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(temp, 6, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStore32(&temp, CWACGetInterfaceIPv4AddressAtIndex(i));
		CWProtocolStore16(&temp, CWACGetInterfaceWTPCountAtIndex(i));
		
		CWAssembleMsgElem(&temp, CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE);
		
		if(i == 0)
		{
			CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, (temp.offset)*count, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		}
		
		CWProtocolStoreMessage(msgPtr, &temp);
		CW_FREE_PROTOCOL_MESSAGE(temp);
	}
	
	return CW_TRUE;
}

CWBool CWAssembleMsgElemCWControlIPv6Addresses(CWProtocolMessage *msgPtr) {
	int count, i;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	count = CWACGetInterfacesIpv6Count();
	if(count <= 0)
	{
		wid_syslog_err("%s: No IPv6 Interfaces Configured",__func__);
	}
	for(i = 0; i < count; i++) { // one Message Element for each interface
		CWProtocolMessage temp;
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(temp, 18, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStoreRawBytes(&temp, CWACGetInterfaceIPv6AddressAtIndex(i), 16);
		CWProtocolStore16(&temp, CWACGetInterfaceWTPCountAtIndex(i));
		
		CWAssembleMsgElem(&temp, CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE);
		
		if(i == 0) {
			CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, (temp.offset)*count, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		}
		
		CWProtocolStoreMessage(msgPtr, &temp);
		CW_FREE_PROTOCOL_MESSAGE(temp);
	}
	
	return CW_TRUE;
}

CWBool CWAssembleMsgElemCWTimer (CWProtocolMessage *msgPtr, int WTPID) 
{
	int discoveryTimer, echoTimer;
	const int CWTimer_length=2;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, CWTimer_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if(!(CWACGetDiscoveryTimer(&discoveryTimer)) || !(CWACGetEchoRequestTimer(&echoTimer))) return CW_FALSE;
	echoTimer = AC_WTP[WTPID]->EchoTimer;
	CWProtocolStore8(msgPtr, discoveryTimer);
	CWProtocolStore8(msgPtr, echoTimer);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_CW_TIMERS_CW_TYPE);
}

/* Le informazioni sui Radio ID vengono prese dalle informazioni del Configure Message 
   Provvisoriamente l'error Report Period 猫 settato allo stesso valore per tutte le radio del WTP*/
CWBool CWAssembleMsgElemDecryptErrorReportPeriod (CWProtocolMessage *msgPtr, int WTPID)
{
	const int radio_Decrypt_Error_Report_Period_Length=3;
	const int reportInterval=15;
	CWProtocolMessage *msgs = NULL;
	CWRadioAdminInfoValues *radiosInfoPtr = NULL;
	int radioCount=0;
	int iPtr = 0;
	int len = 0;
	int i = 0;
	int j = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	/*if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL) {
		wid_syslog_crit("Critical Error... closing thread");
		CWCloseThread();
	}*/
	iPtr = WTPID;
	radiosInfoPtr=gWTPs[iPtr].WTPProtocolManager.radioAdminInfo.radios;
	radioCount=gWTPs[iPtr].WTPProtocolManager.radioAdminInfo.radiosCount;
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, radioCount, return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for (i=0; i<radioCount; i++)
	{
		// create message
		/* 1/2 coverity-CID: 10869  Resource leak */
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], radio_Decrypt_Error_Report_Period_Length, {});

		if(NULL == msgs[i].msg)
		{
			for(j = i-1; j >= 0; j--) {
				CW_FREE_PROTOCOL_MESSAGE(msgs[j]);
			}
			CW_FREE_OBJECT(msgs);
			return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
		}
		
		CWProtocolStore8(&(msgs[i]), radiosInfoPtr[i].ID); // ID of the radio
		CWProtocolStore16(&(msgs[i]), reportInterval); // state of the radio
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_PERIOD_CW_TYPE)))
		{
			for(j = i; j >= 0; j--)
			{
				CW_FREE_PROTOCOL_MESSAGE(msgs[j]);
			}
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
//	wid_syslog_debug_debug("Decrypt Error Report Period: %d - %d", radiosInfoPtr[i].ID, reportInterval);
	}
	
	/* 2/2 coverity-CID: 10869  Resource leak */
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, {});
	
	if(NULL == msgPtr->msg)
	{
		for(i = 0; i < radioCount; i++) {
			CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
		}
		CW_FREE_OBJECT(msgs);
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
	}
	
	for(i = 0; i < radioCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);

	return CW_TRUE;
}

CWBool CWAssembleMsgElemIdleTimeout (CWProtocolMessage *msgPtr)
{
	int idleTimeout;
	const int idle_Timeout_length=4;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, idle_Timeout_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if(!(CWACGetIdleTimeout(&idleTimeout))) return CW_FALSE;
	CWProtocolStore32(msgPtr, idleTimeout);
	
	//wid_syslog_debug_debug("Idle Timeout: %d", idleTimeout);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IDLE_TIMEOUT_CW_TYPE);
}


CWBool CWAssembleMsgElemWTPFallback (CWProtocolMessage *msgPtr)
{
	int value=0; //PROVVISORIO
	const int WTP_fallback_length=1;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, WTP_fallback_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	
//	wid_syslog_debug_debug("Fallback: %d", value);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_FALLBACK_CW_TYPE);
}

CWBool CWAssembleMsgElemAPScanningSet(CWProtocolMessage *msgPtr)
{
	//printf("#### CWAssembleMsgElemAPScanningSet ####\n");
	//printf("opstate = %d\n",gapscanset.opstate);
	//printf("reportinterval = %d\n",gapscanset.reportinterval);
	
	int value=0; //ap scanning settting 0
	const int ap_scanset_length = 4;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, ap_scanset_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); //ap sanning setting  0
	CWProtocolStore8(msgPtr, gapscanset.opstate);
	CWProtocolStore16(msgPtr, gapscanset.reportinterval);
		
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool CWAssembleMsgElemAPStatisticsSet(CWProtocolMessage *msgPtr,int apstatics)  //fengwenchao modify 20110422
{
	//printf("#### CWAssembleMsgElemAPStatisticsSet ####\n");
	//printf("apstatistics = %d\n",apstatistics);
	
	int value=1; //ap statistics settting 1
	const int ap_statistics_length = 2;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	unsigned char char_apstatics = 0;
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, ap_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); //ap statistics setting  1
	/*fengwenchao modify begin 20110422*/
	if(apstatics == -1)
	{
		CWProtocolStore8(msgPtr, apstatistics);
	}
	else
	{
		char_apstatics = (unsigned char)apstatics;
		CWProtocolStore8(msgPtr, char_apstatics);
	}
	/*fengwenchao modify end*/		

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool CWAssembleMsgElemAPStatistics_Interval_Set(CWProtocolMessage *msgPtr,unsigned wtpid)
{
	
	int value=1; //ap statistics settting 1
	unsigned char Reserve = 0;
	unsigned short interval = 0;
	const int ap_statistics_length = 2;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	interval = (unsigned short)AC_WTP[wtpid]->apstatisticsinterval;

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, ap_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); //ap statistics setting  1
	CWProtocolStore8(msgPtr, Reserve);
	CWProtocolStore16(msgPtr, interval); 
	CWProtocolStore8(msgPtr, Reserve); 
	CWProtocolStore8(msgPtr, Reserve); 
	CWProtocolStore8(msgPtr, Reserve); 
	CWProtocolStore8(msgPtr, Reserve); 
	wid_syslog_debug_debug(WID_WTPINFO,"%s,%d,apstatistics switch is %s,interval is %d.\n",__func__,__LINE__,(apstatistics != 0)?"enable":"disable",interval);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE);

}
/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,start*/

CWBool CWAssembleMsgElemAP_NTP_Set(CWProtocolMessage *msgPtr,unsigned wtpid)
{
	
	int value=10; //ap ntp settting 1
	unsigned char ipv4_v6 = 0;//default ipv4--0,ipv6--1
	unsigned char state = 0;
	struct in6_addr list;
	unsigned int interval = 0;
	unsigned char Reserve = 0;
	unsigned int ap_ntp_length = 16;
	if(ipv4_v6 == 0)
	{
		ap_ntp_length = 16;
	}
	else
	{
		ap_ntp_length = 76;
	}
	struct sockaddr_in adr_inet;
	unsigned int ip= 0;
	if(msgPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	interval = AC_WTP[wtpid]->ntp_interval;
	int ret = 0;
	wid_syslog_info("wid_set_wtp_ntp wtpid %d.\n",wtpid);
	char *addr = NULL;
	char *ifname = NULL;

	ifname = (char *)malloc(ETH_IF_NAME_LEN+1);
	if(ifname == NULL)
	{
		wid_syslog_info("%s malloc %s",__func__,strerror(errno));
		perror("malloc");
		return CW_FALSE;
	}
	memset(ifname,0,ETH_IF_NAME_LEN+1);
	memcpy(ifname,AC_WTP[wtpid]->BindingIFName,strlen(AC_WTP[wtpid]->BindingIFName));

	struct ifi_info *ifi = (struct ifi_info*)calloc(1, sizeof(struct ifi_info));
	/* By the way modify for coverity-CID: 10867  Resource leak */
	if(!ifi)
	{
		wid_syslog_err("%s line %d ifi malloc failed!\n", __func__, __LINE__);
		CW_FREE_OBJECT(ifname);
		return CW_FALSE;
	}
	memset(ifi->ifi_name,0,sizeof(ifi->ifi_name));
	strncpy(ifi->ifi_name,ifname,sizeof(ifi->ifi_name));
	ret = Get_Interface_Info(ifname,ifi);
	if(ret != 0){
		wid_syslog_err("<err>%s ret=%d.",__func__,ret);
		if(ifi->ifi_addr != NULL){
			free(ifi->ifi_addr);
			ifi->ifi_addr = NULL;
		}		
		if(ifi->ifi_brdaddr != NULL){
			free(ifi->ifi_brdaddr);
			ifi->ifi_brdaddr = NULL;
		}
		free(ifi);
		ifi = NULL;
		CW_FREE_OBJECT(ifname);
		//return WID_DBUS_ERROR;		
	}else{
		addr = (char *)malloc(ETH_IF_NAME_LEN+1);
		if(addr == NULL)
		{
			wid_syslog_info("%s malloc %s",__func__,strerror(errno));
			perror("malloc");
			/* coverity-CID: 10867  Resource leak */
			/* coverity-CID: 10868  Resource leak */
			CW_FREE_OBJECT(ifname);
			CW_FREE_OBJECT(ifi);
			return 0;
		}
		memset(addr,0,ETH_IF_NAME_LEN+1);
		
		wid_syslog_info("netaddr:%x\n",((struct sockaddr_in*)(ifi->ifi_addr))->sin_addr);
		sprintf(addr,"%s",inet_ntoa(((struct sockaddr_in*)(ifi->ifi_addr))->sin_addr));
		
		inet_aton(addr,&adr_inet.sin_addr);
		CW_FREE_OBJECT(ifname);
		CW_FREE_OBJECT(addr);
		/* coverity-CID: 10867  Resource leak */
		CW_FREE_OBJECT(ifi);
	}
	ip = adr_inet.sin_addr.s_addr;
	wid_syslog_info("iP_0x==%x\n",ip);
	state = AC_WTP[wtpid]->ntp_state;
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, ap_ntp_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, value); //ap ntp  1
	CWProtocolStore8(msgPtr, Reserve); //ap statistics setting  1
	CWProtocolStore8(msgPtr, ipv4_v6);
	CWProtocolStore8(msgPtr, state); 
	CWProtocolStore32(msgPtr, interval); 
	if(ipv4_v6==0){
		CWProtocolStore32(msgPtr, ip); 
	}else{
		CWProtocolStoreRawBytes(msgPtr, (char*)list.s6_addr, 16);
	}
	CWProtocolStore8(msgPtr, Reserve); 
	CWProtocolStore8(msgPtr, Reserve); 
	CWProtocolStore8(msgPtr, Reserve); 
	CWProtocolStore8(msgPtr, Reserve); 
	wid_syslog_info("AC_WTP[%d]->ntp_interval=%d,ntp_state=%d.\n",wtpid,AC_WTP[wtpid]->ntp_interval,AC_WTP[wtpid]->ntp_state);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,end*/

CWBool CWAssembleMsgElemAPThroughoutSet(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char value=2; //ap throughout settting 0
	//unsigned char packetlen = 2;
	unsigned char len = 0;
	unsigned char count = 0;
	int i = 0;
	if(AC_WTP[wtpid]->WTP_Radio[0] != NULL)
	{
		for(i=0; i<L_BSS_NUM; i++)
		{
			if(AC_WTP[wtpid]->WTP_Radio[0]->BSS[i] != NULL)
			{
				count++;
			}
		}
	}
	len = count*2+3;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); //ap throughout setting 2
	CWProtocolStore8(msgPtr, AC_WTP[wtpid]->WTP_Radio[0]->bandwidth);
	CWProtocolStore8(msgPtr, count);
	for(i=0; i<L_BSS_NUM; i++)
	{
		if(AC_WTP[wtpid]->WTP_Radio[0]->BSS[i] != NULL)
		{
			CWProtocolStore8(msgPtr, AC_WTP[wtpid]->WTP_Radio[0]->BSS[i]->WlanID);
			CWProtocolStore8(msgPtr, AC_WTP[wtpid]->WTP_Radio[0]->BSS[i]->band_width);
		}
	}
		
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPExtensinCommandSet(CWProtocolMessage *msgPtr,int wtpid,char *command)
{
	unsigned char value=3; //ap throughout settting 0
	unsigned short packetlen = 2 + strlen(command);
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, packetlen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); //ap sanning setting  0
	CWProtocolStore8(msgPtr, (packetlen-2));
	CWProtocolStoreStr(msgPtr, command); 
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool CWAssembleMsgElemAPOption60ParameterSet(CWProtocolMessage *msgPtr,int wtpid,char *parameter)
{
	unsigned short value=28; 
	unsigned short packetlen = 4 + strlen(parameter);
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, packetlen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore16(msgPtr, value); 
	CWProtocolStore16(msgPtr, (packetlen-4));
	CWProtocolStoreStr(msgPtr, parameter); 
	return CWAssembleMsgElemVendorOption60(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

//weichao add 2011.11.03
CWBool CWAssembleMsgElemAPFlowCheck(CWProtocolMessage *msgPtr,unsigned char radioid,unsigned char wlanid,unsigned short flow_check,unsigned int no_flow_time,unsigned int  limit_flow)
{
	
	char value=12; 
	const short int length = 16;
	char reserved = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); 
	CWProtocolStore16(msgPtr, (length-3));
	CWProtocolStore8(msgPtr, reserved); 
	CWProtocolStore8(msgPtr, radioid); 
	CWProtocolStore8(msgPtr, wlanid); 
	CWProtocolStore16(msgPtr, flow_check); 
	CWProtocolStore32(msgPtr, no_flow_time); 
	CWProtocolStore32(msgPtr, limit_flow); 
			
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPSetCPEChannelIntf(CWProtocolMessage *msgPtr, unsigned char op, unsigned short vlanId,unsigned char radioId,unsigned char wlanId)
{
	char  value = 32; 
	unsigned char length = 10;
	
	if(msgPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); 
	CWProtocolStore8(msgPtr, op);
	CWProtocolStore16(msgPtr, vlanId); 
	CWProtocolStore8(msgPtr, radioId); 
	CWProtocolStore8(msgPtr, wlanId); 
	
		
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleMsgElemAPPasswd(CWProtocolMessage *msgPtr,char *username,char*password)
{
	
	char  value=13; 
	char  length = 0;
	char name_length = 0;
	char passwd_length = 0;
	name_length = strlen(username);
	passwd_length =  strlen(password);
	length = 4+name_length+passwd_length;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); 
	CWProtocolStore8(msgPtr, (length-4));
	CWProtocolStore8(msgPtr, name_length); 
	CWProtocolStoreStr(msgPtr,username);
	CWProtocolStore8(msgPtr, passwd_length); 
	CWProtocolStoreStr(msgPtr,password);
			
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool CWAssembleMsgElemAPMultiUserOptimize(CWProtocolMessage *msgPtr,unsigned char wlanid,unsigned char radioid,unsigned char value)
{
	
	unsigned short   length = 13;
	unsigned short elementid = 14;
	unsigned char op_type = 5;
	unsigned char state = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore16(msgPtr, elementid);
	CWProtocolStore16(msgPtr, (length-8)); 
	CWProtocolStore8(msgPtr, op_type); 
	CWProtocolStore8(msgPtr, radioid); 
	CWProtocolStore8(msgPtr, wlanid); 
	CWProtocolStore8(msgPtr, state); 
	CWProtocolStore8(msgPtr,value);
			
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool CWAssembleMsgElemAPnoRespToStaProReq(CWProtocolMessage *msgPtr,MQ_Radio radioinfo)
{
	
	short int elementid = 20; 
	short int length = 8;
	short int policy = 0;
	policy = (short int)radioinfo.id1;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, length-4);
	CWProtocolStore8(msgPtr, radioinfo.Radio_L_ID); 
	CWProtocolStore8(msgPtr, radioinfo.wlanid); 
	CWProtocolStore16(msgPtr, policy); 
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPUniMutiBroCastIsolationSWandRateSet(CWProtocolMessage *msgPtr,MQ_Radio radioinfo)
{
	
	short int elementid = 21; 
	short int length = 8;
//	char reserved = 0;
	short int policy = 0;
	policy = (short int)radioinfo.id_char;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));
	CWProtocolStore8(msgPtr, radioinfo.Radio_L_ID); 
	CWProtocolStore8(msgPtr, radioinfo.wlanid); 
	CWProtocolStore16(msgPtr, policy);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPUniMutiBroCastRateSet(CWProtocolMessage *msgPtr,MQ_Radio radioinfo)
{
	
	short int elementid = 22; 
	short int length = 12;
	//char reserved = 0;
	unsigned char rate = 0;
	
	rate = (radioinfo.id1*2)/10;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));
	CWProtocolStore8(msgPtr, radioinfo.Radio_L_ID); 
	CWProtocolStore8(msgPtr, radioinfo.wlanid); 
	CWProtocolStore16(msgPtr, 0);
	CWProtocolStore8(msgPtr, rate); 
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore8(msgPtr, 0); 
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool  CWAssembleStaticAPIP(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char valuelen=13; //
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_ipadd);
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_mask);
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_gateway); 
	CWProtocolStore8(msgPtr, 1); 
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE);

}
//fengwenchao add 20110126 for XJDEV-32  from 2.0
CWBool  CWAssembleWtpEthMtu(CWProtocolMessage *msgPtr,int wtpid,unsigned char eth_index)
{
	unsigned char valuelen = 10; 
	unsigned int reseved = 0;
	unsigned char value = 1;
	unsigned int mtu = 1500;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(eth_index < AP_ETH_IF_NUM){
		mtu = AC_WTP[wtpid]->apifinfo.eth[eth_index].eth_mtu;
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, eth_index);
	CWProtocolStore32(msgPtr, mtu);
	CWProtocolStore32(msgPtr, reseved);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_ETH_SET_CW_TYPE);

}
//fengnwenchao add end
CWBool CWAssembleTimestamp(CWProtocolMessage *msgPtr, int wtpid)
{
	unsigned char valuelen = 4;
	int timestamp = 0;
	if(msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, timestamp);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE);
}

CWBool CWAssemblewtpextensioninfomation(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char valuelen = 4;
	unsigned char value = 4;
	unsigned char flag = AC_WTP[wtpid]->wifi_extension_reportswitch;
	unsigned short interval = AC_WTP[wtpid]->wifi_extension_reportinterval;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, flag);
	CWProtocolStore16(msgPtr, interval);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool  CWAssemblewtpstainfomationreport(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char valuelen = 4; 
	unsigned char value = 6;
	unsigned char flag = AC_WTP[wtpid]->ap_sta_report_switch;
	unsigned short interval = AC_WTP[wtpid]->ap_sta_report_interval;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, flag);
	CWProtocolStore16(msgPtr, interval);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool  CWAssembleWtpStaWapiInfoReport(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char valuelen = 4; 
	unsigned char value = 9;
	unsigned char flag = AC_WTP[wtpid]->ap_sta_wapi_report_switch;
	unsigned short interval = 0;
	interval  = (unsigned short)AC_WTP[wtpid]->ap_sta_wapi_report_interval;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, flag);
	CWProtocolStore16(msgPtr, interval);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssemblewtpifinforeport(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char valuelen = 4; 
	unsigned char value = 7;
	unsigned char flag = 0;
	unsigned short interval = 0;
	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	WID_CHECK_WTP_STANDARD_RET(wtpid, CW_FALSE);
	
	flag = AC_WTP[wtpid]->apifinfo.report_switch;
	interval = AC_WTP[wtpid]->apifinfo.report_interval;
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, flag);
	CWProtocolStore16(msgPtr, interval);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool  CWAssembleWidsSet(CWProtocolMessage *msgPtr,int wtpid)
{
	unsigned char valuelen = 7; 
	unsigned char value = 8;
	unsigned char flooding = gwids.flooding;
	unsigned char sproof = gwids.sproof;
	unsigned char weakiv = gwids.weakiv;
	unsigned char probethreshold = gprobethreshold;
	unsigned char otherthreshold = gotherthreshold;
	unsigned char interval = gwidsinterval;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, flooding);
	CWProtocolStore8(msgPtr, sproof);
	CWProtocolStore8(msgPtr, weakiv);

	CWProtocolStore8(msgPtr, otherthreshold);
	CWProtocolStore8(msgPtr, probethreshold);
	CWProtocolStore8(msgPtr, interval);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPInterfaceInfo(CWProtocolMessage *msgPtr)
{
	int value=5; //ap APInterfaceInfo
	int value_length = 3;//
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, value_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value); //ap APInterfaceInfo
	CWProtocolStore8(msgPtr, WID_SAMPLE_INFORMATION.sample_switch);
	CWProtocolStore8(msgPtr, WID_SAMPLE_INFORMATION.sample_time);
		
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemRadioOperationalState(int radioID, CWProtocolMessage *msgPtr) 
{
	const int radio_Operational_State_Length=3;
	CWRadiosOperationalInfo infos;
	CWProtocolMessage *msgs = NULL;
	int len = 0;
	int i = 0;
	int j = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	/* Coverity: CID: 10893  Error-Type: Uninitialized scalar variable */
	memset(&infos, 0, sizeof(CWRadiosOperationalInfo));
	if(!(CWGetWTPRadiosOperationalState(radioID, &infos))) {
		return CW_FALSE;
	}
	
    /* coverity-CID: 10329 Resource leak */
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), {CW_FREE_OBJECT(infos.radios);return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
    
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		/* coverity-CID: 10329 Resource leak */
	    CW_CREATE_PROTOCOL_MESSAGE(msgs[i], radio_Operational_State_Length, {for(j = i-1; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}\
																				CW_FREE_OBJECT(infos.radios);\
																				CW_FREE_OBJECT(msgs);\
																				return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].state); // state of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].cause);
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE))) {
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
//		wid_syslog_debug_debug("Radio operational State: %d - %d - %d", infos.radios[i].ID, infos.radios[i].state, infos.radios[i].cause);
	}
	
	/* coverity-CID: 10329 Resource leak */
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, {\
				for(i = 0; i < infos.radiosCount; i++) {CW_FREE_PROTOCOL_MESSAGE(msgs[i]);} \
				CW_FREE_OBJECT(msgs);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
}


/*_________________________________________________________________________*/
/*  *****************************___PARSE___*****************************  */
CWBool CWParseACNameWithIndex (CWProtocolMessage *msgPtr, int len, CWACNameWithIndexValues *valPtr)
{
	CWParseMessageElementStart();

	valPtr->index = CWProtocolRetrieve8(msgPtr);
	//wid_syslog_debug_debug("CW_MSG_ELEMENT_WTP_RADIO_ID: %d",	(valPtr->radios)[radioIndex].ID);
	
	valPtr->ACName = CWProtocolRetrieveStr(msgPtr,len-1);
	//wid_syslog_debug_debug("CW_MSG_ELEMENT_WTP_RADIO_TYPE: %d",	(valPtr->radios)[radioIndex].type);
	
	//wid_syslog_debug_debug("AC Name with index: %d - %s", valPtr->index, valPtr->ACName);
	
	CWParseMessageElementEnd();
}

CWBool CWParseDiscoveryType(CWProtocolMessage *msgPtr, int len, CWDiscoveryRequestValues *valPtr) {	
	CWParseMessageElementStart();
										
	valPtr->type = CWProtocolRetrieve8(msgPtr);
	//wid_syslog_debug_debug("CW_MSG_ELEMENT_DISCOVERY_TYPE: %d",	valPtr->type);
	
	CWParseMessageElementEnd();
}

CWBool CWParseLocationData(CWProtocolMessage *msgPtr, int len, char **valPtr) {	
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieveStr(msgPtr, len);
	if(valPtr == NULL) return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
	
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemDuplicateIPv4Address(CWProtocolMessage *msgPtr, int len, WTPDuplicateIPv4 *valPtr) 
{
	CWParseMessageElementStart();
	
	valPtr->ipv4Address =  CWProtocolRetrieve32(msgPtr);
	valPtr->status = CWProtocolRetrieve8(msgPtr);
	valPtr->length = CWProtocolRetrieve8(msgPtr);
	valPtr->MACoffendingDevice_forIpv4 = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr, valPtr->length);
	
	//valPtr->MACoffendingDevice_forIpv4 = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr,6);
	//valPtr->status = CWProtocolRetrieve8(msgPtr);
	//wid_syslog_debug_debug("Duplicate IPv4: %d", valPtr->ipv4Address);

	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemDuplicateIPv6Address(CWProtocolMessage *msgPtr, int len, WTPDuplicateIPv6 *valPtr) 
{
	CWParseMessageElementStart();
	char *aux = NULL;
	int i = 0;
	
	for(i=0; i<16; i++)
	{	
		aux= CWProtocolRetrieveRawBytes(msgPtr, 1);
		/* By the way modify for coverity-CID: 10875  Resource leak */
		if(!aux)
		{
			wid_syslog_err("%s line %d malloc failed!\n", __func__, __LINE__);
			continue;
		}
		(valPtr->ipv6Address).s6_addr[i] = *aux;
		/* coverity-CID: 10875  Resource leak */
		CW_FREE_OBJECT(aux);
	}

	//wid_syslog_debug_debug("Duplicate IPv6");
	//valPtr->MACoffendingDevice_forIpv6 = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr,6);

	valPtr->status = CWProtocolRetrieve8(msgPtr);

	valPtr->length = CWProtocolRetrieve8(msgPtr);

	valPtr->MACoffendingDevice_forIpv6 = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr, valPtr->length);

	CWParseMessageElementEnd();
}

//added by weiay 20080702
CWBool CWParseMsgElemCWStationInfoValue(CWProtocolMessage *msgPtr, int len, CWStationInfoValues *valPtr)
{
	//printf("*** CWParseMsgElemCWStationInfoValue ***\n");
	//printf("*** len:%d ***\n",len);
	
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_length = CWProtocolRetrieve8(msgPtr);
	if(valPtr->mac_length<6||valPtr->mac_length>64){
		wid_syslog_err("%s mac_length==%d\n",__func__,valPtr->mac_length);
	}else{
		//printf("*** len:%d ***\n",(int)valPtr->mac_length);
		valPtr->mac_addr =  CWProtocolRetrieveStr(msgPtr,valPtr->mac_length);
	}
	//printf("*** radio id:%d mac length:%d mac value:%s ***\n",valPtr->radio_id,valPtr->mac_length,valPtr->mac_addr);
	//printf("mac value = ");
	//int i;
	//for (i = 0; i < 6; i++)
	//{
	//	printf("%02x", valPtr->mac_addr[i]);
	//}
	//printf("\n");
	
	CWParseMessageElementEnd();	

}

CWBool CWParseMsgElemCWWtpStaIpMacReportInfo(CWProtocolMessage *msgPtr, int len, CWStationReportInfo *valPtr)
{
	//printf("*** CWParseMsgElemCWStationInfoValue ***\n");
	//printf("*** len:%d ***\n",len);
	int i = 0;
	CWParseMessageElementStart();
	
	valPtr->op = 0;
	valPtr->radioId = 0;
	valPtr->wlanId = 0;
	valPtr->vlanId = 0;
	valPtr->length = 0;
	valPtr->ipv4Address = 0;
	
	valPtr->op=   CWProtocolRetrieve8(msgPtr);

	valPtr->radioId= CWProtocolRetrieve8(msgPtr);

	valPtr->wlanId= CWProtocolRetrieve8(msgPtr);
	valPtr->vlanId= CWProtocolRetrieve32(msgPtr);

	unsigned char* mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,6);
	
	memcpy(valPtr->mac, mac, 6);
	free(mac);
	mac = NULL;

	//valPtr->mac =  CWProtocolRetrieveStr(msgPtr,valPtr->mac_length);
	valPtr->length = CWProtocolRetrieve8(msgPtr);

	if(valPtr->length == 16){

		for(i=0; i<16; i++)
		{
			char *aux = NULL;
			aux= CWProtocolRetrieveRawBytes(msgPtr, 1);
			(valPtr->ipv6Address).s6_addr[i] = *aux;
			CW_FREE_OBJECT(aux);
		}
	}else {
		valPtr->ipv4Address =  CWProtocolRetrieve32(msgPtr);
	}
	
	CWParseMessageElementEnd();	

}
CWBool CWParseMsgElemAPNeighborAPInfos(CWProtocolMessage *msgPtr, int len, Neighbor_AP_INFOS *valPtr)
{
	CWParseMessageElementStart();
	int i = 0;
	int essidlen = 0;
	int ielen = 0;
	char *ptr = NULL;
	char *essid = NULL;
	unsigned int count_n = 0;
	unsigned int oldOffset2 = 0;
	oldOffset2 = msgPtr->offset;
	/*
	value = CWProtocolRetrieve8(msgPtr);//AP_SCANNING_INFO = 1
	printf("*** value:%d ***\n",value);

	if(value != 1)
	{
		valPtr->neighborapInfosCount = 0;
		valPtr->neighborapInfos = NULL;
		return CW_FALSE;
	}
	*/
	valPtr->DeviceInterference = 0;
	valPtr->DeviceInterference = CWProtocolRetrieve8(msgPtr);	//wtpDeviceInterferenceDetectedTrap
	valPtr->neighborapInfosCount = CWProtocolRetrieve8(msgPtr);
	valPtr->neighborapInfos = NULL;
	struct Neighbor_AP_ELE *neighborapelem = NULL;
	struct Neighbor_AP_ELE *phead = NULL;

	for(i = 0; i < valPtr->neighborapInfosCount; i++)
	{
		CW_CREATE_OBJECT_SIZE_ERR(neighborapelem, sizeof(struct Neighbor_AP_ELE), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););		
		memset(neighborapelem, 0, sizeof(struct Neighbor_AP_ELE));
		char * str = CWProtocolRetrieveStr(msgPtr, MAC_LEN);
		memcpy(neighborapelem->BSSID ,str, MAC_LEN);
		CW_FREE_OBJECT(str);

		neighborapelem->Rate = CWProtocolRetrieve16(msgPtr);
		neighborapelem->Channel = CWProtocolRetrieve8(msgPtr);
		neighborapelem->RSSI = CWProtocolRetrieve8(msgPtr);
		neighborapelem->NOISE = CWProtocolRetrieve8(msgPtr);
		neighborapelem->BEACON_INT = CWProtocolRetrieve8(msgPtr);

		neighborapelem->status = 1;
		neighborapelem->opstatus = CWProtocolRetrieve8(msgPtr);
		neighborapelem->capabilityinfo = CWProtocolRetrieve16(msgPtr);
		neighborapelem->wtpid = 0;

		wid_syslog_debug_debug(WID_DEFAULT,"BSSID: "MACSTR" status %d opstatus %d channel %d\n",
		MAC2STR(neighborapelem->BSSID),neighborapelem->status,neighborapelem->opstatus,neighborapelem->Channel);

		essidlen = CWProtocolRetrieve8(msgPtr);
		if((essidlen < 0) || (essidlen > 32))    //fengwenchao change "&&" to "||" for autelan-3251 20121129
		{
			wid_syslog_err("%s essidlen==%d\n",__func__,essidlen);
			essidlen = 32;
			valPtr->neighborapInfosCount = count_n;
			wid_syslog_warning("<warning>%s line==%d,count_n =%d.\n",__func__,__LINE__,count_n);
			CW_FREE_OBJECT(neighborapelem);			//fengwenchao add for autelan-3251 20121129
			oldOffset2 = msgPtr->offset;
			msgPtr->offset += len - oldOffset2;
			return CW_TRUE;
		}
		//neighborapelem->ESSID = CWProtocolRetrieveStr(msgPtr,essidlen);
		essid = CWProtocolRetrieveStr(msgPtr,essidlen);
		if(check_ascii_32_to126(essid) == CW_FALSE)
		{
			CW_FREE_OBJECT(essid);
			//CW_CREATE_OBJECT_SIZE_ERR(neighborapelem->ESSID, ESSID_DEFAULT_LEN, return CW_FALSE;);
			memset(neighborapelem->ESSID, 0, 5);
			memcpy(neighborapelem->ESSID, "none", 4);
			//neighborapelem->ESSID[0] = '\0';
		}
		else
		{
			memset(neighborapelem->ESSID, 0, strlen(essid)+1);
			memcpy(neighborapelem->ESSID,  essid, strlen(essid));
			CW_FREE_OBJECT(essid);
		}

		ielen = CWProtocolRetrieve8(msgPtr);
		if((ielen < 0)||(ielen > 64))
		{
			ielen = 64;
			wid_syslog_err("%s ielen==%d\n",__func__,ielen);
			valPtr->neighborapInfosCount = count_n;
			wid_syslog_warning("<warning>%s line==%d,count_n =%d.\n",__func__,__LINE__,count_n);
			CW_FREE_OBJECT(neighborapelem);		//fengwenchao add for autelan-3251 20121129	
			oldOffset2 = msgPtr->offset;
			msgPtr->offset += len - oldOffset2;
			return CW_TRUE;
		}
		neighborapelem->IEs_INFO = CWProtocolRetrieveStr(msgPtr,ielen);
		if(check_ascii_32_to126(neighborapelem->IEs_INFO) == CW_FALSE)
		{
			CW_FREE_OBJECT(neighborapelem->IEs_INFO);
			/* coverity-CID:10948 Resource leak */
			neighborapelem->IEs_INFO = (char *)malloc(1);
			if(NULL == neighborapelem->IEs_INFO)
			{
				CW_FREE_OBJECT(neighborapelem);
				wid_syslog_err("%s %d,malloc error\n", __func__, __LINE__);
				msgPtr->offset = len + oldOffset2;
				return CW_FALSE;
			}
	
			neighborapelem->IEs_INFO[0] = '\0';
		}

		time(&neighborapelem->fst_dtc_tm);

		ptr = strchr(neighborapelem->IEs_INFO, 'P'); 

		if(ptr)
		{
			neighborapelem->encrp_type = 1;
			ptr = strstr(neighborapelem->IEs_INFO, "WPA"); 
			if(ptr)
			{
				neighborapelem->encrp_type = 2;
			}
			ptr = strstr(neighborapelem->IEs_INFO, "RSN"); 
			if(ptr)
			{
				neighborapelem->encrp_type = 3;
			}
		}
		else
		{
			neighborapelem->encrp_type = 0;
		}
		ptr = NULL;
		
		neighborapelem->polcy = 0;
		
		neighborapelem->next = NULL;

		if(valPtr->neighborapInfos == NULL)
		{
			//printf("parse first ap info\n");
			valPtr->neighborapInfos = neighborapelem;
			phead = neighborapelem;
			neighborapelem = NULL;
		}
		else
		{
			//printf("parse more ap info\n");
			phead->next = neighborapelem;
			phead = neighborapelem;
			neighborapelem = NULL;
		}
		count_n ++;
		/*if(count_n >= 20)	msg buff is 2048,so we can only save 2048/sizeof(struct Neighbor_AP_ELE) = 25,here we only save firt 20 neighbor ap
		{
			valPtr->neighborapInfosCount = count_n;
			wid_syslog_warning("<warning>%s,%d,neighbo ap count_n access to %d,we will save first %d only.\n",__func__,__LINE__,count_n,count_n);
			oldOffset2 = msgPtr->offset;
			msgPtr->offset += len - oldOffset2;
			return CW_TRUE;
//			break;
		}*/
	}	

	CWParseMessageElementEnd();	
}


CWBool CWParseMsgElemAPWidsInfos(CWProtocolMessage *msgPtr, int len, wid_wids_device *valPtr,int wtpindex)
{
	CWParseMessageElementStart();
	int i = 0;
	unsigned char * str = NULL;

	valPtr->count = CWProtocolRetrieve8(msgPtr);
	
	valPtr->wids_device_info = NULL;
	
	struct tag_wids_device_ele *wids_device_ele = NULL;
	struct tag_wids_device_ele *phead = NULL;

	for(i=0; i<valPtr->count; i++)
	{
		CW_CREATE_OBJECT_SIZE_ERR(wids_device_ele, sizeof(struct tag_wids_device_ele), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););		


		wids_device_ele->attacktype = CWProtocolRetrieve8(msgPtr);

		//printf("03333 %d\n",wids_device_ele->attacktype);
		if(wids_device_ele->attacktype == 1)
		{
			AC_WTP[wtpindex]->wids_statist.floodingcount++;
		}
		if(wids_device_ele->attacktype == 2)
		{
			AC_WTP[wtpindex]->wids_statist.sproofcount++;
		}
		if(wids_device_ele->attacktype == 3)
		{
			AC_WTP[wtpindex]->wids_statist.weakivcount++;
		}
		else
		{
			wid_syslog_debug_debug(WID_WTPINFO,"*** CWParseMsgElemAPWidsInfos  format error***\n");	
		}
		//printf("03333 %d %d %d\n",AC_WTP[wtpindex]->wids_statist.floodingcount,AC_WTP[wtpindex]->wids_statist.sproofcount,AC_WTP[wtpindex]->wids_statist.weakivcount);
		str = (unsigned char*)CWProtocolRetrieveStr(msgPtr,6);
		memcpy(wids_device_ele->bssid,str, 6);
		free(str);
		str = NULL;

		str = (unsigned char*)CWProtocolRetrieveStr(msgPtr,6);
		memcpy(wids_device_ele->vapbssid,str, 6);
		free(str);
		str = NULL;
						
		wids_device_ele->frametype = CWProtocolRetrieve8(msgPtr);
		wids_device_ele->channel = CWProtocolRetrieve8(msgPtr);
		wids_device_ele->rssi = CWProtocolRetrieve8(msgPtr);


		time(&wids_device_ele->fst_attack);
		time(&wids_device_ele->lst_attack);
		wids_device_ele->attackcount = 1;
		
		wids_device_ele->next = NULL;

		wid_asd_send_wids_info(wids_device_ele,wtpindex);
		
		if(valPtr->wids_device_info== NULL)
		{
			//printf("parse first ap info\n");
			valPtr->wids_device_info = wids_device_ele;
			phead = wids_device_ele;
			wids_device_ele = NULL;
		}
		else
		{
			//printf("parse more ap info\n");
			phead->next = wids_device_ele;
			phead = wids_device_ele;
			wids_device_ele = NULL;
		}
		
	}	

	CWParseMessageElementEnd();	

}


CWBool CWParseMsgElemAPInterfaceInfo(CWProtocolMessage *msgPtr, int len, wid_sample_rate_info *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->current_uplink_throughput = CWProtocolRetrieve32(msgPtr);	
	valPtr->current_downlink_throughput = CWProtocolRetrieve32(msgPtr);

	CWParseMessageElementEnd(); 

}
CWBool CWParseMsgElemAPStaInfoReport(CWProtocolMessage *msgPtr, int len, WIDStationInfo *valPtr,int wtpindex)
{
	int i = 0;
	unsigned int count = 0;
	unsigned int count1 = 0;
	WIDStationInfo valPtr1[64];
	int needoffset = 0;
	
	CWParseMessageElementStart();
	count = CWProtocolRetrieve8(msgPtr);
	count1 = count;
	memset(valPtr1,0, 64*sizeof(WIDStationInfo));
	//printf("count %d\n",count);
	if((count*15 != len-1)&&(count*23 != len-1)&&(count*65 != len-1))
	{
		wid_syslog_info("__func__ something wrong count %d len %d\n",count,len);
		needoffset = len-1;
		count1 = 0;
	}
	if(count > 64)
	{
		needoffset = len - 1 - (len-1)/count*64;
		count1 = 64;
		wid_syslog_info("__func__ count %d large than 64 needoffset %d\n",count,needoffset);
	}
	for(i=0;i<count1;i++)
	{
		valPtr1[i].radioId = CWProtocolRetrieve8(msgPtr);	
		valPtr1[i].wlanId = CWProtocolRetrieve8(msgPtr);
		
		unsigned char* mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,6);
		memcpy(valPtr1[i].mac, mac, 6);
		CW_FREE_OBJECT(mac);
			
		//for(i=0;i<6;i++)
		//{
		//	valPtr->mac[i] = CWProtocolRetrieve8(msgPtr);
		//}
		
		valPtr1[i].mode = CWProtocolRetrieve8(msgPtr);
		valPtr1[i].channel = CWProtocolRetrieve8(msgPtr);
		valPtr1[i].rssi = CWProtocolRetrieve8(msgPtr);
		valPtr1[i].tx_Rate = CWProtocolRetrieve16(msgPtr);
		valPtr1[i].isPowerSave = CWProtocolRetrieve8(msgPtr);
		valPtr1[i].isQos = CWProtocolRetrieve8(msgPtr);

		if((len-1)/count > 15)
		{
			valPtr1[i].rx_bytes = CWProtocolRetrieve32(msgPtr);
			valPtr1[i].tx_bytes = CWProtocolRetrieve32(msgPtr);
		}
		/* zhangshu append 2010-08-28 */
		if ((len-1)/count > 23)
		{
			CWProtocolRetrieve64(msgPtr, &valPtr1[i].rx_data_bytes);
			
			CWProtocolRetrieve64(msgPtr, &valPtr1[i].tx_data_bytes);

			valPtr1[i].rx_data_frames = CWProtocolRetrieve32(msgPtr);

			valPtr1[i].tx_data_frames = CWProtocolRetrieve32(msgPtr);

			valPtr1[i].rx_frames = CWProtocolRetrieve32(msgPtr);

			valPtr1[i].tx_frames = CWProtocolRetrieve32(msgPtr);

			valPtr1[i].rx_frag_packets = CWProtocolRetrieve32(msgPtr);
			
			valPtr1[i].tx_frag_packets = CWProtocolRetrieve32(msgPtr);

			valPtr1[i].rx_Rate = CWProtocolRetrieve16(msgPtr);			
		}
	}
	if (count1 != 0)
	{
	    WidAsd_StationInfoUpdate(wtpindex,count1,valPtr1);
	}
	if (needoffset != 0)
	{		
		(msgPtr->offset) += needoffset;
	}
	CWParseMessageElementEnd();
}


CWBool CWParseStaInfoAddSta(CWProtocolMessage *msgPtr, int len, int wtpid)
{
	unsigned short IGNORE_UBSV priv_type = 0;
	unsigned short IGNORE_UBSV dev_type = 0;
	unsigned char wlanid = 0;
	unsigned char l_radioid = 0;
	unsigned char sta_type = 0, portal_status = 0;
	unsigned char ostype_len = 0;
	unsigned char pd_model_len = 0;
	unsigned char hostname_len = 0;
	unsigned char cpu_model_len = 0;
	unsigned int sta_ipv4 = 0;
	unsigned int inactive_time = 0;
	unsigned char *apmac = NULL;
	unsigned char *stamac = NULL;
	unsigned char *bssid = NULL;
	unsigned char *ostype = NULL;
	unsigned char *pd_model = NULL;
	unsigned char *hostname = NULL;
	unsigned char *cpu_model = NULL;
	struct in_addr ipaddr;
	struct sta_info *sta = NULL;
	unsigned int beginoffset = msgPtr->offset;
	
	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);
	memset(&ipaddr, 0, sizeof(struct in_addr));
	
    /*当前针对协议版本前后兼容，编码解析处理未对"报文格式变化后，缺失中间某些字段"进行容错处理，
    因此要求后续报文格式变化，仅支持在现有格式上增加上报属性或者对于中间字段填入0xff表示该字段无效 wangwei*/
    
	priv_type = CWProtocolRetrieve16(msgPtr);
	sta_type = CWProtocolRetrieve8(msgPtr);
	portal_status = CWProtocolRetrieve8(msgPtr);
	sta_ipv4 = CWProtocolRetrieve32(msgPtr);
	stamac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
	apmac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
	bssid = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
	l_radioid = CWProtocolRetrieve8(msgPtr);
	wlanid = CWProtocolRetrieve8(msgPtr);
	inactive_time = CWProtocolRetrieve32(msgPtr);
	dev_type = CWProtocolRetrieve16(msgPtr);
	ostype_len = CWProtocolRetrieve8(msgPtr);
	ostype = (unsigned char*)CWProtocolRetrieveStr(msgPtr,ostype_len);
	pd_model_len = CWProtocolRetrieve8(msgPtr);
	pd_model = (unsigned char*)CWProtocolRetrieveStr(msgPtr,pd_model_len);
	hostname_len = CWProtocolRetrieve8(msgPtr);
	hostname = (unsigned char*)CWProtocolRetrieveStr(msgPtr,hostname_len);
	cpu_model_len = CWProtocolRetrieve8(msgPtr);
	cpu_model = (unsigned char*)CWProtocolRetrieveStr(msgPtr,cpu_model_len);
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"radio %u wlan %u bss["MACSTR"] add sta(%s)["MACSTR"] "WAI_IP_FMT
					"%s portal_status:%s(%u) ostype(%d)%s pd_model(%d)%s hostname(%d)%s cpu_model(%d)%s\n",
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid, wlanid, WAI_MAC_LITERAL(bssid),
					WAI_STA_DEVTYPE_STR(dev_type), WAI_MAC_LITERAL(stamac), WAI_IP_LITERAL(sta_ipv4),
					sta_type ? "guest":"user", WAI_STA_PORTALSTATUS_STR(portal_status), portal_status,
					ostype_len, ostype, pd_model_len, pd_model, hostname_len, hostname, cpu_model_len, cpu_model);
	
	if ((msgPtr->offset - beginoffset) < len)
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"add-sta "WAI_MAC_FMT""WAI_IP_FMT""
				"type %u portal %u from radio %u wlan %u bss["MACSTR"] with pkt short offset %u len %u\n", \
				WAI_FUNC_LINE_LITERAL, wtpid,AC_WTP[wtpid]->WTPNAME,WAI_MAC_LITERAL(stamac), WAI_IP_LITERAL(sta_ipv4), \
				sta_type, portal_status, l_radioid, wlanid, WAI_MAC_LITERAL(bssid), msgPtr->offset, len);
	}

	if (98 == wlanid)
	{
		int i = 0;
		for (i = 1; i < WTP_NUM; i++)
		{
			if (AC_WTP[i] != NULL)
			{
				if (!memcmp(AC_WTP[i]->WTPMAC, stamac, MAC_LEN))
				{					
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT""WAI_IP_FMT"["MACSTR"] through wireless uplink to the "WAI_AFI_FMT"["MACSTR"] radio %u bss["MACSTR"]\n",
									WAI_FUNC_LINE_LITERAL, i, AC_WTP[i]->WTPNAME, WAI_IP_LITERAL(sta_ipv4), WAI_MAC_LITERAL(stamac),
									wtpid, AC_WTP[wtpid]->WTPNAME, WAI_MAC_LITERAL(apmac), l_radioid, WAI_MAC_LITERAL(bssid));
					
					AC_WTP[i]->WTPStat = WID_WIRE_UPSIDE;
					break;
				}
			}
		}
		if (i >= WTP_NUM)
		{
			wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT""WAI_IP_FMT"["MACSTR"] through wireless uplink to the "WAI_AFI_FMT" radio %u bss["MACSTR"]\n",
							WAI_FUNC_LINE_LITERAL, 0, WAI_IP_LITERAL(sta_ipv4), WAI_MAC_LITERAL(stamac),
							wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid, WAI_MAC_LITERAL(bssid));
		}
	}
	else if ((wlanid >= WLAN_NUM) || (wlanid <= 0) || (NULL == AC_WLAN[wlanid]) || (0 == AC_WLAN[wlanid]->used))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT" wlan %u not exist when "WAI_AFI_FMT"radio %u add sta\n", \
						WAI_FUNC_LINE_LITERAL, wlanid, wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid);
		
		CW_FREE_OBJECT(stamac);
		CW_FREE_OBJECT(apmac);
		CW_FREE_OBJECT(bssid);
		CW_FREE_OBJECT(ostype);
		CW_FREE_OBJECT(pd_model);
		CW_FREE_OBJECT(hostname);
		CW_FREE_OBJECT(cpu_model);
		
		return -1;
	}
	else
	{
		unsigned int bssindex = AC_WLAN[wlanid]->S_WTP_BSS_List[wtpid][l_radioid];
		if (NULL == AC_BSS[bssindex])
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) nil on wlan %u radio %u when add sta\n", \
							WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid);
			
			CW_FREE_OBJECT(stamac);
			CW_FREE_OBJECT(apmac);
			CW_FREE_OBJECT(bssid);
			CW_FREE_OBJECT(ostype);
			CW_FREE_OBJECT(pd_model);
			CW_FREE_OBJECT(hostname);
			CW_FREE_OBJECT(cpu_model);
			
			return -1;
		}
		if (0 != memcmp(bssid, AC_BSS[bssindex]->BSSID, MAC_LEN))
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan %u radio %u bssid "
						WAI_MAC_FMT"mismatch reported "WAI_MAC_FMT"when add sta\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid, \
						WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID),WAI_MAC_LITERAL(bssid));
			
			if (0 == memcmp(gdefaultMac, AC_BSS[bssindex]->BSSID, MAC_LEN))
			{
				memcpy(AC_BSS[bssindex]->BSSID, bssid, MAC_LEN);
			}
		}
    	
		sta = ap_get_sta(bssindex, stamac);
		if (sta)
		{
			sta->sta_type = sta_type;
			sta->dev_type = (unsigned char)dev_type;
			sta->portal_status = portal_status;
			sta->inactive_time = inactive_time;
			sta->sta_online_time = 0;
			sta->roam_type = 0;
			
			if (sta->ip_addr.s_addr != sta_ipv4)
			{
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update exist sta(%s) "\
							WAI_MAC_FMT""WAI_IP_FMT" to "WAI_IP_FMT	"on bss(%u) "WAI_MAC_FMT"\n", \
							WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_STA_DEVTYPE_STR(sta->dev_type),
							WAI_MAC_LITERAL(sta->addr), WAI_IP_LITERAL(sta->ip_addr.s_addr),
							WAI_IP_LITERAL(sta_ipv4), bssindex, WAI_MAC_LITERAL(bssid));
				
				sta->ip_addr.s_addr = sta_ipv4;
				if (sta->in_addr == NULL)
				{
					sta->in_addr = os_zalloc(sizeof(unsigned char)*16);
					if (sta->in_addr == NULL)
					{
						CW_FREE_OBJECT(stamac);
						CW_FREE_OBJECT(apmac);
						CW_FREE_OBJECT(bssid);
						CW_FREE_OBJECT(ostype);
						CW_FREE_OBJECT(pd_model);
						CW_FREE_OBJECT(hostname);
						CW_FREE_OBJECT(cpu_model);
						
						wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"addr mem error when update sta "\
								WAI_MAC_FMT""WAI_IP_FMT"on bss(%u) "WAI_MAC_FMT"\n", \
								WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_MAC_LITERAL(sta->addr), \
								WAI_IP_LITERAL(sta->ip_addr.s_addr), bssindex, WAI_MAC_LITERAL(bssid));
						return -1;
					}
				}
				
				ipaddr.s_addr = htonl(sta_ipv4);
				os_memset(sta->in_addr, 0, 16);
				os_strncpy(sta->in_addr, "0.0.0.0", 7);
				snprintf(sta->in_addr, 16, "%s", inet_ntoa(ipaddr));
			}
			
			if ((NULL != ostype) && (*ostype != '\0'))
			{
				memset(sta->ostype, 0, DEFAULT_LEN);
				memcpy(sta->ostype, ostype, ostype_len);
			}
			if ((NULL != pd_model) && (*pd_model != '\0'))
			{
				memset(sta->pd_model, 0, DEFAULT_LEN);
				memcpy(sta->pd_model, pd_model, pd_model_len);
			}
			if ((NULL != hostname) && (*hostname != '\0'))
			{
				memset(sta->hostname, 0, DEFAULT_LEN);
				memcpy(sta->hostname, hostname, hostname_len);
			}
			if ((NULL != cpu_model) && (*cpu_model != '\0'))
			{
				memset(sta->cpu_model, 0, DEFAULT_LEN);
				memcpy(sta->cpu_model, cpu_model, cpu_model_len);
			}
			
			time(sta->add_time);
			get_sysruntime(&sta->add_time_sysruntime);
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update exist sta "\
						WAI_MAC_FMT""WAI_IP_FMT" on bss(%u) "WAI_MAC_FMT"sys-time %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_MAC_LITERAL(sta->addr), \
						WAI_IP_LITERAL(sta->ip_addr.s_addr), bssindex, WAI_MAC_LITERAL(bssid), \
						(int)sta->add_time_sysruntime);
		}
		else
		{
			if ((AC_WTP[wtpid] != NULL)
				&& (AC_WTP[wtpid]->wtp_accessed_sta_num > AC_WTP[wtpid]->wtp_allowed_max_sta_num))
			{
				wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"access sta %u excess specs %u when add new sta\n",
							WAI_FUNC_LINE_LITERAL, wtpid,  AC_WTP[wtpid]->WTPNAME, \
							AC_WTP[wtpid]->wtp_accessed_sta_num, AC_WTP[wtpid]->wtp_allowed_max_sta_num);
			}
			CW_CREATE_OBJECT_ERR(sta, struct sta_info,\
								{CW_FREE_OBJECT(stamac);CW_FREE_OBJECT(apmac);CW_FREE_OBJECT(bssid);\
								CW_FREE_OBJECT(ostype);CW_FREE_OBJECT(pd_model);CW_FREE_OBJECT(hostname);\
								CW_FREE_OBJECT(cpu_model);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
			
			memset(sta, 0, sizeof(struct sta_info));
			sta->ip_addr.s_addr = sta_ipv4;
			sta->in_addr = os_zalloc(sizeof(unsigned char)*16);
			
			if (sta->in_addr == NULL)
			{
				CW_FREE_OBJECT(sta);
				CW_FREE_OBJECT(stamac);
				CW_FREE_OBJECT(apmac);
				CW_FREE_OBJECT(bssid);			
				CW_FREE_OBJECT(ostype);
				CW_FREE_OBJECT(pd_model);
				CW_FREE_OBJECT(hostname);
				CW_FREE_OBJECT(cpu_model);
				
				wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"addr mem error when add new sta "\
								WAI_MAC_FMT""WAI_IP_FMT"on bss(%u) "WAI_MAC_FMT"\n", \
								WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, \
								bssindex, WAI_MAC_LITERAL(sta->addr), \
								WAI_IP_LITERAL(sta->ip_addr.s_addr), WAI_MAC_LITERAL(bssid));
				return -1;
			}
			
			ipaddr.s_addr = htonl(sta_ipv4);
			os_memset(sta->in_addr, 0, 16);
			os_strncpy(sta->in_addr, "0.0.0.0", 7);
			snprintf(sta->in_addr, 16, "%s", inet_ntoa(ipaddr));
			
			memcpy(sta->addr, stamac, MAC_LEN);
			
			char name[32] = {0};
			snprintf(name, 31, "sta_%02x%02x%02x", stamac[3], stamac[4], stamac[5]);
			memcpy(sta->sta_name, name, strlen(name));
			
			sta->add_time = (time_t *)os_zalloc(sizeof(time_t));
			if (sta->add_time == NULL)
			{
				CW_FREE_OBJECT(sta->in_addr);
				CW_FREE_OBJECT(sta);
				CW_FREE_OBJECT(stamac);
				CW_FREE_OBJECT(apmac);
				CW_FREE_OBJECT(bssid);
				CW_FREE_OBJECT(ostype);
				CW_FREE_OBJECT(pd_model);
				CW_FREE_OBJECT(hostname);
				CW_FREE_OBJECT(cpu_model);
				
				wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"time mem error when add new sta "\
								WAI_MAC_FMT""WAI_IP_FMT" on bss(%u) "WAI_MAC_FMT"\n", \
								WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex,\
								WAI_MAC_LITERAL(sta->addr), WAI_IP_LITERAL(sta->ip_addr.s_addr), WAI_MAC_LITERAL(bssid));
				return -1;
			}
			
			time(sta->add_time);
			get_sysruntime(&sta->add_time_sysruntime);
			
			sta->sta_type = sta_type;
			sta->dev_type = (unsigned char)dev_type;
			sta->portal_status = portal_status;
			sta->inactive_time = inactive_time;
			sta->sta_online_time = 0;
			memcpy(sta->ostype, ostype, ostype_len);
			memcpy(sta->pd_model, pd_model, pd_model_len);
			memcpy(sta->hostname, hostname, hostname_len);
			memcpy(sta->cpu_model, cpu_model, cpu_model_len);
			
			sta->BssIndex = bssindex;
			memcpy(sta->bssid, AC_BSS[bssindex]->BSSID, MAC_LEN);
			sta->next = AC_BSS[bssindex]->sta_list;
			AC_BSS[bssindex]->sta_list = sta;
			ap_sta_hash_add(bssindex, sta);
			AC_BSS[bssindex]->bss_accessed_sta_num++;
			AC_WTP[wtpid]->wtp_accessed_sta_num++;
			AC_WLAN[wlanid]->wlan_accessed_sta_num++;
			
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"add new sta(%s) "WAI_MAC_FMT""WAI_IP_FMT"%s portal_status %s(%u) "
						"ostype(%d)%s pd_model(%d)%s hostname(%d)%s cpu_model(%d)%s on bss(%u) "WAI_MAC_FMT"sys-time %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_STA_DEVTYPE_STR(dev_type),\
						WAI_MAC_LITERAL(sta->addr), WAI_IP_LITERAL(sta->ip_addr.s_addr), \
						sta_type ? "guest":"user", WAI_STA_PORTALSTATUS_STR(portal_status), portal_status,
						ostype_len, ostype, pd_model_len, pd_model, hostname_len, hostname, cpu_model_len, cpu_model,
						bssindex, WAI_MAC_LITERAL(bssid), (int)sta->add_time_sysruntime);
			
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"current sta cnt of bss(%u) "WAI_MAC_FMT"%u wlan(%u) %s %u wtp %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(bssid),  \
						AC_BSS[bssindex]->bss_accessed_sta_num, wlanid, AC_WLAN[wlanid]->WlanName, \
						AC_WLAN[wlanid]->wlan_accessed_sta_num, AC_WTP[wtpid]->wtp_accessed_sta_num);
			
			YLog(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"add new sta(%s) "WAI_MAC_FMT""WAI_IP_FMT"%s portal_status %s(%u) "
						"ostype(%d)%s pd_model(%d)%s hostname(%d)%s cpu_model(%d)%s on bss(%u) "WAI_MAC_FMT"sys-time %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_STA_DEVTYPE_STR(dev_type),\
						WAI_MAC_LITERAL(sta->addr), WAI_IP_LITERAL(sta->ip_addr.s_addr), \
						sta_type ? "guest":"user", WAI_STA_PORTALSTATUS_STR(portal_status), portal_status,
						ostype_len, ostype, pd_model_len, pd_model, hostname_len, hostname, cpu_model_len, cpu_model,
						bssindex, WAI_MAC_LITERAL(bssid), (int)sta->add_time_sysruntime);
			
			YLog(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"current sta cnt of bss(%u) "WAI_MAC_FMT"%u wlan(%u) %s %u wtp %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(bssid),  \
						AC_BSS[bssindex]->bss_accessed_sta_num, wlanid,  AC_WLAN[wlanid]->WlanName, \
						AC_WLAN[wlanid]->wlan_accessed_sta_num, AC_WTP[wtpid]->wtp_accessed_sta_num);
		}
		
		wid_check_sta_roam(sta);
		
		if ((NULL != sta) && (sta->inactive_time >= 300000))	//5min timeout
		{
			wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_MAC_FMT""WAI_IP_FMT"inactive_time %dms "WAI_AFI_FMT"del sta on bss%u["WAI_MAC_FMT"]\n",
						WAI_FUNC_LINE_LITERAL, WAI_MAC_LITERAL(sta->addr), WAI_IP_LITERAL(sta->ip_addr.s_addr), sta->inactive_time,
						wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID));
			ap_sta_hash_del(bssindex, sta);
			ap_sta_list_del(bssindex, sta);
			destroy_sta_info(sta);
			AC_BSS[bssindex]->bss_accessed_sta_num--;
			AC_WTP[wtpid]->wtp_accessed_sta_num--;
			AC_WLAN[wlanid]->wlan_accessed_sta_num--;
		}
	}
	CW_FREE_OBJECT(stamac);
	CW_FREE_OBJECT(apmac);
	CW_FREE_OBJECT(bssid);
	CW_FREE_OBJECT(ostype);
	CW_FREE_OBJECT(pd_model);
	CW_FREE_OBJECT(hostname);
	CW_FREE_OBJECT(cpu_model);

	return 0;
}


CWBool CWParseStaInfoDelSta(CWProtocolMessage *msgPtr, int len,int wtpid)
{
	unsigned short IGNORE_UBSV priv_type  = 0;
	unsigned char wlanid = 0;
	unsigned char l_radioid = 0;
	unsigned int online_time = 0;
	unsigned int uplink_packages = 0;
	unsigned int downlink_packages = 0;
	unsigned long long tx_bytes = 0, rx_bytes = 0;
	unsigned char *apmac = NULL;
	unsigned char *stamac = NULL;
	unsigned char *bssid = NULL;
	struct sta_info *sta = NULL;

	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);
	
	priv_type = CWProtocolRetrieve16(msgPtr);
	stamac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	apmac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	bssid = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	l_radioid = CWProtocolRetrieve8(msgPtr);
	wlanid = CWProtocolRetrieve8(msgPtr);
	uplink_packages = CWProtocolRetrieve32(msgPtr);
	downlink_packages = CWProtocolRetrieve32(msgPtr);
	CWProtocolRetrieve64(msgPtr, &tx_bytes);
	CWProtocolRetrieve64(msgPtr, &rx_bytes);
	online_time = CWProtocolRetrieve32(msgPtr);
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"delete sta "WAI_MAC_FMT"from radio %u wlan %u bss "
					WAI_MAC_FMT"online %u day %u:%u:%u txpackages %u rxpackages %u  tx %llu rx %llu\n",
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, \
					WAI_MAC_LITERAL(stamac), l_radioid, wlanid, WAI_MAC_LITERAL(bssid),
					WAI_TIME_DAY(online_time), WAI_TIME_HOUR_INDAY(online_time), \
					WAI_TIME_MINUTE_INHOUR(online_time),WAI_TIME_SECOND_INMINUTE(online_time),
					uplink_packages, downlink_packages, tx_bytes, rx_bytes);
	
	if (98 == wlanid)
	{
		int i = 0;
		for (i = 1; i < WTP_NUM; i++)
		{
			if (AC_WTP[i] != NULL)
			{
				if (!memcmp(AC_WTP[i]->WTPMAC, stamac, MAC_LEN))
				{
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"["MACSTR"] from wireless uplink "WAI_AFI_FMT"["MACSTR"] radio %u bss["MACSTR"] drops\n",
									WAI_FUNC_LINE_LITERAL, i, AC_WTP[i]->WTPNAME, WAI_MAC_LITERAL(stamac),
									wtpid, AC_WTP[wtpid]->WTPNAME, WAI_MAC_LITERAL(apmac), l_radioid, WAI_MAC_LITERAL(bssid));
					
					AC_WTP[i]->WTPStat = WID_OFFLINE;
					break;
				}
			}
		}
		if (i >= WTP_NUM)
		{
			wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"["MACSTR"] through wireless uplink to the "WAI_AFI_FMT"["MACSTR"] radio %u bss["MACSTR"]\n",
							WAI_FUNC_LINE_LITERAL, 0, WAI_MAC_LITERAL(stamac), wtpid, AC_WTP[wtpid]->WTPNAME,
							WAI_MAC_LITERAL(apmac), l_radioid, WAI_MAC_LITERAL(bssid));
		}
	}
	else if ((wlanid >= WLAN_NUM) || (wlanid <= 0) || (NULL == AC_WLAN[wlanid]) || (0 == AC_WLAN[wlanid]->used))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT" wlan%u not exist when "WAI_AFI_FMT"radio %u del sta\n", \
						WAI_FUNC_LINE_LITERAL, wlanid, wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid);
		
		CW_FREE_OBJECT(stamac);
		CW_FREE_OBJECT(apmac);
		CW_FREE_OBJECT(bssid);
		
		return -1;
	}
	else
	{
		unsigned int bssindex = AC_WLAN[wlanid]->S_WTP_BSS_List[wtpid][l_radioid];
		if (NULL == AC_BSS[bssindex])
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) nil on wlan %u radio %u when delete sta\n", \
						WAI_FUNC_LINE_LITERAL,  wtpid, AC_WTP[wtpid]->WTPNAME, \
						bssindex, wlanid, l_radioid);
			CW_FREE_OBJECT(stamac);
			CW_FREE_OBJECT(apmac);
			CW_FREE_OBJECT(bssid);
			
			return -1;
		}
		
		if (0 != memcmp(bssid, AC_BSS[bssindex]->BSSID, MAC_LEN))
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan %u radio %u bssid "
						WAI_MAC_FMT"mismatch reported "WAI_MAC_FMT"when delete sta\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,bssindex, wlanid, l_radioid, \
						WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID),WAI_MAC_LITERAL(bssid));
			
			if (0 == memcmp(gdefaultMac, AC_BSS[bssindex]->BSSID, MAC_LEN))
			{
				memcpy(AC_BSS[bssindex]->BSSID, bssid, MAC_LEN);
			}
		}
    	
		sta = ap_get_sta(bssindex, stamac);
		if (sta)
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"del sta "WAI_MAC_FMT""WAI_IP_FMT" on bss(%u) "WAI_MAC_FMT"\n",
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,WAI_MAC_LITERAL(sta->addr), 
						WAI_IP_LITERAL(sta->ip_addr.s_addr),bssindex, WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID));
			
			YLog(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"del sta "WAI_MAC_FMT""WAI_IP_FMT" on bss(%u) "WAI_MAC_FMT"\n",
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,WAI_MAC_LITERAL(sta->addr), 
						WAI_IP_LITERAL(sta->ip_addr.s_addr),bssindex, WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID));
			
			sta->sta_online_time = online_time;
			sta->txpackets = uplink_packages;
			sta->rxpackets = downlink_packages;
			sta->txbytes = tx_bytes;
			sta->rxbytes = rx_bytes;
			//Upload the database
			ap_sta_hash_del(bssindex, sta);
			ap_sta_list_del(bssindex, sta);
			destroy_sta_info(sta);
			AC_BSS[bssindex]->bss_accessed_sta_num--;
			AC_WTP[wtpid]->wtp_accessed_sta_num--;
			AC_WLAN[wlanid]->wlan_accessed_sta_num--;
			
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"current sta cnt of bss(%u) "WAI_MAC_FMT"%u wlan(%u) %s %u wtp %u\n",
							WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(bssid),
							(AC_BSS[bssindex]->bss_accessed_sta_num), wlanid,  AC_WLAN[wlanid]->WlanName,
							(AC_WLAN[wlanid]->wlan_accessed_sta_num), (AC_WTP[wtpid]->wtp_accessed_sta_num));
			
			YLog(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"current sta cnt of bss(%u) "WAI_MAC_FMT"%u wlan(%u) %s %u wtp %u\n",
				WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(bssid),
				(AC_BSS[bssindex]->bss_accessed_sta_num), wlanid,  AC_WLAN[wlanid]->WlanName,
				(AC_WLAN[wlanid]->wlan_accessed_sta_num), (AC_WTP[wtpid]->wtp_accessed_sta_num));
		}
		else
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"delete sta "\
						WAI_MAC_FMT"from bss(%u) "WAI_MAC_FMT"wlan(%u) %s but not found\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,WAI_MAC_LITERAL(stamac), \
						bssindex, WAI_MAC_LITERAL(bssid), wlanid, AC_WLAN[wlanid]->WlanName);
		}
	}
	
	CW_FREE_OBJECT(stamac);
	CW_FREE_OBJECT(apmac);
	CW_FREE_OBJECT(bssid);

	return 0;
}


CWBool CWParseStaStatisticsInfo(CWProtocolMessage *msgPtr, int len,int wtpid)
{
	unsigned int i = 0, j = 0;
	unsigned short IGNORE_UBSV priv_type  = 0;
	unsigned short bss_num = 0;
	unsigned short sta_num = 0;
	unsigned char wlanid = 0;
	unsigned char l_radioid = 0;
	unsigned char total_num = 0, cur_num = 0;
	unsigned char sta_type = 0, portal_type = 0;        
	unsigned char *apmac = NULL;
	unsigned char *stamac = NULL;
	unsigned char *bssid = NULL;
	struct sta_info *sta = NULL;
	struct sta_info *stalist = NULL;
	struct in_addr ipaddr;
	
	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);
	memset(&ipaddr, 0, sizeof(struct in_addr));
	
    priv_type = CWProtocolRetrieve16(msgPtr);
	total_num = CWProtocolRetrieve8(msgPtr);
	cur_num = CWProtocolRetrieve8(msgPtr);
    apmac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
    bss_num = CWProtocolRetrieve16(msgPtr);
    
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT""WAI_MAC_FMT"%d/%d report sta under %u bss\n", \
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_MAC_LITERAL(apmac), cur_num, total_num, bss_num);
	
	if (0 == bss_num)
	{
		for (i = 0; (i < L_RADIO_NUM) && ((AC_WTP[wtpid])&&(i < AC_WTP[wtpid]->RadioCount)); i++)
		{
			int j = 0;
			for (j = 0; j < L_BSS_NUM; j++)
			{
				if ((AC_WTP[wtpid])
					&& (AC_WTP[wtpid]->WTP_Radio[i])
					&& (AC_WTP[wtpid]->WTP_Radio[i]->BSS[j] != NULL))
				{
					unsigned int BssIndex = AC_WTP[wtpid]->WTP_Radio[i]->BSS[j]->BSSIndex;
					asd_free_stas(BssIndex);
				}
			}
		}
	}
	
	for (i = 0; i < bss_num; i++)
	{    
		sta_type = CWProtocolRetrieve8(msgPtr);
		portal_type = CWProtocolRetrieve8(msgPtr);
		l_radioid = CWProtocolRetrieve8(msgPtr);
		wlanid = CWProtocolRetrieve8(msgPtr);
		bssid = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
		sta_num = CWProtocolRetrieve16(msgPtr);
		
		if ((l_radioid >= L_RADIO_NUM) || (wlanid <= 0) || ((wlanid != 98) && (wlanid >= WLAN_NUM || NULL == AC_WLAN[wlanid])))
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"radioid= %d AC_WLAN[%d] %s NULL\n",
							WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid, wlanid, (NULL == AC_WLAN[wlanid]) ? "==":"!=");
			CW_FREE_OBJECT(apmac);			
			CW_FREE_OBJECT(bssid);
			return -1;
		}
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss[%u] "WAI_MAC_FMT"has %u sta\n", \
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, i+1, WAI_MAC_LITERAL(bssid), sta_num);
		
		struct sta_info *staelem = NULL;
		struct sta_info *ptail = NULL;
		for (j = 0; j < sta_num; j++)
		{
			CW_CREATE_OBJECT_SIZE_ERR(staelem, sizeof(struct sta_info),\
					{CW_FREE_OBJECT(apmac);CW_FREE_OBJECT(bssid);destroy_sta_info_list(&(stalist)); return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});      
			memset(staelem, 0, sizeof(struct sta_info));

			stamac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
			memcpy(staelem->addr, stamac, MAC_LEN);
			CW_FREE_OBJECT(stamac);

			staelem->ip_addr.s_addr = CWProtocolRetrieve32(msgPtr);
			staelem->sta_online_time = CWProtocolRetrieve32(msgPtr);
			staelem->inactive_time = CWProtocolRetrieve32(msgPtr);
			staelem->nRate = CWProtocolRetrieve32(msgPtr);
			staelem->txRate = CWProtocolRetrieve32(msgPtr);
			staelem->rr = CWProtocolRetrieve32(msgPtr);
			staelem->tr = CWProtocolRetrieve32(msgPtr);
			staelem->sta_average_delay = CWProtocolRetrieve16(msgPtr);
			staelem->pkt_loss_per = CWProtocolRetrieve8(msgPtr);
			staelem->rssi = CWProtocolRetrieve8(msgPtr);
			staelem->portal_status = CWProtocolRetrieve8(msgPtr);
			staelem->rx_retry_per = CWProtocolRetrieve8(msgPtr);
			staelem->tx_retry_per = CWProtocolRetrieve8(msgPtr);
			(msgPtr->offset) += 1;
			staelem->txpackets = (unsigned int)CWProtocolRetrieve32(msgPtr);
			staelem->rxpackets = (unsigned int)CWProtocolRetrieve32(msgPtr);
			CWProtocolRetrieve64(msgPtr, (unsigned long long *)&staelem->rxbytes);
			CWProtocolRetrieve64(msgPtr, (unsigned long long *)&staelem->txbytes);

			staelem->sta_type = sta_type;
			staelem->portal_type = portal_type;

			staelem->next = NULL;
                
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss[%u] sta[%u] "WAI_MAC_FMT""WAI_IP_FMT"%s portal_status:%s(%u)"
							" inactive_time(%ldms) sta_delay %d(0.1ms) rssi(%u) pkt_loss %d%% rx_retry %d%% tx_retry %d%% "
							"rx-wifirate(%u) tx-wifirate(%u) txpkt(%u) txpkt(%u) rxbytes(%llu) txbytes(%llu) online %u day %u:%u:%u\n",
							WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, i+1, j+1,
							WAI_MAC_LITERAL(staelem->addr), WAI_IP_LITERAL(staelem->ip_addr.s_addr), \
							sta_type ? "guest":"user", WAI_STA_PORTALSTATUS_STR(portal_type), portal_type,
							staelem->inactive_time, staelem->sta_average_delay, staelem->rssi,
							staelem->pkt_loss_per, staelem->rx_retry_per, staelem->tx_retry_per, staelem->nRate,
							staelem->txRate, staelem->txpackets, staelem->rxpackets, staelem->rxbytes, staelem->txbytes,
							WAI_TIME_DAY(staelem->sta_online_time), WAI_TIME_HOUR_INDAY(staelem->sta_online_time), \
							WAI_TIME_MINUTE_INHOUR(staelem->sta_online_time),WAI_TIME_SECOND_INMINUTE(staelem->sta_online_time));
			
			if (98 == wlanid)
			{
				int k = 0;
				for (k = 1; k < WTP_NUM; k++)
				{
					if (AC_WTP[k] != NULL)
					{
						if (!memcmp(AC_WTP[k]->WTPMAC, staelem->addr, MAC_LEN))
						{					
							wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT""WAI_IP_FMT"["MACSTR"] through wireless "
											"uplink to the "WAI_AFI_FMT"["MACSTR"] radio %u bss["MACSTR"]\n",
											WAI_FUNC_LINE_LITERAL, k, AC_WTP[k]->WTPNAME, WAI_IP_LITERAL(staelem->ip_addr.s_addr),
											WAI_MAC_LITERAL(staelem->addr), wtpid, AC_WTP[wtpid]->WTPNAME,
											WAI_MAC_LITERAL(apmac), l_radioid, WAI_MAC_LITERAL(bssid));
							
							AC_WTP[i]->WTPStat = WID_WIRE_UPSIDE;
							break;
						}
					}
				}
				if (k >= WTP_NUM)
				{
					wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT""WAI_IP_FMT"["MACSTR"] through wireless uplink to the "WAI_AFI_FMT" radio %u bss["MACSTR"]\n",
									WAI_FUNC_LINE_LITERAL, 0, WAI_IP_LITERAL(staelem->ip_addr.s_addr), WAI_MAC_LITERAL(staelem->addr),
									wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid, WAI_MAC_LITERAL(bssid));
				}
				CW_FREE_OBJECT(staelem);
			}
			else
			{
				if (stalist == NULL)
				{
					stalist = staelem;
					ptail = staelem;
					staelem = NULL;
				}
				else
				{
					ptail->next = staelem;
					ptail = staelem;
					staelem = NULL;
				}
			}
		}
		
		if (98 != wlanid)
		{
			unsigned int bssindex = AC_WLAN[wlanid]->S_WTP_BSS_List[wtpid][l_radioid];
			if (NULL == AC_BSS[bssindex])
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) nil on wlan %u radio %u when report sta\n", \
								WAI_FUNC_LINE_LITERAL,  wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid);
				CW_FREE_OBJECT(bssid);
				destroy_sta_info_list(&(stalist));
				continue;
			}
			
			if (0 != memcmp(bssid, AC_BSS[bssindex]->BSSID, MAC_LEN))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan %u radio %u bssid "
							WAI_MAC_FMT"mismatch reported "WAI_MAC_FMT"\n", \
							WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid, \
							WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID),WAI_MAC_LITERAL(bssid));
				
				if (0 == memcmp(gdefaultMac, AC_BSS[bssindex]->BSSID, MAC_LEN))
				{
					memcpy(AC_BSS[bssindex]->BSSID, bssid, MAC_LEN);
				}
			}
			
			if (stalist == NULL)
			{
				wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan%u radio%u bssid "WAI_MAC_FMT"no sta reported\n", \
								WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid, WAI_MAC_LITERAL(bssid));
			}
			
			struct sta_info *pnode = AC_BSS[bssindex]->sta_list;
			struct sta_info *pnext = NULL;
			while (pnode != NULL)
			{
				pnext = pnode->next;
				
				struct sta_info *p = NULL;
				p = find_elem_from_sta_list(stalist,pnode);
				if (p == NULL)  //elem not in the list
				{
					ap_sta_hash_del(bssindex, pnode);
					ap_sta_list_del(bssindex, pnode);
					
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan %u radio %u sta "
								WAI_MAC_FMT""WAI_IP_FMT"may logoff, we remove it\n", \
								WAI_FUNC_LINE_LITERAL,	wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid, \
								WAI_MAC_LITERAL(pnode->addr), WAI_IP_LITERAL(pnode->ip_addr.s_addr));
					
					YLog(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan %u radio %u sta "
								WAI_MAC_FMT""WAI_IP_FMT"may logoff, we remove it\n", \
								WAI_FUNC_LINE_LITERAL,	wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid, \
								WAI_MAC_LITERAL(pnode->addr), WAI_IP_LITERAL(pnode->ip_addr.s_addr));
					
					destroy_sta_info(pnode);
					
					AC_BSS[bssindex]->bss_accessed_sta_num--;
					AC_WTP[wtpid]->wtp_accessed_sta_num--;
					AC_WLAN[wlanid]->wlan_accessed_sta_num--;
				}
				
				pnode = pnext;
			}
			
			pnode = stalist;
			stalist = NULL;
			while (pnode != NULL)
			{
				pnext = pnode->next;
				
				sta = ap_get_sta(bssindex, pnode->addr);
				if (sta)
				{
					if (sta->ip_addr.s_addr != pnode->ip_addr.s_addr)
					{
						sta->ip_addr.s_addr = pnode->ip_addr.s_addr;
						ipaddr.s_addr = htonl(sta->ip_addr.s_addr);
						os_memset(sta->in_addr, 0, 16);
						os_strncpy(sta->in_addr, "0.0.0.0", 7);
						snprintf(sta->in_addr, 16, "%s", inet_ntoa(ipaddr));
					}
					
					sta->sta_online_time = pnode->sta_online_time;
					sta->inactive_time = pnode->inactive_time;
					sta->sta_average_delay = pnode->sta_average_delay;
					sta->sta_type = pnode->sta_type;
					sta->rssi = pnode->rssi;
					sta->rx_retry_per = pnode->rx_retry_per;
					sta->tx_retry_per = pnode->tx_retry_per;
					sta->pkt_loss_per = pnode->pkt_loss_per;
					sta->portal_type = pnode->portal_type;
					sta->portal_status = pnode->portal_status;
					sta->txRate = pnode->txRate;
					sta->nRate = pnode->nRate;
					sta->tr = pnode->tr;
					sta->rr = pnode->rr;
					sta->txbytes = pnode->txbytes;
					sta->rxbytes = pnode->rxbytes;
					sta->txpackets = pnode->txpackets;
					sta->rxpackets = pnode->rxpackets;
					
					destroy_sta_info(pnode);
					
					wid_check_sta_roam(sta);
					
					if ((NULL != sta) && (sta->inactive_time >= 300000))	//5min timeout
					{
						wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_MAC_FMT""WAI_IP_FMT"inactive_time %dms "WAI_AFI_FMT"del sta on bss%u["WAI_MAC_FMT"]\n",
										WAI_FUNC_LINE_LITERAL, WAI_MAC_LITERAL(sta->addr), WAI_IP_LITERAL(sta->ip_addr.s_addr), sta->inactive_time,
										wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID));
						ap_sta_hash_del(bssindex, sta);
						ap_sta_list_del(bssindex, sta);
						destroy_sta_info(sta);
						AC_BSS[bssindex]->bss_accessed_sta_num--;
						AC_WTP[wtpid]->wtp_accessed_sta_num--;
						AC_WLAN[wlanid]->wlan_accessed_sta_num--;
					}
				}
				else
				{
					if ((AC_WTP[wtpid] != NULL)
						&& (AC_WTP[wtpid]->wtp_accessed_sta_num > AC_WTP[wtpid]->wtp_allowed_max_sta_num))
					{
						wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"access sta %u excess specs %u\n",
									WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, \
									AC_WTP[wtpid]->wtp_accessed_sta_num, AC_WTP[wtpid]->wtp_allowed_max_sta_num);
					}
					
					pnode->in_addr = os_zalloc(sizeof(unsigned char)*16);
					if (pnode->in_addr == NULL)
					{
						wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"addr mem error when add sta "\
										WAI_MAC_FMT""WAI_IP_FMT"on bss "WAI_MAC_FMT"\n", \
										WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, \
										WAI_MAC_LITERAL(pnode->addr), \
										WAI_IP_LITERAL(pnode->ip_addr.s_addr), WAI_MAC_LITERAL(bssid));
					}
					else
					{
						ipaddr.s_addr = htonl(pnode->ip_addr.s_addr);
						os_memset(pnode->in_addr, 0, 16);
						os_strncpy(pnode->in_addr, "0.0.0.0", 7);
						snprintf(pnode->in_addr, 16, "%s", inet_ntoa(ipaddr));
					}
					
					char name[32] = {0};
					snprintf(name, 31, "sta_%02x%02x%02x", pnode->addr[3], pnode->addr[4], pnode->addr[5]);
					memcpy(pnode->sta_name, name, strlen(name));
					
					pnode->add_time = (time_t *)os_zalloc(sizeof(time_t));
					if (pnode->add_time == NULL)
					{
						wid_syslog_crit(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"time mem error when add sta "\
										WAI_MAC_FMT""WAI_IP_FMT" on bss "WAI_MAC_FMT"\n", \
										WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,
										WAI_MAC_LITERAL(pnode->addr), \
										WAI_IP_LITERAL(pnode->ip_addr.s_addr), WAI_MAC_LITERAL(bssid));
					}
					
					time(pnode->add_time);
					get_sysruntime(&pnode->add_time_sysruntime);
					pnode->sta_online_time = 0;
					
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"sta "WAI_MAC_FMT""WAI_IP_FMT
									" on bss "WAI_MAC_FMT"sys-time %u may just login, we add it\n",
									WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,
									WAI_MAC_LITERAL(pnode->addr), WAI_IP_LITERAL(pnode->ip_addr.s_addr),
									WAI_MAC_LITERAL(bssid), (int)pnode->add_time_sysruntime);
					
					YLog(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"sta "WAI_MAC_FMT""WAI_IP_FMT
						" on bss "WAI_MAC_FMT"sys-time %u may just login, we add it\n",
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,
						WAI_MAC_LITERAL(pnode->addr), WAI_IP_LITERAL(pnode->ip_addr.s_addr),
						WAI_MAC_LITERAL(bssid), (int)pnode->add_time_sysruntime);
					
					pnode->BssIndex = bssindex;
					memcpy(pnode->bssid, AC_BSS[bssindex]->BSSID, MAC_LEN);
					pnode->next = AC_BSS[bssindex]->sta_list;
					AC_BSS[bssindex]->sta_list = pnode;
					ap_sta_hash_add(bssindex, pnode);
					AC_BSS[bssindex]->bss_accessed_sta_num++;
					AC_WTP[wtpid]->wtp_accessed_sta_num++;
					AC_WLAN[wlanid]->wlan_accessed_sta_num++;
					
					wid_check_sta_roam(pnode);
					
					if ((NULL != pnode) && (pnode->inactive_time >= 300000))    //5min timeout
					{
						wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_MAC_FMT""WAI_IP_FMT"inactive_time %dms "WAI_AFI_FMT"del sta on bss%u["WAI_MAC_FMT"]\n",
										WAI_FUNC_LINE_LITERAL, WAI_MAC_LITERAL(pnode->addr), WAI_IP_LITERAL(pnode->ip_addr.s_addr), pnode->inactive_time,
										wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID));
						ap_sta_hash_del(bssindex, pnode);
						ap_sta_list_del(bssindex, pnode);
						destroy_sta_info(pnode);
						AC_BSS[bssindex]->bss_accessed_sta_num--;
						AC_WTP[wtpid]->wtp_accessed_sta_num--;
						AC_WLAN[wlanid]->wlan_accessed_sta_num--;
					}
				}
				
				pnode = pnext;
			}
		}
		CW_FREE_OBJECT(bssid);
	}
	
	CW_FREE_OBJECT(apmac);
	
    return 0;
}


CWBool CWParseStaDeviceInfo(CWProtocolMessage *msgPtr, int len, int wtpid)
{
	unsigned short IGNORE_UBSV priv_type = 0;
	unsigned short IGNORE_UBSV dev_type  = 0;
	unsigned char wlanid = 0;
	unsigned char l_radioid = 0;
	unsigned char ostype_len = 0;
	unsigned char pd_model_len = 0;
	unsigned char hostname_len = 0;
	unsigned char cpu_model_len = 0;
	unsigned char *apmac = NULL;
	unsigned char *stamac = NULL;
	unsigned char *bssid = NULL;
	unsigned char *ostype = NULL;
	unsigned char *pd_model = NULL;
	unsigned char *hostname = NULL;
	unsigned char *cpu_model = NULL;
	struct sta_info *sta = NULL;
	unsigned int beginoffset = msgPtr->offset;

	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);
	
	/*当前针对协议版本前后兼容，编码解析处理未对"报文格式变化后，缺失中间某些字段"进行容错处理，
	因此要求后续报文格式变化，仅支持在现有格式上增加上报属性或者对于中间字段填入0xff表示该字段无效 wangwei*/
	
	priv_type = CWProtocolRetrieve16(msgPtr);
	stamac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	apmac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	bssid = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	l_radioid = CWProtocolRetrieve8(msgPtr);
	wlanid = CWProtocolRetrieve8(msgPtr);
	dev_type = CWProtocolRetrieve16(msgPtr);
	ostype_len = CWProtocolRetrieve8(msgPtr);
	ostype = (unsigned char*)CWProtocolRetrieveStr(msgPtr,ostype_len);
	pd_model_len = CWProtocolRetrieve8(msgPtr);
	pd_model = (unsigned char*)CWProtocolRetrieveStr(msgPtr,pd_model_len);
	hostname_len = CWProtocolRetrieve8(msgPtr);
	hostname = (unsigned char*)CWProtocolRetrieveStr(msgPtr,hostname_len);
	cpu_model_len = CWProtocolRetrieve8(msgPtr);
	cpu_model = (unsigned char*)CWProtocolRetrieveStr(msgPtr,cpu_model_len);
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"parse sta dev-info radio %u wlan %u bss["MACSTR"] supplement sta(%s) "
					WAI_MAC_FMT"ostype(len%d)%s pd_model(len%d)%s hostname(len%d)%s cpu_model(len%d)%s\n",
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid, wlanid, WAI_MAC_LITERAL(bssid),
					WAI_STA_DEVTYPE_STR(dev_type), WAI_MAC_LITERAL(stamac),
					ostype_len, ostype, pd_model_len, pd_model, hostname_len, hostname, cpu_model_len, cpu_model);
	
	if ((msgPtr->offset - beginoffset) < len)
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"supplement sta(%s) "WAI_MAC_FMT
						"from radio%u wlan%u bss["MACSTR"] with pkt short offset %u len %u\n", \
						WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,
						WAI_STA_DEVTYPE_STR(dev_type), WAI_MAC_LITERAL(stamac),
						l_radioid, wlanid, WAI_MAC_LITERAL(bssid), msgPtr->offset, len);
	}
	
	if ((wlanid >= WLAN_NUM) || (wlanid <= 0) || (NULL == AC_WLAN[wlanid]) || (0 == AC_WLAN[wlanid]->used))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT"wlan%u not exist when "WAI_AFI_FMT"radio%u add sta(%s) dev-info\n", \
						WAI_FUNC_LINE_LITERAL, wlanid, wtpid, AC_WTP[wtpid]->WTPNAME, l_radioid, WAI_STA_DEVTYPE_STR(dev_type));
		
		CW_FREE_OBJECT(stamac);
		CW_FREE_OBJECT(bssid);
		CW_FREE_OBJECT(apmac);
		CW_FREE_OBJECT(ostype);
		CW_FREE_OBJECT(pd_model);
		CW_FREE_OBJECT(hostname);
		CW_FREE_OBJECT(cpu_model);
		
		return -1;
    }
	
	unsigned int bssindex = AC_WLAN[wlanid]->S_WTP_BSS_List[wtpid][l_radioid];
	if (NULL == AC_BSS[bssindex])
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) nil on wlan %u radio %u when add sta\n", \
						WAI_FUNC_LINE_LITERAL,	wtpid, AC_WTP[wtpid]->WTPNAME,bssindex, wlanid, l_radioid);
		
		CW_FREE_OBJECT(stamac);
		CW_FREE_OBJECT(bssid);
		CW_FREE_OBJECT(apmac);
		CW_FREE_OBJECT(ostype);
		CW_FREE_OBJECT(pd_model);
		CW_FREE_OBJECT(hostname);
		CW_FREE_OBJECT(cpu_model);
		
		return -1;
	}
	
	if (0 != memcmp(bssid, AC_BSS[bssindex]->BSSID, MAC_LEN))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"bss(%u) on wlan %u radio %u bssid "
					WAI_MAC_FMT"mismatch reported "WAI_MAC_FMT"when add sta\n", \
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, bssindex, wlanid, l_radioid, \
					WAI_MAC_LITERAL(AC_BSS[bssindex]->BSSID),WAI_MAC_LITERAL(bssid));
		
		if (0 == memcmp(gdefaultMac, AC_BSS[bssindex]->BSSID, MAC_LEN))
		{
			memcpy(AC_BSS[bssindex]->BSSID, bssid, MAC_LEN);
		}
	}
	
	sta = ap_get_sta(bssindex, stamac);
	if (sta)
	{
		sta->dev_type = (unsigned char)dev_type;
		get_sysruntime(&sta->add_time_sysruntime);
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"update exist sta "\
					WAI_MAC_FMT""WAI_IP_FMT" on bss(%u) "WAI_MAC_FMT"sys-time %u\n", \
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME, WAI_MAC_LITERAL(sta->addr), \
					WAI_IP_LITERAL(sta->ip_addr.s_addr), bssindex, WAI_MAC_LITERAL(bssid), \
					(int)sta->add_time_sysruntime);
		sta->sta_online_time = 0;
	}
	else
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"add sta "WAI_MAC_FMT
					"devinfo from bss(%u) "WAI_MAC_FMT"wlan(%u) %s but not found\n", \
					WAI_FUNC_LINE_LITERAL, wtpid, AC_WTP[wtpid]->WTPNAME,WAI_MAC_LITERAL(stamac), \
					bssindex, WAI_MAC_LITERAL(bssid), wlanid, AC_WLAN[wlanid]->WlanName);
	}
	
	CW_FREE_OBJECT(stamac);
	CW_FREE_OBJECT(bssid);
	CW_FREE_OBJECT(apmac);
	CW_FREE_OBJECT(ostype);
	CW_FREE_OBJECT(pd_model);
	CW_FREE_OBJECT(hostname);
	CW_FREE_OBJECT(cpu_model);
	
	return 0;
}

CWBool CWParseStaInfo(CWProtocolMessage *msgPtr, int len, int wtpid)
{
	unsigned short priv_type = 0;

	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);
	
	//wid_hex_dump((unsigned char *)&((msgPtr->msg)[(msgPtr->offset)]), len);
	
    priv_type = CWProtocolPeep16(msgPtr);
	
	if (1 == priv_type)
	{
		return CWParseStaInfoAddSta(msgPtr,len,wtpid);
	}
	else if (2 == priv_type)
	{
		return CWParseStaInfoDelSta(msgPtr,len,wtpid);
	}
	else if (3 == priv_type)
	{
		return CWParseStaStatisticsInfo(msgPtr,len,wtpid);
	}
	else if (4 == priv_type)
	{
		return CWParseStaDeviceInfo(msgPtr,len,wtpid);
	}
	else
	{
		return -1;
	}    
}


CWBool CWParseAfiStatisticsInfo(CWProtocolMessage *msgPtr, int len, int wtpid)
{
	wid_syslog_debug_debug(WID_WTPINFO,WAI_FUNC_LINE_FMT"wtp%d len:%d\n", WAI_FUNC_LINE_LITERAL, wtpid, len);
	
	unsigned int i = 0, j = 0, k = 0;
	unsigned char l_radioid = 0;
	unsigned int g_radioid = 0;
	unsigned char wlanid = 0;
	unsigned char wlan_num = 0;
	unsigned char bss_num = 0;
	unsigned char wifi_snr = 0;
	unsigned char delay_num = 0;
	unsigned short radio_num = 0;
	unsigned int radio_snr = 0;
	unsigned int sta_asso_count = 0;
	unsigned int sta_auth_count = 0;
	unsigned int sta_abn_deauth_cut = 0;
	unsigned int uplink_bandwidth = 0;
	unsigned int downlink_bandwidth = 0;
	unsigned int uplink_packages = 0;
	unsigned int downlink_packages = 0;
	unsigned long long rx_bytes = 0;
	unsigned long long tx_bytes = 0;
	int offsetTillMessages = 0;
	
	offsetTillMessages = msgPtr->offset;
	
	WID_CHECK_WTP_STANDARD_RET(wtpid, -1);
	
	AC_WTP[wtpid]->cpu_per = CWProtocolRetrieve8(msgPtr);
	AC_WTP[wtpid]->mem_per = CWProtocolRetrieve8(msgPtr);
	AC_WTP[wtpid]->flash_per = CWProtocolRetrieve8(msgPtr);
	delay_num = CWProtocolRetrieve8(msgPtr);
	if (delay_num > TOTAL_AP_DELAY_NUM)
	{
		AC_WTP[wtpid]->apdelayinfo.delay_num = TOTAL_AP_DELAY_NUM;
	}
	else
	{
		AC_WTP[wtpid]->apdelayinfo.delay_num = delay_num;
	}
	for (k = 0; k < delay_num; k++)
	{
		if (k < TOTAL_AP_DELAY_NUM)
		{
			AC_WTP[wtpid]->apdelayinfo.ap_average_delay[k].delay_type = CWProtocolRetrieve8(msgPtr);
			AC_WTP[wtpid]->apdelayinfo.ap_average_delay[k].pkt_loss = CWProtocolRetrieve8(msgPtr);
			AC_WTP[wtpid]->apdelayinfo.ap_average_delay[k].delay_time = CWProtocolRetrieve16(msgPtr);
		}
		else
		{
			(msgPtr->offset) += 4;
		}
	}
	
	AC_WTP[wtpid]->uplink_bandwidth = CWProtocolRetrieve32(msgPtr);
	AC_WTP[wtpid]->downlink_bandwidth = CWProtocolRetrieve32(msgPtr);
	AC_WTP[wtpid]->wifi_extension_info.tx_packets = CWProtocolRetrieve32(msgPtr);
	AC_WTP[wtpid]->wifi_extension_info.rx_packets = CWProtocolRetrieve32(msgPtr);
	CWProtocolRetrieve64(msgPtr, &(AC_WTP[wtpid]->tx_bytes));
	CWProtocolRetrieve64(msgPtr, &(AC_WTP[wtpid]->rx_bytes));
	
	radio_num = CWProtocolRetrieve8(msgPtr);
	
	for (i = 0; i < radio_num; i++)
	{
		l_radioid = CWProtocolRetrieve8(msgPtr);
		WID_CHECK_L_RADIOID_STANDARD_RET(l_radioid, -1);
		
		g_radioid = wtpid * L_RADIO_NUM + l_radioid;
		WID_CHECK_G_RADIOID_STANDARD_RET(g_radioid, -1);
		
		AC_RADIO[g_radioid]->Radio_TXP = CWProtocolRetrieve8(msgPtr);
		AC_RADIO[g_radioid]->Radio_Chan = CWProtocolRetrieve8(msgPtr);
		AC_RADIO[g_radioid]->chan_per = CWProtocolRetrieve8(msgPtr);
		AC_RADIO[g_radioid]->disturbinfo.disturb_num = CWProtocolRetrieve8(msgPtr);
		
		for (k = 0; k < AC_RADIO[g_radioid]->disturbinfo.disturb_num; k++)
		{
			AC_RADIO[g_radioid]->disturbinfo.radio_disturb[k].disturb_type = CWProtocolRetrieve8(msgPtr);
			
			AC_RADIO[g_radioid]->disturbinfo.radio_disturb[k].disturb_rssi = CWProtocolRetrieve8(msgPtr);
    	}
		
		wlan_num = CWProtocolRetrieve8(msgPtr);
		
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"radio%d has bind %u wlan\n", WAI_FUNC_LINE_LITERAL, wtpid, l_radioid, wlan_num);
		k = 0;
		for (j = 0; j < wlan_num; j++)
		{
			wlanid = CWProtocolRetrieve8(msgPtr);
			wifi_snr = CWProtocolRetrieve8(msgPtr);
			(msgPtr->offset) += 1;
			uplink_bandwidth = CWProtocolRetrieve32(msgPtr);
			downlink_bandwidth = CWProtocolRetrieve32(msgPtr);
			uplink_packages = CWProtocolRetrieve32(msgPtr);
			downlink_packages = CWProtocolRetrieve32(msgPtr);
			CWProtocolRetrieve64(msgPtr, &tx_bytes);
			CWProtocolRetrieve64(msgPtr, &rx_bytes);
			sta_asso_count = CWProtocolRetrieve32(msgPtr);
			sta_auth_count = CWProtocolRetrieve32(msgPtr);
			sta_abn_deauth_cut = CWProtocolRetrieve32(msgPtr);
			
			if (98 == wlanid)
			{
				k++;
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"radio%d has connected to %u afi uplink_bandwidth(%u) downlink_bandwidth(%u) "
								"tx_packets(%u) rx_packets(%u) tx_bytes(%llu) rx_bytes(%llu) sta_asso_count %d sta_auth_count %d sta_abn_deauth_cut %d\n",
								WAI_FUNC_LINE_LITERAL, wtpid, l_radioid, k, uplink_bandwidth, downlink_bandwidth, uplink_packages,
								downlink_packages, tx_bytes, rx_bytes, sta_asso_count, sta_auth_count, sta_abn_deauth_cut);
				
				if (AC_WTP[wtpid]->uplink_bandwidth >= uplink_bandwidth)
				{
					AC_WTP[wtpid]->uplink_bandwidth -= uplink_bandwidth;
				}
				if (AC_WTP[wtpid]->downlink_bandwidth >= downlink_bandwidth)
				{
					AC_WTP[wtpid]->downlink_bandwidth -= downlink_bandwidth;
				}
				if (AC_WTP[wtpid]->wifi_extension_info.tx_packets >= uplink_packages)
				{
					AC_WTP[wtpid]->wifi_extension_info.tx_packets -= uplink_packages;
				}
				if (AC_WTP[wtpid]->wifi_extension_info.rx_packets >= downlink_packages)
				{
					AC_WTP[wtpid]->wifi_extension_info.rx_packets -= downlink_packages;
				}
				if (AC_WTP[wtpid]->tx_bytes >= tx_bytes)
				{
					AC_WTP[wtpid]->tx_bytes -= tx_bytes;
				}
				if (AC_WTP[wtpid]->rx_bytes >= rx_bytes)
				{
					AC_WTP[wtpid]->rx_bytes -= rx_bytes;
				}
			}
			else if ((wlanid >= WLAN_NUM) || (wlanid <= 0))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_WLAN_FMT"illegal\n", WAI_FUNC_LINE_LITERAL, wlanid);
			}
			else if ((NULL == AC_WLAN[wlanid]) || (0 == AC_WLAN[wlanid]->used))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_WLAN_FMT"not exist\n", WAI_FUNC_LINE_LITERAL, wlanid);
			}
			else
			{
				unsigned int bssindex = AC_WLAN[wlanid]->S_WTP_BSS_List[wtpid][l_radioid];
				if ((check_bssid_func(bssindex)) && (NULL != AC_BSS[bssindex]))
				{
					AC_BSS[bssindex]->wifi_snr = wifi_snr;
					AC_BSS[bssindex]->uplink_bandwidth = uplink_bandwidth;
					AC_BSS[bssindex]->downlink_bandwidth = downlink_bandwidth;
					AC_BSS[bssindex]->tx_packets = uplink_packages;
					AC_BSS[bssindex]->rx_packets = downlink_packages;
					AC_BSS[bssindex]->tx_bytes = tx_bytes;
					AC_BSS[bssindex]->rx_bytes = rx_bytes;
					if ((sta_asso_count != 0) && (sta_auth_count <= sta_asso_count))
					{
						AC_BSS[bssindex]->sta_auth_per = (sta_auth_count/sta_asso_count)*100;
					}
					else
					{
						wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"radio[%u] wlan[%u] bss[%u] sta_auth_count %d sta_asso_count %d\n",
										WAI_FUNC_LINE_LITERAL, wtpid, l_radioid, wlanid, bssindex, sta_auth_count, sta_asso_count);
					}
					if ((sta_asso_count != 0) && (sta_abn_deauth_cut <= sta_asso_count))
					{
						AC_BSS[bssindex]->sta_abn_deauth_per = (sta_abn_deauth_cut/sta_asso_count)*100;
					}
					else
					{
						wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"radio[%u] wlan[%u] bss[%u] sta_abn_deauth_cut %d sta_asso_count %d\n",
										WAI_FUNC_LINE_LITERAL, wtpid, l_radioid, wlanid, bssindex, sta_abn_deauth_cut, sta_asso_count);
					}
					
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"radio[%u] wlan[%u] bss[%u] wifi_snr %d uplink bandwidth(%lu) uplink_bandwidth(%lu) "
									"tx_packets(%u) rx_packets(%u) txbytes(%llu) rxbytes(%llu) sta_auth_per %d%% sta_abn_deauth_per %d%%\n",
									WAI_FUNC_LINE_LITERAL, wtpid, l_radioid, wlanid, bssindex, wifi_snr,
									AC_BSS[bssindex]->uplink_bandwidth, AC_BSS[bssindex]->downlink_bandwidth,
									AC_BSS[bssindex]->tx_packets, AC_BSS[bssindex]->rx_packets,
									AC_BSS[bssindex]->tx_bytes, AC_BSS[bssindex]->rx_bytes,
									AC_BSS[bssindex]->sta_auth_per, AC_BSS[bssindex]->sta_abn_deauth_per);
				}
				bss_num++;
				radio_snr += wifi_snr;
				AC_WLAN[wlanid]->uplink_bandwidth += uplink_bandwidth;
				AC_WLAN[wlanid]->downlink_bandwidth += downlink_bandwidth;
				AC_WLAN[wlanid]->wifi_extension_info.tx_packets += uplink_packages;
				AC_WLAN[wlanid]->wifi_extension_info.rx_packets += downlink_packages;
				AC_WLAN[wlanid]->wifi_extension_info.tx_bytes += tx_bytes;
				AC_WLAN[wlanid]->wifi_extension_info.rx_bytes += rx_bytes;
			}
		}
		if (0 != bss_num)
		{
			AC_RADIO[g_radioid]->wifi_snr = (unsigned char)radio_snr/bss_num;
		}
	}
	
	if ((msgPtr->offset - offsetTillMessages) != len)
	{
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	
	return CW_TRUE; 
}

CWBool CWParseWlanConfigurationBssInfo
(
	CWProtocolMessage *msgPtr,
	int len,
	CWProtocolResultCode *resultCode,
	WID_BSS *BSS,
	unsigned char *bss_num
)
{
	int offsetTillMessages = 0;
	
	offsetTillMessages = msgPtr->offset;
	
	CWParseMsgElemAssignedWTPBSSLIST(msgPtr, len, BSS, resultCode, bss_num);
	
	if ((msgPtr->offset - offsetTillMessages) != len)
	{
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the WlanConf BssInfo Report Message");
	}
	wid_syslog_debug_debug(WID_WTPINFO, WAI_FUNC_LINE_FMT"Wlan Configuration Bss Info Report Parsed", WAI_FUNC_LINE_LITERAL);
	
	return CW_TRUE; 
}

CWBool CWParseMsgElemAPIfInfoReport(CWProtocolMessage *msgPtr, int len, wid_ap_if_state_time *valPtr,int wtpindex)
{
	wid_syslog_debug_debug(WID_WTPINFO,"*** CWParseMsgElemAPIfInfoReport len:%d***\n",len);
	int i = 0;
	int state = 0;
	
	CWParseMessageElementStart();
	AC_WTP[wtpindex]->apifinfo.eth_num = CWProtocolRetrieve8(msgPtr);
	if ((0 == AC_WTP[wtpindex]->apifinfo.eth_num)
		||(AC_WTP[wtpindex]->apifinfo.eth_num > AP_ETH_IF_NUM))
	{
		wid_syslog_err("%s wtp%d report ethnum %d error\n",
				__func__, wtpindex, AC_WTP[wtpindex]->apifinfo.eth_num);
		AC_WTP[wtpindex]->apifinfo.eth_num = 1;
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_WTPINFO,"%s: wtp%d report eth_num %d\n",
	            __func__, wtpindex, AC_WTP[wtpindex]->apifinfo.eth_num);
	
	for(i = 0; i < (AC_WTP[wtpindex]->apifinfo.eth_num); i++)
	{
		AC_WTP[wtpindex]->apifinfo.eth[i].type = 0;// 0-eth
		AC_WTP[wtpindex]->apifinfo.eth[i].ifindex = CWProtocolRetrieve8(msgPtr);
		state = CWProtocolRetrieve8(msgPtr);// 0-not exist/1-up/2-down/3-error
		if ((state != 0) && (state != AC_WTP[wtpindex]->apifinfo.eth[i].state))
		{
			time_t t;
			time(&t);
			AC_WTP[wtpindex]->apifinfo.eth[i].state_time = t;
		}
		
		AC_WTP[wtpindex]->apifinfo.eth[i].state = state;

		wid_syslog_debug_debug(WID_WTPINFO,"eth type %d 0-eth 1-wifi\n",AC_WTP[wtpindex]->apifinfo.eth[i].type);
		wid_syslog_debug_debug(WID_WTPINFO,"eth index %d eth state %d 0-not exist/1-up/2-down/3-error\n",
								AC_WTP[wtpindex]->apifinfo.eth[i].ifindex, AC_WTP[wtpindex]->apifinfo.eth[i].state);
		
	}
	/* zhangshu modify for show over 2 eth ap information, 2010-09-27
	//added by weianying for ZheJiang telecom test ,we only display one eth interface

	if(AC_WTP[wtpindex]->apifinfo.eth_num > 1)
	{
		AC_WTP[wtpindex]->apifinfo.eth_num = 1;
	}*/

	AC_WTP[wtpindex]->apifinfo.wifi_num = CWProtocolRetrieve8(msgPtr);
	if (AC_WTP[wtpindex]->apifinfo.wifi_num > AP_WIFI_IF_NUM)
	{
		AC_WTP[wtpindex]->apifinfo.wifi_num = 1;
		wid_syslog_err("CWParseMsgElemAPIfInfoReport ap send wifi num error\n");
		return CW_FALSE;
	}
	wid_syslog_debug_debug(WID_WTPINFO,WAI_FUNC_LINE_FMT" wifi_num %d\n", WAI_FUNC_LINE_LITERAL, AC_WTP[wtpindex]->apifinfo.wifi_num);
	
	for (i = 0; i < (AC_WTP[wtpindex]->apifinfo.wifi_num);i++)
	{
		AC_WTP[wtpindex]->apifinfo.wifi[i].type = 1;// 1-wifi
		AC_WTP[wtpindex]->apifinfo.wifi[i].ifindex = CWProtocolRetrieve8(msgPtr);
		state = CWProtocolRetrieve8(msgPtr);// 0-not exist/1-up/2-down/3-error
		if ((state != 0) && (state != AC_WTP[wtpindex]->apifinfo.wifi[i].state))
		{
			time_t t;
			time(&t);
			AC_WTP[wtpindex]->apifinfo.wifi[i].state_time = t;
		}
		
		AC_WTP[wtpindex]->apifinfo.wifi[i].state = state;

		wid_syslog_debug_debug(WID_WTPINFO,"wifi type %d 0-eth 1-wifi\n",AC_WTP[wtpindex]->apifinfo.wifi[i].type);
		wid_syslog_debug_debug(WID_WTPINFO,"wifi index %d wifi state %d 0-not exist/1-up/2-down/3-error\n",
								AC_WTP[wtpindex]->apifinfo.wifi[i].ifindex, AC_WTP[wtpindex]->apifinfo.wifi[i].state);
	}
	
	CWParseMessageElementEnd(); 
}	

CWBool CWParseMsgElemAPExtensionInfo(CWProtocolMessage *msgPtr, int len, wid_wifi_info *valPtr)
{
	wid_syslog_debug_debug(WID_WTPINFO,"*** CWParseMsgElemAPExtensionInfo len:%d ***\n");
	
	if (len < 103)//if less than this len,it means the msg is sent by a ap which is running past software version
	{
		wid_syslog_debug_debug(WID_WTPINFO,"***CWParseMsgElemAPExtensionInfo len:%d too short***\n",len);
		return CW_FALSE;
	}
	int i = 0;
	int num = 0;
	CWParseMessageElementStart();
	int start = msgPtr->offset;  //fengwenchao add 20120314 for onlinebug-162//qiuchen copy from v1.3v
	valPtr->cpu = CWProtocolRetrieve32(msgPtr);	
	valPtr->tx_mgmt = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_mgmt = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_packets = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_errors = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_retry = CWProtocolRetrieve32(msgPtr);

	valPtr->tx_unicast = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_broadcast = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_multicast = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_drop = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_unicast = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_broadcast = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_multicast = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_drop = CWProtocolRetrieve32(msgPtr);

	valPtr->wpi_replay_error = CWProtocolRetrieve32(msgPtr);
	valPtr->wpi_decryptable_error = CWProtocolRetrieve32(msgPtr);
	valPtr->wpi_mic_error = CWProtocolRetrieve32(msgPtr);
	
	valPtr->disassoc_unnormal = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_assoc_norate = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_assoc_capmismatch = CWProtocolRetrieve32(msgPtr);
	valPtr->assoc_invaild = CWProtocolRetrieve32(msgPtr);
	valPtr->reassoc_deny = CWProtocolRetrieve32(msgPtr);
	valPtr->ipmode = CWProtocolRetrieve8(msgPtr);
	valPtr->memoryall = CWProtocolRetrieve16(msgPtr);
	valPtr->memoryuse = CWProtocolRetrieve8(msgPtr);
	valPtr->flashall = CWProtocolRetrieve16(msgPtr);
	valPtr->flashempty = CWProtocolRetrieve32(msgPtr);
	valPtr->wifi_snr = CWProtocolRetrieve8(msgPtr);
	//sz add  04-15
	valPtr->temperature = CWProtocolRetrieve8(msgPtr);
	
	valPtr->eth_count = CWProtocolRetrieve8(msgPtr);
	if(valPtr->eth_count > AP_ETH_IF_NUM){
		valPtr->eth_count = 1;
		wid_syslog_err("CWParseMsgElemAPExtensionInfo ap send eth num error\n");
		return CW_FALSE;
	}
	for(i=0;i<valPtr->eth_count;i++)
	{
		num = CWProtocolRetrieve8(msgPtr);
		/*coverity-CID: 10834, Out-of-bounds write*/
		if(num >= AP_ETH_IF_NUM)
		{
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemAPExtensionInfo ap send eth num error\n");
			return CW_FALSE;
		}
		valPtr->eth_updown_time[num] = CWProtocolRetrieve8(msgPtr);
		wid_syslog_debug_debug(WID_WTPINFO,"num %d valPtr->eth_updown_time %d\n",num,valPtr->eth_updown_time[num]);
	}
	valPtr->ath_count = CWProtocolRetrieve8(msgPtr);
	if(valPtr->ath_count > AP_ATH_IF_NUM){
		valPtr->ath_count = 1;
		wid_syslog_err("CWParseMsgElemAPExtensionInfo ap send ath num error\n");
		return CW_FALSE;
	}
	for(i=0;i<valPtr->ath_count;i++)
	{
		valPtr->ath_if_info[i].radioid = CWProtocolRetrieve8(msgPtr);
		valPtr->ath_if_info[i].wlanid = CWProtocolRetrieve8(msgPtr);
		valPtr->ath_if_info[i].ath_updown_times = CWProtocolRetrieve8(msgPtr);
		if((valPtr->ath_if_info[i].radioid > L_RADIO_NUM)||(valPtr->ath_if_info[i].wlanid > WLAN_NUM))
		{
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemAPExtensionInfo ap send radio num error\n");
			return CW_FALSE;
		}
		wid_syslog_debug_debug(WID_WTPINFO,"radioid %d wlanid %d ath_updown_times %d\n",valPtr->ath_if_info[i].radioid,valPtr->ath_if_info[i].wlanid,valPtr->ath_if_info[i].ath_updown_times);
	}
	//sz add  04-15
	valPtr->wifi_count = CWProtocolRetrieve8(msgPtr);
	if(valPtr->wifi_count > AP_WIFI_IF_NUM){
		valPtr->wifi_count = 1;
		wid_syslog_err("CWParseMsgElemAPExtensionInfo ap send wifi num error\n");
		return CW_FALSE;
	}
	for(i=0;i<valPtr->wifi_count;i++)
	{
		num = CWProtocolRetrieve8(msgPtr);
		/*coverity-CID: 10835, Out-of-bounds write*/
		if(num >= AP_WIFI_IF_NUM)
		{
			wid_syslog_err("CWParseMsgElemAPExtensionInfo ap send wifi num error\n");
			return CW_FALSE;
		}
		valPtr->wifi_state[num] = CWProtocolRetrieve8(msgPtr);
		wid_syslog_debug_debug(WID_WTPINFO,"num %d valPtr->wifi_state %d\n",num,valPtr->wifi_state[num]);
	}
	/*fengwenchao add 20120314 for onlinebug-162*///qiuchen copy from v1.3
	if((msgPtr->offset - start)<len)
	{
		for(i=0;i<valPtr->wifi_count;i++)
		{
			valPtr->wifi_snr_new[i] = CWProtocolRetrieve8(msgPtr);
			valPtr->wifi_noise_new[i] = CWProtocolRetrieve16(msgPtr);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemAPExtensionInfo ap send radio %d wifi_snr_new  %d\n",i,valPtr->wifi_snr_new[i]);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemAPExtensionInfo ap send radio %d wifi_noise_new  %d\n",i,valPtr->wifi_noise_new[i]);
		}	
	}
	else
	{
		for(i=0;i<valPtr->wifi_count;i++){
			valPtr->wifi_snr_new[i] = valPtr->wifi_snr;
			valPtr->wifi_noise_new[i] = 95;
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemAPExtensionInfo not receiv  %d wifi_snr_new  %d\n",i,valPtr->wifi_snr_new[i]);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemAPExtensionInfo not receiv %d wifi_noise_new	%d\n",i,valPtr->wifi_noise_new[i]);
		}
	}

	if(((msgPtr->offset) - oldOffset) == len)
	{
		return CW_TRUE;
	}
	else
	{
		if(((msgPtr->offset) - oldOffset) > len)
		{
			wid_syslog_err("%s,%d,parse more than msg,((msgPtr->offset) - oldOffset)=%d,len=%d.\n",__func__,__LINE__,((msgPtr->offset) - oldOffset),len);
		}
		else
		{
			wid_syslog_err("%s,%d,parse less than msg,((msgPtr->offset) - oldOffset)=%d,len=%d.\n",__func__,__LINE__,((msgPtr->offset) - oldOffset),len);
		}
		return CW_FALSE;
	}
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemAPStaWapiInfos(CWProtocolMessage *msgPtr, int len, WIDStaWapiInfoList *valPtr,int wtpindex)
{
	wid_syslog_debug_debug(WID_DEFAULT,"*** CWParseMsgElemAPStaWapiInfos ***\n");
	wid_syslog_debug_debug(WID_DEFAULT,"*** len:%d ***\n",len);
	int i = 0;
	
	CWParseMessageElementStart();
	valPtr->WTPID = wtpindex;
	valPtr->sta_num = CWProtocolRetrieve8(msgPtr);
	
	wid_syslog_debug_debug(WID_DEFAULT,"eth_num %d\n",AC_WTP[wtpindex]->apifinfo.eth_num);
	
	for(i=0;i<(valPtr->sta_num)&& i < 64;i++)
	{
		valPtr->StaWapiInfo[i].RadioId = CWProtocolRetrieve8(msgPtr);		
		valPtr->StaWapiInfo[i].WlanId = CWProtocolRetrieve8(msgPtr);		
		char * str = CWProtocolRetrieveStr(msgPtr,6);
		memcpy(valPtr->StaWapiInfo[i].mac,str, 6);
		free(str);
		valPtr->StaWapiInfo[i].WAPIVersion = CWProtocolRetrieve32(msgPtr); 
		valPtr->StaWapiInfo[i].ControlledPortStatus = CWProtocolRetrieve8(msgPtr);		
		str =  CWProtocolRetrieveStr(msgPtr,4);
		memcpy(valPtr->StaWapiInfo[i].SelectedUnicastCipher,str, 4);
		free(str);		
		valPtr->StaWapiInfo[i].WPIReplayCounters= CWProtocolRetrieve32(msgPtr); 
		valPtr->StaWapiInfo[i].WPIDecryptableErrors = CWProtocolRetrieve32(msgPtr); 
		valPtr->StaWapiInfo[i].WPIMICErrors = CWProtocolRetrieve32(msgPtr); 	
		printf("wtpid %d,radioid %d,wlanid %d\nsta mac %02X:%02X:%02X:%02X:%02X:%02X\nversion %d,controlStatus %d,cipher %02X-%02X-%02X-%02X, replay %d,decryerror %d,micerror %d",
			wtpindex,valPtr->StaWapiInfo[i].RadioId,valPtr->StaWapiInfo[i].WlanId,valPtr->StaWapiInfo[i].mac[0],valPtr->StaWapiInfo[i].mac[1],valPtr->StaWapiInfo[i].mac[2],valPtr->StaWapiInfo[i].mac[3],valPtr->StaWapiInfo[i].mac[4],valPtr->StaWapiInfo[i].mac[5],
		valPtr->StaWapiInfo[i].WAPIVersion,valPtr->StaWapiInfo[i].ControlledPortStatus,valPtr->StaWapiInfo[i].SelectedUnicastCipher[0],valPtr->StaWapiInfo[i].SelectedUnicastCipher[1],valPtr->StaWapiInfo[i].SelectedUnicastCipher[2],valPtr->StaWapiInfo[i].SelectedUnicastCipher[3],
		valPtr->StaWapiInfo[i].WPIReplayCounters,valPtr->StaWapiInfo[i].WPIDecryptableErrors,valPtr->StaWapiInfo[i].WPIMICErrors);
	}
	WidAsdStaWapiInfoUpdate(wtpindex, valPtr);
	CWParseMessageElementEnd(); 
}	


CWBool CWParseSessionID(CWProtocolMessage *msgPtr, int len, CWProtocolJoinRequestValues *valPtr) {
	
	CWParseMessageElementStart();
	
	valPtr->sessionID= CWProtocolRetrieve32(msgPtr);
	
	CWParseMessageElementEnd();
}



void sa_res_deinit(struct sa_res **sa_res)
{
	unsigned int i = 0;
	struct sa_res *res;

	if (!sa_res)
	{
		return;
	}
	
	res = *sa_res;

	if (!res)
	{
		return;
	}

	if (res->info)
	{
		for (i = 0; i < res->channel_num; i++)
		{
		
			if (res->info)
			{
				if (res->info[i].info)
				{
					res->info[i].disturb_num = 0;
					free(res->info[i].info);
					res->info[i].info = NULL;
				}
			}
		}
		res->channel_num = 0;
		free(res->info);
		res->info = NULL;
	}
	
	free(res);
	*sa_res = NULL;
}

CWBool CWParseSpectrumAnalysis_report(CWProtocolMessage *msgPtr, int len, int wtpid)
{
	unsigned int i = 0, j = 0, k = 0;
	unsigned char radio_num = 0;
	unsigned int radioid = 0;
	unsigned char l_radioid = 0;
	unsigned char channel_num = 0;
	unsigned char disturb_num = 0;
	unsigned int size = 0;

	struct sa_res *res = NULL, *prev_res = NULL;
	struct sa_channel_info *channel_info = NULL;
	struct sa_disturb_info *disturb_info = NULL;

	WID_CHECK_WTP_STANDARD_RET(wtpid, CW_FALSE);

	radio_num = CWProtocolRetrieve8(msgPtr);

	for (i = 0; i < radio_num; i++)
	{
		l_radioid = CWProtocolRetrieve8(msgPtr);
		radioid = wtpid * L_RADIO_NUM + l_radioid;
		if (NULL == AC_RADIO[radioid])
		{
			return CW_FALSE;
		}
		res = (struct sa_res *)malloc(sizeof(struct sa_res));
		if (!res)
		{
			wid_syslog_err("%s:%d malloc failed: %s\n", __func__, __LINE__, strerror(errno));
			return CW_FALSE;
		}
		memset(res, 0, sizeof(struct sa_res));
		
		channel_num = CWProtocolRetrieve8(msgPtr);

		size = sizeof(struct sa_channel_info) * channel_num;
		channel_info = (struct sa_channel_info *)malloc(size);
		if (!channel_info)
		{
			wid_syslog_err("%s:%d malloc failed: %s\n", __func__, __LINE__, strerror(errno));
			sa_res_deinit(&res);
			return CW_FALSE;
		}
		memset(channel_info, 0, size);
		res->info = channel_info;		
		res->channel_num = channel_num;
		
		wid_syslog_debug_debug(WID_WTPINFO,"%s: radio %d-%d channel num %d\n", __func__,
											wtpid, l_radioid, channel_num);
		
		for (j = 0; j < channel_num; j++)
		{
			channel_info[j].channel = CWProtocolRetrieve8(msgPtr);
			disturb_num = CWProtocolRetrieve8(msgPtr);

			wid_syslog_debug_debug(WID_WTPINFO,"channel %d disturb num %d \n", 
				channel_info[j].channel, disturb_num);
			
			size = sizeof(struct sa_disturb_info) * disturb_num;
			disturb_info = (struct sa_disturb_info *)malloc(size);
			if (!disturb_info)
			{
				wid_syslog_err("%s:%d malloc failed: %s\n", __func__, __LINE__, strerror(errno));
				sa_res_deinit(&res);
				return CW_FALSE;
			}
			memset(disturb_info, 0, size);
			channel_info[j].info = disturb_info;
			channel_info[j].disturb_num = disturb_num;

			for (k = 0; k < channel_info[j].disturb_num; k++)
			{
				disturb_info[k].type = CWProtocolRetrieve8(msgPtr);
				disturb_info[k].cnt = CWProtocolRetrieve16(msgPtr);
				disturb_info[k].rssi = CWProtocolRetrieve8(msgPtr);
				
				wid_syslog_debug_debug(WID_WTPINFO,"cnt %d rssi %d type %d\n", 
					disturb_info[k].cnt, disturb_info[k].rssi, disturb_info[k].type);
			}
		}
		prev_res = AC_RADIO[radioid]->spec_analysis.res;
		AC_RADIO[radioid]->spec_analysis.res = res;
		sa_res_deinit(&prev_res);
	}
	return CW_TRUE;
}

CWBool CWParseWTPStatisticsTimer (CWProtocolMessage *msgPtr, int len, int *valPtr)
{
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieve16(msgPtr);
	
	//wid_syslog_debug_debug("WTP Statistics Timer: %d", *valPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPBoardData (CWProtocolMessage *msgPtr, int len, CWWTPVendorInfos *valPtr)
{
	int theOffset = 0, i = 0, vendorID = 0;
	char *strmodel = NULL;
	char *codever = NULL;
	unsigned short IGNORE_UBSV unserved = 0;
	char *SN_t = NULL;
	CWParseMessageElementStart();
	
	valPtr->vendorInfosCount = 0;
	
	// see how many vendor ID we have in the message
	vendorID = CWProtocolRetrieve32(msgPtr); // ID
	theOffset = msgPtr->offset;
	while ((msgPtr->offset-oldOffset) < len)	// oldOffset stores msgPtr->offset's value at the beginning of this function.
	{
		// See the definition of the CWParseMessageElementStart() macro.
		int tmp = 0;

		CWProtocolRetrieve16(msgPtr); // type
		tmp = CWProtocolRetrieve16(msgPtr);
		msgPtr->offset += tmp; // len
		valPtr->vendorInfosCount++;
	}
	
	msgPtr->offset = theOffset;
	// actually read each vendor ID
	CW_CREATE_ARRAY_ERR(valPtr->vendorInfos, valPtr->vendorInfosCount, CWWTPVendorInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	memset(valPtr->vendorInfos,0,sizeof(CWWTPVendorInfoValues)*valPtr->vendorInfosCount);

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"has %u board data with vendor %u\n", \
						WAI_FUNC_LINE_LITERAL, 0, valPtr->vendorInfosCount, vendorID);
	for (i = 0; i < valPtr->vendorInfosCount; i++)
	{
		(valPtr->vendorInfos)[i].vendorIdentifier = vendorID;
		(valPtr->vendorInfos)[i].type = CWProtocolRetrieve16(msgPtr);																
		(valPtr->vendorInfos)[i].length = CWProtocolRetrieve16(msgPtr);
		if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{	
			if (((valPtr->vendorInfos)[i].length  < 0)||((valPtr->vendorInfos)[i].length > 64))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"sn length %u error\n", \
						WAI_FUNC_LINE_LITERAL, 0, (valPtr->vendorInfos)[i].length);
				(valPtr->vendorInfos)[i].length = 64;
			}

			SN_t = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos)[i].length));	
			(valPtr->vendorInfos)[i].SN = (unsigned char *)malloc(((valPtr->vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos)[i].SN,0,(((valPtr->vendorInfos)[i].length)+1));
			if (wid_illegal_character_check(SN_t, strlen(SN_t),0) == 1)
			{
				memcpy((valPtr->vendorInfos)[i].SN,SN_t,(valPtr->vendorInfos)[i].length);
			}
			else
			{
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"sn has non readable char\n", \
						WAI_FUNC_LINE_LITERAL, 0);
			}

			CW_FREE_OBJECT(SN_t);

			//(valPtr->vendorInfos)[i].SN = (unsigned char*)(CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos)[i].length));	
			if ((valPtr->vendorInfos)[i].SN == NULL)
			{
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
    		}
    		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: sn %s\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].SN);
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			if (((valPtr->vendorInfos)[i].length < 6)||((valPtr->vendorInfos)[i].length > 64))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u:mac length %u error\n", \
								WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].length);
				(valPtr->vendorInfos)[i].length = 64;
			}

			(valPtr->vendorInfos)[i].mac = (unsigned char*)(CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos)[i].length));
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: mac "WAI_MAC_FMT"\n", \
						WAI_FUNC_LINE_LITERAL, 0, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac));
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			if (((valPtr->vendorInfos)[i].length < 0)||((valPtr->vendorInfos)[i].length > 64))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: model length %u error\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].length);
				(valPtr->vendorInfos)[i].length = 64;
			}

			strmodel = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos)[i].length));	

			(valPtr->vendorInfos)[i].model = (unsigned char *)malloc(((valPtr->vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos)[i].model,0,(((valPtr->vendorInfos)[i].length)+1));
			if (wid_illegal_character_check(strmodel, strlen(strmodel),0) == 1)
			{
				memcpy((valPtr->vendorInfos)[i].model,strmodel,(((valPtr->vendorInfos)[i].length)));
			}
			else
			{
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: model has non readable char\n", \
							WAI_FUNC_LINE_LITERAL, 0, i);
			}
			CW_FREE_OBJECT(strmodel);

			if ((valPtr->vendorInfos)[i].model == NULL)
			{
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
			if ((valPtr->vendorInfos)[i].length == 4)
			{
				(valPtr->vendorInfos)[i].valuePtr = (int*) ((valPtr->vendorInfos)[i].model);
			}
			else
			{
				(valPtr->vendorInfos)[i].valuePtr = NULL;
			}

			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: model %s\n", \
						WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].model);
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			if (((valPtr->vendorInfos)[i].length < 0)||((valPtr->vendorInfos)[i].length > 64))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: real model length %u error\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].length);
				(valPtr->vendorInfos)[i].length = 64;
			}
			strmodel = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos)[i].length));	

			(valPtr->vendorInfos)[i].Rmodel = (unsigned char *)malloc(((valPtr->vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos)[i].Rmodel, 0, (((valPtr->vendorInfos)[i].length)+1));
			if (wid_illegal_character_check(strmodel, strlen(strmodel),0) == 1)
			{
				memcpy((valPtr->vendorInfos)[i].Rmodel,strmodel,(((valPtr->vendorInfos)[i].length)));
			}
			else
			{
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: real model has non readable char\n", \
							WAI_FUNC_LINE_LITERAL, 0, i);
			}
			CW_FREE_OBJECT(strmodel);
			
			if ((valPtr->vendorInfos)[i].Rmodel == NULL)
			{
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
			if ((valPtr->vendorInfos)[i].length == 4)
			{
				(valPtr->vendorInfos)[i].valuePtr = (int*) ((valPtr->vendorInfos)[i].Rmodel);
			}
			else
			{
				(valPtr->vendorInfos)[i].valuePtr = NULL;
			}			
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: real model %s\n", \
						WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].Rmodel);
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_CODE_VERSION)
		{
			if (((valPtr->vendorInfos)[i].length < 0)||((valPtr->vendorInfos)[i].length > 64))
			{
				wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: code version length %u error\n",\
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].length);
				(valPtr->vendorInfos)[i].length = 64;
			}
			codever = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos)[i].length));	

			(valPtr->vendorInfos)[i].codever = (unsigned char *)malloc(((valPtr->vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos)[i].codever, 0, (((valPtr->vendorInfos)[i].length)+1));
			if (wid_illegal_character_check(codever, strlen(codever), 0) == 1)
			{
				memcpy((valPtr->vendorInfos)[i].codever,codever,(((valPtr->vendorInfos)[i].length)));
			}
			else
			{
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: code version has non readable char\n", \
							WAI_FUNC_LINE_LITERAL, 0, i);
			}
			CW_FREE_OBJECT(codever);
			if ((valPtr->vendorInfos)[i].codever == NULL)
			{
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: code version %s\n", \
								WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos)[i].codever);
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_MANUFACTURE_OPTION)
		{
			(valPtr->vendorInfos)[i].manuoption =  CWProtocolRetrieve16(msgPtr);
			unserved = 	CWProtocolRetrieve16(msgPtr);		
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u: vendor option %u\n", \
								WAI_FUNC_LINE_LITERAL, 0, i,(valPtr->vendorInfos)[i].manuoption);
		}		
		else
		{			
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"board data %u:ignore unknown type %u\n", \
								WAI_FUNC_LINE_LITERAL, 0, i,(valPtr->vendorInfos)[i].type);
		}
	}

	CWParseMessageElementEnd();
}

CWBool CWCheckWTPBoardData(int WTPIndex, CWWTPVendorInfos *valPtr)
{
	//int mac = -1;
	int i = 0;
	int isbasemac = 0;
	char *savesn = NULL;

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check %u board data with "WAI_AFI0_FMT"state(%u) %s\n", \
									WAI_FUNC_LINE_LITERAL, valPtr->vendorInfosCount, WTPIndex, \
									AC_WTP[WTPIndex]->WTPStat, WAI_STATE_STR(AC_WTP[WTPIndex]->WTPStat));
	for (i = 0; i < valPtr->vendorInfosCount; i++)
	{
		if ((valPtr->vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			if ((AC_WTP[WTPIndex]->APCode != NULL)
				&& (memcmp((valPtr->vendorInfos)[i].model, AC_WTP[WTPIndex]->APCode, strlen(AC_WTP[WTPIndex]->APCode)) == 0))
			{
				continue;
			}
			else
			{
				//special upgrade 20081007 by weiay
				if ((((valPtr->vendorInfos)[i].valuePtr != NULL) && (*((valPtr->vendorInfos)[i].valuePtr) == 1000))
					|| ((AC_WTP[WTPIndex]->WTPModel != NULL) && (strncasecmp((char*)((valPtr->vendorInfos)[i].model), AC_WTP[WTPIndex]->WTPModel, strlen(AC_WTP[WTPIndex]->WTPModel)) == 0)))
				{
					continue;
				}
				wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u model %s with "WAI_AFI0_FMT" model %s not match\n", \
								WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].model,WTPIndex,  AC_WTP[WTPIndex]->APCode);
			}
		}	
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			char * sn = (char *)((valPtr->vendorInfos)[i].SN);
			str2higher(&sn);
			(valPtr->vendorInfos)[i].SN = (unsigned char *)sn;
		
			if ((AC_WTP[WTPIndex]->WTPSN != NULL)
				&& (memcmp((valPtr->vendorInfos)[i].SN, AC_WTP[WTPIndex]->WTPSN, strlen(AC_WTP[WTPIndex]->WTPSN)) == 0))    
			{
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u sn %s with "WAI_AFI0_FMT"found match\n", \
								WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].SN, WTPIndex, AC_WTP[WTPIndex]->WTPSN);
				isbasemac = 1;//wuwl add 20101118 to resolv prolem for jinhua,ng'ap have the same sn.wtp1 maybe run into another wtp.
				continue;
			}
			else
			{
				CW_FREE_OBJECT(savesn);
				if ((AC_WTP[WTPIndex]->WTPSN != NULL)
					&& (memcmp(AC_WTP[WTPIndex]->WTPSN, gdefaultsn, strlen(AC_WTP[WTPIndex]->WTPSN)) == 0))    
				{
					savesn = (char*)malloc((valPtr->vendorInfos)[i].length +1);
					memset(savesn, 0, (valPtr->vendorInfos)[i].length+1);
					/*??*/
					if ((valPtr->vendorInfos)[i].length > NAS_IDENTIFIER_NAME)
					{
						memcpy(savesn, (valPtr->vendorInfos)[i].SN, NAS_IDENTIFIER_NAME);
					}
					else
					{
						memcpy(savesn, (valPtr->vendorInfos)[i].SN, (valPtr->vendorInfos)[i].length);
					}
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u sn %s with "WAI_AFI0_FMT"but afi has default sn\n", \
									WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].SN, WTPIndex, AC_WTP[WTPIndex]->WTPSN);
					isbasemac = 1;
					continue;
				}
				else
				{
					savesn = (char*)malloc((valPtr->vendorInfos)[i].length +1);
					memset(savesn, 0, (valPtr->vendorInfos)[i].length+1);
					if ((valPtr->vendorInfos)[i].length > NAS_IDENTIFIER_NAME)
					{
						wid_syslog_info("SN length %d\n",(valPtr->vendorInfos)[i].length);
						memcpy(savesn, (valPtr->vendorInfos)[i].SN, NAS_IDENTIFIER_NAME);
					}
					else
					{
						memcpy(savesn, (valPtr->vendorInfos)[i].SN, (valPtr->vendorInfos)[i].length);
					}
					wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u sn %s with "WAI_AFI0_FMT"sn %s but not match\n", \
									WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].SN, WTPIndex, AC_WTP[WTPIndex]->WTPSN);
					isbasemac = 1;
					continue;
				}
			}
				
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_ID)
		{
			continue;
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_REVISION)
		{
			continue;
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			if (isbasemac == 1)
			{
				isbasemac = 0;

				if (0 == memcmp((valPtr->vendorInfos)[i].mac, AC_WTP[WTPIndex]->WTPMAC, MAC_LEN))
				{
					if (savesn != NULL)
					{
						if (wid_illegal_character_check(savesn, strlen(savesn),0) == 1)
						{
							if (AC_WTP[WTPIndex]->WTPSN != NULL)
							{
								memset(AC_WTP[WTPIndex]->WTPSN, 0, NAS_IDENTIFIER_NAME);
								memcpy(AC_WTP[WTPIndex]->WTPSN, savesn, strlen(savesn));							
								wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u mac "WAI_MAC_FMT"with "WAI_AFI0_FMT"found match we record sn\n", \
												WAI_FUNC_LINE_LITERAL, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac), WTPIndex);
							}
						}
						else
						{
							wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u mac "WAI_MAC_FMT"with "WAI_AFI0_FMT"found match but give up illegal sn\n", \
											WAI_FUNC_LINE_LITERAL, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac), WTPIndex);
						}
						CW_FREE_OBJECT(savesn);
					}
					else
					{
						wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u mac "WAI_MAC_FMT"with "WAI_AFI0_FMT"found match\n", \
									WAI_FUNC_LINE_LITERAL, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac), WTPIndex);
					}
					continue;
				}
				else
				{
					CW_FREE_OBJECT(savesn);
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u mac "WAI_MAC_FMT"with "WAI_AFI0_FMT"mac "WAI_MAC_FMT" found not match\n", \
									WAI_FUNC_LINE_LITERAL, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac),  \
									WTPIndex, WAI_MAC_LITERAL(AC_WTP[WTPIndex]->WTPMAC));
					return CW_FALSE;
				}
			}
			else
			{
				memcpy(AC_WTP[WTPIndex]->WTPMAC, (valPtr->vendorInfos)[i].mac, MAC_LEN);
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u mac "WAI_MAC_FMT"with "WAI_AFI0_FMT"we only record\n", \
								WAI_FUNC_LINE_LITERAL, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac), WTPIndex);
				continue;
			}
		}
		else if ((valPtr->vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			if ((AC_WTP[WTPIndex]->WTPModel != NULL)
				&& ((valPtr->vendorInfos)[i].Rmodel != NULL)
				&& (memcmp((valPtr->vendorInfos)[i].Rmodel, AC_WTP[WTPIndex]->WTPModel, strlen(AC_WTP[WTPIndex]->WTPModel)) == 0))
			{
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u real model %s with "WAI_AFI0_FMT"found match\n", \
						WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].Rmodel, WTPIndex);
			    	continue;
			}
			else
			{
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u real model %s with "WAI_AFI0_FMT"model %s found not match\n", \
						WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].Rmodel, WTPIndex, AC_WTP[WTPIndex]->WTPModel);
				CW_FREE_OBJECT(savesn); 
				return CW_FALSE;
			}
		}
		else
		{
			continue;
		}
	}

	CW_FREE_OBJECT(savesn); 
	return CW_TRUE;
}


CWBool CWParseWtp_Sta_Ptk_Report(CWProtocolMessage *msgPtr, int len, WIDStationInfo *valPtr, int wtpindex)
{		
	WID_CHECK_WTP_STANDARD_RET(wtpindex, CW_FALSE);
	
	CWParseMessageElementStart();

	unsigned char *mac = NULL;
	
	valPtr->radioId = CWProtocolRetrieve8(msgPtr);
	valPtr->wlanId = CWProtocolRetrieve8(msgPtr);
		
	mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	memcpy(valPtr->mac, mac, MAC_LEN);
	free(mac);
	mac = NULL;
	
	valPtr->ptk_res = CWProtocolRetrieve8(msgPtr);
	
	wid_syslog_debug_debug(WID_DEFAULT, "%s radio %d wlan %d sta mac %02X:%02X:%02X:%02X:%02X:%02X ptk result %d\n",
							__func__, valPtr->radioId, valPtr->wlanId,	
							valPtr->mac[0],valPtr->mac[1],valPtr->mac[2],valPtr->mac[3],valPtr->mac[4],valPtr->mac[5],
							valPtr->ptk_res);
	
	WID_CHECK_L_RADIOID_STANDARD_RET((valPtr->radioId), CW_FALSE);
	WID_CHECK_WLAN_EXIST_RET((valPtr->wlanId), CW_FALSE);
	
	WidAsd_StationKeyNegotiReport(wtpindex, valPtr);
	CWParseMessageElementEnd(); 
}

CWBool CWCheckRunWTPBoardData(int WTPIndex, CWWTPVendorInfos *valPtr, WTPLoginState WTP_LOGIN)
{
	int i = 0;
	msgq msg;
	int WTPMsgqID = 0;
	CWGetMsgQueue(&WTPMsgqID);

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check %u board data with "WAI_AFI0_FMT"state(%u) %s\n", \
									WAI_FUNC_LINE_LITERAL, valPtr->vendorInfosCount, WTPIndex, \
									AC_WTP[WTPIndex]->WTPStat, WAI_STATE_STR(AC_WTP[WTPIndex]->WTPStat));
	for (i = 0; i < valPtr->vendorInfosCount; i++)
	{
		if ((valPtr->vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			char * sn = (char *)((valPtr->vendorInfos)[i].SN);
			str2higher(&sn);
			(valPtr->vendorInfos)[i].SN = (unsigned char *)sn;
		
			if (memcmp((valPtr->vendorInfos)[i].SN, AC_WTP[WTPIndex]->WTPSN, strlen(AC_WTP[WTPIndex]->WTPSN)) == 0)
			{
				continue;
			}
			else
			{
				wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u sn %s with "WAI_AFI0_FMT"sn %s but not match\n", \
								WAI_FUNC_LINE_LITERAL, i, (valPtr->vendorInfos)[i].SN, WTPIndex, AC_WTP[WTPIndex]->WTPSN);
				return CW_FALSE;
			}						
		}
		else if ((valPtr->vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			if (0 == memcmp((valPtr->vendorInfos)[i].mac, AC_WTP[WTPIndex]->WTPMAC, MAC_LEN))
			{
				continue;
			}
			else
			{
				wid_syslog_warning(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u mac "WAI_MAC_FMT"with "WAI_AFI0_FMT"mac "WAI_MAC_FMT" found not match\n", \
								WAI_FUNC_LINE_LITERAL, i, WAI_MAC_LITERAL((valPtr->vendorInfos)[i].mac),  \
								WTPIndex, WAI_MAC_LITERAL(AC_WTP[WTPIndex]->WTPMAC));
				return CW_FALSE;
			}
		}
		else
		{
			continue;
		}
	}
	
	if (WTP_LOGIN == WID_RELOGIN)
	{
		wid_syslog_info(WAI_FUNC_LINE_FMT" afi%d "MACSTR" state offline check rediscovery, wait ap relogin\n",
						WAI_FUNC_LINE_LITERAL, WTPIndex, MAC2STR(AC_WTP[WTPIndex]->WTPMAC));
	}
    else
    {
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"check board data%u found mac and sn match with "WAI_AFI0_FMT"state(%d) %s we try to reboot it\n", \
						WAI_FUNC_LINE_LITERAL, i, WTPIndex, AC_WTP[WTPIndex]->WTPStat, WAI_STATE_STR(AC_WTP[WTPIndex]->WTPStat));
		
		gWTPs[WTPIndex].isRequestClose = CW_TRUE;	
		memset((char*)&msg, 0, sizeof(msg));
		msg.mqid = WTPIndex%THREAD_NUM+1;
		msg.mqinfo.WTPID = WTPIndex;
		msg.mqinfo.type = CONTROL_TYPE;
		msg.mqinfo.subtype = WTP_S_TYPE;
		msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
		msg.mqinfo.u.WtpInfo.WTPID = WTPIndex;
		if (msgsnd(WTPMsgqID, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
		{
			wid_syslog_crit(WAI_FUNC_LINE_FMT" afi%d msgsend error(%d) %s",WAI_FUNC_LINE_LITERAL, WTPIndex, errno, strerror(errno));
		}
	}
	
	return CW_TRUE;
}

int CWCmpWTPAttach(CWNetworkLev4Address *addrPtr)
{
	int ret = 0;
	int i = 0;

	CWThreadMutexLock(&AttachMutex);
	for(i = 0; i < WTP_NUM; i++)
	{
		if(AC_ATTACH[i] != NULL
		    && (&(AC_ATTACH[i]->address) != NULL)
			&& !sock_cmp_addr((struct sockaddr*)addrPtr, (struct sockaddr*)&(AC_ATTACH[i]->address), sizeof(CWNetworkLev4Address)))
		{
			ret = AC_ATTACH[i]->WTPID;

			CWUseSockNtop(addrPtr,  \
				wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"from %s match attach table %u we drop attach\n",WAI_FUNC_LINE_LITERAL, ret, str, i););
			
			if(!check_wtpid_func(ret))
			{
				CWUseSockNtop(addrPtr,
					wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"from %s found id illegal when drop attach\n",  \
							WAI_FUNC_LINE_LITERAL, ret, str););
				CWThreadMutexUnlock(&AttachMutex);
				return -1;
			}
			
			if((AC_WTP[ret] == NULL)||((AC_WTP[ret] != NULL)&&(AC_WTP[ret]->WTPStat != WID_QUIT)))
			{
				CW_FREE_OBJECT(AC_ATTACH[i]);
				continue;
			}
			
			CW_FREE_OBJECT(AC_ATTACH[i]);
			CWThreadMutexUnlock(&AttachMutex);
			return ret;
		}
	}
	CWThreadMutexUnlock(&AttachMutex);
	
	return -1;
}


CWBool CWDisCheckWTPBoardData
(
	int bindingSystemIndex,
	CWNetworkLev4Address *addrPtr,
	CWWTPVendorInfos *valPtr,
	unsigned int *WTPID,
	WTPLoginState WTP_LOGIN
)
{
	int i = 1, j = 0, i1 = 0;
	
	CWThreadMutexLock(&AttachMutex);
	for (i1 = 0; i1 < WTP_NUM; i1++)
	{
		if (AC_ATTACH[i1] != NULL
			&& (&(AC_ATTACH[i1]->address) != NULL)
			&& !sock_cmp_addr((struct sockaddr*)addrPtr, (struct sockaddr*)&(AC_ATTACH[i1]->address), sizeof(CWNetworkLev4Address)))
		{
			*WTPID = AC_ATTACH[i1]->WTPID;

			CWUseSockNtop(addrPtr, wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"from %s check address match attach(%u) found "WAI_AFI0_FMT" \n", \
							WAI_FUNC_LINE_LITERAL, str,  i1, *WTPID););
			CWThreadMutexUnlock(&AttachMutex);
			return CW_TRUE;
		}
	}
	CWThreadMutexUnlock(&AttachMutex);

	int n = 0, k = 0;
	unsigned char mac[MAC_LEN] = {0};
	unsigned char zero[MAC_LEN] = {0};
	CWTimerID id = 0;
	
	for (n = 0; n < valPtr->vendorInfosCount; n++)
	{
		if ((valPtr->vendorInfos)[n].type == CW_BOARD_MAC_ADDRESS)
		{
			memcpy(mac, (valPtr->vendorInfos)[n].mac, MAC_LEN); 
			break;
		}
	}

    CWThreadMutexLock(&AttachMutex);
	if (0 != memcmp(zero, mac, MAC_LEN))
	{
		for (k = 0; k < WTP_NUM; k++)
		{
			if (AC_ATTACH[k] != NULL)
			{
				if (0 == memcmp(AC_ATTACH[k]->wtp_mac, mac, MAC_LEN))
				{
					CW_FREE_OBJECT(AC_ATTACH[k]);
					CWUseSockNtop(addrPtr, wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT""WAI_DISCOVERY_LITERAL"from %s check board data mac "WAI_MAC_FMT"match attach(%u) we drop attach\n", \
									WAI_FUNC_LINE_LITERAL, 0 , str, WAI_MAC_LITERAL(mac), k););
				}
			}
		}
	}
	CWThreadMutexUnlock(&AttachMutex);
			
	while (i < WTP_NUM)
	{
		if ((AC_WTP[i] != NULL) && (AC_WTP[i]->isused == 1))
		{
			if (((AC_WTP[i]->WTPStat == WID_QUIT) && (CWCheckWTPBoardData(i, valPtr)))
				|| ((AC_WTP[i]->WTPStat != WID_QUIT) && (CWCheckRunWTPBoardData(i, valPtr, WTP_LOGIN))))
			{
				CWThreadMutexLock(&AttachMutex);
				while (j < WTP_NUM)
				{
					if (AC_ATTACH[j] == NULL)
					{						
						AC_ATTACH[j] = (CWWTPAttach*)malloc(sizeof(CWWTPAttach));
						AC_ATTACH[j]->WTPID = i;
						CW_COPY_NET_ADDR_PTR(&(AC_ATTACH[j]->address), addrPtr);
						memcpy(AC_ATTACH[j]->wtp_mac, mac, MAC_LEN);
						memcpy(gWTPs[i].wtp_mac, mac, MAC_LEN);
						*WTPID = i;

						CWTimerRequest(MAX_SURVIVAL_TIME, NULL, &id, TIMER_TYPE_AC_ATTCH, j);
						CWThreadMutexUnlock(&AttachMutex);
						CWUseSockNtop(addrPtr, wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"from %s check board data match "WAI_AFI0_FMT"build attach(%u)\n", \
										WAI_FUNC_LINE_LITERAL, str, *WTPID,	j););
						return CW_TRUE;
					}
					j++;
				}
				CWThreadMutexUnlock(&AttachMutex);
				CWUseSockNtop(addrPtr, wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_DISCOVERY_LITERAL"from %s check board data match "WAI_AFI0_FMT"but attach full\n", \
								WAI_FUNC_LINE_LITERAL, str, *WTPID, j););
				return CW_FALSE;
			}
		}
		i++;
	}
	return CW_FALSE;
}

CWBool CWAddAC_ATTACH_For_Auto
(
	CWNetworkLev4Address *addrPtr,
	unsigned int WTPID,
	unsigned char *mac
)
{
	int j = 0, i = 0;	
	int id = 0;

	CWThreadMutexLock(&AttachMutex);
	for(i = 0; i < WTP_NUM; i++)
	{
		if(AC_ATTACH[i] != NULL
			&& (&(AC_ATTACH[i]->address) != NULL)
			&& !sock_cmp_addr((struct sockaddr*)addrPtr, (struct sockaddr*)&(AC_ATTACH[i]->address), sizeof(CWNetworkLev4Address)))
		{
			CWUseSockNtop(addrPtr, wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"from %s auto login match attach %u\n", \
							WAI_FUNC_LINE_LITERAL, WTPID,str, i););
		            CWThreadMutexUnlock(&AttachMutex);
			return CW_TRUE;
		}
	}
	CWThreadMutexUnlock(&AttachMutex);
	
	if((AC_WTP[WTPID] != NULL)&&(AC_WTP[WTPID]->WTPStat == WID_QUIT)&&(AC_WTP[WTPID]->isused == 1))
	{
	    CWThreadMutexLock(&AttachMutex);
		while(j < WTP_NUM)
		{
			if(AC_ATTACH[j] == NULL)
			{						
				AC_ATTACH[j] = (CWWTPAttach*)malloc(sizeof(CWWTPAttach));
				AC_ATTACH[j]->WTPID = WTPID;
				CW_COPY_NET_ADDR_PTR(&(AC_ATTACH[j]->address), addrPtr);	
				memcpy(AC_ATTACH[j]->wtp_mac, mac, MAC_LEN);
				CWUseSockNtop(addrPtr,
					wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"from %s found attach exists we assign new attach %u\n", \
									WAI_FUNC_LINE_LITERAL, WTPID, str,  j););
				
				CWTimerRequest(MAX_SURVIVAL_TIME, NULL, &id, TIMER_TYPE_AC_ATTCH, j);
				CWThreadMutexUnlock(&AttachMutex);
				
				return CW_TRUE;
			}
			j++;
		}
		CWThreadMutexUnlock(&AttachMutex);
		return CW_FALSE;
	}
	
	return CW_FALSE;
}


//added by weiay 20080617
CWBool CWWTPMatchBindingInterface(int wtpid,int bindingSystemIndex)
{
	wid_syslog_debug_debug(WID_WTPINFO,"*** BindingSystemIndex is %d input index is %d\n", AC_WTP[wtpid]->BindingSystemIndex,bindingSystemIndex); 
	
	if(AC_WTP[wtpid]->BindingSystemIndex == -1)
	{
		wid_syslog_debug_debug(WID_WTPINFO,"***admin doesn't binding any interface allow all interface access**\n");
		return CW_TRUE;
	}
	else
	{
		if(AC_WTP[wtpid]->BindingSystemIndex == bindingSystemIndex)
		{
			wid_syslog_debug_debug(WID_WTPINFO,"*** this sock has banding access success\n");
			return CW_TRUE;
		}					

		else
		{
			wid_syslog_debug_debug(WID_WTPINFO,"*** this BindingSystemIndex hasn't banding access fail\n");
			return CW_FALSE;
		}
	}
}

CWBool CWParseWTPDescriptor(CWProtocolMessage *msgPtr, int len, CWWTPDescriptor *valPtr)
{
	int theOffset = 0, i = 0;
	char *strversion = NULL;
	CWParseMessageElementStart();

	valPtr->maxRadios = CWProtocolRetrieve8(msgPtr);
	//wid_syslog_debug_debug("WTP Descriptor Max Radios: %d", valPtr->maxRadios);
	
	valPtr->radiosInUse = CWProtocolRetrieve8(msgPtr);
	//wid_syslog_debug_debug("WTP Descriptor Active Radios: %d",	valPtr->radiosInUse);
		
	valPtr->encCapabilities	= CWProtocolRetrieve16(msgPtr);					
	//wid_syslog_debug_debug("WTP Descriptor Encryption Capabilities: %d", valPtr->encCapabilities);
		
	valPtr->vendorInfos.vendorInfosCount = 0;
	
	theOffset = msgPtr->offset;
	
	// see how many vendor ID we have in the message
	while ((msgPtr->offset-oldOffset) < len) 	// oldOffset stores msgPtr->offset's value at the beginning of this function.
	{											// See the definition of the CWParseMessageElementStart() macro.
		int tmp = 0;
		CWProtocolRetrieve32(msgPtr); // ID
		CWProtocolRetrieve16(msgPtr); // type
		tmp = CWProtocolRetrieve16(msgPtr); // len
		msgPtr->offset += tmp;
		valPtr->vendorInfos.vendorInfosCount++;
	}
	
	msgPtr->offset = theOffset;
	
	// actually read each vendor ID
	CW_CREATE_ARRAY_ERR(valPtr->vendorInfos.vendorInfos, valPtr->vendorInfos.vendorInfosCount, CWWTPVendorInfoValues,
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"has %u descriptors\n", \
						WAI_FUNC_LINE_LITERAL, 0, valPtr->vendorInfos.vendorInfosCount);
	for (i = 0; i < valPtr->vendorInfos.vendorInfosCount; i++)
	{
		(valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier = CWProtocolRetrieve32(msgPtr);
		(valPtr->vendorInfos.vendorInfos)[i].type = CWProtocolRetrieve16(msgPtr);																
		(valPtr->vendorInfos.vendorInfos)[i].length = CWProtocolRetrieve16(msgPtr);
						
		if ((valPtr->vendorInfos.vendorInfos)[i].type == CW_WTP_HARDWARE_VERSION)//CW_WTP_SOFTWARE_VERSION = 1
		{
			//(valPtr->vendorInfos.vendorInfos)[i].ver = CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length);
			strversion = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length));	

			(valPtr->vendorInfos.vendorInfos)[i].sysver = (unsigned char *)malloc(((valPtr->vendorInfos.vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos.vendorInfos)[i].sysver, 0, (((valPtr->vendorInfos.vendorInfos)[i].length)+1));
			memcpy((valPtr->vendorInfos.vendorInfos)[i].sysver, strversion, (((valPtr->vendorInfos.vendorInfos)[i].length)));
			CW_FREE_OBJECT(strversion);
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"desc %u vendor %u hw version %s\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier, \
							(valPtr->vendorInfos.vendorInfos)[i].sysver);
		}
		else if ((valPtr->vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)//CW_WTP_SOFTWARE_VERSION = 1
		{
			//(valPtr->vendorInfos.vendorInfos)[i].ver = CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length);
			strversion = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length));	

			(valPtr->vendorInfos.vendorInfos)[i].ver = (unsigned char *)malloc(((valPtr->vendorInfos.vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos.vendorInfos)[i].ver, 0, (((valPtr->vendorInfos.vendorInfos)[i].length)+1));
			memcpy((valPtr->vendorInfos.vendorInfos)[i].ver, strversion, (((valPtr->vendorInfos.vendorInfos)[i].length)));
			CW_FREE_OBJECT(strversion);

			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"desc %u vendor %u sw version %s\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier, \
							(valPtr->vendorInfos.vendorInfos)[i].ver);
		}
		else if ((valPtr->vendorInfos.vendorInfos)[i].type == CW_BOOT_VERSION) 
		{
			strversion = (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length));	
			
			(valPtr->vendorInfos.vendorInfos)[i].bootver = (unsigned char *)malloc(((valPtr->vendorInfos.vendorInfos)[i].length)+1);
			memset((valPtr->vendorInfos.vendorInfos)[i].bootver, 0, (((valPtr->vendorInfos.vendorInfos)[i].length)+1));
			memcpy((valPtr->vendorInfos.vendorInfos)[i].bootver, strversion, (((valPtr->vendorInfos.vendorInfos)[i].length)));
			CW_FREE_OBJECT(strversion);

			#define C4FMT	"%u %u %u %u "
			#define C4STR(a)	a[0],a[1],a[2],a[3]
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"desc %u vendor %u boot version len %u %u "C4FMT"\n", \
							WAI_FUNC_LINE_LITERAL, 0, i, (valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier, \
							(valPtr->vendorInfos.vendorInfos)[i].length, ntohl(*((int *)(valPtr->vendorInfos.vendorInfos)[i].bootver)), \
							C4STR((valPtr->vendorInfos.vendorInfos)[i].bootver));
		}
		else
		{
			(valPtr->vendorInfos.vendorInfos)[i].valuePtr = (int*) (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length));
			
			if ((valPtr->vendorInfos.vendorInfos)[i].valuePtr == NULL)
			{
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
			if ((valPtr->vendorInfos.vendorInfos)[i].length == 4)
			{
				*((valPtr->vendorInfos.vendorInfos)[i].valuePtr) = ntohl(*((valPtr->vendorInfos.vendorInfos)[i].valuePtr));
			}
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"desc %u vendor %u:ignore unknown type %u %u bytes\n", \
								WAI_FUNC_LINE_LITERAL,  0, i, (valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier, \
								(valPtr->vendorInfos.vendorInfos)[i].type, (valPtr->vendorInfos.vendorInfos)[i].length);
		}
	}
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPFrameTunnelMode(CWProtocolMessage *msgPtr, int len, CWframeTunnelMode *valPtr)
{	
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieve8(msgPtr);					
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPIPv4Address(CWProtocolMessage *msgPtr, int len, CWProtocolJoinRequestValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->addr.sin_addr.s_addr = htonl(CWProtocolRetrieve32(msgPtr));
	valPtr->addr.sin_family = AF_INET;
	valPtr->addr.sin_port = htons(CW_CONTROL_PORT);
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPMACType(CWProtocolMessage *msgPtr, int len, CWMACType *valPtr)
{	
	CWParseMessageElementStart();
										
	*valPtr = CWProtocolRetrieve8(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPLOGINType(CWProtocolMessage *msgPtr, int len, WTPLoginState *valPtr)
{
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieve8(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPName(CWProtocolMessage *msgPtr, int len, char **valPtr)
{	
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieveStr(msgPtr, len);
	if(valPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
	}
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPRebootStatistics (CWProtocolMessage *msgPtr, int len, WTPRebootStatisticsInfo *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->rebootCount=CWProtocolRetrieve16(msgPtr);
	valPtr->ACInitiatedCount=CWProtocolRetrieve16(msgPtr);
	valPtr->linkFailurerCount=CWProtocolRetrieve16(msgPtr);
	valPtr->SWFailureCount=CWProtocolRetrieve16(msgPtr);
	valPtr->HWFailuireCount=CWProtocolRetrieve16(msgPtr);
	valPtr->otherFailureCount=CWProtocolRetrieve16(msgPtr);
	valPtr->unknownFailureCount=CWProtocolRetrieve16(msgPtr);
	valPtr->lastFailureType=CWProtocolRetrieve8(msgPtr);
	
//	wid_syslog_debug_debug("WTPRebootStat(1): %d - %d - %d", valPtr->rebootCount, valPtr->ACInitiatedCount, valPtr->linkFailurerCount);
//	wid_syslog_debug_debug("WTPRebootStat(2): %d - %d - %d", valPtr->SWFailureCount, valPtr->HWFailuireCount, valPtr->otherFailureCount);
//	wid_syslog_debug_debug("WTPRebootStat(3): %d - %d", valPtr->unknownFailureCount, valPtr->lastFailureType);
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPRadioStatistics(CWProtocolMessage *msgPtr, int len, WTPRadioStatisticsValues *valPtr, int WTPIndex) 
{
	CWParseMessageElementStart();
	
	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	valPtr->WTPRadioStatistics.lastFailureType = CWProtocolRetrieve8(msgPtr);
	valPtr->WTPRadioStatistics.resetCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.SWFailureCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.HWFailuireCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.otherFailureCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.unknownFailureCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.configUpdateCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.channelChangeCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.bandChangeCount = CWProtocolRetrieve16(msgPtr);
	valPtr->WTPRadioStatistics.currentNoiseFloor = CWProtocolRetrieve16(msgPtr);
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"RadioStat radio %u last fail '%s' reset %u sw fail %u hw fail %u other %u unkown fail %u\n", \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,\
				valPtr->radioID, WAI_RADIO_FAIL_TYPE_STR(valPtr->WTPRadioStatistics.lastFailureType), \
				valPtr->WTPRadioStatistics.resetCount, valPtr->WTPRadioStatistics.SWFailureCount, \
				valPtr->WTPRadioStatistics.HWFailuireCount, \
				valPtr->WTPRadioStatistics.otherFailureCount, valPtr->WTPRadioStatistics.unknownFailureCount);
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"RadioStat radio %u config update %u channal change %u band change %u cur noise floor %u\n", \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
				valPtr->radioID, valPtr->WTPRadioStatistics.configUpdateCount, \
				valPtr->WTPRadioStatistics.channelChangeCount, valPtr->WTPRadioStatistics.bandChangeCount, \
				valPtr->WTPRadioStatistics.currentNoiseFloor);


	CWParseMessageElementEnd();
}

CWBool CWParseWTPOperationalStatistics
(
	CWProtocolMessage *msgPtr,
	int len,
	WTPOperationalStatisticsValues *valPtr,
	int WTPIndex
) 
{
	CWParseMessageElementStart();
	valPtr->eth_count = 1;
	valPtr->eth_rate = 100;
	
	memset(valPtr->cpuType,0,WTP_TYPE_DEFAULT_LEN);
	memcpy(valPtr->cpuType,"soc",3);
	memset(valPtr->memType,0,WTP_TYPE_DEFAULT_LEN);
	memcpy(valPtr->memType,"flash",5);
	memset(valPtr->flashType,0,WTP_TYPE_DEFAULT_LEN);
	memcpy(valPtr->flashType,"flash",5);/*wuwl add.when wtp into run ,but ap(old ap) didn't support report cpu,mem,flash type ,display this*/

	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	valPtr->TxQueueLevel = CWProtocolRetrieve8(msgPtr);
	valPtr->wirelessLinkFramesPerSec = CWProtocolRetrieve16(msgPtr);
	valPtr->ElectrifyRegisterCircle = CWProtocolRetrieve16(msgPtr);
	valPtr->ColdStart = CWProtocolRetrieve8(msgPtr);

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"OpStat radio %u txq %u wireless pps %u ERC %u %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					valPtr->radioID, valPtr->TxQueueLevel, valPtr->wirelessLinkFramesPerSec,
					valPtr->ElectrifyRegisterCircle, valPtr->ColdStart ? "cold reload":"hot reload");
	if(len > 7)
	{
		valPtr->ipmask = CWProtocolRetrieve32(msgPtr);
		valPtr->ipgateway = CWProtocolRetrieve32(msgPtr);
		valPtr->ipdnsfirst = CWProtocolRetrieve32(msgPtr);
		valPtr->ipdnssecend = CWProtocolRetrieve32(msgPtr);

		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"OpStat mask "WAI_IP_FMT"gw "WAI_IP_FMT"dns "WAI_IP_FMT" "WAI_IP_FMT"\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
						WAI_IP_LITERAL(valPtr->ipmask),  WAI_IP_LITERAL(valPtr->ipgateway), \
						WAI_IP_LITERAL(valPtr->ipdnsfirst), WAI_IP_LITERAL(valPtr->ipdnssecend));
	}
	if(len > 23)
	{
	
		unsigned char* cpuType = (unsigned char*)CWProtocolRetrieveStr(msgPtr,32);
		memcpy(valPtr->cpuType, cpuType, 32);
		CW_FREE_OBJECT(cpuType);

		unsigned char* flashType = (unsigned char*)CWProtocolRetrieveStr(msgPtr,32);
		memcpy(valPtr->flashType, flashType, 32);
		CW_FREE_OBJECT(flashType);

		unsigned char* memType = (unsigned char*)CWProtocolRetrieveStr(msgPtr,32);
		memcpy(valPtr->memType, memType, 32);
		CW_FREE_OBJECT(memType);

		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"OpStat cpu '%s' memory '%s' flash '%s'\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,valPtr->cpuType, valPtr->memType, valPtr->flashType);
	}
	if(len > 35)
	{
		valPtr->eth_count = CWProtocolRetrieve8(msgPtr);
		if(valPtr->eth_count > AP_ETH_IF_NUM)
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"OpStat eth port number %u error we force take one\n", \
							WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,valPtr->eth_count);
			
			valPtr->eth_count = 1;
			return CW_FALSE;
		}
		valPtr->eth_rate  = CWProtocolRetrieve32(msgPtr);
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"OpStat eth port number %u rate %u\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,valPtr->eth_count, valPtr->eth_rate);
	}

	CWParseMessageElementEnd();
}


CWBool CWParseMsgElemDecryptErrorReport(CWProtocolMessage *msgPtr, int len, CWDecryptErrorReportValues *valPtr) 
{
	CWParseMessageElementStart();
	valPtr->ID = CWProtocolRetrieve8(msgPtr);
	valPtr->numEntries = CWProtocolRetrieve8(msgPtr);

	valPtr->length = CWProtocolRetrieve8(msgPtr);

	valPtr->decryptErrorMACAddressList = NULL;
	if((valPtr->numEntries) > 0)
	{
		CW_CREATE_ARRAY_ERR(valPtr->decryptErrorMACAddressList, valPtr->numEntries, CWMACAddress,  return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		int size=sizeof(CWMACAddress)*(valPtr->numEntries);
		/* coverity-CID: 10874  Resource leak */
		char * listPtr = CWProtocolRetrieveRawBytes(msgPtr, size);
		if(listPtr)
		{
			CW_COPY_MEMORY(valPtr->decryptErrorMACAddressList, listPtr, size);
			CW_FREE_OBJECT(listPtr);
		}
		else
		{
			wid_syslog_err("%s line %d malloc failed!\n", __func__, __LINE__);
		}
	}
	

	//wid_syslog_debug_debug("Radio Decrypt Error Report of radio \"%d\": %d", valPtr->ID, valPtr->numEntries);
		
	CWParseMessageElementEnd();
}

/*
CWBool CWParseWTPRadioInfo(CWPr<otocolMessage *msgPtr, int len, CWRadiosInformation *valPtr, int radioIndex) {	
	CWParseMessageElementStart();

	(valPtr->radios)[radioIndex].ID = CWProtocolRetrieve8(msgPtr);
	(valPtr->radios)[radioIndex].type = CWProtocolRetrieve32(msgPtr);
	wid_syslog_debug_debug("WTP Radio info: %d %d ", (valPtr->radios)[radioIndex].ID, (valPtr->radios)[radioIndex].type);
	
	CWParseMessageElementEnd();
}
*/
/*fengwenchao add for GM-3 ,20111121*/
CWBool CWParselinkquality(CWProtocolMessage *msgPtr, int len, int WTPIndex)
{
	wid_syslog_debug_debug(WID_WTPINFO,"*** CWParseLinkQuality ***\n");
	wid_syslog_debug_debug(WID_WTPINFO,"*** len:%d ***\n",len);

	unsigned int heart_time_delay = 0;
	unsigned int heart_transfer_pkt = 0;
	unsigned int heart_lose_pkt = 0;
	msgPtr->offset +=3;//skip 2 byte : length , 1 byte :resev
	heart_time_delay = CWProtocolRetrieve32(msgPtr);
	heart_transfer_pkt = CWProtocolRetrieve32(msgPtr);
	heart_lose_pkt = CWProtocolRetrieve32(msgPtr);
	AC_WTP[WTPIndex]->heart_time.heart_time_delay = heart_time_delay;
	AC_WTP[WTPIndex]->heart_time.heart_transfer_pkt = heart_transfer_pkt;
	AC_WTP[WTPIndex]->heart_time.heart_lose_pkt = heart_lose_pkt;

	wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo  wtp %d	heart_time_delay = %d \n",WTPIndex,AC_WTP[WTPIndex]->heart_time.heart_time_delay);
	wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo  wtp %d	heart_transfer_pkt = %d \n",WTPIndex,AC_WTP[WTPIndex]->heart_time.heart_transfer_pkt); 
	wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo  wtp %d	heart_lose_pkt = %d \n",WTPIndex,AC_WTP[WTPIndex]->heart_time.heart_lose_pkt); 	

	return CW_TRUE;
}

/*fengwenchao add end*/
CWBool CWParseAPStatisInfo(CWProtocolMessage *msgPtr, int len, int WTPIndex)
{
	wid_syslog_debug_debug(WID_WTPINFO,"*** CWParseAPStatisInfo ***\n");
	wid_syslog_debug_debug(WID_WTPINFO,"*** len:%d ***\n",len);

	int i = 0;
	unsigned char count = 0;
	unsigned long long rx_bytes_ull = 0;
	unsigned long long tx_bytes_ull = 0;
	unsigned int rx_pkt_mgmt = 0;
	unsigned int tx_pkt_mgmt = 0;
	count = CWProtocolRetrieve8(msgPtr);
	wid_syslog_debug_debug(WID_WTPINFO,"*** count:%d ***\n",count);

	if(count > TOTAL_AP_IF_NUM)
	{
		(msgPtr->offset) += (len-1);
		return CW_TRUE;
	}
	wid_apstatsinfo_init(WTPIndex);
	for(i = 0; ((i < count)&&(i < TOTAL_AP_IF_NUM)); i++)
	{
		AC_WTP[WTPIndex]->apstatsinfo[i].type = CWProtocolRetrieve8(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].radioId = CWProtocolRetrieve8(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].wlanId = CWProtocolRetrieve8(msgPtr);
		
		unsigned char* mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,6);
		memcpy(AC_WTP[WTPIndex]->apstatsinfo[i].mac, mac, 6);
		CW_FREE_OBJECT(mac);

		AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data = CWProtocolRetrieve32(msgPtr);	//xiaodawei modify, ap report data pkts not all pkts, 20110225
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data = CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_errors = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_errors = CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes = (unsigned long long)CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes = (unsigned long long)CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes = AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes*1024;
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes = AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes*1024;

		if(AC_WTP[WTPIndex]->apstatsinfo[i].type == 2)
		{
			AC_WTP[WTPIndex]->rx_bytes = AC_WTP[WTPIndex]->rx_bytes_before + (unsigned long long)(AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
			AC_WTP[WTPIndex]->tx_bytes = AC_WTP[WTPIndex]->tx_bytes_before + (unsigned long long)(AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_bytes %llu \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_bytes %llu \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
		}

		AC_WTP[WTPIndex]->apstatsinfo[i].rx_rate = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_rate = CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_crcerr = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badcrypt = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badmic = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_phyerr = CWProtocolRetrieve32(msgPtr);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_pkt_data %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_pkt_data %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_errors %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_errors);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_errors %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_errors);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_bytes %llu \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_bytes %llu \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_rate %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_rate);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_rate %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_rate);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) ast_rx_crcerr %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_crcerr);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) ast_rx_badcrypt %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badcrypt);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) ast_rx_badmic %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badmic);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) ast_rx_phyerr %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_phyerr);
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets = AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data; //xiaodawei add for old ap(e.g. 1.2.22.1), report only data pkts, 20110225
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets = AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data; //xiaodawei add, 20110225
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_packets(data pkts only) %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets);
		wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_packets(data pkts only) %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets);

		if((len-1)/count >= 65)
		{		
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_drop = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_drop = CWProtocolRetrieve32(msgPtr);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_drop %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_drop);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_drop %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_drop);
		}
		
		if((len-1)/count > 65)
		{
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_multicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_multicast = CWProtocolRetrieve32(msgPtr);
	
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_broadcast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_broadcast = CWProtocolRetrieve32(msgPtr);
	
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_unicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_unicast = CWProtocolRetrieve32(msgPtr);
	
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_multicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_multicast = CWProtocolRetrieve32(msgPtr);
	
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_broadcast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_broadcast = CWProtocolRetrieve32(msgPtr);
	
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_unicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_unicast = CWProtocolRetrieve32(msgPtr);
	
	
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_retry = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_retry = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_retry = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_retry = CWProtocolRetrieve32(msgPtr);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_pkt_multicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_multicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_pkt_multicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_multicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_pkt_broadcast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_broadcast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_pkt_broadcast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_broadcast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_pkt_unicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_unicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_pkt_unicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_unicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_multicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_multicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_multicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_multicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_broadcast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_broadcast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_broadcast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_broadcast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_unicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_unicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_unicast %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_unicast);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_retry %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_retry);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_retry %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_retry);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_pkt_retry %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_retry);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_pkt_retry %u\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_retry);
		}
		if((len-1)/count > 129)
		{
			rx_pkt_mgmt = CWProtocolRetrieve32(msgPtr);
			tx_pkt_mgmt = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_mgmt = rx_pkt_mgmt;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_mgmt = tx_pkt_mgmt;
			
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi)	rx_pkt_mgmt %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,rx_pkt_mgmt);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi)	tx_pkt_mgmt %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,tx_pkt_mgmt);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets = rx_pkt_mgmt + AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data;	//xiaodawei add, 20110225
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets = rx_pkt_mgmt + AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data;	//xiaodawei add, 20110225
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi)	after##########rx_packets %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi)	after##########tx_packets %u \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets);
		}
		if((len-1)/count > 137)
		{
			CWProtocolRetrieve64(msgPtr, &rx_bytes_ull);
			CWProtocolRetrieve64(msgPtr, &tx_bytes_ull);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes = rx_bytes_ull;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes = tx_bytes_ull;

			if(AC_WTP[WTPIndex]->apstatsinfo[i].type == 2)
			{
				AC_WTP[WTPIndex]->rx_bytes = AC_WTP[WTPIndex]->rx_bytes_before + (AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
				AC_WTP[WTPIndex]->tx_bytes = AC_WTP[WTPIndex]->tx_bytes_before + (AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
				wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) (len-1)/count > 137 \n");			
				wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) rx_bytes %llu \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
				wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) tx_bytes %llu \n",AC_WTP[WTPIndex]->apstatsinfo[i].type,AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);

			}
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi)	rx_bytes_ull :%llu\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,rx_bytes_ull);
			wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi)	tx_bytes_ull :%llu\n",AC_WTP[WTPIndex]->apstatsinfo[i].type,tx_bytes_ull);
		}
	}

	return CW_TRUE;
}
CWBool CWParseAPStatisInfo_v2(CWProtocolMessage *msgPtr, int len,char *valPtr, int WTPIndex)
{
	int i = 0;
	unsigned char count = 0;
	unsigned int rx_pkt_mgmt = 0;
	unsigned int tx_pkt_mgmt = 0;
	unsigned int rx_pkt_control = 0;
	unsigned int tx_pkt_control = 0;
	unsigned int rx_errors_frames = 0;
	unsigned int is_refuse_lowrssi = 0;  //fengwenchao add for chinamobile-177,20111122
	unsigned long long rx_mgmt = 0;
	unsigned long long tx_mgmt = 0;
	unsigned long long rx_bytes_ull = 0;
	unsigned long long tx_bytes_ull = 0;
	unsigned long long rx_sum_bytes = 0;
	unsigned long long tx_sum_bytes = 0;
	unsigned long long rx_unicast = 0;
	unsigned long long tx_unicast = 0;
	//CWParseMessageElementStart();
	count = CWProtocolRetrieve8(msgPtr);

	wid_syslog_debug_debug(WID_DEFAULT,"***%s len:%d count:%d***\n", __func__, len, count);

	if (count > TOTAL_AP_IF_NUM)
	{
		(msgPtr->offset) += (len-1);
		return CW_TRUE;
	}
	
	wid_apstatsinfo_init(WTPIndex);
	
	for (i = 0; ((i < count) && (i < TOTAL_AP_IF_NUM)); i++)
	{
		AC_WTP[WTPIndex]->apstatsinfo[i].type = CWProtocolRetrieve8(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].radioId = CWProtocolRetrieve8(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].wlanId = CWProtocolRetrieve8(msgPtr);
		
		unsigned char *mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
		memcpy(AC_WTP[WTPIndex]->apstatsinfo[i].mac, mac, MAC_LEN);
		CW_FREE_OBJECT(mac);

		AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data = CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_errors = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_errors = CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes = (unsigned long long)CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes = (unsigned long long)CWProtocolRetrieve32(msgPtr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes = AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes*1024;
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes = AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes*1024;

		if (AC_WTP[WTPIndex]->apstatsinfo[i].type == 2)
		{
			AC_WTP[WTPIndex]->rx_bytes = AC_WTP[WTPIndex]->rx_bytes_before + (unsigned long long)(AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
			AC_WTP[WTPIndex]->tx_bytes = AC_WTP[WTPIndex]->tx_bytes_before + (unsigned long long)(AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo_type:%d(0-ath,1-eth,2-wifi) rx_bytes %llu tx_bytes %llu\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes, AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
		}
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_rate = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_rate = CWProtocolRetrieve32(msgPtr);		
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_crcerr = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badcrypt = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badmic = CWProtocolRetrieve32(msgPtr);
		AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_phyerr = CWProtocolRetrieve32(msgPtr);
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_data %u tx_pkt_data %u\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data, AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data);
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_errors %u tx_errors %u\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].rx_errors, AC_WTP[WTPIndex]->apstatsinfo[i].tx_errors);
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_bytes %llu tx_bytes %llu\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes, AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_rate %u tx_rate %u\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].rx_rate, AC_WTP[WTPIndex]->apstatsinfo[i].tx_rate);
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) ast_rx_crcerr %u ast_rx_badcrypt %u\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_crcerr, AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badcrypt);
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) ast_rx_badmic %u ast_rx_phyerr %u\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_badmic, AC_WTP[WTPIndex]->apstatsinfo[i].ast_rx_phyerr);
		
		AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets = AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data;	//xiaodawei add for old ap(e.g. 1.2.22.1), report only data pkts, 20110225
		AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets = AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data;
		wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_packets(data pkts only) %u tx_packets(data pkts only) %u\n",
								__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
								AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets, AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets);

		if ((len-1)/count >= 65)
		{
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_drop = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_drop = CWProtocolRetrieve32(msgPtr);
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_drop %u tx_drop %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_drop, AC_WTP[WTPIndex]->apstatsinfo[i].tx_drop);
		}

		if ((len-1)/count > 65)
		{
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_multicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_multicast = CWProtocolRetrieve32(msgPtr);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_broadcast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_broadcast = CWProtocolRetrieve32(msgPtr);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_unicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_unicast = CWProtocolRetrieve32(msgPtr);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_multicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_multicast = CWProtocolRetrieve32(msgPtr);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_broadcast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_broadcast = CWProtocolRetrieve32(msgPtr);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_unicast = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_unicast = CWProtocolRetrieve32(msgPtr);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_retry = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_retry = CWProtocolRetrieve32(msgPtr);
			
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_retry = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_retry = CWProtocolRetrieve32(msgPtr);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_multicast %u tx_pkt_multicast %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_multicast, AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_multicast);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_broadcast %u tx_pkt_broadcast %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_broadcast,AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_broadcast);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_unicast %u tx_pkt_unicast %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_unicast, AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_unicast);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_multicast %u tx_multicast %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_multicast, AC_WTP[WTPIndex]->apstatsinfo[i].tx_multicast);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_broadcast %u tx_broadcast %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_broadcast, AC_WTP[WTPIndex]->apstatsinfo[i].tx_broadcast);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_unicast %u tx_unicast %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_unicast, AC_WTP[WTPIndex]->apstatsinfo[i].tx_unicast);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_retry %u tx_retry %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_retry, AC_WTP[WTPIndex]->apstatsinfo[i].tx_retry);
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_retry %u tx_pkt_retry %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_retry, AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_retry);
		}
		
		if ((len-1)/count > 129)
		{
			rx_pkt_mgmt = CWProtocolRetrieve32(msgPtr);
			tx_pkt_mgmt = CWProtocolRetrieve32(msgPtr);
			
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_mgmt = rx_pkt_mgmt;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_mgmt = tx_pkt_mgmt;
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_mgmt %u tx_pkt_mgmt %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type, rx_pkt_mgmt, tx_pkt_mgmt);

			AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets = rx_pkt_mgmt + AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_data;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets = tx_pkt_mgmt + AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_data;
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_packets %u tx_packets %u\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_packets, AC_WTP[WTPIndex]->apstatsinfo[i].tx_packets);
		}
		
		if ((len-1)/count > 137)
		{
			CWProtocolRetrieve64(msgPtr, &rx_bytes_ull);
			CWProtocolRetrieve64(msgPtr, &tx_bytes_ull);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes = rx_bytes_ull;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes = tx_bytes_ull;

			if (AC_WTP[WTPIndex]->apstatsinfo[i].type == 2)
			{	
				AC_WTP[WTPIndex]->rx_bytes = AC_WTP[WTPIndex]->rx_bytes_before + (AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes);
				AC_WTP[WTPIndex]->tx_bytes = AC_WTP[WTPIndex]->tx_bytes_before + (AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
				wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) (len-1)/count > 137 \n");			
				wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_bytes %llu  tx_bytes %llu\n",
										__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
										AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes, AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);
			}
			
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_bytes_ull:%llu tx_bytes_ull :%llu\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type, rx_bytes_ull, tx_bytes_ull);
		}
		if((len-1)/count > 153)
		{
			CWProtocolRetrieve64(msgPtr, &rx_mgmt);
			CWProtocolRetrieve64(msgPtr, &tx_mgmt);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_mgmt = rx_mgmt;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_mgmt = tx_mgmt;
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_mgmt :%llu tx_mgmt :%llu\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type, rx_mgmt, tx_mgmt);
		}
		
		if ((len-1)/count > 169)
		{
			CWProtocolRetrieve64(msgPtr, &rx_sum_bytes);
			CWProtocolRetrieve64(msgPtr, &tx_sum_bytes);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_sum_bytes = rx_sum_bytes;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_sum_bytes = tx_sum_bytes;
			if (AC_WTP[WTPIndex]->apstatsinfo[i].type == 2)
			{
				AC_WTP[WTPIndex]->rx_bytes = AC_WTP[WTPIndex]->rx_bytes_before + (AC_WTP[WTPIndex]->apstatsinfo[i].rx_sum_bytes);
				AC_WTP[WTPIndex]->tx_bytes = AC_WTP[WTPIndex]->tx_bytes_before + (AC_WTP[WTPIndex]->apstatsinfo[i].tx_sum_bytes);
				wid_syslog_debug_debug(WID_WTPINFO,"CWParseAPStatisInfo type:%d(0-ath,1-eth,2-wifi) (len-1)/count > 169 \n");			
				wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_bytes %llu tx_bytes %llu\n",
										__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
										AC_WTP[WTPIndex]->apstatsinfo[i].rx_bytes, AC_WTP[WTPIndex]->apstatsinfo[i].tx_bytes);

			}
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_sum_bytes :%llu rx_sum_bytes :%llu\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type, rx_sum_bytes, tx_sum_bytes);
		}
		
		if ((len-1)/count > 185)
		{
			CWProtocolRetrieve64(msgPtr, &rx_unicast);
			CWProtocolRetrieve64(msgPtr, &tx_unicast);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_unicast = rx_unicast;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_unicast = tx_unicast;
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_unicast :%llu tx_unicast :%llu\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type,
									AC_WTP[WTPIndex]->apstatsinfo[i].rx_unicast, AC_WTP[WTPIndex]->apstatsinfo[i].tx_unicast);
		}
		/*zhaoruijia,20100913,增加接收，发送控制帧,start*/
		if ((len-1)/count > 201)
		{
			rx_pkt_control = CWProtocolRetrieve32(msgPtr);
			tx_pkt_control = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_pkt_control = rx_pkt_control;
			AC_WTP[WTPIndex]->apstatsinfo[i].tx_pkt_control = tx_pkt_control;
			wid_syslog_debug_debug(WID_WTPINFO,"type:%d(0-ath,1-eth,2-wifi)  control_len = %d",len);
			wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d statsinfo:%d(0-ath,1-eth,2-wifi) rx_pkt_control :%d tx_pkt_control :%d\n",
									__func__, __LINE__, WTPIndex, AC_WTP[WTPIndex]->apstatsinfo[i].type, rx_pkt_control, tx_pkt_control);
		}
		/* zhangshu add for error packets and error frames, 2010-09-26 */
		if ((len-1)/count > 209)
		{
			rx_errors_frames = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].rx_errors_frames = rx_errors_frames;
			wid_syslog_debug_debug(WID_WTPINFO,"wtp%d type:%d(0-ath,1-eth,2-wifi) rx_errors_frames :%u\n",WTPIndex,AC_WTP[WTPIndex]->apstatsinfo[i].type, AC_WTP[WTPIndex]->apstatsinfo[i].rx_errors_frames);
		}

		/*fengwenchao add for chinamobile-177,20111122*/
		if((len-1)/count > 213)
		{
			is_refuse_lowrssi = CWProtocolRetrieve32(msgPtr);
			AC_WTP[WTPIndex]->apstatsinfo[i].is_refuse_lowrssi = is_refuse_lowrssi;
			wid_syslog_debug_debug(WID_WTPINFO,"wtp%d type:%d(0-ath,1-eth,2-wifi) is_refuse_lowrssi :%u\n",WTPIndex,AC_WTP[WTPIndex]->apstatsinfo[i].type, AC_WTP[WTPIndex]->apstatsinfo[i].is_refuse_lowrssi);
		}
		/*fengwenchao add end*/	
	}

	return CW_TRUE;
}

/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,start*/
CWBool CWParseAP_Ntp_resultcode(CWProtocolMessage *msgPtr, int len,char *valPtr, int WTPIndex)
{
	wid_syslog_debug_debug(WID_DEFAULT,"*** CWParseAP_Ntp_resultcode len:%d***\n",len);
	unsigned char resultcode = 0;
	unsigned char IGNORE_UBSV reserve = 0;
	resultcode = CWProtocolRetrieve8(msgPtr);
	reserve = CWProtocolRetrieve8(msgPtr);
	reserve = CWProtocolRetrieve8(msgPtr);
	if (resultcode != 0)
	{
		if (gtrapflag >= 4)    //trap
		{
		#if NOSOFTAC
			wid_dbus_trap_wtp_ap_ACTimeSynchroFailure(WTPIndex,0);
		#endif
			AC_WTP[WTPIndex]->ntp_trap_flag = 1;
		}
	}
	else
	{
		if (AC_WTP[WTPIndex]->ntp_trap_flag == 1)   //trap clear
		{
		#if NOSOFTAC
			wid_dbus_trap_wtp_ap_ACTimeSynchroFailure(WTPIndex,1);
		#endif
			AC_WTP[WTPIndex]->ntp_trap_flag = 0;
		}
	}
	return CW_TRUE;
}

/*zhaoruijia,20100904,transplant ACTIMESYNCHROFAILURE from 1.2omc to 1.3,end*/

/*wuwl add*/
CWBool CWParseAttack_addr_Redirect(CWProtocolMessage *msgPtr, int len, int WTPIndex)
{
	wid_syslog_debug_debug(WID_DEFAULT,"***wtp%d CWParseAttack_addr_RD len:%d ***\n", WTPIndex, len);
	WIDStaWapiInfoList *valPtr = NULL;
	unsigned short rd_sta_count = 0;
	unsigned char radio_id = 0;
	unsigned char wlan_id = 0;
	unsigned char flag = 0;/*1--trap;0--trap clear*/
//	unsigned char sta_mac[6];
	unsigned int IGNORE_UBSV reserve = 0;
	unsigned int i = 0;
	unsigned char *stamac = NULL;
	reserve = CWProtocolRetrieve32(msgPtr);
	rd_sta_count = CWProtocolRetrieve16(msgPtr);

	CW_CREATE_OBJECT_ERR(valPtr, WIDStaWapiInfoList, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	memset(valPtr, 0, sizeof(WIDStaWapiInfoList));
	valPtr->WTPID = WTPIndex;
	valPtr->sta_num = rd_sta_count;

	for (i = 0; ((i < rd_sta_count) && (i < 64)); i++)
	{
		radio_id = CWProtocolRetrieve8(msgPtr);
		wlan_id = CWProtocolRetrieve8(msgPtr);
		flag  = CWProtocolRetrieve8(msgPtr);
		stamac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
		/* By the way modify for coverity-CID: 10872  Resource leak */
		if (!stamac)
		{
			wid_syslog_err("%s line %d malloc failed!\n", __func__, __LINE__);
			continue;
		}
	
		wid_syslog_debug_debug(WID_DEFAULT,"i = %d radio%d wlan%d flag= %d sta["MACSTR"] rd_sta_count= %d gtrapflag= %d\n",
								i, radio_id, wlan_id, flag, MAC2STR(stamac), rd_sta_count, gtrapflag);
		
		if (flag == 1)
		{
			valPtr->StaWapiInfo[i].wapi_trap_flag = flag;
		}
		else
		{
			valPtr->StaWapiInfo[i].wapi_trap_flag = 2;
		}
		valPtr->StaWapiInfo[i].RadioId = radio_id;
		valPtr->StaWapiInfo[i].WlanId = wlan_id;
		memset(valPtr->StaWapiInfo[i].mac, 0, MAC_LEN);
		memcpy(valPtr->StaWapiInfo[i].mac, stamac, MAC_LEN);
		CW_FREE_OBJECT(stamac);
	}
	WidAsdStaWapiInfoUpdate(WTPIndex, valPtr);

	CW_FREE_OBJECT(valPtr);

	return CW_TRUE;
}

/* zhangshu add for challenge replay, 2010-09-26 */
CWBool CWParseAP_challenge_replay(CWProtocolMessage *msgPtr, int len, int WTPIndex)
{
	wid_syslog_debug_debug(WID_DEFAULT,"***wtp%d CWParseAP_challenge_replay  len:%d ***\n", WTPIndex, len);
	WIDStaWapiInfoList *valPtr = NULL;
	unsigned short rd_sta_count = 0;
	unsigned char radio_id = 0;
	unsigned char wlan_id = 0;
	unsigned char flag = 0; /*1--trap;0--trap clear*/
	unsigned int IGNORE_UBSV reserve = 0;
	unsigned int i = 0;
	unsigned char *stamac = NULL;
	reserve = CWProtocolRetrieve32(msgPtr);
	rd_sta_count = CWProtocolRetrieve16(msgPtr);

	CW_CREATE_OBJECT_ERR(valPtr, WIDStaWapiInfoList, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	memset(valPtr, 0, sizeof(WIDStaWapiInfoList));
	valPtr->WTPID = WTPIndex;
	valPtr->sta_num = rd_sta_count;

	for (i = 0; ((i < rd_sta_count) && (i < 64)); i++)
	{
		radio_id = CWProtocolRetrieve8(msgPtr);
		wlan_id = CWProtocolRetrieve8(msgPtr);
		flag  = CWProtocolRetrieve8(msgPtr);
		stamac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
		if(!stamac)
		{
			wid_syslog_err("%s line %d malloc failed!\n", __func__, __LINE__);
			continue;
		}
		
		wid_syslog_debug_debug(WID_DEFAULT,"i = %d radio%d wlan%d flag= %d sta["MACSTR"] rd_sta_count= %d gtrapflag= %d\n",
											i, radio_id, wlan_id, flag, MAC2STR(stamac), rd_sta_count, gtrapflag);

		if (flag == 1)
		{
			valPtr->StaWapiInfo[i].wapi_trap_flag = 3;
		}
		else
		{
			valPtr->StaWapiInfo[i].wapi_trap_flag = 4;
		}
		valPtr->StaWapiInfo[i].RadioId = radio_id;
		valPtr->StaWapiInfo[i].WlanId = wlan_id;
		memset(valPtr->StaWapiInfo[i].mac, 0, MAC_LEN);
		memcpy(valPtr->StaWapiInfo[i].mac, stamac, MAC_LEN);
		CW_FREE_OBJECT(stamac);
	}
	
	WidAsdStaWapiInfoUpdate(WTPIndex, valPtr);
	
	CW_FREE_OBJECT(valPtr);
	
	return CW_TRUE;
}

//weichao add for flow check report 2011.11.03
CWBool CWPareseWtp_Sta_Flow_Check_Report(CWProtocolMessage *msgPtr, int len, WIDStationInfo *valPtr,int wtpindex)
{
	CWParseMessageElementStart();
	unsigned short length = 0;
	char IGNORE_UBSV reserved = 0;
    unsigned char *mac = NULL;

	length = CWProtocolRetrieve16(msgPtr);
	wid_syslog_debug_debug(WID_DEFAULT,"%s: wtp%d length = %d\n",__func__,wtpindex,length);
	reserved = CWProtocolRetrieve8(msgPtr);
	
	mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
	memcpy(valPtr->mac, mac, MAC_LEN);
	CW_FREE_OBJECT(mac);
	
	valPtr->rx_frames = CWProtocolRetrieve32(msgPtr);
	valPtr->tx_frames = CWProtocolRetrieve32(msgPtr);
	CWProtocolRetrieve64(msgPtr, &valPtr->rx_data_bytes);
	CWProtocolRetrieve64(msgPtr, &valPtr->tx_data_bytes);
	
	wid_syslog_debug_debug(WID_DEFAULT,MACSTR " rx_data_bytes:%llu tx_data_bytes:%llu rx_frames:%u tx_frames:%u\n",
			MAC2STR(valPtr->mac),valPtr->rx_data_bytes,valPtr->tx_data_bytes,valPtr->rx_frames,valPtr->tx_frames);
		
	WidAsd_StationInfoUpdate1(wtpindex,valPtr);
	CWParseMessageElementEnd(); 
}

CWBool CWPareseWtp_Sta_leave_Report(CWProtocolMessage *msgPtr, int len, WIDStationInfo *valPtr,int wtpindex)
{
	wid_syslog_debug_debug(WID_WTPINFO,"***%s****\n",__func__);
	
	WIDStationInfo valPtr1[20];
	memset(valPtr1, 0, sizeof(WIDStationInfo));
	CWParseMessageElementStart();
	unsigned short length = 0;
	unsigned char radioid = 0;
	unsigned char wlanid = 0;
	unsigned char sta_count = 0;
	unsigned char *mac = NULL;
	unsigned char sta_reason;
	unsigned short sub_reason;
	int i = 0;
	length = CWProtocolRetrieve16(msgPtr);
	wid_syslog_debug_debug(WID_DEFAULT,"in case CWPareseWtp_Sta_leave_Report length = %d\n",length);

	radioid = CWProtocolRetrieve8(msgPtr);
	wlanid = CWProtocolRetrieve8(msgPtr);
	sta_count = CWProtocolRetrieve8(msgPtr);
	sta_reason = CWProtocolRetrieve8(msgPtr);
	sub_reason = CWProtocolRetrieve16(msgPtr);
	wid_syslog_debug_debug(WID_DEFAULT,"radioid = %d wlanid = %d count = %d sta_reason = %d sub_reason = %d\n",
										radioid,wlanid,sta_count,sta_reason,sub_reason);
	if(sta_count == 255)
	{
		valPtr1[0].wlanId = wlanid ;
		valPtr1[0].radioId = radioid ;
		valPtr1[0].sta_reason = sta_reason;
		valPtr1[0].sub_reason = sub_reason;
	}
	else
	{
		if (sta_count > 20)
		{
			wid_syslog_debug_debug(WID_WTPINFO,"count = %d, change to 20!\n",sta_count);
			sta_count = 20;
		}
		for (i  = 0 ; i < sta_count ; i++)
		{
			valPtr1[i].wlanId = wlanid;
			valPtr1[i].radioId = radioid;
			valPtr1[i].sta_reason = sta_reason;
			valPtr1[i].sub_reason = sub_reason;
			mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr,MAC_LEN);
			if (NULL != mac)
			{
				memcpy(valPtr1[i].mac, mac, MAC_LEN);
				CW_FREE_OBJECT(mac);
			}
			
			CWProtocolRetrieve64(msgPtr, &valPtr1[i].rx_data_bytes);
			CWProtocolRetrieve64(msgPtr, &valPtr1[i].tx_data_bytes);
			valPtr1[i].rx_data_frames = CWProtocolRetrieve32(msgPtr);
			valPtr1[i].tx_data_frames = CWProtocolRetrieve32(msgPtr);
			valPtr1[i].rx_frames = CWProtocolRetrieve32(msgPtr);
			valPtr1[i].tx_frames = CWProtocolRetrieve32(msgPtr);
			valPtr1[i].rx_frag_packets = CWProtocolRetrieve32(msgPtr);
			valPtr1[i].tx_frag_packets = CWProtocolRetrieve32(msgPtr);	
			wid_syslog_debug("valPtr mac:"MACSTR" rx_data_bytes:%llu tx_data_bytes:%llu "
							"rx_frames:%u tx_frames:%u sta_reason+ %d sub_reason= %d\n",
							MAC2STR(valPtr1[i].mac),valPtr1[i].rx_data_bytes,valPtr1[i].tx_data_bytes,
							valPtr1[i].rx_frames,valPtr1[i].tx_frames,valPtr1[i].sta_reason,valPtr1[i].sub_reason);	
		}
	}	
	WidAsd_StationLeaveReport(wtpindex,sta_count,valPtr1);
	CWParseMessageElementEnd(); 
}

/* zhangshu add for Terminal Disturb Info Report, 2010-10-08 */
CWBool CWParseWtp_Sta_Terminal_Disturb_Report(CWProtocolMessage *msgPtr, int len,char *valPtr, int WTPIndex)
{
	unsigned short sta_trap_count = 0;
	unsigned char radio_id = 0;
	unsigned char wlan_id = 0;
	unsigned char state = 0;/*1--trap;0--trap clear*/
	unsigned char *sta_mac = NULL;
	unsigned int i = 0;
	unsigned char IGNORE_UBSV currentchannel = 0;
	int IGNORE_UBSV temp = 0;
	
	wid_syslog_debug_debug(WID_DEFAULT,"*** CWParseWtp_Sta_Terminal_Disturb_Report len:%d ***\n",len);
	
	radio_id = CWProtocolRetrieve8(msgPtr);
	wlan_id = CWProtocolRetrieve8(msgPtr);
	state = CWProtocolRetrieve8(msgPtr);
	temp = CWProtocolRetrieve32(msgPtr);
	sta_trap_count = CWProtocolRetrieve16(msgPtr);
	
	wid_syslog_debug_debug(WID_DEFAULT,"radio%d-%d wlan_id = %d state = %d sta_trap_count = %d\n",
							WTPIndex, radio_id, wlan_id, state, sta_trap_count);
	
	currentchannel = AC_WTP[WTPIndex]->WTP_Radio[radio_id]->Radio_Chan;
	
	for (i = 0; i < sta_trap_count; i++)
	{
		sta_mac = (unsigned char*)CWProtocolRetrieveStr(msgPtr, MAC_LEN);
		if (!sta_mac)
		{
			wid_syslog_err("%s line %d malloc failed!\n", __func__, __LINE__);
			continue;
		}
		
		wid_syslog_debug_debug(WID_DEFAULT,"i = %d sta_mac["MACSTR"] gtrapflag = %d\n", i, MAC2STR(sta_mac), gtrapflag);
		
		if (gtrapflag >= 24)
		{
			if (state == 1)
			{
				if (AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag == 0)
				{
				#if NOSOFTAC
					wid_dbus_trap_wtp_channel_terminal_interference(WTPIndex, radio_id, currentchannel, sta_mac);
				#endif
					AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag = 1;
				}
			}
			else
			{
				if (AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag == 1)
				{
	    		#if NOSOFTAC
		    		wid_dbus_trap_wtp_channel_terminal_interference_clear(WTPIndex, radio_id, currentchannel, sta_mac);
				#endif
					AC_WTP[WTPIndex]->wid_trap.rogue_terminal_trap_flag = 0;
				}
			}
		}
		CW_FREE_OBJECT(sta_mac);
    }
    
	return CW_TRUE;
}

CWBool  CWAssembleStaticAPIPDNS(CWProtocolMessage *msgPtr,int wtpid)
{
	char addr_ip[64] = {0};
	char addr_mask[64] = {0};    
	char addr_gateway[64] = {0};
	char addr_fstdns[64] = {0};
	char addr_snddns[64] = {0};
	
	unsigned char valuelen = 24;
	unsigned char reserved = 1;				//xiaodawei modify, change 0 to 1, static, 20101230
	
	if(msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_ipadd);
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_mask_new);
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_gateway); 
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_dnsfirst); 
	CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_dnssecend); 
	CWProtocolStore8(msgPtr, reserved);			//xiaodawei modify, 20101231
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore16(msgPtr, 0); 
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s:%d set wtp%u ip: %s(%u) mask: %s(%u) gateway: %s(%u) fristdns: %s(%u) fristdns: %s(%u)\n",
							__func__, __LINE__, wtpid,
							CWParse_inet_ntoa(AC_WTP[wtpid]->ap_ipadd, addr_ip), AC_WTP[wtpid]->ap_ipadd,
							CWParse_inet_ntoa(AC_WTP[wtpid]->ap_mask_new, addr_mask), AC_WTP[wtpid]->ap_mask_new,
							CWParse_inet_ntoa(AC_WTP[wtpid]->ap_gateway, addr_gateway), AC_WTP[wtpid]->ap_gateway,
							CWParse_inet_ntoa(AC_WTP[wtpid]->ap_dnsfirst, addr_fstdns), AC_WTP[wtpid]->ap_dnsfirst,
							CWParse_inet_ntoa(AC_WTP[wtpid]->ap_dnssecend, addr_snddns), AC_WTP[wtpid]->ap_dnssecend);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2);

}
CWBool  CWAssembleSnoopingAble(CWProtocolMessage *msgPtr,int wtpid)
{
	wid_syslog_debug_debug(WID_WTPINFO,"#### CWAssembleSnoopingAble ####\n");
	
	unsigned char valuelen = 8;
	//unsigned int reserved = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, WTP);// type
	CWProtocolStore8(msgPtr, DHCP_SNOOPING);// op
	CWProtocolStore8(msgPtr, 0);//Radio id
	CWProtocolStore8(msgPtr, 0); //wlan id
	CWProtocolStore8(msgPtr, AC_WTP[wtpid]->dhcp_snooping); 
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore16(msgPtr, 0); 
	//CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_dnssecend); 
	//CWProtocolStore32(msgPtr, reserved); 	
	
	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_SET);

}
CWBool  CWAssembleIpMacReport(CWProtocolMessage *msgPtr,int wtpid)
{
	wid_syslog_debug_debug(WID_WTPINFO,"#### CWAssembleIpMacReport ####\n");
	
	unsigned char valuelen=8; //
	//unsigned int reserved = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, STA);//type
	CWProtocolStore8(msgPtr, STA_IP_MAC_REPORT);// op
	CWProtocolStore8(msgPtr, 0);//Radio id
	CWProtocolStore8(msgPtr, 0); //wlan id
	CWProtocolStore8(msgPtr, AC_WTP[wtpid]->sta_ip_report);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore16(msgPtr, 0); //value
	//CWProtocolStore32(msgPtr, AC_WTP[wtpid]->ap_dnssecend); 
	//CWProtocolStore32(msgPtr, reserved); 	
	
	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_SET);

}

/* zhangshu add for Terminal Disturb Info Report, 2010-10-08 */
CWBool  CWAssembleTerminalDisturbInfoReport(CWProtocolMessage *msgPtr,int wtpid)
{
    printf("1111111  Send Terminal Disturb info to ap  111111111\n");
	unsigned char valuelen = 10; 
	unsigned char value = 11;
	unsigned char flag = AC_WTP[wtpid]->ter_dis_info.reportswitch;
	unsigned short trap_pkt = AC_WTP[wtpid]->ter_dis_info.reportpkt;
	unsigned short sta_count = AC_WTP[wtpid]->ter_dis_info.sta_trap_count;
	int jump = 0;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

    wid_syslog_debug_debug(WID_DEFAULT,"1111111 Send Terminal Disturb info to ap 111111\n");
	wid_syslog_debug_debug(WID_DEFAULT,"value = %d\n",value);
	wid_syslog_debug_debug(WID_DEFAULT,"flag = %d\n",flag);
	wid_syslog_debug_debug(WID_DEFAULT,"sta_count = %d\n",sta_count);
	wid_syslog_debug_debug(WID_DEFAULT,"trap_pkt = %d\n",trap_pkt);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, valuelen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, flag);
	CWProtocolStore16(msgPtr, sta_count);
	CWProtocolStore16(msgPtr, trap_pkt);
	CWProtocolStore32(msgPtr, jump);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleMsgElemAPBlackWhiteList
(
	CWProtocolMessage *msgPtr,
	unsigned char wlanid,
	unsigned char radioid,
	unsigned char list_class,
	unsigned char level,
	unsigned char oprate,
	unsigned char mac_count,
	MACTYPE *macarry
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Element ID         |        Element Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  list class   |   level       |   radio id    |   wlan id     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  operate      |             mac count         |    mac list
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                         mac list ........			|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
	wid_syslog_debug_debug(WID_WTPINFO,"#### CWAssembleMsgElemAPBlackWhiteList ####\n");

	short int elementid = CW_VENDOR_CONFIG_ELEMENT_WTP_BLACK_WHITE_LIST; 
	short int length = 0;
	int i = 0;

	if(msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	length = 11 + mac_count * 6;
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	
	CWProtocolStore8(msgPtr, list_class); 
	CWProtocolStore8(msgPtr, level); 
	CWProtocolStore8(msgPtr, radioid); 
	CWProtocolStore8(msgPtr, wlanid); 
	CWProtocolStore8(msgPtr, oprate); 
	CWProtocolStore16(msgPtr, mac_count);
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s: elementid %d length %d radio %d wlan %d list_class %d level %d oprate %d mac_count %d\n",
							__func__, elementid, length, radioid, wlanid, list_class, level, oprate, mac_count);
	for(i = 0; i < mac_count; i++)
	{
		CWProtocolStore8(msgPtr, macarry[i].mac[0]); 
		CWProtocolStore8(msgPtr, macarry[i].mac[1]); 
		CWProtocolStore8(msgPtr, macarry[i].mac[2]); 
		CWProtocolStore8(msgPtr, macarry[i].mac[3]);
		CWProtocolStore8(msgPtr, macarry[i].mac[4]);
		CWProtocolStore8(msgPtr, macarry[i].mac[5]);
		
        wid_syslog_debug_debug(WID_WTPINFO,"%s: i=%d, "MACSTR"\n", __func__, i, MAC2STR(macarry[i].mac));
	}
	

	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPElectronicMenu
(
	CWProtocolMessage *msgPtr,
	unsigned char wlanid,
	unsigned char radioid,
	unsigned char level,
	unsigned char state
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Element ID         |        Element Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     switch    |   level       |   radio id    |   wlan id     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
	wid_syslog_debug_debug(WID_WTPINFO,"#### CWAssembleMsgElemAPElectronicMenu ####\n");

	short int elementid = CW_VENDOR_CONFIG_ELEMENT_WTP_ELECTRONIC_MENU; 
	short int length = 8;
	
	if(msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	
	CWProtocolStore8(msgPtr, state); 
	CWProtocolStore8(msgPtr, level); 
	CWProtocolStore8(msgPtr, radioid); 
	CWProtocolStore8(msgPtr, wlanid); 

	wid_syslog_debug_debug(WID_WTPINFO,"%s: elementid %d length %d state %d level %d radio %d wlan %d\n",
			__func__, elementid, length, state, level, radioid, wlanid);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleWtpStaDeauthreport(CWProtocolMessage *msgPtr,int wtpid)
{
	wid_syslog_debug_debug(WID_WTPINFO,"#### wtp%d CWAssemblewtpstadeauthreport ####\n", wtpid);
	
	unsigned short elementid = 26;
	unsigned short length = 5;
	unsigned char flag = AC_WTP[wtpid]->sta_deauth_message_reportswitch;
	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid);
	CWProtocolStore16(msgPtr, length-4);
	CWProtocolStore8(msgPtr, flag);
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}
CWBool  CWAssembleWtpStaFlowInformationreport(CWProtocolMessage *msgPtr,int wtpid)
{
	wid_syslog_debug_debug(WID_WTPINFO,"#### CWAssembleWtpStaFlowInformationreport ####\n");
	
	unsigned short elementid = 27;
	unsigned short length = 5; 
	unsigned char flag = AC_WTP[wtpid]->sta_flow_information_reportswitch;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid);
	CWProtocolStore16(msgPtr, length-4);
	CWProtocolStore8(msgPtr, flag);
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPspectrum_analysis
(
	CWProtocolMessage *msgPtr, 
	unsigned char radioid,
	unsigned char enable,	
	unsigned short scan_cycle,
	unsigned short scan_time,
	unsigned short report_interval
)
{
/*
TODO
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|		  element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	  radio 	|  scan time each channel   	|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
    wid_syslog_debug_debug(WID_WTPINFO,"#### CWAssembleMsgElemAPspectrum_analysis ####\n");

	short int elementid = CW_VENDOR_CONFIG_ELEMENT_SPECTRUM_ANALYSIS; 
	short int length = 14;
	
	if(msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	

	CWProtocolStore8(msgPtr, enable);
	CWProtocolStore16(msgPtr, report_interval);
	CWProtocolStore8(msgPtr, 1);	/* radio num */
	CWProtocolStore8(msgPtr, radioid);
	CWProtocolStore16(msgPtr, scan_cycle);
	CWProtocolStore16(msgPtr, scan_time);
	CWProtocolStore8(msgPtr, 0);	/* channel num */

	wid_syslog_debug_debug(WID_DEFAULT,"elemid %d elemlen %d radio %d cycle %d time %d report inerval %d\n", 
										elementid, length, radioid, scan_cycle, scan_time, report_interval);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPlanvlan(CWProtocolMessage *msgPtr, 
	unsigned char state, unsigned short vlanid)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|		  element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	  operate 	|         lan vlan vlanid   	|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

	short int elementid = CW_VENDOR_CONFIG_ELEMENT_LAN_VLAN; 
	short int length = 7;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	

	CWProtocolStore8(msgPtr, state);
	CWProtocolStore16(msgPtr, vlanid);

	wid_syslog_debug_debug(WID_DEFAULT,"elemid %d elemlen %d state %d lan vlan %d\n", 
		elementid,length,state,vlanid);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

/* REQUIREMENTS-536/ENR4-2 for 5G-FIRST */
CWBool CWAssembleMsgElemAP5gJoinFirst
(
	CWProtocolMessage *msgPtr,
	unsigned char join_switch,
	unsigned char discard_count,
	unsigned short scan_time,
	unsigned int aging_time,
	unsigned char sta_count,
	unsigned char signal_strength
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Element ID         |        Element Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  switch       | discard_count |        scantime_thr           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          agingtime_thr	                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| stacount_thr  |signalstrength |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
	short int elementid = 29; 
	short int length = 14;
	
	if (msgPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length - 4));	
	CWProtocolStore8(msgPtr,  join_switch); 
	CWProtocolStore8(msgPtr,  discard_count);
	CWProtocolStore16(msgPtr, scan_time);
	CWProtocolStore32(msgPtr, aging_time); 
	CWProtocolStore8(msgPtr,  sta_count); 
	CWProtocolStore8(msgPtr,  signal_strength);

	wid_syslog_debug_debug(WID_WTPINFO, "elementid = %d\n", elementid);
	wid_syslog_debug_debug(WID_WTPINFO, "length = %d\n", length);
	wid_syslog_debug_debug(WID_WTPINFO, 
		"join_switch=%d,discard_count=%d,scan_time=%d,aging_time=%d,sta_count=%d,sig_stren=%d\n",
		join_switch, discard_count, scan_time, aging_time, sta_count, signal_strength);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleWtpStaAclList
(
	CWProtocolMessage *msgPtr,
	unsigned char wlanid,
	unsigned char radioid,										
	unsigned char oprate,
	unsigned char op_level,
	unsigned char groupid
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Element ID         |        Element Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    level      |  radio id     |   wlan id     |   oprate      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   acl_count   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             rule_proto        |              rule_len         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  rule_action  |  rule_type    |     direct	|     chain     | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                            dset ip                            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                            source ip                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            dst port           |       source port             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| dset mask     |sorce mask     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           destip end                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             source ip end                     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|              rule_proto       |            rule_len           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   rule_action | domain_str                           ................                    
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
	short int elementid = 41; 
	short int length = 0;
	int i = 0;
	unsigned int rule_id = 0;
	unsigned char acl_count = 0;
	ACL_RULElist *rulelist = NULL;	
	STA_ACL *STA_ACL_TMP[GROUP_ACL_COUNT] = {0};

	if(NULL == msgPtr)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	length = 9;
	
	if(CANCEL == oprate)
	{
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStore16(msgPtr, elementid); 
		CWProtocolStore16(msgPtr, (length-4));
		CWProtocolStore8(msgPtr, op_level);
		CWProtocolStore8(msgPtr, radioid); 
		CWProtocolStore8(msgPtr, wlanid); 
		CWProtocolStore8(msgPtr, oprate); 
		CWProtocolStore8(msgPtr, acl_count);
		
        wid_syslog_debug_debug(WID_DEFAULT,"%s: elemid %d elemlen %d op_level %d radio %d wlan%d oprate %d acl_count %d\n", 
                            __func__,elementid,length,op_level,radioid,wlanid,oprate,acl_count);
	}
	else
	{
		if((ACL_GROUP_NUM <= groupid) || (NULL == STA_ACL_GROUP[groupid]))
		{
			return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		}
		memset(STA_ACL_TMP, 0, sizeof(STA_ACL *) * GROUP_ACL_COUNT);
		
		CWThreadMutexLock(&STA_ACL_GROUP[groupid]->AclGrpMutex);
		
		acl_count = STA_ACL_GROUP[groupid]->rule_cnt;
		rulelist = STA_ACL_GROUP[groupid]->rule_list;
		
		for (i = 0; (i < acl_count) && (NULL != rulelist); i++)
		{
			rule_id = rulelist->rule_id;
			
			if(NULL == STA_ACL_RULE[rule_id])
			{
				CWThreadMutexUnlock(&STA_ACL_GROUP[groupid]->AclGrpMutex);
				return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
			}
			
			if (ACLGRP_PROTO_DOMAIN == STA_ACL_RULE[rule_id]->rule_proto)
			{
				length += 5 + STA_ACL_RULE[rule_id]->u.domain.len;
			}
			else
			{
				length += 30;
			}
			
			STA_ACL_TMP[i] = STA_ACL_RULE[rule_id];
			rulelist = rulelist->next;
		}
		CWThreadMutexUnlock(&STA_ACL_GROUP[groupid]->AclGrpMutex);
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStore16(msgPtr, elementid); 
		CWProtocolStore16(msgPtr, (length-4));
		CWProtocolStore8(msgPtr, op_level);
		CWProtocolStore8(msgPtr, radioid); 
		CWProtocolStore8(msgPtr, wlanid); 
		CWProtocolStore8(msgPtr, oprate); 
		CWProtocolStore8(msgPtr, acl_count);
		
        wid_syslog_debug_debug(WID_DEFAULT,"%s: elemid %d elemlen %d op_level %d radio %d wlan%d oprate %d acl_count %d\n", 
                            __func__,elementid,length,op_level,radioid,wlanid,oprate,acl_count);
                            
		for (i = 0; i < acl_count; i++)
		{			
			if(NULL == STA_ACL_TMP[i])
			{				
				return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
			}

			if (ACLGRP_PROTO_DOMAIN == STA_ACL_TMP[i]->rule_proto)
			{
				CWProtocolStore16(msgPtr, STA_ACL_TMP[i]->rule_proto);
				CWProtocolStore16(msgPtr, (1 + STA_ACL_TMP[i]->u.domain.len));
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->rule_action);
				CWProtocolStoreRawBytes(msgPtr, (char *)STA_ACL_TMP[i]->u.domain.dom_str, STA_ACL_TMP[i]->u.domain.len);

                wid_syslog_debug_debug(WID_DEFAULT,"%s: i=%d proto %d action %d domain %s len %d\n",
                                __func__,i,STA_ACL_TMP[i]->rule_proto,STA_ACL_TMP[i]->rule_action,
                                STA_ACL_TMP[i]->u.domain.dom_str,STA_ACL_TMP[i]->u.domain.len);
			    
			}
			else if ((ACLGRP_PROTO_TCP == STA_ACL_TMP[i]->rule_proto)
					||(ACLGRP_PROTO_UDP == STA_ACL_TMP[i]->rule_proto))
			{
				CWProtocolStore16(msgPtr, STA_ACL_TMP[i]->rule_proto);
				CWProtocolStore16(msgPtr, 26);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->rule_action);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->rule_type); 		
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->direct);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->direct);
				CWProtocolStore32(msgPtr, STA_ACL_TMP[i]->u.tcpudp.dip); 		
				CWProtocolStore32(msgPtr, STA_ACL_TMP[i]->u.tcpudp.sip); 		
				CWProtocolStore16(msgPtr, STA_ACL_TMP[i]->u.tcpudp.dport);
				CWProtocolStore16(msgPtr, STA_ACL_TMP[i]->u.tcpudp.sport);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.tcpudp.dmask);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.tcpudp.smask);
				CWProtocolStore32(msgPtr, 0);
				CWProtocolStore32(msgPtr, 0);
				
                wid_syslog_debug_debug(WID_DEFAULT,"%s: i=%d proto %d action %d type %d direct %d "
                                                   "dip %d sip %d dport %d sport %d dmask %d smask %d\n",
                                __func__,i,STA_ACL_TMP[i]->rule_proto,STA_ACL_TMP[i]->rule_action,
                                STA_ACL_TMP[i]->rule_type,STA_ACL_TMP[i]->direct,
                                STA_ACL_TMP[i]->u.tcpudp.dip,STA_ACL_TMP[i]->u.tcpudp.sip,
                                STA_ACL_TMP[i]->u.tcpudp.dport,STA_ACL_TMP[i]->u.tcpudp.sport,
                                STA_ACL_TMP[i]->u.tcpudp.dmask,STA_ACL_TMP[i]->u.tcpudp.smask);
			}
			else if ((ACLGRP_PROTO_ICMP == STA_ACL_TMP[i]->rule_proto))
			{
				CWProtocolStore16(msgPtr, STA_ACL_TMP[i]->rule_proto);
				CWProtocolStore16(msgPtr, 26);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->rule_action);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->rule_type); 		
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->direct);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->direct);
				CWProtocolStore32(msgPtr, STA_ACL_TMP[i]->u.icmp.dip);			
				CWProtocolStore32(msgPtr, STA_ACL_TMP[i]->u.icmp.sip);			
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.icmp.type);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.icmp.typemask);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.icmp.code);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.icmp.codemask);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.icmp.dmask);
				CWProtocolStore8(msgPtr, STA_ACL_TMP[i]->u.icmp.smask);
				CWProtocolStore32(msgPtr, 0);
				CWProtocolStore32(msgPtr, 0);
				
                wid_syslog_debug_debug(WID_DEFAULT,"%s: i=%d proto %d action %d type %d direct %d dip %d sip %d "
                                                   "type %d typemask %d code %d codemask %d dmask %d smask %d\n",
                                __func__,i,STA_ACL_TMP[i]->rule_proto,STA_ACL_TMP[i]->rule_action,
                                STA_ACL_TMP[i]->rule_type,STA_ACL_TMP[i]->direct,
                                STA_ACL_TMP[i]->u.icmp.dip,STA_ACL_TMP[i]->u.icmp.sip,
                                STA_ACL_TMP[i]->u.icmp.type,STA_ACL_TMP[i]->u.icmp.typemask,
                                STA_ACL_TMP[i]->u.icmp.code,STA_ACL_TMP[i]->u.icmp.codemask,
                                STA_ACL_TMP[i]->u.icmp.dmask,STA_ACL_TMP[i]->u.icmp.smask);
			}			
		}
	}

	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}


CWBool CWAssembleMsgElemWTPUpgradeMode(CWProtocolMessage *msgPtr, unsigned char mode)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|		  element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	  mode   	|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

	short int elementid = CW_VENDOR_SPEC_ELEMENT_UPGRADE_MODE; 
	short int length = 5;
	
	if (msgPtr == NULL) 
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	

	CWProtocolStore8(msgPtr, mode);

	wid_syslog_debug_debug(WID_DEFAULT,"%s: elemid: %d elemlen: %d mode: %d\n", 
						__func__,elementid,length,mode);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}


CWBool CWAssembleMsgElemAPQosMapInfo(CWProtocolMessage *msgPtr,
												unsigned char radioid,
												unsigned char wlanid,
												unsigned char subtype,
												unsigned char count,
												unsigned char *mapinfo)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|		  element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	  radio 	|  wlan 		|	subtype 	|	   count	|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	  index 	|  value		|	  index 	|  value		|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
	short int elementid = 46; 
	short int length = 8 + count*2;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if(mapinfo == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	
	CWProtocolStore8(msgPtr, radioid); 
	CWProtocolStore8(msgPtr, wlanid);
	CWProtocolStore8(msgPtr, subtype);
	CWProtocolStore8(msgPtr, count); 
	
	CWProtocolStoreRawBytes(msgPtr,(char *)mapinfo,count*2);

	wid_syslog_debug_debug(WID_DEFAULT,"elemid %d,elemlen %d,radio %d,wlan %d,type %d,count %d\n", elementid,length,radioid,wlanid,subtype,count);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemAPQosMapSwitch(CWProtocolMessage *msgPtr,
												unsigned char radioid,
												unsigned char wlanid,
												unsigned char subtype,
												unsigned char policy)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|		  element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	  radio 	|  wlan 		|	subtype 	|	   switch	|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
	short int elementid = 45; 
	short int length = 8;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	
	CWProtocolStore8(msgPtr, radioid); 
	CWProtocolStore8(msgPtr, wlanid);
	CWProtocolStore8(msgPtr, subtype);
	CWProtocolStore8(msgPtr, policy); 	

	wid_syslog_debug_debug(WID_DEFAULT,"elemid %d,elemlen %d,radio %d,wlan %d,type %d,policy %d\n", elementid,length,radioid,wlanid,subtype,policy);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);

}

CWBool CWAssembleMsgElemWLANloadbalance
(
	 CWProtocolMessage *msgPtr,
	 unsigned char radioid,
	 unsigned char wlanid,
	 unsigned char method
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|		  	element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|	l_radioid	|     wlanid	|	 method 	|		
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
	short int elementid = CW_VENDOR_SPEC_ELEMENT_WLAN_LOAD_BALANCE; 
	short int length = 7;
	
	if (msgPtr == NULL) 
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));	
	CWProtocolStore8(msgPtr, radioid);
	CWProtocolStore8(msgPtr, wlanid);
	CWProtocolStore8(msgPtr, method);

	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}




CWBool CWAssembleWtpRougeApList
(
    CWProtocolMessage *msgPtr,
    unsigned int wtpid,
    unsigned int oprate,
	unsigned char radioid
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            type = 37          |             length            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|f|                      vendor_id = 31656                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Element ID         |        Element Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Sub Element ID = 1       |             length            |  //Rogue AP restrain Switch
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Radioid    |  AP type = 1  |     Switch	|    Reserved   | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Sub Element ID = 2       |            length             |  //Rogue AP restrain oprate 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       Rogue AP Number         |     oprate    |    channel   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             BSSID                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             BSSID             |     oprate    |    channel    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          ..........                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
	short int elementid = CW_VENDOR_SPEC_ELEMENT_WTP_ROGUE_AP_LIST; 
	short int length = 0;
	int i = 0;
	unsigned int RougeapInfosCount = 0;
	unsigned int rouge_count = 0;
	unsigned int check_rouge_count = 0;

	if(NULL == msgPtr)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	length = 12;
	
	if(ROGUE_CANCEL == oprate)
	{
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStore16(msgPtr, elementid); 
		CWProtocolStore16(msgPtr, (length-4));
		CWProtocolStore16(msgPtr, 1);
		CWProtocolStore16(msgPtr, 4);
		CWProtocolStore8(msgPtr, radioid); 
		CWProtocolStore8(msgPtr, 1); 
		CWProtocolStore8(msgPtr, gapscanset.countermeasures_switch); 
		CWProtocolStore8(msgPtr, 0);
		
        wid_syslog_debug_debug(WID_DEFAULT,"%s: elemid %d,elemlen %d,radio %d switch %d\n",
                                __func__,elementid,length,radioid,gapscanset.countermeasures_switch);
	}
	else
	{
		Neighbor_AP_INFOS *paplist = NULL;
		struct Neighbor_AP_ELE *pnode = NULL;
		struct Neighbor_AP_ELE *pnext = NULL;

		WID_CHECK_WTP_STANDARD_RET(wtpid,CW_FALSE);
		WID_CHECK_L_RADIOID_STANDARD_RET(radioid,CW_FALSE);
		
		CWThreadMutexLock(&(gWTPs[wtpid].RRMThreadMutex));
		
		paplist = AC_WTP[wtpid]->rouge_ap_infos[radioid];
		if((paplist == NULL)||(paplist->neighborapInfos == NULL)||(paplist->neighborapInfosCount == 0))
		{
			CWThreadMutexUnlock(&(gWTPs[wtpid].RRMThreadMutex));
			return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		}

		RougeapInfosCount = paplist->neighborapInfosCount;
		pnext = paplist->neighborapInfos;

        for (i = 0; (i < RougeapInfosCount) && (NULL != pnext); i++)
        {
            if((0 == pnext->agingstatus)||(2 == pnext->agingstatus))
            {
                /*if((0 == paplist->checkrogueapcount)
                    || ((0 != paplist->checkrogueapcount) && (i > paplist->checkrogueapcount)))*/
                {    
                    rouge_count++;
                }    
            }
            pnext = pnext->next;
        }
        
        if(0 == rouge_count)
        {
            wid_syslog_debug_debug(WID_WTPINFO,"%s: radioid %d rouge_count %d\n",__func__,radioid,rouge_count);
            CWThreadMutexUnlock(&(gWTPs[wtpid].RRMThreadMutex));
            return CW_FALSE;
        }
        int blackcount = rouge_count;
        if(AP_SCAN_COUNTERMEASURES_PTK < rouge_count)
        {
            rouge_count = AP_SCAN_COUNTERMEASURES_PTK;
        }

		length += 6 + rouge_count * 8;
		CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, 
								{CWThreadMutexUnlock(&(gWTPs[wtpid].RRMThreadMutex));
								 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});

		CWProtocolStore16(msgPtr, elementid); 
		CWProtocolStore16(msgPtr, (length-4));
		CWProtocolStore16(msgPtr, 1);
		CWProtocolStore16(msgPtr, 4);
		CWProtocolStore8(msgPtr, radioid); 
		CWProtocolStore8(msgPtr, 1); 
		CWProtocolStore8(msgPtr, gapscanset.countermeasures_switch); 
		CWProtocolStore8(msgPtr, 0); 
		CWProtocolStore16(msgPtr, 2);
		CWProtocolStore16(msgPtr, 2 + rouge_count * 8);
		CWProtocolStore16(msgPtr, rouge_count);
		
		wid_syslog_debug_debug(WID_DEFAULT,"%s: elemid %d,elemlen %d,radio %d switch %d count %d\n",
    			    __func__,elementid,length,radioid,gapscanset.countermeasures_switch,rouge_count);

		pnext = paplist->neighborapInfos;
		for (i = 0; (i < RougeapInfosCount) && (NULL != pnext); i++)
		{
			if((0 == pnext->agingstatus)||(2 == pnext->agingstatus))
			{
                /*if((0 == paplist->checkrogueapcount)
                    || ((0 != paplist->checkrogueapcount) && (i > paplist->checkrogueapcount)))*/
			//{
			    if(0 == pnext->agingstatus)
			    {
                    oprate = ROGUE_ADD;
                    pnext->agingstatus = 1;
			    }
			    else if(2 == pnext->agingstatus)
			    {
			        oprate = ROGUE_DEL;
                    pnext->agingstatus = 3;
			    }
    			CWProtocolStore8(msgPtr, oprate);
				CWProtocolStore8(msgPtr, pnext->Channel);
				CWProtocolStore8(msgPtr, pnext->BSSID[0]); 
				CWProtocolStore8(msgPtr, pnext->BSSID[1]); 
				CWProtocolStore8(msgPtr, pnext->BSSID[2]); 
				CWProtocolStore8(msgPtr, pnext->BSSID[3]);
				CWProtocolStore8(msgPtr, pnext->BSSID[4]);
				CWProtocolStore8(msgPtr, pnext->BSSID[5]);
				
                check_rouge_count++;
				wid_syslog_debug_debug(WID_DEFAULT,"%s: i=%d "MACSTR" channel %d oprate %d\n",__func__,i,MAC2STR(pnext->BSSID),pnext->Channel,oprate); 
            //}
            } 
            
            if((AP_SCAN_COUNTERMEASURES_PTK < blackcount)&&(check_rouge_count >= AP_SCAN_COUNTERMEASURES_PTK))
            {
                paplist->checkrogueapcount = i;
                break;
            }
            else
            {
                paplist->checkrogueapcount = 0;
            }

			pnext = pnext->next;
		}
		
		pnode = paplist->neighborapInfos;
        while (NULL != pnode)
        {
            pnext = pnode->next;
            if(3 == pnode->agingstatus)
            {
                delete_elem_from_ap_list(paplist,pnode);
            }

            pnode = pnext;
        }

		CWThreadMutexUnlock(&(gWTPs[wtpid].RRMThreadMutex));
	}

	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

