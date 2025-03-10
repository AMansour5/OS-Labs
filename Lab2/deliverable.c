#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <ctype.h>

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
void printMatrix(Matrix *matrix);
Matrix readMatrix(char *matrix_name);
void writeMatrix(Matrix *matrix, char *file_name);

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
    printf("Seconds taken by whole program  %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken by whole program: %lu\n", stop.tv_usec - start.tv_usec);
}

void matrixMultiplication(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    threadPerMatrix(matrix_a, matrix_b, matrix_name);
    threadPerRow(matrix_a, matrix_b, matrix_name);
    threadPerElement(matrix_a, matrix_b, matrix_name);
}

void threadPerMatrix(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    struct timeval start_mat, stop_mat;
    gettimeofday(&start_mat, NULL);
    Matrix result;
    char *name;
    name = strdup(matrix_name);
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
    strcat(name, "_per_matrix.txt");
    writeMatrix(&result, name);
    free(result.data);
    printf("Seconds taken by thread per matrix  %lu\n", stop_mat.tv_sec - start_mat.tv_sec);
    printf("Microseconds taken by thread per matrix: %lu\n", stop_mat.tv_usec - start_mat.tv_usec);  
}
void threadPerRow(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    struct timeval start_row, stop_row;
    gettimeofday(&start_row, NULL);
    char *name;
    name = strdup(matrix_name);
    Matrix result;
    result.rows = matrix_a->rows;
    result.cols = matrix_b->cols;
    result.data = malloc(result.rows * result.cols * sizeof(int));
    if (result.data == NULL) {
        perror("Memory allocation failed for result matrix");
        exit(EXIT_FAILURE);
    }
    pthread_t *threads = malloc(result.rows * sizeof(pthread_t));
    ThreadRow *thread_data = malloc(result.rows * sizeof(ThreadRow));
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
    strcat(name, "_per_row.txt");
    writeMatrix(&result, name);  
    free(result.data);
    printf("Seconds taken by thread per row  %lu\n", stop_row.tv_sec - start_row.tv_sec);
    printf("Microseconds taken by thread per row: %lu\n", stop_row.tv_usec - start_row.tv_usec);
    printf("Number of threads used in thread per rows: %d\n", matrix_a->rows);
}

void* multiplyRow(void *arg){
    ThreadRow *data = (ThreadRow *) arg;
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

void threadPerElement(Matrix *matrix_a, Matrix *matrix_b, char *matrix_name){
    struct timeval start_element, stop_element;
    gettimeofday(&start_element, NULL);
    char *name;
    name = strdup(matrix_name);
    Matrix result;
    result.rows = matrix_a->rows;
    result.cols = matrix_b->cols;
    result.data = malloc(result.rows * result.cols * sizeof(int));
    if (result.data == NULL) {
        perror("Memory allocation failed for result matrix");
        exit(EXIT_FAILURE);
    }
    int num_elements = result.rows * result.cols;
    pthread_t *threads = malloc(num_elements * sizeof(pthread_t));
    ThreadElement *thread_data = malloc(num_elements * sizeof(ThreadElement));
    if (!threads || !thread_data) {
        perror("Memory allocation failed for threads");
        free(result.data);
        free(threads);
        free(thread_data);
        exit(EXIT_FAILURE);
    }
    int index = 0;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            thread_data[index].row = i;
            thread_data[index].col = j;
            thread_data[index].matrix_a = matrix_a;
            thread_data[index].matrix_b = matrix_b;
            thread_data[index].result = &result;
            
            pthread_create(&threads[index], NULL, multiplyElement, &thread_data[index]);
            index++;
        }
    }
    for (int i = 0; i < num_elements; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    free(thread_data);
    strcat(name, "_per_element.txt");
    writeMatrix(&result, name);  
    free(result.data);
    printf("Seconds taken by thread per element  %lu\n", stop_element.tv_sec - start_element.tv_sec);
    printf("Microseconds taken by thread per element: %lu\n", stop_element.tv_usec - start_element.tv_usec);
    printf("Number of threads used in thread per element: %d\n", num_elements);
}


void* multiplyElement(void *arg) {
    ThreadElement *data = (ThreadElement *) arg;
    int i = data->row;
    int j = data->col;
    int sum = 0;
    for (int k = 0; k < data->matrix_a->cols; k++) {
        sum += data->matrix_a->data[i * data->matrix_a->cols + k] *
               data->matrix_b->data[k * data->matrix_b->cols + j];
    }
    data->result->data[i * data->result->cols + j] = sum;
    return NULL;
}

void printMatrix(Matrix *matrix){
    if (matrix == NULL || matrix->data == NULL) {
        fprintf(stderr, "Matrix is empty or null\n");
        return;
    }
    
    printf("Matrix (%d x %d):\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%d\t", matrix->data[i * matrix->cols + j]);
        }
        printf("\n");
    }
}

Matrix readMatrix(char *matrix_name){
    strcat(matrix_name, ".txt");
    Matrix matrix = {0, 0, NULL};
    FILE *file = fopen(matrix_name, "r");
    if (file == NULL){
        perror("File not Found");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "row=%d col=%d", &matrix.rows, &matrix.cols) != 2) {
        fprintf(stderr, "Error reading dimensions from file\n");
        fclose(file);
        exit(EXIT_FAILURE);   
    }
    matrix.data = malloc(matrix.rows * matrix.cols * sizeof(int));
    if (matrix.data == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            if (fscanf(file, "%d", &matrix.data[i * matrix.cols + j]) != 1) {
                fprintf(stderr, "Error reading data at row %d, col %d\n", i, j);
                free(matrix.data);
                matrix.data = NULL;
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
    return matrix;
}

void writeMatrix(Matrix *matrix, char *file_name){
    FILE *file = fopen(file_name, "w");
    if (file == NULL){
        perror("Error creating file to write matrix");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "row=%d col=%d\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++){
        for (int j = 0; j < matrix->cols; j++){
            fprintf(file, "%d\t", matrix->data[i * matrix->cols + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

