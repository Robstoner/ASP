all: hw2

hw2: hw2.o pgm_io.o
	gcc hw2.o pgm_io.o -o hw2

hw2.o: hw2.c
	gcc -c hw2.c -o hw2.o

pgm_io.o: pgm_io.c
	gcc -c pgm_io.c -o pgm_io.o

clean: 
	rm -f hw2 hw2.o pgm_io.o
