# makefile
.SUFFIXES : .c .o

OBJECTS = rpi4.o \
	  rpi5.o \
	  rpi6.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lwiringPi -pthread
TARGET = $(OBJECTS:.o=)

all : rpi4 rpi5 rpi6

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core
