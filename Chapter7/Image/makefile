# makefile
.SUFFIXES : .c .o

OBJECTS = fbinfo.o \
          fbdraw_point.o \
          fbdraw_line.o \
          fbdraw_circle.o \
          fbdraw_rect.o \
          fbdraw_mmap.o \
          ledmatrix.o \
          lednumber.o \
	  bmpHeader.o \
          bmpViewer.o

SUBDIRS = QtImageViewer

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lwiringPi -pthread
TARGET = $(OBJECTS:.o=)

all : fbinfo fbdraw_point fbdraw_line fbdraw_circle fbdraw_rect fbdraw_mmap \
      ledmatrix lednumber bmpViewer \
      $(SUBDIRS)

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c 

bmpViewer : bmpViewer.c bmpHeader.c
	$(CC) -o $@ bmpViewer.c bmpHeader.c

$(SUBDIRS):
	$(MAKE) -C $@

clean: 
	$(RM) $(OBJECTS) $(TARGET) core QtImageViewer/QtImageViewer
	$(MAKE) -C $(SUBDIRS) clean

.PHONY: $(TOPTARGETS) $(SUBDIRS) $(CLEANDIRS)
