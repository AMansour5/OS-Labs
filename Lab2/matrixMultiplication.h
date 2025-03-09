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


void *matrixMultiplication();
void *threadPerMatrix();
void *threadPerRow();
void *threadPerElement();

#endif /* MATRIX_LIB_H */


