/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.
*/

/*
Copyright (c) 2016 by contributors.
Author: Chao Ma (mctt90@gmail.com)

Unit Test for linear_algebra.h (reader.h and reader.cc)
*/

#include "gtest/gtest.h"

#include "src/common/common.h"
#include "src/common/data_structure.h"
#include "src/common/linear_algebra.h"

typedef DMatrix Matrix;
typedef DSVector Vector;

/* -----------------------------------------------------------------------------
 *   1 0 0 1      1       5
 *   2 0 2 0      2       8
 *   0 3 0 3  x   3   =   18
 *   4 0 0 0      4       5
 *   0 0 0 5              20
 * -----------------------------------------------------------------------------
 */

real_t  row_value_0[2] = {1.0, 1.0};
index_t row_index_0[2] = {0, 3};
real_t  row_value_1[2] = {2.0, 2.0};
index_t row_index_1[2] = {0, 2};
real_t  row_value_2[2] = {3.0, 3.0};
index_t row_index_2[2] = {1, 3};
real_t  row_value_3[1] = {4.0};
index_t row_index_3[1] = {0};
real_t  row_value_4[1] = {5.0};
index_t row_index_4[1] = {3};

real_t model[4] = {1.0, 2.0, 3.0, 4.0};

TEST(LinearAlgebraTest, SparseMatrixDenseVectorTimes) {
  // Initialize a sparse matrix
  Matrix matrix(5);
  matrix[0].feature_value_.resize(2, row_value_0);
  matrix[0].feature_index_.resize(2, row_index_0);
  matrix[1].feature_value_.resize(2, row_value_1);
  matrix[1].feature_index_.resize(2, row_index_1);
  matrix[2].feature_value_.resize(2, row_value_2);
  matrix[2].feature_index_.resize(2, row_index_2);
  matrix[3].feature_value_.resize(2, row_value_3);
  matrix[3].feature_index_.resize(2, row_index_3);
  matrix[4].feature_value_.resize(2, row_value_4);
  matrix[4].feature_index_.resize(2, row_index_4);
  // Initialize a dense vector
  Vector vec_in;
  vec_in.data_.resize(4, model);


  // Times
}