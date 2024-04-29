all: hw1_mpi

hw1_mpi: hw1_mpi.o
	mpicc -o hw1_mpi hw1_mpi.o

hw1_mpi.o: hw1_mpi.c
	mpicc -c hw1_mpi.c

clean:
	rm -f hw1_mpi hw1_mpi.o