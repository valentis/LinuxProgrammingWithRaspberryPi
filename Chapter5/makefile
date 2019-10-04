# makefile
.SUFFIXES : .c .o

OBJECTS = rpi4.o 

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lwiringPi
TARGET = $(OBJECTS:.o=)

all : rpi4

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core
