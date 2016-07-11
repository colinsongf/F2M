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
    Reader reader(file_name = "/tmp/testdata", 
                  num_samples = 100, 
                  in_memory = true or false); 

    Loop until converge {

       Data = reader.Samples(); // return N data samples

       ... use the data samples
       
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
// of your main memory is big enough for current training task.
//
// Reader is an algorithm-agnostic class and can mask the details of
// the data source (on disk or in memory), and it is flexible for 
// different gradient descent methods (e.g., SGD, mini-batch GD, and 
// batch GD)
//
#ifndef F2M_READER_READER_H_
#define F2M_READER_READER_H_

#include <string>
#include <vector>

namespace f2m {
    
class Reader {
 public:
  Reader(std::string filename,
         int num_samples,
         bool in_memory = false)
    : filename_(filename),
      num_samples_(num_samples)
    in_memory_(in_memory) {
        
  }

  ~Reader() {}

 // return N samples
  vector<std::string*> DataSamples();

 private:
  std::string filename_;
  int num_samples_;
  bool in_memory_;
};

} // namespace f2m

#endif // F2M_READER_READER_H_
