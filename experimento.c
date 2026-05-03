#include <stdio.h>
#include <stdlib.h>

static void standard_multiplication(int n, double* A, double* B, double* C){
  for(int i = 0; i < n; i++){ //filas
    for(int j = 0; j < n; j++){ //columnas
      C[i*n + j] = 0.0;
      for(int k = 0; k < n; k++)
        C[i*n + j] += A[i*n + k]*B[k*n + j];
    }
  } 
}

static void print(double* matrix, int n){
  for(int i = 0; i < n; i++){
    for(int j = 0;j < n; j++){
      printf("%.2f ", matrix[i*n + j]);
    }
    printf("\n");
  }
}

int main(void){
 
  int n = 2;

  double* matrix_A = malloc(n*n*sizeof(double));
  double* matrix_B = malloc(n*n*sizeof(double));
  double* matrix_C = malloc(n*n*sizeof(double));
 
  if (!matrix_A || !matrix_B || !matrix_C) return 1; 
 
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      matrix_A[i*n + j] = (i+1)*(j+2);
      matrix_B[i*n + j] = (i+j);
    }
  }

  standard_multiplication(n, matrix_A, matrix_B, matrix_C);

  printf("\nMatriz A:\n");
  print(matrix_A, n);

  printf("\nMatriz B:\n");
  print(matrix_B, n);

  printf("\nMatriz C = A*B:\n");
  print(matrix_C, n);
 
  free(matrix_A);
  free(matrix_B);
  free(matrix_C);

  return 0;
}
