
.PHONY : all clean

all: a b

b 06_b: 06_b.o
	gcc 06_b.o -o 06_b

06_b.o: 06_b.c
	gcc -pedantic -Wall -c 06_b.c


a 06_a: 06_a.o
	gcc 06_a.o -o 06_a

06_a.o: 06_a.c
	gcc -pedantic -Wall -c 06_a.c

clean:
	rm -f 06_a 06_a.o 06_b 06_b.o
