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
// Unit Test for Reader (reader.h and reader.cc)
//
// We write some data to a file, then check the sample result.
//
#include "gtest/gtest.h"

#include "src/reader/reader.h"

#include <string>
#include <fstream>

using f2m::reader;

const std::string filename = "/tmp/reader-test.txt";

const int num_data = 6;

const std::string testdata[num_data] = { "apple", "banana", "cat",
                                         "orange", "Pig", "Cake" };
const int num_samples = 1;

class ReaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() { // Write some data to a temp file.
    ofstream file;
    file.open(filename.c_str());
    for (int i = 0; i < num_data; ++i) {
      file << testdata[i] << "\n";
    }
  }
};

TEST(ReaderTest, SampleFromDisk) {
  
}

TEST(ReaderTest, SampleFromMemory) {

}
