// to compile for openblas: gcc openblas_matrix_multiplication.c -lopenblas -lm && sudo ./a.out

#include <cblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

static float random_decimal() {
    return random() / (float) RAND_MAX;
}

static void fill_matrix(float *p, const int height, const int width) {
    int i, j;

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            p[i * width + j] = cosf(2.0 * M_PI * random_decimal()) * sqrtf(-2.0 * logf(1.0 - random_decimal()));
        }
    }
}

int write_data(int row) {
    char *cmd = "sudo rdserialtool --device=um25c --bluetooth-address=00:16:A6:00:13:2B --quiet --json";

    FILE * fPtr;
    fPtr = fopen("data.txt", "a");

    char buf[128];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if (fPtr != NULL) {
        fputs("\"", fPtr);
        fprintf(fPtr, "%d", row);
        fputs("\": ", fPtr);
    }

    while (fgets(buf, 128, fp) != NULL) {
        if (fPtr != NULL) {
            fputs(buf, fPtr);
        }
    }

    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    fclose(fPtr);
    return 0;
}

int run_sgemm(const unsigned rows, const unsigned columns) {
    float *A, *B, *C;
    struct timeval start, end, tv;

    A = malloc(rows * columns * 4);
    B = malloc(columns * rows * 4);
    C = malloc(rows * rows * 4);
    
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec ^ tv.tv_usec);

    fill_matrix(A, rows, columns);
    fill_matrix(B, columns, rows);
    fill_matrix(C, rows, rows);

    fflush(stdout);

    //Runs SGEMM and times it with start and end
    gettimeofday(&start, NULL);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows, rows, columns, 1.0, A, columns, B, rows, 0.0, C, rows);
    gettimeofday(&end, NULL);

    //Prints out FLOP/S
    printf("%g\n", (2 * rows * columns * rows + 3 * rows * rows) / ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6));

    free(C);
    free(B);
    free(A);

    return 0;
}

int main() {
    for (int i = 64; i <= 2048; i = i * 2) {
        const unsigned rows = i;
        const unsigned columns = 256;

        run_sgemm(rows, columns);
        // write_data(rows);
    }

    return 0;
}