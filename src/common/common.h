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
// This file provides the following basic facilities to make
// programming convennient:
// 
//  * A set of CHECKxx macros via gtest.
// 
//  * Basic POD typedefs. 
//
#ifndef F2M_COMMON_COMMON_H_
#define F2M_COMMON_COMMON_H_

#include <stdlib.h>
#ifndef _MSC_VER
#include <stdint.h> // Linux, MacOSX and Cygwin has this standard header.
#else
#include "src/common/stdint_msvc.h"   // Visual C++ use this header.
#endif

#include "gflags/gflags.h"

#include "src/common/logging.h"

//------------------------------------------------------------------------------
// In cases when the program must quit imediately (e.g., due to severe bugs), 
// CHECKxx macros invoke abort() to cause a core dump. To ensure the generation
// of the core dump, you might want to set the following shell option:
//
//    ulimit -c unlimited
//
// Once the core dump is generated, we can check the failure using a debugger, 
// for example, GDB:
//   
//    gbd program_file core
//
// The GDB command 'where' will show you the stack trace.
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// This macros is used to disallow copy constructor and assign operator in 
// class definition. For more details, please refer to Google coding style
// document.
//
// To use this macro, just put it in private section of a class, illustrated as
// the following example.
//
// class Foo {
//   public:
//    Foo();
//   private:
//    DISALLOW_COPY_AND_ASSIGN(Foo);
// };
//------------------------------------------------------------------------------

#define DISALLOW_COPY_AND_ASSIGN(TypeName)              \
  TypeName(const TypeName&);                            \
  void operator=(const TypeName&)

//------------------------------------------------------------------------------
// 
// Basic POD type
//
//------------------------------------------------------------------------------

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

#endif // F2M_COMMON_COMMON_H_