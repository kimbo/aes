CC = gcc
CFLAGS = -g -Wall -Werror -Wno-unused-variable

.PHONY: all clean aes_test

all: aes

aes: main.o aes.o util.o

test: aes_test

aes_test: unit-test/aes_test.o aes.o util.o
	gcc unit-test/aes_test.o aes.o util.o -o test

clean:
	rm -f *.o aes test unit-test/*.o