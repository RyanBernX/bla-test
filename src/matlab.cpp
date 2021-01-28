/*
 * ==========================================================================
 *
 *       Filename:  matlab.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/28/2020 02:43:46 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Haoyang Liu (@liuhy), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, PKU
 *
 * ==========================================================================
 */

#include <iostream>
#include "core.h"
#include "matlab.h"

namespace bla_test {
    template<typename T>
    void get_matrix_from_file(const char *filename, const char *varname,
                              Eigen::Matrix<T, Dynamic, Dynamic> & mat){
        MATFile *pmat;
        mxArray *pa;
        int info;

        // read from .mat
        info = read_variable_from_mat(filename, varname, &pmat, &pa);
        if (info) return;

        // convert to Eigen dense
        info = mxArray_to_dense(pa, mat);

        if (info){
            std::cerr << "Failed to read variable \'" << varname << "\': type mismatch.\n";
        }

        mxDestroyArray(pa);
        matClose(pmat);
    }

    template<typename T>
    void get_matrix_from_file(const char *filename, const char *varname,
                              Eigen::SparseMatrix<T, ColMajor, SparseIndex> & spmat){
        MATFile *pmat;
        mxArray *pa;
        int info;

        // read from .mat
        info = read_variable_from_mat(filename, varname, &pmat, &pa);
        if (info) return;

        // convert to Eigen sparse
        info = mxArray_to_sparse(pa, spmat);

        if (info){
            std::cerr << "Failed to read variable \'" << varname << "\': type mismatch.\n";
        }

        mxDestroyArray(pa);
        matClose(pmat);
    }


    int read_variable_from_mat(const char *filename, const char *varname,
                               MATFile **pmat, mxArray **pa){
        // open mat file
        *pmat = matOpen(filename, "r");
        if (*pmat == NULL){
            std::cerr << "Failed to open file \'" << filename << "\'.\n";
            return 1;
        }

        // extract specified variable
        *pa = matGetVariable(*pmat, varname);
        if (*pa == NULL){
            std::cerr << "No variable named \'" << varname << "\' found.\n";
            matClose(*pmat);
            return 1;
        }

        return 0;
    }

    template<typename T>
    int mxArray_to_dense(const mxArray *pa,
                          Eigen::Matrix<T, Dynamic, Dynamic> & mat){
        // check type
        bool matched = (std::is_same<T, Scalar>::value && mx_is_scalar(pa)) ||
            (std::is_same<T, ComplexScalar>::value && mx_is_complex_scalar(pa));
        matched = matched && !mxIsSparse(pa);

        if (matched && !mxIsEmpty(pa)){
            mwSize m = mxGetM(pa);
            mwSize n = mxGetN(pa);
            T* data_ptr = (T*)mxGetPr(pa); 

            // resize and copy
            mat.resize(m, n);
            memcpy(mat.data(), data_ptr, m * n * sizeof(T));

        } else {
            return 1;
        }
        return 0;

    }

    template<typename T>
    int mxArray_to_sparse(const mxArray *pa,
                          Eigen::SparseMatrix<T, ColMajor, SparseIndex> & spmat){
        // check type
        bool matched = (std::is_same<T, Scalar>::value && mx_is_scalar(pa)) ||
            (std::is_same<T, ComplexScalar>::value && mx_is_complex_scalar(pa));
        matched = matched && mxIsSparse(pa);

        if (matched && !mxIsEmpty(pa)){
            mwSize m = mxGetM(pa);
            mwSize n = mxGetN(pa);
            mwSize nnz = mxGetNzmax(pa);
            T* data_ptr = (T*)mxGetPr(pa);

            mwIndex *jc = mxGetJc(pa);
            mwIndex *ir = mxGetIr(pa);

            // MATLAB uses unsigned indexing, Eigen uses signed indexing
            // Thus it is safer to use elementwise conversion
            // Note: it is not recommended to operate directly on
            //       the buffers of SpMat. Use Map objects instead
            std::vector<SparseIndex> ir_v;
            std::vector<SparseIndex> jc_v;
            ir_v.reserve(nnz);
            jc_v.reserve(n + 1);

            for (mwSize i = 0; i < n + 1; ++i){
                jc_v.push_back(*jc++);
            }

            for (mwSize i = 0; i< nnz; ++i){
                ir_v.push_back(*ir++);
            }

            spmat = Map<Eigen::SparseMatrix<T>, ColMajor, SparseIndex>(
                    m, n, nnz, jc_v.data(), ir_v.data(), data_ptr);

        } else {
            return 1;
        }
        return 0;
    }

    // instantiate
    template void get_matrix_from_file(const char *, const char *,
            Eigen::Matrix<Scalar, Dynamic, Dynamic>&);
    template void get_matrix_from_file(const char *, const char *,
            Eigen::Matrix<ComplexScalar, Dynamic, Dynamic>&);
    template void get_matrix_from_file(const char *, const char *,
            Eigen::SparseMatrix<Scalar, ColMajor, SparseIndex>&);
    template void get_matrix_from_file(const char *, const char *,
            Eigen::SparseMatrix<ComplexScalar, ColMajor, SparseIndex>&);

}
