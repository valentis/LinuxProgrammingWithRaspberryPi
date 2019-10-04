# makefile
.SUFFIXES : .c .o

OBJECTS = loop.o \
	  handleSignal.o \
	  limit.o \
	  fork.o \
	  vfork.o \
	  system.o \
	  spawn.o \
	  spawn2.o \
	  pipe.o \
	  fifo_server.o \
	  fifo_client.o \
	  msg_server.o \
	  msg_client.o \
	  sem.o \
	  shm.o \
	  posix_msg_server.o \
	  posix_msg_client.o \
	  posix_sem.o \
	  thread.o \
	  thread_mutex.o \
	  joystick.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -pthread -lrt
TARGET = $(OBJECTS:.o=)

all : loop handleSignal limit fork vfork system spawn spawn2 \
      pipe fifo_server fifo_client msg_server msg_client sem shm \
      posix_msg_server posix_msg_client posix_sem \
      thread thread_mutex joystick

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core
