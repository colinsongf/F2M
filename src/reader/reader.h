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
// Reader returns N data samples (string) in each iteration.
//
// We can use Reader class like this (it is not the real C++ code):
/*
    #include "reader.h"

    // Constructor
    Reader reader(filename = "/tmp/testdata", 
                  num_samples = 100); 

    Loop until converge {

       Data = reader.Samples(); // return N data samples from disk file

       ... use the data samples to train model
       
    }
*/
//
// We can set the number of N in Reader class:
// 
//   N = 1 indicates that we use SGD.
//   N = total_samples indicates that we use Bactch GD.
//   others indicate that we use mini-batch GD.
//
// Before sampling, we can load all data into memory if the capacity
// of your main memory is big enough for current training task:
//
/*
    #include "reader.h"

    // Constructor
    Reader reader(filename = "/tmp/testdata",
                  num_samples = 100,
                  in_memory = true,
                  size_memory_buffer = 5 * 1024 * 1024 * 1024); // size = 5 GB

    Loop until converge {
    
       Data = reader.Samples(); // return N data samples from memory

       ... use the dat samples to train model

    }
*/
// Reader is an algorithm-agnostic class and can mask the details of
// the data source (on disk or in memory), and it is flexible for 
// different gradient descent methods (e.g., SGD, mini-batch GD, and 
// batch GD)
//
#ifndef F2M_READER_READER_H_
#define F2M_READER_READER_H_

#include <string>
#include <vector>

#include "src/common/common.h"
#include "src/common/scoped_ptr.h"

namespace f2m {
    
class Reader {
 public:
  Reader(std::string filename,
         int num_samples,
         bool in_memory = false
         int size_memory_buffer = 0)
    : filename_(filename),
      num_samples_(num_samples)
      in_memory_(in_memory) 
      size_memory_buffer_(size_memory_buffer)
  {
    CHECK_GT(num_samples_, 0);
    CHECK_GE(size_memory_buffer_, 0);

    if (in_memory_) {

    }
  }

  ~Reader() {}

  // return a pointer to N lines of data samples
  vector<std::string>* DataSamples();

 private:
  std::string filename_;    // identify the input file
  int num_samples_;         // how many data samples can be return
  bool in_memory_;          // whether load all the data into memory
  int size_memory_buffer_;  // the size of the memory buffer

  FILE* file_ptr_;                     // maintain current file pointer
  scoped_array<char> memory_buffer_;   // in-memory buffer
  
};

} // namespace f2m

#endif // F2M_READER_READER_H_
