#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// integral
int a = 0, b = 10;

double f(double x)
{
  return x * x;
}

double g(double x)
{
  return x * x * x;
}

// calculate the surface integral of f and g from a to b using the monte carlo method
double integral(double (*f)(double), double (*g)(double), int a, int b, int m, int p)
{
  double sum = 0;
  for (int i = m; i < p; i++)
  {
    double x = (double)rand() / RAND_MAX * (b - a) + a;
    sum += f(x) * g(x);
  }

  return sum;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  int rank;
  int numprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = 1000000;
  double totalSum = 0;

  if (rank != 0)
  {
    int m = (n / (numprocs - 1)) * rank;
    int p = (n / (numprocs - 1)) * (rank + 1);

    double sum = integral(f, g, a, b, m, p);

    MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  else
  {
    for (int i = 1; i < numprocs; i++)
    {
      double sum;
      MPI_Recv(&sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      totalSum += sum;
    }

    double finalSum = (b - a) * totalSum / n;

    printf("integral: %f\n", finalSum);
  }

  MPI_Finalize();

  return 0;
}
