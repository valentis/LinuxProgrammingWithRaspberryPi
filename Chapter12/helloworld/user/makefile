# makefile
.SUFFIXES : .c .o

OBJECTS = helloworld

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = 
TARGET = $(OBJECTS:.o=)

all : helloworld

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c

clean :
	$(RM) $(OBJECTS) $(TARGET) core
