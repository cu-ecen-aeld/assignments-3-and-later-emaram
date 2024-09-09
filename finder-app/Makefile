CC=gcc

CFLAGS=-Wall -Werror

all: writer

writer: writer.o
	$(CROSS_COMPILE)$(CC) writer.c $(CFLAGS) -o writer

clean:
	-rm -rf *.o writer