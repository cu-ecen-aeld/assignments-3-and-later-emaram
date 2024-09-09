CC=gcc

GFLAGS=-Wall -Werror

ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	CC=aarch64-none-linux-gnu-gcc 
endif

all: writer

writer: writer.o
	$(CC) writer.c $(CFLAGS) -o writer

clean:
	-rm -rf *.o writer