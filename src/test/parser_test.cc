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

Unit Test for Reader (reader.h and reader.cc)
We write some data to a file, then check the parser's result.
*/

#include "gtest/gtest.h"

#include "src/reader/reader.h"
#include "src/reader/parser.h"
#include "src/common/common.h"
#include "src/common/data_structure.h"

#include <string>
#include <fstream>

using f2m::Reader;
using f2m::StringList;
using f2m::Parser;
using f2m::FFMParser;
using f2m::DataMatrix;

const std::string filename_1 = "/tmp/reader-test-1.txt";
const std::string filename_2 = "/tmp/reader-test-2.txt";

const int num_data = 6;
const int num_line = 8;
const int num_iteration = 1;

const std::string testdata[num_data] = { "12.34", "0.123", "23.43",
                                         "34.123", "0.123", "1.01" };

class ParserTest : public ::testing::Test {
 protected:
  virtual void SetUp() { // Write some data to a temp file.
    std::ofstream file;
    // test file 1 for LR and FM
    file.open(filename_1.c_str());
    // write 10 lines.
    for (int i = 0; i < num_line; ++i) {
      for (int j = 0; j < num_data; ++j) {
      	// index:value
      	file << j << ":" << testdata[j];
      	if (j != num_data - 1) {
      	  file << "\t";
      	} else { // y and \n
      	  file << "\t0.0\n";
      	}
      }
    }
    // test file 2 for FFM
    std::ofstream file_2;
    file_2.open(filename_2.c_str());
    // write 10 lines.
    for (int i = 0; i < num_line; ++i) {
      for (int j = 0; j < num_data; ++j) {
      	file_2 << "1" << ":" << j << ":" << testdata[j];
      	if (j != num_data - 1) {
      	  file_2 << "\t";
      	} else { // y and \n
      	  file_2 << "\t1.0\n";
      	}
      }
    }
  }
};

TEST_F(ParserTest, LR_and_FM) {
  StringList *samples = NULL;
  Reader reader(filename_1, num_line); // sample 10 records at each time.
  DataMatrix matrix(num_line);
  Parser parser;
  // 100 iterations
  for (int i = 0; i < num_iteration; ++i) {
    samples = reader.Samples();
    for (int n = 0; n < num_line; ++n) {
      EXPECT_EQ((*samples)[n], 
      	std::string("0:12.34\t1:0.123\t2:23.43\t3:34.123\t4:0.123\t5:1.01\t0.0"));
    }
    parser.Parse(samples, &matrix);
    for (int n = 0; n < num_line; ++n) {
      EXPECT_EQ(matrix[n].y, 0.0);
      EXPECT_EQ(matrix[n].size, 6);
      for (int k = 0; k < num_data; ++k) {
      	EXPECT_EQ(matrix[n].position[k], k);
      }
      for (int k = 0; k < num_data; ++k) {
      	float value = atof(testdata[k].c_str());
      	EXPECT_EQ(matrix[n].x[k], value);
      }
    }
  }
}

TEST_F(ParserTest, FFM) {
  StringList *samples = NULL;
  Reader reader(filename_2, num_line); // sample 10 records at each time.
  DataMatrix matrix(num_line);
  FFMParser parser;
  // 100 iterations
  for (int i = 0; i < num_iteration; ++i) {
    samples = reader.Samples();
    for (int n = 0; n < num_line; ++n) {
      EXPECT_EQ((*samples)[n],
      	std::string("1:0:12.34\t1:1:0.123\t1:2:23.43\t1:3:34.123\t1:4:0.123\t1:5:1.01\t1.0"));
    }
    parser.Parse(samples, &matrix);
    for (int n = 0; n < num_line; ++n) {
      EXPECT_EQ(matrix[n].size, 6);
      EXPECT_EQ(matrix[n].y, 1.0);
      for (int k = 0; k < num_data; ++k) {
      	EXPECT_EQ(matrix[n].position[k], k);
      }
      for (int k = 0; k < num_data; ++k) {
      	float value = atof(testdata[k].c_str());
      	EXPECT_EQ(matrix[n].x[k], value);
      }
      for (int k = 0; k < num_data; ++k) {
      	EXPECT_EQ(matrix[n].field[k], 1.0);
      }
    }
  }
}