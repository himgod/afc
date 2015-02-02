#ifndef _DCLI_WID_WLAN_H
#define _DCLI_WID_WLAN_H

#define WLAN_IF_NAME_LEN 20

/*------------------------liuzhenhua append begin---------------------*/
/*2010-05-20*/
struct SSIDConfigInfo {
	
	long  wlanCurrID;		//WLANID����
	char *NewSSIDName;		//WLANID���� 
	long NewSSIDEnabled;		//WLAN�Ƿ����� {0,1}=wlan.status{1,x}
	long NewSSIDHidden;		//WLAN�Ƿ����� wlan.HideESSid
	long NewStaIsolate;		//��������Ƿ���

	long NewDot11Auth;		//802.11��Ȩ
	long Newsecurity;		//��ȫ����
	long NewAuthenMode;		//��֤ģʽ

	long NewSecurityCiphers;	//��������
	long NewEncrInputType;		//��Կ��������
	char *NewSecurityKEY;		//��Կֵ
	long NewExtensibleAuth;		//��չ��ֵ֤
	char *NewAuthIP;	//��֤������IP
	long NewAuthPort;		//��֤�������˿�
	char *NewAuthSharedSecret;	//��֤��������Կֵ
	char *NewAcctIP;	//�Ʒѷ�����IP
	long NewAcctPort;		//�Ʒѷ������˿�
	char *NewAcctSharedSecret;	//�Ʒѷ�������Կֵ
	long NewVlanId;			//VLAN��ʶ
	long NewMaxSimultUsers;		//���֧�ֵ��ն�����wlan.wlan_traffic_limit;
	long NewStaUplinkMaxRate;	//�ն������������ wlan.wlan_traffic_limit;
	long NewStaDwlinkMaxRate;	//�ն������������wlan.wlan_send_traffic_limit
	long SSIDRowStatus;		//������  1

	unsigned int SecurityID; // append for High efficiency
	unsigned int sta_aged;
	unsigned int authentication_aged;
	char *asip;
	long cert_type;
	char *as_path;
	char *ae_path;
	struct SSIDConfigInfo *next;
};
/*------------------------liuzhenhua append begin---------------------*/

void dcli_wlan_init(void);
int parse_char_ID(char* str,unsigned char* ID);
void CheckWIDIfPolicy(char *whichinterface, unsigned char wlan_if_policy);

#define		DCLI_FORMIB_FREE_OBJECT(obj_name)				{if(obj_name){free((obj_name)); (obj_name) = NULL;}}

struct ConfigWapiInfo {
	unsigned char  WlanId;		       //WAPI SSID
	unsigned char  SecurityID;              //�����İ�ȫ���Ե�ID
	char *WAPIASIPAddress; //Wapi IP��ַ
	unsigned int WapiCipherKeyCharType;    //wapi��������
	
		
    struct ConfigWapiInfo *next;
	struct ConfigWapiInfo *ConfigWapiInfo_list;
	struct ConfigWapiInfo *ConfigWapiInfo_last;
};
/*table 21-2*/
struct Sub_Sta_WtpWAPIPerformance {
	unsigned char *staMacAddr;					//station��MAC��ַ
	int sta_seq;
	unsigned int wtpWapiVersion;					//��վ��WAPI�汾��
	unsigned char wtpWapiControlledPortStatus;			//��Ȩ���ƶ˿ڵ�״̬
	unsigned char wtpWapiSelectedUnicastCipher[4];			//ѡ��ĵ��������׼�
	
	unsigned int wtpWapiWPIReplayCounters;			//������Ӧ���ƶ�������WPI?MPDU����
	unsigned int wtpWapiWPIDecryptableErrors;		//������Կ��Ч��������WPI?MPDU����
	unsigned int wtpWapiWPIMICErrors;			//����MICУ��ʧ�ܶ�������WPI?MPDU����
	unsigned int wtpWapiWAISignatureErrors;		//ǩ�������WAI���ݰ�������
	unsigned int wtpWapiWAIHMACErrors;			//��Ȩ��ԿУ��ʧ�ܵ�WAI���ݰ�������
	unsigned int wtpWapiWAIAuthResultFailures;		//WAI��Ȩ���ʧ�ܵ�����
	unsigned int wtpWapiWAIDiscardCounters;		//WAI���ݰ�����������
	unsigned int wtpWapiWAITimeoutCounters;		//WAI���ݰ���ʱ������
	unsigned int wtpWapiWAIFormatErrors;			//WAI���ݰ���ʽ���������
	unsigned int wtpWapiWAICertificateHandshakeFailures;	//WAI��֤����ʧ�ܵ�����
	unsigned int wtpWapiWAIUnicastHandshakeFailures;	//WAI������ԿЭ��ʧ�ܵ�����
	unsigned int wtpWapiWAIMulticastHandshakeFailures;     //WAI�ಥ��Կʧ�ܵ�����
	
			
    struct Sub_Sta_WtpWAPIPerformance *next;
	struct Sub_Sta_WtpWAPIPerformance *Sub_Sta_WtpWAPIPerformance_list;
	struct Sub_Sta_WtpWAPIPerformance *Sub_Sta_WtpWAPIPerformance_last;
	
};

/*table 21*/
struct WtpWAPIPerformanceStatsInfo {
	unsigned char  WlanId;
	unsigned int SecurityType;
	unsigned int wlan_bss_num;			//for showing
	int wlan_total_sta_num ;			//for showing
	unsigned char  SecurityID;              //�����İ�ȫ���Ե�ID

	struct Sub_Sta_WtpWAPIPerformance *Sub_Sta_WtpWAPIPerformance_head;
    struct WtpWAPIPerformanceStatsInfo *next;
	struct WtpWAPIPerformanceStatsInfo *WtpWAPIPerformanceStatsInfo_list;
	struct WtpWAPIPerformanceStatsInfo *WtpWAPIPerformanceStatsInfo_last;
	
};

struct WtpWAPIExtendConfigInfo {
								
	unsigned char  WapiWlanID;				//Wapi��ȫ���԰󶨵�WLANID��������				//
	unsigned char  SecurityID;				//WLAN�󶨵İ�ȫ����ID								//
	unsigned int wtpWapiConfigVersion;			//֧�ֵ�WAPI ��߰汾��						//Ĭ��1
	unsigned int SecurityType;			//for showting     

	unsigned char wtpWapiControlledAuthenControlenabled; //�Ƿ����ü�Ȩ								//
	char *wtpWapiControlledPortControl; 	//�˿ڵĿ�������							//Ĭ��"auto"
	unsigned int wtpWapiWPIOptionImplement; 		//�Ƿ�֧��WAPI							//1
	
	unsigned int wtpWapiWPIPreauthImplemented;		//�Ƿ�֧��WAPIԤ��Ȩ							//
	unsigned int wtpWapiEnabled;				//�Ƿ�����WAPI										//
	unsigned int wtpWapiPreauthEnabled; 		//�Ƿ�����WAPIԤ��Ȩ								//
	
	unsigned int wtpWapiUnicastKeysSupported;	//֧�ֵ�WAPI ������Կ��				//Ĭ��2
	unsigned char wtpWapiUnicastRekeyMethod; 		//������Կ���»���								//
	unsigned int wtpWapiUnicastRekeyTime;		//������Կ��Чʱ��									//
	unsigned int wtpWapiUnicastRekeyPackets;	//������Կ��Ч�����ݰ�����						//
	char wtpWapiMulticastCipher[4];			//�ಥ�����׼�													//
	unsigned char wtpWapiMulticastRekeyMethod;		//�鲥��Կ���»���								//
	unsigned int wtpWapiMulticastRekeyTime;	//�鲥��Կ��Чʱ��										//
	unsigned int wtpWapiMulticastRekeyPackets; //�鲥��Կ��Ч�����ݰ�����							//
	unsigned char wtpWapiMulticastRekeyStrict;		//�Ƿ����ն��뿪BSS ������鲥��Կ		//
	
	char *wtpWapiPSKValue;				//PSKֵ
	char *wtpWapiPSKPassPhrase; 		//��������PSK�� pass-phrase
	unsigned int wtpWapiCertificateUpdateCount;	//��Ȩ���ֵ����Դ���									//
	unsigned int wtpWapiMulticastUpdateCount;	//MSK ���ֵ����Դ���									//
	unsigned int wtpWapiUnicastUpdateCount;	//������Կ���ֵ����Դ���								//
	unsigned int wtpWapiMulticastCipherSize;	//�鲥��Կ��bit����						//Ĭ��512
	unsigned int wtpWapiBKLifetime;		//�㲥��Կ(BK)����Ч��										//
	unsigned int wtpWapiBKReauthThreshold; 	//BK���¼�Ȩ������ʱ��									//
	unsigned int wtpWapiSATimeout; 		//������ȫ���ӵ����ʱ��									//

	
	char wtpWapiAuthSuiteSelected[4]; 		//ѡ���AKM									//Ĭ��
	char *wtpWapiUnicastCipherSelected; 	//��һ������ѡ��ļ�������			//Ĭ��
	char *wtpWapiMulticastCipherSelected;		//��һ���鲥ѡ��ļ�������		//Ĭ��
	unsigned char wtpWapiBKIDUsed[16];				//��һ��ʹ�õ�BK ID								//Ĭ��
	char wtpWapiAuthSuiteRequested[4];		//��һ��AKM �����ѡ��					//Ĭ��
	char *wtpWapiUnicastCipherRequested;		//��һ���������������ѡ��		//Ĭ��
	char *wtpWapiMulticastCipherRequested;		//��һ���鲥���������ѡ��		//Ĭ��
	
			
	struct WtpWAPIExtendConfigInfo *next;
	struct WtpWAPIExtendConfigInfo *WtpWAPIExtendConfigInfo_list;
	struct WtpWAPIExtendConfigInfo *WtpWAPIExtendConfigInfo_last;
};

struct UnicastInfo {

	unsigned char  SecurityID;                    //�����İ�ȫ����ID
	unsigned char  UnicastWlanID;                 //����Wlan ID
	unsigned int SecurityType;					//��������
	char *NewUnicastCipher;              //��������		//mib��Ĭ�ϵ�
	long NewUnicastCipherEnabled;        //����������Ч//�����
	unsigned long NewUnicastCipherSize;  //�������볤��//512
		
    struct UnicastInfo *next;
	struct UnicastInfo *UnicastInfo_list;
	struct UnicastInfo *UnicastInfo_last;
};

/*table 28-2*/
struct Sub_BssWAPIPerformanceStatsInfo {	
	unsigned char *wtpMacAddr;					//AP��MAC��ַ
	unsigned char *wtpBssCurrID;					//AP��bssID

	unsigned char bss_id;					//for showwting
	unsigned char wlan_id;					//for showwting
	unsigned int wtp_id;					//for showwting
	
	unsigned int bssWapiControlledPortStatus;			//��Ȩ���ƶ˿ڵ�״̬			//Ĭ�� 0
	unsigned char bssWapiSelectedUnicastCipher[4];			//ѡ��ĵ��������׼�		//mib Ĭ���Լ���{0x00, 0x14, 0x72, 0x01};
	
	unsigned int bssWapiWPIReplayCounters;			//������Ӧ���ƶ�������WPI?MPDU����
	unsigned int bssWapiWPIDecryptableErrors;		//������Կ��Ч��������WPI?MPDU����
	unsigned int bssWapiWPIMICErrors;			//����MICУ��ʧ�ܶ�������WPI?MPDU����
	unsigned int bssWapiWAISignatureErrors;		//ǩ�������WAI���ݰ�������
	unsigned int bssWapiWAIHMACErrors;			//��Ȩ��ԿУ��ʧ�ܵ�WAI���ݰ�������
	unsigned int bssWapiWAIAuthResultFailures;		//WAI��Ȩ���ʧ�ܵ�����
	unsigned int bssWapiWAIDiscardCounters;		//WAI���ݰ�����������
	unsigned int bssWapiWAITimeoutCounters;		//WAI���ݰ���ʱ������
	unsigned int bssWapiWAIFormatErrors;			//WAI���ݰ���ʽ���������
	unsigned int bssWapiWAICertificateHandshakeFailures;	//WAI��֤����ʧ�ܵ�����
	unsigned int bssWapiWAIUnicastHandshakeFailures;	//WAI������ԿЭ��ʧ�ܵ�����
	unsigned int bssWapiWAIMulticastHandshakeFailures;	//WAI�ಥ��Կʧ�ܵ�����
										    
    struct Sub_BssWAPIPerformanceStatsInfo *next;
	struct Sub_BssWAPIPerformanceStatsInfo *Sub_BssWAPIPerformanceStatsInfo_list;
	struct Sub_BssWAPIPerformanceStatsInfo *Sub_BssWAPIPerformanceStatsInfo_last;
};
/*table 28*/
struct BssWAPIPerformanceStatsInfo {	
	unsigned char WlanID;
	unsigned char  SecurityID;
	unsigned int SecurityType;	
	int wlan_bss_num;

	
	struct Sub_BssWAPIPerformanceStatsInfo	*Sub_BssWAPIPerformanceStatsInfo_head;
    struct BssWAPIPerformanceStatsInfo *next;
	struct BssWAPIPerformanceStatsInfo *BssWAPIPerformanceStatsInfo_list;
	struct BssWAPIPerformanceStatsInfo *BssWAPIPerformanceStatsInfo_last;
};

#define		DCLI_FORMIB_FREE_OBJECT(obj_name)				{if(obj_name){free((obj_name)); (obj_name) = NULL;}}
#endif

