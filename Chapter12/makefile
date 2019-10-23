# makefile
.SUFFIXES : .c .o

OBJECTS = turnled.o gpio.o catch_signal.o

SUBDIRS = helloworld add module

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CXX = g++
CFLAGS = -O2
LDFLAGS = 
TARGET = $(OBJECTS:.o=)

all : turnled gpio catch_signal \
      $(SUBDIRS)

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c 

$(SUBDIRS) :
	$(MAKE) -C $@

clean: 
	$(RM) $(OBJECTS) $(TARGET) core 
	$(MAKE) -C helloworld clean
	$(MAKE) -C add clean
	$(MAKE) -C module clean

.PHONY: $(TOPTARGETS) $(SUBDIRS) $(CLEANDIRS)
