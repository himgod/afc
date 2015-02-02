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
* ACDiscoveryState.c
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

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */

__inline__ int CWACGetHWVersion();
__inline__ int CWACGetSWVersion();
__inline__ int CWACGetStations();
__inline__ int CWACGetLimit();
__inline__ int CWACGetActiveWTPs();
__inline__ int CWACGetMaxWTPs();
__inline__ int CWACGetSecurity();

__inline__ char * CWACGetName();

__inline__ int CWACGetInterfacesCount();
__inline__ int  CWACGetInterfacesIpv4Count(void);
__inline__ int  CWACGetInterfacesIpv6Count(void);

/*_________________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */

// send Discovery Response to the host at the specified address
CWBool CWAssembleDiscoveryResponse(CWProtocolMessage **messagesPtr, int seqNum, unsigned int WTPID, int cap_pro_ver)
{
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 4;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount = 0;
	int fragmentsNum = 0;
	int k = -1;
	if(messagesPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if(CWACSupportIPv6())
	{
		msgElemCount++;
	}
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemACDescriptor(&(msgElems[++k]))))
	    || (!(CWAssembleMsgElemACName(&(msgElems[++k]))))
		|| ((!(CWAssembleMsgElemCWControlIPv4Addresses(&(msgElems[++k]),WTPID)))
		    && (CWACSupportIPv6() && (!(CWAssembleMsgElemCWControlIPv6Addresses(&(msgElems[++k]))))))
		|| (!(CWAssembleMsgElemWTPContainMAC(&(msgElems[++k])))))
	{
		CWErrorHandleLast();
		int i = 0;
		for(i = 0; i <= k; i++)
		{
		    CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!CWAssembleMessage(messagesPtr, &fragmentsNum, 0, seqNum, CW_MSG_TYPE_VALUE_DISCOVERY_RESPONSE, \
							msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_PLAIN, cap_pro_ver)) 
	{
		return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT""WAI_DISCOVERY_LITERAL"assemble %s msg seq %u capprover %u with %u elements\n", \
				WAI_FUNC_LINE_LITERAL, WTPID,  \
				WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_DISCOVERY_RESPONSE), seqNum, cap_pro_ver, msgElemCount);
	
	return CW_TRUE;
}

CWBool CWGetMACFromDiscovery(char *msg, CWProtocolHeaderInfo *header_info) 
{
	int n = 0;
	int offsetTillMessages = 0;
	CWBool dataFlag = CW_FALSE;
	unsigned short int elemid = 0;
	unsigned short int elemlen = 0;
	CWDiscoveryRequestValues value;
	CWControlHeaderValues controlVal;
	CWProtocolTransportHeaderValues transportVal;
	CWProtocolMessage completeMsg;

	
	if(msg == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}

	memset(&value, 0, sizeof(CWDiscoveryRequestValues));
	memset(&controlVal, 0, sizeof(CWControlHeaderValues));
	memset(&transportVal, 0, sizeof(CWProtocolTransportHeaderValues));
	memset(&completeMsg, 0, sizeof(CWProtocolMessage));
		
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseTransportHeader(&completeMsg, &transportVal, &dataFlag)))
	{
		return CW_FALSE;	// will be handled by the caller
	}
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
	{
		return CW_FALSE;	// will be handled by the caller
	}
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST)
	{
	    wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"msg(%u) %s is not discovery request when try parse\n", \
						WAI_FUNC_LINE_LITERAL, 0,controlVal.messageTypeValue,  \
						WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST));
	    return CW_FALSE;
	}
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
		
	// parse message elements
	while ((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) 
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);

		switch(elemType)
		{
			case CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE:
			{
				if(!(CWParseDiscoveryType(&completeMsg, elemLen, &value)))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
			{
				if(!(CWParseWTPBoardData(&completeMsg, elemLen, &(value.WTPBoardData))))    // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
			{
				if(!(CWParseWTPDescriptor(&completeMsg, elemLen, &(value.WTPDescriptor))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
			{
				if(!(CWParseWTPFrameTunnelMode(&completeMsg, elemLen, &(value.frameTunnelMode))))// will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
			{
				if(!(CWParseWTPMACType(&completeMsg, elemLen, &(value.MACType))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			{
				CWParseFormatMsgElem(&completeMsg, &elemid, &elemlen);
				if(elemlen > controlVal.msgElemsLen)
				{
					wid_syslog_err("%s elemType %d elemlen:%d > controlVal.msgElemsLen:%d\n", __func__, elemType, elemlen, controlVal.msgElemsLen);
					return CW_TRUE;
				}
				if(CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE == elemid)
				{
					if(!(CWParseWTPLOGINType(&completeMsg, elemlen, &(value.Connect_status))))   // will be handled by the caller
					{
						return CW_FALSE;
					}
					wid_syslog_info("%s:%d spec_payload:Connect_status %d\n", __func__, __LINE__, value.Connect_status);
				}
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,"Unrecognized Message Element");
		}
	}

	if(0 == header_info->mac_exist)
	{
		for(n = 0; n < value.WTPBoardData.vendorInfosCount; n++)
		{
			if((value.WTPBoardData.vendorInfos)[n].type == CW_BOARD_MAC_ADDRESS)
			{
				memcpy(header_info->wtp_mac, (value.WTPBoardData.vendorInfos)[n].mac, MAC_LEN);
				break;
			}
		}
		header_info->mac_exist = 1;
	}
	
	header_info->wtp_contype = value.Connect_status;
	
	CWDestroyDiscoveryRequestValues(&value);
		
	return CW_TRUE;
}


CWBool CWParseDiscoveryRequestMessage(char *msg, int len, int *seqNumPtr, CWDiscoveryRequestValues *valuesPtr)
{
	CWControlHeaderValues controlVal;
	CWProtocolTransportHeaderValues transportVal;
	int offsetTillMessages = 0;
	CWProtocolMessage completeMsg;
	CWBool dataFlag = CW_FALSE;

	if (msg == NULL || seqNumPtr == NULL || valuesPtr == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if (!(CWParseTransportHeader(&completeMsg, &transportVal, &dataFlag)))
	{
		return CW_FALSE; // will be handled by the caller
	}	
	if (!(CWParseControlHeader(&completeMsg, &controlVal)))
	{
		return CW_FALSE; // will be handled by the caller
	}
	
	if (controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST)
	{
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Discovery Request as Expected");
	}
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"start parse discovery request seq %u\n", \
				WAI_FUNC_LINE_LITERAL, 0, controlVal.seqNum);

	*seqNumPtr = controlVal.seqNum;
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
	
	//(*valuesPtr).radios.radiosCount = 0;
	
	while ((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		unsigned short int subelemid = 0;
		unsigned short int subelemlen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

//		wid_syslog_debug_debug("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
									
		switch(elemType)
		{
			case CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE:
			{
				if (!(CWParseDiscoveryType(&completeMsg, elemLen, valuesPtr))) return CW_FALSE; // will be handled by the caller
				break;
			}
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
			{
				if (!(CWParseWTPBoardData(&completeMsg, elemLen, &(valuesPtr->WTPBoardData)))) return CW_FALSE; // will be handled by the caller
				break; 
			}
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
			{
				if (!(CWParseWTPDescriptor(&completeMsg, elemLen, &(valuesPtr->WTPDescriptor)))) return CW_FALSE; // will be handled by the caller
				break;
			}
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
			{
				if (!(CWParseWTPFrameTunnelMode(&completeMsg, elemLen, &(valuesPtr->frameTunnelMode)))) return CW_FALSE; // will be handled by the caller
				break;
			}
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
			{
				if (!(CWParseWTPMACType(&completeMsg, elemLen, &(valuesPtr->MACType)))) return CW_FALSE; // will be handled by the caller
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			{
				CWParseFormatMsgElem(&completeMsg, &subelemid, &subelemlen);
				if (subelemlen > controlVal.msgElemsLen)
				{
					wid_syslog_err("%s elemType %d elemlen:%d > controlVal.msgElemsLen:%d\n", __func__, elemType, subelemlen, controlVal.msgElemsLen);
					return CW_TRUE;
				}
				if (CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE == subelemid)
				{
					if (!(CWParseWTPLOGINType(&completeMsg, subelemlen, &(valuesPtr->Connect_status))))   // will be handled by the caller
					{
						return CW_FALSE;
					}
					
					wid_syslog_info("%s:%d spec_payload:Connect_status %d \n", __func__, __LINE__, valuesPtr->Connect_status);
				}
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,"Unrecognized Message Element");
		}
	}
	
	if (completeMsg.offset != len)
	{
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	
	return CW_TRUE;
}

void CWDestroyDiscoveryRequestValues(CWDiscoveryRequestValues *valPtr)
{
	int i = 0;
	
	if(valPtr == NULL) return;
	
	for(i = 0; i < (valPtr->WTPDescriptor.vendorInfos).vendorInfosCount; i++)
	{
		if((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_HARDWARE_VERSION)
		{
			CW_FREE_OBJECT(((valPtr->WTPDescriptor.vendorInfos).vendorInfos)[i].sysver);
		}
		else if((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
		{
			CW_FREE_OBJECT(((valPtr->WTPDescriptor.vendorInfos).vendorInfos)[i].ver);
		}
		else if((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].type == CW_BOOT_VERSION)
		{
			CW_FREE_OBJECT(valPtr->WTPDescriptor.vendorInfos.vendorInfos[i].bootver);
		}
		else
		{
			CW_FREE_OBJECT((valPtr->WTPDescriptor.vendorInfos.vendorInfos)[i].valuePtr);
		}
	}
	
	CW_FREE_OBJECT((valPtr->WTPDescriptor.vendorInfos).vendorInfos);

	for(i = 0; i < valPtr->WTPBoardData.vendorInfosCount; i++)
	{
		if((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].model);
		}
		else if((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].SN);
		}
		else if((valPtr->WTPBoardData.vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].mac);
		}
		else if((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].Rmodel);
		}
		else if((valPtr->WTPBoardData.vendorInfos)[i].type == CW_WTP_CODE_VERSION)
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].codever);
		}
		else
		{
			CW_FREE_OBJECT((valPtr->WTPBoardData.vendorInfos)[i].valuePtr);
		}
	}
	
	CW_FREE_OBJECT(valPtr->WTPBoardData.vendorInfos);
	
	//CW_FREE_OBJECT((valPtr->radios).radios);
}
