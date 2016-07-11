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
// Provide scoped_ptr and scoped_array.
// 
// Copied and modified from Google's protobuf library.
// google/protobuf/stub/common.h

#ifndef F2M_COMMON_SCOPED_PTR_H_
#define F2M_COMMON_SCOPED_PTR_H_

#include <assert.h>

// =============================================================================
//
// This is an implementation deisgned to match anticipated future TR2
// implementation of the scoped_ptr class, and its closely-related brethren,
// scoped_array, scoped_ptr_malloc, and make_scoped_ptr.
//
// =============================================================================

template <class C> class scoped_ptr;
template <class C> class scoped_array;

// A scoped_ptr<T> is like a T*, except that the destructor of scoped_ptr<T>
// automatically deletes the pointer it holds (if any).
// That is, scoped_ptr<T> owns the T object that it points to.
// Like a T*, a scoped_ptr<T> may hold any either NULL or a pointer to a T object.
//
// The size of a scoped_ptr is small:
// sizeof(scoped_ptr<C>) == size_of(C*)
template <class C>
class scoped_ptr {
 public:
  // The element type
  typedef C element_type;
 
  // Constructor. Defaults to initializing with NULL.
  // There is no way to create an unintialized scoped_ptr
  // The input parameter must be allocated with new.
  explicit scoped_ptr(C* p = NULL) : ptr_(p) {}

  // Destructor. If there is a C object, delete it.
  // We don't need to test ptr_ == NULL because C++ does that for us.
  // By contrast, the free() function in C does not do that.
  ~scoped_ptr() {
    enum { type_must_be_complete = sizeof(C) };
    delete ptr_;
  }

  // Reset. Deletes the current owned object, if any.
  // Then takes ownership of a new object , if given.
  // this->reset(this->get()) works.
  void reset(C* p = NULL) {
    if (p != NULL) {
      enum { type_must_be_complete = sizeof(C) };
      delete ptr_;
      ptr_  = p;
    }
  }

  // Accessors to get the owned object.
  // operator* and operator-> will assert() if there is no current object.
  C& operator*() const {
    assert(ptr_ != NULL);
    return *ptr_;
  }

  C* operator->() const {
    assert(ptr_ != NULL);
    return ptr_;
  }

  C* get() const { return ptr_; }

  // Comparision operators.
  // These return whether two scoped_ptr refer to the same object, not just to
  // two different but equal objects.
  bool operator==(C* p) const { return ptr_ == p; }
  bool operator!=(C* p) const { return ptr_ != p; }

  // Swap two scoped pointers.
  void swap(scoped_ptr& p2) {
    C* tmp = ptr_;
    ptr_ = p2.ptr_;
    p2.ptr_  = tmp;
  }

// Release a pointer.
// The return value is the current pointer held by this object.
// If this object holds a NULL pointer, the return value is NULL.
// After this operation, this object will hold a NULL pointer
// and will not own the object any more.
C* release() {
  C* retVal = ptr_;
  ptr_ = NULL;
  return retVal;
}

private:
  C* ptr_;

  // Forbid comparision of scoped_ptr types. If C2 != C, it totally dosen't
  // make sense, and if C2 == C, it still doesn't make sense becasue you should 
  // never have the same object owned by two different scoped_ptrs.
  template <class C2> bool operator==(scoped_ptr<C2> const& p2) const;
  template <class C2> bool operator!=(scoped_ptr<C2> const& p2) const;

  // Disallow evil construtors
  scoped_ptr(const scoped_ptr&);
  void operator=(const scoped_ptr&);
};

// scoped_array<C> is like scoped_ptr<C>, except that the caller must allocate 
// with new [] and the destructor must delete objects with delete [].
//
// As with scoped_ptr<C>, a scoped_array<C> either points to an object 
// or is NULL. A scoped_array<C> owns the object that it points to.
//
// Size: sizeof(scoped_array<C>) == sizeof(C*) (8 bytes on a 64 bits machine)
template <class C>
class scoped_array {
 public:
  // The element type
  typedef C element_type;

  // Constructor. Defaults to intializing with NULL.
  // There is no way to create an unintialized scoped_array.
  // The input parameter must be allocated with new [].
  explicit scoped_array(C* p = NULL) : array_(p) {}

  // Destructor. If there is a C object, delete it.
  // We do not need to test ptr_ == NULL because C++ does that for us.
  ~scoped_array() {
    enum { type_must_be_complete = sizeof(C) };
    delete [] array_;
  }

  // Reset. Deletes the cuurent owned object, if any. 
  // Then takes ownership of a new object, if given.
  // this->reset(this->get()) works.
  void reset(C* p = NULL) {
    if (p != array_) {
      enum { type_must_be_complete = sizeof(C) };
      delete[] array_;
      array_ = p;
    }   
  }

  // Get one element of the current object.
  // Will assert() if there is no current object, or index i is negative.
  // Note that unlike the size_t, ptrdiff_t can be a negative value.
  C& operator[] (std::ptrdiff_t i) const {
    assert(i >= 0);
    assert(array_ != NULL);
    return array_[i];
  }

  // Get a pointer to the first element of the current object.
  // If there is no current object, return NULL.
  C* get() const {
    return array_;   
  }

  // Release a pointer.
  // The return value is the current pointer held by this object.
  // If this object holds a NULL pointer, the return value is NULL.
  // After this operation, this object will hold a NULL pointer
  // and will not own the object any more.
  C* release() {
    C* retVal = array_;
    array_ = NULL;
    return retVal;
  } 
   
 private:
   C* array_;

 // Forbid comparision of different scoped_array types.
 template <class C2> bool operator==(scoped_array<C2> const& p2) const;
 template <class C2> bool operator!=(scoped_array<C2> const& p2) const;

 // Disallow evil construtors
 scoped_array(const scoped_array&);
 void operator=(const scoped_array&);
};

#endif // F2M_COMMON_SCOPED_PTR_H_
