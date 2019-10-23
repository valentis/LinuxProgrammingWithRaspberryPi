# makefile
.SUFFIXES : .c .o

OBJECTS = gstreamer.o \
          element.o \
	  rtsp.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -O2
LDFLAGS = `pkg-config --cflags --libs gstreamer-1.0`
TARGET = $(OBJECTS:.o=)

all : gstreamer element rtsp


$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

rtsp : rtsp.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LDFLAGS) -L/usr/local/lib -lgstrtspserver-1.0

clean :
	$(RM) $(OBJECTS) $(TARGET) core
