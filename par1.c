#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int N = 100000;
    int opt;
    int num_threads = omp_get_max_threads(); // Использовать максимум по умолчанию

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

    omp_set_num_threads(num_threads);

    double* array = malloc(N * sizeof(double));
    if (array == NULL) {
        perror("Failed to allocate memory for array");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) ^ getpid());
    for (int i = 0; i < N; i++) {
        array[i] = (double)rand() / RAND_MAX;
    }

    double final_sum = 0.0;
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+:final_sum)
    for (int i = 0; i < N; i++) {
        final_sum += array[i];
    }
    double end = omp_get_wtime();

    printf("Parallel sum: %.6f\n", final_sum);
    printf("Threads used: %d\n", num_threads);
    printf("Time: %.5f seconds\n", end - start);

    free(array);
    return 0;
}
