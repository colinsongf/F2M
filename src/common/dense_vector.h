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

Define the class template DenseVector and the operations 
required by this class.
*/

#ifndef F2M_COMMON_DENSE_VECTOR_H_
#define F2M_COMMON_DENSE_VECTOR_H_

#include "src/common/common.h" // use CArray class

/* -----------------------------------------------------------------------------
 * DenseVector inherit the CArray class, which is designed to encapsulate 
 * C-style vectors with a minimum interface. Compared with std::vector, 
 * CArray ensures that elements are stored successively in memory layout, 
 * and provides an interface to access the elements in C-style.
 * -----------------------------------------------------------------------------
 */

template <typename ValueType>
class DenseVector : public CArray<ValueType> {
 public:
  DenseVector(int size, const ValueType& init)
    : CArray<ValueType>(size, init) {}

  DenseVector(int size)
    : CArray<ValueType>(size) {}
};

/* -----------------------------------------------------------------------------
 * This file also provides a set of mathmatic operations used by a DenseVector.
 * -----------------------------------------------------------------------------
 */

/* Scale(v, c) : v <- v * c 
 */
template<typename ValueType, typename ScaleType>
void Scale(DenseVector<ValueType>* v, const ScaleType& c) {
  // TODO: we can use openmp to speedup 
  // the performance here.
  for (int i = 0; i < v->size(); ++i) {
    ((*v).data())[i] *= c;
  }
}

/* Scale Into(u,v,c) : u <- v * c 
 */
template <typename ValueType, typename ScaleType>
void ScaleInto(DenseVector<ValueType>* u, 
               const DenseVector<ValueType>& v,
               const ScaleType& c) {
  CHECK_EQ(v.size(), u->size());
  CHECK_LT(0, v.size());
  // TODO: we can use openmp to speedup 
  // the performance here.
  for (int i = 0; i < v.size(); ++i) {
    ((*u).data())[i] = v.data()[i] * c;
  }
}

/* AddScaled(u,v,c) : u <- u + v * c
 */
template <class ValueType, class ScaleType>
void AddScaled(DenseVector<ValueType>* u,
               const DenseVector<ValueType>& v,
               const ScaleType& c) {
  CHECK_EQ(v.size(), u->size());
  CHECK_LT(0, v.size());
  // TODO: we can use openmp to speedup 
  // the performance here.
  for (int i = 0; i < v.size(); ++i) {
    ((*u).data())[i] += v.data()[i] * c;
  }
}

/* AddScaledInto(w,u,v,c) : w <- u + v * c
 */
template <class ValueType, class ScaleType>
void AddScaledInto(DenseVector<ValueType>* w,
                   const DenseVector<ValueType>& u,
                   const DenseVector<ValueType>& v,
                   const ScaleType& c) {
  CHECK_EQ(u.size(), v.size());
  CHECK_EQ(u.size(), w->size());
  CHECK_LT(0, u.size());
  // TODO: we can use openmp to speedup 
  // the performance here.
  for (int i = 0; i < u.size(); ++i) {
    ((*w).data())[i] = u.data()[i] + v.data()[i] * c;
  }
}

/* DotProduct(u,v) : r <- dot(u, v)
 */
template <class ValueType>
ValueType DotProduct(const DenseVector<ValueType>& v1,
                     const DenseVector<ValueType>& v2) {
  CHECK_EQ(v1.size(), v2.size());
  ValueType ret = 0;
  // TODO: we can use openmp to speedup 
  // the performance here.
  for (int i = 0; i < v1.size(); ++i) {
    ret += v1.data()[i] * v2.data()[i];
  }
  return ret;
}

/* Output a dense vector in human readable format.
 */
template <class ValueType>
std::ostream& operator<< (std::ostream& output,
                          const DenseVector<ValueType>& vec) {
  output << "[ ";
  for (int i = 0; i < vec.size(); ++i) {
    if (vec.data()[i] != 0) // to keep the format the same with sparse
      output << i << ":" << vec.data()[i] << " ";
    if (i % 100 == 0) output << endl;
  }
  output << "]";
  return output;
}

#endif // F2M_COMMON_DENSE_VECTOR_H_