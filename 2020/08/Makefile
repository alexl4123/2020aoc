
CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)

.PHONY: all clean

all: 08

08: 08.o
	$(CC) -o $@ $^

08.o: 08.c
	$(CC) $(CFLAGS) $^



clean: 
	rm -f a.out 08 08.o
