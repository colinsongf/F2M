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

 This file defines the data structures used by f2m.
 */

#ifndef F2M_COMMON_DATA_STRUCTURE_H_
#define F2M_COMMON_DATA_STRUCTURE_H_

#include <vector>

#include "src/common/common.h" 

/* The float point type will be used tos store feature values.
 */
typedef float real_t;

/* The unsigned integer type will be used to store feature index.
 */
typedef size_t index_t;

/* -----------------------------------------------------------------------------
 * DSVector (Dense-Sparse Vector) can store both the dense value and 
 * the sparse value.
 *
 * For dense value, such as the global model parameter in single
 * machine, data_ is the real value vector, position_ is empty.
 *
 * For sparse value, such as the gradients at each iteraton, or the 
 * working set parameters fetched from a parameter server, data_
 * is the value vector and position_ stores the index of each value. 
 *
 * Note that, we use this data strcuture to represent all the models,
 * include Logistic Regresson, FM, as well as FFM. For FM, to parse 
 * model, we also need to know the number of K. For FFM, we need to
 * know the number of K and field number.
 * -----------------------------------------------------------------------------
 */

struct DSVector {
  /* The real value vector
   */
  std::vector<real_t> data_;

  /* The value postion (optional)
   */
  std::vector<index_t> position_;
};

/* To check a vector is sparse or dense.
 */
bool IsASparseVector(const Vector& vec) {
  return (!vec.position_.empty());
}

/* -----------------------------------------------------------------------------
 * DMatrix (data matrix) is responsble for storing the input data.
 *
 * Each row of the DMatrix is a RowData structure.  
 * 
 * -----------------------------------------------------------------------------
 */

 struct RowData {
   /* The input feature values.
    */
   std::vector<real_t> feature_value_;

   /* The position of these features value.
    */
   std::vector<index_t> feature_index_;

   /* The field number. (optional, only for FFM)
    */
   std::vector<int> filed_num_;
 };

 typedef std::vector<RowData> DMatrix;

#endif // F2M_COMMON_DATA_STRUCTURE_H_