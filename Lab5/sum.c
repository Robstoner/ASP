#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define MATRIX_SIZE 2

void print_matrix(double a[MATRIX_SIZE][MATRIX_SIZE])
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            printf("%lf ", a[i][j]);
        }
        printf("\n");
    }
}

void copy_matrix(double a[MATRIX_SIZE][MATRIX_SIZE], double b[MATRIX_SIZE][MATRIX_SIZE])
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            b[i][j] = a[i][j];
        }
    }
}

void matrix_multiply(double a[MATRIX_SIZE][MATRIX_SIZE], double b[MATRIX_SIZE][MATRIX_SIZE], double result[MATRIX_SIZE][MATRIX_SIZE])
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            result[i][j] = 0; // Initialize the element
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_add(double a[MATRIX_SIZE][MATRIX_SIZE], double b[MATRIX_SIZE][MATRIX_SIZE], double result[MATRIX_SIZE][MATRIX_SIZE])
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

double factorial(int n)
{
    double fact = 1;
    for (int i = 1; i <= n; i++)
    {
        fact *= i;
    }

    return fact;
}

void matrix_exp(double a[MATRIX_SIZE][MATRIX_SIZE], double result[MATRIX_SIZE][MATRIX_SIZE], int terms)
{
    double term[MATRIX_SIZE][MATRIX_SIZE];
    // Initialize result as Identity matrix
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            result[i][j] = (i == j) ? 1.0 : 0.0;
            term[i][j] = result[i][j];
        }
    }

    // Add terms of the series
    for (int n = 1; n <= terms; n++)
    {
        double fact = factorial(n);
        double temp[MATRIX_SIZE][MATRIX_SIZE], temp2[MATRIX_SIZE][MATRIX_SIZE];
        matrix_multiply(term, a, temp);
        copy_matrix(temp, term);
        print_matrix(term);
        printf("\n");
        copy_matrix(temp, temp2);

        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            for (int j = 0; j < MATRIX_SIZE; j++)
            {
                temp2[i][j] /= fact;
            }
        }

        matrix_add(result, temp2, temp);
        copy_matrix(temp, result);
    }
}

double matrix_det(double a[MATRIX_SIZE][MATRIX_SIZE])
{
    return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}

int main(int argc, char **argv)
{
    int n = 10;
    if (argc > 1)
    {
        n = atoi(argv[1]);
    }

    double A[MATRIX_SIZE][MATRIX_SIZE] = {{0, -3.14159265358979323846}, {3.14159265358979323846, 0}};
    double expA[MATRIX_SIZE][MATRIX_SIZE];

    matrix_exp(A, expA, n); // using 10 terms for approximation

    // Print result
    print_matrix(expA);

    double det = matrix_det(expA);
    printf("Determinant: %lf\n", det);

    return 0;
}
