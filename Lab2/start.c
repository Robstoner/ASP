#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

int main (int argc, char **argv){
  int myrank;
  int numprocs;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  printf("mpi node %i\n", myrank);

  MPI_Finalize();

  return 0;
}