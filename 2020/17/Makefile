CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)
LFLAGS = -lm

.PHONY: all clean

all: 17 17_a

17: 17.o
	$(CC) -o $@ $^ $(LFLAGS)

17.o: 17.c
	$(CC) $(CFLAGS) $^

17_a: 17_a.o
	$(CC) -o $@ $^ $(LFLAGS)

17_a.o: 17_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 17.o 17 17_a.o 17_a
