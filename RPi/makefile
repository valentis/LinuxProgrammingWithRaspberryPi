# makefile
.SUFFIXES : .c .o

OBJECTS = rpi4.o \
	  rpi5.o \
	  rpi6.o \
          rpi7.o 

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CXX = g++
CFLAGS = -g
LDFLAGS = -lwiringPi -pthread
TARGET = $(OBJECTS:.o=)

all : rpi4 rpi5 rpi6 rpi7 rpi8

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

rpi7 : $(TARGET:=.c)
	$(CC) -o $@ $@.c bmpCapture.c $(LDFLAGS)

rpi8 : $(TARGET:=.c)
	$(CXX) -o $@ rpi7.c opencv_facedetect.cpp `pkg-config --cflags --libs opencv` $(LDFLAGS) 

#bmpCapture.c 

clean :
	$(RM) $(OBJECTS) $(TARGET) rpi8 core capture.bmp
