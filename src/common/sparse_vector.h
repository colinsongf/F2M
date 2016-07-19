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
// Define the class template SparseVector and operations required
// by this class.
//
#ifndef F2M_COMMON_SPARSE_VECTOR_H_
#define F2M_COMMON_SPARSE_VECTOR_H_

#include <map>

namespace f2m {

using std::map;
using std::pair;
using std::ostream;

// We use std::map as the container of a sparse vector. Because
// std::map implements a sorting tree (RB-tree), so its iterator
// acesses elements in known order of keys. This is important for 
// sparse vector operations like dot-product and add-mutil-into.
// The ValueType must be a numeric type supporting const 0.
template <class KeyType, class ValueType>
class SparseVector : public map<KeyType, ValueType> {
 public:
  typedef typename map<KeyType, ValueType>::const_iterator const_iterator;
  typedef typename map<KeyType, ValueType>::iterator iterator;
  
  // We constrain operator[] a read-only operation to prevent
  // accidential insert of elements.
  const ValueType& operator[](const KeyType& key) const {
    const_iterator iter = this->find(key);
    if (iter == this->end()) {
      return zero_; 
    }
    return iter->second;
  }

// Set a value at given key. If value == 0, an existing key-value 
// pair is removed. If value != 0, the value is set or inserted.
// This function also serves as a convenient form of insert(), no
// need to use std::pair.
void set(const KeyType& key, const ValueType& value) {
  iterator iter = this->find(key);
  if (iter != this->end()) {
    if (IsZero(value)) {
      this->erase(iter);
    } else {
      iter->second = value;
    }
  } else {
    if (!IsZero(value)) {
      this->insert(pair<KeyType, ValueType>(key, value));
    }
  }
}

bool has(const KeyType& key) const {
  return this->find(key) != this->end();
}

 protected:
  static const ValueType zero_;

  static bool IsZero(const ValueType& value) {
    return value == 0;    
  }
};

template <class KeyType, class ValueType>
const ValueType SparseVector<KeyType, ValueType>::zero_(0);



} // namespace f2m

#endif // F2M_COMMON_SPARSE_VECTOR_H_
