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
* wai_log.c
*
*
* CREATOR:
* autelan.rdcenter.wireless-controller.softac-team
*
* DESCRIPTION:
* log for minor-Y
*
*
*******************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <syslog.h>

#include "wai_public.h"
#include "wai_log.h"

int	g_wai_log_fd = -1;
int	g_wai_syslog_open = 0;

void wai_syslog_debug(char *format,...)
{
	char buf[512] = {0};
	char *indent = "<dbg>";
	
	if(!g_wai_syslog_open) {
		openlog("wai", 0, LOG_DAEMON);
		g_wai_syslog_open = 1;
	}

	if(!indent) {
		return;
	}
	
	snprintf(buf, strlen(indent), "%s", indent);
	
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_ERR,"%s", buf);
}

void wai_syslog_err(char *format,...)
{
	char buf[512] = {0};
	char *indent = "<err>";
	
	if(!g_wai_syslog_open) {
		openlog("wai", 0, LOG_DAEMON);
		g_wai_syslog_open = 1;
	}

	if(!indent) {
		return;
	}
	
	snprintf(buf, strlen(indent), "%s", indent);
	
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_ERR,"%s", buf);
}


void wai_syslog_warning(char *format,...)
{
	char buf[512] = {0};
	char *indent = "<warn>";
	
	if(!g_wai_syslog_open) {
		openlog("wai", 0, LOG_DAEMON);
		g_wai_syslog_open = 1;
	}

	if(!indent) {
		return;
	}
	
	snprintf(buf, strlen(indent), "%s", indent);
	
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buf+strlen(buf),format,ptr);
	va_end(ptr);
	syslog(LOG_ERR,"%s", buf);
}

void wai_open_log()
{
	if(g_wai_log_fd < 0) 
	{
		/* open file with create mode 0666 */
		g_wai_log_fd = open(WAI_LOG_FILE_PATH"/"WAI_LOG_FILE_NAME, O_CREAT|O_RDWR|O_APPEND, \
							S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH);
		if( g_wai_log_fd < 0) 
		{
			wai_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"open wai log %s error(%d) %s\n", \
						WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME, errno, strerror(errno));
			return ;
		}
		wai_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"open wai log %s %d\n", \
					WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME, g_wai_log_fd);
	}
	return ;
}

void wai_close_log()
{
	int ret = -1;
	
	if(g_wai_log_fd >0) {
		/* close file */
		ret = close(g_wai_log_fd);
		if(!ret) {
			wai_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"close wai log %s  fd %d ok\n", \
						WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME, g_wai_log_fd);			
			g_wai_log_fd = -1;
		}
		else {
			wai_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"close wai log %s fd %d error(%d) %s\n", \
						WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME,g_wai_log_fd, errno, strerror(errno));
		}
	}
	else {
		wai_syslog_warning(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"close wai log %s fd invalid\n", \
					WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME);
	}
	return ;
}

void wai_flush_log()
{
	FILE *wai_fp = NULL;

	wai_fp = fopen(WAI_LOG_FILE_PATH"/"WAI_LOG_FILE_NAME, "w+");
	if(NULL == wai_fp) {
		wai_syslog_err(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"open wai log %s error(%d) %s when flush\n", \
						WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME, errno, strerror(errno));
		return;
	}
	else {
		fclose(wai_fp);
		#if WAI_LOG
		wai_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"flush wai log %s\n", \
					WAI_FUNC_LINE_LITERAL, 0, WAI_LOG_FILE_NAME);
		#endif
	}
	return ;
}

void YLog(char *format,...)
{
	char buf[512] = {0};
	int len = 0, size = 0;
	static int linecnt = 0;
	va_list ptr;

	if( g_wai_log_fd < 0) {
		wai_open_log();
	}
	else if(access(WAI_LOG_FILE_PATH"/"WAI_LOG_FILE_NAME,F_OK)) {
		/* file maybe removed by someone else */
		wai_close_log();
		wai_open_log();
		wai_syslog_warning(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"log file %s may be removed, we try to reopen it\n", \
						WAI_FUNC_LINE_LITERAL,0,WAI_LOG_FILE_NAME);
	}

	if(linecnt > WAI_LOG_LINE_PERFLUSH) {
		wai_flush_log();
		linecnt = 0;
	}
	
	va_start(ptr,format);
	len = vsprintf(buf ,format,ptr);
	va_end(ptr);

	if(len > 0)  {
		size = write(g_wai_log_fd, buf, len);
		linecnt++;
	}
	if(size != len) {
		wai_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"%u write %d bytes with format %d bytes\n", \
						WAI_FUNC_LINE_LITERAL,0, linecnt, size, len);
	}
	else  {
		#if WAI_LOG
		wai_syslog_debug(WAI_FUNC_LINE_FMT""WAI_AFI0_FMT"%u write %d bytes\n", \
						WAI_FUNC_LINE_LITERAL,0, linecnt, size);
		#endif
	}

	return;
}
