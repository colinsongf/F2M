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

namespace f2m {

/* The float point type will be used to 
   store feature values. */

typedef float real_t;

/* The unsigned integer (32 bit) type will be 
   used to store feature index. */

typedef uint32 index_t;

/* -----------------------------------------------------------------------------
 * DSVector (Dense-Sparse Vector) can store both the dense value and            *
 * the sparse value.                                                            *
 *                                                                              *
 * For dense value, such as the global model parameter on a single              *
 * machine, data_ is the real value vector, position_ is empty.                 *
 *                                                                              *
 * For sparse value, such as the input trainning data, data_                    *         
 * is the sparse value vector and position_ stores the index of each value.     *                                                                             
 *                                                                              *
 * Note that, we use this data strcuture to represent all of the models,        *
 * including Logistic Regresson, FM, as well as FFM.  In other words, we        *
 * represent these models in a flat way.                                        *
 *                                                                              *
 * For FM, to parse the model, we also need to know the number of K.            *
 * For FFM, in addition to the K, we also need to know the field number.        *
 * All of this information is stored in the Loss class, and is setted by        *
 * users' input arguments.                                                      *
 * -----------------------------------------------------------------------------
 */

struct DSVector {
  /* The real value vector */

  std::vector<real_t> data_;

  /* The value postion (optional) */

  std::vector<index_t> position_;
};

/* To check a vector is sparse or dense. */

bool IsASparseVector(const DSVector& vec) {
  return (!vec.position_.empty());
}

/* -----------------------------------------------------------------------------
 * DMatrix (data matrix) is responsble for storing the input data set.          *      
 * (in one mini-batch).                                                         *
 *                                                                              *
 * Each row of the DMatrix is a RowData structure, which stores the sparse      *
 * data of each input line.                                                     *
 * -----------------------------------------------------------------------------
 */

struct RowData {
  /* The input feature values. 
     The last element is y (-1 or 1). */

  std::vector<real_t> feature_value_;

  /* The positions of these features value. */

  std::vector<index_t> feature_index_;

  /* The field number. (optional, only for FFM) */

  std::vector<int> field_;
};

typedef std::vector<RowData> DMatrix;

} // namespace f2m

#endif // F2M_COMMON_DATA_STRUCTURE_H_