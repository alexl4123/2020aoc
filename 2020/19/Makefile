CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)
LFLAGS = -lm

.PHONY: all clean

all: 19 19_a

19: 19.o
	$(CC) -o $@ $^ $(LFLAGS)

19.o: 19.c
	$(CC) $(CFLAGS) $^

19_a: 19_a.o
	$(CC) -o $@ $^ $(LFLAGS)

19_a.o: 19_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 19.o 19 19_a.o 19_a
