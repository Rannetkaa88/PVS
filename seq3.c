#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void array_operations_sequential(double* a, double* b, double* sum, double* diff, double* prod, double* quot, int N) {
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

    clock_t start = clock();
    array_operations_sequential(a, b, sum, diff, prod, quot, N);
    clock_t end = clock();

    printf("Sequential time: %.5f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(a); free(b); free(sum); free(diff); free(prod); free(quot);
    return 0;
}
