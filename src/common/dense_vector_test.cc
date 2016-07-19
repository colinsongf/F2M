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
// Unit test for DenseVector (dense_vector.h)
//
#include <string>

#include "src/common/common.h"
#include "src/common/dense_vector.h"

#include "gtest/gtest.h"

using f2m::DenseVector;

typedef DenseVector<double> RealVector;

TEST(DenseVectorTest, Scale) {
  RealVector v(2);
  v.data()[0] = 2;
  v.data()[1] = 4;
  Scale(&v, 0.5);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.data()[0], 1);
  EXPECT_EQ(v.data()[1], 2);
}

TEST(DenseVectorTest, ScaleInto) {
  RealVector u(2);
  RealVector v(2);
  u.data()[0] = 2;
  u.data()[1] = 2;
  v.data()[0] = 2;
  v.data()[1] = 4;
  ScaleInto(&u, v, 0.5);
  EXPECT_EQ(u.size(), 2);
  EXPECT_EQ(u.data()[0], 1);
  EXPECT_EQ(u.data()[1], 2);
}

TEST(DenseVectorTest, AddScaled) {
  RealVector u(3);
  RealVector v(3);
  u.data()[0] = 2;
  u.data()[1] = 0;
  u.data()[2] = 0;
  v.data()[0] = 0;
  v.data()[1] = 2;
  v.data()[2] = 4;
  AddScaled(&u, v, 0.5);
  EXPECT_EQ(u.size(), 3);
  EXPECT_EQ(u.data()[0], 2);
  EXPECT_EQ(u.data()[1], 1);
  EXPECT_EQ(u.data()[2], 2);
}

TEST(DenseVectorTest, AddScaledInto) {
  RealVector w(0);
  RealVector u(3);
  RealVector v(3);
  w.resize(3, 1);
  u.data()[0] = 2;
  u.data()[1] = 4;
  u.data()[2] = 6;
  v.data()[0] = 2;
  v.data()[1] = 4;
  v.data()[2] = 0;
  AddScaledInto(&w, u, v, 0.5);
  EXPECT_EQ(w.size(), 3);
  EXPECT_EQ(w.data()[0], 3);
  EXPECT_EQ(w.data()[1], 6);
  EXPECT_EQ(w.data()[2], 6);
}

TEST(DenseVectorTest, DotProduct) {
  RealVector v(2);
  RealVector u(2);
  RealVector w(2);
  v.data()[0] = 1;
  v.data()[1] = 0;
  u.data()[0] = 0;
  u.data()[1] = 1;
  w.data()[0] = 1;
  w.data()[1] = 1;
  EXPECT_EQ(DotProduct(v, u), 0);
  EXPECT_EQ(DotProduct(u, v), 0);
  EXPECT_EQ(DotProduct(v, w), 1);
  EXPECT_EQ(DotProduct(u, w), 1);  
}
