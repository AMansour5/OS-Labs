#ifndef MATRIX_LIB_H
#define MATRIX_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <linux/limits.h>

typedef struct {
    int rows;
    int cols;
    int *data;  
} Matrix;


void matrixMultiplication(Matrix *matrix_a, Matrix *matrix_b, char *matix_name);
void threadPerMatrix(Matrix *matrix_a, Matrix *matrix_b, char *martix_name);
void threadPerRow(Matrix *matrix_a, Matrix *matrix_b, char *martix_name);
void* multiplyRow(void *arg);
void threadPerElement(Matrix *matrix_a, Matrix *matrix_b, char *martix_name);

#endif /* MATRIX_LIB_H */


