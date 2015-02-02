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
* ACImageData.c
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
#include "CWAC.h"
#include "ACImageData.h"
#include "ACDbus.h"
#include "wcpss/waw.h"

#include "wcpss/wid/WID.h"
#include "dbus/wcpss/ACDbusDef1.h"
#include "ACDbus.h"
#include "ACDbus_handler.h"
#include "ACUpdateManage.h"
#include "AC.h"
#include "CWLog.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

int gCWImageDataPendingTimer = CW_IMAGE_INTERVAL_DEFAULT;
extern struct ap_update_config  g_ap_udapte_config;
extern CWBool CWAssembleResetRequestMessage_cw
(
	CWProtocolMessage **messagesPtr, 
	int *fragmentsNumPtr, 
	int PMTU, 
	unsigned char  seqNum,
	CWImageIdentifier_cw *resPtr,
	int WTPIndex
);

CWBool CWCheckImageIdentifier(CWImageIdentifier *valPtr, CWImageIdentifier *resPtr){
	CWConfigVersionInfo *info = gConfigVersionInfo;
	if(info == NULL){
		wid_syslog_err("gConfigVersionInfo is NULL\n");
		return CW_FALSE;
	}
	while(info != NULL){
		//printf("info->str_ap_version_name %s\n",info->str_ap_version_name);
		wid_syslog_err("info->str_ap_version_name %s\n",info->str_ap_version_name);
		unsigned int model_len = 0;
		
		model_len = strlen(info->str_ap_model);
		if(model_len == valPtr->modelLEN)
		{
			if((memcmp(info->str_ap_version_name, valPtr->Ver, valPtr->VerLen) == 0)
				||((memcmp(info->str_ap_model, valPtr->model, valPtr->modelLEN) == 0)&&(memcmp(info->str_ap_version_name, valPtr->Ver, valPtr->VerLen) == 0)))
			{
				resPtr->Ver = info->str_ap_version_path;
				resPtr->VerLen = strlen(info->str_ap_version_path);
				//printf("give ap version is:%s\n",resPtr->Ver);
				wid_syslog_err("give ap version is:%s\n",resPtr->Ver);
				resPtr->VerType = 0;
				resPtr->VerNum = 1;
				return CW_TRUE;
			}
		}
		info = info->next;
	}
	wid_syslog_err("info == NULL\n");
	//printf("info == NULL\n");
	return CW_FALSE;
}
CWBool CWParseMsgElemImageIdentifier(CWProtocolMessage *msgPtr, int len, CWImageIdentifier *valPtr) 
{
	wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemImageIdentifier\n");
	CWParseMessageElementStart();
	int a=0, b=0;
	valPtr->VerNum = CWProtocolRetrieve8(msgPtr);
	a = CWProtocolRetrieve8(msgPtr);
	b = CWProtocolRetrieve16(msgPtr);
	wid_syslog_debug_debug(WID_WTPINFO,"a %d b %d\n",a,b);
	valPtr->VerType = CWProtocolRetrieve16(msgPtr);
	valPtr->VerLen = CWProtocolRetrieve16(msgPtr);
	if((valPtr->VerLen<0)||(valPtr->VerLen>64)){
		wid_syslog_err("%s VerLen==%d\n",__func__,valPtr->VerLen);
		valPtr->VerLen = 64;
	}else{
	
	}

	switch(valPtr->VerType)
	{
		case 1000:
			
			CW_CREATE_STRING_ERR(valPtr->model, 4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			memset(valPtr->model,0, 5);
			memcpy(valPtr->model,"2010",4);
			valPtr->modelLEN = 4;
			wid_syslog_debug_debug(WID_WTPINFO,"2010\n");
			valPtr->Ver = CWProtocolRetrieveRawBytes(msgPtr, valPtr->VerLen);
			break;
		case 2000:
			CW_CREATE_STRING_ERR(valPtr->model, 5, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			memset(valPtr->model,0, 6);
			memcpy(valPtr->model,"1110T",5);
			valPtr->modelLEN = 5;
			wid_syslog_debug_debug(WID_WTPINFO,"1110T\n");
			valPtr->Ver = CWProtocolRetrieveRawBytes(msgPtr, valPtr->VerLen);
			break;
		case 1:
			valPtr->modelLEN = valPtr->VerLen;
			valPtr->model = CWProtocolRetrieveRawBytes(msgPtr, valPtr->VerLen);
			valPtr->VerLen = CWProtocolRetrieve16(msgPtr);
			valPtr->Ver = CWProtocolRetrieveRawBytes(msgPtr, valPtr->VerLen);
			//printf("valPtr->model:%s,valPtr->VerLen:%d,valPtr->Ver:%s",valPtr->model,valPtr->VerLen,valPtr->Ver);
			break;
		default:
			CW_CREATE_STRING_ERR(valPtr->model, 4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			memset(valPtr->model,0, 5);
			memcpy(valPtr->model,"1000",4);
			valPtr->modelLEN = 4;
			wid_syslog_debug_debug(WID_WTPINFO,"1000\n");
			valPtr->Ver = CWProtocolRetrieveRawBytes(msgPtr, valPtr->VerLen);
			break;			

	}
	
	wid_syslog_debug_debug(WID_WTPINFO,"valPtr->VerLen %d  valPtr->Ver %s valPtr->model %s \n",valPtr->VerLen,valPtr->Ver,valPtr->model);
	//printf("valPtr->VerLen %d  valPtr->Ver %s valPtr->model %s \n",valPtr->VerLen,valPtr->Ver,valPtr->model);
	CWParseMessageElementEnd();
}


CWBool CWParseMsgElemEncryptType(CWProtocolMessage *msgPtr, int len, CWEncryptionType *valPtr) 
{
	wid_syslog_debug_debug(WID_WTPINFO,"%s\n",__func__);
	CWParseMessageElementStart();
	valPtr->encrypt_type= CWProtocolRetrieve8(msgPtr);
	wid_syslog_debug_debug(WID_WTPINFO,"%s encrypt_type %d\n",__func__, valPtr->encrypt_type);
	CWParseMessageElementEnd();
}

CWBool CWParseImageDataRequestMessage(CWProtocolMessage* msgPtr, int len, CWImageDataRequest *valuesPtr){
	int offsetTillMessages;
	if(msgPtr == NULL || valuesPtr==NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;
	//printf("Parsing Image data request...\n");
	wid_syslog_debug_debug(WID_WTPINFO,"Parsing Image data request...");
	valuesPtr->ImageRequest = NULL;
	valuesPtr->EncryptType = NULL;
	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		unsigned short int subelemid=0;
		unsigned short int subelemlen=0;
		int IGNORE_UBSV venderid = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				if(valuesPtr->ImageRequest)
				{
					CW_FREE_OBJECT(valuesPtr->ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr->ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr->ImageRequest);
				}
				CW_CREATE_OBJECT_ERR(valuesPtr->ImageRequest, CWImageIdentifier, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				memset(valuesPtr->ImageRequest, 0, sizeof(CWImageIdentifier));
				if (!(CWParseMsgElemImageIdentifier(msgPtr, elemLen, valuesPtr->ImageRequest))){
					wid_syslog_debug_debug(WID_WTPINFO,"wrong in CWParseMsgElemImageIdentifier\n");
					/*free valuesPtr->ImageRequest*/
					CW_FREE_OBJECT(valuesPtr->ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr->ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr->ImageRequest);
					return CW_FALSE;
				}
				break;	
		      case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				venderid = CWProtocolRetrieve32(msgPtr);
				CWParseFormatMsgElem(msgPtr, &subelemid, &subelemlen);
				if(subelemid == CW_VENDOR_EVENT_ELEMENT_ENCRYPT_TYPE )
				{
					if(valuesPtr->EncryptType)
					{
						CW_FREE_OBJECT(valuesPtr->EncryptType);
					}
					
					CW_CREATE_OBJECT_ERR(valuesPtr->EncryptType, CWEncryptionType, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
					memset(valuesPtr->EncryptType, 0, sizeof(CWEncryptionType));
					
					if (!(CWParseMsgElemEncryptType(msgPtr, subelemlen, valuesPtr->EncryptType)))
					{
						wid_syslog_debug_debug(WID_WTPINFO,"wrong in %s\n",__func__);
						CW_FREE_OBJECT(valuesPtr->EncryptType);
						return CW_FALSE;
					}	
				}
				break;
			default:				
				wid_syslog_debug_debug(WID_WTPINFO,"wrong in default\n");
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Image data request");
				// no need break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len){ 
		//printf("((msgPtr->offset - offsetTillMessages) != len)\n");
		wid_syslog_debug_debug(WID_WTPINFO,"((msgPtr->offset - offsetTillMessages) != len)");
		/*free valuesPtr->ImageRequest and its elements */
		if(valuesPtr->ImageRequest)
		{
			CW_FREE_OBJECT(valuesPtr->ImageRequest->Ver);
			CW_FREE_OBJECT(valuesPtr->ImageRequest->model);
			CW_FREE_OBJECT(valuesPtr->ImageRequest);
		}
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	//printf("Image data request Parsed\n");
	wid_syslog_debug_debug(WID_WTPINFO,"Image data request Parsed");
	return CW_TRUE;	
}

CWBool CWAssembleMsgElemImageIdentifierAC(CWProtocolMessage *msgPtr, CWImageIdentifier *resPtr){
	int size = 8;
	size += resPtr->VerLen;	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CWProtocolStore8(msgPtr, resPtr->VerNum);
	CWProtocolStore8(msgPtr, 0);	
	CWProtocolStore16(msgPtr, 0);
	CWProtocolStore16(msgPtr, resPtr->VerType);	
	CWProtocolStore16(msgPtr, resPtr->VerLen);
	CWProtocolStoreRawBytes(msgPtr, resPtr->Ver, resPtr->VerLen);
	//wid_syslog_debug_debug("resPtr->VerLen %d resPtr->Ver %s\n",resPtr->VerLen, resPtr->Ver);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE);
}

CWBool CWAssembleMsgElemServerInformation
(
	CWProtocolMessage *msgPtr,
	CWEncryptionType *resPtr,
	int WTPIndex
)
{
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|		  element				|		   len					|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|	                          TFTP/FTP server ip                                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|	           user-len                       |                  user                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|	  . . .                         |     passwd-len    |    passwd ....        
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|	  . . .                                |     encryption type    |         
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
*/	
	int user_len = 0;
	int passwd_len = 0;
	
	//char *str = " ";
	//char __str[128];	
	unsigned int ipaddr = 0;
	struct sockaddr_in  *sin = NULL;
	unsigned char encryption_type_list = 0;
	int i=0;
	short int elementid = CW_VENDOR_SPEC_ELEMENT_SUPPORT_ENCRYPTION; 
	short int length = 4;
	
	for(i=0;g_ap_udapte_config.user[i] != 0;i++)
	{
		user_len++;
	}

	for(i=0;g_ap_udapte_config.passwd[i] != 0;i++)
	{
		passwd_len++;
	}
	wid_syslog_debug_debug(WID_DEFAULT,"%s: user_len  %d passwd_len %d\n",__func__, user_len, passwd_len);

	/*ip + user_len + user + passed_len + passwd + encryption_type_list*/
	length +=4 +  2 + user_len + 2 + passwd_len + 1;

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	

	CWProtocolStore16(msgPtr, elementid); 
	CWProtocolStore16(msgPtr, (length-4));
	
	if(0 != g_ap_udapte_config.ipaddr)
	{
		CWProtocolStore32(msgPtr, g_ap_udapte_config.ipaddr);
	}
	else
	{
		if(gWTPs != NULL && gInterfaces != NULL)
		{
			sin = (struct sockaddr_in*)&(gInterfaces[gWTPs[WTPIndex].interfaceIndex].addr);
			ipaddr = (unsigned int)(sin->sin_addr.s_addr);
		}
		CWProtocolStore32(msgPtr, ipaddr);
	}
	wid_syslog_debug_debug(WID_DEFAULT,"%s: 111\n",__func__);
	CWProtocolStore16(msgPtr, (unsigned short)user_len);
	CWProtocolStoreRawBytes(msgPtr, (char*)(g_ap_udapte_config.user), user_len);
	
	wid_syslog_debug_debug(WID_DEFAULT,"%s: 222\n",__func__);
	CWProtocolStore16(msgPtr, (unsigned short)passwd_len);
	CWProtocolStoreRawBytes(msgPtr, (char*)(g_ap_udapte_config.passwd), passwd_len);
	
	wid_syslog_debug_debug(WID_DEFAULT,"%s: 333\n",__func__);
	encryption_type_list = (resPtr->encrypt_type)&(g_ap_udapte_config.encrypt_type);
	CWProtocolStore8(msgPtr, encryption_type_list);	

	//str = sock_ntop_r(((struct sockaddr*)&(gInterfaces[gWTPs[WTPIndex].interfaceIndex].addr)), __str);
	/*
	wid_syslog_debug_debug(WID_DEFAULT, "%s: user %s passwd %s encrypt %d",
					__func__, 
					g_ap_udapte_config.user,
					g_ap_udapte_config.passwd,
					encryption_type_list);
	*/	
	wid_syslog_debug_debug(WID_DEFAULT,"%s: before CWAssembleMsgElem\n",__func__);
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);


	/*
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

	wid_syslog_debug_debug(WID_DEFAULT,"%s: elemid %d elemlen %d mode %d\n", 
						__func__,elementid,length,mode);
		
	return CWAssembleMsgElemVendor(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
	*/
}

CWBool CWAssembleImageDataRequestMessage
(
	CWProtocolMessage **messagesPtr, 
	int *fragmentsNumPtr,
	int PMTU,
	int seqNum,
	CWImageIdentifier *resPtr,
	int WTPIndex
)
{
	CWProtocolMessage *msgElems = NULL;
	const int MsgElemCount = 1;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemImageIdentifierAC(&(msgElems[++k]), resPtr))))
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST, msgElems, MsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n",  \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,  \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST),  \
					seqNum, AC_WTP[WTPIndex]->cap_pro_ver, MsgElemCount);
	
	return CW_TRUE;
}

CWBool CWAssembleImageDataRequestMessage_new
(
	CWProtocolMessage **messagesPtr,
	int *fragmentsNumPtr, int PMTU, 
	int seqNum,
	CWImageIdentifier *resPtr,
	CWEncryptionType *encryptypePtr,
	int WTPIndex
)
{
	CWProtocolMessage *msgElems = NULL;
	const int MsgElemCount = 2;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount = 0;
	int k = -1;
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemImageIdentifierAC(&(msgElems[++k]), resPtr)))
		||(!(CWAssembleMsgElemServerInformation(&(msgElems[++k]), 
													encryptypePtr,
													WTPIndex))))
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr, 
							fragmentsNumPtr,
							PMTU, 
							seqNum,
							CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST, 
							msgElems, 
							MsgElemCount,
							msgElemsBinding,
							msgElemBindingCount,
							CW_PACKET_CRYPT,
							AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n",  \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
				WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST), seqNum, AC_WTP[WTPIndex]->cap_pro_ver, MsgElemCount);
	return CW_TRUE;
}

CWBool CWParseImageDataResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode)
{
	int offsetTillMessages = 0;
	if(msgPtr == NULL || (msgPtr->msg) == NULL || resultCode == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	offsetTillMessages = msgPtr->offset;
	
	wid_syslog_debug_debug(WID_WTPINFO,"Parsing Image Data Response...");
	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType)
		{
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode = CWProtocolRetrieve32(msgPtr);
				break;	
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				break;	
		}
	}
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s *resultCode %d", __func__, *resultCode);
	if((msgPtr->offset - offsetTillMessages) != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
    }
    
	wid_syslog_debug_debug(WID_WTPINFO,"Image Data Response Parsed");
	return CW_TRUE;	
}


CWBool ACEnterImageData(int WTPIndex, CWProtocolMessage *msgPtr)
{
	int i = 0, result = 0;
	CWControlHeaderValues controlVal;
	enum wtp_upgrade_mode upgrade_mode = WTP_UPGRADE_MODE_HTTP;
	
	wid_syslog_debug_debug(WID_WTPINFO,"wtp%d enter image data state msgPtr %p\n",WTPIndex, msgPtr);
	//CWProtocolMessage* messages =NULL;
	//int messagesCount=0;
	if(!msgPtr)
	{
		wid_syslog_err("%s line %d msgPtr NULL pointer!\n", __func__, __LINE__);
		return CW_FALSE;
	}
	
	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	memset(&controlVal, 0, sizeof(CWControlHeaderValues));
	msgPtr->offset = 0;
	
	if (!(CWACParseGenericRunMessage(WTPIndex, msgPtr, &controlVal)))
	{
		//## Two possible errors: WRONG_ARG and INVALID_FORMAT
		//## In the second case we have an unexpected response: ignore the
		//## message and log the event.
		return CW_FALSE;
	}
	
	if (!CWErr(CWTimerCancel(&(gWTPs[WTPIndex].currentTimer),1)))
	{
		_CWCloseThread(WTPIndex);
	}

	upgrade_mode = AC_WTP[WTPIndex]->upgrade.mode;

	if ((WTP_UPGRADE_MODE_TFTP == upgrade_mode)
		|| (WTP_UPGRADE_MODE_FTP == upgrade_mode)
		|| (WTP_UPGRADE_MODE_HTTP == upgrade_mode))
	{
		switch(controlVal.messageTypeValue)
		{
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
			{
				CWImageDataRequest valuesPtr;
				CWImageIdentifier resPtr;
				CWEncryptionType encryptypePtr;
				memset(&valuesPtr, 0, sizeof(CWImageDataRequest));
				memset(&resPtr, 0, sizeof(CWImageIdentifier));
				memset(&encryptypePtr, 0, sizeof(encryptypePtr));
				if(!(CWParseImageDataRequestMessage(msgPtr, controlVal.msgElemsLen, &valuesPtr)))
				{
					wid_syslog_warning("%s:%d wrong in CWParseImageDataRequestMessage\n",__func__,__LINE__);
					return CW_FALSE;		
				}
				if (NULL != valuesPtr.EncryptType)
				{
					encryptypePtr.encrypt_type = valuesPtr.EncryptType->encrypt_type;
				}
				
				if(gtrapflag >= 1)
				{
					#if NOSOFTAC
					wid_dbus_trap_wtp_enter_imagedata_state(WTPIndex);//wu:ap升级告警
					#endif
				}
							
				if((AC_WTP[WTPIndex]->updateversion != NULL)&&(AC_WTP[WTPIndex]->updatepath != NULL))
				{
					resPtr.Ver = AC_WTP[WTPIndex]->updatepath;
					resPtr.VerLen = strlen(AC_WTP[WTPIndex]->updatepath);
					resPtr.VerType = 0;
					resPtr.VerNum = 1;				
				}
				else
				{
					/*mahz modify code here to match ap upgrade batchlly*/
					CWConfigVersionInfo_new *tmpnode = gConfigVerInfo;
					CWBool bMatchVersion = CW_FALSE;
					
					if(find_in_wtp_list(WTPIndex) == CW_TRUE)
					{
						for(i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
						{
							CWConfigVersionInfo *update_node = gConfigVersionUpdateInfo[i];
							if((update_node != NULL)
								&&(update_node->str_ap_model != NULL)
								&&(0 == strcmp(update_node->str_ap_model,AC_WTP[WTPIndex]->WTPModel)))
							{
								while(update_node != NULL)
								{
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d enter image data upgrade\n",__func__,__LINE__,WTPIndex);
									if(0 == strcmp(update_node->str_ap_code,AC_WTP[WTPIndex]->APCode))
									{
										wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d code: %s upgrade code: %s version: %s path: %s\n",
										                        __func__,__LINE__,WTPIndex,AC_WTP[WTPIndex]->APCode,update_node->str_ap_code,
                                                                update_node->str_ap_version_name,update_node->str_ap_version_path);

										resPtr.Ver = update_node->str_ap_version_path;
										resPtr.VerLen = strlen(update_node->str_ap_version_path);
										resPtr.VerType = 0;
										resPtr.VerNum = 1;
										bMatchVersion = CW_TRUE;

										if(AC_WTP[WTPIndex]->updateStat >= 1)
										{
											AC_WTP[WTPIndex]->updatefailcount++;	
										}

										if(AC_WTP[WTPIndex]->updatefailcount >= updatemaxfailcount)
										{
											delete_wtp_list(WTPIndex);
											
											insert_uptfail_wtp_list(WTPIndex);
											
											update_complete_check();						

											AC_WTP[WTPIndex]->updatefailstate = 1;

											_CWCloseThread(WTPIndex);
										}
										//printf("ACEnterImageData update\n");

										result = 1;
										break;
									}
									update_node = update_node->next;
								}
								if(bMatchVersion == CW_FALSE)	//it will not goto this branch
								{
									resPtr.Ver = valuesPtr.ImageRequest->Ver;
									resPtr.VerLen = valuesPtr.ImageRequest->VerLen;
									resPtr.VerType = 0;
									resPtr.VerNum = 1;
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d check image data ver response\n",__func__,__LINE__);
									/*if(!(CWCheckImageIdentifier(valuesPtr.ImageRequest, &resPtr)))
									{
										//printf("wrong in CWCheckImageIdentifier\n");
										wid_syslog_debug_debug(WID_WTPINFO,"wrong in CWCheckImageIdentifier\n");
										return CW_FALSE;
									}*/
								}
								break;
							}
						}
					}
					/*ap upgrade automatically here,the gloable variable gConfigVersionUpdateInfo may be NULL here,so use gConfigVerInfo instead*/
					/*the upgrade automatically can be used now,but after the upgrade is over,the wtpcompatible.xml is not updated,so what the cmd
					show model list shows is still old information, this situation should be modified sooner or later*/
					else if(img_now == 1)
					{
						while(tmpnode != NULL)
						{
							if(strcmp(tmpnode->str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0)
							{
								wid_syslog_debug_debug(WID_WTPINFO,"%s:%d find model %s in apimg.xml\n",__func__,__LINE__,tmpnode->str_ap_model);
								CWCodeInfo *codenode = tmpnode->code_info;
								while(codenode != NULL)
								{
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d match code %s operation\n",__func__,__LINE__,codenode);
									if(strcmp(codenode->str_ap_version_code,AC_WTP[WTPIndex]->APCode) == 0)
									{
										resPtr.Ver = codenode->str_ap_version_path;
										resPtr.VerLen = strlen(codenode->str_ap_version_path);
										resPtr.VerType = 0;
										resPtr.VerNum = 1;
										
										wid_syslog_debug_debug(WID_WTPINFO,"%s:%d ac surport version name:%s\n",__func__,__LINE__,codenode->str_ap_version_name);
										break;
									}
									codenode = codenode->next;
								}
								break;
							}
							tmpnode = tmpnode->next;
						}
					}
				}
				gWTPs[WTPIndex].responseSeqNum = controlVal.seqNum;
				if(!CWAssembleImageDataRequestMessage_new(&(gWTPs[WTPIndex].messages), 
													     &(gWTPs[WTPIndex].messagesCount),
													      gWTPs[WTPIndex].pathMTU, controlVal.seqNum, 
													      &resPtr,
													      &encryptypePtr,
													      WTPIndex))
				{
					CW_FREE_OBJECT(valuesPtr.ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr.ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr.ImageRequest);
					CW_FREE_OBJECT(valuesPtr.EncryptType);
					return CW_FALSE;
				}
				gWTPs[WTPIndex].responseType = CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE;
				AC_WTP[WTPIndex]->updateStat = 1;
		
				if(gtrapflag >= 4)
				{
					#if NOSOFTAC
					wid_dbus_trap_wtp_tranfer_file(WTPIndex);
					#endif	
				}
				//fengwenchao add 20110216   save ap report version  for ap updata successful or fail
				if((valuesPtr.ImageRequest)&&(valuesPtr.ImageRequest->Ver))
				{
    				AC_WTP[WTPIndex]->ApReportVer = malloc(strlen(valuesPtr.ImageRequest->Ver)+1);
    				memset(AC_WTP[WTPIndex]->ApReportVer,0,strlen(valuesPtr.ImageRequest->Ver)+1);
    				memcpy(AC_WTP[WTPIndex]->ApReportVer,valuesPtr.ImageRequest->Ver,strlen(valuesPtr.ImageRequest->Ver));
				}

				//AC_WTP[WTPIndex]->ApReportVerLen = 	valuesPtr.ImageRequest->VerLen;
				//wid_syslog_debug_debug(WID_WTPINFO,"valuesPtr.ImageRequest->Ver = %s\n",valuesPtr.ImageRequest->Ver);
				//wid_syslog_debug_debug(WID_WTPINFO,"AC_WTP[WTPIndex]->ApReportVer = %s\n",AC_WTP[WTPIndex]->ApReportVer);
				//fengwenchao add end 
				CW_FREE_OBJECT(valuesPtr.ImageRequest->model);
				CW_FREE_OBJECT(valuesPtr.ImageRequest->Ver);
				CW_FREE_OBJECT(valuesPtr.ImageRequest);
				CW_FREE_OBJECT(valuesPtr.EncryptType);
				break;
			}
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE:
			{
				wid_syslog_debug_debug(WID_WTPINFO,"CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE");
				CWProtocolResultCode resultCode;			
				CWImageIdentifier resPtr;
				memset(&resultCode, 0, sizeof(CWProtocolResultCode));
				memset(&resPtr, 0, sizeof(CWImageIdentifier));
	//			CWThreadMutexLock(&(gAllThreadMutex));		
				//ap doesn't use the value of resPtr in Reset state
				if(AC_WTP[WTPIndex]->ver != NULL)
				{
					resPtr.Ver = AC_WTP[WTPIndex]->ver;
				}
				else
				{
					resPtr.Ver = " ";
				}
				//resPtr.Ver = gConfigVersionInfo->str_ap_version_path;
				resPtr.VerNum = 1;
				resPtr.VerType = 0;
				resPtr.VerLen = strlen(resPtr.Ver);

				if(!(CWParseImageDataResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode)))
				{
					//CWThreadMutexUnlock(&(gAllThreadMutex));		
					return CW_FALSE;
				}
				gWTPs[WTPIndex].responseSeqNum = controlVal.seqNum;
				if(!CWAssembleResetRequestMessage(&(gWTPs[WTPIndex].messages), 
												     &(gWTPs[WTPIndex].messagesCount),
												     gWTPs[WTPIndex].pathMTU, controlVal.seqNum,
												     &resPtr, WTPIndex))
				{
					
					wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wrong in CWAssembleResetRequestMessage",__func__,__LINE__);
					//CWThreadMutexUnlock(&(gAllThreadMutex));		
					return CW_FALSE;
				}
				
				//CWThreadMutexUnlock(&(gAllThreadMutex));		
				if(resultCode == CW_PROTOCOL_FAILURE_FIRM_WRT_ERROR)
				{	
					if(gtrapflag >= 4)
					{
					#if NOSOFTAC
						wid_dbus_trap_wtp_ap_flash_write_failed(WTPIndex);
					#endif
					}
				}
				
				time(&AC_WTP[WTPIndex]->imagedata_time);
				gWTPs[WTPIndex].responseType = CW_MSG_TYPE_VALUE_RESET_RESPONSE;
				gWTPs[WTPIndex].currentState = CW_ENTER_RESET;
				AC_WTP[WTPIndex]->WTPStat = WID_RESET;
				AC_WTP[WTPIndex]->updateStat = 2;
				break;			
			}
			default:
				//printf("controlVal.messageTypeValue %d\n",controlVal.messageTypeValue);
				result = 0;
				for(i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
				{
					if(gConfigVersionUpdateInfo[i] != NULL)
					{
						result = 1;
						break;
					}
				}
				if((result != 0)&&(find_in_wtp_list(WTPIndex) == CW_TRUE))
				{
					delete_wtp_list(WTPIndex);
					
					insert_uptfail_wtp_list(WTPIndex);	
					
					update_complete_check();
					
					AC_WTP[WTPIndex]->updatefailstate = 2;

					_CWCloseThread(WTPIndex);		
				}
				
				wid_syslog_debug_debug(WID_WTPINFO,"controlVal.messageTypeValue %d",controlVal.messageTypeValue);
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				// no need break;
		}
		
		if(!CWACSendFragments(WTPIndex))
		{
			for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
			{
				CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
			}
			
			CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
			return CW_FALSE;
		}
		
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter image-data send msg %s\n", \
							WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
							WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST)); 
		if(!CWErr(CWTimerRequest(gCWImageDataPendingTimer, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex))) 
		{
			// start Change State Pending timer
			_CWCloseThread(WTPIndex);
		}
		
		for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++) 
		{
			CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
		}
		
		CW_FREE_OBJECT(gWTPs[WTPIndex].messages);

	}
	else if (WTP_UPGRADE_MODE_CAPWAP == upgrade_mode)
	{
		/* CAPWAP */
		switch(controlVal.messageTypeValue) 
		{
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
			{
				CWImageDataRequest valuesPtr;
				CWImageIdentifier resPtr;
				memset(&valuesPtr, 0, sizeof(CWImageDataRequest));
				memset(&resPtr, 0, sizeof(CWImageIdentifier));
				
				if(!(CWParseImageDataRequestMessage_cw(msgPtr, controlVal.msgElemsLen, &valuesPtr)))
				{
					wid_syslog_err("%s:%d wtp%d parse imagedata request error\n",__func__,__LINE__,WTPIndex);
					return CW_FALSE;		
				}

				if(gtrapflag >= 1)
				{
				#if NOSOFTAC
					wid_dbus_trap_wtp_enter_imagedata_state(WTPIndex);
				#endif
				}
							
				//printf("ACEnterImageData\n");
				if((AC_WTP[WTPIndex]->updateversion != NULL)&&(AC_WTP[WTPIndex]->updatepath != NULL))
				{
					resPtr.Ver = AC_WTP[WTPIndex]->updatepath;
					resPtr.VerLen = strlen(AC_WTP[WTPIndex]->updatepath);
					resPtr.VerType = 0;
					resPtr.VerNum = 1;				
				}
				else
				{
					/*mahz modify code here to match ap upgrade batchlly*/
					CWConfigVersionInfo_new *tmpnode = gConfigVerInfo;
					CWBool bMatchVersion = CW_FALSE;
					
					if(find_in_wtp_list(WTPIndex) == CW_TRUE)
					{
						for(i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
						{
							CWConfigVersionInfo *update_node = gConfigVersionUpdateInfo[i];
							if((update_node != NULL)
							    &&(update_node->str_ap_model != NULL)
							    &&(strcmp(update_node->str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0))
							{
								while(update_node != NULL)
								{
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d enter image data upgrade\n",__func__,__LINE__,WTPIndex);
									if(strcmp(update_node->str_ap_code,AC_WTP[WTPIndex]->APCode) == 0)
									{
										wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d code: %s upgrade code: %s name: %s path: %s\n",
            										__func__,__LINE__,WTPIndex,AC_WTP[WTPIndex]->APCode,update_node->str_ap_code,
                                                    update_node->str_ap_version_name,update_node->str_ap_version_path);
				
										resPtr.Ver = update_node->str_ap_version_path;
										resPtr.VerLen = strlen(update_node->str_ap_version_path);
										resPtr.VerType = 0;
										resPtr.VerNum = 1;
										bMatchVersion = CW_TRUE;
				
										if(AC_WTP[WTPIndex]->updateStat >= 1)
										{
											AC_WTP[WTPIndex]->updatefailcount++;	
										}
				
										if(AC_WTP[WTPIndex]->updatefailcount >= updatemaxfailcount)
										{
											delete_wtp_list(WTPIndex);
											insert_uptfail_wtp_list(WTPIndex);
											
											update_complete_check();						
				
											AC_WTP[WTPIndex]->updatefailstate = 1;
				
											_CWCloseThread(WTPIndex);
										}
										//printf("ACEnterImageData update\n");
				
										result = 1;
										break;
									}
									update_node = update_node->next;
								}
								if(bMatchVersion == CW_FALSE)	//it will not goto this branch
								{
									resPtr.Ver = valuesPtr.ImageRequest->Ver;
									resPtr.VerLen = valuesPtr.ImageRequest->VerLen;
									resPtr.VerType = 0;
									resPtr.VerNum = 1;
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d check image data ver response\n",__func__,__LINE__);
									/*if(!(CWCheckImageIdentifier(valuesPtr.ImageRequest, &resPtr)))
									{
										//printf("wrong in CWCheckImageIdentifier\n");
										wid_syslog_debug_debug(WID_WTPINFO,"wrong in CWCheckImageIdentifier\n");
										return CW_FALSE;
									}*/
								}
								break;
							}
						}
					}
					/*ap upgrade automatically here,the gloable variable gConfigVersionUpdateInfo may be NULL here,so use gConfigVerInfo instead*/
					/*the upgrade automatically can be used now,but after the upgrade is over,the wtpcompatible.xml is not updated,so what the cmd
					show model list shows is still old information, this situation should be modified sooner or later*/
					else if(img_now == 1)
					{
						while(tmpnode != NULL)
						{
							if(strcmp(tmpnode->str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0)
							{
								wid_syslog_debug_debug(WID_WTPINFO,"%s:%d find model %s in apimg.xml\n",__func__,__LINE__,tmpnode->str_ap_model);
								CWCodeInfo *codenode = tmpnode->code_info;
								while(codenode != NULL)
								{
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d match code %s operation\n",__func__,__LINE__,codenode);
									if(strcmp(codenode->str_ap_version_code,AC_WTP[WTPIndex]->APCode) == 0)
									{
										resPtr.Ver = codenode->str_ap_version_path;
										resPtr.VerLen = strlen(codenode->str_ap_version_path);
										resPtr.VerType = 0;
										resPtr.VerNum = 1;
										
										wid_syslog_debug_debug(WID_WTPINFO,"%s:%d ac surport version name:%s\n",__func__,__LINE__,codenode->str_ap_version_name);
										break;
									}
									codenode = codenode->next;
								}
								break;
							}
							tmpnode = tmpnode->next;
						}
					}
				}

				
				/* init md5 hash  */
				if ((WTP_UPGRADE_MODE_CAPWAP == upgrade_mode)
					&& (WTP_UPGRADE_STATE_JOIN == AC_WTP[WTPIndex]->upgrade.state))
				{
					wtp_upgrade_init_hash(WTPIndex, resPtr.Ver, NULL);
				}

				CWProtocolImageDataResponseValues ImageDateRespVal;
				
				memset(&ImageDateRespVal, 0, sizeof(ImageDateRespVal));
				ImageDateRespVal.ImageInfoHash.filesize = AC_WTP[WTPIndex]->upgrade.filesize;
				memcpy(ImageDateRespVal.ImageInfoHash.hash, AC_WTP[WTPIndex]->upgrade.hash, WID_MD5_LEN);
				
			//	gWTPs[WTPIndex].responseSeqNum = controlVal.seqNum;
				if (!CWAssembleImageDataResponseMessage(&(gWTPs[WTPIndex].messages), 
														&(gWTPs[WTPIndex].messagesCount),
														gWTPs[WTPIndex].pathMTU,
									 					controlVal.seqNum,
														&ImageDateRespVal, WTPIndex))
				{
					/*valuesPtr.ImageRequest and its elements leak */
					CW_FREE_OBJECT(valuesPtr.ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr.ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr.ImageRequest);
					return CW_FALSE;
				}
			//	gWTPs[WTPIndex].responseType = 0;
				AC_WTP[WTPIndex]->updateStat = 1;

				/* SEND ImageData response */
				if(!CWACSendFragments(WTPIndex)) 
				{
					/*gWTPs[WTPIndex].messages leak*/
					for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
					{
						CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
					}					
					CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
					
					return CW_FALSE;
				}
				
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter image-data send msg %s\n", \
							WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
							WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE)); 
				for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
				{
					CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
				}
				CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
				
				if(gtrapflag >= 4)
				{
				#if NOSOFTAC
					wid_dbus_trap_wtp_tranfer_file(WTPIndex);
				#endif
				}

				if((valuesPtr.ImageRequest)&&(valuesPtr.ImageRequest->Ver))
				{
    				AC_WTP[WTPIndex]->ApReportVer = malloc(strlen(valuesPtr.ImageRequest->Ver)+1);
    				memset(AC_WTP[WTPIndex]->ApReportVer,0,strlen(valuesPtr.ImageRequest->Ver)+1);
    				memcpy(AC_WTP[WTPIndex]->ApReportVer,valuesPtr.ImageRequest->Ver,strlen(valuesPtr.ImageRequest->Ver));
				}
				CW_FREE_OBJECT(valuesPtr.ImageRequest->model);
				CW_FREE_OBJECT(valuesPtr.ImageRequest->Ver);
				CW_FREE_OBJECT(valuesPtr.ImageRequest);

				/* send image file */
				wid_wtp_upgrade(WTPIndex);
				
				break;
			}
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE:
			{
		//		wid_syslog_debug_debug(WID_WTPINFO,"CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE");
				
				CWProtocolResultCode resultCode;			
				memset(&resultCode, 0, sizeof(CWProtocolResultCode));

				if(!(CWParseImageDataResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode)))
				{
					return CW_FALSE;
				}

				/* TODO: */
				if(CW_PROTOCOL_SUCCESS != resultCode)
				{
					/* 
					delete_wtp_list(WTPIndex);
					insert_uptfail_wtp_list(WTPIndex);	
					update_complete_check();
					*/
					AC_WTP[WTPIndex]->updatefailstate = 2;				
					_CWCloseThread(WTPIndex);		
				}
				
				CWACStopRetransmission(WTPIndex);

				/* upgradeing ... */
				if (WTP_UPGRADE_STATE_IMAGEING == AC_WTP[WTPIndex]->upgrade.state)
				{
					wid_wtp_upgrade(WTPIndex);
				}				
				else if (WTP_UPGRADE_STATE_IMAGEEND == AC_WTP[WTPIndex]->upgrade.state)
				{
					CWImageIdentifier_cw resPtr;
					memset(&resPtr, 0 , sizeof(resPtr));

					if('\0' != AC_WTP[WTPIndex]->upgrade.ImageName[0])
					{
						strncpy(resPtr.ImageName, AC_WTP[WTPIndex]->upgrade.ImageName, WTP_IMAGENAME_LEN-1);
						resPtr.ImageNameLen = strlen(resPtr.ImageName);		
					}
					else
					{
						strncpy(resPtr.ImageName, "wtp.img", WTP_IMAGENAME_LEN-1);
						resPtr.ImageNameLen = strlen(resPtr.ImageName);				
					}
					
					if('\0' != AC_WTP[WTPIndex]->upgrade.version[0])
					{
						strncpy(resPtr.version, AC_WTP[WTPIndex]->upgrade.version, WTP_VERSION_LEN-1);
						resPtr.VersionLen = strlen(resPtr.version);		
					}
					else
					{
						strncpy(resPtr.version, "version", WTP_VERSION_LEN-1);
						resPtr.VersionLen = strlen(resPtr.version);		
					}
					
					if(AC_WTP[WTPIndex]->APCode != NULL)
					{
						strncpy(resPtr.Model, AC_WTP[WTPIndex]->APCode, WTP_MODE_LEN-1);
						resPtr.ModelLen = strlen(resPtr.Model);	
					}
					else
					{
						strncpy(resPtr.Model, "Model", WTP_MODE_LEN-1);
						resPtr.ModelLen = strlen(resPtr.Model);	
					}
					
					/* upgrade end enter reset state */
					
					int seqNum = CWGetSeqNum();
					if(!CWAssembleResetRequestMessage_cw(&(gWTPs[WTPIndex].messages),
														 &(gWTPs[WTPIndex].messagesCount),
														 gWTPs[WTPIndex].pathMTU, 
														 seqNum, &resPtr, WTPIndex))
					{
						wid_syslog_err("wtp%d assemble reset request msg failed\n", WTPIndex);
						return CW_FALSE;
					}
				
					AC_WTP[WTPIndex]->upgrade.state = WTP_UPGRADE_STATE_DONE;

					if(CWACSendAcknowledgedPacket(WTPIndex, CW_MSG_TYPE_VALUE_RESET_RESPONSE, seqNum)) 
					{
						//bResult = CW_TRUE;
					}
					else
					{
						CWACStopRetransmission(WTPIndex);
					}

					time(&AC_WTP[WTPIndex]->imagedata_time);
					gWTPs[WTPIndex].currentState = CW_ENTER_RESET;
					AC_WTP[WTPIndex]->WTPStat = WID_RESET;
					AC_WTP[WTPIndex]->updateStat = 2;
				
					wid_syslog_debug("%s:%d wtp%d imagedata finished enter reset\n",__func__,__LINE__,WTPIndex);

				}
				else
				{
					wid_syslog_err("%s:%d wtp%d %s upgrade state %d err\n",__func__,__LINE__,WTPIndex,
                                    mac2str(AC_WTP[WTPIndex]->WTPMAC), AC_WTP[WTPIndex]->upgrade.state);
				}
					
				if(resultCode == CW_PROTOCOL_FAILURE_FIRM_WRT_ERROR)
				{	
					if(gtrapflag>=4)
					{
						#if NOSOFTAC
						wid_dbus_trap_wtp_ap_flash_write_failed(WTPIndex);
						#endif
					}
				}
				break;			
			}

 			default:
			
			result = 0;
			for(i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
			{
				if(gConfigVersionUpdateInfo[i] != NULL)
				{
					result = 1;
					break;
				}
			}
			if((result != 0)&&(find_in_wtp_list(WTPIndex) == CW_TRUE))
			{
				delete_wtp_list(WTPIndex);
				insert_uptfail_wtp_list(WTPIndex);	
				update_complete_check();
				
				AC_WTP[WTPIndex]->updatefailstate = 2;

				_CWCloseThread(WTPIndex);		

			}			
			wid_syslog_err("%s:%d wtp%d rcv messageType %d in imagedata state\n",
					        __func__, __LINE__, WTPIndex, controlVal.messageTypeValue);
			return CW_FALSE;
			/*no need break*/
 		}
	}
	else if (WTP_UPGRADE_MODE_NONE == upgrade_mode)
	{
		switch(controlVal.messageTypeValue)
		{
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
			{
				CWImageDataRequest valuesPtr;
				CWImageIdentifier resPtr;
				memset(&valuesPtr, 0, sizeof(CWImageDataRequest));
				memset(&resPtr, 0, sizeof(CWImageIdentifier));
				
				if(!(CWParseImageDataRequestMessage(msgPtr, controlVal.msgElemsLen, &valuesPtr)))
				{
					wid_syslog_warning("%s:%d wrong in CWParseImageDataRequestMessage",__func__, __LINE__);
					return CW_FALSE;		
				}
				
				if(gtrapflag >= 1)
				{
				#if NOSOFTAC
					wid_dbus_trap_wtp_enter_imagedata_state(WTPIndex);//wu:ap升级告警
				#endif
				}
							
				if((AC_WTP[WTPIndex]->updateversion != NULL)&&(AC_WTP[WTPIndex]->updatepath != NULL))
				{
					resPtr.Ver = AC_WTP[WTPIndex]->updatepath;
					resPtr.VerLen = strlen(AC_WTP[WTPIndex]->updatepath);
					resPtr.VerType = 0;
					resPtr.VerNum = 1;				
				}
				else
				{
					/*mahz modify code here to match ap upgrade batchlly*/
					CWConfigVersionInfo_new *tmpnode = gConfigVerInfo;
					CWBool bMatchVersion = CW_FALSE;
					
					if(find_in_wtp_list(WTPIndex) == CW_TRUE)
					{
						for(i=0;i<BATCH_UPGRADE_AP_NUM;i++)
						{
							CWConfigVersionInfo *update_node = gConfigVersionUpdateInfo[i];
							if((update_node != NULL)
								&&(update_node->str_ap_model != NULL)
								&&(0 == strcmp(update_node->str_ap_model,AC_WTP[WTPIndex]->WTPModel) ))
							{
								while(update_node != NULL)
								{
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wtp%d enter image data upgrade\n",__func__, __LINE__,WTPIndex);
									if(strcmp(update_node->str_ap_code,AC_WTP[WTPIndex]->APCode) == 0)
									{
										wid_syslog_debug_debug(WID_WTPINFO,
															"%s:%d wtp%d code: %s upgrade code: %s name: %s path: %s\n",
															__func__,__LINE__,WTPIndex,AC_WTP[WTPIndex]->APCode,update_node->str_ap_code,
                                                            update_node->str_ap_version_name,update_node->str_ap_version_path);

										resPtr.Ver = update_node->str_ap_version_path;
										resPtr.VerLen = strlen(update_node->str_ap_version_path);
										resPtr.VerType = 0;
										resPtr.VerNum = 1;
										bMatchVersion = CW_TRUE;

										if(AC_WTP[WTPIndex]->updateStat >= 1)
										{
											AC_WTP[WTPIndex]->updatefailcount++;	
										}

										if(AC_WTP[WTPIndex]->updatefailcount >= updatemaxfailcount)
										{
											delete_wtp_list(WTPIndex);
											
											insert_uptfail_wtp_list(WTPIndex);
											
											update_complete_check();						

											AC_WTP[WTPIndex]->updatefailstate = 1;

											_CWCloseThread(WTPIndex);
										}
										//printf("ACEnterImageData update\n");

										result = 1;
										break;
									}
									update_node = update_node->next;
								}
								if(bMatchVersion == CW_FALSE)	//it will not goto this branch
								{
									resPtr.Ver = valuesPtr.ImageRequest->Ver;
									resPtr.VerLen = valuesPtr.ImageRequest->VerLen;
									resPtr.VerType = 0;
									resPtr.VerNum = 1;
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d check image data ver response\n",__func__,__LINE__);
									/*if(!(CWCheckImageIdentifier(valuesPtr.ImageRequest, &resPtr)))
									{
										//printf("wrong in CWCheckImageIdentifier\n");
										wid_syslog_debug_debug(WID_WTPINFO,"wrong in CWCheckImageIdentifier\n");
										return CW_FALSE;
									}*/
								}
								break;
							}
						}
					}
					/*ap upgrade automatically here,the gloable variable gConfigVersionUpdateInfo may be NULL here,so use gConfigVerInfo instead*/
					/*the upgrade automatically can be used now,but after the upgrade is over,the wtpcompatible.xml is not updated,so what the cmd
					show model list shows is still old information, this situation should be modified sooner or later*/
					else if(img_now == 1)
					{
						while(tmpnode != NULL)
						{
							if(strcmp(tmpnode->str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0)
							{
								wid_syslog_debug_debug(WID_WTPINFO,"%s:%d find model %s in apimg.xml\n",__func__,__LINE__,tmpnode->str_ap_model);
								CWCodeInfo *codenode = tmpnode->code_info;
								while(codenode != NULL)
								{
									wid_syslog_debug_debug(WID_WTPINFO,"%s:%d match code %s operation\n",__func__,__LINE__,codenode);
									if(strcmp(codenode->str_ap_version_code,AC_WTP[WTPIndex]->APCode) == 0)
									{
										resPtr.Ver = codenode->str_ap_version_path;
										resPtr.VerLen = strlen(codenode->str_ap_version_path);
										resPtr.VerType = 0;
										resPtr.VerNum = 1;
										
										wid_syslog_debug_debug(WID_WTPINFO,"%s:%d ac surport version name:%s\n",__func__,__LINE__,codenode->str_ap_version_name);
										break;
									}
									codenode = codenode->next;
								}
								break;
							}
							tmpnode = tmpnode->next;
						}
					}
				}
				gWTPs[WTPIndex].responseSeqNum = controlVal.seqNum;
				if(!CWAssembleImageDataRequestMessage(&(gWTPs[WTPIndex].messages), 
													     &(gWTPs[WTPIndex].messagesCount),
													      gWTPs[WTPIndex].pathMTU, 
													      controlVal.seqNum, 
													      &resPtr,
													      WTPIndex))
				{
					/*valuesPtr.ImageRequest and its elements leak */
					CW_FREE_OBJECT(valuesPtr.ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr.ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr.ImageRequest);
					return CW_FALSE;
				}
				gWTPs[WTPIndex].responseType = CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE;
				AC_WTP[WTPIndex]->updateStat = 1;
		
				if(gtrapflag >= 4)
				{
				#if NOSOFTAC
					wid_dbus_trap_wtp_tranfer_file(WTPIndex);
				#endif	
				}
				
				if((valuesPtr.ImageRequest)&&(valuesPtr.ImageRequest->Ver))
				{
    				AC_WTP[WTPIndex]->ApReportVer = malloc(strlen(valuesPtr.ImageRequest->Ver)+1);
    				memset(AC_WTP[WTPIndex]->ApReportVer,0,strlen(valuesPtr.ImageRequest->Ver)+1);
    				memcpy(AC_WTP[WTPIndex]->ApReportVer,valuesPtr.ImageRequest->Ver,strlen(valuesPtr.ImageRequest->Ver));
				}

				CW_FREE_OBJECT(valuesPtr.ImageRequest->model);
				CW_FREE_OBJECT(valuesPtr.ImageRequest->Ver);
				CW_FREE_OBJECT(valuesPtr.ImageRequest);
				CW_FREE_OBJECT(valuesPtr.EncryptType);
				break;
			}
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE:
			{
				wid_syslog_debug_debug(WID_WTPINFO,"CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE");
				CWProtocolResultCode resultCode;			
				CWImageIdentifier resPtr;
				memset(&resultCode, 0, sizeof(CWProtocolResultCode));
				memset(&resPtr, 0, sizeof(CWImageIdentifier));
	//			CWThreadMutexLock(&(gAllThreadMutex));		
				//ap doesn't use the value of resPtr in Reset state
				if(AC_WTP[WTPIndex]->ver != NULL)
				{
					resPtr.Ver = AC_WTP[WTPIndex]->ver;
				}
				else
				{
					resPtr.Ver = " ";
				}
				//resPtr.Ver = gConfigVersionInfo->str_ap_version_path;
				resPtr.VerNum = 1;
				resPtr.VerType = 0;
				resPtr.VerLen = strlen(resPtr.Ver);

				if(!(CWParseImageDataResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode)))
				{
					//CWThreadMutexUnlock(&(gAllThreadMutex));		
					return CW_FALSE;
				}
				gWTPs[WTPIndex].responseSeqNum = controlVal.seqNum;
				if(!CWAssembleResetRequestMessage(&(gWTPs[WTPIndex].messages), 
												     &(gWTPs[WTPIndex].messagesCount),
												     gWTPs[WTPIndex].pathMTU, controlVal.seqNum,
												     &resPtr, WTPIndex))
				{
					
					wid_syslog_debug_debug(WID_WTPINFO,"%s:%d wrong in CWAssembleResetRequestMessage",__func__,__LINE__);
					//CWThreadMutexUnlock(&(gAllThreadMutex));		
					return CW_FALSE;
				}
				
				//CWThreadMutexUnlock(&(gAllThreadMutex));		
				if(resultCode == CW_PROTOCOL_FAILURE_FIRM_WRT_ERROR)
				{	
					if(gtrapflag>=4)
					{
						#if NOSOFTAC
						wid_dbus_trap_wtp_ap_flash_write_failed(WTPIndex);
						#endif
					}
				}
				
				time(&AC_WTP[WTPIndex]->imagedata_time);
				gWTPs[WTPIndex].responseType = CW_MSG_TYPE_VALUE_RESET_RESPONSE;
				gWTPs[WTPIndex].currentState = CW_ENTER_RESET;
				AC_WTP[WTPIndex]->WTPStat = WID_RESET;
				AC_WTP[WTPIndex]->updateStat = 2;
				break;			
			}
			default:
				//printf("controlVal.messageTypeValue %d\n",controlVal.messageTypeValue);
				result = 0;
				for(i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
				{
					if(gConfigVersionUpdateInfo[i] != NULL)
					{
						result = 1;
						break;
					}
				}
				if((result != 0)&&(find_in_wtp_list(WTPIndex) == CW_TRUE))
				{
					delete_wtp_list(WTPIndex);
					
					insert_uptfail_wtp_list(WTPIndex);
					
					update_complete_check();
					
					AC_WTP[WTPIndex]->updatefailstate = 2;

					_CWCloseThread(WTPIndex);		

				}			
				wid_syslog_debug_debug(WID_WTPINFO,"controlVal.messageTypeValue %d",controlVal.messageTypeValue);
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				// no need break;
		}
		
		if(!CWACSendFragments(WTPIndex))
		{
			/*gWTPs[WTPIndex].messages leak*/
			for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
			{
				CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
			}
			CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
			
			return CW_FALSE;
		}
		
		wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter image-data send msg %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_RESET_REQUEST)); 
		if(!CWErr(CWTimerRequest(gCWImageDataPendingTimer, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex))) 
		{
			// start Change State Pending timer
			_CWCloseThread(WTPIndex);
		}
		
		for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++) 
		{
			CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
		}
		CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
	}

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter image data next state %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_STATE_STR(gWTPs[WTPIndex].currentState));
	
	return CW_TRUE;
}


int wid_wtp_upgrade(unsigned int wtpid)
{
	int wtpindex = wtpid;
	IMG_UPDATE type = IMGDATA_DATA;
	int datasize = MAX_DATE_SIZE;
	CWBool IGNORE_UBSV bResult = CW_FALSE;
//	int DATA_SIZE = MAX_DATE_SIZE;
	char buf[MAX_DATE_SIZE] = {0};
	char path[256] = {0};
	int ssize = 0;
	CWImageDateRequestDate ImageData;
	FILE *fp = NULL;

	if ((wtpid >= WTP_NUM) || (NULL == AC_WTP[wtpid]))
	{
		wid_syslog_err("%s:%d max wtp%d not exist\n", __func__, wtpid);
		return -1;
	}

	snprintf(path, sizeof(path), "/mnt/wtp/%s", AC_WTP[wtpid]->upgrade.ImageName);

	if (NULL == AC_WTP[wtpid]->upgrade.fp)
	{
		fp = fopen(path, "r");
		if (fp == NULL)
		{		
			wid_syslog_err("%s fopen %s failed %s\n",__func__,path, strerror(errno));
			return -1;
		}
		AC_WTP[wtpid]->upgrade.fp = fp;
	}
	else
	{
		fp = AC_WTP[wtpid]->upgrade.fp;
	}

	memset(buf, 0, sizeof(buf));
	ssize = fread(buf, 1, datasize, fp);
	/* TODO */
	if (0 != ssize)
	{
	//	gWTPs[wtpindex].IMG.type = IMGDATA_DATA;
		type = IMGDATA_DATA;

		AC_WTP[wtpid]->upgrade.pktcnt++;

		wid_syslog_debug("wtp%d send imagedata request %d times\n",
				wtpindex, AC_WTP[wtpid]->upgrade.pktcnt);			
		
		if ((datasize != ssize) && (feof(fp)))
		{
			type = IMGDATA_EOF;
			fclose(fp);
			AC_WTP[wtpid]->upgrade.fp = NULL;	
			wid_syslog_debug("wtp%d image upgrade end\n", wtpindex);
		}	
	}
	
	memset(&ImageData, 0, sizeof(ImageData));
	ImageData.ImageData = buf;
	ImageData.ImageDataLen = ssize;
	ImageData.ImageDateType = type;

//	int seqNum = CWGetSeqNum();
	int seqNum = CWGetWtpSeqNum(wtpindex);
	//int seqNum = CWGetSeqNumWtp(wtpindex);

//	wid_syslog_debug_debug(WID_WTPINFO,"%s,%d,wtp%d seqNum=%d\n",__func__,__LINE__,wtpindex,seqNum);
	if(CWAssembleImageDateRequest(&(gWTPs[wtpindex].messages), 
			&(gWTPs[wtpindex].messagesCount), 
			gWTPs[wtpindex].pathMTU, 
			seqNum, &ImageData, wtpindex))
	{
		if(CWACSendAcknowledgedPacket(wtpindex, CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE, seqNum)) 
		{
			bResult = CW_TRUE;
		}
		else
		{
			CWACStopRetransmission(wtpindex);
		}
	}

	if (IMGDATA_EOF == type)
	{
		AC_WTP[wtpindex]->upgrade.state = WTP_UPGRADE_STATE_IMAGEEND;
	}
	else
	{
		AC_WTP[wtpindex]->upgrade.state = WTP_UPGRADE_STATE_IMAGEING;
	}

	AC_WTP[wtpindex]->updateStat = 1;

	return 0;
}


CWBool CWAssembleImageDataElem(CWProtocolMessage *msgPtr, CWImageDateRequestDate *ImageData) 
{
	 int headerLength = 2;
	 int messageBodyLength = 0;
	 int totalMessageLength = 0;

	if(msgPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	messageBodyLength = ImageData->ImageDataLen;
	totalMessageLength = headerLength + messageBodyLength;
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, totalMessageLength, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, ImageData->ImageDateType);
	CWProtocolStoreRawBytes(msgPtr, ImageData->ImageData, ImageData->ImageDataLen);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_DATA_CW_TYPE);
}




CWBool CWProtocolAssembleImageDataRequest(CWProtocolMessage **msgElems, 
				int *msgElemCountPtr, CWImageDateRequestDate *ImageData)
{
	int k = -1;
	
	if(msgElems == NULL || msgElemCountPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	*msgElemCountPtr = 1;

//	wid_syslog_debug_debug(WID_DEFAULT,"Assembling Protocol Configuration Update Request...\n");

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(*msgElems, *msgElemCountPtr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// Assemble Message Elements
	if (!(CWAssembleImageDataElem(&(*msgElems[++k]), ImageData)))
	{
		int i;
		for(i = 0; i <= k; i++) {CW_FREE_PROTOCOL_MESSAGE(*msgElems[i]);}
		CW_FREE_OBJECT(*msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

//	wid_syslog_debug_debug(WID_DEFAULT,"Protocol Configuration Update Request Assembled\n");
	
	return CW_TRUE;
}


CWBool CWAssembleImageDateRequest
(
	CWProtocolMessage **messagesPtr,
    int *fragmentsNumPtr,
    int PMTU,
	int seqNum,
	CWImageDateRequestDate *ImageData,
	int WTPIndex) 
{

	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=0;
	
	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if (!CWProtocolAssembleImageDataRequest(&msgElems, &msgElemCount, ImageData)) 
	{
		return CW_FALSE;
	}
	
	if(!(CWAssembleMessage(messagesPtr,
							fragmentsNumPtr,
							PMTU,
							seqNum,
							CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST,
							msgElems,
							msgElemCount,
							msgElemsBinding,
							msgElemBindingCount,
							CW_PACKET_CRYPT,
							AC_WTP[WTPIndex]->cap_pro_ver)))
	{
		return CW_FALSE;
	}
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n",  \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
				WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST), seqNum, msgElemBindingCount);
	return CW_TRUE;
}

CWBool CWAssembleMsgElemImageInfor(CWProtocolMessage *msgPtr, CWImageInformationHash *resPtr)
{
	int size = 0;
	size += 4*5;	

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CWProtocolStore32(msgPtr, resPtr->filesize);	//sizeof imagefile
	CWProtocolStoreRawBytes(msgPtr, (char*)resPtr->hash, WID_MD5_LEN);	//hash
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE);
}




CWBool CWParseImageDataRequestMessage_cw
(
	CWProtocolMessage* msgPtr,
	int len,
	CWImageDataRequest *valuesPtr
)
{
	int offsetTillMessages;
	if(msgPtr == NULL || valuesPtr==NULL) 
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if((msgPtr->msg) == NULL) 
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	offsetTillMessages = msgPtr->offset;
	
//	wid_syslog_debug_debug(WID_WTPINFO,"Parsing Image data request...");
	valuesPtr->ImageRequest = NULL;
	
	/* parse message elements*/
	while((msgPtr->offset - offsetTillMessages) < len) 
	{
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) 
		{
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:

				/* TODO packet prase */
				if(valuesPtr->ImageRequest)
				{
					CW_FREE_OBJECT(valuesPtr->ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr->ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr->ImageRequest);
				}
				
				CW_CREATE_OBJECT_ERR(valuesPtr->ImageRequest, 
										      CWImageIdentifier,
										      return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				memset(valuesPtr->ImageRequest, 0, sizeof(CWImageIdentifier));
				
				if (!(CWParseMsgElemImageIdentifier_cw(msgPtr, elemLen, valuesPtr->ImageRequest)))
				{
					wid_syslog_debug_debug(WID_WTPINFO,"wrong in CWParseMsgElemImageIdentifier\n");
					/*free valuesPtr->ImageRequest*/
					CW_FREE_OBJECT(valuesPtr->ImageRequest->Ver);
					CW_FREE_OBJECT(valuesPtr->ImageRequest->model);
					CW_FREE_OBJECT(valuesPtr->ImageRequest);
					return CW_FALSE;
				}
				
				break;	

			case CW_MSG_ELEMENT_INITIATED_DOWNLOAD_CW_TYPE:
				msgPtr->offset += elemLen;
				wid_syslog_info("%s,%d,elemType=%d,elemLen=%d.",__func__,__LINE__,elemType,elemLen);
				break;	
				
			default:				
				wid_syslog_debug_debug(WID_WTPINFO,"wrong in default\n");
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Image data request");				
				/*no need break;*/	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len)
	{ 
		wid_syslog_debug_debug(WID_WTPINFO,"((msgPtr->offset - offsetTillMessages) != len)");
		/*free valuesPtr->ImageRequest and its elements */
		if(valuesPtr->ImageRequest)
		{ /* CID:10468: null pointer dereference */
			CW_FREE_OBJECT(valuesPtr->ImageRequest->Ver);
			CW_FREE_OBJECT(valuesPtr->ImageRequest->model);
			CW_FREE_OBJECT(valuesPtr->ImageRequest);
		}
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
//	wid_syslog_debug_debug(WID_WTPINFO,"Image data request Parsed");
	return CW_TRUE;	
}


CWBool CWParseMsgElemImageIdentifier_cw
(
	CWProtocolMessage *msgPtr,
	int len, 
	CWImageIdentifier *valPtr
) 
{
	wid_syslog_debug_debug(WID_WTPINFO,"CWParseMsgElemImageIdentifier, len : %d\n", len);
	CWParseMessageElementStart();
	int initOffSet = msgPtr->offset;
	unsigned short type = 0;

	valPtr->vendorid = CWProtocolRetrieve32(msgPtr);
	
	if (CW_MSG_ELEMENT_VENDOR_IDENTIFIER != valPtr->vendorid)
	{
		return CW_FALSE;
	}
	
	while((msgPtr->offset - initOffSet )<(len-4))
	{
		type = CWProtocolRetrieve16(msgPtr);

		if (CW_IMAGEIDENTIFIER_VERSION == type)
		{
			valPtr->version_len = CWProtocolRetrieve16(msgPtr);
			if (valPtr->version_len >= WTP_VERSION_LEN)
			{
				return CW_FALSE;
			}
			CWProtocolGetRawBytes(msgPtr, (char *)valPtr->version, valPtr->version_len);
			wid_syslog_debug_debug(WID_WTPINFO,"%s: version is %s len is %d\n", __func__, valPtr->version, valPtr->version_len);
		}
		else if (CW_IMAGEIDENTIFIER_IMAGENAME == type)
		{
			valPtr->version_filename_len = CWProtocolRetrieve16(msgPtr);
			if (valPtr->version_filename_len >= WTP_IMAGENAME_LEN)
			{
				return CW_FALSE;
			}
			CWProtocolGetRawBytes(msgPtr, (char *)valPtr->version_filename, valPtr->version_filename_len);
			wid_syslog_debug_debug(WID_WTPINFO,"%s: version_filename is %s len is %d\n", __func__, valPtr->version_filename, valPtr->version_filename_len);
		}
		else if (CW_IMAGEIDENTIFIER_MODE == type)
		{
			valPtr->version_model_len = CWProtocolRetrieve16(msgPtr);
			if (valPtr->version_model_len >= WTP_MODE_LEN)
			{
				return CW_FALSE;
			}
			CWProtocolGetRawBytes(msgPtr, (char *)valPtr->version_model, valPtr->version_model_len);
			wid_syslog_debug_debug(WID_WTPINFO,"%s: model is %s len is %d\n", __func__, valPtr->version_model, valPtr->version_model_len);
		}
		else
		{
			wid_syslog_err("<err> %s,%d,error type %d\n",__func__,__LINE__, type);
			return CW_FALSE;
		}
	}

	CWParseMessageElementEnd();
}

CWBool CWAssembleImageDataRequestMessage_new2(CWProtocolMessage **messagesPtr, 
	int *fragmentsNumPtr, int PMTU, int seqNum, CWImageIdentifier *resPtr, int WTPIndex)
{
	
		CWProtocolMessage *msgElems = NULL;
		const int MsgElemCount=1;
		CWProtocolMessage *msgElemsBinding = NULL;
		int msgElemBindingCount=0;
		int k = -1;
		
		if(messagesPtr == NULL || fragmentsNumPtr == NULL)
			return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		// Assemble Message Elements
		if ((!(CWAssembleMsgElemImageIdentifierAC(&(msgElems[++k]), resPtr)))){
			int i;
			for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
		
		if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST, msgElems, MsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver))) 
		{
			return CW_FALSE;
		}
		
		wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u with %u elements\n",  \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST), seqNum, MsgElemCount);
		return CW_TRUE;
}


CWBool CWAssembleMsgElemImageInformationAC(CWProtocolMessage *msgPtr, 
	CWImageInformationHash *resPtr)
{
	int size = 0;
	size += 4*5;	
	//wid_syslog_info("%s,%d.\n",__func__,__LINE__);
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CWProtocolStore32(msgPtr, resPtr->filesize);	//sizeof imagefile
	CWProtocolStoreRawBytes(msgPtr, (char *)resPtr->hash, WID_MD5_LEN);	//hash
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE);
}




CWBool CWAssembleImageDataResponseMessage
(
	CWProtocolMessage **messagesPtr,
	int *fragmentsNumPtr,
	int PMTU,
	int seqNum, 
	CWProtocolImageDataResponseValues *ImageDateRespVal,
	int WTPIndex
)
{

	CWProtocolMessage *msgElems = NULL;
	const int MsgElemCount=2;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	int k = -1;
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if ((!(CWAssembleMsgElemResultCode(&(msgElems[++k]), CW_PROTOCOL_SUCCESS)))||
		(!(CWAssembleMsgElemImageInfor(&(msgElems[++k]), &(ImageDateRespVal->ImageInfoHash)))))
	{
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
			
	
	if(!(CWAssembleMessage(messagesPtr,
							fragmentsNumPtr,
							PMTU,
							seqNum,
							CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE,
							msgElems,
							MsgElemCount,
							msgElemsBinding,
							msgElemBindingCount,
							CW_PACKET_CRYPT,
							AC_WTP[WTPIndex]->cap_pro_ver))) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n",  \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
				WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE), seqNum, AC_WTP[WTPIndex]->cap_pro_ver, MsgElemCount);
	return CW_TRUE;
}


int wid_get_upgrade_info(unsigned int wtpid, WIDUpgradeInfo *UpgradeInfo)
{
	int i=0, IGNORE_UBSV result=0;

	WID_CHECK_POINTER_RET(UpgradeInfo, -1);
	WID_CHECK_WTP_STANDARD_RET(wtpid,-1);
	
	if((AC_WTP[wtpid]->updateversion != NULL)&&(AC_WTP[wtpid]->updatepath != NULL))
	{
		UpgradeInfo->NeedUpgrade = CW_TRUE;
		strncpy(UpgradeInfo->ImageName, AC_WTP[wtpid]->updatepath, WTP_IMAGENAME_LEN-1);
	}
	else
	{
		/*mahz modify code here to match ap upgrade batchlly*/
		CWConfigVersionInfo_new *tmpnode = gConfigVerInfo;
		CWBool IGNORE_UBSV bMatchVersion = CW_FALSE;
		
		if(find_in_wtp_list(wtpid) == CW_TRUE)
		{
			for(i=0;i<BATCH_UPGRADE_AP_NUM;i++)
			{
				CWConfigVersionInfo *update_node = gConfigVersionUpdateInfo[i];
				if((update_node != NULL)
					&&(update_node->str_ap_model != NULL)
					&&(strcmp(update_node->str_ap_model,AC_WTP[wtpid]->WTPModel) == 0))
				{
					while(update_node != NULL)
					{
						wid_syslog_debug_debug(WID_WTPINFO,"*** enter image data upgrade ***\n");	//for test
						if(strcmp(update_node->str_ap_code, AC_WTP[wtpid]->APCode) == 0)
						{
							wid_syslog_debug_debug(WID_WTPINFO,"***111 upgrade node is not null***\n"); //for test
							wid_syslog_debug_debug(WID_WTPINFO,"**111 ap code: %s **\n",AC_WTP[wtpid]->APCode); //for test
							wid_syslog_debug_debug(WID_WTPINFO,"**111 upgrade node code: %s **\n",update_node->str_ap_code);	//for test
							wid_syslog_debug_debug(WID_WTPINFO,"**111 upgrade node name: %s **\n",update_node->str_ap_version_name);	//for test
							wid_syslog_debug_debug(WID_WTPINFO,"**111 upgrade node path: %s **\n",update_node->str_ap_version_path);	//for test

	
							UpgradeInfo->NeedUpgrade = CW_TRUE;
							strncpy(UpgradeInfo->ImageName, update_node->str_ap_version_path, WTP_IMAGENAME_LEN-1);
							
							bMatchVersion = CW_TRUE;
	
							if(AC_WTP[wtpid]->updateStat >= 1)
							{
								AC_WTP[wtpid]->updatefailcount++;	
							}
	
							if(AC_WTP[wtpid]->updatefailcount >= updatemaxfailcount)
							{
								delete_wtp_list(wtpid);
								insert_uptfail_wtp_list(wtpid);
								
								update_complete_check();						
	
								AC_WTP[wtpid]->updatefailstate = 1;
	
								_CWCloseThread(wtpid);
							}
							//printf("ACEnterImageData update\n");
	
							result = 1;
							break;
						}
						update_node = update_node->next;
					}
					#if 0
					if(bMatchVersion == CW_FALSE)	//it will not goto this branch
					{
						resPtr.Ver = valuesPtr.ImageRequest->Ver;
						resPtr.VerLen = valuesPtr.ImageRequest->VerLen;
						resPtr.VerType = 0;
						resPtr.VerNum = 1;
						wid_syslog_debug_debug(WID_WTPINFO,"check image data ver response\n");	//for test
						/*if(!(CWCheckImageIdentifier(valuesPtr.ImageRequest, &resPtr))){
							//printf("wrong in CWCheckImageIdentifier\n");
							wid_syslog_debug_debug(WID_WTPINFO,"wrong in CWCheckImageIdentifier\n");
							return CW_FALSE;
						}*/
					}
					#endif
					break;
				}
			}
		}
		/*ap upgrade automatically here,the gloable variable gConfigVersionUpdateInfo may be NULL here,so use gConfigVerInfo instead*/
		/*the upgrade automatically can be used now,but after the upgrade is over,the wtpcompatible.xml is not updated,so what the cmd
		show model list shows is still old information, this situation should be modified sooner or later*/
		else if (img_now == 1)
		{
			while(tmpnode != NULL)
			{
				if(strcmp(tmpnode->str_ap_model,AC_WTP[wtpid]->WTPModel) == 0)
				{
					wid_syslog_debug_debug(WID_WTPINFO,"**** find model in apimg.xml ****\n");	//for test
					CWCodeInfo *codenode = tmpnode->code_info;
					while(codenode != NULL)
					{
						wid_syslog_debug_debug(WID_WTPINFO,"**** match code operation ****\n"); //for test
						if (strcmp(codenode->str_ap_version_code,AC_WTP[wtpid]->APCode) == 0)
						{
						
							UpgradeInfo->NeedUpgrade = CW_TRUE;
							strncpy(UpgradeInfo->ImageName, codenode->str_ap_version_path, WTP_IMAGENAME_LEN-1);
							
							
							wid_syslog_debug_debug(WID_WTPINFO,"ac surport version name:%s\n",codenode->str_ap_version_name);
							break;
						}
						codenode = codenode->next;
					}
					break;
				}
				tmpnode = tmpnode->next;
			}
		}
	}

return 0;
}
