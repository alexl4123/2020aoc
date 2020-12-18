CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)
LFLAGS = -lm

.PHONY: all clean

all: 18 18_a

18: 18.o
	$(CC) -o $@ $^ $(LFLAGS)

18.o: 18.c
	$(CC) $(CFLAGS) $^

18_a: 18_a.o
	$(CC) -o $@ $^ $(LFLAGS)

18_a.o: 18_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 18.o 18 18_a.o 18_a
