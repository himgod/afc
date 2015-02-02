LDFLAGS = -L../pub -lwcpsspub -lpthread -ldl -D_REENTRANT 
LDFLAGS += -L../wai -lwai

export CFG=/usr/bin/mysql_config
MYSQLFLAGS=`${CFG} --cflags`
MYSQLLIBS=`${CFG} --libs`
CFLAGS = -Wall -g -O0 -D_REENTRANT -DCW_NO_DTLS -D_AC_BAK_UDP_ -D_DISTRIBUTION_  -I. -I../pub -I../wai
CFLAGS += -DOS_NO_C_LIB_DEFINES
CFLAGS += -I$(ACCAPI_DIR)
CFLAGS += -I$(DBLIB_MOD) $(MYSQLFLAGS)

CFLAGS += ${XML2_INCLUDE}
CFLAGS += -I/usr/include/libxml2 -I/usr/local/include
CFLAGS += -Werror
CFLAGS += -Wno-char-subscripts
LIBS = -L./  -lm ${XML2_LIBS} -lz -lrt
LIBS += -L$(DBLIB_MOD) -ldbop $(MYSQLLIBS)

LIBS += -L/lib/x86_64-linux-gnu -lxml2 -lz  -liconv
# Memory leak
#LDFLAGS += ../dmalloc-5.5.0/libdmallocth.a
#CFLAGS += -DDMALLOC

# Capwap Debugging
#CFLAGS += -DCW_DEBUGGING

RM = /bin/rm -f 

# list of generated object files for AC. 
WID_OBJS = AC.o ACConfigFile.o ACMainLoop.o ACDiscoveryState.o ACJoinState.o ACReset.o ACImageData.o ACConfigureState.o ACDataCheckState.o ACRunState.o ACProtocol_User.o ACRetransmission.o ACMultiHomedSocket.o  ACProtocol.o ACBinding.o ACTxpowerControl.o ACChannelSelection.o ACInterface.o ACSTARoaming.o  ACSettingsFile.o ACDbus.o ACDbus_handler.o ACMsgq.o iplist.o ACNetlink.o ACBak.o ACIPv6Addr.o ACAccessCheck.o ACUpdateManage.o ACDbusPathInit.o ACLoadbanlance.o ACChannelSelection2.o os_internal.o CWThread_wid.o
 
WID_SRCS = $(WID_OBJS:.o=.c) 

WID_DEPS := $(WID_OBJS:.o=.d)

# program executables. 
WID_APP = wid 

.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: $(WID_APP) 

$(WID_APP): $(WID_OBJS) 
	$(CC) $(WID_OBJS) $(CC_FLAGS) $(LDFLAGS) $(LDLIBS)  $(DBUS_LIBS) $(LIBS) -o $(WID_APP) 
%.o: %.c
	$(CC) -c $(CFLAGS) $(DBUS_INCLUDE) $(INCLUDES) -o $*.o $<
clean: 
	$(RM) $(WID_APP) $(WID_OBJS) $(WID_DEPS) 

clean_deps:
	$(WID_DEPS) 
	
deps: $(WID_SRC) 
	$(CC) -MD -E $(WID_SRCS) $(CFLAGS) >/dev/null

-include $(WID_DEPS)
