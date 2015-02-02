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
* ACInterface.c
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

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#include "CWAC.h"
#include "wcpss/waw.h"

#include "wcpss/wid/WID.h"
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

int g_bRunTest = 0;

void CWInitQosValues(WTPQosValues* qosValues);

int CWActiveWTPsMenu(int* selection, int* err)
{
	int i, numActiveWTPs=0;
	CWBool firstTime = CW_TRUE;
	while(numActiveWTPs == 0){
		if(!CWErr(CWThreadMutexLock(&gActiveWTPsMutex))) 
		{
			wid_syslog_crit("Error locking the mutex");
			return -1;
		}
			numActiveWTPs = gActiveWTPs;
		CWThreadMutexUnlock(&gActiveWTPsMutex);
		
		for(i=0; i<WTP_NUM; i++)
		{
			if((gWTPs[i].isNotFree) && (gWTPs[i].currentState!=CW_ENTER_RUN))
			{
				numActiveWTPs--;
			}
		}
	
		if(numActiveWTPs>0)
		{
			printf("\n\t\t------ WTP ------\n\n");
			if (*err)
			{
				printf("ERROR: Wrong number or the selected WTP has closed the connection.\n");
				printf("Select another WTP or -1 to refresh.\n\n");
			}
			*err=0;
			printf("There are %d active WTP:\n\n", numActiveWTPs);
			printf("-3 - Exit\n\n");
			printf("-2 - Start Test\n\n");
			printf("-1 - Refresh list\n\n");
			if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) 
			{
				wid_syslog_crit("Error locking the mutex");
				return -1;
			}
				for(i=0; i<WTP_NUM; i++)
				{
					if(gWTPs[i].isNotFree)
						printf("%d - %s \n", i, gWTPs[i].WTPProtocolManager.name);
				}
			CWThreadMutexUnlock(&gWTPsMutex);
			printf("\nSelect a WTP: ");
			scanf("%d", selection);
			if(*selection == -1) {return 0;}
			if(*selection == -2) {return 6;}
			if(*selection == -3) {return 10;}
			if(*selection <0 || *selection >WTP_NUM) {*err=1; return 0;}
			return 1;
		}
		else {
			if(firstTime) {
				printf("There aren't active WTPs in Run State. Please wait... \n");
				firstTime = CW_FALSE;
			}
			sleep(1);
		}
		//else {*err=0; return 9;}
	}

	return -1;
}

int CWParameterMenu(int selection, int* parameter, int confirm, int* err)
{
	if(gWTPs[selection].isNotFree)
	{
		printf("\n\t\t------ PARAMETERS ------\n\n");
		if(*err ==1){
		printf("ERROR: Wrong selection for parameter.\n");
		}

		*err = 0;

		printf("Select a parameter to set for the WTP \"%s\":\n\n", gWTPs[selection].WTPProtocolManager.name);
		printf("1 - Set CWMin\n");
		printf("2 - Set CWMax\n");
		printf("3 - Set AIFS\n");
		if (confirm!=1){printf("0 - Back\n");}
		printf("\n");
		scanf("%d", parameter);
		if (*parameter==0) {return 0;}
		if(*parameter<0 || *parameter>3){
			*err = 1;
			return 1;
		}
		return 2;
	}
	else {
		*err = 1;
		return 0;
	}
	
}

int CWQueueMenu(int selection, int* queue, int* err)
{
	printf("\n\t\t------ QUEUES ------\n\n");
	if(*err==1){
		printf("ERROR: Wrong selection for queue.\n");
	}
	*err=0;
	printf("Select the Queue to set for the WTP \"%s\":\n\n", gWTPs[selection].WTPProtocolManager.name);
	printf("1 - Set Queue Voice\n");
	printf("2 - Set Queue Video\n");
	printf("3 - Set Queue Best Effort\n");
	printf("4 - Set Queue Background\n");
	printf("0 - Back\n");
	printf("\n");
	scanf("%d", queue);
	if (*queue==0) {return 1;}
	if(*queue<0 || *queue>4){*err=1;return 2;}
	(*queue)--;
	return 3;
}

int CWGetValueMenu(int* value)
{
	printf("\n\t\t------ VALUE ------\n\n");
	printf("0 - Back\n\n");
	printf("Insert the value:");
	scanf("%d", value);
	if (*value==0) {return 2;}
	return 4;	
}

void CWSummary(WTPQosValues* qosValues){
	int i;

	printf("\n\t------ SUMMARY ------\n");
	
	printf("\n---------------------------------------\n");
	printf("\t\tCWMIN\tCWMAX\tAIFS\t\n");
	printf("---------------------------------------\n");
	
	for(i=0; i<NUM_QOS_PROFILES; i++){
		if(i==0) printf(" VOICE\t\t");
		if(i==1) printf(" VIDEO\t\t");
		if(i==2) printf(" BEST EFFORT\t");
		if(i==3) printf(" BACKGROUND\t");
		if(qosValues[i].cwMin != UNUSED_QOS_VALUE) printf("  %d",qosValues[i].cwMin);
		else printf(" ");
		if(qosValues[i].cwMax != UNUSED_QOS_VALUE) printf("\t  %d",qosValues[i].cwMax);
		else printf("\t");
		if(qosValues[i].AIFS != UNUSED_QOS_VALUE) printf("\t %d",qosValues[i].AIFS);
		else printf("\t");
		printf("\t\n");
	}
	
	printf("---------------------------------------\n");
	return;
}

int CWConfirm(int *confirm)
{
	printf("\n\t\t------ CONFIRM ------\n\n");
	printf("1 - Set another parameter\n");
	printf("2 - Send settings\n");
	printf("0 - Back\n");
	scanf("%d", confirm);
	if (*confirm==0) {return 3;}
	if ((*confirm!=1) && (*confirm!=2)) {return 4;}
	return 5;
}

int CWSetValues(int selection, int parameter, int queue, int value, int confirm, WTPQosValues* qosValues)
{
	switch(parameter)
	{
		case 1:{qosValues[queue].cwMin=value; break;}
		case 2:{qosValues[queue].cwMax=value; break;}
		case 3:{qosValues[queue].AIFS=value; break;}
		default: {break;}
	}
	
	if(confirm==1) {return 1;}
	if(confirm==2) 
	{
		printf("\nSetting requested parameters. Please wait...\n");

#ifdef DMALLOC
		dmalloc_log_stats();
#endif

		CWThreadMutexLock(&(gWTPs[selection].interfaceMutex));

		gWTPs[selection].qosValues = qosValues;
		gWTPs[selection].interfaceCommand = QOS_CMD;
		CWSignalThreadCondition(&gWTPs[selection].interfaceWait);
		CWWaitThreadCondition(&gWTPs[selection].interfaceComplete, &gWTPs[selection].interfaceMutex);

		CWThreadMutexUnlock(&(gWTPs[selection].interfaceMutex));

#ifdef DMALLOC		
		dmalloc_log_stats();
#endif

		CWInitQosValues(qosValues);
		return 0;
	}	

	return 4;
}

CW_THREAD_RETURN_TYPE CWThreadTest(void *arg)
{
	wid_pid_write_v2("CWThreadTest",0,vrrid);
	int j;
	int nIndex = (long)arg;
	CWBool bOk = CW_TRUE;
	WTPQosValues* qosValues;

	//
	CW_CREATE_ARRAY_ERR(qosValues, NUM_QOS_PROFILES, WTPQosValues, return NULL;);

//	printf("run %d\n", nIndex);
	wid_syslog_debug_debug(WID_DEFAULT,"run %d\n", nIndex);

	while (bOk && (g_bRunTest != 0))
	{
		if (gWTPs[nIndex].isNotFree && (gWTPs[nIndex].currentState == CW_ENTER_RUN))
		{
			for (j = 0; j < NUM_QOS_PROFILES; j++)
			{
				qosValues[j].cwMin = rand() % 5 + 1;
				qosValues[j].cwMax = qosValues[j].cwMin + rand() % 5;
				qosValues[j].AIFS = rand() % 10 + 1;
			}

			CWThreadMutexLock(&(gWTPs[nIndex].interfaceMutex));

			gWTPs[nIndex].qosValues = qosValues;
			gWTPs[nIndex].interfaceCommand = QOS_CMD;
			CWSignalThreadCondition(&gWTPs[nIndex].interfaceWait);
			CWWaitThreadCondition(&gWTPs[nIndex].interfaceComplete, &gWTPs[nIndex].interfaceMutex);

			bOk = ((gWTPs[nIndex].interfaceResult == 1) ? CW_TRUE : CW_FALSE);

			CWThreadMutexUnlock(&(gWTPs[nIndex].interfaceMutex));
		}
	}

//	printf("exit %d\n", nIndex);
	wid_syslog_debug_debug(WID_DEFAULT,"exit %d\n", nIndex);

	CW_FREE_OBJECT(qosValues);
	return NULL;
}

int CWStartTest()
{
	long i;

	g_bRunTest = 1;
	for(i=0; i<WTP_NUM; i++)
	{
		CWThread thread_interface;
		
		if(gWTPs[i].isNotFree && (gWTPs[i].currentState == CW_ENTER_RUN))
			CWCreateThread(&thread_interface, CWThreadTest, (void*)i,0);
	}

	return 7;
}

int CWWaitTest()
{
	int stop;

//	printf("\n\t\t------ TEST ------\n\n");
//	printf("Test is running...\n");
//	printf("Enter 0 to stop it ");
	scanf("%d", &stop);
	if(stop==0)
	{
		g_bRunTest = 0;
		return 0;
	}
	else
 	{
		return 7;
	}
}

int CWNoActiveWTPsMenu(int* selection)
{
	*selection=0;
	do
	{
//		printf("There aren't active WTP\n");
		wid_syslog_debug_debug(WID_DEFAULT,"There aren't active WTP\n");
//		printf("Enter -1 to refresh: ");
		scanf("%d", selection);
		fflush(stdin);
		fflush(stdout);
	}while(*selection!=-1);
	return 0;
}

void CWInitQosValues(WTPQosValues* qosValues)
{
	int i;
	
	for(i=0; i<NUM_QOS_PROFILES; i++)
	{
		qosValues[i].queueDepth=UNUSED_QOS_VALUE;
		qosValues[i].cwMin=UNUSED_QOS_VALUE;
		qosValues[i].cwMax=UNUSED_QOS_VALUE;
		qosValues[i].AIFS=UNUSED_QOS_VALUE;
		qosValues[i].dot1PTag=UNUSED_QOS_VALUE;
		qosValues[i].DSCPTag=UNUSED_QOS_VALUE;
	}
}

CW_THREAD_RETURN_TYPE CWInterface(void* arg)
{
	int err=0, selection=0, parameter=0, queue=0, value=0, confirm=0, step=0;
	WTPQosValues* qosValues = NULL;
	/* Coverity: CID: 10193  Error-Type: Resource leak */
	CW_CREATE_ARRAY_ERR(qosValues, NUM_QOS_PROFILES, WTPQosValues, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL); return 0;});
	CWInitQosValues(qosValues);

	CW_REPEAT_FOREVER
	{
		switch(step)
		{
			case -1: {CW_FREE_OBJECT(qosValues); return 0;}
			case 0: {step=CWActiveWTPsMenu(&selection, &err); break;}
			case 1: {step=CWParameterMenu(selection, &parameter, confirm, &err); break;}
			case 2: {step=CWQueueMenu(selection, &queue, &err); break;}
			case 3: {step=CWGetValueMenu(&value); break;}
			case 4: {step=CWConfirm(&confirm); break;}
			case 5: {step=CWSetValues(selection, parameter, queue, value, confirm, qosValues); break;}
			case 6: {step=CWStartTest(); break;}
			case 7: {step=CWWaitTest(); break;}
			case 9: {step=CWNoActiveWTPsMenu(&selection); break;}
			case 10: 
			{
#ifdef DMALLOC
				dmalloc_shutdown(); 
#endif
				exit(0); 
			}
			
			default: 
			{
				step=0; 
				break;
			}
		}
	}

	return 0;
}

int wid_new_sem(){
	
	key_t key;
	int semid;	
	key = ftok(AFC_RUN_PAH"/widsem",0);
	semid = semget(key,0,0);
	if (-1 == semid){
	  //printf("create semaphore error\n");
	  wid_syslog_crit("create semaphore error");
	  exit(-1);
	}
	return semid;
}

void wait_v(int semid)
{
	struct sembuf sops={0,0,0};
	semop(semid,&sops,1);
}

CW_THREAD_RETURN_TYPE CWInterface1(void * arg)
{	int nIndex = 0;
//	int wIndex = 0;
//	int semid;
//	semid = wid_new_sem();
	while(1){
		
		CWThreadMutexLock(&(gAllThreadMutex));		
		CWThreadMutexLock(&(gACInterfaceMutex));
//		state = 1;
//		printf("state 2\n");
		for(nIndex=0;nIndex<WTP_NUM;nIndex++){
			if(AC_WTP[nIndex] != NULL){				
//				state = 1;
			if(gWTPs[nIndex].isNotFree && (gWTPs[nIndex].currentState == CW_ENTER_RUN)){
				CWThreadMutexLock(&(gWTPs[nIndex].WTPThreadMutex));
					while((AC_WTP[nIndex]->CMD->wlanCMD != 0))
					{	
						wid_syslog_debug_debug(WID_DEFAULT,"wtp%d wlanCMD:%d\n",nIndex,AC_WTP[nIndex]->CMD->wlanCMD);
						if(gWTPs[nIndex].isNotFree && (gWTPs[nIndex].currentState == CW_ENTER_RUN)){
							CWThreadMutexLock(&(gWTPs[nIndex].interfaceMutex));			
							gWTPs[nIndex].interfaceCommand = 1;
							CWSignalThreadCondition(&gWTPs[nIndex].interfaceWait);
							CWWaitThreadCondition(&gWTPs[nIndex].interfaceComplete, &gWTPs[nIndex].interfaceMutex);
							CWThreadMutexUnlock(&(gWTPs[nIndex].interfaceMutex));
						}else
							break;
					}
				CWThreadMutexUnlock(&(gWTPs[nIndex].WTPThreadMutex));
			}

			}
		}
		
		CWThreadMutexUnlock(&gACInterfaceMutex);		
		CWThreadMutexUnlock(&(gAllThreadMutex));		
		CWThreadMutexLock(&(gACInterfaceMutex));
//		state = 0;
		CWWaitThreadCondition(&gACInterfaceWait, &gACInterfaceMutex);
		CWThreadMutexUnlock(&gACInterfaceMutex);
//	p(WidSemid);
	}
}


CW_THREAD_RETURN_TYPE CWThreadWD(void * arg)
{
	return NULL;
	
	FILE *fp = NULL;
	char filename[PATH_LEN] = {0};	
	char filename1[PATH_LEN] = {0};
	unsigned int cnt = 0;
	//unsigned int sleep_time = 60;
	char buff[128] = {0};
	unsigned int times = 0;
	unsigned int stat;
	int i = 0;
	sprintf(filename,"./wid_watch_dog%d",vrrid);	
	sprintf(filename1,"/tmp/.wid_dump%d",vrrid);
	fp = fopen(filename, "w");
	if (!fp)
	{
		return NULL;
	}
	wid_pid_write_v2("CWThreadWD",0,vrrid);
	/* Build shell script */
	fputs("#!/bin/bash\n\n", fp);
	fputs("IF=$1\n", fp);
	fputs("IP=$2\n", fp);
	fputs("FILE=$3\n", fp);
	fputs("TCPDUMP=/usr/sbin/tcpdump\n", fp);
	fputs("GAP_TIME=20\n", fp);
	fputs("$TCPDUMP -i $IF -n \"host $IP &&(udp port 5246 || udp port 1234)\" 1>$FILE 2>/dev/null &\n", fp);
	fputs("PID=$!\n", fp);
	fputs("sleep $GAP_TIME\n", fp);
	fputs("kill $PID\n", fp);
	fputs("COUNT=$(cat $FILE | grep -v ^$ | wc -l | awk '{print$1}')\n", fp);
	fputs("exit $COUNT\n", fp);
	fclose(fp);

	/* Set permission */
	sprintf(buff, "chmod a+x %s", filename);
	system(buff);
	while(1)
	{
		if(WID_WATCH_DOG_OPEN == 0)
		{
			sleep(60);
			continue;
		}
		memset(buff, 0, 128);
		sprintf(buff, "%s %s %s %s", filename,"any","255.255.255.255",filename1);
		printf("buff %s\n",buff);		

		stat = system(buff);		
		cnt = WEXITSTATUS(stat);	
		printf("cnt %d\n",cnt);
		printf("ifname any count %d\n",G_LocalHost_num);		
		if (cnt != 0)
		{
			if(G_LocalHost_num == 0)
			{
				times ++;
				if(times > 3)
				{
					wid_syslog_crit("Ai,AC localhost can't recv AP Broadcast, niaoxue exit\n");
					exit(1);
				}
			}
			else
			{
				G_LocalHost_num = 0;
				times = 0;
			}
		}
		for(i=0; i<gMaxInterfacesCount; i++)
		{
			if((gInterfaces[i].enable == 0)||(gInterfaces[i].tcpdumpflag == 0))
				continue;
			memset(buff, 0, 128);
			sprintf(buff, "%s %s %s %s", filename,gInterfaces[i].ifname,gInterfaces[i].ip,filename1);	
			printf("buff2222 %s\n",buff);		
			stat = system(buff);		
			cnt = WEXITSTATUS(stat);				
			printf("cnt %d\n",cnt);
			printf("ifname %s count %d\n",gInterfaces[i].ifname,gInterfaces[i].datacount);		
			if (cnt != 0)
			{
				if(gInterfaces[i].datacount== 0){
					gInterfaces[i].times ++;
					if(gInterfaces[i].times > 3){
						wid_syslog_crit("Ai,AC %s can't recv AP Broadcast, niaoxue exit\n",gInterfaces[i].ifname);
						exit(1);
					}
				}else{
					gInterfaces[i].datacount = 0;
					gInterfaces[i].times = 0;
					gInterfaces[i].tcpdumpflag = 0;
					continue;
				}				
			}
			if(gInterfaces[i].WTPCount !=0){
				gInterfaces[i].datacount = 0;
				gInterfaces[i].times = 0;
				gInterfaces[i].tcpdumpflag = 0;
			}
		}
		sleep(60);
	}
}



