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

This file provides the basic facilities to make the
programming more convennient, including:

 1. Logging facilities
 2. CHECKxxx macros
 3. DISALLOW_COPY_AND_ASSIGN macro

*/

#ifndef F2M_COMMON_COMMON_H_
#define F2M_COMMON_COMMON_H_

#include <stdlib.h>
#ifndef _MSC_VER
#include <stdint.h> // Linux, MacOSX and Cygwin has this standard header.
#endif

#if defined _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0501  // windows xp
# endif
# define NOMINMAX 1
# include <windows.h>
#elif defined __unix__ || defined __APPLE__
# include <pthread.h>
# include <sys/time.h>
#else
# error Unknown platform
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

using std::string;
using std::vector;
using std::set;

/* -----------------------------------------------------------------------------
 * Provde a basic logging facilities that treat log messages by their           *
 * severites. If function |InitializeLogger| was invoked and was able           *
 * to open files specified by the parameters, log messages of various           *
 * severity will be written into corresponding files.                           *
 * Otherwise, all log messages will be written to stderr.                       *
 *                                                                              *
 * Example:                                                                     *
 *                                                                              *
 *  int main() {                                                                *
 *    InitializeLogger("/tmp/info.log", "/tmp/warn.log", "/tmp/erro/log");      *
 *    LOG(INFO)    << "An info message going into /tmp/info.log";               *
 *    LOG(WARNING) << "An warn message going into /tmp/warn.log";               *
 *    LOG(ERROR)   << "An erro message going into /tmp/erro.log";               *
 *    LOG(FATAL)   << "An fatal message going into /tmp/erro.log, "             *
 *                 << "and kills current process by a segmentation fault.";     *
 *    return 0;                                                                 *
 *  }                                                                           *
 * -----------------------------------------------------------------------------
 */

void InitializeLogger(const std::string& info_log_filename,
                      const std::string& warn_log_filename,
                      const std::string& erro_log_filename);

enum LogSeverity { INFO, WARNING, ERROR, FATAL };

class Logger {
  friend void InitializeLogger(const std::string& info_log_filename,
                               const std::string& warn_log_filename,
                               const std::string& erro_log_filename);
 public:
  Logger(LogSeverity s) : severity_(s) {}
  ~Logger();

  static std::ostream& GetStream(LogSeverity severity);
  static std::ostream& Start(LogSeverity severity,
                             const std::string& file,
                             int line,
                             const std::string& function);

 private:
  static std::ofstream info_log_file_;
  static std::ofstream warn_log_file_;
  static std::ofstream erro_log_file_;
  LogSeverity severity_;
};

#define LOG(severity)                                                     \
  Logger(severity).Start(severity, __FILE__, __LINE__, __FUNCTION__)

/* -----------------------------------------------------------------------------
 * In cases when the program must quit imediately (e.g., due to severe bugs),   *
 * CHECKxx macros invoke abort() to cause a core dump. To ensure the generation *
 * of the core dump, you might want to set the following shell option:          *
 *                                                                              *
 *    ulimit -c unlimited                                                       *
 *                                                                              *
 * Once the core dump is generated, we can check the failure using a debugger,  *
 * for example, GDB:                                                            *
 *                                                                              *
 *    gbd program_file core                                                     *
 *                                                                              *
 * The GDB command 'where' will show you the stack trace.                       *
 * -----------------------------------------------------------------------------
 */

#define CHECK(a) if (!(a)) {                            \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n";           \
    abort();                                            \
  }                                                     \

#define CHECK_NOTNULL(a) if ((a) == NULL) {             \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " == NULL \n";                  \
    abort();                                            \
  }                                                     \

#define CHECK_NULL(a) if ((a) != NULL) {                \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_EQ(a, b) if (!((a) == (b))) {             \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n"            \
               << #b << " = " << (b) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_NE(a, b) if (!((a) != (b))) {             \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n"            \
               << #b << " = " << (b) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_GT(a, b) if (!((a) > (b))) {              \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n"            \
               << #b << " = " << (b) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_LT(a, b) if (!((a) < (b))) {              \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n"            \
               << #b << " = " << (b) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_GE(a, b) if (!((a) >= (b))) {             \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n"            \
               << #b << " = " << (b) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_LE(a, b) if (!((a) <= (b))) {             \
    LOG(ERROR) << "CHECK failed "                       \
               << __FILE__ << ":" << __LINE__ << "\n"   \
               << #a << " = " << (a) << "\n"            \
               << #b << " = " << (b) << "\n";           \
    abort();                                            \
}                                                       \

#define CHECK_NEAR(a, b, margin)                        \
    do {                                                \
      CHECK_LE((a), (b)+(margin));                      \
      CHECK_GE((a), (b)-(margin));                      \
    } while (0)

#define CHECK_DOUBLE_EQ(a, b)                           \
    do {                                                \
      CHECK_LE((a), (b)+0.000000000000001L);            \
      CHECK_GE((a), (b)-0.000000000000001L);            \
    } while(0)

/* -----------------------------------------------------------------------------
 * This macros is used to disallow copy constructor and assign operator in      *      
 * class definition. For more details, please refer to Google coding style      *
 * document.                                                                    *
 *                                                                              *
 * To use this macro, just put it in private section of a class, illustrated as *
 * the following example.                                                       *
 *                                                                              *
 * class Foo {                                                                  *
 *  public:                                                                     *
 *   Foo();                                                                     *
 *  private:                                                                    *
 *   DISALLOW_COPY_AND_ASSIGN(Foo);                                             *
 * };                                                                           *
 * -----------------------------------------------------------------------------
 */

#define DISALLOW_COPY_AND_ASSIGN(TypeName)              \
  TypeName(const TypeName&);                            \
  void operator=(const TypeName&)

/* -----------------------------------------------------------------------------
 * Basic POD type                                                               *
 * -----------------------------------------------------------------------------
 */

typedef unsigned int uint;

#ifdef _MSC_VER
typedef __int8  int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#else
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
#endif

static const int32 kInt32Max = 0x7FFFFFFF;
static const int32 kInt32Min = -kInt32Max - 1;
static const int64 kInt64Max = 0x7FFFFFFFFFFFFFFFll;
static const int64 kInt64Min = -kInt64Max - 1;
static const uint32 kUInt32Max = 0xFFFFFFFFu;
static const uint64 KUInt64Max = 0xFFFFFFFFFFFFFFFFull;

/* -----------------------------------------------------------------------------
 * Open file or die. Usage:                                                     *
 *                                                                              *
 *  std::string filename = "training_data.txt";                                 *
 *  FILE* file = NULL;                                                          *
 *                                                                              *
 *  // Read only                                                                *
 *  file = OpenFileOrDie(filename.c_str(), "r");                                *
 *                                                                              *
 *  // Write only                                                               *
 *  file = OpenFileOrDie(filename.c_str(), "w");                                *
 * -----------------------------------------------------------------------------
 */

 inline FILE* OpenFileOrDie(const char* filename, const char* mode) {
  FILE* input_stream = fopen(filename, mode);
  if (input_stream == NULL) {
    LOG(FATAL) << "Cannot open file: " << filename << " with mode: " << mode;
  }
  return input_stream;
}

/* -----------------------------------------------------------------------------
 * The facilities that enhance the STL.                                         *
 *                                                                              *
 * 1. Delete elements (in pointer type) in a STL container like                 *
 *    vector, list, and deque.                                                  *
 *                                                                              *
 * 2. Delete elements (in pointer type) in a STL associative container          *
 *    like map and hash_map.                                                    *
 * -----------------------------------------------------------------------------
 */

template <class Container>
void STLDeleteElementsAndClear(Container* c) {
  for (typename Container::iterator iter = c->begin();
       iter != c->end(); ++iter) {
    if (*iter != NULL) {
      delete *iter;
    }
  }
  c->clear();
}

template <class AssocContainer>
void STLDeleteValuesAndClear(AssocContainer* c) {
  for (typename AssocContainer::iterator iter = c->begin();
       iter != c->end(); ++iter) {
    if (iter->second != NULL) {
      delete iter->second;
    }
  }
  c->clear();
}

/* -----------------------------------------------------------------------------
 * The following source code is mainly copied from http://www.partow.net, with  *
 * the following Copyright information.                                         *
 *                                                                              *
 *                                                                              *
 *          General Purpose Hash Function Algorithms Library                    *
 *                                                                              *
 * Author: Arash Partow - 2002                                                  *
 * URL: http://www.partow.net                                                   *
 * URL: http://www.partow.net/programming/hashfunctions/index.html              *
 *                                                                              *
 * Copyright notice:                                                            *
 * Free use of the General Purpose Hash Function Algorithms Library is          *
 * permitted under the guidelines and in accordance with the most current       *
 * version of the Common Public License.                                        *
 * http://www.opensource.org/licenses/cpl1.0.php                                *
 *                                                                              *
 * -----------------------------------------------------------------------------
 */

typedef unsigned int (*HashFunction)(const std::string&);

unsigned int RSHash(const std::string& str);
unsigned int JSHash(const std::string& str);
unsigned int PJWHash(const std::string& str);
unsigned int ELFHash(const std::string& str);
unsigned int BKDRHash(const std::string& str);
unsigned int SDBMHash(const std::string& str);
unsigned int DJBHash(const std::string& str);
unsigned int DEKHash(const std::string& str);
unsigned int BPHash(const std::string& str);
unsigned int FNVHash(const std::string& str);
unsigned int APHash(const std::string& str);

/* -----------------------------------------------------------------------------
 * This is an implementation deisgned to match anticipated future TR2           *
 * implementation of the scoped_ptr class, and its closely-related brethren,    *
 * scoped_array, scoped_ptr_malloc, and make_scoped_ptr.                        *
 * -----------------------------------------------------------------------------
 */ 

template <class C> class scoped_ptr;
template <class C> class scoped_array;

/* A scoped_ptr<T> is like a T*, except that the destructor of scoped_ptr<T>
   automatically deletes the pointer it holds (if any).
  
   That is, scoped_ptr<T> owns the T object that it points to.
   Like a T*, a scoped_ptr<T> may hold any either NULL or a pointer to a 
   T object.
   The size of a scoped_ptr is small:
  
   sizeof(scoped_ptr<C>) == size_of(C*) */

template <class C>
class scoped_ptr {
 public:
  typedef C element_type;
 
  /* Constructor. Defaults to initializing with NULL.
     There is no way to create an unintialized scoped_ptr
     The input parameter must be allocated with new. */

  explicit scoped_ptr(C* p = NULL) : ptr_(p) {}

  /* Destructor. If there is a C object, delete it.
     We don't need to test ptr_ == NULL because C++ does that for us.
     By contrast, the free() function in C does not do that. */

  ~scoped_ptr() {
    enum { type_must_be_complete = sizeof(C) };
    delete ptr_;
  }

  /* Reset. Deletes the current owned object, if any.
     Then takes ownership of a new object , if given.
     this->reset(this->get()) works. */

  void reset(C* p = NULL) {
    if (p != NULL) {
      enum { type_must_be_complete = sizeof(C) };
      delete ptr_;
      ptr_  = p;
    }
  }

  /* Accessors to get the owned object.
     operator* and operator-> will assert() if there is 
     no current object. */

  C& operator*() const {
    assert(ptr_ != NULL);
    return *ptr_;
  }

  C* operator->() const {
    assert(ptr_ != NULL);
    return ptr_;
  }

  C* get() const { return ptr_; }

  /* Comparision operators.
     These return whether two scoped_ptr refer to the same object, 
     not just to two different but equal objects. */

  bool operator==(C* p) const { return ptr_ == p; }
  bool operator!=(C* p) const { return ptr_ != p; }

  /* Swap two scoped pointers. */

  void swap(scoped_ptr& p2) {
    C* tmp = ptr_;
    ptr_ = p2.ptr_;
    p2.ptr_  = tmp;
  }

  /* Release a pointer.
     The return value is the current pointer held by this object.
     If this object holds a NULL pointer, the return value is NULL.
     After this operation, this object will hold a NULL pointer
     and will not own the object any more. */

  C* release() {
    C* retVal = ptr_;
    ptr_ = NULL;
    return retVal;
  }

 private:
  C* ptr_;

  /* Forbid comparision of scoped_ptr types. If C2 != C, it totally dosen't
     make sense, and if C2 == C, it still doesn't make sense becasue you should 
     never have the same object owned by two different scoped_ptrs. */

  template <class C2> bool operator==(scoped_ptr<C2> const& p2) const;
  template <class C2> bool operator!=(scoped_ptr<C2> const& p2) const;

  /* Disallow evil construtors. */

  scoped_ptr(const scoped_ptr&);
  void operator=(const scoped_ptr&);
};

/* scoped_array<C> is like scoped_ptr<C>, except that the caller must allocate 
   with new [] and the destructor must delete objects with delete [].
 
   As with scoped_ptr<C>, a scoped_array<C> either points to an object 
   or is NULL. A scoped_array<C> owns the object that it points to.
 
   Size: sizeof(scoped_array<C>) == sizeof(C*) (8 bytes on a 64 bits machine) */

template <class C>
class scoped_array {
 public:

  typedef C element_type;

  /* Constructor. Defaults to intializing with NULL.
     There is no way to create an unintialized scoped_array.
     The input parameter must be allocated with new []. */

  explicit scoped_array(C* p = NULL) : array_(p) {}

  /* Destructor. If there is a C object, delete it.
     We do not need to test ptr_ == NULL because C++ 
     does that for us. */

  ~scoped_array() {
    enum { type_must_be_complete = sizeof(C) };
    delete [] array_;
  }

  /* Reset. Deletes the cuurent owned object, if any. 
     Then takes ownership of a new object, if given.
     this->reset(this->get()) works. */

  void reset(C* p = NULL) {
    if (p != array_) {
      enum { type_must_be_complete = sizeof(C) };
      delete[] array_;
      array_ = p;
    }   
  }

  /* Get one element of the current object.
     Will assert() if there is no current object, or index i is negative.
     Note that unlike the size_t, ptrdiff_t can be a negative value. */

  C& operator[] (std::ptrdiff_t i) const {
    assert(i >= 0);
    assert(array_ != NULL);
    return array_[i];
  }

  /* Get a pointer to the first element of the current object.
     If there is no current object, return NULL. */

  C* get() const {
    return array_;   
  }

  /* Release a pointer.
     The return value is the current pointer held by this object.
     If this object holds a NULL pointer, the return value is NULL.
     After this operation, this object will hold a NULL pointer
     and will not own the object any more. */

  C* release() {
    C* retVal = array_;
    array_ = NULL;
    return retVal;
  } 
   
 private:
   C* array_;

 /* Forbid comparision of different scoped_array types. */

 template <class C2> bool operator==(scoped_array<C2> const& p2) const;
 template <class C2> bool operator!=(scoped_array<C2> const& p2) const;

 /* Disallow evil construtors. */

 scoped_array(const scoped_array&);
 void operator=(const scoped_array&);
}; 

/* -----------------------------------------------------------------------------
 * Defines several helper macros for registering class by a string name and     *
 * creating them later per the registered name.                                 *
 * The motivation is to help implement the factory class. C++ doesn't support   *
 * reflection so we defines several macros to do this.                          *
 *                                                                              *
 * All macros defined here are NOT used by final user directly, they are used   *
 * to create register macros for a specific base class. Here is an example:     *
 *                                                                              *
 * ---------------------------------------------------------------------------- *
 *   [mapper.h (the interface definition)]                                      *
 *                                                                              *
 *   #include "class_register.h"                                                *
 *                                                                              *
 *   class Mapper {                                                             *
 *     ...                                                                      *
 *   };                                                                         *
 *   CLASS_REGISTER_DEFINE_REGISTRY(mapper_register, Mapper);                   *
 *   #define REGISTER_MAPPER(mapper_name)                                  \    *
 *       CLASS_REGISTER_OBJECT_CREATOR(                                    \    *
 *           mapper_register, Mapper, #mapper_name, mapper_name)           \    *
 *                                                                              *
 *   #define CREATE_MAPPER(mapper_name_as_string)                          \    *
 *       CLASS_REGISTER_CREATE_OBJECT(mapper_register,                     \    *
 *                                    mapper_name_as_string)                    *
 *                                                                              *
 * ---------------------------------------------------------------------------- *
 *   [hello_mapper.cc (an implementation of Mapper)]                            *
 *                                                                              *
 *   #include "mapper.h"                                                        *
 *                                                                              *
 *   class HelloMapper : public Mapper {                                        *
 *   };                                                                         *
 *   REGISTER_MAPPER(HelloMapper);                                              *
 *                                                                              *
 * ---------------------------------------------------------------------------- *
 *   [mapper_user.cc (the final user of all registered mappers)]                *
 *                                                                              *
 *   #include "mapper.h"                                                        *
 *                                                                              *
 *   Mapper* mapper = CREATE_MAPPER("HelloMapper");                             *
 *                                                                              *
 * ---------------------------------------------------------------------------- * 
 * Another usage is to register by class by an arbitrary string instead of its  *
 * class name, and register a default class when no registerd name is matched.  *
 *                                                                              *
 * Here is an example:                                                          *
 *                                                                              *
 *   [file_impl.h (the interface definition)]                                   *
 *                                                                              *
 *   class FileImpl {                                                           *
 *   };                                                                         *
 *   CLASS_REGISTER_DEFINE_REGISTRY(file_impl_register, FileImpl);              *
 *   #define REGISTER_DEFAULT_FILE_IMPL(file_impl_name)                   \     *
 *     CLASS_REGISTER_DEFAULT_OBJECT_CREATOR(                             \     *
 *        file_impl_register, FileImpl, file_impl_name)                         *
 *                                                                              *
 *   #define REGISTER_FILE_IMPL(path_prefix_as_string, file_impl_name)    \     *
 *     CLASS_REGISTER_OBJECT_CREATOR(                                     \     *
 *        file_impl_register, FileImpl, path_prefix_as_string,            \     *
 *        file_impl_name)                                                       *
 *                                                                              *
 *   #define CREATE_FILE_IMPL(path_prefix_as_string)                      \     *
 *     CLASS_REGISTER_CREATE_OBJECT(file_impl_register,                   \     *
 *                                  path_prefix_as_string)                      *
 *                                                                              *
 * ---------------------------------------------------------------------------- *
 *   [local_file.cc (an implementation of FileImpl)]                            *
 *                                                                              *
 *   #include "file.h"                                                          *
 *                                                                              *
 *   class LocalFileImpl : public FileImpl {                                    *
 *   };                                                                         *
 *   REGISTER_DEFAULT_FILE_IMPL(LocalFileImpl);                                 *
 *   REGISTER_FILE_IMPL("/local", LocalFileImpl);                               *
 *                                                                              *
 * ---------------------------------------------------------------------------- *
 *   [file_user.cc (the final user of all registered file implementations)]     *
 *                                                                              *
 *   #include "file_impl.h"                                                     *
 *                                                                              *
 *   FileImpl* file_impl = CREATE_FILE_IMPL("/local");                          *
 * ---------------------------------------------------------------------------- *
 */

/* The first parameter, register_name, should be unique globally.
   Another approach for this is to define a template for base class. It would
   make the code more readable, but the only issue of using template is that
   each base class could have only one register then. It doesn't sound very
   likely that a user wants to have multiple registers for one base class,
   but we keep it as a possibility.
   We would switch to using template class if necessary. */

#define CLASS_REGISTER_DEFINE_REGISTRY(register_name, base_class_name)    \
  class ObjectCreatorRegistry_##register_name {                           \
   public:                                                                \
   typedef base_class_name* (*Creator)();                                 \
                                                                          \
   ObjectCreatorRegistry_##register_name()                                \
   : m_default_creator(NULL) {}                                           \
   ~ObjectCreatorRegistry_##register_name() {}                            \
                                                                          \
   void SetDefaultCreator(Creator creator) {                              \
     m_default_creator = creator;                                         \
   }                                                                      \
                                                                          \
   void AddCreator(std::string entry_name, Creator creator) {             \
     m_creator_registry[entry_name] = creator;                            \
   }                                                                      \
                                                                          \
   base_class_name* CreateObject(const std::string& entry_name);          \
                                                                          \
   private:                                                               \
   typedef std::map<std::string, Creator> CreatorRegistry;                \
   Creator m_default_creator;                                             \
   CreatorRegistry m_creator_registry;                                    \
  };                                                                      \
                                                                          \
  inline ObjectCreatorRegistry_##register_name&                           \
  GetRegistry_##register_name() {                                         \
    static ObjectCreatorRegistry_##register_name registry;                \
    return registry;                                                      \
  }                                                                       \
                                                                          \
  class DefaultObjectCreatorRegister_##register_name {                    \
   public:                                                                \
   DefaultObjectCreatorRegister_##register_name(                          \
       ObjectCreatorRegistry_##register_name::Creator creator) {          \
     GetRegistry_##register_name().SetDefaultCreator(creator);            \
   }                                                                      \
   ~DefaultObjectCreatorRegister_##register_name() {}                     \
  };                                                                      \
                                                                          \
  class ObjectCreatorRegister_##register_name {                           \
   public:                                                                \
   ObjectCreatorRegister_##register_name(                                 \
       const std::string& entry_name,                                     \
       ObjectCreatorRegistry_##register_name::Creator creator) {          \
     GetRegistry_##register_name().AddCreator(entry_name,                 \
                                              creator);                   \
   }                                                                      \
   ~ObjectCreatorRegister_##register_name() {}                            \
  }

#define CLASS_REGISTER_IMPLEMENT_REGISTRY(register_name, base_class_name) \
  base_class_name* ObjectCreatorRegistry_##register_name::CreateObject(   \
      const std::string& entry_name) {                                    \
    Creator creator = m_default_creator;                                  \
    CreatorRegistry::const_iterator it =                                  \
        m_creator_registry.find(entry_name);                              \
    if (it != m_creator_registry.end()) {                                 \
      creator = it->second;                                               \
    }                                                                     \
                                                                          \
    if (creator != NULL) {                                                \
      return (*creator)();                                                \
     } else {                                                             \
      return NULL;                                                        \
    }                                                                     \
  }

#define CLASS_REGISTER_DEFAULT_OBJECT_CREATOR(register_name,              \
                                              base_class_name,            \
                                              class_name)                 \
  base_class_name* DefaultObjectCreator_##register_name##class_name() {   \
    return new class_name;                                                \
  }                                                                       \
  DefaultObjectCreatorRegister_##register_name                            \
  g_default_object_creator_register_##register_name##class_name(          \
      DefaultObjectCreator_##register_name##class_name)

#define CLASS_REGISTER_OBJECT_CREATOR(register_name,                      \
                                      base_class_name,                    \
                                      entry_name_as_string,               \
                                      class_name)                         \
  base_class_name* ObjectCreator_##register_name##class_name() {          \
    return new class_name;                                                \
  }                                                                       \
  ObjectCreatorRegister_##register_name                                   \
  g_object_creator_register_##register_name##class_name(                  \
      entry_name_as_string,                                               \
      ObjectCreator_##register_name##class_name)

#define CLASS_REGISTER_CREATE_OBJECT(register_name, entry_name_as_string) \
  GetRegistry_##register_name().CreateObject(entry_name_as_string)

/* -----------------------------------------------------------------------------
 * String splitting utilities.                                                  *
 *                                                                              *
 * Subdivide string |full| into substrings according to delimitors              *
 * given in |delim|. |delim| should pointing to a string including              *
 * one or more characters.  Each character is considerred a possible            *
 * delimitor.  For example:                                                     *
 *                                                                              *
 *   vector<string> substrings;                                                 *
 *   SplitStringUsing("apple orange\tbanana", "\t ", &substrings);              *
 *                                                                              *
 * results in three substrings:                                                 *
 *                                                                              *
 *   substrings.size() == 3                                                     *
 *   substrings[0] == "apple"                                                   *
 *   substrings[1] == "orange"                                                  *
 *   substrings[2] == "banana"                                                  *
 * -----------------------------------------------------------------------------
 */

void SplitStringUsing(const std::string& full,
                      const char* delim,
                      std::vector<std::string>* result);

/* This function has the same semnatic as SplitStringUsing.  
   Results are saved in an STL set container. */

void SplitStringToSetUsing(const std::string& full,
                           const char* delim,
                           std::set<std::string>* result);
   
template <typename T>
struct simple_insert_iterator {
  explicit simple_insert_iterator(T* t) : t_(t) { }

  simple_insert_iterator<T>& operator=(const typename T::value_type& value) {
    t_->insert(value);
    return *this;
  }

  simple_insert_iterator<T>& operator*() { return *this; }
  simple_insert_iterator<T>& operator++() { return *this; }
  simple_insert_iterator<T>& operator++(int placeholder) { return *this; }

  T* t_;
};

template <typename T>
struct back_insert_iterator {
  explicit back_insert_iterator(T& t) : t_(t) {}

  back_insert_iterator<T>& operator=(const typename T::value_type& value) {
    t_.push_back(value);
    return *this;
  }

  back_insert_iterator<T>& operator*() { return *this; }
  back_insert_iterator<T>& operator++() { return *this; }
  back_insert_iterator<T> operator++(int placeholder) { return *this; }

  T& t_;
};

template <typename StringType, typename ITR>
static inline
void SplitStringToIteratorUsing(const StringType& full,
                                const char* delim,
                                ITR& result) {
  // Optimize the common case where delim is a single character.
  if (delim[0] != '\0' && delim[1] == '\0') {
    char c = delim[0];
    const char* p = full.data();
    const char* end = p + full.size();
    while (p != end) {
      if (*p == c) {
        ++p;
      } else {
        const char* start = p;
        while (++p != end && *p != c) {
          // Skip to the next occurence of the delimiter.
        }
        *result++ = StringType(start, p - start);
      }
    }
    return;
  }

  std::string::size_type begin_index, end_index;
  begin_index = full.find_first_not_of(delim);
  while (begin_index != std::string::npos) {
    end_index = full.find_first_of(delim, begin_index);
    if (end_index == std::string::npos) {
      *result++ = full.substr(begin_index);
      return;
    }
    *result++ = full.substr(begin_index, (end_index - begin_index));
    begin_index = full.find_first_not_of(delim, end_index);
  }
}

/* -----------------------------------------------------------------------------
 * String join utilities. Here is an example:                                   *
 *                                                                              *    
 *  std::vector<std::string>  vec;                                              *
 *  vec.push_back("apple");                                                     *
 *  vec.push_back("banana");                                                    *
 *  vec.push_back("orange");                                                    *
 *                                                                              *
 *  std::string output;                                                         *
 *  JoinStrings(vec.begin(), vec.end(), ",", &output);                          *
 *  EXPECT_EQ("apple,banana,orange", output);                                   *
 *  EXPECT_EQ("apple,banana,orange",                                            *
 *            JoinStrings(vec.begin(), vec.end(), ","));                        *
 * -----------------------------------------------------------------------------
 */

template <class ConstForwardIterator>
void JoinStrings(const ConstForwardIterator& begin,
                 const ConstForwardIterator& end,
                 const std::string& delimiter,
                 std::string* output) {
  output->clear();
  for (ConstForwardIterator iter = begin; iter != end; ++iter) {
    if (iter != begin) {
      output->append(delimiter);
    }
    output->append(*iter);
  }
}

template <class ConstForwardIterator>
std::string JoinStrings(const ConstForwardIterator& begin,
                        const ConstForwardIterator& end,
                        const std::string& delimiter) {
  std::string output;
  JoinStrings(begin, end, delimiter, &output);
  return output;
}

template <class Container>
std::string JoinStrings(const Container& container,
                        const std::string& delimiter = " ") {
  return JoinStrings(container.begin(), container.end(), delimiter);
}

/* -----------------------------------------------------------------------------
 * String printf utilities.                                                     *
 * This code comes from the re2 project host on Google Code                     *
 * (http://code.google.com/p/re2/), in particular, the following source file    *
 * http://code.google.com/p/re2/source/browse/util/stringprintf.cc              *
 *                                                                              *
 * Here is an example:                                                          *
 *                                                                              *
 *   EXPECT_EQ(StringPrintf("%d"), 1), std::string("1"));                       *
 *   std::string target;                                                        *
 *   EXPECT_EQ(target, string("1"));                                            *
 *   StringAppendF(&target, "%d", 2);                                           *
 *   EXPECT_EQ(target, string("12"));                                           *                           
 * -----------------------------------------------------------------------------
 */

std::string StringPrintf(const char* format, ...);
void SStringPrintf(std::string* dst, const char* format, ...);
void StringAppendF(std::string* dst, const char* format, ...);

/* -----------------------------------------------------------------------------
 * scoped_locker                                                                *
 * This implementation is a copy from :                                         *
 *   http://tc-svn.tencent.com/setech/setech_infrastructure_rep/Infra_proj/     *
 *   trunk/src/common/system/concurrency/scoped_locker.hpp                      *                         
 * -----------------------------------------------------------------------------
 */

template <typename LockType>
class ScopedLocker {
 public:
  explicit ScopedLocker(LockType* lock) : m_lock(lock) {
    m_lock->Lock();
  }
  ~ScopedLocker() {
    m_lock->Unlock();
  }
 private:
  LockType* m_lock;
};

template <typename LockType>
class ScopedReaderLocker {
 public:
  explicit ScopedReaderLocker(LockType* lock) : m_lock(lock) {
    m_lock->ReaderLock();
  }
  ~ScopedReaderLocker() {
    m_lock->ReaderUnlock();
  }
 private:
  LockType* m_lock;
};

template <typename LockType>
class ScopedWriterLocker {
 public:
  explicit ScopedWriterLocker(LockType* lock) : m_lock(*lock) {
    m_lock.WriterLock();
  }
  ~ScopedWriterLocker() {
    m_lock.WriterUnlock();
  }
 private:
  LockType& m_lock;
};

/* -----------------------------------------------------------------------------
 * Mutex                                                                        *
 * This implementation is a copy from                                           *
 *   http://tc-svn.tencent.com/setech/setech_infrastructure_rep/Infra_proj/     *
 *   trunk/src/common/system/concurrency/mutex.hpp                              *    
 * -----------------------------------------------------------------------------
 */

class ConditionVariable;

#if defined _WIN32

// if _WIN32_WINNT not defined, TryEnterCriticalSection will not be declared
// in windows.h
extern "C" WINBASEAPI
BOOL WINAPI TryEnterCriticalSection(
    __inout LPCRITICAL_SECTION lpCriticalSection);

class Mutex {
 public:
  typedef ScopedLocker<Mutex> Locker;
 public:
  explicit Mutex(bool recursive = true) {
    ::InitializeCriticalSection(&m_Mutex);
  }

  ~Mutex() {
    ::DeleteCriticalSection(&m_Mutex);
  }

  void Lock() {
    ::EnterCriticalSection(&m_Mutex);
  }

  bool TryLock() {
    return ::TryEnterCriticalSection(&m_Mutex) != FALSE;
  }

  void Unlock() {
    ::LeaveCriticalSection(&m_Mutex);
  }

 private:
  // In Microsoft Windows Server 2003 Service Pack 1 and later versions of
  // Windows, the LockCount field is parsed as follows:
  // * The lowest bit shows the lock status. If this bit is 0, the critical
  //   section is locked; if it is 1, the critical section is not locked.
  // * The next bit shows whether a thread has been woken for this lock.
  //   If this bit is 0, then a thread has been woken for this lock; if it
  //   is 1, no thread has been woken.
  // * The remaining bits are the ones-complement of the number of threads
  //   waiting for the lock.
  static bool IsNewBehavior() {
    static bool result = DoIsNewBehavior();
    return result;
  }
  static bool DoIsNewBehavior() {
    Mutex mutex;
    int old_lock_count = mutex.m_Mutex.LockCount;
    mutex.Lock();
    int new_lock_count = mutex.m_Mutex.LockCount;
    return new_lock_count < old_lock_count;
  }
 private:
  Mutex(const Mutex& right);
  Mutex& operator = (const Mutex& right);
  CRITICAL_SECTION m_Mutex;
  friend class Cond;
};

#elif defined __unix__ || defined __APPLE__

class Mutex {
 public:
  typedef ScopedLocker<Mutex> Locker;
 public:
  explicit Mutex(bool recursive = true) {
    int n;
    if (recursive) {
      pthread_mutexattr_t attr;
      n = pthread_mutexattr_init(&attr);
      if (n == 0) {
        n = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        if (n == 0)
          n = pthread_mutex_init(&m_Mutex, &attr);
        n = pthread_mutexattr_destroy(&attr);
      }
    } else {
      n = pthread_mutex_init(&m_Mutex, NULL);
    }
    CheckError("Mutex::Mutex", n);
  }
  ~Mutex() {
    pthread_mutex_destroy(&m_Mutex);
  }

  void Lock() {
    CheckError("Mutex::Lock", pthread_mutex_lock(&m_Mutex));
  }

  bool TryLock() {
    int n = pthread_mutex_trylock(&m_Mutex);
    if (n == EBUSY) {
      return false;
    } else {
      CheckError("Mutex::Lock", n);
      return true;
    }
  }

  void Unlock() {
    CheckError("Mutex::Unlock", pthread_mutex_unlock(&m_Mutex));
    // NOTE: can't check unlocked here, maybe already locked by
    // other thread
  }

 private:
  static void CheckError(const char* context, int error) {
    if (error != 0) {
      std::string msg = context;
      msg += " error: ";
      msg += strerror(error);
      throw std::runtime_error(msg);
    }
  }
 private:
  Mutex(const Mutex& right);
  Mutex& operator = (const Mutex& right);
 private:
  pthread_mutex_t m_Mutex;
  friend class ConditionVariable;
};

#endif

typedef Mutex::Locker MutexLocker;

/// null mutex for template mutex param placeholder
/// NOTE: don't make this class uncopyable
class NullMutex {
 public:
  typedef ScopedLocker<NullMutex> Locker;
 public:
  NullMutex() : m_locked(false) {
  }

  void Lock() {
    m_locked = true;
  }

  bool TryLock() {
    m_locked = true;
    return true;
  }

  void Unlock() {
    m_locked = false;
  }
 private:
  bool m_locked;
};

/* -----------------------------------------------------------------------------
 * ConditionVariable                                                            *
 * This implementation is a copy from                                           *
 *   http://tc-svn.tencent.com/setech/setech_infrastructure_rep/Infra_proj/     *
 *   trunk/src/common/system/concurrency/condition_variable.hpp                 *    
 * -----------------------------------------------------------------------------
 */

class ConditionVariable {
 public:
  ConditionVariable();
  ~ConditionVariable();

  void Signal();
  void Broadcast();

  bool Wait(Mutex* inMutex, int inTimeoutInMilSecs);
  void Wait(Mutex* inMutex);

 private:
#if defined _WIN32
  HANDLE m_hCondition;
  unsigned int m_nWaitCount;
#elif defined __unix__ || defined __APPLE__
  pthread_cond_t m_hCondition;
#endif
  static void CheckError(const char* context, int error);
};

#endif // F2M_COMMON_COMMON_H_