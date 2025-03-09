#ifndef UTILITIES_LIB_H
#define UTILITIES_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <linux/limits.h>
#include <ctype.h>
#include "matrixMultiplication.h"

int PWD();
void printMatrix(Matrix *matrix);
Matrix readMatrix(char *matix_name);
void writeMatrix(Matrix *matrix, char *file_name);


#endif /* UTILITIES_LIB_H */