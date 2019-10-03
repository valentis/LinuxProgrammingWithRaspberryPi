# makefile
.SUFFIXES : .c .o

OBJECTS = main.o \
	  print.o \
	  input.o
SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
TARGET = test

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJECTS) $(TARGET) core

main.o : main.c common.h
print.o : print.c common.h
input.o : input.c common.h
