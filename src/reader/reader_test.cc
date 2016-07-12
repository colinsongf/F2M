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
// Unit Test for reader.cc and reader.h
//
#include "src/reader/reader.h"

#include <iostream>

#include "gtest/gtest.h"

using namespace std;

const int kNumIteration = 10;
const int kNumSamples = 2;

namespace f2m {

TEST(ReaderTest, SampleFromDisk) {
  StringList *samples = NULL;
  Reader reader("testdata.txt", kNumSamples);
  for (int i = 0; i < kNumIteration; ++i) {
    samples = reader.Samples();
    //for (int n = 0; n < samples->size(); ++i) {
      //cout << (*samples)[n] << "\n";
   //}
    cout << samples->size() << endl;
  }
}

TEST(ReaderTest, SampleFromMemory) {

}

}
