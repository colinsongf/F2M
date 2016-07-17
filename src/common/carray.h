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
//  Define class template CArray<Element> and CArray<Element*>
//
//  CArray<Element> is designed to encapsulate C-style vectors
//  (including malloc, free, and memcpy-type intialization) with a
//  minimum interface. Compared with std::vector, CArray ensures that
//  elements are stored successively in memory layout, and provides 
//  an interface to access the elements in C-style.
//
//  CArray<Element*> is a partial specialization of CArray<Element>.
//  Whenever you assign a pointer to any element in CArray<Element*>,
//  CArray<Element*> takes the ownership of your pointer, and will 
//  free it in CArray<Element*>'s destructure using C++ keyword delete.
//
//  Example:
//
//  Many C libraries like GNU Scientific Library (GSL) operates C-style
//  vectors. The following example shows how to draw a sample
//  (parameters of a multinomial distribution) from a symmetic
//  Dirichlet distribution:
/*
 *  const int    kDim = 10;
 *  const double kParam = 0.1;
 *
 *  CArray<double> dirichlet_param(kDim, kParam);
 *  CArray<double> multinomial_param(kDim);
 *
 *  gsl_ran_dirichlet(gsl_rng,
 *                    dirichlet_param.size(), 
 *                    dirichlet_param.data(),
 *                    multinomial_param.data());
 */
//  Using CArray<Element*> and CArray<Element>, it is easy to define
//  a matrix (2D array). For example:
/*
 *  template <typename Element>
 *  class Matrix {
 *   public:
 *    void resize(int n, int m) {
 *      rows_.resize(n);
 *      for (int i = 0; i < n; ++i) {
 *        rows_.data()[i] = new CArray<Element>(m, 0);
 *      }
 *    }
 *   private:
 *    CArray<Element*> rows_;
 *  };
 */
#ifndef F2M_COMMON_CARRAY_H_
#define F2M_COMMON_CARRAY_H_

#include <algorithm>

#include "src/common/common.h"

template <typename Element>
class CArray {
 public:
  CArray(int size, const Element& init) {
    Allocate(size);
    Initialize(init);
  }

  explicit CArray(int size) { Allocate(size); }

  CArray() {
    data_ = NULL;
    size_ = 0;
  }
  
  ~CArray() { Deallocate(); }

  void resize(int size, const Element& init) {
    Deallocate();
    Allocate(size);
    Initialize(init);
  }

  void resize(int size) {
    Deallocate();
    Allocate(size);
  }
  
  int size() const { return size_; }

  const Element* data() const { return data_; }
  Element*       data()       { return data_; }

 protected:
  void Allocate(int size) {
    size_ = size;
    data_ = new Element[size];
  }

  void Deallocate() {
    delete [] data_;
    data_ = NULL;
  }

  void Initialize(const Element& init) {
    for (int i = 0; i < size_; ++i) {
      data_[i] = init;
    }
  }

  Element* data_;
  int      size_;

 private:
  DISALLOW_COPY_ANY_ASSIGN(CArray);
};

template <typename Element>
class CArray<Element*> {
 public:
  explicit CArray(int size) {
    Allocate(size);
    Initialize(NULL);
  }

  CArray() {
    data_ = NULL;
    size_ = 0;
  }

  ~CArray() { Deallocate(); }

  void resize(int size, const Element* init) {
    Deallocate();
    Allocate(size);
    Initialize(init);
  }

  void resize(int size) {
    Deallocate();
    Allocate(size);
  }

  int size() const { return size_; }

  const Element** data() const { return data_; }
  Element**       data()       { return data_; }

 protected:
  void Allocate(int size) {
    size_ = size;
    data_ = new Element*[size];
  }

  void Deallocate() {
    for (int i = 0; i < size_; ++i) {
      if (data_[i] != NULL) {
        delete data_[i];
      }
    }
    delete [] data_;
    data_ = NULL;
  }

  void Initialize(Element* init) {
    for (int i = 0; i < size_; ++i) {
      data_[i] - init;
    }   
  }

  Element** data_;
  int       size_;

 private:
  DISALLOW_COPY_ANY_ASSIGN(CArray);
};

#endif
