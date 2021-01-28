/*
 * ===========================================================================
 *
 *       Filename:  spblas_test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/27/2021 03:26:36 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Haoyang Liu (), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, Peking University
 *      Copyright:  Copyright (c) 2021, Haoyang Liu
 *
 * ===========================================================================
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include "core.h"
#include "tictoc.h"
#include "matlab.h"
#include "mkl.h"

constexpr int nspmv = 50;
constexpr int nspmm = 3;

using namespace bla_test;

int main(int argc, char **argv){
    SpMat A;

    if (argc != 2){
        std::cerr << "Usage: spbla_test <filename>" << std::endl;
        return -1;
    }

    get_matrix_from_file(argv[1], "spA", A);

    if (A.rows() == 0){
        std::cerr << "Failed to read file." << std::endl;
        return -1;
    }

    // set SDL parameters
    std::cout << "Select MKL interface: GNU LP64 + GNU threading" << std::endl;
    mkl_set_interface_layer(MKL_INTERFACE_LP64+MKL_INTERFACE_GNU);
    mkl_set_threading_layer(MKL_THREADING_GNU);

    Vec b = Mat::Random(A.cols(), 1);
    Vec c = Mat::Random(A.rows(), 1);
    Vec y = Mat::Zero(A.rows(), 1);
    Vec y1 = Mat::Zero(A.rows(), 1);
    Vec z = Mat::Zero(A.cols(), 1);
    Vec z1 = Mat::Zero(A.cols(), 1);

    Index nB = std::max(2L, A.cols() / 100), nC = std::max(2L, A.rows() / 100);
    Mat B = Mat::Random(A.cols(), nB);
    Mat C = Mat::Random(A.rows(), nC);

    Mat Y = Mat::Zero(A.rows(), nB);
    Mat Y1 = Mat::Zero(A.rows(), nB);
    Mat Z = Mat::Zero(A.cols(), nC);
    Mat Z1 = Mat::Zero(A.cols(), nC);

    // show information
    std::cout << "Read sparse matrix A (" << A.rows() << " x " << A.cols() \
              << ") with " << A.nonZeros() << " elements." << std::endl;
    std::cout << "Creating dense vector b (" << A.cols() << " x " << 1 << ")" << std::endl;
    std::cout << "         dense matrix B (" << B.rows() << " x " << nB << ")" << std::endl;
    std::cout << "         dense matrix C (" << C.rows() << " x " << nC << ")" << std::endl;

    // native SpMV
    auto t = tic();
    std::cout << "Testing native SpMV A * b (" << nspmv <<" times): ";
    for (int i = 0; i < nspmv; ++i)
        y = A * b;
    std::cout << toc(t) << " sec." << std::endl;

    t = tic();
    std::cout << "Testing native SpMV A' * b (" << nspmv << " times): ";
    for (int i = 0; i < nspmv; ++i)
        z = A.transpose() * c;
    std::cout << toc(t) << " sec." << std::endl;

    // native SpMM
    t = tic();
    std::cout << "Testing native SpMM A * B (" << nspmm << " times): ";
    for (int i = 0; i < nspmm; ++i)
        Y = A * B;
    std::cout << toc(t) << " sec." << std::endl;

    t = tic();
    std::cout << "Testing native SpMM A' * B (" << nspmm << " times): ";
    for (int i = 0; i < nspmm; ++i)
        Z = A.transpose() * C;
    std::cout << toc(t) << " sec." << std::endl;


    // MKL SpMV
    std::cout << "Switch to Intel MKL ..." << std::endl;
    std::cout << "Creating MKL sparse matrix ...";
    t = tic();
    sparse_matrix_t A_mkl;
    mkl_sparse_d_create_csr(&A_mkl, SPARSE_INDEX_BASE_ZERO, A.rows(), A.cols(), \
            A.outerIndexPtr(), A.outerIndexPtr() + 1, A.innerIndexPtr(), A.valuePtr());
    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    descr.diag = SPARSE_DIAG_NON_UNIT;
    std::cout << toc(t) << " sec." << std::endl;


    t = tic();
    std::cout << "Testing MKL SpMV A * b (" << nspmv << " times): ";
    for (int i = 0; i < nspmv; ++i)
        mkl_sparse_d_mv(SPARSE_OPERATION_TRANSPOSE, 1.0, A_mkl, descr,
                b.data(), 0.0, y1.data());
    std::cout << toc(t) << " sec." << std::endl;
    std::cout << "Error: " << (y - y1).norm() << std::endl;

    t = tic();
    std::cout << "Testing MKL SpMV A' * b (" << nspmv << " times): ";
    for (int i = 0; i < nspmv; ++i)
        mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A_mkl, descr,
                c.data(), 0.0, z1.data());
    std::cout << toc(t) << " sec." << std::endl;
    std::cout << "Error: " << (z - z1).norm() << std::endl;

    // MKL SpMM
    t = tic();
    std::cout << "Testing MKL SpMM A * B (" << nspmm << " times): ";
    for (int i = 0; i < nspmm; ++i)
        mkl_sparse_d_mm(SPARSE_OPERATION_TRANSPOSE, 1.0, A_mkl, descr,
                SPARSE_LAYOUT_COLUMN_MAJOR, B.data(), nB, B.rows(), 0.0, Y1.data(), Y1.rows());
    std::cout << toc(t) << " sec." << std::endl;
    std::cout << "Error: " << (Y - Y1).norm() << std::endl;

    t = tic();
    std::cout << "Testing MKL SpMM A' * B (" << nspmm << " times): ";
    for (int i = 0; i < nspmm; ++i)
        mkl_sparse_d_mm(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A_mkl, descr,
                SPARSE_LAYOUT_COLUMN_MAJOR, C.data(), nC, C.rows(), 0.0, Z1.data(), Z1.rows());
    std::cout << toc(t) << " sec." << std::endl;
    std::cout << "Error: " << (Z - Z1).norm() << std::endl;

    mkl_sparse_destroy(A_mkl);
    
    return 0;
}
