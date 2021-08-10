/*
 * Copyright (c) 2016 Idein Inc. ( http://idein.jp/ )
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include <mkl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#if 0
#define DO_1FILL
#endif

#ifdef DO_1FILL
static void mf_init_constant(float *p, const int height, const int width, const float c)
{
    int i, j;

    for (i = 0; i < height; i ++)
        for (j = 0; j < width; j ++)
            p[i * width + j] = c;
}

#else /* DO_1FILL */

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
            p[i * width + j] = cosf(2.0 * M_PI * urand())
                               * sqrtf(-2.0 * logf(1.0 - urand()));
}

#endif /* DO_1FILL */

int run_sgemm(const unsigned P_val, const unsigned Q_val)
{
    const unsigned P = P_val;
    const unsigned Q = Q_val;
    const unsigned R = P_val;
    float *A, *A_ref, *B, *B_ref, *C, *C_ref;
    float ALPHA;
    float BETA;
    struct timeval start, end;

    A     = mkl_malloc(P * Q * (32 / 8), 4096);
    A_ref = malloc(P * Q * (32 / 8));
    B     = mkl_malloc(Q * R * (32 / 8), 4096);
    B_ref = malloc(Q * R * (32 / 8));
    C     = mkl_malloc(P * R * (32 / 8), 4096);
    C_ref = malloc(P * R * (32 / 8));
#ifdef DO_1FILL
    mf_init_constant(&ALPHA, 1, 1, 1);
    mf_init_constant(&BETA,  1, 1, 1);
    mf_init_constant(A, P, Q, 1);
    mf_init_constant(B, Q, R, 1);
    mf_init_constant(C, P, R, 1);
#else /* DO_1FILL */
    ALPHA = 1.0;
    BETA = 0.0;
    mf_srandom();
    mf_init_random(A, P, Q);
    mf_init_random(B, Q, R);
    mf_init_random(C, P, R);
#endif /* DO_1FILL */
    memcpy(A_ref, A, P * Q * (32 / 8));
    memcpy(B_ref, B, Q * R * (32 / 8));
    memcpy(C_ref, C, P * R * (32 / 8));

    fflush(stdout);
    gettimeofday(&start, NULL);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, P, R, Q, ALPHA, A, Q, B, R, BETA, C, R);
    gettimeofday(&end, NULL);
    
    printf("%g\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6);
    // gflops: (2 * P * Q * R + 3 * P * R) / ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6)
    
    free(C_ref);
    free(B_ref);
    free(A_ref);
    mkl_free(C);
    mkl_free(B);
    mkl_free(A);

    return 0;
}


// This function multiplies two randommatrices with equal sizes. Change the rows and columns values to edit their size. 
int main() {
    for (int i = 1; i <= 2000; i++) {
        const unsigned rows = i;
        const unsigned columns = 64;

        run_sgemm(rows, columns);
    }
}
