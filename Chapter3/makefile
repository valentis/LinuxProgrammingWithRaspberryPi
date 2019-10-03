# makefile
.SUFFIXES : .c .o

OBJECTS = gpioled.o \
	  wiringled.o \
	  pwmled.o \
	  wiringswitch.o \
	  wiringmusic.o \
	  wiringlight.o \
	  wiringmotor.o \
	  wiringfnd.o \
	  pressure.o \
	  temperature.o \
	  accelerometer.o

SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g
LDFLAGS = -lwiringPi
TARGET = $(OBJECTS:.o=)

all : gpioled wiringled pwmled wiringswitch wiringmusic wiringlight wiringmotor wiringfnd pressure temperature accelerometer

gpioled : gpioled.c
	$(CC) -o $@ $<

wiringled : wiringled.c
	$(CC) -o $@ $< $(LDFLAGS)

pwmled : pwmled.c
	$(CC) -o $@ $< $(LDFLAGS)

wiringswitch : wiringswitch.c
	$(CC) -o $@ $< $(LDFLAGS)

wiringmusic : wiringmusic.c
	$(CC) -o $@ $< $(LDFLAGS)

wiringlight : wiringlight.c
	$(CC) -o $@ $< $(LDFLAGS)

wiringmotor : wiringmotor.o
	$(CC) -o $@ $< $(LDFLAGS)

wiringfnd : wiringfnd.o
	$(CC) -o $@ $< $(LDFLAGS)

pressure : pressure.o
	$(CC) -o $@ $< $(LDFLAGS)

temperature : temperature.o
	$(CC) -o $@ $< $(LDFLAGS)

accelerometer : accelerometer.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	$(RM) $(OBJECTS) $(TARGET) core
