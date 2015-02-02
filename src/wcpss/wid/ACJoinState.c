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
* ACJoinState.c
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
#include "CWCommon.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

CWBool CWAssembleJoinResponse
(
	CWProtocolMessage **messagesPtr,
	int *fragmentsNumPtr,
	int PMTU,
	int seqNum,
	CWList msgElemList,
	char *strVersion,
	CWProtocolJoinResponseValues *joinResponseVal,
	enum wtp_upgrade_mode upgrade_mode,
	int WTPIndex,
	WTPLoginState WTP_LOGIN
);
CWBool CWParseJoinRequestMessage(char *msg, int len, int *seqNumPtr, CWProtocolJoinRequestValues *valuesPtr, int WTPIndex, WTPLoginState *WTP_LOGIN);
CWBool CWSaveJoinRequestMessage(CWProtocolJoinRequestValues *joinRequest, CWWTPProtocolManager *WTPProtocolManager,unsigned int WTPIndex);
CWBool CWAssembleMsgElemImageIdentifier(CWProtocolMessage *msgPtr, CWImageIdentifier_cw *ImageIdentifier);
int wid_md5_hash(char *path, unsigned char *md5);
extern CWConfigVersionXml SOFT_XMLINFO[CW_MODEL_COUNT];


CWBool ACEnterJoin(int WTPIndex, CWProtocolMessage *msgPtr)
{
	int ret = 0;
	int seqNum = 0;
	WTPLoginState WTP_LOGIN = 0;
	CWProtocolJoinRequestValues joinRequest;
	CWList msgElemList = NULL;

	WID_CHECK_WTP_STANDARD_RET(WTPIndex, CW_FALSE);
	
	if (msgPtr == NULL) 
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	memset(&joinRequest, 0, sizeof(CWProtocolJoinRequestValues));
	if (!(CWParseJoinRequestMessage(msgPtr->msg, msgPtr->offset, &seqNum, &joinRequest, WTPIndex, &WTP_LOGIN)))
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"parse %s request error\n",  \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME,
					(WID_LOGIN == WTP_LOGIN) ? "join" : "rejoin");
		return CW_FALSE;
		// note: we can kill our thread in case of out-of-memory error to free some space/
		// we can see this just calling CWErrorGetLastErrorCode()
	}
#if 0
	if(*gcurrent_wtp_count[gWTPs[WTPIndex].oemoption] > *gmax_wtp_count[gWTPs[WTPIndex].oemoption])
	{
		printf("###can not access ap type %d access count over type count\n",gWTPs[WTPIndex].oemoption);
		wid_syslog_debug_debug(WID_WTPINFO,"###can not access ap type %d access count over type count",gWTPs[WTPIndex].oemoption);	
		return CW_FALSE;
	}
#endif
	// cancel waitJoin timer
	if (!CWTimerCancel(&(gWTPs[WTPIndex].currentTimer),1))
	{
		return CW_FALSE;
	}
	if (find_in_wtp_list(WTPIndex) == CW_TRUE)
	{
		if (!CWTimerCancel(&(gWTPs[WTPIndex].updateTimer),1))
		{
			wid_syslog_info("CWTimerCancel error\n");	
			return CW_FALSE;
		}
	}
	enum wtp_upgrade_mode upgrade_mode = AC_WTP[WTPIndex]->upgrade.mode;

	CWBool ACIpv4List = CW_FALSE;
	CWBool ACIpv6List = CW_FALSE;
	CWBool resultCode = CW_TRUE;
	int resultCodeValue = CW_PROTOCOL_SUCCESS;
	CWMsgElemData *auxData = NULL;
	//CWBool sessionID = CW_FALSE;
/*	unsigned char *mac;	
	mac = (unsigned char*)malloc(6);
	memset(mac,0,6);
	if(!(CWCheckWTPBoardData(WTPIndex, &(joinRequest.WTPBoardData), mac))){
		gWTPs[WTPIndex].currentState = CW_QUIT;
		AC_WTP[WTPIndex]->WTPStat = 0;
		memset(AC_WTP[WTPIndex]->WTPIP, 0, 128);
		free(mac);
		mac = NULL;
		return CW_TRUE;
	}	
	memcpy(AC_WTP[WTPIndex]->WTPMAC, mac, 6);
	free(mac);
	mac = NULL;*/
	if (!(CWSaveJoinRequestMessage(&joinRequest,&(gWTPs[WTPIndex].WTPProtocolManager),WTPIndex))) 
	{
		resultCodeValue = CW_PROTOCOL_FAILURE_RES_DEPLETION;
	}
	
	if (ACIpv4List)
	{
		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
        auxData->type = CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE;
		auxData->value = 0;
		CWAddElementToList(&msgElemList,auxData);
	}
	if (ACIpv6List)
	{
		CW_FREE_OBJECT(auxData);
		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
        auxData->type = CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE;
        auxData->value = 0;
        CWAddElementToList(&msgElemList,auxData);
	}
	if (resultCode)
	{
		CW_FREE_OBJECT(auxData);
		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
        auxData->type =  CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE;
        auxData->value = resultCodeValue;
        CWAddElementToList(&msgElemList,auxData);
	}

	CWWTPDescriptor *valPtr = &gWTPs[WTPIndex].WTPProtocolManager.descriptor;	
	CWWTPVendorInfos *vendorPtr = &gWTPs[WTPIndex].WTPProtocolManager.WTPBoardData;
//  unsigned char *str_wtp_version = NULL;
	unsigned char *str_wtp_model = NULL;
	char *str_ac_version = NULL;		
	CWConfigVersionInfo_new *tmpnode = gConfigVerInfo;
	CWBool bMatchVersion = CW_FALSE;
	int i = 0, j = 0, len = 0, aclen = 0, lenmodel = 0, verlen = 0, IGNORE_UBSV result = 0;
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join has %u board data\n", \
						WAI_FUNC_LINE_LITERAL,  WTPIndex, AC_WTP[WTPIndex]->WTPNAME,vendorPtr->vendorInfosCount);
	for (i = 0; i < vendorPtr->vendorInfosCount; i++)
	{
		if ((vendorPtr->vendorInfos)[i].type == 0)//CW_WTP_MODEL_NUMBER = 0
		{
			str_wtp_model = (vendorPtr->vendorInfos)[i].model;
			lenmodel = (vendorPtr->vendorInfos)[i].length;
			
			if ((AC_WTP[WTPIndex])
			    && (AC_WTP[WTPIndex]->APCode)
			    && (strncmp(AC_WTP[WTPIndex]->APCode,(char *)str_wtp_model,lenmodel) != 0))
			{
				CW_FREE_OBJECT(AC_WTP[WTPIndex]->APCode);
				AC_WTP[WTPIndex]->APCode = (char *)malloc(lenmodel+1);
				memset(AC_WTP[WTPIndex]->APCode,0,lenmodel+1);
				memcpy(AC_WTP[WTPIndex]->APCode,str_wtp_model,lenmodel);
			}
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"board data %u: model %s\n", \
						WAI_FUNC_LINE_LITERAL,  WTPIndex,AC_WTP[WTPIndex]->WTPNAME, \
						i, (vendorPtr->vendorInfos)[i].model);
		}
		else if ((vendorPtr->vendorInfos)[i].type == 6)//CW_WTP_CODE_VERSION = 6
		{
			AC_WTP[WTPIndex]->codever = (char*)(vendorPtr->vendorInfos)[i].codever;
			len = (vendorPtr->vendorInfos)[i].length;
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"board data %u: code version %s\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME, \
						i, (vendorPtr->vendorInfos)[i].codever);
		}
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join has %u descriptors\n", \
				WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, valPtr->vendorInfos.vendorInfosCount);

	for (i = 0; i < valPtr->vendorInfos.vendorInfosCount; i++)
	{
		if ((valPtr->vendorInfos.vendorInfos)[i].type == 0)//CW_WTP_HARDWARE_VERSION = 0
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"descriptor %u: hw version %s\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME, \
						i, (valPtr->vendorInfos.vendorInfos)[i].sysver);
			ret = wid_illegal_character_check((char *)(valPtr->vendorInfos.vendorInfos)[i].sysver,(valPtr->vendorInfos.vendorInfos)[i].length, 0);
			if (ret == 1)
			{
			    AC_WTP[WTPIndex]->sysver = (char *)(valPtr->vendorInfos.vendorInfos)[i].sysver;
			}
			else
			{
				AC_WTP[WTPIndex]->sysver = NULL;
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"hw version has non readable char\n", \
							WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME);
			}
		}

		if ((valPtr->vendorInfos.vendorInfos)[i].type == 1)//CW_WTP_SOFTWARE_VERSION = 1
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"descriptor %u: sw version %s\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
						i, (valPtr->vendorInfos.vendorInfos)[i].ver);
			ret = wid_illegal_character_check((char *)(valPtr->vendorInfos.vendorInfos)[i].ver,(valPtr->vendorInfos.vendorInfos)[i].length, 0);
			if (ret == 1)
			{
				AC_WTP[WTPIndex]->ver = (char *)(valPtr->vendorInfos.vendorInfos)[i].ver;
				verlen = (valPtr->vendorInfos.vendorInfos)[i].length;
			}
			else
			{
				AC_WTP[WTPIndex]->ver = NULL;
				str_ac_version = (char *)(valPtr->vendorInfos.vendorInfos)[i].ver;				
				verlen = (valPtr->vendorInfos.vendorInfos)[i].length;
				wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"sw version has non readable char\n", \
							WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME);
			}
			break;
		}
	}

	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join model %s version %s code version %s\n",
	                        		WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
	                        		str_wtp_model ? (char*)str_wtp_model:"nil", \
	                        		AC_WTP[WTPIndex]->ver ? AC_WTP[WTPIndex]->ver : "nil" , \
	                        		(AC_WTP[WTPIndex]->codever != NULL) ? AC_WTP[WTPIndex]->codever : "nil");

	if ((AC_WTP[WTPIndex]->updateversion != NULL)&&(AC_WTP[WTPIndex]->updatepath != NULL))
	{
		str_ac_version = AC_WTP[WTPIndex]->updateversion;

		if (AC_WTP[WTPIndex]->codever == NULL)
		{
			if ((strlen(AC_WTP[WTPIndex]->updateversion) == verlen)
			    &&(strncasecmp(AC_WTP[WTPIndex]->ver,AC_WTP[WTPIndex]->updateversion,verlen) == 0))
			{
				bMatchVersion = CW_TRUE;
			}		
		}
		else if ((AC_WTP[WTPIndex]->codever != NULL)
			  && (strlen(AC_WTP[WTPIndex]->updateversion) == len)
			  && (strncasecmp(AC_WTP[WTPIndex]->codever,AC_WTP[WTPIndex]->updateversion,len) == 0))
		{
			str_ac_version = AC_WTP[WTPIndex]->ver;
			bMatchVersion = CW_TRUE;
		}		
	}
	else
	{
		if (find_in_wtp_list(WTPIndex) == CW_TRUE)
		{
			wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join found in batch upgrade list\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex,AC_WTP[WTPIndex]->WTPNAME);
			
			for (i = 0; i < BATCH_UPGRADE_AP_NUM; i++)
			{
				CWConfigVersionInfo *update_node = gConfigVersionUpdateInfo[i];
				if ((update_node != NULL)
					&& (update_node->str_ap_model != NULL)
					&& (strcmp(update_node->str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0))
				{
					while (update_node != NULL)
					{
						wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join code %s upgrade code %s version %s in %s\n",
        									WAI_FUNC_LINE_LITERAL , WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
        									AC_WTP[WTPIndex]->APCode, update_node->str_ap_code,  \
        									update_node->str_ap_version_name,update_node->str_ap_version_path);
                                
						if ((update_node->str_ap_code != NULL)&&(strcmp(update_node->str_ap_code,AC_WTP[WTPIndex]->APCode) == 0))
						{
							str_ac_version = update_node->str_ap_version_name;
							
							if (AC_WTP[WTPIndex]->codever == NULL)
							{
								if ((strlen(update_node->str_ap_version_name) == verlen)
									&&(strncasecmp(AC_WTP[WTPIndex]->ver,update_node->str_ap_version_name,verlen) == 0))
								{
									bMatchVersion = CW_TRUE;
								}
							}
							else if ((AC_WTP[WTPIndex]->codever != NULL)
									&& (strlen(update_node->str_ap_version_name) == len)
									&& (strncasecmp(AC_WTP[WTPIndex]->codever,update_node->str_ap_version_name,len) == 0))
							{
								str_ac_version = AC_WTP[WTPIndex]->ver;//zhanglei add for ap bug,cao
								bMatchVersion = CW_TRUE;
							}
							
							wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join code %s decide to update version %s\n", \
												WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
												update_node->str_ap_code, str_ac_version);
							result = 1;
							break;
						}
						update_node = update_node->next;
					}
					if (update_node == NULL)
					{
						str_ac_version = AC_WTP[WTPIndex]->ver;
						bMatchVersion = CW_TRUE;
					}
					break;
				}
			}
		}
		else
		{
			int do_check = 0;
			
//			CWThreadMutexLock(&(gAllThreadMutex));
			tmpnode = gConfigVerInfo;
			
			if (img_now == 0)
			{
				str_ac_version = AC_WTP[WTPIndex]->ver;
				bMatchVersion = CW_TRUE;
			}
			else for (j = 0; j < ((sizeof(SOFT_XMLINFO))/sizeof(SOFT_XMLINFO[0])); j++)
			{
				str_ac_version = AC_WTP[WTPIndex]->ver;
				if (strcmp(SOFT_XMLINFO[j].str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0)
				{	
					/*use while here to match the right information*/
					while (tmpnode != NULL)
					{
						if (strcmp(tmpnode->str_ap_model,AC_WTP[WTPIndex]->WTPModel) == 0)
						{
							CWCodeInfo *codenode = tmpnode->code_info;
							while (codenode != NULL)
							{
								if (strcmp(codenode->str_ap_version_code,AC_WTP[WTPIndex]->APCode) == 0)
								{
									do_check = 1;
									str_ac_version = codenode->str_ap_version_name;
									aclen = strlen(codenode->str_ap_version_name);
                                                             									
									if (AC_WTP[WTPIndex]->codever == NULL)
									{						
										if((strlen(codenode->str_ap_version_name) == verlen)
										    && (strncasecmp(AC_WTP[WTPIndex]->ver,codenode->str_ap_version_name,verlen) == 0))
										{
											bMatchVersion = CW_TRUE;
											break;
										}
									}
									else if ((aclen == len)
									        && (AC_WTP[WTPIndex]->codever != NULL)
									        && (strncasecmp(AC_WTP[WTPIndex]->codever,codenode->str_ap_version_name,len) == 0))
									{
										bMatchVersion = CW_TRUE;
										str_ac_version = AC_WTP[WTPIndex]->ver;//zhanglei add for ap bug,cao
										break;
									}
									break;
								}
								codenode = codenode->next;
							}
						}
						if (do_check == 1)
						{
						    break;
						}
						tmpnode = tmpnode->next;
					}
					if (do_check == 0)
					{
						bMatchVersion = CW_TRUE;
						str_ac_version = AC_WTP[WTPIndex]->ver; 		
					}
					break;
				}
			}
			if (do_check == 0)
			{
				bMatchVersion = CW_TRUE;
				str_ac_version = AC_WTP[WTPIndex]->ver; 		
			}
			//CWThreadMutexUnlock(&(gAllThreadMutex));		
#if 0
		    if(pVersionNode->str_ap_code != NULL)//book add, 2011-11-15
		    {
    			aclenmodel = strlen(pVersionNode->str_ap_code);
    			wid_syslog_debug_debug(WID_WTPINFO,"** AC surport model:%s **\n",pVersionNode->str_ap_code);
    			wid_syslog_debug_debug(WID_WTPINFO,"** AC version model len:%d WTP model len:%d **\n",aclenmodel,lenmodel);
			
    			if((((aclenmodel == lenmodel)&&(strncasecmp((char*)str_wtp_model,pVersionNode->str_ap_code,lenmodel) == 0))||
    				((strlen(pVersionNode->str_ap_model) == lenmodel)&&(strncasecmp((char*)str_wtp_model,pVersionNode->str_ap_model,lenmodel) == 0)))
    				//&&((strlen(pVersionNode->str_ap_model) == strlen(AC_WTP[WTPIndex]->WTPModel))&&(strncasecmp(AC_WTP[WTPIndex]->WTPModel,pVersionNode->str_ap_model,strlen(AC_WTP[WTPIndex]->WTPModel)) == 0))
    				)
    			{	
    				do_check = 1;
    				str_ac_version = pVersionNode->str_ap_version_name;
    				wid_syslog_debug_debug(WID_WTPINFO,"ac surport version:%s\n",str_ac_version);
    				aclen = strlen(pVersionNode->str_ap_version_name);
    				wid_syslog_debug_debug(WID_WTPINFO,"** AC surport version:%s **\n",pVersionNode->str_ap_version_name);
    				wid_syslog_debug_debug(WID_WTPINFO,"** AC version name len:%d WTP version name len:%d **\n",aclen,len);
    				
    				if((AC_WTP[WTPIndex]->codever == NULL)&&(img_now == 0)){
    					str_ac_version = AC_WTP[WTPIndex]->ver;
    					bMatchVersion = CW_TRUE;
    					break;						
    				}else if((AC_WTP[WTPIndex]->codever == NULL)){						
    					if((strlen(pVersionNode->str_ap_version_name) == verlen)&&(strncasecmp(AC_WTP[WTPIndex]->ver,pVersionNode->str_ap_version_name,verlen) == 0))
    					{
    						bMatchVersion = CW_TRUE;
    						break;
    					}
    				}
    				else if((img_now == 0)||((aclen == len)&&(AC_WTP[WTPIndex]->codever != NULL)&&(strncasecmp(AC_WTP[WTPIndex]->codever,pVersionNode->str_ap_version_name,len) == 0)))
    				{
    					bMatchVersion = CW_TRUE;
    					str_ac_version = AC_WTP[WTPIndex]->ver;//zhanglei add for ap bug,cao
    					break;
    				}
    			}
    	    }
#endif
		}
	}

	CWProtocolJoinResponseValues joinResponseVal;
	memset(&joinResponseVal, 0, sizeof(joinResponseVal));

	if (bMatchVersion)
	{
		if (!(CWAssembleJoinResponse(&(gWTPs[WTPIndex].messages), &(gWTPs[WTPIndex].messagesCount),
				gWTPs[WTPIndex].pathMTU, seqNum, msgElemList,str_ac_version,
				&joinResponseVal, WTP_UPGRADE_STATE_DONE, WTPIndex, WTP_LOGIN)))
		{ // random session ID
			CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);
			return CW_FALSE;
		}
	}
	else
	{
		char ac_version[DEFAULT_LEN];		
		memset(ac_version,0,DEFAULT_LEN);
		
		if ((WTP_UPGRADE_MODE_FTP == upgrade_mode)
			|| (WTP_UPGRADE_MODE_TFTP == upgrade_mode)
			|| (WTP_UPGRADE_MODE_HTTP == upgrade_mode))
		{
			sprintf(ac_version,"%s1",str_ac_version);
		}
		else if (WTP_UPGRADE_MODE_CAPWAP == upgrade_mode)
		{
			sprintf(ac_version,"%s1",str_ac_version);
			
			wtp_upgrade_init_hash(WTPIndex, NULL, str_ac_version);
			strncpy(joinResponseVal.ImageIndentifier.version, ac_version, WTP_VERSION_LEN-1);			
			joinResponseVal.ImageIndentifier.VersionLen = strlen(joinResponseVal.ImageIndentifier.version);

			/* TODO */
			snprintf(joinResponseVal.ImageIndentifier.ImageName, WTP_IMAGENAME_LEN, "%s.img", "wtp");			
			joinResponseVal.ImageIndentifier.ImageNameLen = strlen(joinResponseVal.ImageIndentifier.ImageName);

			snprintf(joinResponseVal.ImageIndentifier.Model, WTP_MODE_LEN, "%s", str_wtp_model);			
			joinResponseVal.ImageIndentifier.ModelLen = strlen(joinResponseVal.ImageIndentifier.Model);
			
		}
		else if (WTP_UPGRADE_MODE_NONE == upgrade_mode)
		{
			sprintf(ac_version,"%s1",str_ac_version);
		}
		else
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join upgrade mode %u error\n", \
							WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
							AC_WTP[WTPIndex]->upgrade.mode);
		}
		
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join model %s version %s upgrade mode %u\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
						str_wtp_model, ac_version,AC_WTP[WTPIndex]->upgrade.mode);

		if (!(CWAssembleJoinResponse(&(gWTPs[WTPIndex].messages), &(gWTPs[WTPIndex].messagesCount),
				gWTPs[WTPIndex].pathMTU, seqNum, msgElemList,ac_version, &joinResponseVal, upgrade_mode, WTPIndex, WTP_LOGIN)))
		{ // random session ID
			CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);
			return CW_FALSE;
		}
		AC_WTP[WTPIndex]->upgrade.state = WTP_UPGRADE_STATE_JOIN;
	}
	CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);
	
    CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
	if (!CWACSendFragments(WTPIndex))
	{
		CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
		return CW_FALSE;
 	}

	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join send msg %s\n", \
						WAI_FUNC_LINE_LITERAL, WTPIndex, (AC_WTP[WTPIndex]->WTPNAME) ? AC_WTP[WTPIndex]->WTPNAME : "",
						WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_RESPONSE)); 
	for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++)
	{
		CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
	}
	
	CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
	
	CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
	
	if (!CWErr(CWTimerRequest(CW_JOIN_INTERVAL_DEFAULT, &(gWTPs[WTPIndex].thread), &(gWTPs[WTPIndex].currentTimer), CW_CRITICAL_TIMER_EXPIRED_SIGNAL,WTPIndex)))
	{ // start Change State Pending timer
		_CWCloseThread(WTPIndex);
	}

	if (bMatchVersion)
	{
		if (WTP_LOGIN == WID_LOGIN)
		{
			gWTPs[WTPIndex].currentState = CW_ENTER_CONFIGURE;	
			AC_WTP[WTPIndex]->WTPStat = WID_CONFIGURE;
			wid_four_way_handshake_compatible(WTPIndex);
		}
		else
		{
			gWTPs[WTPIndex].currentState = CW_ENTER_RUN;	
			AC_WTP[WTPIndex]->WTPStat = WID_RUN;
			AC_WTP[WTPIndex]->neighbordeatimes = 0;
			if(AC_WTP[WTPIndex]->add_time == NULL)
			{
				AC_WTP[WTPIndex]->add_time = (time_t *)malloc(sizeof(time_t));
				time(AC_WTP[WTPIndex]->add_time);
			}
    	}
	}
	else
	{	
		if((updatewtplist != NULL)&&(find_in_wtp_list(WTPIndex) == CW_FALSE))
		{
			if(updatewtplist->count >= gupdateCountOneTime)
			{
			    if(WTP_LOGIN == WID_LOGIN)
        		{
                    gWTPs[WTPIndex].currentState = CW_QUIT; 
                    AC_WTP[WTPIndex]->WTPStat = WID_QUIT;   
                    _CWCloseThread(WTPIndex);
            	}
            	else
            	{
            		gWTPs[WTPIndex].currentState = CW_ENTER_RUN;	
            		AC_WTP[WTPIndex]->WTPStat = WID_RUN;
            	}
			}
			else
			{
				gWTPs[WTPIndex].currentState = CW_ENTER_IMAGE_DATA; 
				AC_WTP[WTPIndex]->WTPStat = WID_IMAGE_DATA;
			}
		}
		else
		{
			gWTPs[WTPIndex].currentState = CW_ENTER_IMAGE_DATA; 
			AC_WTP[WTPIndex]->WTPStat = WID_IMAGE_DATA;
		}
	}
	AC_WTP[WTPIndex]->quitreason = WTP_NORMAL;
	wid_syslog_info(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"enter join version %s next state %s\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					bMatchVersion ? "match":"mismatch", \
					WAI_STATE_STR(gWTPs[WTPIndex].currentState));
	return CW_TRUE;
}

// assemble Join Response
CWBool CWAssembleJoinResponse
(
	CWProtocolMessage **messagesPtr,
	int *fragmentsNumPtr,
	int PMTU,
	int seqNum,
	CWList msgElemList,
	char *strVersion,
	CWProtocolJoinResponseValues *joinResponseVal,
	enum wtp_upgrade_mode upgrade_mode,
	int WTPIndex,
	WTPLoginState WTP_LOGIN
)
{	
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 0;
	int mandatoryMsgElemCount = 4; 	//Result code is not included because it's already in msgElemList. Control IPv6 to be added
	CWProtocolMessage *msgElemsBinding = NULL;
	const int msgElemBindingCount=0;
	int i = 0;
	CWListElement *current = NULL;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL || msgElemList == NULL)
    {
        return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
    }
	msgElemCount = CWCountElementInList(msgElemList);

	if (WTP_UPGRADE_MODE_CAPWAP == upgrade_mode)
	{
		mandatoryMsgElemCount++;
		
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount+mandatoryMsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		if((!(CWAssembleMsgElemACDescriptor(&(msgElems[++k]))))
			|| (!(CWAssembleMsgElemACName(&(msgElems[++k]))))
			|| (!(CWAssembleMsgElemWTPUpgradeMode(&(msgElems[++k]), upgrade_mode)))
			|| (!(CWAssembleMsgElemImageIdentifier(&(msgElems[++k]), &(joinResponseVal->ImageIndentifier))))
			|| (!(CWAssembleMsgElemCWControlIPv4Addresses(&(msgElems[++k]),0)))	//zhanglei set wtpid 0 in join	  
			)
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
	}
	else if(WTP_UPGRADE_MODE_FTP == upgrade_mode
	        || WTP_UPGRADE_MODE_TFTP == upgrade_mode
	        || WTP_UPGRADE_MODE_HTTP == upgrade_mode)
	{
		mandatoryMsgElemCount++;
		
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount+mandatoryMsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		if((!(CWAssembleMsgElemACDescriptor(&(msgElems[++k])))) ||
		   (!(CWAssembleMsgElemACName(&(msgElems[++k])))) ||
		   (!(CWAssembleMsgElemWTPUpgradeMode(&(msgElems[++k]), upgrade_mode))) ||
		   (!(CWAssembleMsgElemWTPVersion(&(msgElems[++k]),strVersion))) ||
		   (!(CWAssembleMsgElemCWControlIPv4Addresses(&(msgElems[++k]),0)))//zhanglei set wtpid 0 in join	  
			)
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
	}
	else if(WTP_UPGRADE_MODE_NONE == upgrade_mode)
	{
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount+mandatoryMsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		if((!(CWAssembleMsgElemACDescriptor(&(msgElems[++k])))) ||
		   (!(CWAssembleMsgElemACName(&(msgElems[++k])))) ||
		   (!(CWAssembleMsgElemWTPVersion(&(msgElems[++k]),strVersion))) ||
		   (!(CWAssembleMsgElemCWControlIPv4Addresses(&(msgElems[++k]),0))))
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
	}

	current = msgElemList;
	for (i = 0; i < msgElemCount; i++)
	{
        switch (((CWMsgElemData *) (current->data))->type)
		{
			case CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE:
				if (!(CWAssembleMsgElemACIPv4List(&(msgElems[++k]))))
					goto cw_assemble_error;	
				break;			
			case CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE:
				if (!(CWAssembleMsgElemACIPv6List(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				if (!(CWAssembleMsgElemResultCode(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value)))
					goto cw_assemble_error;
				break;
/*			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
				if (!(CWAssembleMsgElemSessionID(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value)))
					goto cw_assemble_error;
				break;*/
            default:
            {
                int j = 0;
                for(j = 0; j <= k; j++)
                {
                    CW_FREE_PROTOCOL_MESSAGE(msgElems[j]);
                }
                CW_FREE_OBJECT(msgElems);
                return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element for Join Response Message");
				break;
	        }
        }

		current = current->next;
	}

	if (!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, CW_MSG_TYPE_VALUE_JOIN_RESPONSE, msgElems, msgElemCount+mandatoryMsgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT, AC_WTP[WTPIndex]->cap_pro_ver)))
	{
	    return CW_FALSE;
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"assembled %s msg seq %u capprover %u with %u elements\n",  \
					 WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_RESPONSE),\
					 seqNum, AC_WTP[WTPIndex]->cap_pro_ver, msgElemCount+mandatoryMsgElemCount);
	return CW_TRUE;

cw_assemble_error:
	{
		int i = 0;
		for(i = 0; i <= k; i++)
		{
		    CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
	    }
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	return CW_TRUE;
}

// Parse Join Request
CWBool CWParseJoinRequestMessage
(
	char *msg,
	int len,
	int *seqNumPtr,
	CWProtocolJoinRequestValues *valuesPtr,
	int WTPIndex,
	WTPLoginState *WTP_LOGIN
)
{
	int offsetTillMessages = 0;
	CWControlHeaderValues controlVal;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || seqNumPtr == NULL || valuesPtr == NULL || WTP_LOGIN == NULL)
	{
	    return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	memset(&completeMsg, 0, sizeof(CWProtocolMessage));
	completeMsg.msg = msg;
	completeMsg.offset = 0;
		
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
	{
	    return CW_FALSE; // will be handled by the caller
    }

	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_JOIN_REQUEST)
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"try parse %s but msg type(%u) %s no as expect\n", \
					WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME, \
					WAI_CAPWAP_MSG_TYPE_STR(CW_MSG_TYPE_VALUE_JOIN_REQUEST), \
					controlVal.messageTypeValue, WAI_CAPWAP_MSG_TYPE_STR(controlVal.messageTypeValue));
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Join Request as Expected");
	}
	
	wid_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI_FMT"Parse join Request", WAI_FUNC_LINE_LITERAL, WTPIndex, AC_WTP[WTPIndex]->WTPNAME);
	
	*seqNumPtr = controlVal.seqNum;
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
	
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen)
	{
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		unsigned short int subelemid = 0;
		unsigned short int subelemlen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
										
		switch(elemType)
		{
			case CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE:
			{
				if(!(CWParseLocationData(&completeMsg, elemLen, &(valuesPtr->location))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
			{
				if(!(CWParseWTPBoardData(&completeMsg, elemLen, &(valuesPtr->WTPBoardData))))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
			{
				if(!(CWParseSessionID(&completeMsg, elemLen, valuesPtr)))   // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
			{
				if(!(CWParseWTPDescriptor(&completeMsg, elemLen, &(valuesPtr->WTPDescriptor))))// will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE:
			{
				if(!(CWParseWTPIPv4Address(&completeMsg, elemLen, valuesPtr)))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_NAME_CW_TYPE:
			{
				if(!(CWParseWTPName(&completeMsg, elemLen, &(valuesPtr->name))))// will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
			{
				if(!(CWParseWTPFrameTunnelMode(&completeMsg, elemLen, &(valuesPtr->frameTunnelMode))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
			{
				if(!(CWParseWTPMACType(&completeMsg, elemLen, &(valuesPtr->MACType))))  // will be handled by the caller
				{
					return CW_FALSE;
				}
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			{
				CWParseFormatMsgElem(&completeMsg, &subelemid, &subelemlen);
				if(subelemlen > controlVal.msgElemsLen)
				{
					wid_syslog_err("%s elemType %d elemlen:%d > controlVal.msgElemsLen:%d\n", __func__, elemType, subelemlen, controlVal.msgElemsLen);
					return CW_TRUE;
				}
				if(CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE == subelemid)
				{
					if(!(CWParseWTPLOGINType(&completeMsg, subelemlen, &(valuesPtr->ConStatus))))   // will be handled by the caller
					{
						return CW_FALSE;
					}
				}
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}
	
	if(completeMsg.offset != len)
	{
	    return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
	
	*WTP_LOGIN = valuesPtr->ConStatus;
	
	return CW_TRUE;
}

CWBool CWSaveJoinRequestMessage (CWProtocolJoinRequestValues *joinRequest, CWWTPProtocolManager *WTPProtocolManager, unsigned int WTPIndex)
{  
	int i = 0;
	if (joinRequest == NULL || WTPProtocolManager == NULL)
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if ((joinRequest->location) != NULL)
	{
		CW_FREE_OBJECT(WTPProtocolManager->locationData);
		WTPProtocolManager->locationData= joinRequest->location;
	}
	else
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	if ((joinRequest->name)!= NULL)
	{
		CW_FREE_OBJECT(WTPProtocolManager->name);
		WTPProtocolManager->name= joinRequest->name;
	}
	else
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	for (i = 0; i < WTPProtocolManager->WTPBoardData.vendorInfosCount; i++)
	{
		if ((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].model);
		}
		else if ((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].SN);
		}
		else if ((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].mac);
		}
		else if ((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].Rmodel);
		}
		else if ((WTPProtocolManager->WTPBoardData.vendorInfos)[i].type == CW_WTP_CODE_VERSION)
		{
			AC_WTP[WTPIndex]->codever = NULL;
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].codever);
		}
		else
		{
			CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData.vendorInfos)[i].valuePtr);
		}
	}
	CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData).vendorInfos);
	WTPProtocolManager->WTPBoardData = joinRequest->WTPBoardData;

	WTPProtocolManager->sessionID = joinRequest->sessionID;
	WTPProtocolManager->ipv4Address = joinRequest->addr;
	
	for (i = 0; i < (WTPProtocolManager->descriptor.vendorInfos).vendorInfosCount; i++)
	{
		if ((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_HARDWARE_VERSION)
		{
			CW_FREE_OBJECT(((WTPProtocolManager->descriptor.vendorInfos).vendorInfos)[i].sysver);
		}
		else if((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
		{
			AC_WTP[WTPIndex]->ver = NULL;
			CW_FREE_OBJECT(((WTPProtocolManager->descriptor.vendorInfos).vendorInfos)[i].ver);
		}
		else if ((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].type == CW_BOOT_VERSION)
		{
			CW_FREE_OBJECT(WTPProtocolManager->descriptor.vendorInfos.vendorInfos[i].bootver);
		}
		else
		{
			CW_FREE_OBJECT((WTPProtocolManager->descriptor.vendorInfos.vendorInfos)[i].valuePtr);
		}
	}
	CW_FREE_OBJECT((WTPProtocolManager->descriptor.vendorInfos).vendorInfos);
	WTPProtocolManager->descriptor= joinRequest->WTPDescriptor;
	WTPProtocolManager->radiosInfo.radioCount = (joinRequest->WTPDescriptor).radiosInUse;
	CW_FREE_OBJECT(WTPProtocolManager->radiosInfo.radiosInfo);
	CW_CREATE_ARRAY_ERR(WTPProtocolManager->radiosInfo.radiosInfo, WTPProtocolManager->radiosInfo.radioCount, CWWTPRadioInfoValues,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	for (i = 0; i < WTPProtocolManager->radiosInfo.radioCount; i++)
	{
		WTPProtocolManager->radiosInfo.radiosInfo[i].radioID = i;
		//WTPProtocolManager->radiosInfo.radiosInfo[i].stationCount = 0;
		WTPProtocolManager->radiosInfo.radiosInfo[i].adminState = ENABLED; //default value for CAPWAP
		WTPProtocolManager->radiosInfo.radiosInfo[i].adminCause = AD_NORMAL;
		WTPProtocolManager->radiosInfo.radiosInfo[i].operationalState = DISABLED;
		WTPProtocolManager->radiosInfo.radiosInfo[i].operationalCause = OP_NORMAL;
		WTPProtocolManager->radiosInfo.radiosInfo[i].TxQueueLevel = 0;
		WTPProtocolManager->radiosInfo.radiosInfo[i].wirelessLinkFramesPerSec = 0; 
	}

	return CW_TRUE;
}

CWBool CWAssembleMsgElemImageIdentifier
(
	CWProtocolMessage *msgPtr, 
	CWImageIdentifier_cw *ImageIdentifier
)
{
	short length = 0;  

	if ((NULL == msgPtr) || (NULL == ImageIdentifier))
	{
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	} 

	length = 4 + ((4 + ImageIdentifier->ImageNameLen)
			   + (4 + ImageIdentifier->VersionLen)
			   + (4 + ImageIdentifier->ModelLen));
	
	wid_syslog_debug_debug(WID_WTPINFO,"%s image %s len %d version %s %d model %s len %d\n",
						__func__, 
						ImageIdentifier->ImageName, ImageIdentifier->ImageNameLen,
						ImageIdentifier->version, ImageIdentifier->VersionLen, 
						ImageIdentifier->Model, ImageIdentifier->ModelLen);
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

//	wid_syslog_debug_debug(WID_WTPINFO,"%s:%d\n", __func__, __LINE__);
	
	CWProtocolStore32(msgPtr, CW_MSG_ELEMENT_VENDOR_IDENTIFIER);
 
	CWProtocolStore16(msgPtr, CW_IMAGEIDENTIFIER_IMAGENAME);
	CWProtocolStore16(msgPtr, ImageIdentifier->ImageNameLen);
	CWProtocolStoreStr(msgPtr, ImageIdentifier->ImageName);
 
	CWProtocolStore16(msgPtr, CW_IMAGEIDENTIFIER_VERSION);
	CWProtocolStore16(msgPtr, ImageIdentifier->VersionLen);
	CWProtocolStoreStr(msgPtr, ImageIdentifier->version);

	CWProtocolStore16(msgPtr, CW_IMAGEIDENTIFIER_MODE);
	CWProtocolStore16(msgPtr, ImageIdentifier->ModelLen);
	CWProtocolStoreStr(msgPtr, ImageIdentifier->Model);

//	wid_syslog_debug_debug(WID_WTPINFO,"%s:%d\n", __func__, __LINE__);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE);
}

unsigned long int HexToDec(unsigned char *hex)
{
	char *tmp=(char *)hex;
	unsigned int dec=0;
	//int m = 0;
	//printf("tmp:%p\n");
	while (*tmp)
	{
		//m++;
		//printf("$m:%d$",m);
		dec<<=4;
		//printf("$dec:%d$",dec);
		if (*tmp&16) dec+=*tmp++&15;
		else dec+=(*tmp++&15)+9;
	}
	//printf("dec:%d\n",dec);
	return dec;
}

unsigned int get_file_size(const char *path)
{
	unsigned int size = 0;
	FILE *fp = NULL;

	fp = fopen(path, "rb");
	if(NULL == fp)
	{
		wid_syslog_err("%s Open file %s failed.", path, strerror(errno));
		return 0;
	}

	fseek(fp, SEEK_SET, SEEK_END);
	size = ftell(fp);
	fclose(fp);
	
	wid_syslog_debug_debug(WID_DEFAULT, "Size of file %s size %d bytes\n", path, size);
	
	return size;
}

int md5_hash(char *path, unsigned char *md5)
{
	char cmd[256] = {0};
	char buf[256] = {0};
	FILE *fp = 	NULL;

	snprintf(cmd, sizeof(cmd), "sor.sh imgmd5 %s 120", path);
	wid_syslog_debug("%s %s\n", __func__, cmd);

	fp = popen(cmd, "r");
	if (NULL == fp)
	{
		wid_syslog_err("%s popen %s failed %s\n", __func__, path, strerror(errno));
		return -1;
	}	
	fgets(buf, sizeof(buf)-1, fp);	
	pclose(fp);
	fp = NULL;

	char *p = NULL;
	p = strstr(buf, "= ");
	if (NULL == p)
	{
		wid_syslog_err("%s get md5 string error, buf %s\n", __func__, buf);
		return -1;
	}
	p +=2;	/* stip string "= " */

	unsigned char a[3];
	int i = 0;
	unsigned long ul = 0;
	
	memset(md5, 0, WID_MD5_LEN);
	
	for(i = 0; i < 32; i = i + 2)
	{
		memset(a, 0, sizeof(a));
		strncpy((char *)a, p+i, 2);
		ul = HexToDec(a);
		md5[i/2] = ul;
	}
	
	wid_syslog_debug("%02x %02x %02x %02x %02x %02x %02x %02x\n",
		md5[0], md5[1], md5[2], md5[3], md5[4], md5[5], md5[6], md5[7]);

	wid_syslog_debug("%02x %02x %02x %02x %02x %02x %02x %02x\n",
		md5[8], md5[9], md5[10], md5[11], md5[12], md5[13], md5[14], md5[15]);

	#if 0
	ret = pclose(fp);
	
	switch (WEXITSTATUS(ret)) 
	{
		case 0:
			wid_syslog_info("%s,%s\n",__func__, md5);
			return 0;
		case 1:
			wid_syslog_info("Sysetm internal error (1).\n");
			break;
		case 2:
			wid_syslog_info("Sysetm internal error (2).\n");
			break;
		case 3:
			wid_syslog_info("Storage media is busy.\n");
			break;
		case 4:
			wid_syslog_info("Storage operation time out.\n");
			break;
		case 5:
			wid_syslog_info("No left space on storage media.\n");
			break;
		default:
			wid_syslog_info("Sysetm internal error (3).\n");
			break;
		}
	#endif
	
	return 0;
}

int wid_md5_hash(char *path, unsigned char *md5)
{
	unsigned char buf[WID_MD5_LEN] = {0};

	if (md5_hash(path, buf) < 0)
	{
		return -1;
	}

	memcpy(md5, buf, WID_MD5_LEN);
	
	return 0;
}


int wtp_upgrade_init_hash(unsigned int wtpid, char *ImageName, char *version)
{
	char path[128] = {0};
	
	/* init upgrade image name */
	if (NULL != version)
	{
		strncpy(AC_WTP[wtpid]->upgrade.version, version, WTP_VERSION_LEN-1);
	}

	if (NULL != ImageName)
	{
		strncpy(AC_WTP[wtpid]->upgrade.ImageName, ImageName, WTP_IMAGENAME_LEN-1);
		
		snprintf(path, sizeof(path), "/mnt/wtp/%s", ImageName);
		
		AC_WTP[wtpid]->upgrade.filesize = get_file_size(path);
		wid_md5_hash(path, AC_WTP[wtpid]->upgrade.hash);
		AC_WTP[wtpid]->upgrade.state = WTP_UPGRADE_STATE_INITMD5;
		
		wid_syslog_debug("wtp%d init md5 hash image %s size %d\n", 
			wtpid, ImageName, AC_WTP[wtpid]->upgrade.filesize);
	}

	return 0;	
}


