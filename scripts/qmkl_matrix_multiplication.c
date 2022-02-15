#include <mkl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

// Returns a random float by dividing random() by the RAND_MAX constant
static float random_float() {
    return random() / (float) RAND_MAX;
}

// Fills in the matrix with the given width and height with a random number
static void fill_matrix(float *p, const int height, const int width) {
    int i, j;

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            p[i * width + j] = cosf(2.0 * M_PI * random_float()) * sqrtf(-2.0 * logf(1.0 - random_float()));
        }
    }
}

// Writes out performance data to a text file in JSON format
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

// Runs QPU-accelerated SGEMM (Single precision, GEneral Matrix Multiply) as C = A * B
// Prints out the FLOP/s for analysis
int run_sgemm(const unsigned rows, const unsigned columns) {
    float *A, *B, *C;
    float ALPHA;
    float BETA;
    struct timeval start, end;

    A = mkl_malloc(rows * columns * 4, 4096);
    B = mkl_malloc(columns * rows * 4, 4096);
    C = mkl_malloc(rows * rows * 4, 4096);
    ALPHA = 1.0;
    BETA = 0.0;
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec ^ tv.tv_usec);

    fill_matrix(A, rows, columns);
    fill_matrix(B, columns, rows);
    fill_matrix(C, rows, rows);

    fflush(stdout);

    gettimeofday(&start, NULL);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows, rows, columns, ALPHA, A, columns, B, rows, BETA, C, rows);
    gettimeofday(&end, NULL);
    
    printf("%g\n", (2 * rows * columns * rows + 3 * rows * rows) / ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6));

    mkl_free(C);
    mkl_free(B);
    mkl_free(A);

    return 0;
}

// Main driver, runs SGEMM and varies the rows from 64 to 2048
int main() {
    for (int i = 64; i <= 2048; i = i * 2) {
        const unsigned rows = i;
        const unsigned columns = 256;

        run_sgemm(rows, columns);
        // write_data(rows);
    }

    return 0;
}