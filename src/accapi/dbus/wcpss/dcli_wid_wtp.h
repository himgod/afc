#ifndef DCLI_WID_WTP_H
#define DCLI_WID_WTP_H

/*nl add for mib showting wtp  infromation begin*/
/*------------------------------------------------*/
#define		DCLI_FORMIB_FREE_OBJECT(obj_name)				{if(obj_name){free((obj_name)); (obj_name) = NULL;}}

/* for mib showting wtp basic infromation*/
struct WtpBasicInfo{
    int wtpCurrID;
    unsigned char *wtpMacAddr;				//AP��MAC��ַ
    char *wtpModel;				//APģʽ	
    char *wtpDevName;				//AP����
    char *wtpPosInfo;				//APλ����Ϣ
    char *wtpProduct;				//AP������
    char *wtpDevTypeNum;			//AP����
    char *wtpVersionInfo;			//AP����汾
    unsigned long wtpUpTime;			//AP����ʱ��
    unsigned long wtpOnlineTime;		//AP����ʱ��
    char *wtpSysSoftName;			//AP�������
    char *wtpSysVersion;			//APӲ���汾
    char *wtpSeriesNum;				//AP SN
    char *wtpSysSoftProductor;			//AP���������
    char *wtpDomain;				//AP Domian
    long wtpSysRestart;				//APϵͳ����
    long wtpSysReset;				//AP ϵͳ����
    long wtpColdReboot;				//AP������
    unsigned long  Mib_wtpUpTime;			//AP��������ʱ�䳤��
    unsigned int ElectrifyRegisterCircle;
	unsigned int acNeighbordeadTimes;
    struct WtpBasicInfo *next;
    struct WtpBasicInfo *WtpBasicInfo_list;
    struct WtpBasicInfo *WtpBasicInfo_last;
};
/*for mib showting wtp collect infromation*/
struct WtpCollectInfo{
    int  wtpCurrID;				
    unsigned char *wtpMacAddr;
    unsigned int wtpRtCollectOnOff;			//AP ʵʱ�ɼ�����
    unsigned int CPURTUsage;				//AP CPUʵʱ������
    unsigned char MemRTUsage;				//AP�ڴ�ʵʱ������info.memoryuse
    unsigned int wtpAssocTimes;		//AP �����ܴ���
    unsigned int wtpAssocFailtimes;		//AP ����ʧ�ܴ���
    unsigned int wtpReassocTimes;		//AP�ع�������
    unsigned int wtpIfIndiscardPkts;		//AP ���߶˿ڽ��յĶ�������exten_info.rx_drop
    unsigned long long  wtpStaTxBytes;		//AP���ж˿����߽��ܵ����ֽ���
    unsigned long long  wtpStaRxBytes;		//AP ���ж˿����߷��͵����ֽ���

    unsigned int wtpReassocFailure;		//AP�ع���ʧ�ܴ���//20100721   //copy from 1.2 by zhangshu  0909
    unsigned int wtpSuccAssociatedNum;	//assoc successfully times, xiaodawei add, 20110418
    unsigned char wtpMomentCollectSwith;/*AP collect switch for lixiang's mib table by nl 20100823*/ //copy from 1.2 by zhangshu  0909
    
    struct WtpCollectInfo *next;
	struct WtpCollectInfo *WtpCollectInfo_list;
    struct WtpCollectInfo *WtpCollectInfo_last;
};
/*for mib showting WtpParaInfo*/

struct WtpParaInfo{
	int wtpCurrID;
	unsigned char *wtpMacAddr;
	char *wtpModel;				
	unsigned char *wtpCurBssid;			//AP��ǰ��BSSID
	char *wtpGateAddr;		//AP���ص�ַ
	char *wtpNetElementCode;		//AP��Ԫ����
	char *wtpAddrMask;		//AP��ַ����
	unsigned char wtpRemoteRestartFun;		//APԶ����������
	unsigned int wtpReceiverSignalPWL;		//AP�����ź�ǿ��
	unsigned char *wtpMacConApAc;		//AP��MAC��ַ
	unsigned char wtpState;				//AP״̬
	char *wtpIP;			//AP��IP��ַ//20100714
	char *wtpName;			//AP������//20100722
	unsigned char wtpDefenceDOSAttack;		//AP��DOS����
	unsigned char wtpIGMPSwitch;			//AP��IGMP����
	unsigned char wtpCurrAPMode;			//AP����ģʽ��������������ء����أ�
	
	unsigned int wtpCurBssidNumber;					//ssid number of one wtp  
	// copy from 1.2omc by zhangshu   2010-09-09
	unsigned char total_if_num; /*add for mib require total if num : wifi_num + eth_num*/

	char *wtpIfType;			//AP֧�ֵĽӿ�����
	long wtpWorkMode;			//AP����ģʽ
	unsigned int wtpBridgingWorkMode;		//AP�Ź���ģʽ
	unsigned int wtpsupport11b;			//AP�Ƿ�֧��802.11B
	unsigned int wtpsupport11g;			//AP�Ƿ�֧��802.11G

	//xiaodawei transplant from 2.0 for telecom test, 20110302
	time_t add_time;
	time_t ElectrifyRegisterCircle;
	time_t imagadata_time;
	time_t config_update_time;
	time_t starttime;
	//END
	struct WtpParaInfo *next;
	struct WtpParaInfo *WtpParaInfo_list;
	struct WtpParaInfo *WtpParaInfo_last;
};

/* book modify, 2011-1-19 */
struct WtpWirelessIfstatsInfo_radio{

	unsigned char wtpWirelessIfIndex;				//AP���߽ӿ�����
	unsigned char wirelessIfUpdownTimes;				//AP���߽ӿ�UpDown����

	unsigned int wirelessIfChStatsUplinkUniFrameCnt;		//�����ŵ����е�����֡��
	unsigned int wirelessIfChStatsDwlinkUniFrameCnt;		//�����ŵ����е�����֡��
	unsigned int wirelessIfUpChStatsFrameNonUniFrameCnt;	//�����ŵ����зǵ�����֡��
	unsigned int wirelessIfDownChStatsFrameNonUniFrameCnt;	//�����ŵ����зǵ���֡��

	unsigned int wirelessIfApChStatsFrameFragRate;		//�ŵ���֡�ķֶ�����
	unsigned int wirelessIfChStatsNumStations;			//ʹ�ø��ŵ����ն���

	unsigned int wirelessIfRxMgmtFrameCnt;			//���յ��Ĺ���֡������
	unsigned int wirelessIfRxCtrlFrameCnt;			//���յ��Ŀ���֡������
	unsigned int wirelessIfRxDataFrameCnt;			//���յ�������֡������
	unsigned int wirelessIfRxAuthenFrameCnt;			//���յ�����֤֡������
	unsigned int wirelessIfRxAssociateFrameCnt; 		//���յ��Ĺ���֡������
	unsigned int wirelessIfTxMgmtFrameCnt;			//���͵Ĺ���֡������
	unsigned int wirelessIfTxCtrlFrameCnt;			//���͵Ŀ���֡������
	unsigned int wirelessIfTxDataFrameCnt;			//���͵�����֡������
	unsigned int wirelessIfTxAuthenFrameCnt;			//���͵���֤֡������
	unsigned int wirelessIfTxAssociateFrameCnt; 		//���͵Ĺ���֡������


	unsigned int sub_wirelessIfRxDataPkts;				//add to sub radio c1
	unsigned int sub_wirelessIfTxDataPkts;
	unsigned int sub_wirelessIfChStatsFrameErrorCnt;
	unsigned long sub_wirelessIfRxErrPkts;				//xiaodawei add rx error pkts, 20110406
	unsigned long sub_wirelessIfTxDropPkts;				//xiaodawei add tx drop pkts, 20110406
	unsigned long sub_wirelessIfRxDropPkts;				//xiaodawei add rx drop pkts, 20110406
	unsigned long long  sub_wirelessIfDwlinkDataOctets;
	unsigned long long  sub_wirelessIfUplinkDataOctets;
	unsigned int sub_wirelessIfChStatsMacFcsErrPkts;
	unsigned int sub_wirelessIfChStatsMacDecryptErrPkts;
	unsigned int sub_wirelessIfChStatsMacMicErrPkts;
	unsigned int sub_wirelessIfChStatsPhyErrPkts;			//add to sub radio c9

	unsigned char radio_cur_snr ;				//snr ǿ��nl 20100723
	unsigned char radio_max_snr;				//snr maxǿ��nl20100723
	unsigned char radio_min_snr;				//snr minǿ��nl20100723
	double radio_aver_snr;				//snr averǿ��nl20100723

    char radio_cur_snr2 ;				//snr ǿ��nl 20100907
	char radio_max_snr2;				//snr maxǿ��nl 20100907
	char radio_min_snr2;				//snr minǿ��nl 20100907
	
	/*add for new mib requirement 20100809*/  //zhangshu copy from 1.2omc 0909
	unsigned int sub_rx_pkt_mgmt ;  // packets received of management
	unsigned int sub_tx_pkt_mgmt ; // packets transtmitted of management
	unsigned long long sub_rx_mgmt ;
	unsigned long long sub_tx_mgmt ;
	unsigned long long sub_total_rx_bytes ;
	unsigned long long sub_total_tx_bytes ;
	unsigned long long sub_total_rx_pkt ;
	unsigned long long sub_total_tx_pkt ;

    /* book modify, 2011-5-19 */
	unsigned int wirelessIfTxSignalPkts;			//���͵��������
    unsigned int wirelessIfRxSignalPkts;			//���յ��������
    unsigned int wirelessIfChStatsDwlinkTotRetryPkts;		//�����ŵ������ش��İ���	//only wtp
    unsigned int wirelessIfChStatsFrameRetryCnt;		//�ŵ������ش���֡��				//only wtp
	
	struct WtpWirelessIfstatsInfo_radio *next;
	struct WtpWirelessIfstatsInfo_radio *WtpWirelessIfstatsInfo_radio_list;
	struct WtpWirelessIfstatsInfo_radio *WtpWirelessIfstatsInfo_radio_last;
};

struct WtpWirelessIfstatsInfo {		//aaaa
    int wtpCurrID;
    unsigned char *wtpMacAddr;
	unsigned char wtp_radio_num;
    unsigned char wirelessIfAvgRxSignalStrength;			//AP���߽ӿڽ��յ��ź�ƽ��ǿ��	//nnnn
    unsigned char wirelessIfHighestRxSignalStrength;		//AP���߽ӿڽ����źŵ����ǿ��	//nnnn
    unsigned char wirelessIfLowestRxSignalStrength;		//AP���߽ӿڽ����źŵ����ǿ��		//nnnn
    unsigned char wifiExtensionInfoReportswitch;		/*set ap extension infomation switch (enable|disable)*/	//nnnn
    double 	math_wirelessIfAvgRxSignalStrength;			//AP���߽ӿڽ��յ��ź�����ƽ��ǿ��	//nnnn

    char wirelessIfAvgRxSignalStrength2;			//AP���߽ӿڽ��յ��ź�ƽ��ǿ��	//add 2010-09-07
    char wirelessIfHighestRxSignalStrength2;		//AP���߽ӿڽ����źŵ����ǿ��	//add 2010-09-07
    char wirelessIfLowestRxSignalStrength2;		//AP���߽ӿڽ����źŵ����ǿ��		//add 2010-09-07
	
    unsigned int wirelessIfTxDataPkts;				//���͵����ݰ���//	add to sub radio
    unsigned int wirelessIfRxDataPkts;				//���յ����ݰ���//	add to sub radio
    unsigned long long wirelessIfUplinkDataOctets;			//���͵����ݰ��ֽ���//	add to sub radio
    unsigned long long wirelessIfDwlinkDataOctets;			//���յ����ݰ��ֽ���//	add to sub radio

	unsigned int wirelessIfChStatsPhyErrPkts;			//�ŵ����յ����������//	add to sub radio
    unsigned int wirelessIfChStatsMacFcsErrPkts;		//�ŵ����յ�FCS MAC�����//	add to sub radio
    unsigned int wirelessIfChStatsMacMicErrPkts;		//�ŵ����յ�MIC MAC�����//	add to sub radio
    unsigned int wirelessIfChStatsMacDecryptErrPkts;		//�ŵ����յĽ���ʧ�ܵ�MAC�����//	add to sub radio
    unsigned int wirelessIfChStatsFrameErrorCnt;		//�ŵ����յĴ�֡��				//	add to sub radio

	unsigned int asd_radio_num;						//asd ��radio num
    
	struct WtpWirelessIfstatsInfo_radio *wireless_sub_radio_head;
	struct Neighbor_AP_ELE *neighbor_wtp;   //fengwenchao add 20110521
	int neighbor_ap_count;   //fengwenchao add 20110521
    struct WtpWirelessIfstatsInfo *next;
	struct WtpWirelessIfstatsInfo *WtpWirelessIfstatsInfo_list;
	struct WtpWirelessIfstatsInfo *WtpWirelessIfstatsInfo_last;
};

struct WtpDeviceInfo {
	unsigned char  *wtpMacAddr;										
	unsigned char *wtpModel;
	int wtpCurrID;
	
	unsigned char wtpCPUType;				//AP CPU���� 	// change type to unsigned char
	unsigned char wtpMemoryType;				//AP�ڴ�����// change type to unsigned char

	unsigned int wtpCPUusageThreshhd;			//AP CPU��������ֵ	//
	unsigned int wtpMemUsageThreshhd;			//AP�ڴ���������ֵ//
	
	unsigned int  wtpCPURTUsage;				//AP CPUʵʱ������		3w
	unsigned long long wtpMemoryCapacity;		//AP�ڴ�����			//3w
	unsigned int  wtpMemRTUsage;				//AP�ڴ�ʵʱ������	//3w
	unsigned long long wtpFlashCapacity;				//AP Flash����		//3w
	unsigned int wtpFlashRemain;				//AP Flashʣ��ռ�		//3w
	unsigned char wtpWayGetIP;				//AP��ȡIP��ʽ				//3w
	unsigned char wtpWorkTemp;				//AP�����¶�				//3w

	unsigned int wtpCPUPeakUsage;				//AP CPU��ֵ������
	unsigned int  wtpCPUAvgUsage;				//AP CPUƽ��������
	unsigned int  wtpMemPeakUsage;				//AP�ڴ��ֵ������
	unsigned int  wtpMemAvgUsage;				//AP�ڴ�ƽ��������

	unsigned char cpuType_str[WTP_TYPE_DEFAULT_LEN];				//nl add 20100728
	unsigned char flashType_str[WTP_TYPE_DEFAULT_LEN];				//nl add 20100728
	unsigned char memType_str[WTP_TYPE_DEFAULT_LEN];				//nl add 20100728

	char *wtpCPUProcessAbility;			//AP CPU��������			//mib�Լ���
	char *wtpFlashType;				//AP Flash����						//Ĭ��
	char *WtpIP;				//AP��IP��ַ					//ip

	unsigned int cpu_collect_average;
/////////////////////////
	
	struct WtpDeviceInfo *next;
	struct WtpDeviceInfo *WtpDeviceInfo_list;
	struct WtpDeviceInfo *WtpDeviceInfo_last;
};	


struct WtpDataPktsInfo {
    unsigned  char *wtpMacAddr;
    int wtpCurrID;
    unsigned long long wtpWirelessMacRecvAllByte;			//���߲�MAC����յ��ܵ�byte��		//
    unsigned long long wtpWirelessMacSendAllByte;			//���߲�MAC�㷢�͵��ܵ�byte��		//
    unsigned int wtpWirelessMacRecvPack;			//���߲�mac����յİ���				//
    unsigned int wtpWirelessMacRecvWrongPack;			//���߲�mac����յĴ����		//
    unsigned long long wtpWiredMacRecvRightByte;			//���߲�MAC����յ��ܵ�byte��					//
    unsigned long long wtpWiredMacSendAllByte;			//���߲�MAC�㷢�͵��ܵ�byte��					//
    unsigned int wtpWiredMacRecvWrongPack;			//���߲�MAC����յĴ����						//
    unsigned int wtpDropPkts;					//���߲�MAC�㷢�͵Ķ�����				//
    unsigned int wtpWirelessMacSendPack;			//���߲�MAC�㷢�͵İ���				//
    unsigned int wtpWirelessMacSendErrPack; 			//���߲�MAC�㷢�͵Ĵ����		//
    unsigned int wtpWirelessMacRecvRightPack;			//���߲�MAC����յ���ȷ�İ���//
    unsigned int wtpWirelessMacSendRightPack;			//���߲�MAC�㷢�͵���ȷ�İ���	//
    unsigned int wtpWiredMacRecvPack;				//���߲�MAC����յİ���							//
    unsigned int wtpWiredMacRecvRightPack;			//���߲�MAC����յ���ȷ�İ���					//
    unsigned int wtpWiredMacSendPack;				//���߲�MAC�㷢�͵İ���							//
    unsigned int wtpWiredMacSendErrPack;			//���߲�MAC�㷢�͵Ĵ����						//
    unsigned int wtpWiredMacSendRightPack;			//���߲�MAC�㷢�͵���ȷ�İ���					//
    unsigned int wtpWirelessradioRecvBytes;			//
    unsigned long long wtpWirelesscoreRecvBytes;																		//
    unsigned long long wtpWirelessRecvFlowByte;		//���߲���յ������ֽ�����AP�����������㣩
    unsigned long long wtpWirelessSendFlowByte;		//���߲෢�͵������ֽ�����AP�����������㣩
    
    struct WtpDataPktsInfo *next;
	struct WtpDataPktsInfo *WtpDataPktsInfo_list;
	struct WtpDataPktsInfo *WtpDataPktsInfo_last;
};

struct WtpStatsInfo {
    int wtpCurrID;
    unsigned char *wtpMacAddr;
	unsigned char wtpRadioCount;						//add for radio count
	char *wtpModel;									//APģʽ
	unsigned char wtpBwlanNum;							//add for wtpBwlanNum  for showing wlan infor
    unsigned int wtpConfigBSSIDNum;				//���õ�BSSID����
    unsigned char wtpSSIDNum;					//SSID����				//
	unsigned char wtpSupportSSID[WLAN_NUM] ; 		//should make 
    unsigned int wtpAllowConUsrMnt;				//AP���������û���

	/*station information*/
  	unsigned int wtpMountConUsrTimes;			//�û��������
    unsigned int wtpMountConSuccTimes;			//assoc success times
    unsigned int wtpMountReConSuccTimes;		//reassco success times
    unsigned int wtpMountConFailTimes;			//�û�����ʧ�ܴ���
    unsigned int wtpMountReConFailTimes;			//reassoc failure times
    unsigned int wtpMountConReasTimes;			//�û��ع�������
    unsigned int wtpOnlineUsrNum;				//AP�����û���
    unsigned int wtpStatsDisassociated;		//�����ն��쳣�����¶Ͽ����ӵĴ���
    unsigned int wtpAllStaAssTime;			//AP������ʱ��

	/*station connect information*/	
    unsigned int wtpUsrRequestConnect;			//�û��ܵ�����������
    unsigned int wtpResponseUsrConnect;		//AP��Ӧ�û�������
    unsigned int wtpLoginSuccNum;			//�ɹ��������
    unsigned int wtpLessResourceRefuseNewUsrMount;		//��Դ��������¾ܾ���������û�����
	unsigned int wtpResponseUsrSuccConnect;	//�û��������ӳɹ���
	unsigned int wtpResponseUsrFailConnect;	//�û���������ʧ����

    float wtpUsrWirelessResoUseRate;//�û��ࣨ���߲ࣩ��Դ������//mib ��wtpOnlineUsrNum/wtpAllowConUsrMnt
    unsigned char total_if_num;         //zhangshu copy from 1.2, 2010-09-14
   /*fengwenchao add 20111125 for GM-3*/
    unsigned int heart_time_avarge;		//AC���AP����������ƽ���ӳ�ʱ��								
    unsigned int heart_lose_pkt;			//AC��ʧ��AP������Ӧ�ܰ���							
    unsigned int heart_transfer_pkt;		//AC���AP�������������ܰ���						
   /*fengwenchao add end*/
    unsigned int is_refuse_lowrssi;	 /*fengwenchao add for chinamobile-177.20111122*/	
    struct WtpStatsInfo  *next;
	struct  WtpStatsInfo *WtpStatsInfo_list;
	struct  WtpStatsInfo *WtpStatsInfo_last;
};

struct WtpWlanStatsInfo_wlan{
	unsigned char wlan_id;

	unsigned int wtpAttachedFlowThreshold;			 //�������ؾ�����ֵ
	unsigned int wtpUsrNumAppendThreshold; 		 //�û����ؾ�����ֵ
	unsigned char wtpLoadBalance;				 //���ؾ���
	unsigned char *wirelessSSID;					 //����SSID
	unsigned char wtpSsidBroadcast; 			 //SSID�Ƿ�㲥
    unsigned char wlanSecID;
	char *wlan_essid;

	/*security infor*/
	unsigned char * wtpConRadiusServerIP;			//Radius server IP
    unsigned int wtpConRadiusServerPort;			//Radius server �˿ں�
    unsigned int wtpEAPAuthenSupport;				//EAP ģ���Ƿ���
    unsigned int wtpConfSecurMech;				//��ǰ���õİ�ȫ����


	unsigned int wtpSsidCurrOnlineUsrNum;			//SSID��ǰ�����û���//asd
    unsigned int wtpAcAppointDistOnlineUsrNum;			//ָ���������û���//asd

	unsigned char wlanL2Isolation;				//xiaodawei add isolation for wlan, 20110304
	
	/*fengwenchao add 20110426 for dot11WlanDataPktsTable*/
	/*-----------------------------------------------*/
	unsigned int ethernetRecvCorrectFrames;                    //����ࣨ���߲ࣩMAC����յ�����ȷ����֡��Ŀ
	unsigned long long ethernetRecvCorrectBytes;            //����ࣨ���߲ࣩMAC����յ���������ȷ���ݰ��ֽ���
	unsigned long long ethernetSendCorrectBytes;           //����ࣨ���߲ࣩMAC��ӷ��ͳ�ȥ��������ȷ���ݰ��ֽ���
	/*-----------------------------------------------*/
	
	struct WtpWlanStatsInfo_wlan *next;
	struct WtpWlanStatsInfo_wlan *WtpWlanStatsInfo_wlan_list;
	struct WtpWlanStatsInfo_wlan *WtpWlanStatsInfo_wlan_last;
};


struct WtpWlanStatsInfo {
	/*set ap statistics enable*/
    int wtpCurrID;
    unsigned char *wtpMacAddr;
 	unsigned char wtpBwlanNum;							//add for wtpBwlanNum  for showing wlan infor
 	unsigned char wtpSupportSSID[WLAN_NUM] ; 		//should make 
    unsigned char wtpL2Insulate;					//��������Ƿ���		//f

    long wtpSSIDPriority;				//SSID ���ȼ�			//mib�Լ��ӣ�ΪĬ��1

	struct WtpWlanStatsInfo_wlan *WtpWlanStatsInfo_wlan_head;
    struct WtpWlanStatsInfo *next;
	struct WtpWlanStatsInfo *WtpWlanStatsInfo_list;
	struct WtpWlanStatsInfo *WtpWlanStatsInfo_last;

};

struct SSIDStatsInfo_sub_wlan{
    unsigned char wlanCurrID;
	/*should input update wtp bss pakets infomation*/
	/*------------------------------------------*/
    unsigned long long SSIDChStatsDwlinkTotRetryPkts;	//SSID�����ش�����		//wtp f
    unsigned long long SSIDChStatsUplinkUniFrameCnt;			//SSID���е���֡��//wtp f
    unsigned long long SSIDChStatsDwlinkUniFrameCnt;			//SSID���е���֡��//wtp f
    unsigned long long SSIDUpChStatsFrameNonUniFrameCnt;		//SSID���зǵ���֡��//wtp f
    unsigned long long SSIDDownChStatsFrameNonUniFrameCnt;		//SSID���зǵ���֡��//wtp f
    
    unsigned long long SSIDDwlinkTotFrameCnt;		//SSID���е���֡��//wtp f
    unsigned long long SSIDUplinkTotFrameCnt;		//SSID���е���֡��//wtp f
	/*------------------------------------------*/

	unsigned int SSIDRxCtrlFrameCnt;			//SSID���յĿ���֡��
    unsigned int SSIDRxDataFrameCnt;			//SSID���յ�����֡��
    unsigned int SSIDRxAuthenFrameCnt;			//SSID���յ���֤֡��
    unsigned int SSIDRxAssociateFrameCnt;		//SSID���յĹ���֡��
    unsigned int SSIDTxCtrlFrameCnt;			//SSID���͵Ŀ���֡��
    unsigned int SSIDTxDataFrameCnt;			//SSID���͵�����֡��
    unsigned int SSIDTxAuthenFrameCnt;			//SSID���͵���֤֡��
    unsigned int SSIDTxAssociateFrameCnt;		//SSID���͵Ĺ���֡��
/*fengwenchao add 20110127*/
	unsigned int SSIDDwErrPkts;                 //SSID���еĴ������
	unsigned int SSIDDwDropPkts;				//SSID���е��ܶ�֡��
	unsigned int SSIDDwTotErrFrames;			//SSID���е��ܴ�֡��
	unsigned int SSIDUpErrPkts;					//SSID���еĴ������
	unsigned int SSIDUpDropPkts;				//SSID���е��ܶ�֡��
	unsigned int SSIDUpTotErrFrames;			//SSID���е��ܴ�֡��
/*fengwenchao add end*/    
    unsigned int SSIDApChStatsNumStations;			//ʹ�ø��ŵ����ն���
    unsigned int SSIDAccessTimes;				//access times
    
	/*table 34*/
	/*--------------------------------------*/
	/*��wtp ���������update wtp bss pakets infomation*/
	unsigned char *wtpSSIDName;
	unsigned int wtpSSIDMaxLoginUsr;
	unsigned char wtpSSIDState;
	unsigned char wtpSSIDSecurityPolicyID;
	unsigned char wtpSSIDLoadBalance;
	char *wtpSSIDESSID;
	unsigned char *WlanName;
	unsigned int vlanid;	//xiaodawei add vlanid 20101028
	/*--------------------------------------*/

	/*for dot11RadioWlanTable   fengwenchao add 20110331*/ 
	/*--------------------------------------*/
	unsigned char traffic_limit_able;   //��������״̬
	unsigned int send_traffic_limit;    //������������
	/*--------------------------------------*/

	struct SSIDStatsInfo_sub_wlan *next;
	struct SSIDStatsInfo_sub_wlan *SSIDStatsInfo_sub_wlan_list;
	struct SSIDStatsInfo_sub_wlan *SSIDStatsInfo_sub_wlan_last;

/*fengwenchao add 20110127*/
	unsigned long long wl_up_flow;  //�������ж˿�����
    unsigned long long wl_dw_flow;  //�������ж˿�����
    unsigned long long ch_dw_pck;		   // �ŵ������ܰ���
    unsigned long long ch_dw_los_pck;    //�ŵ������ܵĶ�����
    unsigned long long ch_dw_mac_err_pck;  //�ŵ�����MAC�����
    unsigned long long ch_dw_resend_pck;  //�ŵ������ش�����
    unsigned long long ch_up_frm;		   //�ŵ������ܵ�֡��
    unsigned long long ch_dw_frm;    //�ŵ������ܵ�֡��
    unsigned long long ch_dw_err_frm;	   //�ŵ����д�֡��
    unsigned long long ch_dw_los_frm;  //�ŵ������ܵĶ�֡��
    unsigned long long ch_dw_resend_frm;  //�ŵ������ܵ��ش�֡��
    unsigned long long ch_up_los_frm;   //�ŵ������ܵĶ�֡��                    fengwenchao modify 20110224
    unsigned long long ch_up_resend_frm;  //�ŵ������ܵ��ش�֡��           fengwenchao modify 20110224

	unsigned int WlanradioRecvSpeed;  //����ʱ�����������ж˿�����
	unsigned int WlanradioSendSpeed;  //����ʱ�����������ж˿�����
/*fengwenchao add end*/
};

//mahz add 2011.11.9 for GuangZhou Mobile
struct WtpStationinfo {
	unsigned int	wtpid;
	char 		 *wtpMacAddr;				//AP��MAC��ַ	
	unsigned int no_auth_sta_num;			/*����֤�����û���*/
	unsigned int assoc_auth_sta_num;		/*������֤�����û���*/
	
	unsigned int no_auth_accessed_total_time;		/*����֤�û�����AP��ʱ��*/
	unsigned int assoc_auth_accessed_total_time;	/*������֤�û�����AP ��ʱ��*/
	
	unsigned int no_auth_sta_abnormal_down_num;		/*�쳣���ߴ���*/
	unsigned int assoc_auth_sta_abnormal_down_num;

	unsigned int assoc_auth_req_num;
	unsigned int assoc_auth_succ_num;
	unsigned int assoc_auth_fail_num;
	//qiuchen
	unsigned int assoc_auth_online_sta_num;			/* WEP/PSK associate auth  (SHARE:WEP) */
	unsigned int auto_auth_online_sta_num;			/* SIM/PEAP */
	unsigned int all_assoc_auth_sta_total_time;		/*all WEP/PSK sta online time */
	unsigned int auto_auth_sta_total_time;			/* SIM/PEAP sta online time */
	unsigned int assoc_auth_sta_drop_cnt;		/* WEP/PSK assoc auth sta drop count */
	unsigned int auto_auth_sta_drop_cnt;			/* SIM/PEAP sta drop count */
	unsigned int weppsk_assoc_req_cnt, weppsk_assoc_succ_cnt, weppsk_assoc_fail_cnt;	
	unsigned int auto_auth_req_cnt, auto_auth_suc_cnt, auto_auth_fail_cnt;
	unsigned int auto_auth_resp_cnt;	/* radius response auth count */
    struct WtpStationinfo *next;
};

#if 0 //for old version
struct SSIDStatsInfo {
    unsigned char *wtpMacAddr;
    int wtpCurrID;
	unsigned char wtpBwlanNum;
 	unsigned char wtpSupportSSID[WLAN_NUM] ; 		//should make ]
 	unsigned char wtpBssNum;
	
    unsigned int SSIDTxSignalPkts;			//SSID���͵��������	//wtp f
    unsigned int SSIDRxSignalPkts;			//SSID���յ��������	//wtp f
    
    unsigned int SSIDTxDataPkts;			//SSID���͵����ݰ���//wtp f
    unsigned int SSIDRxDataPkts;			//SSID���յ����ݰ���//wtp f
    unsigned long long SSIDUplinkDataOctets;			//SSID���յ��ֽ���//wtp f
    unsigned long long SSIDDwlinkDataOctets;			//SSID���͵��ֽ���//wtp f
    unsigned int SSIDApChStatsFrameFragRate;			//�ŵ���֡�ķֶ�����//wtp f

	
    struct SSIDStatsInfo_sub_wlan *SSIDStatsInfo_sub_wlan_head;
    struct SSIDStatsInfo *next;
	struct SSIDStatsInfo *SSIDStatsInfo_list;
	struct SSIDStatsInfo *SSIDStatsInfo_last;
};
#endif
/* zhangshu copy from 1.2,2010-09-13 */
/*zhaoruijia,20100805,���Radioid��㵽wtp��*/
struct SSIDStatsInfo_Radioid_info{
    
  unsigned int radioId;
  unsigned char radioBwlanNum;
  unsigned char RadioidSupportSSID[WLAN_NUM];
  MixedGreenfieldParameter Mixed_Green_Field;   //fengwenchao add 20110331
  struct SSIDStatsInfo_sub_wlan     *SSIDStatsInfo_sub_wlan_head;
  struct SSIDStatsInfo_Radioid_info *next;
  struct SSIDStatsInfo_Radioid_info *SSIDStatsInfo_Radioid_info_list;
  struct SSIDStatsInfo_Radioid_info *SSIDStatsInfo_Radioid_info_last;

};
struct SSIDStatsInfo {
    unsigned char *wtpMacAddr;
    int wtpCurrID;
	//unsigned char wtpBwlanNum;
	unsigned char wtpBwlanRadioNum;//wtp �¿��õ�radio��
 	unsigned char wtpSupportSSID[WLAN_NUM] ; 		//should make ]
 	unsigned int wtpSupportRadioId[L_RADIO_NUM];
 	unsigned char wtpBssNum;
	
    unsigned int SSIDTxSignalPkts;			//SSID���͵��������	//wtp f
    unsigned int SSIDRxSignalPkts;			//SSID���յ��������	//wtp f
    
    unsigned int SSIDTxDataPkts;			//SSID���͵����ݰ���//wtp f
    unsigned int SSIDRxDataPkts;			//SSID���յ����ݰ���//wtp f
    unsigned long long SSIDUplinkDataOctets;			//SSID���յ��ֽ���//wtp f
    unsigned long long SSIDDwlinkDataOctets;			//SSID���͵��ֽ���//wtp f
    unsigned int SSIDApChStatsFrameFragRate;			//�ŵ���֡�ķֶ�����//wtp f

	
   // struct SSIDStatsInfo_sub_wlan *SSIDStatsInfo_sub_wlan_head;
    struct SSIDStatsInfo_Radioid_info *SSIDStatsInfo_Radioid_info_head;
    struct SSIDStatsInfo *next;
	struct SSIDStatsInfo *SSIDStatsInfo_list;
	struct SSIDStatsInfo *SSIDStatsInfo_last;
};


struct WtpIfInfo_sub_info{
	/* should input set ap interface infomation report switch enable to get the data*/
	unsigned char	wtpIfIndex;		  //AP���߽ӿ�����						//ethindex
	unsigned int	wtpIfSpeed;		  //AP���߽ӿ�����						//rate *1000
	unsigned int    wtpMTU;           //AP���߽ӿ�����䵥Ԫ          fengwenchao add 20110127 for XJDEV-32 from 2.0
	unsigned char	wtpIfPhyAddr[6];	  //AP���߽ӿ������ַ
	char *wtpIfIntro;				  //AP���߽ӿ�����
	char *wtpIfName;				  //AP���߽ӿ�����
	unsigned char	wtpIfType; 				  //AP���߽ӿ�����				//type
	unsigned char 	wtpifinfo_report_switch;
	unsigned char	wtpIfAdminStatus;			  //AP���߽ӿ�״̬����	//state
	unsigned char	wtpIfOperStatus;			  //AP���߽ӿڵ�ǰ״̬	//state

	unsigned int    wtpIfUplinkRealtimeRate; //AP���߽ӿ�����ʵʱ����
	unsigned int    wtpIfDownlinkRealtimeRate; //AP���߽ӿ�����ʵʱ����
	time_t state_time;
	time_t wtpIfLastChange;			  //AP���߽ӿڵ�ǰ״̬����ʱ��// mib ��state_time �����

	struct WtpIfInfo_sub_info *next;
	struct WtpIfInfo_sub_info *WtpIfInfo_sub_info_list;
	struct WtpIfInfo_sub_info *WtpIfInfo_sub_info_last;
	
};

struct WtpIfnameInfo {
	int wtpCurrID;			  //AP��ǰID
	unsigned char *wtpMacAddr;                 //AP��MAC��ַ
	unsigned char *wtpModel;
	unsigned char wtpIfIndexNum;
	unsigned int wtpMTU;                      //AP���߽ӿ���������ݵ�Ԫ
	
	struct WtpIfInfo_sub_info *WtpIfInfo_sub_info_head;
    struct WtpIfnameInfo *next;
	struct WtpIfnameInfo *WtpIfnameInfo_list;
	struct WtpIfnameInfo *WtpIfnameInfo_last;
	
};

struct Sub_RadioParaInfo{
	unsigned char wtpRadLocalID;                 //Radio ��Local ID
	unsigned int  wtpRadCurrID;		    //Radio ��ID

	
	unsigned short wtpFrequencyHopTimes;          //���ڱ����ŵ����Ŷ�����Ƶ�ʸı�Ĵ���
	unsigned short wtpFreHopDetectTime;           //���ڱ����ŵ����Ŷ�����Ƶ�ʸı侭����ʱ��
	
	unsigned char wtpConfigLongRetransThreshold; //���ش���ֵ
	unsigned short wtpMessageNeafThreshold;       //��Ϣ��Ƭ��ֵ
	unsigned short wtpRTSThreshold;               //RTS��ֵ
	unsigned short wtpSignalAveIntensity;         //ƽ���ź�ǿ��
	
	unsigned char wtpPreambleLen;                //preamble����

	struct Sub_RadioParaInfo *next;
	struct Sub_RadioParaInfo *Sub_RadioParaInfo_list;
	struct Sub_RadioParaInfo *Sub_RadioParaInfo_last;

};

struct WtpRadioParaInfo {
	int wtpCurrID;                         //AP��ID
	unsigned char *wtpMacAddr;		    //AP��MAC��ַ
	unsigned char wtpRadioNum;

	long wtpAntAPlus;                   //�������棨802.11a��
	long wtpAntBGPlus;                  //�������棨802.11b/g��
	char wtpSignalPathMode;            //AP�ŵ�ģʽ
	unsigned char wtpSignalSNR;                 //�ź������	//wtp
	

	struct Sub_RadioParaInfo *Sub_RadioParaInfo_head;
    struct WtpRadioParaInfo *next;
    struct WtpRadioParaInfo *WtpRadioParaInfo_list;
    struct WtpRadioParaInfo *WtpRadioParaInfo_last;
};

struct WtpEthPortInfo {
	int wtpCurrID;             	 //AP��ǰID
	unsigned char *wtpMacAddr;                //AP��MAC��ַ
	
	unsigned int  wtpWirelessUpPortRate;      //�����������������߽ӿ�����
	unsigned int  wtpWirelessDownPortRate;    //�����������������߽ӿ�����
	
	unsigned char wtpWirelessUpPortUDTimes;   //�����������������߽ӿ�UpDown����
	unsigned char wtpWirelessDownPortUDTimes; //�����������������߽ӿ�UpDown����
	
	unsigned int wtpUplinkDataThroughput;    //��̫��ָ����������������
	unsigned int wtpDownlinkDataThroughput;  //��̫��ָ����������������
	unsigned char wtpPhyInterMnt;             //AP����ӿ�����	
	
    struct WtpEthPortInfo *next;
	struct WtpEthPortInfo *WtpEthPortInfo_list;
	struct WtpEthPortInfo *WtpEthPortInfo_last;

};

struct Sub_RadioStatsInfo{

	unsigned char wtpRadLocalID;		//Radio��Local ID
	unsigned int  wtpRadCurrID;		//Radio ��ID	
	
	unsigned short wtpCurSendPower;		//AP���书��
	unsigned short wtpRecvPower;		//AP���չ���
	
	unsigned char wtpCurTakeFreqPoint;	//AP��ǰռ��Ƶ��	//mib �Լ���
	unsigned char wtpCurConfChannel;		//AP��ǰ���õ��ŵ�
	unsigned int radio_type;	//���wtpProtoMode�õ�
	long wtpProtoMode;		//APЭ��ģʽ	//��radio type�õ�mib�Լ���
	
	unsigned int wtpTerminalConRate;	//�ն˽�������	
	char *wtpConfigAvailSpeed;      //��Ч���ʼ�	//mib��Ĭ��ֵ
	char *wtpConfigForceSpeed;      //ǿ�����ʼ�	//mib ��Ĭ��ֵ
	long wtpRTSCTSEnable;           //�Ƿ�ʹ��RTS/CTS		//Ĭ��1
	
	unsigned short wtpconfigBeaconFrameBlank; //���õ�beacon֡���

	struct Sub_RadioStatsInfo *next;
	struct Sub_RadioStatsInfo *Sub_RadioStatsInfo_list;
	struct Sub_RadioStatsInfo *Sub_RadioStatsInfo_last;

};


struct RadioStatsInfo {
	int wtpCurrID; 
	unsigned char *wtpMacAddr;		//AP��MAC��ַ
	unsigned char wtp_radio_num;
	unsigned int wtpPowerManage;		//AP���ʹ���				//nnnn
	unsigned char wtpSampleTime;	//AP����ʱ�� mib need *100		//nnnn
	
	struct Sub_RadioStatsInfo *Sub_RadioStatsInfo_head;
    struct RadioStatsInfo *next;
    struct RadioStatsInfo *RadioStatsInfo_list;
	struct RadioStatsInfo *RadioStatsInfo_last;
};
/*16-2*/
struct Sub_WtpConfigRadioInfo{
	unsigned char wtpRadLocalID;	//Radio��Local ID		//
	unsigned int  wtpRadCurrID;	//Radio ID			//
	unsigned int radioType;		//Radio����
	unsigned int radioWlanNum;

	unsigned char radioChannel;	//Radio���ŵ�				//
	unsigned short radioTxPower;	//Radio�ķ��书��	//
	unsigned int radioSpeed;	//Radio����				//ͬradioMaxSpeed
	
	unsigned short radioBeacon;	//beacon				//
	unsigned short radioBeaf;		//��Ƭ����	//
	unsigned char radioDtim;		//Dtim����		//
	unsigned short radioRTS;		//RTS����		//
	
	unsigned char radioLeadCode;	//ǰ����		//mib���ж�
	unsigned char radioShortRetry;	//Short retry		//
	unsigned char radioLongRetry;	//Long retry		//
	unsigned char radioService;	//Radio����			//
	
	unsigned int radioMaxSpeed;	//Radio�������				//
	unsigned char radioMaxFlow;	//�������		//
	unsigned char radioBindWlan[WLAN_NUM];	//�󶨵�wlan						
	unsigned char radioDelbindWlan[WLAN_NUM];	//Radioɾ���󶨵�wlan  ͬradioDelbindWlan
	unsigned int radioBindQos;	//Radio��Qos					//   
	unsigned int radioDelbindQos;  //Radioɾ���󶨵�Qos		//ͬradioDelbindQos
		
	struct Sub_WtpConfigRadioInfo *next;
	struct Sub_WtpConfigRadioInfo * Sub_WtpConfigRadioInfo_list;
	struct Sub_WtpConfigRadioInfo * Sub_WtpConfigRadioInfo_last;


};
/*16*/
struct WtpConfigRadioInfo {
	int wtpCurrID;                         //AP��ID
	unsigned char *wtpMacAddr;	//AP��MAC��ַ
	unsigned char wtp_radio_num;
	
	struct Sub_WtpConfigRadioInfo *Sub_WtpConfigRadioInfo_head;
    struct WtpConfigRadioInfo *next;
	struct WtpConfigRadioInfo * WtpConfigRadioInfo_list;
	struct WtpConfigRadioInfo * WtpConfigRadioInfo_last;

	
};
/*17*/
struct UsrLinkInfo {
	int  wtpCurrID;			  //AP�ĵ�ǰID
	unsigned char *wtpMacAddr; 			  //AP��MAC��ַ
	
	unsigned int wtpReasonInvalidFailLinkTimes;	  //������Ч����������ʧ�ܵĴ���
	unsigned int wtpReasonTimeOutFailLinkTimes;	  //���ڳ�ʱ����������ʧ�ܵĴ���
	unsigned int wtpReasonRefuseFailLinkTimes;	  //���ھܾ�����������ʧ�ܵĴ���
	unsigned int wtpReasonOthersFailLinkTimes;        //��������ԭ�����������ʧ�ܵĴ���
	
	unsigned int wtpSolutionLinksVerifyLinkTimes;     //�������֤����
	unsigned int wtpReasonUsrLeaveVerfiyLinkTimes;    //�����û��뿪��������Ĵ���
	unsigned int wtpReasonLackAbilityVerifyLinkTimes; //�������������������Ĵ���
	unsigned int wtpReasonExceptionVerifyLinkTimes;   //�����쳣��������Ĵ���
	unsigned int wtpReasonOtherVerfyLinkTimes;        //����ԭ�������Ĵ���
	
	unsigned int wtpStaOnlineTime;	          //����station������ʱ��				//mib *100
	unsigned long long wtpStaNewTotalOnlineTime;	  //����station������ʱ��֮��//mib *100
		
    struct UsrLinkInfo *next;
	struct UsrLinkInfo *UsrLinkInfo_list;
	struct UsrLinkInfo *UsrLinkInfo_last;
};

/*19-1 book modify, 2011-1-19 */
struct WiredIfStatsInfo{

    unsigned char wtpIfIndex;               //���߽ӿ�����
    unsigned char wtpWiredififUpDwnTimes;   //���߽ӿ�UpDown����
    
	unsigned int wtpWiredifInUcastPkts;     //���߽ӿڽ��յĵ�������		//
	unsigned int wtpWiredififInNUcastPkts;  //���߽ӿڽ��յķǵ�������	//
	unsigned int wtpWiredififInPkts;        //���߽ӿڽ��յ����ֽ���
	unsigned int wtpWiredififInDiscardPkts; //���߽ӿڽ��յĶ�����
	unsigned int wtpWiredififInErrors;      //���߽ӿڽ��յĴ����
	unsigned int wtpWiredifInBcastPkts;
	unsigned int wtpWiredififOutUcastPkts;  //���߽ӿڷ��͵ĵ�������		//
	unsigned int wtpWiredififOutNUcastPkts; //���߽ӿڷ��͵ķǵ�������	//
	unsigned int wtpWiredififOutPkts;       //���߽ӿڷ��͵����ֽ���
	unsigned int wtpWiredififOutDiscardPkts;//���߽ӿڷ��͵Ķ�����
	unsigned int wtpWiredififOutErrors;     //���߽ӿڷ��͵Ĵ���	
	unsigned int wtpWiredifOutBcastPkts;

	
	unsigned long long wtpWiredififInOctets;      //���߽ӿڽ����ֽ���
	unsigned long long wtpWiredififOutOctets;     //���߽ӿڷ����ֽ���

	unsigned long long rx_sum_bytes ;  // �¼�    ���ӿ��յ������ֽ�����20100910
   	unsigned long long tx_sum_bytes ;  // �¼�    ���ӿڷ��������ֽ�) 		20100910

   	struct WiredIfStatsInfo *next;
};

/*19-2 */
struct WtpWiredIfStatsInfo {
	/*SYSTEM(config-wtp 1)# set ap extension infomation switch enable
	 SYSTEM(config)# set ap statistics enable*/
	int wtpCurrID;	                         //AP��ǰID
	unsigned char *wtpMacAddr;                        //Ap��MAC��ַ
	unsigned char wtpWireIfNum;				//add for showing

	struct WiredIfStatsInfo *EthInfo;
		
    struct WtpWiredIfStatsInfo *next;
    struct WtpWiredIfStatsInfo *WtpWiredIfStatsInfo_list;
	struct WtpWiredIfStatsInfo *WtpWiredIfStatsInfo_last;
	
};

/*20-2*/
struct Sub_WtpWirelessIfInfo {
	/*"set ap interface infomation report switch (enable|disable)",*/

	unsigned char  wtpWirelessIfIndex;                      //AP���߽ӿ�����
	unsigned int g_radio_id;
	char *wtpWirelessIfDescr;                      //AP���߽ӿ�����									//mib ��
	unsigned char radio_has_bss;					//���radio��û��bss
	unsigned int surport_rate_num;					//for showting
	
	unsigned char wtpWirelessIfType;                       //AP���߽ӿ�����				//mibĬ��41
	unsigned int wtpWirelessIfSpeed;              //AP���߽ӿ�����
	unsigned char *wtpWirelessIfPhysAddress;                //AP���߽ӿ������ַ		//bssid	
	unsigned char wtpWirelessIfAdminStatus;                 //AP���߽ӿ�״̬����				//
	unsigned char wtpWirelessIfOperStatus;                  //AP���߽ӿڵ�ǰ״̬					//state 				
	unsigned int wtpWirelessIfLastChange;         //AP���߽ӿڵ�ǰ״̬����ʱ��					//������100	
	unsigned char wtpWirelessIfRadioChannelAutoSelectEnable;//AP���߽ӿ��Ƿ����Զ��ŵ�ѡ��//
	unsigned char wtpWirelessIfRadioChannelConfig;          //AP���߽ӿ��ŵ�����					//
	unsigned int wtpWirelessIfDiversitySelectionRx;        //AP�����Ƿ�֧�ַּ�ģʽ			//Ĭ��0
	unsigned int wtpWirelessIfCurrRadioModeSupport;        //AP��ǰradio֧�ֵ�ģʽ				//
	int wtpWirelessIfTransmitSpeedConfig[20];        //AP���߽ӿڴ�����������	
	unsigned short wtpWirelessIfPwrAttValue;                 //AP���߽ӿڹ���˥������			//
	unsigned char wtpWirelessIfPowerMgmtEnable;             //AP���߽ӿ��Ƿ����Զ����ʿ���//
	unsigned short wtpWirelessIfTxPwrStep;// AP txpower step
			
    struct Sub_WtpWirelessIfInfo *next;
	struct Sub_WtpWirelessIfInfo *Sub_WtpWirelessIfInfo_list;
	struct Sub_WtpWirelessIfInfo *Sub_WtpWirelessIfInfo_last;
};

/*20*/
struct WtpWirelessIfInfo {
	
	int wtpCurrID;                                //AP��ǰID				//wtp
	unsigned char *wtpMacAddr;	                       //AP��MAC��ַ		//wtp
	unsigned char wifi_num;
	unsigned char radio_count;		//xiaodawei add radiocount from wtpcompatible.xml, 20110124
	unsigned char *wtpModel;
	unsigned int wtpWirelessIfMTU;
	unsigned int wtpWirelessIfPwrAttRange;                 //AP���߽ӿڹ���˥����Χ				//wtp
	unsigned int wtpWirelessIfAntennaGain;                 //AP���߽ӿ���������					//wtp
	unsigned int wtpWirelessIfMaxStationNumPermitted;      //AP���߽ӿ������������station��	//wtp
	unsigned int wtpWirelessIfMaxTxPwrLvl;                //AP���߽ӿ�����书��				//wtp
	
	struct Sub_WtpWirelessIfInfo *Sub_WtpWirelessIfInfo_head;
    struct WtpWirelessIfInfo *next;
	struct WtpWirelessIfInfo *WtpWirelessIfInfo_list;
	struct WtpWirelessIfInfo *WtpWirelessIfInfo_last;
};

/*liuzhenhua append 2010-05-28-*/
struct WtpStaInfo {
	
	char 		 *wtpMacAddr;				//AP��MAC��ַ	
	char 		 *wtpTerminalMacAddr;			//��վ��MAC��ַ
	long 		  wtpStaIp;					//��վ��IP
	long 		  wtp_sta_realip;	//real ip (in Centralized authentication local forwarding)
	unsigned char forward_mode;    //0:local 1:tunnel 2 tunnel auth, local forward
	unsigned long wtpWirelessClientSNR;		//���߶�վ�������
	unsigned long long wtpSendTerminalPackMount;		//���͵���վ���ܰ���//xiaodawei modify, change 32bits to 64bits, 20110309
	unsigned long long wtpSendTerminalDataPackMount;	//data pkts sent to terminal, xiaodawei add, 20110309
	unsigned long long wtpSendTerminalByteMount;		//���͵���վ�����ֽ���//xiaodawei modify, change 32bits to 64 bits, 20110104
	unsigned long long wtpTerminalRecvPackMount;		//�Ӷ�վ���յ��ܰ���//xiaodawei modify, change 32bits to 64bits, 20110309
	unsigned long long wtpTerminalRecvDataPackMount;	//data pkts received from terminal, xiaodawei add, 20110309
	unsigned long long wtpTerminalRecvByteMount;		//�Ӷ�վ���յ����ֽ���//xiaodawei modify, change 32bits to 64 bits, 20110104
	unsigned long wtpTerminalResPack;		//��վ�ش�����
	unsigned long wtpTerminalResByte;		//��վ�ش��ֽ���
	unsigned long wtpTerminalRecvWrongPack;		//��վ���յĴ����
	unsigned long wtpMacTermAddrUsrOnlineTime;	//����mac��ַ���û�����ʱ��
	unsigned long long wtpMacTermAddrUsrSendSpd;			//����mac��ַ���û���������	//xiaodawei modify, change 32bits to 64bits, 20110309
	unsigned long long wtpMacTermAddrUsrRecvSpd;			//����MAC��ַ���û���������	//xiaodawei modify, change 32bits to 64bits, 20110309
	unsigned long long wtpMacTermAddrUsrAllThroughput;		//����mac��ַ���û�������		//xiaodawei modify, change 32bits to 64bits, 20110309
	char*		  wtpMacTermAPReceivedStaSignalStrength;	//AP�Ӷ�վ���յ��ĵ�ǰ�ź�ǿ��
	unsigned long long wtpMacTermStaTxFragmentedPkts;		//��վ���͵��ܵķ�Ƭ����//xiaodawei modify, change 32bits to 64bits, 20110309
	unsigned long long wtpMacTermAPTxFragmentedPkts;		//AP���͸���վ���ܵķ�Ƭ����//xiaodawei modify, change 32bits to 64bits, 20110309
	long 		  wtpBelongAPID;				//������AP��IP��ַ
	char 		 *wtpterminalaccesstime;			//�ն˽���ʱ��
	unsigned int wtpterminalaccesstime_int;
	char*        wtpName;		
	unsigned int	security_type;		//mahz add 2011.11.10
    struct WtpStaInfo *next;
	time_t sta_online_time;//qiuchen add it 2012.10.31
};

/*------------------------------------------------*/
struct Wtp_TerminalInfo {
    int staqSecID;
    int staqWlanID;
    int  staqRadioID; 
	
    char *wtpTerminalMacAddr;				
    long wtpEndWMMSta;					//��վ��WMM����
    unsigned int wtpStaIPAddress;			//��վIP��ַ
    unsigned int wtpStaRadioMode;				//��վ������ģʽ
    long wtpStaRadioChannel;				//��վ���ŵ�
    long wtpAPTxRates;					//��վ��ǰ��������
    long wtpAPRxRates;					//sta tx rates, xiaodawei add, 20110103, ap report 0 now
    long wtpStaPowerSaveMode;				//��վ����ģʽ
    long wtpStaVlanId;					//��վ�������vlan��ID
    char *wtpStaSSIDName;				//SSID ����
    long wtpStaAuthenMode;				//��Ȩģʽ
    long wtpStaSecurityCiphers;				//��վ��������
    long wtpAutenModel;					//��վ��֤ģʽ
    int wtpEndStaID;					//��վ����apid
    unsigned int encryption_type;		//encryption_type  add for mib showting
    struct Wtp_TerminalInfo *next;
};

struct WtpTerminalInfo{
	int wtpCurrID;
	unsigned char *wtpMacAddr;
	int sta_num;    //Wtp_TerminalInfo entries number in terminalInfo_list;
	struct Wtp_TerminalInfo * terminalInfo_list;
	struct WtpTerminalInfo *next;
};
/*liuzhenhua append 2010-05-26*/
struct WlanDataPktsInfo{
		
	long  wlanCurrID;			//Wlan��ǰID
	unsigned long long wtpSsidSendTermAllByte;		//ָ��SSID AP���͵��ն˵��ֽ���
	unsigned long wtpSsidRecvTermAllPack;		//ָ��SSID AP���ն˽��յİ���
	unsigned long long wtpSsidRecvTermAllByte;		//ָ��SSID AP���ն˽��յ��ֽ���	
	unsigned long long wtpSsidWirelessMacRecvDataRightByte;	/*xiaodawei modify,20101116, ָ��SSID MAC����յ���ȷ�������ֽ���*/
	unsigned long long wtpSsidWirelessMacSendDataRightByte;	/*xiaodawei modify,20101116, ָ��SSID MAC�㷢�͵���ȷ�������ֽ���*/
	unsigned long wtpSsidWiredMacRecvDataWrongPack;	//ָ��SSID MAC����յĴ�������ݰ���
	unsigned long wtpNetWiredRecvPack;		//ָ��SSID ����⣨���߲ࣩ���յİ���
	unsigned long wtpUsrWirelessMacRecvDataPack;/*xiaodawei modify,20101116, ���߲�MAC���յ������ݰ���*/
	unsigned long wtpUsrWirelessMacSendDataPack;/*xiaodawei modify,20101116, ���߲�MAC�㷢�͵����ݰ���*/
	unsigned long wtpNetWiredSendPack;		//ָ��SSID ����ࣨ���߲ࣩ���͵İ���
	unsigned long WtpWirelessSendFailPkts;		//ָ��SSID ���߲෢��ʧ�ܵİ���
	unsigned long wtpWirelessResendPkts; 	//ָ��SSID ���߲��ܵ��ش�����
	char *wtpWirelessWrongPktsRate; 	//ָ��SSID ���߲�����
	unsigned long wtpWirelessSendBroadcastMsgNum;	//ָ��SSID ���߲෢�͵Ĺ㲥����
	unsigned long wtpStaUplinkMaxRate;		//ָ��SSID station�����������
	unsigned long wtpStaDwlinkMaxRate;		//ָ��SSID station�����������
	unsigned long wtpNetWiredRecvErrPack;		//ָ��SSID ����ࣨ���߲ࣩ���յĴ����
	unsigned long wtpNetWiredRecvRightPack;		//ָ��SSID ����ࣨ���߲ࣩ���յ���ȷ����
	unsigned long long wtpNetWiredRecvByte;		//ָ��SSID ����ࣨ���߲ࣩ���յ��ֽ���
	unsigned long long wtpNetWiredSendByte;		//ָ��SSID ����ࣨ���߲ࣩ���͵��ֽ���
	unsigned long wtpNetWiredSendErrPack;		//ָ��SSID ����ࣨ���߲ࣩ���͵Ĵ����
	unsigned long wtpNetWiredSendRightPack;		//ָ��SSID ����ࣨ���߲ࣩ���͵���ȷ����
	unsigned long wtpSsidSendDataAllPack;		//�ƶ�SSID ����ࣨ���߲ࣩ���͵����а���
	char *wtpNetWiredRxWrongPktsRate;	//ָ��SSID ����ࣨ���߲ࣩ���յĴ�����ٷֱ�
	char *wtpNetWiredTxWrongPktsRate;	//ָ��SSID ����ࣨ���߲ࣩ���͵Ĵ�����ٷֱ�
	unsigned int wtpSsidTxDataDropPkts;
	unsigned int wtpSsidRxDataDropPkts;
	
	struct WlanDataPktsInfo *next;
};
struct WtpWlanDataPktsInfo {
	long  wtpCurrID;			//AP��ǰID
	char *wtpMacAddr;           //AP��MAC��ַ
	unsigned int wlan_num;    //numbers of entries in wlan_list
	struct WlanDataPktsInfo* wlan_list;
	
	struct WtpWlanDataPktsInfo* next;
};




struct WtpWlanRadioInfo {
    unsigned char wlanid;
	unsigned int WlanradioRecvSpeed ;//����ʱ�����������ж˿�����
    unsigned int WlanradioSendSpeed ;//����ʱ�����������ж˿�����
    struct WtpWlanRadioInfo* next;
};

/*26 27 31*/
/*set ap extension infomation switch (enable|disable)*/
struct NewWtpWirelessIfInfo {	
	int  wtpCurrID;					//ap id						//
	unsigned char  NewapWirelessIfIndex;			//���߽ӿ�	//
	unsigned int wtpRadCurrID;				//radio id				//
	unsigned char wtpRadLocalID; 	//radio local id   //for showing
	unsigned char *wtpMacAddr;	     //AP��MAC��ַ		//
	
	unsigned short NewwtpWirelessIfBeaconIntvl;		//beacon ���		//					//t26,t31							
	unsigned char NewwtpWirelessIfDtimIntvl;			//DTIM			//					//t26,t31
	unsigned char NewwtpWirelessIfShtRetryThld;		//ShortRetry		//					//t26,t31
	unsigned char NewwtpWirelessIfLongRetryThld;		//LongRetry		//					//t26,t31
	unsigned int NewwtpWirelessIfMaxRxLifetime;	//Received data packets lifetime(units ms)			//t26
	unsigned char NewWtpPreambleLen;				//ǰ����			//					//t26
	unsigned short NewRtsThreshold;				//��Ƭ					//					//t26
	unsigned short NewFragThreshlod;				//��Ƭ				//					//t26

	unsigned int wtpWiredIfInMulticastPkts;	//���߽ӿڽ��յ��鲥����				//t27
	unsigned int wtpWiredIfOutMulticastPkts;	//���߽ӿڷ��͵��鲥����			//t27
	unsigned int wtpWirelessIfMaxRxLifetime;	//AP�������ݰ�������					//t31

    struct NewWtpWirelessIfInfo *next;
	struct NewWtpWirelessIfInfo *NewWtpWirelessIfInfo_list;
	struct NewWtpWirelessIfInfo *NewWtpWirelessIfInfo_last;
};

struct NewWtpWirelessIfstatsInfo_radio{

	unsigned char wtpWirelessIfIndex;				//AP���߽ӿ�����
	unsigned char wirelessIfUpdownTimes;				//AP���߽ӿ�UpDown����

	unsigned int wirelessIfChStatsUplinkUniFrameCnt;		//�����ŵ����е�����֡��
	unsigned int wirelessIfChStatsDwlinkUniFrameCnt;		//�����ŵ����е�����֡��
	unsigned int wirelessIfUpChStatsFrameNonUniFrameCnt;	//�����ŵ����зǵ�����֡��
	unsigned int wirelessIfDownChStatsFrameNonUniFrameCnt;	//�����ŵ����зǵ���֡��

	unsigned int wirelessIfApChStatsFrameFragRate;		//�ŵ���֡�ķֶ�����
	unsigned int wirelessIfChStatsNumStations;			//ʹ�ø��ŵ����ն���

	unsigned int wirelessIfRxMgmtFrameCnt;			//���յ��Ĺ���֡������
	unsigned int wirelessIfRxCtrlFrameCnt;			//���յ��Ŀ���֡������
	unsigned int wirelessIfRxDataFrameCnt;			//���յ�������֡������
	unsigned int wirelessIfRxAuthenFrameCnt;			//���յ�����֤֡������
	unsigned int wirelessIfRxAssociateFrameCnt; 		//���յ��Ĺ���֡������
	unsigned int wirelessIfTxMgmtFrameCnt;			//���͵Ĺ���֡������
	unsigned int wirelessIfTxCtrlFrameCnt;			//���͵Ŀ���֡������
	unsigned int wirelessIfTxDataFrameCnt;			//���͵�����֡������
	unsigned int wirelessIfTxAuthenFrameCnt;			//���͵���֤֡������
	unsigned int wirelessIfTxAssociateFrameCnt; 		//���͵Ĺ���֡������

	unsigned int radioWlanNum;
	unsigned char radioBindWlan[WLAN_NUM];	//�󶨵�wlan	
	struct NewWtpWirelessIfstatsInfo_radio *next;
	struct NewWtpWirelessIfstatsInfo_radio *NewWtpWirelessIfstatsInfo_radio_list;
	struct NewWtpWirelessIfstatsInfo_radio *NewWtpWirelessIfstatsInfo_radio_last;
};

struct NewWtpWirelessIfstatsInfo {
    int wtpCurrID;
    unsigned char *wtpMacAddr;
	unsigned char wtp_radio_num;
    unsigned char wirelessIfAvgRxSignalStrength;			//AP���߽ӿڽ��յ��ź�ƽ��ǿ��
    unsigned char wirelessIfHighestRxSignalStrength;		//AP���߽ӿڽ����źŵ����ǿ��
    unsigned char wirelessIfLowestRxSignalStrength;		//AP���߽ӿڽ����źŵ����ǿ��
    unsigned char wifiExtensionInfoReportswitch;		/*set ap extension infomation switch (enable|disable)*/
	
    unsigned int wirelessIfTxSignalPkts;			//���͵��������
    unsigned int wirelessIfRxSignalPkts;			//���յ��������
    unsigned int wirelessIfTxDataPkts;				//���͵����ݰ���//
    unsigned int wirelessIfRxDataPkts;				//���յ����ݰ���//
    unsigned long long wirelessIfUplinkDataOctets;			//���͵����ݰ��ֽ���//
    unsigned long long wirelessIfDwlinkDataOctets;			//���յ����ݰ��ֽ���//

    unsigned int wirelessIfChStatsDwlinkTotRetryPkts;		//�����ŵ������ش��İ���

	unsigned int wirelessIfChStatsPhyErrPkts;			//�ŵ����յ����������//
    unsigned int wirelessIfChStatsMacFcsErrPkts;		//�ŵ����յ�FCS MAC�����//
    unsigned int wirelessIfChStatsMacMicErrPkts;		//�ŵ����յ�MIC MAC�����//
    unsigned int wirelessIfChStatsMacDecryptErrPkts;		//�ŵ����յĽ���ʧ�ܵ�MAC�����//
    unsigned int wirelessIfChStatsFrameErrorCnt;		//�ŵ����յĴ�֡��				//
    unsigned int wirelessIfChStatsFrameRetryCnt;		//�ŵ������ش���֡��

	unsigned int asd_radio_num;						//asd ��radio num
	struct NewWtpWirelessIfstatsInfo_radio *NewWtpWirelessIfstatsInfo_radio_head;

    struct NewWtpWirelessIfstatsInfo *next;
	struct NewWtpWirelessIfstatsInfo *NewWtpWirelessIfstatsInfo_list;
	struct NewWtpWirelessIfstatsInfo *NewWtpWirelessIfstatsInfo_last;
};

struct 	Sub_RogueAPInfo
{
	int wtpID;
	int  rogueAPIndex;		//�Ƿ�AP������
	unsigned char rogueAPMac[MAC_LEN];		//�Ƿ�AP��MAC
	
	short rogueAPRate;		//�Ƿ�AP������
	unsigned char rogueAPChannel;		//�Ƿ�AP�����ŵ�
	
	unsigned char rogueAPRssi;		//�Ƿ�AP��Rssi
	unsigned char rogueAPNoise;		//�Ƿ�AP����
	unsigned char rogueAPBeaconInterval;	//�Ƿ�APbeacon���
	
	unsigned short rogueAPCapability;		//�Ƿ�AP����
	unsigned char rogueAPEssid[ESSID_DEFAULT_LEN+1];		//�Ƿ�AP ESSID
	char *rogueAPElemInfo;		//�Ƿ�AP��Ԫ��Ϣ
	
	struct Sub_RogueAPInfo *next;
	struct Sub_RogueAPInfo *Sub_RogueAPInfo_list;
	struct Sub_RogueAPInfo *Sub_RogueAPInfo_last;
};


/*table 29 dot11RogueAPTable*/
struct RogueAPInfo {	

	int wtpCurrID;
	unsigned int rogue_ap_num;
	unsigned char *wtpMacAddr;		//AP��MAC��ַ		
	
	struct Sub_RogueAPInfo *Sub_RogueAPInfo_head;
    struct RogueAPInfo *next;
    struct RogueAPInfo *RogueAPInfo_list;
    struct RogueAPInfo *RogueAPInfo_last;
};

/*table 30-2*/
struct Sub_SecurityMechInfo {	
	unsigned char  wlanSecID;				//���ް�ȫ����ID
	unsigned char  wlanCurrID;				//wlan ID
	char *wtpWirelessSecurMechName;			//��ȫ��������
	unsigned char wtpWirelessSecurMechID;			//��ȫ����ID
	unsigned int wtpWirelessSecurMechType;			//��ȫ��������
	unsigned int wtpWirelessSecurMechEncryType;		//��ȫ���Լ�������

	char *wtpWirelessSecurMechSecurPolicyKey;	//��ȫ������Կ
	int wtpWirelessSecurMechKeyInputType;		//��ȫ����������Կ
	unsigned int wlan_num;						//for showwing

    struct Sub_SecurityMechInfo *next;
	struct Sub_SecurityMechInfo *Sub_SecurityMechInfo_list;
	struct Sub_SecurityMechInfo *Sub_SecurityMechInfo_last;
	
};

/*table 30 dot11SecurityMechTable*/
struct SecurityMechInfo {	
    int wtpCurrID;
	unsigned char *wtpMacAddr;				//AP��MAC��ַ                
	unsigned int wlan_num;						//for showwing

	struct Sub_SecurityMechInfo *Sub_SecurityMechInfo_head;
    struct SecurityMechInfo *next;
	struct SecurityMechInfo *SecurityMechInfo_list;
	struct SecurityMechInfo *SecurityMechInfo_last;
	
};
/*table 33 dot11ConjunctionTable*/
struct WtpInfor{
    int wtpCurrID;
	unsigned char *wtpMacAddr;				//AP��MAC��ַ          
	
	unsigned int acc_tms;						//e1
	unsigned int auth_tms;						//e2
	unsigned int repauth_tms;					//e3
	
	unsigned int auth_success_num ; 			//b1
	unsigned int auth_fail_num ; 
	unsigned int auth_invalid_num ; 
	unsigned int auth_timeout_num ; 
	unsigned int auth_refused_num ; 			//b5
	
	unsigned int auth_others_num ; 				//b6
	unsigned int assoc_req_num ; 
	unsigned int assoc_resp_num ; 
	unsigned int assoc_invalid_num ; 
	unsigned int assoc_timeout_num ; 			//b10

	unsigned int assoc_success_num;			//assoc success times
	
	unsigned int assoc_refused_num ; 			//b11
	unsigned int assoc_others_num ; 
	unsigned int reassoc_request_num ; 
	unsigned int reassoc_success_num ; 
	unsigned int reassoc_invalid_num ; 			//b15
	
	unsigned int reassoc_timeout_num ; 
	unsigned int reassoc_refused_num ; 
	unsigned int reassoc_others_num ; 
	unsigned int identify_request_num ; 
	unsigned int identify_success_num ; 		//b20
	
	unsigned int abort_key_error_num ; 			//b21
	unsigned int abort_invalid_num ; 
	unsigned int abort_timeout_num ; 
	unsigned int abort_refused_num ; 
	unsigned int abort_others_num ; 			//b25
	
	unsigned int deauth_request_num ; 			//b26
	unsigned int deauth_user_leave_num  ;
	unsigned int deauth_ap_unable_num ;
	unsigned int deauth_abnormal_num ; 
	unsigned int deauth_others_num ; 			//b30
	
	unsigned int disassoc_request_num ; 		//b31
	unsigned int disassoc_user_leave_num ; 
	unsigned int disassoc_ap_unable_num ; 
	unsigned int disassoc_abnormal_num ; 
	unsigned int disassoc_others_num ; 			//b35

	unsigned int rx_mgmt_pkts ;					//c1
	unsigned int tx_mgmt_pkts ;
	unsigned int rx_ctrl_pkts ;
	unsigned int tx_ctrl_pkts ;					//c4
	
	unsigned int rx_data_pkts ;					//c5
	unsigned int tx_data_pkts ;
	unsigned int rx_auth_pkts ;
	unsigned int tx_auth_pkts ;					//c8

	unsigned long long wtp_total_online_time ;	/*	xm0703*///a1a2
	unsigned int num_assoc_failure ; /*	xm0703*/	//z1

	//zhanglei add 20101007
	unsigned int rx_assoc_norate; /*���ն˲�֧�ֻ������ʼ�Ҫ����������ʶ�����ʧ�ܵ��ܴ���*/
	unsigned int rx_assoc_capmismatch;	  /*�ɲ���802.11��׼�ƶ���Χ�ڵ�ԭ�������ʧ�ܵ��ܴ���*/
	unsigned int assoc_invaild;   /*δ֪ԭ������¹���ʧ�ܵ��ܴ���*/
	unsigned int reassoc_deny;	  /*����֮ǰ�Ĺ����޷�ʶ����ת�ƶ��������¹���ʧ�ܵ��ܴ���*/

	//mahz add 2011.5.5
	unsigned int num_assoc_record, num_reassoc_record, num_assoc_failure_record, num_reassoc_failure_record;
	unsigned int assoc_success_record, reassoc_success_record, assoc_req_record, assoc_resp_record;
	unsigned int auth_success_record, auth_fail_record,usr_auth_tms_record,ac_rspauth_tms_record;
	unsigned long long total_ap_flow_record;
	//
	
	struct WtpInfor *WtpInfor_list;
	struct WtpInfor *WtpInfor_last;
	struct WtpInfor *next;
	
};
/*nl add for mib showting wtp  infromation end*/
/*xdw add for show ap network information of all wtp, 20101215*/
struct WtpNetworkInfo {	
    unsigned int WTPID;
	unsigned char *WTPMAC;
	char * WTPIP;

	unsigned int ap_mask_new;	//ap mask
	unsigned int ap_gateway;	//ap gateway
	unsigned int ap_dnsfirst;	//ap first dns
	unsigned int ap_dnssecend;	//ap second dns
	
    struct WtpNetworkInfo *next;
	struct WtpNetworkInfo *WtpNetworkInfo_list;
	struct WtpNetworkInfo *WtpNetworkInfo_last;
};
struct WtpAthStatisticInfo{
    long wtpCurrID;   //AP��ǰID
    unsigned char *wtpMacAddr;
    long  wtpwirelessifindex; //radio_local_ID
    long wlanid;
    unsigned int wirelessIfUpdownTimes;//radio����ʱ���
    unsigned int wirelessIfTxSignalPkts; //���͵��������  apstatsinfo[i].tx_pkt_mgmt
    unsigned int wirelessIfRxSignalPkts; //���յ��������  apstatsinfo[i].rx_pkt_mgmt
 
    unsigned int wirelessIfTxDataPkts; //���͵����ݰ���//         apstatsinfo[i].tx_packets 
    unsigned int wirelessIfRxDataPkts; //���յ����ݰ���//         apstatsinfo[i].rx_packets 
    unsigned long long wirelessIfUplinkDataOctets; //���͵����ݰ��ֽ���//apstatsinfo[i].tx_bytes 
    unsigned long long wirelessIfDwlinkDataOctets; //���յ����ݰ��ֽ���//apstatsinfo[i].rx_bytes 
    unsigned int  wirelessIfChStatsDwlinkTotRetryPkts; //�����ش��İ���//apstatsinfo[i].tx_pkt_retry 
    unsigned int  wirelessIfChStatsFrameRetryCnt;	//apstatsinfo[i].tx_pkt_retry*103%
    unsigned int  wirelessifUplinkUniPktCnt;// ���е����İ��� //apstatsinfo[i].rx_unicast 
    unsigned int  wirelessifDwlinkUniPktCnt ;//���е����İ��� apstatsinfo[i].rx_unicast 
    unsigned int  wirelessifUpNonUniPktCnt  ;//���зǵ����İ��� apstatsinfo[i].rx_multicast ��Ϊ�ಥ�����Ѿ������˹㲥����
    unsigned int  wirelessifDownNonUniPktCnt;//��зǵ����İ��?
    unsigned int  wirelessifFrameFragRate ;//֡�ķֶ����� B
    unsigned int  wirelessifRxCtrlFrameCnt ;//���յ��Ŀ���֡������ B
    unsigned int  wirelessifRxDataFrameCnt ;//���յ�������֡������ B
    unsigned int  wirelessifRxAuthenFrameCnt ;//���յ�����֤֡������ B
    unsigned int  wirelessifRxAssociateFrameCnt ;//���յ��Ĺ���֡������ B
    unsigned int  wirelessifTxCtrlFrameCnt ;//���͵Ŀ���֡������ B
    unsigned int  wirelessifTxDataFrameCnt;//���͵�����֡������ B
    unsigned int  wirelessifTxAuthenFrameCnt ;//���͵���֤֡������  B
    unsigned int  wirelessifTxAssociateFrameCnt ; //���͵Ĺ���֡������ B
    unsigned int  wirelessIfSuccAssociatedNum;//associated successfully times, xiaodawei add, 20110418
    unsigned int  wirelessIfAssociatedUserNum;
    unsigned int  wirelessifNumStations; //��ǰ���ӵ���SSID���ն��� ��������
    unsigned int  wirelessIfTxMgmtFrameCnt; //���͵Ĺ���֡apstatsinfo[i].tx_pkt_mgmt ,zhaoruijia,20100903
    unsigned int  wirelessIfRxMgmtFrameCnt;//���յĹ���֡ apstatsinfo[i].rx_pkt_mgmt  ,zhaoruijia,20100903
    unsigned int  wirelessifUplinkCtrlOctets;//���͵Ĺ�����ֽ��� apstatsinfo[i].tx_mgmt 
    unsigned int  wirelessifDownlinkCtrlOctets;//���յĹ�����ֽ��� apstatsinfo[i].rx_mgmt 
    unsigned int wirelessIfChStatsMacFcsErrPkts; // 	�ŵ����յ�FCS MAC�����apstatsinfo[i].ast_rx_crcerr
	unsigned int wirelessIfChStatsMacMicErrPkts;//	�ŵ����յ�MIC MAC�����apstatsinfo[i].ast_rx_badmic
	unsigned int wirelessIfChStatsMacDecryptErrPkts;//	�ŵ����յĽ���ʧ�ܵ�MAC�����apstatsinfo[i].ast_rx_badcrypt
	unsigned int wirelessIfChStatsFrameErrorCnt; //�ŵ����յĴ�֡�� apstatsinfo[i].tx_errors

	unsigned int wirelessIfChStatsPhyErrPkts;  //�ŵ����յ����������  fengwenchao add 20101228
	unsigned char wirelessIfLowestRxSignalStrength ;    //AP���߽ӿڽ����źŵ����ǿ�� fengwenchao add 20101228
	unsigned char wirelessIfHighestRxSignalStrength;    //AP���߽ӿڽ����źŵ����ǿ�� fengwenchao add 20101228
	double wirelessIfAvgRxSignalStrength ;              // AP���߽ӿڽ��յ��ź�ƽ��ǿ��fengwenchao add 20101228

	char wirelessIfHighestRxSignalStrength2;			//AP���߽ӿڽ����źŵ����ǿ�� fengwenchao add 20101228
	char wirelessIfLowestRxSignalStrength2;	           //AP���߽ӿڽ����źŵ����ǿ�� fengwenchao add 20101228
	
	
	/*for dot11WtpStatisticsTable ,dot11WtpChannelTable fengwenchao add 20110330*/
	/*----------------------------------------------------------------*/
   	//unsigned int wtpRxPacketLossRate;           //�붪����
	//unsigned int wtpTxPacketLossRate;			//��������
	unsigned int wtpTotalRx_Drop;               //ath��rx_drop���ܺ�
	unsigned int wtpTotalTx_Drop;               //ath��tx_drop���ܺ�
	unsigned int wtpTotalRx_Pkt;                //ath��rx_pkt���ܺ�
	unsigned int wtpTotalTx_Pkt;                //ath��tx_pkt���ܺ�
	unsigned int wtpDownBandwidthUtilization;   //���д���������
	unsigned int wtpUpBandwidthUtilization;   	//���д���������
	unsigned int wtpReceiveRate;                //AP��������
	unsigned int wtpSendRate;                   //AP��������
	unsigned int sta_num;                       //�����û�����
	unsigned char monitor_time;                 //���ʱ��
	unsigned int ast_rx_phyerr;                 //���������
	unsigned int tx_packets;
	unsigned int tx_errors;
	unsigned int tx_retry;
	/*---------------------------------------------------------------*/


	/*for dot11WtpExtensionTable,dot11WtpWidStatisticsTable,dot11WtpBssIDNumTable  fengwenchao add 20110503*/
	/*----------------------------------------------------------------*/
	unsigned char wtpExtensionUsed;    //AP��չ��Ϣ����
	unsigned int wtpFloodAttackTimes;        //���鹥������
	unsigned int wtpSpoofingAttackTimes;   //Spoof��������
	unsigned int wtpWeakIVAttackTimes;     //WeakIV��������
	unsigned int BssIDTatolNum;              //BSSID����
	/*----------------------------------------------------------------*/
	unsigned long long wirelessIfDwlinkTotFrameCnt;	//tx_pkt_unicast+tx_pkt_multicast, tx_pkt_multicast(ap report)=multicast+broadcast 
	unsigned long long wirelessIfUplinkTotFrameCnt;	//rx_pkt_unicast+rx_pkt_multicast,rx_pkt_multicast(ap report)=multicast+broadcast
   	//qiuchen add for Guangdong mobile v3.2-1
   	unsigned int AllApUserOnlineTime;
	unsigned int APUserLostConnectionCnt;
	unsigned int APAuthReqCnt;
	unsigned int APAuthSucCnt;
	unsigned int APAuthFailCnt;
	struct WtpAthStatisticInfo	*next;
};

/*------------------------------------------------*/

struct WtpList{
	int WtpId;
	char FailReason;
	int WtpStaNum;
	//unsigned char wtpMacAddr[6];
	struct WtpList *next;
	struct WtpList *WtpList_list;
	struct WtpList *WtpList_last;
};

struct WtpOUIInfo_{
   unsigned char* oui_mac;
   unsigned int   oui_num;
   struct WtpOUIInfo_ *oui_list;
   struct WtpOUIInfo_ *oui_last;
   struct WtpOUIInfo_ *next;
};
typedef struct WtpOUIInfo_ WtpOUIInfo;
#endif

