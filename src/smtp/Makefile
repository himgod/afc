all:
	gcc -std=c99 -g -Wall `libesmtp-config --cflags`   -o mail_send mail_send.c -lesmtp
clean:
	rm mail_send
