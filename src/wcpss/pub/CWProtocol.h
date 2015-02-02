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
* CWProtocol.h
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


#ifndef __CAPWAP_CWProtocol_HEADER__
#define __CAPWAP_CWProtocol_HEADER__

#include "wcpss/wid/WID.h"

//#define CWSetField32(obj, start, val)	((obj)[start/64]) |= ((val) << (start%64))	
//#define CWGetField32(obj, start, len)	(((obj)[start/64]) & ((0xFFFFFFFFFFFFFFFF >> (64-(len))) << (start%64)) ) >> (start%64)

/*_____________________________________________________*/
/*  *******************___MACRO___*******************  */
//#define CWSetField32(obj, start, val)					((obj)[start/32]) |= ((val) << (start%32))	
//#define CWGetField32(obj, start, len)					(((obj)[start/32]) & ((0xFFFFFFFFUL >> (32-(len))) << (start%32)) ) >> (start%32)

#define CWSetField32(src,start,len,val)					src |= ((~(0xFFFFFFFF << len)) & val) << (32 - start - len)
#define CWGetField32(src,start,len)					((~(0xFFFFFFFF<<len)) & (src >> (32 - start - len)))

#define CW_REWIND_BYTES(buf, bytes, type)				(buf) = (type*)(((char*) (buf)) - bytes)
#define CW_SKIP_BYTES(buf, bytes, type)					(buf) = (type*)(((char*) (buf)) + bytes)
#define CW_SKIP_BITS(buf, bits, type)					(buf) = (type*)(((char*) (buf)) + ((bits) / 8))
#define CW_BYTES_TO_STORE_BITS(bits)					((((bits) % 8) == 0) ? ((bits) / 8) : (((bits) / 8)+1))

#define		CW_CREATE_PROTOCOL_MESSAGE(mess, size, err)		CW_CREATE_OBJECT_SIZE_ERR(((mess).msg), (size), err);		\
									CW_ZERO_MEMORY(((mess).msg), (size));						\
									(mess).offset = 0;

#define 	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(ar_name, ar_size, on_err) 	{\
											CW_CREATE_ARRAY_ERR(ar_name, ar_size, CWProtocolMessage, on_err)\
											int i;\
											for(i=0;i<(ar_size); i++) {\
												(ar_name)[i].msg = NULL;\
												(ar_name)[i].offset = 0; \
											}\
										}

#define		CW_FREE_PROTOCOL_MESSAGE(mess)				CW_FREE_OBJECT(((mess).msg));								\
									(mess).offset = 0;
															
#define		CWParseMessageElementStart()				int oldOffset;												\
									if(msgPtr == NULL || valPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	\
									oldOffset = msgPtr->offset
						
#define		CWParseMessageElementEnd()				CWDebugLog(NULL);											\
									return ((msgPtr->offset) - oldOffset) == len ? CW_TRUE :	\
									CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message Element Malformed");


/*_________________________________________________________*/
/*  *******************___CONSTANTS___*******************  */

// to be defined
#define     	MAX_UDP_PACKET_SIZE					65536
#define		CW_CONTROL_PORT					        5246
#define		CW_CONTROL_PORT_AU					    1234
#define		CW_DATA_PORT					        5247	
#define		CW_PROTOCOL_VERSION					    0
#define		CW_PROTOCOL_VERSION_PRO					1
#define		CW_IANA_ENTERPRISE_NUMBER				13277
#define		CW_CAPWAP_NUMBER		0
#define 	CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS			3		//Offset "Seq Num" - "Message Elements"
#define		CW_MAX_SEQ_NUM						255
#define 	CW_MAX_FRAGMENT_ID					65535
#define 	CLEAR_DATA						1
#define		DTLS_ENABLED_DATA					2
#define		CW_PACKET_PLAIN						0
#define		CW_PACKET_CRYPT						0/*wuwl change it from 1 to 0*/

// <TRANSPORT_HEADER_FIELDS>
// CAPWAP version (currently 0)
#define 	CW_TRANSPORT_HEADER_VERSION_START			0
#define 	CW_TRANSPORT_HEADER_VERSION_LEN				4

#define		CW_TRANSPORT_HEADER_TYPE_START				4
#define		CW_TRANSPORT_HEADER_TYPE_LEN				4

// Radio ID number (for WTPs with multiple radios)
#define 	CW_TRANSPORT_HEADER_RID_START				13
#define 	CW_TRANSPORT_HEADER_RID_LEN				5

// Length of CAPWAP tunnel header in 4 byte words 
#define 	CW_TRANSPORT_HEADER_HLEN_START				8
#define 	CW_TRANSPORT_HEADER_HLEN_LEN				5

// Wireless Binding ID
#define 	CW_TRANSPORT_HEADER_WBID_START				18
#define 	CW_TRANSPORT_HEADER_WBID_LEN				5

// Format of the frame
#define 	CW_TRANSPORT_HEADER_T_START				23
#define 	CW_TRANSPORT_HEADER_T_LEN				1

// Is a fragment?
#define 	CW_TRANSPORT_HEADER_F_START				24
#define 	CW_TRANSPORT_HEADER_F_LEN				1

// Is NOT the last fragment?
#define 	CW_TRANSPORT_HEADER_L_START				25
#define 	CW_TRANSPORT_HEADER_L_LEN				1

// Is the Wireless optional header present?
#define 	CW_TRANSPORT_HEADER_W_START				26
#define 	CW_TRANSPORT_HEADER_W_LEN				1

// Is the Radio MAC Address optional field present?
#define 	CW_TRANSPORT_HEADER_M_START				27
#define 	CW_TRANSPORT_HEADER_M_LEN				1

// Is the message a keep alive?
#define 	CW_TRANSPORT_HEADER_K_START				28
#define 	CW_TRANSPORT_HEADER_K_LEN				1

// Set to 0 in this version of the protocol
#define 	CW_TRANSPORT_HEADER_FLAGS_START				29
#define 	CW_TRANSPORT_HEADER_FLAGS_LEN				3

// ID of the group of fragments
#define 	CW_TRANSPORT_HEADER_FRAGMENT_ID_START			0
#define 	CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN			16

// Position of this fragment in the group 
#define 	CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START		16
#define 	CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN			13

#define     CW_TRANSPORT_HEADER_CONTAIN_MAC_OFFSET_START    29
#define     CW_TRANSPORT_HEADER_CONTAIN_MAC_OFFSET_LEN      1

// Set to 0 in this version of the protocol
#define 	CW_TRANSPORT_HEADER_RESERVED_START			29
#define 	CW_TRANSPORT_HEADER_RESERVED_LEN			3
// </TRANSPORT_HEADER_FIELDS>


// Message Type Values
#define		CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST			1
#define		CW_MSG_TYPE_VALUE_DISCOVERY_RESPONSE		2
#define		CW_MSG_TYPE_VALUE_JOIN_REQUEST				3
#define		CW_MSG_TYPE_VALUE_JOIN_RESPONSE				4
#define		CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST			5
#define		CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE			6
#define		CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST		7
#define		CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE		8
#define 	CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST			9
#define 	CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE			10
#define		CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST		11
#define		CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE		12
#define		CW_MSG_TYPE_VALUE_ECHO_REQUEST				13
#define		CW_MSG_TYPE_VALUE_ECHO_RESPONSE				14
#define		CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST			15
#define		CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE			16
#define		CW_MSG_TYPE_VALUE_RESET_REQUEST				17
#define		CW_MSG_TYPE_VALUE_RESET_RESPONSE			18
#define		CW_MSG_TYPE_VALUE_PRIMARY_DISCOVERY_REQUEST		19
#define		CW_MSG_TYPE_VALUE_PRIMARY_DISCOVERY_RESPONSE		20
#define		CW_MSG_TYPE_VALUE_DATA_TRANSFER_REQUEST			21
#define		CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE		22
#define		CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST		23
#define		CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE		24
#define		CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST		25
#define		CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE	26

#define		CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_REQUEST		3398912
#define		CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_RESPONSE		3398913

#define WAI_CAPWAP_MSG_TYPE_STR(m)	\
		      ((CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST == m) ? "discov-req" :\
			(CW_MSG_TYPE_VALUE_DISCOVERY_RESPONSE == m) ? "discov-resp" :\
			(CW_MSG_TYPE_VALUE_JOIN_REQUEST == m) ? "join-req" :\
			(CW_MSG_TYPE_VALUE_JOIN_RESPONSE == m) ? "join-resp" :\
			(CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST == m) ? "conf-req" :\
			(CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE == m) ? "conf-resp" :\
			(CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST == m) ? "conf-update-req" :\
			(CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE == m) ? "conf-update-resp" :\
			(CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST == m) ? "wtp-event-req" :\
			(CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE == m) ? "wtp-event-resp" :\
			(CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST == m) ? "chg-state-event-req" :\
			(CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE == m) ? "chg-state-event-resp" :\
			(CW_MSG_TYPE_VALUE_ECHO_REQUEST == m) ? "echo-req" :\
			(CW_MSG_TYPE_VALUE_ECHO_RESPONSE == m) ? "echo-resp" :\
			(CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST == m) ? "img-data-req" :\
			(CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE == m) ? "img-data-resp" :\
			(CW_MSG_TYPE_VALUE_RESET_REQUEST == m) ? "rst-req" :\
			(CW_MSG_TYPE_VALUE_RESET_RESPONSE == m) ? "rst-resp" :\
			(CW_MSG_TYPE_VALUE_PRIMARY_DISCOVERY_REQUEST == m) ? "pri-discov-req" :\
			(CW_MSG_TYPE_VALUE_PRIMARY_DISCOVERY_RESPONSE == m) ? "pri-discov-resp" :\
			(CW_MSG_TYPE_VALUE_DATA_TRANSFER_REQUEST == m) ? "data-trans-req" :\
			(CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE == m) ? "data-trans-resp" :\
			(CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST == m) ? "clr-conf-req" :\
			(CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE == m) ? "clr-conf-resp" :\
			(CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST == m) ? "sta-conf-req" :\
			(CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE == m) ? "sta-conf-resp" :\
			(CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_REQUEST == m) ? "wlan-conf-req" :\
			(CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_RESPONSE == m) ? "wlan-conf-resp" :\
			"unkown")
// Message Elements Type Values
#define 	CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE			1
#define 	CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE			2
#define 	CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE			3
#define 	CW_MSG_ELEMENT_AC_NAME_CW_TYPE				4
#define 	CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE		5
#define		CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE			6
#define		CW_MSG_ELEMENT_ADD_MAC_ACL_CW_TYPE			7
#define		CW_MSG_ELEMENT_ADD_STATION_CW_TYPE			8
#define		CW_MSG_ELEMENT_ADD_STATIC_MAC_ACL_CW_TYPE		9
#define 	CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE		10
#define 	CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE		11
#define		CW_MSG_ELEMENT_CW_TIMERS_CW_TYPE				12
#define		CW_MSG_ELEMENT_DATA_TRANSFER_DATA_CW_TYPE		13
#define		CW_MSG_ELEMENT_DATA_TRANSFER_MODE_CW_TYPE		14
#define 	CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE		15
#define 	CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_PERIOD_CW_TYPE	16
#define 	CW_MSG_ELEMENT_DELETE_MAC_ACL_CW_TYPE			17
#define 	CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE			18
#define 	CW_MSG_ELEMENT_DELETE_STATIC_MAC_ACL_CW_TYPE		19
#define 	CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE			20
#define 	CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE		21
#define 	CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE		22
#define 	CW_MSG_ELEMENT_IDLE_TIMEOUT_CW_TYPE			23
#define 	CW_MSG_ELEMENT_IMAGE_DATA_CW_TYPE			24
#define 	CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE			25
#define 	CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE			26
#define 	CW_MSG_ELEMENT_INITIATED_DOWNLOAD_CW_TYPE		27
#define 	CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE			28
#define 	CW_MSG_ELEMENT_MAX_MSG_LEN_CW_TYPE			29
#define 	CW_MSG_ELEMENT_MTU_DISCOVERY_PADDING_CW_TYPE		30
#define 	CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE		31
#define 	CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE		32
#define 	CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE			33
#define 	CW_MSG_ELEMENT_RETURNED_MSG_ELEM_CW_TYPE		34
#define 	CW_MSG_ELEMENT_SESSION_ID_CW_TYPE			35
#define 	CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE			36
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE		37
#define 	CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE			38
#define 	CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE			39
#define 	CW_MSG_ELEMENT_WTP_FALLBACK_CW_TYPE			40
#define 	CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE		41
#define 	CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE			42
#define 	CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE			43
#define 	CW_MSG_ELEMENT_WTP_NAME_CW_TYPE				44
#define 	CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE		45
#define 	CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE		46
#define 	CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE		47
#define 	CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE			48
#define 	CW_MSG_ELEMENT_WTP_SERVER_INFORMATION_TYPE			49
#define 	CW_MSG_ELEMENT_ETH_SET_CW_TYPE			253          //fengwenchao add 20110126 for XJDEV-32 from 2.0
#define 	CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2			255 //added dns packet
#define 	CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE			252
#define 	CW_MSG_ELEMENT_WTP_11N_MCS_CWMODE_GUARDINTERVAL			254 //11N parameter

#define 	CW_MSG_ELEMENT_VENDOR_IDENTIFIER	31656 		//Autelan Vendor Identifier

#define WAI_CAPWAP_MSG_ELEMENT_TYPE_STR(m)	\
		      ((CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE == m) ? "ac-descriptor" : \
			(CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE == m) ? "ipv4-list" : \
			(CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE == m) ? "ipv6-list" : \
			(CW_MSG_ELEMENT_AC_NAME_CW_TYPE == m) ? "ac-name" : \
			(CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE == m) ? "ac-name-idx" : \
			(CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE == m) ? "timestamp" : \
			(CW_MSG_ELEMENT_ADD_MAC_ACL_CW_TYPE == m) ? "add-mac-acl" : \
			(CW_MSG_ELEMENT_ADD_STATION_CW_TYPE == m) ? "add-sta" : \
			(CW_MSG_ELEMENT_ADD_STATIC_MAC_ACL_CW_TYPE == m) ? "add-sta-mac-acl" : \
			(CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE == m) ? "ctrl-ipv4" : \
			(CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE == m) ? "ctrl-ipv6" : \
			(CW_MSG_ELEMENT_CW_TIMERS_CW_TYPE == m) ? "timer" : \
			(CW_MSG_ELEMENT_DATA_TRANSFER_DATA_CW_TYPE == m) ? "tx-data" : \
			(CW_MSG_ELEMENT_DATA_TRANSFER_MODE_CW_TYPE == m) ? "tx-mode" : \
			(CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE == m) ? "decrypt-err-report" : \
			(CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_PERIOD_CW_TYPE == m) ? "decrypt-err-report-intvl" : \
			(CW_MSG_ELEMENT_DELETE_MAC_ACL_CW_TYPE == m) ? "del-mac-acl" : \
			(CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE == m) ? "del-sta" : \
			(CW_MSG_ELEMENT_DELETE_STATIC_MAC_ACL_CW_TYPE == m) ? "del-sta-mac-acl" : \
			(CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE == m) ? "discover-type" : \
			(CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE == m) ? "dup-ipv4" : \
			(CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE == m) ? "dup-ipv6" : \
			(CW_MSG_ELEMENT_IDLE_TIMEOUT_CW_TYPE == m) ? "idle-timeout" : \
			(CW_MSG_ELEMENT_IMAGE_DATA_CW_TYPE == m) ? "image-data" : \
			(CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE == m) ? "image-id" : \
			(CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE == m) ? "image-info" : \
			(CW_MSG_ELEMENT_INITIATED_DOWNLOAD_CW_TYPE == m) ? "initial-download" : \
			(CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE == m) ? "location-data" : \
			(CW_MSG_ELEMENT_MAX_MSG_LEN_CW_TYPE == m) ? "max-msg-len" : \
			(CW_MSG_ELEMENT_MTU_DISCOVERY_PADDING_CW_TYPE == m) ? "mtu-discover-pad" : \
			(CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE == m) ? "radio-adm-state" : \
			(CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE == m) ? "radio-op-stat" : \
			(CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE == m) ? "result-code" : \
			(CW_MSG_ELEMENT_RETURNED_MSG_ELEM_CW_TYPE == m) ? "return-msg-elem" : \
			(CW_MSG_ELEMENT_SESSION_ID_CW_TYPE == m) ? "session-id" : \
			(CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE == m) ? "stat-timer" : \
			(CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE == m) ? "vs-payload" : \
			(CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE == m) ? "wtp-board-data" : \
			(CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE == m) ? "wtp-desc" : \
			(CW_MSG_ELEMENT_WTP_FALLBACK_CW_TYPE == m) ? "wtp-fallback" : \
			(CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE == m) ? "frm-tun-mode" : \
			(CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE == m) ? "wtp-ipv4" : \
			(CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE == m) ? "wtp-mac-type" : \
			(CW_MSG_ELEMENT_WTP_NAME_CW_TYPE == m) ? "wtp-name" : \
			(CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE == m) ? "wtp-op-stat" : \
			(CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE == m) ? "wtp-radio-stat" : \
			(CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE == m) ? "wtp-reboot-stat" : \
			(CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE == m) ? "wtp-static-ip" : \
			(CW_MSG_ELEMENT_WTP_SERVER_INFORMATION_TYPE == m) ? "wtp-svr-info" : \
			(CW_MSG_ELEMENT_ETH_SET_CW_TYPE == m) ? "eth-set" : \
			(CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2 == m) ? "wtp-static-ip-2" : \
			(CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE == m) ? "all-time-set" : \
			(CW_MSG_ELEMENT_WTP_11N_MCS_CWMODE_GUARDINTERVAL == m) ? "11n-mcs-guard-intvl" : \
			(CW_MSG_ELEMENT_VENDOR_IDENTIFIER == m) ? "vendor-id" : "unknown")
			
/* Vendor Specific Payload Elements Id Values */
typedef enum 
{
	CW_VENDOR_EVENT_ELEMENT_SPECTRUM_ANALYSIS = 16,
	CW_VENDOR_EVENT_ELEMENT_CWPTK_RESULT = 17,
	CW_VENDOR_EVENT_ELEMENT_ENCRYPT_TYPE = 19,
	CW_VENDOR_EVENT_ELEMENT_STA_INFO = 20,
	CW_VENDOR_EVENT_ELEMENT_BSSID = 21,
	CW_VENDOR_EVENT_ELEMENT_AFIINFO = 22,
	CW_VENDOR_EVENT_ELEMENT_TRAFFIC = 23,
	CW_VENDOR_CONFIG_ELEMENT_WTP_BLACK_WHITE_LIST = 24,
	CW_VENDOR_CONFIG_ELEMENT_WTP_ELECTRONIC_MENU = 25,
	CW_VENDOR_CONFIG_ELEMENT_SPECTRUM_ANALYSIS = 30,
	CW_VENDOR_CONFIG_ELEMENT_LAN_VLAN = 33,
	
	CWMULKEY_UPDATE_CYCLE = 42,	
	CW_VENDOR_CONFIG_ELEMENT_PMK = 43,
	CW_VENDOR_CONFIG_ELEMENT_PMKID = 44,

	CW_VENDOR_SPEC_ELEMENT_UPGRADE_MODE = 48,
	CW_VENDOR_SPEC_ELEMENT_SUPPORT_ENCRYPTION = 49,
	CW_VENDOR_SPEC_ELEMENT_WTP_LOGIN_TYPE = 49,
	CW_VENDOR_SPEC_ELEMENT_WLAN_LOAD_BALANCE = 50,
	CW_VENDOR_SPEC_ELEMENT_WTP_ROGUE_AP_LIST = 52,
	CW_VENDOR_SPEC_ELEMENT_WLAN_CONFIG_SCHEME = 53,
	CW_VENDOR_SPEC_ELEMENT_WTP_CONTAIN_MAC = 54,
    CW_VENDOR_SPEC_ELEMENT_WLAN_PORTAL_INFO = 55,
    CW_VENDOR_SPEC_ELEMENT_WLAN_WPAE_INFO = 56,
    CW_VENDOR_SPEC_ELEMENT_WLAN_TRAFFIC_LIMIT = 57,
}CWVendorSpecificElements;

#define WAI_VENDOR_MSG_ELEMENT_TYPE_STR(m) \
		      ((CW_VENDOR_EVENT_ELEMENT_SPECTRUM_ANALYSIS == m) ? "evt-spect-analyze" : \
			(CW_VENDOR_EVENT_ELEMENT_CWPTK_RESULT == m) ? "ptk-result" : \
			(CW_VENDOR_EVENT_ELEMENT_ENCRYPT_TYPE == m) ? "encrypt-type" : \
			(CW_VENDOR_EVENT_ELEMENT_STA_INFO == m) ? "sta-info" : \
			(CW_VENDOR_EVENT_ELEMENT_BSSID == m) ? "bssid-info" : \
			(CW_VENDOR_EVENT_ELEMENT_AFIINFO == m) ? "afi-info" : \
			(CW_VENDOR_EVENT_ELEMENT_TRAFFIC == m) ? "traffic-info" : \
			(CW_VENDOR_CONFIG_ELEMENT_WTP_BLACK_WHITE_LIST == m) ? "wtp-wb-list" : \
			(CW_VENDOR_CONFIG_ELEMENT_WTP_ELECTRONIC_MENU == m) ? "elect-menu" : \
			(CW_VENDOR_CONFIG_ELEMENT_SPECTRUM_ANALYSIS == m) ? "cfg-spect-analyze" : \
			(CW_VENDOR_CONFIG_ELEMENT_LAN_VLAN == m) ? "lan-vlan" : \
			(CWMULKEY_UPDATE_CYCLE == m) ? "update-cycle" : \
			(CW_VENDOR_CONFIG_ELEMENT_PMK == m) ? "pmk" : \
			(CW_VENDOR_CONFIG_ELEMENT_PMKID == m) ? "pmkid" : \
			(CW_VENDOR_SPEC_ELEMENT_UPGRADE_MODE == m) ? "upgrade-mode" :  \
			(CW_VENDOR_SPEC_ELEMENT_SUPPORT_ENCRYPTION == m) ? "support-encrypt" : \
			(CW_VENDOR_SPEC_ELEMENT_WLAN_LOAD_BALANCE == m) ? "wlan-loadbala" :  \
			(CW_VENDOR_SPEC_ELEMENT_WTP_ROGUE_AP_LIST == m) ? "rogue-aps" : 	\
			(CW_VENDOR_SPEC_ELEMENT_WLAN_CONFIG_SCHEME == m) ? "wlan-cfg" :  \
			(CW_VENDOR_SPEC_ELEMENT_WTP_CONTAIN_MAC == m) ? "contain-mac" : 	\
			(CW_VENDOR_SPEC_ELEMENT_WLAN_PORTAL_INFO == m) ? "portal-info" :  \
			(CW_VENDOR_SPEC_ELEMENT_WLAN_WPAE_INFO == m) ? "wpae-info" :  "unknown")

enum CWImageIdentifierVendorSpecificElements
{
	CW_IMAGEIDENTIFIER_IMAGENAME = 1,
	CW_IMAGEIDENTIFIER_VERSION = 2,
	CW_IMAGEIDENTIFIER_MODE = 3
};

// CAPWAP Protocol Variables
#define		CW_MAX_RETRANSMIT_DEFAULT		5
#define 	CW_WAIT_JOIN_DEFAULT			60
#define		CW_REPORT_INTERVAL_DEFAULT		120
#define		CW_STATISTIC_TIMER_DEFAULT		120

#ifdef CW_DEBUGGING
	#define		CW_JOIN_INTERVAL_DEFAULT 	60
#else
	#define		CW_JOIN_INTERVAL_DEFAULT 	60
#endif

#ifdef CW_DEBUGGING
	#define		CW_IMAGE_INTERVAL_DEFAULT 	300
#else
	#define		CW_IMAGE_INTERVAL_DEFAULT 	300
#endif

#ifdef CW_DEBUGGING
	#define		CW_REACCESS_INTERVAL_DEFAULT 	300
#else
	#define		CW_REACCESS_INTERVAL_DEFAULT 	300
#endif


#ifdef CW_DEBUGGING
	#define		CW_CHANGE_STATE_INTERVAL_DEFAULT 10
#else
	#define		CW_CHANGE_STATE_INTERVAL_DEFAULT 25
#endif

#ifdef CW_DEBUGGING
	#define 	CW_RETRANSMIT_INTERVAL_DEFAULT		5
#else
	#define 	CW_RETRANSMIT_INTERVAL_DEFAULT		5
#endif

#ifdef CW_DEBUGGING
	#define		CW_NEIGHBORDEAD_INTERVAL_DEFAULT	60 //zhang lei time 20 to 60
#else
	#define		CW_NEIGHBORDEAD_INTERVAL_DEFAULT	60
#endif

#ifdef CW_DEBUGGING
	#define		CW_ECHO_INTERVAL_DEFAULT		10 
#else
	#define		CW_ECHO_INTERVAL_DEFAULT		10	//zhang lei time30 to 10
#endif


/*_________________________________________________________*/
/*  *******************___VARIABLES___*******************  */

/*_____________________________________________________*/
/*  *******************___TYPES___*******************  */
typedef struct {
	int type;
	int value;
} CWMsgElemData;

typedef unsigned char CWMACAddress[6];

typedef enum {
	CW_PROTOCOL_SUCCESS				= 0, //	Success
	CW_PROTOCOL_FAILURE_AC_LIST			= 1, // AC List message MUST be present
	CW_PROTOCOL_SUCCESS_NAT				= 2, // NAT detected
	CW_PROTOCOL_FAILURE				= 3, // unspecified
	CW_PROTOCOL_FAILURE_RES_DEPLETION		= 4, // Resource Depletion
	CW_PROTOCOL_FAILURE_UNKNOWN_SRC			= 5, // Unknown Source
	CW_PROTOCOL_FAILURE_INCORRECT_DATA		= 6, // Incorrect Data
	CW_PROTOCOL_FAILURE_ID_IN_USE			= 7, // Session ID Alreadyin Use
	CW_PROTOCOL_FAILURE_WTP_HW_UNSUPP		= 8, // WTP Hardware not supported
	CW_PROTOCOL_FAILURE_BINDING_UNSUPP		= 9, // Binding not supported
	CW_PROTOCOL_FAILURE_UNABLE_TO_RESET		= 10, // Unable to reset
	CW_PROTOCOL_FAILURE_FIRM_WRT_ERROR		= 11, // Firmware write error
	CW_PROTOCOL_FAILURE_SERVICE_PROVIDED_ANYHOW	= 12, // Unable to apply requested configuration 
	CW_PROTOCOL_FAILURE_SERVICE_NOT_PROVIDED	= 13, // Unable to apply requested configuration
	CW_PROTOCOL_FAILURE_INVALID_CHECKSUM		= 14, // Image Data Error: invalid checksum
	CW_PROTOCOL_FAILURE_INVALID_DATA_LEN		= 15, // Image Data Error: invalid data length
	CW_PROTOCOL_FAILURE_OTHER_ERROR			= 16, // Image Data Error: other error
	CW_PROTOCOL_FAILURE_IMAGE_ALREADY_PRESENT	= 17, // Image Data Error: image already present
	CW_PROTOCOL_FAILURE_INVALID_STATE		= 18, // Message unexpected: invalid in current state
	CW_PROTOCOL_FAILURE_UNRECOGNIZED_REQ		= 19, // Message unexpected: unrecognized request
	CW_PROTOCOL_FAILURE_MISSING_MSG_ELEM		= 20, // Failure: missing mandatory message element
	CW_PROTOCOL_FAILURE_UNRECOGNIZED_MSG_ELEM	= 21  // Failure: unrecognized message element

} CWProtocolResultCode;

typedef struct {
	char *msg;
	int offset;
	int msgLen;
} CWProtocolMessage;

#define 	MAX_PENDING_REQUEST_MSGS	20/*caosicong 080705 change 15 to 20*/
#define		UNUSED_MSG_TYPE			0

typedef struct
{
	unsigned char msgType;
	unsigned char seqNum;
	int timer_sec;
	void (*timer_hdl)(void *);
	CWTimerArg timer_arg;
	CWTimerID timer;
	int retransmission;
	CWProtocolMessage *msgElems;
	int fragmentsNum;
} CWPendingRequestMessage;

#include "CWBinding.h"

typedef struct {
	int payloadType;
	int type;
	int isFragment;
	int last;
	int fragmentID;
	int fragmentOffset;
	int isContainMac;
	int keepAlive;
	int protocolver;
	unsigned char wtp_mac[6];
	CWBindingTransportHeaderValues *bindingValuesPtr;
} CWProtocolTransportHeaderValues;

typedef struct
{
	int protocolver;
	unsigned int message_type;  
	unsigned int mac_exist;
	unsigned char seqNum;
	unsigned char wtp_mac[MAC_LEN];
	WTPLoginState wtp_contype;
}CWProtocolHeaderInfo;

typedef struct {
	unsigned int messageTypeValue;
	unsigned char seqNum;
	unsigned short msgElemsLen;
//	unsigned int timestamp;
} CWControlHeaderValues;

typedef struct {
	char *data;
	int dataLen;
	CWProtocolTransportHeaderValues transportVal;
} CWProtocolFragment;

typedef struct {
	int vendorIdentifier;
	enum {
		CW_WTP_MODEL_NUMBER	= 0,
		CW_WTP_SERIAL_NUMBER	= 1,
		CW_BOARD_ID		= 2,
		CW_BOARD_REVISION	= 3,
		CW_BOARD_MAC_ADDRESS	= 4,
		CW_WTP_REAL_MODEL_NUMBER	= 5, 
		CW_WTP_CODE_VERSION	= 6,
		CW_WTP_MANUFACTURE_OPTION = 7,//value 0 manufacture is autelen value is 1 is OEM
		
		CW_WTP_HARDWARE_VERSION	= 0,
		CW_WTP_SOFTWARE_VERSION	= 1,
		CW_BOOT_VERSION		= 2
	} type;
	int length;
	int *valuePtr;
	unsigned char *model;
	unsigned char *SN;
	unsigned char *mac;
	unsigned char *ver;/* sw version */
	unsigned char *codever;
	unsigned char *sysver;/* hw version */
	unsigned char *Rmodel;
	int manuoption;//0 autelan made 1 oem vendor made
	unsigned char *bootver; /* boot version */
} CWWTPVendorInfoValues;

typedef struct  {
	int vendorInfosCount;
	CWWTPVendorInfoValues *vendorInfos;
} CWWTPVendorInfos;

typedef struct {
	int maxRadios;
	int radiosInUse;
	int encCapabilities;
	CWWTPVendorInfos vendorInfos;
} CWWTPDescriptor;

typedef enum {
	CW_LOCAL_BRIDGING = 1,
	CW_802_DOT_3_TUNNEL = 2,
	CW_802_DOT_11_TUNNEL = 4,
	CW_ALL_ENC = 7,
	CW_802_IPIP_TUNNEL = 8
} CWframeTunnelMode;

typedef enum {
	CW_LOCAL_MAC = 0,
	CW_SPLIT_MAC = 1,
	CW_BOTH = 2
} CWMACType;

typedef struct {
	enum {
		CW_MSG_ELEMENT_DISCOVERY_TYPE_BROADCAST = 0,
		CW_MSG_ELEMENT_DISCOVERY_TYPE_CONFIGURED = 1
	} type;
	CWWTPVendorInfos WTPBoardData;
	CWWTPDescriptor WTPDescriptor;
	CWframeTunnelMode frameTunnelMode;
	CWMACType MACType;
	WTPLoginState Connect_status;
	//CWRadiosInformation radios;
} CWDiscoveryRequestValues;

typedef enum {
	CW_X509_CERTIFICATE = 1,
	CW_PRESHARED = 0
} CWAuthSecurity;

typedef struct {
	int enable;
	CWNetworkLev4Address addr;
	struct sockaddr_in addrIPv4;
	struct sockaddr_in6 addrIPv6;

	int WTPCount;
	int tcpdumpflag;
	int datacount;
	int times;
	char ip[128];
	char ifname[16];
} CWProtocolNetworkInterface;

typedef struct {
	int WTPCount;
	struct sockaddr_in addr;
} CWProtocolIPv4NetworkInterface;

typedef struct {
	int WTPCount;
	struct sockaddr_in6 addr;
} CWProtocolIPv6NetworkInterface;

typedef struct {
	int vendorIdentifier;
	enum {
		CW_AC_HARDWARE_VERSION	= 4,
		CW_AC_SOFTWARE_VERSION	= 5
	} type;
	int length;
	int *valuePtr;
} CWACVendorInfoValues;

typedef struct  {
	int vendorInfosCount;
	CWACVendorInfoValues *vendorInfos;
} CWACVendorInfos;

typedef struct {
	int rebootCount;
	int ACInitiatedCount;
	int linkFailurerCount;
	int SWFailureCount;
	int HWFailuireCount;
	int otherFailureCount;
	int unknownFailureCount;
	enum {
		NOT_SUPPORTED=0,
		AC_INITIATED=1,
		LINK_FAILURE=2,
		SW_FAILURE=3,
		HD_FAILURE=4,
		OTHER_FAILURE=5,
		UNKNOWN=255
	}lastFailureType;
}WTPRebootStatisticsInfo;

typedef struct
{
	int radioID;
	int reportInterval;
}WTPDecryptErrorReportValues;

typedef struct
{
	int radiosCount;
	WTPDecryptErrorReportValues *radios;
}WTPDecryptErrorReport;

typedef struct {
	int index;
	char *ACName;
}CWACNameWithIndexValues;

typedef struct {
	int count;
	CWACNameWithIndexValues *ACNameIndex;
}CWACNamesWithIndex;

typedef struct {
	int ID;
	enum {
		CW_802_DOT_11b = 1,
		CW_802_DOT_11a = 2,
		CW_802_DOT_11g = 4,
		CW_802_DOT_11n = 8,
		CW_ALL_RADIO_TYPES = 0x0F
	} type;
} CWRadioInformationValues;

typedef struct {
	int radiosCount;
	CWRadioInformationValues *radios;
} CWRadiosInformation;

typedef enum {
	ENABLED = 1,
	DISABLED = 2
} CWstate;
#define WAI_RADIO_ADM_STR(m)	\
		((ENABLED == m) ? "enabled" :\
		(DISABLED == m) ? "disabled":"ivalid")
		
typedef enum {
	AD_NORMAL = 1,
	AD_RADIO_FAILURE = 2,
	AD_SOFTWARE_FAILURE = 3,
	AD_RADAR_DETECTION = 4
} CWAdminCause;

typedef enum {
	OP_NORMAL = 0,
	OP_RADIO_FAILURE = 1,
	OP_SOFTWARE_FAILURE = 2,
	OP_ADMINISTRATIVELY_SET = 3
} CWOperationalCause;

typedef struct {
	int ID;
	CWstate state;
	CWAdminCause cause;
} CWRadioAdminInfoValues;

typedef struct {
	int radiosCount;
	CWRadioAdminInfoValues *radios;
} CWRadiosAdminInfo;

typedef struct {
	int ID;
	CWstate state;
	CWOperationalCause cause;
} CWRadioOperationalInfoValues;

typedef struct {
	int radiosCount;
	CWRadioOperationalInfoValues *radios;
} CWRadiosOperationalInfo;

typedef struct {
	int ID;
	unsigned char numEntries;
	unsigned char length;
	CWMACAddress *decryptErrorMACAddressList;
} CWDecryptErrorReportValues;

typedef struct {
	int radiosCount;
	CWDecryptErrorReportValues *radios;
} CWDecryptErrorReportInfo;

enum failure_type_e {
	STATISTICS_NOT_SUPPORTED=0,
	SW_FAILURE_TYPE=1,
	HD_FAILURE_TYPE=2,
	OTHER_TYPES=3,
	UNKNOWN_TYPE=255
};
typedef struct {
	enum failure_type_e lastFailureType;
	short int resetCount;
	short int SWFailureCount;
	short int HWFailuireCount;
	short int otherFailureCount;
	short int unknownFailureCount;
	short int configUpdateCount;
	short int channelChangeCount;
	short int bandChangeCount;
	short int currentNoiseFloor;
}WTPRadioStatisticsInfo;

#define WAI_RADIO_FAIL_TYPE_STR(e) \
	((STATISTICS_NOT_SUPPORTED == e) ? "not support" : \
	  (SW_FAILURE_TYPE == e) ? "sw fail": \
	  (HD_FAILURE_TYPE == e) ? "hw fail": \
	  (OTHER_TYPES == e) ? "other": "unkown")

typedef struct {
	unsigned int radioID;
	//Station Mac Address List

	CWList decryptErrorMACAddressList;

	unsigned int reportInterval;
	
	CWstate adminState;
	CWAdminCause adminCause;

	CWstate operationalState;
	CWOperationalCause operationalCause;

	unsigned int TxQueueLevel;
	unsigned int wirelessLinkFramesPerSec;

	WTPRadioStatisticsInfo statistics;	
	
	void* bindingValuesPtr;
} CWWTPRadioInfoValues;

typedef struct {
	int radioCount;
	CWWTPRadioInfoValues *radiosInfo;
} CWWTPRadiosInfo;

#include "CWList.h"

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */

__inline__ unsigned int CWGetSeqNum(); // provided by the user of CWProtocol lib
__inline__ int CWGetFragmentID(); // provided by the user of CWProtocol lib

void CWWTPResetRadioStatistics(WTPRadioStatisticsInfo *radioStatistics);

void CWProtocolDestroyMsgElemData(void *f);
void CWFreeMessageFragments(CWProtocolMessage* messages, int fragmentsNum);

void CWProtocolStore8(CWProtocolMessage *msgPtr, unsigned char val);
void CWProtocolStore16(CWProtocolMessage *msgPtr, unsigned short val);
void CWProtocolStore32(CWProtocolMessage *msgPtr, unsigned int val);
void CWProtocolStoreStr(CWProtocolMessage *msgPtr, char *str);
void CWProtocolStoreMessage(CWProtocolMessage *msgPtr, CWProtocolMessage *msgToStorePtr);
void CWProtocolStoreRawBytes(CWProtocolMessage *msgPtr, char *bytes, int len);

unsigned char CWProtocolRetrieve8(CWProtocolMessage *msgPtr);
unsigned short CWProtocolRetrieve16(CWProtocolMessage *msgPtr);
unsigned int CWProtocolRetrieve32(CWProtocolMessage *msgPtr);
char *CWProtocolRetrieveStr(CWProtocolMessage *msgPtr, int len);
char *CWProtocolRetrieveRawBytes(CWProtocolMessage *msgPtr, int len);
unsigned char CWProtocolPeep8(CWProtocolMessage *msgPtr);
unsigned short CWProtocolPeep16(CWProtocolMessage *msgPtr);

const char *CWParse_inet_ntoa(unsigned int address, char *addr_p);

CWBool CWProtocolParseFragment(char *buf, int readBytes, CWList *fragmentsListPtr, CWProtocolMessage *reassembledMsg, CWBool *dataFlag);
//luoxun 2008-4-26
CWBool CWProtocolParseFragment_GetCAPWAPheadInfo(char *buf, int readBytes, CWList *fragmentsListPtr, CWProtocolMessage *reassembledMsg, CWBool *dataFlagPtr, CWProtocolTransportHeaderValues *pCAPWAPheadvalue);
void CWProtocolDestroyFragment(void *f);

CWBool CWParseTransportHeader(CWProtocolMessage *msgPtr, CWProtocolTransportHeaderValues *valuesPtr, CWBool *dataFlag);
CWBool CWParseControlHeader(CWProtocolMessage *msgPtr, CWControlHeaderValues *valPtr);
CWBool CWGetProtocolHeaderInfo(char *msg, CWProtocolHeaderInfo *info);
CWBool CWParseFormatMsgElem(CWProtocolMessage *completeMsg,unsigned short int *type,unsigned short int *len);

CWBool CWAssembleTransportHeader(CWProtocolMessage *transportHdrPtr, CWProtocolTransportHeaderValues *valuesPtr);
CWBool CWAssembleControlHeader(CWProtocolMessage *controlHdrPtr, CWControlHeaderValues *valPtr);
CWBool CWAssembleMessage(CWProtocolMessage **completeMsgPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int msgTypeValue, CWProtocolMessage *msgElems, const int msgElemNum, CWProtocolMessage *msgElemsBinding, const int msgElemBindingNum, int is_crypted, int cap_pro_ver);
CWBool CWAssembleMsgElem(CWProtocolMessage *msgPtr, unsigned int type);
CWBool CWAssembleUnrecognizedMessageResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int msgType, int WTPIndex);

CWBool CWAssembleMsgElemRadioAdminState(CWProtocolMessage *msgPtr);			//29
CWBool CWAssembleMsgElemRadioOperationalState(int radioID, CWProtocolMessage *msgPtr);	//30
CWBool CWAssembleMsgElemResultCode(CWProtocolMessage *msgPtr,CWProtocolResultCode code);//31
CWBool CWAssembleMsgElemSessionID(CWProtocolMessage *msgPtr, int sessionID);		//32

CWBool CWParseACName(CWProtocolMessage *msgPtr, int len, char **valPtr);
CWBool CWParseWTPRadioOperationalState (CWProtocolMessage *msgPtr, int len, CWRadioOperationalInfoValues *valPtr);	//30
CWBool CWParseResultCode(CWProtocolMessage *msgPtr, int len, CWProtocolResultCode *valPtr);			//31
CWBool CWAssembleMsgElemVendor(CWProtocolMessage *msgPtr, unsigned int type);
CWBool CWAssembleMsgElemAddVendor(CWProtocolMessage *msgPtr, unsigned int type) ;
CWBool CWAssembleMsgElemVendorOption60(CWProtocolMessage *msgPtr, unsigned int type);
char *CWProtocolGetRawBytes(CWProtocolMessage *msgPtr, char *val, int len);
#endif
