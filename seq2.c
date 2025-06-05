#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

void quicksort_sequential(double* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort_sequential(arr, low, pi - 1);
        quicksort_sequential(arr, pi + 1, high);
    }
}

int main(int argc, char* argv[]) {
    int N = 100000;
    int opt;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt == 'n') {
            N = atoi(optarg);
            if (N <= 0) {
                fprintf(stderr, "Error: N must be a positive integer.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Usage: %s [-n array_size]\n", argv[0]);
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

    clock_t start = clock();
    quicksort_sequential(array, 0, N - 1);
    clock_t end = clock();

    printf("Sequential time: %.5f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(array);
    return 0;
}
