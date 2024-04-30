#include "pgm_IO.h"
#include <mpi.h>

int main(int argc, char **argv)
{
    int my_rank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int tag = 333;

    if (argc != 3)
    {
        printf("Usage: %s <image.pgm> <nr_iter>\n", argv[0]);
        return 0;
    }

    int iter = atoi(argv[2]);
    if (iter <= 0)
    {
        printf("nr_iter must be greater than 0\n");
        return 0;
    }

    int XSIZE, YSIZE;
    pgm_size(argv[1], &XSIZE, &YSIZE);

    if (XSIZE % nprocs != 0)
    {
        printf("XSIZE must be divisible by nprocs\n");
        return 0;
    }

    int MP = XSIZE / nprocs, NP = YSIZE;

    float *data, *pold, *pnew, *plim, *masterdata;

    masterdata = (float *)malloc(XSIZE * YSIZE * sizeof(float *));

    data = (float *)malloc(MP * NP * sizeof(float *));

    plim = (float *)malloc((MP + 2) * (NP + 2) * sizeof(float *));
    pold = (float *)malloc((MP + 2) * (NP + 2) * sizeof(float *));
    pnew = (float *)malloc((MP + 2) * (NP + 2) * sizeof(float *));

    if (my_rank == 0)
    {
        pgm_read(argv[1], masterdata, XSIZE, YSIZE);

        MPI_Scatter(masterdata, MP * NP, MPI_FLOAT, data, MP * NP, MPI_FLOAT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < MP + 2; i++)
        {
            for (int j = 0; j < NP + 2; j++)
            {
                pold[i * (NP + 2) + j] = 255.0;
                pnew[i * (NP + 2) + j] = 255.0;
                plim[i * (NP + 2) + j] = 255.0;
            }
        }

        for (int i = 1; i <= MP; ++i)
        {
            for (int j = 1; j <= NP; ++j)
            {
                plim[i * (NP + 2) + j] = data[(i - 1) * NP + (j - 1)];
            }
        }

        for (int k = 0; k < iter; ++k)
        {
            MPI_Send(&pold[(MP) * (NP + 2)], NP, MPI_FLOAT, my_rank + 1, tag, MPI_COMM_WORLD);
            MPI_Recv(&pold[0], NP, MPI_FLOAT, MPI_PROC_NULL, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&pold[NP + 2], NP, MPI_FLOAT, MPI_PROC_NULL, tag, MPI_COMM_WORLD);
            MPI_Recv(&pold[(MP + 1) * (NP + 2)], NP, MPI_FLOAT, my_rank + 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 1; i < MP + 1; ++i)
            {
                for (int j = 1; j < NP + 1; ++j)
                {
                    pnew[i * (NP + 2) + j] = 0.25 * (plim[i * (NP + 2) + j] + pold[(i - 1) * (NP + 2) + j] + pold[(i + 1) * (NP + 2) + j] + pold[i * (NP + 2) + j - 1] + pold[i * (NP + 2) + j + 1]);
                }
            }

            for (int i = 1; i < MP + 1; ++i)
            {
                for (int j = 1; j < NP + 1; ++j)
                {
                    pold[i * (NP + 2) + j] = pnew[i * (NP + 2) + j];
                }
            }
        }

        for (int i = 1; i <= MP; ++i)
        {
            for (int j = 1; j <= NP; ++j)
            {
                data[(i - 1) * NP + (j - 1)] = pnew[i * (NP + 2) + j];
            }
        }

        MPI_Gather(data, MP * NP, MPI_FLOAT, masterdata, MP * NP, MPI_FLOAT, 0, MPI_COMM_WORLD);

        char output[20];
        sprintf(output, "output_%d.pgm", iter);
        pgm_write(output, masterdata, XSIZE, YSIZE);
    }
    else
    {
        MPI_Scatter(NULL, 0, MPI_FLOAT, data, MP * NP, MPI_FLOAT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < MP + 2; ++i)
        {
            for (int j = 0; j < NP + 2; ++j)
            {
                pold[i * (NP + 2) + j] = 255.0;
                pnew[i * (NP + 2) + j] = 255.0;
                plim[i * (NP + 2) + j] = 255.0;
            }
        }

        for (int i = 1; i <= MP; ++i)
        {
            for (int j = 1; j <= NP; ++j)
            {
                plim[i * (NP + 2) + j] = data[(i - 1) * NP + (j - 1)];
            }
        }

        for (int k = 0; k < iter; ++k)
        {
            if (my_rank == nprocs - 1)
            {
                MPI_Send(&pold[(MP) * (NP + 2)], NP, MPI_FLOAT, MPI_PROC_NULL, tag, MPI_COMM_WORLD);
                MPI_Recv(&pold[0], NP, MPI_FLOAT, my_rank - 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&pold[NP + 2], NP, MPI_FLOAT, my_rank - 1, tag, MPI_COMM_WORLD);
                MPI_Recv(&pold[(MP + 1) * (NP + 2)], NP, MPI_FLOAT, MPI_PROC_NULL, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                MPI_Send(&pold[(MP) * (NP + 2)], NP, MPI_FLOAT, my_rank + 1, tag, MPI_COMM_WORLD);
                MPI_Recv(&pold[0], NP, MPI_FLOAT, my_rank - 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&pold[NP + 2], NP, MPI_FLOAT, my_rank - 1, tag, MPI_COMM_WORLD);
                MPI_Recv(&pold[(MP + 1) * (NP + 2)], NP, MPI_FLOAT, my_rank + 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            for (int i = 1; i < MP + 1; ++i)
            {
                for (int j = 1; j < NP + 1; ++j)
                {
                    pnew[i * (NP + 2) + j] = 0.25 * (plim[i * (NP + 2) + j] + pold[(i - 1) * (NP + 2) + j] + pold[(i + 1) * (NP + 2) + j] + pold[i * (NP + 2) + j - 1] + pold[i * (NP + 2) + j + 1]);
                }
            }

            for (int i = 1; i < MP + 1; ++i)
            {
                for (int j = 1; j < NP + 1; ++j)
                {
                    pold[i * (NP + 2) + j] = pnew[i * (NP + 2) + j];
                }
            }
        }

        for (int i = 1; i <= MP; ++i)
        {
            for (int j = 1; j <= NP; ++j)
            {
                data[(i - 1) * NP + (j - 1)] = pnew[i * (NP + 2) + j];
            }
        }

        MPI_Gather(data, MP * NP, MPI_FLOAT, NULL, 0, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}