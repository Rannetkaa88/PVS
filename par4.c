#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

void initialize_array(double **array, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            array[i][j] = (double)rand() / RAND_MAX * 100.0 + 1.0;
}

void parallel_operations(double **a, double **b, double **result, int rows, int cols, char op) {
#pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            switch (op) {
                case '+': result[i][j] = a[i][j] + b[i][j]; break;
                case '-': result[i][j] = a[i][j] - b[i][j]; break;
                case '*': result[i][j] = a[i][j] * b[i][j]; break;
                case '/': result[i][j] = (b[i][j] != 0.0) ? a[i][j] / b[i][j] : 0.0; break;
            }
        }
}

double **allocate_array(int rows, int cols) {
    double **array = malloc(rows * sizeof(double *));
    if (!array) return NULL;
    for (int i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(double));
        if (!array[i]) {
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return NULL;
        }
    }
    return array;
}

void free_array(double **array, int rows) {
    for (int i = 0; i < rows; i++) free(array[i]);
    free(array);
}

int main(int argc, char *argv[]) {
    int n = 100000;
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt == 'n') {
            n = atoi(optarg);
            if (n < 100000) {
                printf("Array size must be at least 100000. Using default 100000.\n");
                n = 100000;
            }
        } else {
            fprintf(stderr, "Usage: %s -n <number_of_elements>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int rows = (int)sqrt(n), cols = n / rows;
    if (rows * cols < n) cols++;

    double **a = allocate_array(rows, cols);
    double **b = allocate_array(rows, cols);
    double **result = allocate_array(rows, cols);
    if (!a  !b  !result) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    initialize_array(a, rows, cols);
    initialize_array(b, rows, cols);

    char operations[] = {'+', '-', '*', '/'};
    const char *op_names[] = {"Addition", "Subtraction", "Multiplication", "Division"};

    for (int op_idx = 0; op_idx < 4; op_idx++) {
        char op = operations[op_idx];
        printf("\nOperation: %s\n", op_names[op_idx]);

        omp_set_num_threads(omp_get_max_threads());
        double start = omp_get_wtime();
        parallel_operations(a, b, result, rows, cols, op);
        double end = omp_get_wtime();
        printf("Parallel time: %.5f seconds\n", end - start);
    }

    free_array(a, rows);
    free_array(b, rows);
    free_array(result, rows);
    return 0;
}
