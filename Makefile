CC = gcc
CFLAGS = -g -Wall -Werror -Wno-unused-variable

.PHONY: all clean

all: aes

test: aes_test

aes_test: aes_test.o aes.o

clean:
	rm -f *.o aes aes_test
