#include "pgm_IO.h"
#include <mpi.h>

int main(int argc, char **argv)
{
    int my_rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int ndims = 2;
    int dims[2] = {0, 0};
    MPI_Dims_create(num_procs, ndims, dims);

    int nx = dims[0], ny = dims[1];

    if (XSIZE % nx != 0 || YSIZE % ny != 0)
    {
        if (my_rank == 0)
        {
            printf("XSIZE sau YSIZE nu sunt divizibile cu nx sau ny\n");
        }
        MPI_Finalize();
        return 0;
    }

    int periods[2] = {0, 0};
    MPI_Comm comm_cart;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart);

    float *local_image_array, *final_image_array;
    int local_xsize = XSIZE / nx, local_ysize = YSIZE / ny;

    if (my_rank == 0)
    {
        final_image_array = (float *)malloc(XSIZE * YSIZE * sizeof(float *));
        for (int i = 0; i < XSIZE; i++)
        {
            for (int j = 0; j < YSIZE; j++)
            {
                final_image_array[i * local_xsize + j] = 0.75 * CONTRAST;
            }
        }
    }
    else
    {

        local_image_array = (float *)malloc(local_xsize * local_ysize * sizeof(float *));
        for (int i = 0; i < local_xsize; i++)
        {
            for (int j = 0; j < local_ysize; j++)
            {
                local_image_array[i * local_xsize + j] = 0.75 * CONTRAST;
            }
        }

        int coords[ndims];
        MPI_Cart_coords(comm_cart, my_rank, ndims, coords);

        if ((coords[0] + coords[1] + 1) % 2 == 1)
        {
            for (int i = 0; i < local_xsize; ++i)
            {
                for (int j = 0; j < local_ysize; ++j)
                {
                    local_image_array[i * local_xsize + j] = 0;
                }
            }
        }
        else
        {
            for (int i = 0; i < local_xsize; ++i)
            {
                for (int j = 0; j < local_ysize; ++j)
                {
                    local_image_array[i * local_xsize + j] = CONTRAST;
                }
            }
        }
    }

    MPI_Datatype block_type;
    MPI_Type_vector(local_xsize, local_ysize, 1, MPI_FLOAT, &block_type);
    MPI_Type_commit(&block_type);

    if (my_rank != 0)
    {
        // Procesele worker trimit datele
        MPI_Send(&local_image_array, 1, block_type, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        // Procesul master primește datele de la fiecare proces worker
        final_image_array = (float *)malloc(XSIZE * YSIZE * sizeof(float));

        MPI_Status status;
        for (int i = 1; i < num_procs; i++)
        {
            int source = i;
            MPI_Recv(final_image_array + i * local_xsize * local_ysize, 1, block_type, source, 0, MPI_COMM_WORLD, &status);
        }
    }

    if (my_rank == 0)
    {
        // Scrierea fișierului PGM folosind funcția pgm_write
        pgm_write("final_image.pgm", final_image_array, XSIZE, YSIZE);
    }

    MPI_Finalize();

    return 0;
}