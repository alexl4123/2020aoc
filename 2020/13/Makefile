CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)

.PHONY: all clean

all: 13 13_a

13: 13.o
	$(CC) -o $@ $^

13.o: 13.c
	$(CC) $(CFLAGS) $^

13_a: 13_a.o
	$(CC) -o $@ $^

13_a.o: 13_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 13.o 13 13_a.o 13_a
