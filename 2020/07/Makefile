

.PHONY : all clean

all: 07

07: 07.o
	gcc 07.o -o 07

07.o: 07.c
	gcc -pedantic -Wall -c 07.c

clean:
	rm -f 07 07.o
