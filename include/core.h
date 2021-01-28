#ifndef CORE_H
#define CORE_H

#ifdef USE_MKL
#include <mkl.h>
#define EIGEN_USE_MKL_ALL
#endif

#ifdef USE_BLAS_LAPACKE
#include <cblas.h>
#define EIGEN_USE_BLAS
#define EIGEN_USE_LAPACKE
#endif

#include <complex>
#include <Eigen/Eigen>

namespace bla_test {
    using Scalar = double;
    using ComplexScalar = std::complex<Scalar>;
    using Index = ptrdiff_t;
    using SparseIndex = int;

    using Eigen::Dynamic;
    using Eigen::ColMajor;
    using Eigen::RowMajor;
    using Eigen::Map;
    using Mat = Eigen::Matrix<Scalar, Dynamic, Dynamic>;
    using RMat = Eigen::Matrix<Scalar, Dynamic, Dynamic, RowMajor>;
    using Vec = Eigen::Matrix<Scalar, Dynamic, 1>;
    using SpMat = Eigen::SparseMatrix<Scalar, ColMajor, SparseIndex>;
}

#endif
