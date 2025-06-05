#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

int partition(double* arr, int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort_parallel(double* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task
        quicksort_parallel(arr, low, pi - 1);

        #pragma omp task
        quicksort_parallel(arr, pi + 1, high);
    }
}

int main(int argc, char* argv[]) {
    int N = 100000;
    int opt;
    int num_threads = omp_get_max_threads();

    while ((opt = getopt(argc, argv, "n:t:")) != -1) {
        switch (opt) {
            case 'n':
                N = atoi(optarg);
                if (N <= 0) {
                    fprintf(stderr, "Error: N must be a positive integer.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 't':
                num_threads = atoi(optarg);
                if (num_threads <= 0) {
                    fprintf(stderr, "Error: Thread count must be positive.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-n array_size] [-t num_threads]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    double* array = malloc(N * sizeof(double));
    if (array == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) ^ getpid());
    for (int i = 0; i < N; i++) {
        array[i] = (double)rand() / RAND_MAX * 1000.0;
    }

    omp_set_num_threads(num_threads);
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single nowait
        quicksort_parallel(array, 0, N - 1);
    }

    double end = omp_get_wtime();

    printf("Parallel time: %.5f seconds\n", end - start);
    printf("Threads used: %d\n", num_threads);

    free(array);
    return 0;
}
