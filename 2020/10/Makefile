CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)

.PHONY: all clean

all: 10

10: 10.o
	$(CC) -o $@ $^

10.o: 10.c
	$(CC) $(CFLAGS) $^


clean:
	rm -rf 10.o 10
