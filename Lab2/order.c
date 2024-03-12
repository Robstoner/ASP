#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int rank;
  int numprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    printf("mpi node %i\n", rank);
    int message = rank;
    MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  else
  {
    if (rank < 20)
    {
      int message = rank;
      MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("mpi node %i\n", rank);

      if (rank != numprocs - 1)
      {
        MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
      }
    }
    else if (rank == numprocs - 1)
    {
      int message = rank;
      MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("mpi node %i\n", rank);

      MPI_Send(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    }
    else {
      int message;
      MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

      MPI_Recv(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      printf("mpi node %i\n", rank);

      MPI_Send(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

    }
  }

  MPI_Finalize();

  return 0;
}