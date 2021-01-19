#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include "mkl.h"

#define ALIGN 32

int main(int argc, char **argv){
    long N, NN;
    double *A, *B, *C, *b, *c, t;
    int iseed[] = {233, 233, 233, 1};

    if (argc != 2){
        fprintf(stderr, "Usage: ./a.out <dim>\n");
        return 1;
    }

    N = strtol(argv[1], NULL, 10);
    NN = N * N;

#ifdef USE_MKL_MALLOC
    A = (double*)mkl_malloc(NN * sizeof(double), ALIGN);
    B = (double*)mkl_malloc(NN * sizeof(double), ALIGN);
    C = (double*)mkl_malloc(NN * sizeof(double), ALIGN);
    b = (double*)mkl_malloc(N * sizeof(double), ALIGN);
    c = (double*)mkl_malloc(N * sizeof(double), ALIGN);
#else
    A = (double*)malloc(NN * sizeof(double));
    B = (double*)malloc(NN * sizeof(double));
    C = (double*)malloc(NN * sizeof(double));
    b = (double*)malloc(N * sizeof(double));
    c = (double*)malloc(N * sizeof(double));
#endif

    // set random vector / matrices
    LAPACKE_dlarnv(1, iseed, NN, A);
    LAPACKE_dlarnv(1, iseed, NN, B);
    LAPACKE_dlarnv(1, iseed, N, b);

    printf("n = %ld\n", N);

    // gemv
    printf("Testing gemv A * b (50 times): ");
    t = omp_get_wtime();
    for (int i = 0; i < 50; ++i){
        cblas_dgemv(CblasColMajor, CblasNoTrans, N, N, 1.0, A, N, b, 1, 0.0, c, 1);
    }
    printf("%f sec.\n", omp_get_wtime() - t);

    printf("Testing gemv A' * b (50 times): ");
    t = omp_get_wtime();
    for (int i = 0; i < 50; ++i){
        cblas_dgemv(CblasColMajor, CblasTrans, N, N, 1.0, A, N, b, 1, 0.0, c, 1);
    }
    printf("%f sec.\n", omp_get_wtime() - t);

    // gemm
    printf("Testing gemm A * B (5 times): ");
    t = omp_get_wtime();
    for (int i = 0; i < 5; ++i){
        cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, \
                A, N, B, N, 0.0, C, N);
    }
    printf("%f sec.\n", omp_get_wtime() - t);

    printf("Testing gemm A' * B (5 times): ");
    t = omp_get_wtime();
    for (int i = 0; i < 5; ++i){
        cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans, N, N, N, 1.0, \
                A, N, B, N, 0.0, C, N);
    }
    printf("%f sec.\n", omp_get_wtime() - t);


#ifdef USE_MKL_MALLOC
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);
    mkl_free(b);
    mkl_free(c);
#else
    free(A);
    free(B);
    free(C);
    free(b);
    free(c);
#endif
    return 0;
}
