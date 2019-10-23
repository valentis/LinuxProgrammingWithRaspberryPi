# makefile
.SUFFIXES : .cpp .o

OBJECTS = hellocaffe.o \
	  filler.o \
          blob.o

SUBDIRS = CaffeWithCamera \
          DigitRecognizerApp

SRCS = $(OBJECTS:.o=.cpp)

CC = gcc
CXX = g++
CFLAGS = -DCPU_ONLY -std=c++14
LDFLAGS = -I/home/pi/caffe/build/install/include -lcaffe -L/home/pi/caffe/build/install/lib -lboost_system -lglog -L/usr/lib/arm-linux-gnueabihf
TARGET = $(OBJECTS:.o=)

all : hellocaffe filler blob \
      $(SUBDIRS)

$(TARGET) : $(TARGET:=.cpp)
	$(CXX) $(CFLAGS) -o $@ $@.cpp $(LDFLAGS)

$(SUBDIRS) :
	$(MAKE) -C $@

clean: 
	$(RM) $(OBJECTS) $(TARGET) core CaffeWithCamera/CaffeWithCamera DigitRecognizerApp/DigitRecognizerApp
	$(MAKE) -C CaffeWithCamera clean
	$(MAKE) -C DigitRecognizerApp clean

.PHONY: $(TOPTARGETS) $(SUBDIRS) $(CLEANDIRS)
