#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_VALUE 1000.0
#define REPS 30

//termina el programa si un malloc falla
#define MALLOC_CHECK(ptr) do { if (!(ptr)) { fprintf(stderr, "malloc failed\n"); exit(1); } } while(0)

//contabilizar tiempo de ejecución 
static long long elapsedNanoseconds(struct timespec start, struct timespec end) {
  long long seconds = (long long)(end.tv_sec - start.tv_sec);
  long long nanoseconds = (long long)(end.tv_nsec - start.tv_nsec);
  return seconds * 1000000000LL + nanoseconds;
}

//generar matrices con valores aleatorios
static void generateRandomArray(double arr[], int n) {
  for(int i = 0; i < n * n; i++) {
    arr[i] = ((double)rand() / RAND_MAX) * MAX_VALUE;
  }
}

//multiplicación estándar O(n^3)
static void standard_multiplication(int n, double* A, double* B, double* C){
  for(int i = 0; i < n; i++){ //filas
    for(int j = 0; j < n; j++){ //columnas
      C[i*n + j] = 0.0;
      for(int k = 0; k < n; k++)
        C[i*n + j] += A[i*n + k]*B[k*n + j];
    }
  } 
}

static void add(int n, double* A, double* B, double* C) {
  for (int i = 0; i < n*n; i++)
    C[i] = A[i] + B[i];
}
 
static void sub(int n, double* A, double* B, double* C) {
  for (int i = 0; i < n*n; i++)
    C[i] = A[i] - B[i];
}

//multiplicación de Strassen O(n^2.807) 
static void strassen_multiplication(int n, double* A, double* B, double* C)
{
    if (n == 1){ 
        C[0] = A[0]*B[0];
        return;
    }
 
    int k = n / 2;
    int size = k * k;

    // extraer los 4 bloques de A y B 
    double *a = malloc(size * sizeof(double)); MALLOC_CHECK(a);
    double *b = malloc(size * sizeof(double)); MALLOC_CHECK(b);
    double *c = malloc(size * sizeof(double)); MALLOC_CHECK(c);
    double *d = malloc(size * sizeof(double)); MALLOC_CHECK(d);
    double *e = malloc(size * sizeof(double)); MALLOC_CHECK(e);
    double *f = malloc(size * sizeof(double)); MALLOC_CHECK(f);
    double *g = malloc(size * sizeof(double)); MALLOC_CHECK(g);
    double *h = malloc(size * sizeof(double)); MALLOC_CHECK(h);
 
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            a[i*k + j] = A[i*n + j];
            b[i*k + j] = A[i*n + j + k];
            c[i*k + j] = A[(i+k)*n + j];
            d[i*k + j] = A[(i+k)*n + j + k];
            e[i*k + j] = B[i*n + j];
            f[i*k + j] = B[i*n + j + k];
            g[i*k + j] = B[(i+k)*n + j];
            h[i*k + j] = B[(i+k)*n + j + k];
        }
    // los 7 productos de Strassen 
    double *P1 = malloc(size * sizeof(double)); MALLOC_CHECK(P1);
    double *P2 = malloc(size * sizeof(double)); MALLOC_CHECK(P2);
    double *P3 = malloc(size * sizeof(double)); MALLOC_CHECK(P3);
    double *P4 = malloc(size * sizeof(double)); MALLOC_CHECK(P4);
    double *P5 = malloc(size * sizeof(double)); MALLOC_CHECK(P5);
    double *P6 = malloc(size * sizeof(double)); MALLOC_CHECK(P6);
    double *P7 = malloc(size * sizeof(double)); MALLOC_CHECK(P7);
    double *T1 = malloc(size * sizeof(double)); MALLOC_CHECK(T1);
    double *T2 = malloc(size * sizeof(double)); MALLOC_CHECK(T2);
 
    add(k, a, d, T1); add(k, e, h, T2);
    strassen_multiplication(k, T1, T2, P1);
 
    add(k, c, d, T1);
    strassen_multiplication(k, T1, e, P2);
 
    sub(k, f, h, T2);
    strassen_multiplication(k, a, T2, P3);
 
    sub(k, g, e, T2);
    strassen_multiplication(k, d, T2, P4);
 
    add(k, a, b, T1);
    strassen_multiplication(k, T1, h, P5);
 
    sub(k, c, a, T1); add(k, e, f, T2);
    strassen_multiplication(k, T1, T2, P6);
 
    sub(k, b, d, T1); add(k, g, h, T2);
    strassen_multiplication(k, T1, T2, P7);

    // combinar resultados en los 4 bloques de C 
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            C[i*n + j]         = P1[i*k+j] + P4[i*k+j] - P5[i*k+j] + P7[i*k+j];
            C[i*n + j + k]     = P3[i*k+j] + P5[i*k+j];
            C[(i+k)*n + j]     = P2[i*k+j] + P4[i*k+j];
            C[(i+k)*n + j + k] = P1[i*k+j] - P2[i*k+j] + P3[i*k+j] + P6[i*k+j];
        }
 
    free(a); free(b); free(c); free(d);
    free(e); free(f); free(g); free(h);
    free(P1); free(P2); free(P3); free(P4);
    free(P5); free(P6); free(P7);
    free(T1); free(T2);
}

static double measureStrassen(int n , double* A, double* B, double* C) {
  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  strassen_multiplication(n, A, B, C);
  clock_gettime(CLOCK_MONOTONIC, &end);

  return (double)elapsedNanoseconds(start, end) / 1000.0;
}

static double measureStandard(int n , double* A, double* B, double* C) {
  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  standard_multiplication(n, A, B, C);
  clock_gettime(CLOCK_MONOTONIC, &end);

  return (double)elapsedNanoseconds(start, end) / 1000.0;
}

int main(void){
  
  //semilla fija para replicar experimentos
  srand(42);

  printf("# n standar_multiplication_us vs  strassen_multiplication_us\n");

  //casos típicos con n = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}
  for(int n = 2; n <= 1024; n *= 2) {
    double* matrixA = malloc(n * n *  sizeof(double)); MALLOC_CHECK(matrixA);
    double* matrixB = malloc(n * n *  sizeof(double)); MALLOC_CHECK(matrixB);
    double* matrixC = malloc(n * n *  sizeof(double)); MALLOC_CHECK(matrixC);
    double* matrixC_2 = malloc(n * n *  sizeof(double)); MALLOC_CHECK(matrixC_2);

    generateRandomArray(matrixA, n);
    generateRandomArray(matrixB, n);
    
    double standardTotal = 0.0;
    double strassenTotal = 0.0;

    for(int r = 0; r < REPS; r++){
      standardTotal += measureStandard(n, matrixA, matrixB, matrixC);
      strassenTotal += measureStrassen(n, matrixA, matrixB, matrixC_2);
    }

    printf("%d %.3f %.3f\n", n, standardTotal/REPS, strassenTotal/REPS);
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(matrixC_2);
  }

  return EXIT_SUCCESS;
}
