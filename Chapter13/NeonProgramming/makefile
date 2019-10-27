# makefile
.SUFFIXES : .c .o

OBJECTS = helloneon.o \
          helloneon_for.o \
          calculation.o \
	  colorconvert.o bmpHeader.o \
          neonconvert.o \
          colorsepia.o neonsepia.o \
          v4l2_framebuffer_neon.o \
          v4l2_framebuffer_filter.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -O2 -march=armv8-a -mtune=cortex-a72 -mfpu=neon
LDFLAGS = 
TARGET = $(OBJECTS:.o=)

all : helloneon helloneon_for calculation \
      colorconvert neonconvert \
      colorsepia neonsepia \
      v4l2_framebuffer_neon v4l2_framebuffer_filter

colorconvert : $(TARGET:=.c) bmpHeader.c
	$(CC) -o $@ $@.c bmpHeader.c

neonconvert : $(TARGET:=.c) bmpHeader.c
	$(CC) -o $@ $@.c bmpHeader.c $(CFLAGS)

colorsepia : $(TARGET:=.c) bmpHeader.c
	$(CC) -o $@ $@.c bmpHeader.c

neonsepia : $(TARGET:=.c) bmpHeader.c
	$(CC) -o $@ $@.c bmpHeader.c $(CFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core

.PHONY: $(TOPTARGETS) $(SUBDIRS) $(CLEANDIRS)
