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

This file defines a set of linear algebra operations.
*/

#ifndef F2M_COMMON_LINEAR_ALGEBRA_H_
#define F2M_COMMON_LINEAR_ALGEBRA_H_

#include "src/common/common.h"
#include "src/common/data_structure.h"

/* -----------------------------------------------------------------------------
 * Operation: SMDVTimes(Matrix, Vector_Input, Vector_Output)
 * Sparse Matrix Dense Vector multiplication. Given a sparse matrix and a dense 
 * vector, return a new dense vector. For example:
 *
 *   1 0 0 1      1       5
 *   2 0 2 0      2       8
 *   0 3 0 3  x   3   =   18
 *   4 0 0 0      4       5
 *   0 0 0 5              20
 *
 * -----------------------------------------------------------------------------
 */

/* Note that the vector_out shoule be pre-allocated with the 
 * same size of data_matrix.
 */
void SMDVTimes(const DMatrix& data_matrix, 
	           const DSVector& vector_in,
	           DSVector* vector_out) {
  CHECK_EQ(data_matrix.size(), vector_out->data.size());
  // for every row in a data_matrix
  for (int i = 0; i < data_matrix.size(); ++i) {
  	real_t value = 0.0;
    // for every element in a RowData
    for (int j = 0; j < data_matrix[i].feature_index_.size(); ++j) {
      index_t index = data_matrix[i].feature_index_[j];
      value += data_matrix[i].feature_value_[j] 
               * vector_in.data[index];
    }
    vector_out->data_[i] = value;
  }
}

#endif