#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// integral
int a = 0, b = 10;

double f(double x)
{
  return x * x;
}

// calculate the integral of f from a to b using the monte carlo method
double integral(double (*f)(double), int a, int b, int n)
{
  double sum = 0;
  for (int i = 0; i < n; i++)
  {
    double x = (double)rand() / RAND_MAX * (b - a) + a;
    sum += f(x);
  }

  return (b - a) * sum / n;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  int n = 1000000;

  printf("integral: %f\n", integral(f, a, b, n));

  return 0;
}
