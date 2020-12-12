CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)

.PHONY: all clean

all: 12

12: 12.o
	$(CC) -o $@ $^

12.o: 12.c
	$(CC) $(CFLAGS) $^


clean:
	rm -rf 12.o 12
