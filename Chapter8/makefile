# makefile
.SUFFIXES : .c .cpp .o

OBJECTS = helloopencv.o \
          drawline.o \
	  drawrect.o \
	  drawcircle.o \
	  drawpolygon.o \
	  drawtext.o \
	  draw.o \
	  saveimage.o \
	  loadimage.o \
	  matplus.o \
	  matmultiply.o \
	  matminus.o  \
	  matdivide.o \
	  opencv1.o \
	  opencv2.o \
	  histogram.o \
	  equalize.o \
	  rotate.o \
	  blur.o \
	  edge.o \
	  opencv_fb.o \
	  opencv_facedetect.o

SRCS = $(OBJECTS:.o=.cpp)

CC = gcc
CXX = g++
CFLAGS = -O4 -g
CXXFLAGS = -g
LDFLAGS = `pkg-config --cflags --libs opencv`

all : helloopencv drawline drawrect drawcircle drawpolygon drawtext \
      draw saveimage loadimage \
      matplus matmultiply matminus matdivide \
      opencv1 opencv2 \
      histogram equalize \
      rotate blur \
      edge opencv_fb opencv_facedetect

$(TARGET) : $(TARGET:=.cpp)
	$(CXX) -o $@ $@.cpp $(LDFLAGS)

opencv1 : opencv1.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(OBJECTS:.o=) core sample.jpg sample.png

.PHONY: clean
