#include "pgm_IO.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <image.pgm> <nr_iter>\n", argv[0]);
        return 0;
    }

    int XSIZE, YSIZE;
    pgm_size(argv[1], &XSIZE, &YSIZE);

    float *data, *pold, *pnew, *plim;

    data = (float *)malloc(XSIZE * YSIZE * sizeof(float *));

    pgm_read(argv[1], data, XSIZE, YSIZE);

    plim = (float *)malloc((XSIZE + 2) * (YSIZE + 2) * sizeof(float *));
    pold = (float *)malloc((XSIZE + 2) * (YSIZE + 2) * sizeof(float *));
    pnew = (float *)malloc((XSIZE + 2) * (YSIZE + 2) * sizeof(float *));

    for (int i = 1; i <= XSIZE; ++i)
    {
        for (int j = 1; j <= YSIZE; ++j)
        {
            plim[i * (YSIZE + 2) + j] = data[(i - 1) * YSIZE + (j - 1)];
        }
    }

    for (int i = 0; i < XSIZE + 2; ++i)
    {
        for (int j = 0; j < YSIZE + 2; ++j)
        {
            pold[i * (YSIZE + 2) + j] = 255.0;
            pnew[i * (YSIZE + 2) + j] = 255.0;
        }
    }

    int iter = atoi(argv[2]);

    for (int k = 0; k < iter; ++k)
    {
        for (int i = 1; i <= XSIZE; ++i)
        {
            for (int j = 1; j <= YSIZE; ++j)
            {
                pnew[i * (YSIZE + 2) + j] = 0.25 * (plim[i * (YSIZE + 2) + j] + pold[(i - 1) * (YSIZE + 2) + j] + pold[(i + 1) * (YSIZE + 2) + j] + pold[i * (YSIZE + 2) + j - 1] + pold[i * (YSIZE + 2) + j + 1]);
            }
        }

        for (int i = 1; i <= XSIZE; ++i)
        {
            for (int j = 1; j <= YSIZE; ++j)
            {
                pold[i * (YSIZE + 2) + j] = pnew[i * (YSIZE + 2) + j];
            }
        }
    }

    for (int i = 1; i <= XSIZE; ++i)
    {
        for (int j = 1; j <= YSIZE; ++j)
        {
            data[(i - 1) * YSIZE + (j - 1)] = pnew[i * (YSIZE + 2) + j];
        }
    }

    pgm_write("output.pgm", data, XSIZE, YSIZE);

    return 0;
}