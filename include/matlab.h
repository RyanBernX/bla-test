/*
 * ==========================================================================
 *
 *       Filename:  matlab.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/28/2020 12:28:56 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Haoyang Liu (@liuhy), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, PKU
 *
 * ==========================================================================
 */

#ifndef MATLAB_H
#define MATLAB_H

#include <cstring>
#include <type_traits>
#include "core.h"
#include "matrix.h"
#include "mat.h"

namespace bla_test {
    inline bool mx_is_scalar(const mxArray *p){
        return mxIsDouble(p) && !mxIsComplex(p);
    }
    inline bool mx_is_complex_scalar(const mxArray *p){
        return mxIsDouble(p) && mxIsComplex(p);
    }
    template<typename T>
    void get_matrix_from_file(const char *, const char *,
                              Eigen::Matrix<T, Dynamic, Dynamic> &);
    template<typename T>
    void get_matrix_from_file(const char *, const char *,
                              Eigen::SparseMatrix<T, ColMajor, SparseIndex> &);
    int read_variable_from_mat(const char *, const char *,
                              MATFile **, mxArray **);
    template<typename T>
    int mxArray_to_dense(const mxArray *, Eigen::Matrix<T, Dynamic, Dynamic> &);

    template<typename T>
    int mxArray_to_sparse(const mxArray *, Eigen::SparseMatrix<T, ColMajor, SparseIndex> &);
}

#endif
