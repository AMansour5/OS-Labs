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
    if (matrix_1.cols != matrix_2.rows) {
        perror("Error: Incompatible dimensions for multiplication.");
        free(matrix_1.data);
        free(matrix_2.data);
        exit(EXIT_FAILURE);
    }
    matrixMultiplication(&matrix_1, &matrix_2, output_matrix_name);
    gettimeofday(&stop, NULL);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
}

void matrixMultiplication(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    threadPerMatrix(matrix_a, matrix_b, matrix_name);
    threadPerRow(matrix_a, matrix_b, matrix_name);
    // threadPerElement(matrix_a, matrix_b, martix_name);
}

void threadPerMatrix(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    Matrix result;
    result.rows = matrix_a->rows;
    result.cols = matrix_b->cols;
    result.data = malloc(result.rows * result.cols * sizeof(int));
    if (result.data == NULL) {
        perror("Memory allocation failed for result matrix");
        free(result.data);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            int sum = 0;
            for (int k = 0; k < matrix_a->cols; k++) {
                sum += matrix_a->data[i * matrix_a->cols + k] * 
                       matrix_b->data[k * matrix_b->cols + j];
            }
            result.data[i * result.cols + j] = sum;
        }
    } 
    strcat(matrix_name, "__per_matrix");
    writeMatrix(&result, matrix_name);
    free(result.data);  
}
void threadPerRow(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    Matrix result;
    result.rows = matrix_a->rows;
    result.cols = matrix_b->cols;
    result.data = malloc(result.rows * result.cols * sizeof(int));
    if (result.data == NULL) {
        perror("Memory allocation failed for result matrix");
        exit(EXIT_FAILURE);
    }
    pthread_t *threads = malloc(result.rows * sizeof(pthread_t));
    ThreadData *thread_data = malloc(result.rows * sizeof(ThreadData));
    if (!threads || !thread_data) {
        perror("Memory allocation failed for threads");
        free(result.data);
        free(threads);
        free(thread_data);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < result.rows; i++) {
        thread_data[i].row = i;
        thread_data[i].matrix_a = matrix_a;
        thread_data[i].matrix_b = matrix_b;
        thread_data[i].result = &result;
        pthread_create(&threads[i], NULL, multiplyRow, &thread_data[i]);
    }
    for (int i = 0; i < result.rows; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    free(thread_data);
    strcat(matrix_name, "__per_row");
    writeMatrix(&result, matrix_name);  
    free(result.data);
}

void* multiplyRow(void *arg){
    ThreadData *data = (ThreadData *) arg;
    int row = data->row;
    Matrix *A = data->matrix_a;
    Matrix *B = data->matrix_b;
    Matrix *R = data->result;
    for (int j = 0; j < B->cols; j++) {
        int sum = 0;
        for (int k = 0; k < A->cols; k++) {
            sum += A->data[row * A->cols + k] * B->data[k * B->cols + j];
        }
        R->data[row * R->cols + j] = sum;
    }
    return NULL;
}



