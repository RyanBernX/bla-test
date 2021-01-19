#include <iostream>
#include <cstdlib>
#include "core.h"
#include "tictoc.h"

using namespace bla_test;

int main(int argc, char **argv){
    Index n = 100;

    if (argc > 1)
        n = std::strtol(argv[1], NULL, 10);
    
    // generating matrix
    std::cout << "Creating matrix A (" << n << " x " << n << ")" << std::endl;
    std::cout << "         matrix B (" << n << " x " << n << ")" << std::endl;
    std::cout << "         vector b (" << n << " x " << 1 << ")" << std::endl;
    Mat A = Mat::Random(n, n);
    Mat B = Mat::Random(n, n);
    Vec b = Mat::Random(n, 1);
    Mat C = Mat::Zero(n, n);
    Vec c = Mat::Zero(n, 1);

    // gemv
    std::cout << "Testing gemv A * b (50 times): ";
    auto t = tic();
    for (int i = 0; i < 50; ++i){
#ifdef USE_BLAS_DIRECT
        cblas_dgemv(CblasColMajor, CblasNoTrans, n, n, 1.0, A.data(), n, b.data(), 1, 0.0, c.data(), 1);
#else
        c = A * b;
#endif
    }
    std::cout << toc(t) << " sec." << std::endl;

    std::cout << "Testing gemv A' * b (50 times): ";
    t = tic();
    for (int i = 0; i < 50; ++i){
#ifdef USE_BLAS_DIRECT
        cblas_dgemv(CblasColMajor, CblasTrans, n, n, 1.0, A.data(), n, b.data(), 1, 0.0, c.data(), 1);
#else
        c = A.transpose() * b;
#endif
    }
    std::cout << toc(t) << " sec." << std::endl;

    // gemm
    std::cout << "Testing gemm A * B (5 times): ";
    t = tic();
    for (int i = 0; i < 5; ++i){
#ifdef USE_BLAS_DIRECT
        cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, \
                A.data(), n, B.data(), n, 0.0, C.data(), n);
#else
        C.noalias() = A * B;
#endif
    }
    std::cout << toc(t) << " sec." << std::endl;

    std::cout << "Testing gemm A' * B (5 times): ";
    t = tic();
    for (int i = 0; i < 5; ++i){
#ifdef USE_BLAS_DIRECT
        cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans, n, n, n, 1.0, \
                A.data(), n, B.data(), n, 0.0, C.data(), n);
#else
        C.noalias() = A.transpose() * B;
#endif
    }
    std::cout << toc(t) << " sec." << std::endl;


    return 0;
}
