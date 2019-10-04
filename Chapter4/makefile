# makefile
.SUFFIXES : .c .o

OBJECTS = copy.o \
	  fcopy.o \
	  hitkey.o \
	  chmod.o \
	  list.o \
	  time.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lwiringPi
TARGET = $(OBJECTS:.o=)

all : copy fcopy hitkey chmod list time

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c

clean :
	$(RM) $(OBJECTS) $(TARGET) core
