#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <openssl/ssl.h>
#include <libesmtp.h>
#include "mail_send.h"

int mail_send(char * host, unsigned short port, int ssl_enable, char * send_address, char * receive_address, char *content) {
    smtp_session_t session;
    smtp_message_t message;
    struct sigaction sa;
    const smtp_status_t *status;
    char buf[256] = {0};
    char server[256] = {0};
    //FILE *fp;
    /* This program sends only one message at a time. Create an SMTP
       session and add a message to it. */
    if( (session = smtp_create_session ()) == NULL){
        fprintf (stderr, "smtp_create_session problem %s\n",
                smtp_strerror (smtp_errno (), buf, sizeof buf));
        return 1;
    }
    if((message = smtp_add_message (session)) == NULL){
        fprintf (stderr, "smtp_add_message problem %s\n",
                smtp_strerror (smtp_errno (), buf, sizeof buf));
        return 1;
    }
    /* NB. libESMTP sets timeouts as it progresses through the protocol.
       In addition the remote server might close its socket on a timeout.
       Consequently libESMTP may sometimes try to write to a socket with
       no reader. Ignore SIGPIPE, then the program doesn't get killed
       if/when this happens. */
    sa.sa_handler = SIG_IGN;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction (SIGPIPE, &sa, NULL); 
    /* Set the host running the SMTP server. LibESMTP has a default port
       number of 587, however this is not widely deployed so the port
       is specified as 25 along with the default MTA host. */
    sprintf(server, "%s:%d", host, port);
    smtp_set_server (session, server);/**/
    //smtp_set_server (session, "127.0.0.1:25");
    /* Set the reverse path for the mail envelope. (NULL is ok)
     */
	if(ssl_enable)
	{/* I don't know here is right or wrong */
     		smtp_starttls_enable(session, Starttls_ENABLED);
    }
    smtp_set_reverse_path (message, send_address);
    /* RFC 2822 doesn't require recipient headers but a To: header would
     * be nice to have if not present. */
    smtp_set_header (message, "To", NULL, NULL);
    smtp_set_header (message, "Subject", " test mail");
    smtp_set_header_option (message, "Subject", Hdr_OVERRIDE, 1);
    //fprintf(stderr,"%s\n","smtp_set_server.");
    /*
    if ((fp = fopen ("test-mail.eml", "r")) == NULL) {
        fprintf (stderr, "can't open mail file: %s\n", strerror (errno));
        return (1);
    }
    smtp_set_message_fp (message, fp);
    */
    smtp_set_message_str(message, content?content:"");
    
    smtp_add_recipient (message,receive_address);
    /* Initiate a connection to the SMTP server and transfer the
       message. */
    if (!smtp_start_session (session)){
        fprintf (stderr, "SMTP server problem %s\n",
                smtp_strerror (smtp_errno (), buf, sizeof buf));
    }
    else{
        /* Report on the success or otherwise of the mail transfer.
        */
        status = smtp_message_transfer_status (message);
        if(0 != status->code)
		{
			printf ("%d %s", status->code,
                (status->text != NULL) ? status->text : "\n");
    	}
		printf("one test mail send\n");
	}
    /* Free resources consumed by the program.
    */
    smtp_destroy_session (session);
    /*if(fp != NULL){
        fclose(fp);
    }*/

    return 0;
}

int main()
{
	mail_send("mail.autelan.com", 25, 0, "hanhui@autelan.com", "hanhui@autelan.com", "hello test");
	return 0;
}
