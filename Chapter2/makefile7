# makefile
.SUFFIXES : .c .o

OBJECTS = main.o \
	  print.o \
	  input.o
SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
TARGET = test helloworld

all : $(TARGET)
helloworld : helloworld.c
	$(CC) -o $@ $<

test : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

clean :
	$(RM) $(OBJECTS) $(TARGET) helloworld.o core

main.o : main.c common.h
print.o : print.c common.h
input.o : input.c common.h
