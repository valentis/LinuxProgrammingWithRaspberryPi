# makefile
.SUFFIXES : .c .o

OBJECTS = 

SUBDIRS = user

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CXX = g++
CFLAGS = 
LDFLAGS = 
TARGET = $(OBJECTS:.o=)

all : $(SUBDIRS)

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c 

$(SUBDIRS) :
	$(MAKE) -C $@

clean: 
	$(RM) $(OBJECTS) $(TARGET) core 
	$(MAKE) -C $(SUBDIRS) clean

.PHONY: $(TOPTARGETS) $(SUBDIRS) $(CLEANDIRS)
