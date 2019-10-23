# makefile
.SUFFIXES : .c .o

OBJECTS = add.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = 
TARGET = $(OBJECTS:.o=)

all : add

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c

clean :
	$(RM) $(OBJECTS) $(TARGET) core
