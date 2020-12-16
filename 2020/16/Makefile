CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c
CFLAGS = -std=c99 -pedantic -Wall $(DEFS)
LFLAGS = -lm

.PHONY: all clean

all: 16 16_a

16: 16.o
	$(CC) -o $@ $^ $(LFLAGS)

16.o: 16.c
	$(CC) $(CFLAGS) $^

16_a: 16_a.o
	$(CC) -o $@ $^ $(LFLAGS)

16_a.o: 16_a.c
	$(CC) $(CFLAGS) $^



clean:
	rm -rf 16.o 16 16_a.o 16_a
