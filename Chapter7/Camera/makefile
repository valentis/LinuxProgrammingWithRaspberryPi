# makefile
.SUFFIXES : .c .o

OBJECTS = v4l2_framebuffer.o \
          bmpCapture.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lwiringPi -pthread
TARGET = $(OBJECTS:.o=)

all : v4l2_framebuffer bmpCapture

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c 

clean :
	$(RM) $(OBJECTS) $(TARGET) core capture.bmp
