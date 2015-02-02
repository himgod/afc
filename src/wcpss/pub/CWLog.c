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
* CWLog.c
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

#include "CWCommon.h"
#include <syslog.h>
#include <assert.h>
#include "wcpss/wid/WID.h"
#include "CWAC.h"


//#define WRITE_STD_OUTPUT 1 

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

static FILE *gLogFile = NULL;

#ifndef CW_SINGLE_THREAD
CWThreadMutex gFileMutex;
#endif
int gWIDLogdebugLevel = WID_SYSLOG_DEBUG_DEFAULT;
int gWIDLOGLEVEL = WID_SYSLOG_DEBUG_ALL;
char gLogFileName[]=LOG_FILE_NAME;

pthread_key_t thread_log_key;


void CWLogInitFile(char *fileName) {
	if(fileName == NULL) {
		wid_syslog_debug_debug(WID_DEFAULT,"Wrong File Name for Log File");
	}

	if((gLogFile = fopen(fileName, "a")) == NULL) {
		wid_syslog_crit("Can't open log file: %s", strerror(errno));
		exit(1);
	}

#ifndef CW_SINGLE_THREAD
	if(!CWCreateThreadMutex(&gFileMutex)) {
		wid_syslog_crit("Can't Init File Mutex for Log");
		exit(1);
	}
#endif
}


CWBool checkResetFile()
{
	long fileSize=0;

	if((fileSize=ftell(gLogFile))==-1)
	{
		wid_syslog_crit("An error with log file occurred: %s", strerror(errno));
		return 0;
	}
	if (fileSize>=gMaxLogFileSize)
	{
		fclose(gLogFile);
		if((gLogFile = fopen(gLogFileName, "w")) == NULL) 
		{
			wid_syslog_crit("Can't open log file: %s", strerror(errno));
			return 0;
		}
	}
	return 1;
}


void CWLogCloseFile() {
#ifndef CW_SINGLE_THREAD
	CWDestroyThreadMutex(&gFileMutex);
#endif

	fclose(gLogFile);
}

__inline__ void CWVLog(const char *format, va_list args) {
	char *logStr = NULL;
	time_t now;
	char *nowReadable = NULL;

	if(format == NULL) return;

	now = time(NULL);
	nowReadable = ctime(&now);

	nowReadable[strlen(nowReadable)-1] = '\0';

	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);

	//sprintf(logStr, "[CAPWAP::%s]\t\t %s\n", nowReadable, format);
	sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\n", nowReadable, (unsigned int)CWThreadSelf(), format);

	if(gLogFile != NULL) {
		char fileLine[256];

#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gFileMutex);
		fseek(gLogFile, 0L, SEEK_END);
#endif

		vsnprintf(fileLine, 255, logStr, args);

		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}

		fwrite(fileLine, strlen(fileLine), 1, gLogFile);
		fflush(gLogFile);

#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gFileMutex);
#endif
	}
#ifdef WRITE_STD_OUTPUT
	vprintf(logStr, args);
#endif	

	CW_FREE_OBJECT(logStr);
}

__inline__ void CWLog(const char *format, ...) {
	va_list args;

	va_start(args, format);
	if (gEnabledLog)
	{CWVLog(format, args);}
	va_end(args);
}

__inline__ void CWDebugLog(const char *format, ...) {
#ifdef CW_DEBUGGING
	char *logStr = NULL;
	va_list args;
	time_t now;
	char *nowReadable = NULL;

	if (!gEnabledLog) {return;}

	if(format == NULL) {
#ifdef WRITE_STD_OUTPUT
		printf("\n");
#endif
		return;
	}

	now = time(NULL);
	nowReadable = ctime(&now);

	nowReadable[strlen(nowReadable)-1] = '\0';

	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);

	//sprintf(logStr, "[[CAPWAP::%s]]\t\t %s\n", nowReadable, format);
	sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\n", nowReadable, (unsigned int)CWThreadSelf(), format);

	va_start(args, format);

	if(gLogFile != NULL) {
		char fileLine[256];

#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gFileMutex);
		fseek(gLogFile, 0L, SEEK_END);
#endif

		vsnprintf(fileLine, 255, logStr, args);

		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}

		fwrite(fileLine, strlen(fileLine), 1, gLogFile);

		fflush(gLogFile);

#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gFileMutex);
#endif
	}
#ifdef WRITE_STD_OUTPUT	
	vprintf(logStr, args);
#endif

	va_end(args);
	CW_FREE_OBJECT(logStr);
#endif
}

__inline__ void WIDVLog(int level,const char *format, va_list args) {
	char *logStr = NULL;
	time_t now;
	char *nowReadable = NULL;
	char widloglevel[20];	
	if(format == NULL) return;

	now = time(NULL);
	nowReadable = ctime(&now);

	nowReadable[strlen(nowReadable)-1] = '\0';

	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);

	switch(level){
		case WID_SYSLOG_EMERG:
			strcpy(widloglevel, "Emerg");
			break;

		case WID_SYSLOG_DEBUG_INFO:
			strcpy(widloglevel, "Info");
			break;

		case WID_SYSLOG_CRIT:
			strcpy(widloglevel, "Crit");
			break;

		case WID_SYSLOG_ERR:
			strcpy(widloglevel, "Err");
			break;

		case WID_SYSLOG_WARNING:
			strcpy(widloglevel, "Warning");
			break;

		case WID_SYSLOG_NOTICE:
			strcpy(widloglevel, "Notice");
			break;

		case WID_SYSLOG_INFO:
			strcpy(widloglevel, "Info");
			break;

		case WID_SYSLOG_DEBUG:
			strcpy(widloglevel, "Debug");
			break;

		case WID_SYSLOG_DEBUG_DEBUG:
			strcpy(widloglevel, "Debug");
			break;

		default :
			strcpy(widloglevel, "All");
			break;
	}
	sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\t%s\n", nowReadable, (unsigned int)CWThreadSelf(),widloglevel,format);

	if(gLogFile != NULL) {
		char fileLine[256];

#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gFileMutex);
		fseek(gLogFile, 0L, SEEK_END);
#endif

		vsnprintf(fileLine, 255, logStr, args);

		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}

		fwrite(fileLine, strlen(fileLine), 1, gLogFile);
		fflush(gLogFile);

#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gFileMutex);
#endif
	}
#ifdef WRITE_STD_OUTPUT
	vprintf(logStr, args);
#endif	

	CW_FREE_OBJECT(logStr);
}
__inline__ void WID_Log(int level,const char *format, ...) 
{
	va_list args;
	va_start(args, format);
	if (gEnabledLog)
	{
		if ( level == WID_SYSLOG_DEBUG_DEBUG )
		{
			if (gWIDLogdebugLevel & level)
			{
				level = WID_SYSLOG_DEBUG;
				WIDVLog(level,format, args);
			}
		}
		else if ( level == WID_SYSLOG_DEBUG_INFO)
		{
			if (gWIDLogdebugLevel & level)
			{
				level = WID_SYSLOG_DEBUG;
				WIDVLog(level,format, args);}
		}
		else 
			WIDVLog(level,format, args);
	}
	va_end(args);

}
//sz20080927




void wid_syslog_emerg(char *format,...)
{
	char buf[2048] = {0};

#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent, "<emerg>");
	}
#endif
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_EMERG,"%s",buf);
}

void wid_syslog_alert(char *format,...)
{
	char buf[2048] = {0};

#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent, "<alert>");
	}
#endif
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_ALERT,"%s",buf);
}

void wid_syslog_crit(char *format,...)
{
	char buf[2048] = {0};

#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent, "<crit>");
	}
#endif

	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);

	va_end(ptr);
	syslog(LOG_CRIT,"%s",buf);
}

void wid_syslog_err(char *format,...)
{
	char buf[2048] = {0};

#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent, "<err>");
	}
#endif

	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_ERR,"%s", buf);
}

void wid_syslog_warning(char *format,...)
{
	char buf[2048] = {0};

#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent, "<warn>");
	}
#endif

	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_WARNING,"%s",buf);
}

void wid_syslog_notice(char *format,...)
{
	char buf[2048] = {0};

#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent, "<notice>");
	}
#endif

	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_NOTICE,"%s",buf);
}

void wid_syslog_notice_local7(char *format,...)
{
	char buf[2048] = {0};
#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s ", indent);
	}
#endif
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_NOTICE,"%s",buf);
}

void wid_syslog_info(char *format,...)
{
	char buf[2048] = {0};
#if WID_THREAD_LOG
	char *indent = (char *)pthread_getspecific(thread_log_key);

	if (NULL != indent)
	{
		sprintf(buf,"%s %s ", indent,"<info>");
	}
#endif

	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_INFO,"%s",buf);
}

void wid_syslog_debug_debug(int type,char *format,...)
{
	char buf[2048] = {0};

	if(gWIDLOGLEVEL & type)
	{
#if WID_THREAD_LOG
		char *indent = (char *)pthread_getspecific(thread_log_key);

		if (NULL != indent)
		{
			sprintf(buf,"%s ", indent);
		}
#endif
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+strlen(buf),format,ptr);
		va_end(ptr);
		syslog(LOG_DEBUG,"%s",buf);
	}
}

void wid_syslog_debug_info(char *format,...)
{
//	char ident[20] = {0};
	int wid_log_level = WID_SYSLOG_DEBUG_INFO;
	char buf[2048] = {0};


	//first,write to 'ac.log.txt' which is belong to wid itself
	//if 'ac.log.txt' will be removed,delete this step
	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);

	//start syslog writing
	if(gWIDLOGLEVEL & wid_log_level)
	{
#if WID_THREAD_LOG
		char *indent = (char *)pthread_getspecific(thread_log_key);

		if (NULL != indent)
		{
			sprintf(buf,"%s ", indent);
		}
#endif

	//	openlog(ident, 0, LOG_DAEMON);
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+strlen(buf),format,ptr);
		va_end(ptr);
		syslog(LOG_DEBUG,"%s",buf);
	//	closelog();
	}
}

void wid_syslog_debug(char *format,...)
{
//	char ident[20] = {0};
	int wid_log_level = WID_SYSLOG_DEBUG;
	char buf[2048] = {0};

	//first,write to 'ac.log.txt' which is belong to wid itself
	//if 'ac.log.txt' will be removed,delete this step
	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);

	//start syslog writing
	if(gWIDLOGLEVEL & wid_log_level)
	{ 
#if WID_THREAD_LOG
		char *indent = (char *)pthread_getspecific(thread_log_key);

		if (NULL != indent)
		{
			sprintf(buf,"%s %s ", indent, "<dbg>");
		}
#endif
	//	openlog(ident, 0, LOG_DAEMON);
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+strlen(buf),format,ptr);
		va_end(ptr);
		syslog(LOG_DEBUG,"%s",buf);
	//	closelog();
	}
}

void wid_pid_write(unsigned int vrrid)
{
	char pidBuf[128] = {0}, pidPath[128] = {0};
	pid_t myPid = 0;
	int fd = -1;
#ifndef _DISTRIBUTION_
	sprintf(pidPath,"%s%d%s", "/var/run/wcpss/wid", \
			vrrid, ".pid");
#else
	sprintf(pidPath,"%s%d_%d%s", "/var/run/wcpss/wid", \
			local,vrrid, ".pid");
#endif

	fd = open(pidPath, O_RDWR|O_CREAT);
	myPid = getpid();	
	sprintf(pidBuf,"%d\n", myPid);
	write(fd, pidBuf, strlen(pidBuf));
	close(fd);
	return;
}


void wid_pid_write_v2(char *name,int id,unsigned int vrrid)
{
	char pidBuf[128] = {0}, pidPath[128] = {0};
	char idbuf[128] = {0};
	pid_t myPid = 0;
	static int fd;
	static unsigned char opened = 0;
#ifndef _DISTRIBUTION_
	sprintf(pidPath,"%s%d%s", AFC_RUN_PAH"/wid_thread", vrrid, ".pid");
#else
	sprintf(pidPath,"%s%d_%d%s", AFC_RUN_PAH"/wid_thread", local, vrrid, ".pid");
#endif
	if(!opened)
	{

		fd = open(pidPath, O_RDWR|O_CREAT);
		if(fd < 0)
		{
			wid_syslog_err(WAI_FUNC_LINE_FMT""WAI_BOOT_FMT"open %s failed\n", \
					WAI_FUNC_LINE_LITERAL, pidPath);
			return ;
		}
		opened = 1;
	}

	if(id != 0)
	{
		sprintf(idbuf,"HANSI[%d]%s[%d]",vrrid,name,id);
	}
	else
	{
		sprintf(idbuf,"HANSI[%d]%s",vrrid,name);
	}

	myPid = getpid();	
	sprintf(pidBuf,"%s-%d\n",idbuf,myPid);
	write(fd, pidBuf, strlen(pidBuf));
	return;
}


void wid_thread_log(char *indent)
{
	char *thread_log = NULL;

	if (NULL == indent)
	{
		return;
	}

	thread_log = (char *)malloc(WID_LOG_INDENT_LEN);
	if (NULL == thread_log)
	{
		wid_syslog_err(WAI_FUNC_LINE_FMT"malloc failed : %s",WAI_FUNC_LINE_LITERAL,strerror(errno));
		return;
	}
	memset(thread_log, 0, WID_LOG_INDENT_LEN);	
	snprintf(thread_log, WID_LOG_INDENT_LEN-1, "%s", indent);
	pthread_setspecific(thread_log_key, thread_log);

}

void wid_close_thread_log(void* thread_log)
{
	char *tmp = NULL;

	tmp = (char *)thread_log;
	if (NULL != tmp)
	{
		free(tmp);
	}
}

/*****************************************************************************
 *	mac2str
 * 
 *	mac to strig
 *
 *  INPUT:
 *		haddr - mac address 
 *  
 *  OUTPUT:
 * 	 NULL
 *
 *  RETURN:
 * 	 static_buferr - mac string
 * 	 NULL
 *
 ****************************************************************************/

char *mac2str(unsigned char *haddr)
{
	static int count = 0;
	static unsigned char buf[STATIC_BUFFER_SIZE][MAX_MAC_STRING_LEN];
	int len = MAX_MAC_STRING_LEN;	
	unsigned char *tmp = NULL;

	count++;
	if (count >= STATIC_BUFFER_SIZE)
	{
		count = 0;
	}
	
	tmp = (unsigned char *)&(buf[count][0]);
	
	memset(tmp, 0, len);
	if (NULL != haddr)
	{
		snprintf((char *)tmp, MAX_MAC_STRING_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
				 haddr[0], haddr[1], haddr[2],
				 haddr[3], haddr[4], haddr[5]);
	}
	
	return (char *)tmp;
}

/**********************************************************************
 *	u32ip2str
 * 
 *	IPv4 address to string (EXP: 0x0a01010a -> 10.1.1.10)
 *
 *  INPUT:
 *		u32_ipaddr - IPv4 address 
 *  
 *  OUTPUT:
 * 	 NULL
 *
 *  RETURN:
 * 	 char * - ipv4 address string
 * 	 NULL - failed
 *
 **********************************************************************/
char *u32ip2str(unsigned int u32_ipaddr)
{	
	struct in_addr inaddr;

	memset(&inaddr, 0, sizeof(struct in_addr));

	inaddr.s_addr = u32_ipaddr;

	return inet_ntoa(inaddr);
}
