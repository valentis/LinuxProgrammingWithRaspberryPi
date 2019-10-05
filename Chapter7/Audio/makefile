# makefile
.SUFFIXES : .c .o

OBJECTS = alsapcm.o \
          alsavolume.o \
	  wavPlayer.o \
          wavRecorder.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lasound -lm
TARGET = $(OBJECTS:.o=)

all : alsapcm alsavolume \
      wavPlayer wavRecorder

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core
