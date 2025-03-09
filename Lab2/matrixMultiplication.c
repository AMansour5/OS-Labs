#include "matrixMultiplication.h"
#include "utilities.c"

int main(int argc, char *argv[]){
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    char *output_matrix_name;
    char *file2_name;
    char *file1_name;

    if (argc == 4){
        output_matrix_name = strdup(argv[3]);
        file2_name = strdup(argv[2]);
        file1_name = strdup(argv[1]);
    }
    else if (argc == 1) {
        output_matrix_name = strdup("c");
        file2_name = strdup("b");
        file1_name = strdup("a");
    }
    else {
        fprintf(stderr, "Wrong input\n");
        return EXIT_FAILURE;
    }
    Matrix matrix_2 = readMatrix(file2_name);
    Matrix matrix_1 = readMatrix(file1_name);
    writeMatrix(&matrix_1, output_matrix_name);
    gettimeofday(&stop, NULL);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
}


