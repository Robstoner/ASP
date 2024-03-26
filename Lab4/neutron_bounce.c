#include <stdio.h>
#include <stdlib.h>
// #include <mpi.h>
#include <math.h>
#include <stdbool.h>

const double H = 0.5;
const double Cr = 0.3, Cc = 0.3;

int main(int argc, char **argv)
{
    int n = 1000000;

    double r = 0, b = 0, t = 0;

    if (argc > 1)
    {
        n = atoi(argv[1]);
    }

    for (int i = 0; i < n; ++i)
    {
        int d = 0, x = 0;
        bool a = true;
        while (a)
        {
            double u = ((double)rand() / RAND_MAX); 
            double L = (-1.0 / (Cr+ Cc)) * log(u);
            d = ((double)rand() / RAND_MAX) * M_PI;  
            x += L * cos(d); 

            if (x < 0)
            {
                r++;
                a = false;
            }
            else if (x >= H)
            {
                t++;
                a = false;
            }
            else if (u < Cc / (Cr + Cc))
            {
                b++;
                a = false;
            }
            else {
                d = u * M_PI;
            }
        }
    }

    printf("r: %f, b: %f, t: %f\n", r / n, b / n, t / n);

}