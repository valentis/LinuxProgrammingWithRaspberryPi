OBJECTS = main.o print.o input.o
SRCS = main.c print.c input.c
CFLAGS = -g
TARGET = test

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	rm -f $(OBJECTS) $(TARGET) core

main.o : main.c common.h
print.o : print.c common.h
input.o : input.c common.h
