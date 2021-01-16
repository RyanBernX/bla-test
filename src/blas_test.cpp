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
    Mat C;
    Vec c;

    // gemv
    std::cout << "Testing gemv A * b (50 times): ";
    auto t = tic();
    for (int i = 0; i < 50; ++i){
        c = A * b;
    }
    std::cout << toc(t) << " sec." << std::endl;

    std::cout << "Testing gemv A' * b (50 times): ";
    t = tic();
    for (int i = 0; i < 50; ++i){
        c = A.transpose() * b;
    }
    std::cout << toc(t) << " sec." << std::endl;

    // gemm
    std::cout << "Testing gemm A * B (5 times): ";
    t = tic();
    for (int i = 0; i < 5; ++i){
        C = A * B;
    }
    std::cout << toc(t) << " sec." << std::endl;

    std::cout << "Testing gemm A' * B (5 times): ";
    t = tic();
    for (int i = 0; i < 5; ++i){
        C = A.transpose() * B;
    }
    std::cout << toc(t) << " sec." << std::endl;


    return 0;
}
