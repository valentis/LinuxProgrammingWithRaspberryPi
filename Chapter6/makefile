# makefile
.SUFFIXES : .c .o

OBJECTS = socketpair.o \
          udp_server.o \
	  udp_client.o \
	  tcp_server.o \
	  tcp_client.o \
	  tcp_shutdown.o \
	  select_server.o \
	  epoll_server.o \
	  webserver.o \
	  daemon.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -pthread
TARGET = $(OBJECTS:.o=)

all : socketpair \
      udp_server udp_client \
      tcp_server tcp_client tcp_shutdown \
      select_server epoll_server webserver \
      daemon

$(TARGET) : $(TARGET:=.c)
	$(CC) -o $@ $@.c $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core
