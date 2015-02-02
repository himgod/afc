CFLAGS = -Wall -g -O0 -D_REENTRANT 
CFLAGS += -I$(ACCAPI_DIR)
CFLAGS += -Werror -fPIC
CFLAGS += -Wno-char-subscripts # -Wno-unused-but-set-variable
EXTFLAGS =  -shared

RM = /bin/rm -f 

PUB_LIB = libwai.so

PUB_OBJS = wai_log.o 

PUB_SRCS = $(PUB_OBJS:.o=.c) 



.PHONY: all 

all: $(PUB_OBJS) 
	$(CC)  $(EXTFLAGS) $(CFLAGS) $(PUB_OBJS) -o $(PUB_LIB)


.PHONY: clean

clean: 
	$(RM) $(PUB_LIB) $(PUB_OBJS) 


