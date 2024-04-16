#include <stdio.h>
#include <stdlib.h>

int count_lines(char *filename) {
    FILE *f = fopen(filename, "r");

    int count = 0;
    double num;
    while (fscanf(f, "%lf", &num) == 1) {
        count++;
    }

    fclose(f);
    return count;
}

int main() {
    FILE *fx, *fy, *fA;
    double *x, *y, **A;
    double numerator = 0.0, denominator = 0.0, AVG;

    // Determine N from the size of the x.dat file or ask the user for N
    // For now, let's say we ask the user
    int N = count_lines("x.dat");

    // Allocate memory for vectors and matrix
    x = (double *)malloc(N * sizeof(double));
    y = (double *)malloc(N * sizeof(double));
    A = (double **)malloc(N * sizeof(double *));
    for(int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
    }

    // Open files and read data
    fx = fopen("x.dat", "r");
    fy = fopen("y.dat", "r");
    fA = fopen("mat.dat", "r");

    if(fx == NULL || fy == NULL || fA == NULL) {
        printf("Error opening files!\n");
        return 1;
    }

    // Read x and y vectors
    for(int i = 0; i < N; i++) {
        fscanf(fx, "%lf", &x[i]);
        fscanf(fy, "%lf", &y[i]);
    }

    // Read A matrix
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            fscanf(fA, "%lf", &A[i][j]);
        }
    }

    // Compute numerator and denominator
    for(int i = 0; i < N; i++) {
        denominator += x[i] * y[i];
        for(int j = 0; j < N; j++) {
            numerator += x[i] * A[i][j] * y[j];
        }
    }

    // Close files
    fclose(fx);
    fclose(fy);
    fclose(fA);

    // Check for division by zero
    if(denominator == 0) {
        printf("Denominator is zero, cannot compute AVG.\n");
        return 1;
    }

    // Calculate AVG
    AVG = numerator / denominator;

    // Print the result
    printf("AVG = %lf\n", AVG);

    // Free allocated memory
    free(x);
    free(y);
    for(int i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
