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
// Unit test for CArray (carray.h)
//
#include "gtest/gtest.h"

#include "src/common/common.h"
#include "src/common/carray.h"

class DestructDetector {
 public:
  explicit DestructDetector(bool* flag) {
    flag_ = flag;
  }
  ~DestructDetector() {
    *flag_ = true;    
  }
 private:
  bool* flag_;
};

TEST(CArrayTest, NumericalValueElements) {
  CArray<double> ad(3, 0.1);
  EXPECT_EQ(ad.size(), 3);
  for (int i = 0; i < ad.size(); ++i) {
    EXPECT_EQ(ad.data()[i], 0.1);    
  }

  ad.resize(4, 0.2);
  EXPECT_EQ(ad.size(), 4);
  for (int i = 0; i < ad.size(); ++i) {
    EXPECT_EQ(ad.data()[i], 0.2);
  }
}
