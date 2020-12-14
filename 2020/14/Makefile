CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)
LFLAGS = -lm

.PHONY: all clean

all: 14 14_a

14: 14.o
	$(CC) -o $@ $^ $(LFLAGS)

14.o: 14.c
	$(CC) $(CFLAGS) $^

14_a: 14_a.o
	$(CC) -o $@ $^ $(LFLAGS)

14_a.o: 14_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 14.o 14 14_a.o 14_a
