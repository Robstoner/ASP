#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int count_lines(char *filename)
{
    FILE *f = fopen(filename, "r");

    int count = 0;
    double num;
    while (fscanf(f, "%lf", &num) == 1)
    {
        count++;
    }

    fclose(f);
    return count;
}

int is_root_of_group(int my_rank, MPI_Comm comm)
{
    int group_rank;
    MPI_Comm_rank(comm, &group_rank);
    return group_rank == 0;
}

int main(int argc, char **argv)
{
    int my_rank, world_size;
    MPI_Comm comm_num, comm_den; // Communicators for numerator and denominator groups
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int N = count_lines("x.dat");
    if (world_size % 2 != 0)
    {
        printf("Number of processes is not even\n");
        return 1;
    }
    if ((N % (world_size / 2)) != 0)
    {
        printf("N is not divisible by half of world_size\n");
        return 1;
    }

    // Split the MPI_COMM_WORLD into two groups
    int color = my_rank % 2;
    MPI_Comm_split(MPI_COMM_WORLD, color, my_rank, &comm_num);
    MPI_Comm_split(MPI_COMM_WORLD, color, my_rank, &comm_den);


    double *x, *y, *A;

    x = (double *)malloc(N * sizeof(double));
    y = (double *)malloc(N * sizeof(double));
    A = (double *)malloc(N * N * sizeof(double));

    if (my_rank == 0)
    {
        FILE *fx, *fy, *fA;
        fx = fopen("x.dat", "r");
        fy = fopen("y.dat", "r");
        fA = fopen("mat.dat", "r");

        if (fx == NULL || fy == NULL || fA == NULL)
        {
            printf("Error opening files!\n");
            return 1;
        }

        for (int i = 0; i < N; i++)
        {
            fscanf(fx, "%lf", &x[i]);
            fscanf(fy, "%lf", &y[i]);
        }

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fscanf(fA, "%lf", &A[i * N + j]);
            }
        }
    }

    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(A, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double local_numerator = 0.0, local_denominator = 0.0;
    double total_numerator = 0.0, total_denominator = 0.0;

    int local_n = N / (world_size / 2);
    int offset = local_n * (my_rank / 2);

    if (color == 0)
    {
        // compute local_numerator
        for (int i = offset; i < offset + local_n; i++)
        {
            for (int j = 0; j < N; j++)
            {
                local_numerator += x[i] * A[i * N + j] * y[j];
            }
        }
    }

    if (color == 1)
    {
        // compute local_denominator
        for (int i = offset; i < offset + local_n; i++)
        {
            local_denominator += x[i] * y[i];
        }
    }

    MPI_Reduce(&local_numerator, &total_numerator, 1, MPI_DOUBLE, MPI_SUM, 0, comm_num);
    MPI_Reduce(&local_denominator, &total_denominator, 1, MPI_DOUBLE, MPI_SUM, 0, comm_den);

    if (is_root_of_group(my_rank, comm_den) && total_denominator != 0)
    {
        MPI_Send(&total_denominator, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0)
    {
        MPI_Status status;
        int source_rank;
        // Receive from any source
        MPI_Recv(&total_denominator, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        // Get the actual source rank
        MPI_Get_count(&status, MPI_DOUBLE, &source_rank);
        
        printf("Rank 0: total_numerator = %lf\n", total_numerator);
        printf("Rank 0: total_denominator = %lf received from rank: %d\n", total_denominator, source_rank);

        if (total_denominator == 0)
        {
            printf("Denominator is zero, cannot compute AVG.\n");
            MPI_Finalize();
            return 1;
        }

        double AVG = total_numerator / total_denominator;
        printf("AVG = %lf\n", AVG);
    }

    free(x);
    free(y);
    free(A);

    MPI_Finalize();

    return 0;
}