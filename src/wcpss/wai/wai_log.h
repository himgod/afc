#ifndef _WAI_LOG_H_
#define _WAI_LOG_H_
#define WAI_LOG_FILE_PATH	"/opt/run/wcpss"
#define WAI_LOG_FILE_NAME	"y-afi.log"
#define WAI_LOG_LINE_PERFLUSH	10

void YLog(char *format,...);
#endif
