/*
 * ==========================================================================
 *
 *       Filename:  readmat.cpp
 *
 *    Description: 
 *
 *        Version:  1.0
 *        Created:  11/28/2020 01:43:11 PM
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

using namespace bla_test;

int main(int argc, char **argv){
    Mat A;
    SpMat spA;
    get_matrix_from_file("X.mat", "X", A);
    get_matrix_from_file("X.mat", "spX", spA);

    std::cout << A << std::endl;
    std::cout << spA << std::endl;
}
