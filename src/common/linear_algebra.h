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

This file defines the common linear algebra 
operations used in f2m.
*/

#ifndef F2M_COMMON_LINEAR_ALGERBA_H_
#define F2M_COMMON_LINEAR_ALGERBA_H_

#include "src/common/common.h"

namespace f2m {

/* -----------------------------------------------------------------------------
 * SparseVectorDenseVectorTimes, here is an example:                            *
 *                                                                              *
 *                   1                                                          *
 *    0 0 1 1 1  *   2  =   9                                                   *
 *                   1                                                          *
 *                   4                                                          *
 *                   4                                                          *
 *                                                                              *
 * -----------------------------------------------------------------------------
 */

void SparseVectorDenseVectorTimes(const real_t* sparse_vector,
	                              const index_t* position,
	                              const int vec_size,
	                              const real_t* dense_vector,
	                              real_t* result) {
  *result = 0;
  for (int i = 0; i < vec_size; ++i) {
  	index_t idx = position[i];
    *result += sparse_vector[i] * dense_vector[idx];
  }
}

/* -----------------------------------------------------------------------------
 * SparseMatrixDenseVectorTimes, here is an example:                            *
 *                                                                              *
 *    1 2 0 0 0       1       5                                                 *
 *    0 0 1 1 1  *    2  =    11                                                *
 *    1 0 3 0 2       3       18                                                *
 *                    4                                                         *
 *                    4                                                         *
 *                                                                              *
 * Note that the result vector should be pre-allocated with                     *
 * the same row size of matrix.                                                 * 
 * -----------------------------------------------------------------------------
 */

void SparseMatrixDenseVectorTimes(const DataMatrix& matrix, 
	                              const real_t* dense_vector,
	                              std::vector<real_t>* result) {
  CHECK_EQ(matrix.size(), result->size());
  for (int row = 0; row < matrix.size(); ++row) {
  	real_t res = 0;
  	SparseVectorDenseVectorTimes(matrix[row].x.get(), 
  		                         matrix[row].position.get(),
  		                         matrix[row].size,
  		                         dense_vector,
  		                         &res);
  	(*result)[row] = res;
  }
}

} // namespace f2m

#endif // F2M_COMMON_LINEAR_ALGEBRA_H_