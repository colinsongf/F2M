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
// Copyright (c) 2016 by contributors.
// Author: Chao Ma (mctt90@gmail.com)
//
// Define common data structures and data types used in F2M.
//
#ifndef F2M_COMMON_DATA_ML_H_
#define F2M_COMMON_DATA_ML_H_

#include "src/common/common.h"
#include "src/common/sparse_vector.h"
#include "src/common/dense_vector.h"

namespace f2m {

// This define the float point that will be used 
// to store feature value and gradient.
typedef float real_t;

// This defines the unsigned integer type that
// can normally be used to store feature index
typedef uint32 index_t;

// Define sparse value list.
typedef SparseVector<index_t, real_t> SparseList;

// Define dense value list.
typedef DenseVector<real_t> DenseList;

// Define the model used in LR, FM, as well as in FFM.



} // namespace f2m

#endif // F2M_COMMON_DATA_ML_H_
