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
 * For SparseRow, the filed of data_  stores the real data vector and           *
 * the filed of position_ stores their index.                                   *
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

  Model(real_t init_value, index_t feature_num, 
        int k, int filed_num, ModelType type)
     : m_feature_num(feature_num), 
       m_k(k), 
       m_filed_num(filed_num),
       m_type(type) {
    // check the value
    CHECK_GT(m_feature_num, 0);
    CHECK_GT(m_k, 0);
    CHECK_GT(m_filed_num, 0);
    // allocated memory space for the model parameters
    try {
      if (m_type == ModelType::LR) {
        // note that we ignore the w0.
        m_parameters = new real_t[m_feature_num];
      } else if (m_type == ModelType::FM) {
        // for FM, the size of model parameters is:
        // m_feature_num + m_feature_num * k
        m_parameters = new real_t[m_feature_num + m_feature_num * m_k];
      } else if (m_type == ModelType::FFM) {
        // For FFM, the size of model paramters is:
        // m_feature_num + m_feature_num * k * filed_num
        m_parameters = new real_t[m_feature_num + 
                                  m_feature_num * m_k * m_filed_num];
      } else {
        LOG(FATAL) << "Unknow model type: " << type;
      }
    } catch (std::bad_alloc&) {
      LOG(FATAL) << "Cannot not allocate enough memory for   \
                     current model parameters."
    }
  }

 private:
  scoped_array<real_t> m_parameters;   /* To store the model parameters */
  ModelType m_type;                    /* Model type: LR, FM, or FFM */
  index_t m_feature_num;               /* number of features */ 
  int m_k;                             /* The size of k (for FM and FFM) */
  int m_filed_num;                     /* The number of filed (only for FFM) */

  DISALLOW_COPY_AND_ASSIGN(Model);
};

} // namespace f2m

#endif // F2M_COMMON_DATA_STRUCTURE_H_