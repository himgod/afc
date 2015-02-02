INCLUDES = -I$(ACCAPI_DIR)   

#ADDITION_LIB_PATH=/opt/x86libs
DBUS_INCLUDE=-I${SYS_INCLUDE_PATH}/dbus-1.0 -I${SYS_LIB_PATH}/dbus-1.0/include
DBUS_LIBS=-L${SYS_LIB_PATH}/usr/lib -ldbus-1
LDFLAGS  =  -fPIC  -shared -lrt -Wl,-soname,$@
PUB_LIB = libhandw.so
PUB_OBJS = wid_wtp.o wid_ac.o  check_argument.o
 
.PHONY: all
all: $(PUB_LIB)

.PHONY: clean
clean:
	$(RM) *.o $(PUB_LIB)

$(PUB_LIB): $(PUB_OBJS) 
	$(CC) $(PUB_OBJS) $(LDFLAGS) -o $(PUB_LIB)

%.o: %.c
	${RM} $@
	$(CC) -c -fPIC $(DEFS) $(INCLUDES) $(CFLAGS) $(DBUS_LIBS) $(DBUS_INCLUDE) -o $*.o $<
