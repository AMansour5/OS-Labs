#ifndef MATRIX_LIB_H
#define MATRIX_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>

typedef struct {
    int rows;
    int cols;
    int *data;  
} Matrix;

typedef struct {
    int row;           // The row index to compute.
    Matrix *matrix_a;  // Left-hand side matrix.
    Matrix *matrix_b;  // Right-hand side matrix.
    Matrix *result;    // Result matrix.
} ThreadRow;

typedef struct {
    int row;         // The row index of the element in the result
    int col;         // The column index of the element in the result
    Matrix *matrix_a; // Left-hand side matrix
    Matrix *matrix_b; // Right-hand side matrix
    Matrix *result;   // Result matrix
} ThreadElement;

void matrixMultiplication(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name);
void threadPerMatrix(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name);
void threadPerRow(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name);
void* multiplyRow(void *arg);
void threadPerElement(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name);
void* multiplyElement(void *arg);

#endif /* MATRIX_LIB_H */


