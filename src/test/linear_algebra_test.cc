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

Unit Test for linear algebra operations (linear_algebra.h)
*/

#include "gtest/gtest.h"

#include "src/common/data_structure.h"
#include "src/common/linear_algebra.h"
#include "src/common/common.h"

namespace f2m {

const int dense_vector_size = 100;
const int sparse_vector_size = 10;
const int matrix_row_size = 10;

TEST(LinearAlgebraTest, SparseVectorDenseVectorTimes) {
  real_t* sparse_vector = new real_t[sparse_vector_size];
  real_t* dense_vector = new real_t[dense_vector_size];
  index_t* position = new index_t[sparse_vector_size];

  for (int i = 0; i < sparse_vector_size; ++i) {
  	sparse_vector[i] = 2.0;
  	position[i] = i;
  }

  for (int i = 0; i < dense_vector_size; ++i) {
  	dense_vector[i] = (real_t)i;
  }

  real_t result = 0;
  SparseVectorDenseVectorTimes(sparse_vector,
  	                           position,
  	                           sparse_vector_size,
  	                           dense_vector,
  	                           &result);
  EXPECT_EQ(result, 90.0);
}

TEST(LinearAlgebraTest, SparseMatrixDenseVectorTimes) {
  DataMatrix matrix(matrix_row_size);
  for (int i = 0; i < matrix_row_size; ++i) {
    matrix[i].x.reset(new real_t[sparse_vector_size]);
    matrix[i].position.reset(new index_t[sparse_vector_size]);
    for (int n = 0; n < sparse_vector_size; ++n) {
      *(matrix[i].x.get() + n) = 2.0;
      *(matrix[i].position.get() + n) = n;
    }
    matrix[i].size = sparse_vector_size;
  }

  real_t* dense_vector = new real_t[dense_vector_size];
  for (int i = 0; i < dense_vector_size; ++i) {
  	dense_vector[i] = (real_t)i;
  }

  std::vector<real_t> result(matrix_row_size);
  SparseMatrixDenseVectorTimes(matrix,
  	                           dense_vector,
  	                           &result);

  for (int i = 0; i < matrix_row_size; ++i) {
  	EXPECT_EQ(result[i], 90.0);
  }
}

} // namespace f2m