#include "matrixMultiplication.h"
#include "utilities.c"

int main(int argc, char *argv[]){
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    char *output_matrix_name = (argc > 3) ? strdup(argv[3]) : "c";
    char *file2 = (argc > 2) ? strdup(argv[2]) : "b";
    char *file1 = (argc > 1) ? strdup(argv[1]) : "a";
    if (argc != 4 && argc != 1) {
        fprintf(stderr, "Wrong input");
        return EXIT_FAILURE;
    }
    Matrix matrix_2 = readMatrix(file2);
    Matrix matrix_1 = readMatrix(file1);
    writeMatrix(&matrix_1, output_matrix_name);
    gettimeofday(&stop, NULL);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
}


