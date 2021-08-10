// to compile: gcc openblas-sgemm.c -lopenblas -lm

#include <cblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

static void mf_srandom()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec ^ tv.tv_usec);
}

static float urand()
{
    return random() / (float) RAND_MAX;
}

static void mf_init_random(float *p, const int height, const int width)
{
    int i, j;

    for (i = 0; i < height; i ++)
        for (j = 0; j < width; j ++)
            p[i * width + j] = cosf(2.0 * M_PI * urand()) * sqrtf(-2.0 * logf(1.0 - urand()));
}

int run_sgemm(const unsigned P_val, const unsigned Q_val)
{
    const unsigned P = P_val;
    const unsigned Q = Q_val;
    const unsigned R = P_val;
    float *A, *B, *C;
    float ALPHA;
    float BETA;
    struct timeval start, end;

    A = malloc(P * Q * (32 / 8));
    B = malloc(Q * R * (32 / 8));
    C = malloc(P * R * (32 / 8));

    ALPHA = 1.0;
    BETA = 0.0;
    mf_srandom();
    mf_init_random(A, P, Q);
    mf_init_random(B, Q, R);
    mf_init_random(C, P, R);

    fflush(stdout);
    gettimeofday(&start, NULL);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, P, R, Q, ALPHA, A, Q, B, R, BETA, C, R);
    gettimeofday(&end, NULL);
    printf("%g\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6, (2 * P * Q * R + 3 * P * R) / ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6));
    
    free(C);
    free(B);
    free(A);

    return 0;
}

int main(int argc, char *argv[]) {
    for (int i = 1001; i <= 2000; i++) {
        const unsigned rows = i;
        const unsigned columns = 512;

        run_sgemm(rows, columns);
    }
}