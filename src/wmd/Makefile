SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CC = gcc

CFLAGS += -Wall -g ${DBUS_INCLUDE}
LDFLAGS += -lpthread ${DBUS_LIBS}

TARGET = app

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	$(RM) $(OBJ)
	$(RM) $(TARGET)
	$(RM) *~


