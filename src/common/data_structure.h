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
 * SparseRow can store the sparse value such as the input training data.        *
 *                                                                              *
 * For SparseRow, the field of data_  stores the real data vector and           *
 * the field of position_ stores their index.                                   *
 *                                                                              *
 * Note that, we can not index any value in this data structure and just        *
 * can scan the whole value.                                                    *
 *                                                                              *
 * scoped_array<real_t> data is equal to real_t* data, the only difference is   *
 * that scoped_array<real_t> will delete the allocated memory automitically.    *
 * -----------------------------------------------------------------------------
 */

struct SparseRow {
  /* The real value vector */

  scoped_array<real_t> x;

  /* The value postion (optional) */

  scoped_array<index_t> position;

  /* The field number. (optional, only for FFM) */
  
  scoped_array<int> field;

  /* y can be -1 (for negative examples), 
     or 1 (for positive examples) */

  int y;

  /* size of current row */

  uint32 size;
};

/* -----------------------------------------------------------------------------
 * DataMatrix is responsble for storing the input data matrix.                  *
 *                                                                              *
 * Each row of the matrix is a SparseRow structure, which stores                *
 * sparse input data.                                                           *
 * -----------------------------------------------------------------------------
 */

typedef std::vector<SparseRow> DataMatrix;


/* -----------------------------------------------------------------------------
 * SparseGrad is response for storing the calculated gradients.                 *
 *                                                                              *
 * Note that, for LR, we only need the grad_w, position_w, and size_w.          *
 * For FFM and FM, we need all the fields in this data structure.               *
 * -----------------------------------------------------------------------------
 */

struct SparseGrad {
 /* The gradient of w */

 scoped_array<real_t> grad_w;

 /* The position of grad_w */

 scoped_array<index_t> postion_w;

 /* The size of grad_w */

 index_t size_w;

 /* The gradient of v */

 scoped_array<real_t> grad_v;

 /* The position of grad_v */

 scoped_array<index_t> position_v;

 /* The size of grad_v */

 index_t size_v;
};

/* -----------------------------------------------------------------------------
 * Model is responsble for storing the global model parameters.                 *
 *                                                                              *
 * Note that, we represent the machine leanring model in a flat way, that is,   *
 * we store all the parameters in a single dense vector.                        *                          
 * -----------------------------------------------------------------------------
 */

enum ModelType { LR, FM, FFM };

class Model {
 public:
  /* Constructor */

  Model(real_t init_value, ModelType type, 
        index_t feature_num, int k, int field_num)
     : m_feature_num(feature_num), 
       m_k(k), 
       m_field_num(field_num) {
    // check the input value
    CHECK_GT(m_feature_num, 0);
    // Note that, for LR, m_k and m_field_num should be set to 0.
    // For FM, just m_field_num should be set to 0.
    CHECK_GE(m_k, 0);          
    CHECK_GE(m_field_num, 0);
    // allocated memory and initial model parameters 
    if (type == LR) {
      m_size_parameters = m_feature_num;
    } else if (type == FM) {
      m_size_parameters = m_feature_num * (1 + m_k);
    } else if (type == FFM) {
      m_size_parameters = m_feature_num * (1 + m_k * m_field_num);
    } else {
      LOG(FATAL) << "Unknow model type: " << type;
    }
    try {
      m_parameters.reset(new real_t[m_size_parameters]);
      Initialize(init_value, m_size_parameters);
    } catch (std::bad_alloc&) {
      LOG(FATAL) << "Cannot not allocate enough memory for   \
                     current model parameters.";
    }
  }

  /* Return a start pointer of w and its size. 
     Used by LR, FM, and FFM. */

  index_t GetW(real_t** pointer) { 
    *pointer = m_parameters.get();
    return m_feature_num;
  }

  /* Return the start pointer of a vector for specified feature,
     and return the size of this vector. Used by FM. */

  int GetV(real_t** pointer, int index) {
    // 0 <= index < m_feature_num
    CHECK_GE(index, 0);
    CHECK_LT(index, m_feature_num);
    *pointer = m_parameters.get() + m_feature_num + m_k * index;
    return m_k;
  }

  /* Return the start pointer of a vector for specified feature 
     and field, and return the size of this vector. Used by FFM. */

  int GetV(real_t** pointer, int index, int field) {
    // 0 <= index < m_feature_num
    CHECK_GE(index, 0);
    CHECK_LT(index, m_feature_num);
    // 0 <= field < m_field_num
    CHECK_GE(field, 0);
    CHECK_LT(field, m_field_num);
    *pointer = m_parameters.get() + m_feature_num + 
               m_k * m_field_num * index + m_k * field;
    return m_k;
  }

 private:
  scoped_array<real_t> m_parameters;   /* To store the model parameters */
  index_t m_feature_num;               /* number of features */ 
  int m_k;                             /* The size of k (for FM and FFM) */
  int m_field_num;                     /* The number of field (only for FFM) */
  index_t m_size_parameters;           /* The size of total parameters */

  /* Initial the model parameters */

  void Initialize(real_t init_val, index_t len) {
    for (index_t i = 0; i < len; ++i) {
      *(m_parameters.get() + i) = init_val;
    }
  }

  DISALLOW_COPY_AND_ASSIGN(Model);
};

} // namespace f2m

#endif // F2M_COMMON_DATA_STRUCTURE_H_