all: hw2_mpi

hw2_mpi: hw2_mpi.o pgm_io.o
	mpicc hw2_mpi.o pgm_io.o -o hw2_mpi

hw2_mpi.o: hw2_mpi.c
	mpicc -c hw2_mpi.c -o hw2_mpi.o

hw2: hw2.o pgm_io.o
	gcc hw2.o pgm_io.o -o hw2

hw2.o: hw2.c
	gcc -c hw2.c -o hw2.o

pgm_io.o: pgm_io.c
	gcc -c pgm_io.c -o pgm_io.o

clean: 
	rm -f hw2 hw2.o pgm_io.o hw2_mpi hw2_mpi.o
