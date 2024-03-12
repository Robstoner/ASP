#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

int main ( int argc, char **argv){
    	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproces);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(rank==0)
	{
		printf("mpi node %i\n", rank);
		int message=2;
		MPI_Send(&message,1, MPI_INT, 1,0, MPI_COMM_WORLD);
	}
	
	for(int i=1; i< numeproces; i++)
	{
		if(rank==1)
			{ int mesage;
			  MPI_Recv(&message, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			  printf("mpi node %i\n", rank);
			  
			 if(rank !=numproces-1)
			 {
			 	MPI_Send(&message, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
			}
		}
	}
			 	
	MPI_Finalize();
	return 0;
}