#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

void array_operations_parallel(double* a, double* b, double* sum, double* diff, double* prod, double* quot, int N) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        sum[i] = a[i] + b[i];
        diff[i] = a[i] - b[i];
        prod[i] = a[i] * b[i];
        quot[i] = (b[i] != 0.0) ? (a[i] / b[i]) : 0.0;
    }
}

int main(int argc, char* argv[]) {
    int N = 10000000;
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
                    fprintf(stderr, "Error: Number of threads must be positive.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-n array_size] [-t num_threads]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    double* a = malloc(N * sizeof(double));
    double* b = malloc(N * sizeof(double));
    double* sum = malloc(N * sizeof(double));
    double* diff = malloc(N * sizeof(double));
    double* prod = malloc(N * sizeof(double));
    double* quot = malloc(N * sizeof(double));

    if (!a  !b  !sum  !diff  !prod || !quot) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) ^ getpid());
    for (int i = 0; i < N; i++) {
        a[i] = (double)rand() / RAND_MAX * 100.0 + 1.0;
        b[i] = (double)rand() / RAND_MAX * 100.0 + 1.0;
    }

    omp_set_num_threads(num_threads);
    double start = omp_get_wtime();
    array_operations_parallel(a, b, sum, diff, prod, quot, N);
    double end = omp_get_wtime();

    printf("Parallel time: %.5f seconds\n", end - start);
    printf("Threads used: %d\n", num_threads);

    free(a); free(b); free(sum); free(diff); free(prod); free(quot);
    return 0;
}
