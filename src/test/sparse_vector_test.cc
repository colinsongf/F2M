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
// Unit test for SparseVector (sparse_vector.h)
//
#include <string>

#include "src/common/common.h"
#include "src/common/sparse_vector.h"

#include "gtest/gtest.h"

using f2m::SparseVector;

typedef SparseVector<uint32, double> RealVector;

TEST(SparseVectorTest, SquareBrakets) {
  RealVector v;
  v.set(101, 1);
  EXPECT_EQ(v[101], 1);
  EXPECT_EQ(v[102], 0);
  EXPECT_EQ(v.has(101), true);
  EXPECT_EQ(v.has(102), false);
}

TEST(SparseVectorTest, Set) {
  RealVector v;
  EXPECT_EQ(v.size(), 0);
  v.set(101, 0);
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.has(101), false);
  v.set(101, 1);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v.has(101), true);
  EXPECT_EQ(v[101], 1);
  v.set(101, 2);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v.has(101), true);
  EXPECT_EQ(v[101], 2);
  v.set(101, 0);
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.has(101), false);
}

TEST(SparseVectorTest, Scale) {
  RealVector v;
  v.set(101, 2);
  v.set(102, 4);
  Scale(&v, 0.5);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[101], 1);
  EXPECT_EQ(v[102], 2);
}

TEST(SparseVectorTest, ScaleInto) {
  RealVector u, v;
  u.set(200, 2);
  v.set(101, 2);
  v.set(102, 4);
  ScaleInto(&u, v, 0.5);
  EXPECT_EQ(u.size(), 2);
  EXPECT_EQ(u[101], 1);
  EXPECT_EQ(u[102], 2);
}

TEST(SparseVectorTest, AddScaled) {
  RealVector u, v;
  u.set(200, 2);
  v.set(101, 2);
  v.set(102, 4);
  AddScaled(&u, v, 0.5);
  EXPECT_EQ(u.size(), 3);
  EXPECT_EQ(u[200], 2);
  EXPECT_EQ(u[101], 1);
  EXPECT_EQ(u[102], 2);
}

TEST(SparseVectorTest, AddScaledInto) {
  RealVector w, u, v;
  w.set(200, 100);
  u.set(101, 2);
  u.set(102, 4);
  u.set(301, 8);
  u.set(302, 100);
  v.set(101, 2);
  v.set(103, 6);
  v.set(301, 8);
  AddScaledInto(&w, u, v, 0.5);
  EXPECT_EQ(w.size(), 5);
  EXPECT_EQ(w[101], 3);
  EXPECT_EQ(w[102], 4);
  EXPECT_EQ(w[103], 3);
  EXPECT_EQ(w[301], 12);
  EXPECT_EQ(w[302], 100);
}

TEST(SparseVectorTest, DotProduct) {
  RealVector v, u, w;
  v.set(101, 2);
  v.set(102, 4);
  v.set(301, 9);
  v.set(302, 100);
  u.set(101, 2);
  u.set(103, 6);
  u.set(301, 9);
  w.set(200, 10);
  EXPECT_EQ(DotProduct(v, u), 85);
}
