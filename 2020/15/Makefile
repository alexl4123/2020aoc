CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)
LFLAGS = -lm

.PHONY: all clean

all: 15 15_a

15: 15.o
	$(CC) -o $@ $^ $(LFLAGS)

15.o: 15.c
	$(CC) $(CFLAGS) $^

15_a: 15_a.o
	$(CC) -o $@ $^ $(LFLAGS)

15_a.o: 15_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 15.o 15 15_a.o 15_a
