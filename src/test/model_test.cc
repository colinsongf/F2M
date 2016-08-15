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

Unit Test for Model class (data_structure.h)
*/

#include "gtest/gtest.h"

#include "src/common/data_structure.h"
#include "src/common/common.h"

namespace f2m {

const real_t init_value = 0.0;
const index_t feature_num = 10000000;
const int k = 20;
const int field_num = 10;

TEST(ModelTest, Initialize) {
  Model model_lr(init_value, LR, feature_num, 0, 0);
  Model model_fm(init_value, FM, feature_num, k, 0);
  Model model_ffm(init_value, FFM, feature_num, k, field_num);
}

TEST(ModelTest, GetW) {
  Model model_lr(init_value, LR, feature_num, 0, 0);
  real_t* pointer = NULL;
  index_t size = model_lr.GetW(&pointer);
  for (index_t i = 0; i < size; ++i) {
    EXPECT_EQ(*(pointer+i), init_value);
  }
}

TEST(ModelTest, GetV_FM) {
  Model model_fm(init_value, FM, feature_num, k, 0);
  real_t* pointer = NULL;
  index_t size = model_fm.GetV(&pointer, 1000);
  for (index_t i = 0; i < size; ++i) {
    EXPECT_EQ(*(pointer+i), init_value);
  }
}

TEST(ModelTest, GetV_FFM) {
  Model model_ffm(init_value, FFM, feature_num, k, field_num);
  real_t* pointer = NULL;
  index_t size = model_ffm.GetV(&pointer, 1000, 5);
  for (index_t i = 0; i < size; ++i) {
    EXPECT_EQ(*(pointer+i), init_value);
  }
}

} // namespace f2m