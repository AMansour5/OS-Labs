#include "utilities.h"

int PWD(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
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