#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int N = 100000;
    int opt;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                N = atoi(optarg);
                if (N <= 0) {
                    fprintf(stderr, "Error: N must be a positive integer.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-n array_size]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

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
    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        final_sum += array[i];
    }
    clock_t end = clock();

    printf("Sequential sum: %.6f\n", final_sum);
    printf("Time: %.5f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(array);
    return 0;
}
