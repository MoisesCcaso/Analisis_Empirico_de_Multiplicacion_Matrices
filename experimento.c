#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//termina el programa si un malloc falla
#define MALLOC_CHECK(ptr) do { if (!(ptr)) { fprintf(stderr, "malloc failed\n"); exit(1); } } while(0)

//contabilizar tiempo de ejecución 
static long long elapsedNanoseconds(struct timespec start, struct timespec end) {
  long long seconds = (long long)(end.tv_sec - start.tv_sec);
  long long nanoseconds = (long long)(end.tv_nsec - start.tv_nsec);
  return seconds * 1000000000LL + nanoseconds;
}

//generar matrices con valores aleatorios
static void generateRandomArray(double arr[], int n, double maxValue) {
  for(int i = 0; i < n * n; i++) {
    arr[i] = ((double)rand() / RAND_MAX) * maxValue;
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
    if (n == 1) {
        C[0] = A[0] * B[0];
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

// imprimir elementos de una matriz
static void print(double* matrix, int n){
  for(int i = 0; i < n; i++){
    for(int j = 0;j < n; j++){
      printf("%.2f ", matrix[i*n + j]);
    }
    printf("\n");
  }
}
 
int main(void){
  
  //semilla fija para replicar experimentos
  srand(12345);

  return 0;
}
