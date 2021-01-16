#ifndef CORE_H
#define CORE_H

#ifdef USE_MKL
#define EIGEN_USE_MKL_ALL
#endif

#ifdef USE_BLAS_LAPACKE
#define EIGEN_USE_BLAS
#define EIGEN_USE_LAPACKE
#endif

#include <Eigen/Eigen>

namespace bla_test {
    using Scalar = double;
    using Index = ptrdiff_t;

    using Eigen::Dynamic;
    using Mat = Eigen::Matrix<Scalar, Dynamic, Dynamic>;
    using Vec = Eigen::Matrix<Scalar, Dynamic, 1>;
}

#endif
