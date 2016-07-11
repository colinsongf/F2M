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
#include "src/reader/reader.h"

#include "src/common/common.h"
#include "src/common/file_utils.h"

namespace f2m {

Reader::Reader(const std::string& filename,
               int num_samples,
               bool in_memory,
               int size_memory_buffer)
  : filename_(filename),
    num_samples_(num_samples),
    in_memory_(in_memory),
    size_memory_buffer_(size_memory_buffer) 
{
  CHECK_GT(num_samples_, 0);
  CHECK_GE(size_memory_buffer_, 0);

  data_samples_ = new Samples(num_samples_);

  // allocate memory for buffer
  if (in_memory_) {
    try {
      memory_buffer_ = new char[size_memory_buffer_];
    } catch(std::bad_alloc&) {
      LOG(FATAL) << "Cannot allocate enough memory for Reader.";
    }
  }

  // open file and read all data into memory (if needed)
  file_ptr_ = OpenFileOrDie(filename_.c_str(), "r");
  if (in_memory_) {
      
  }
}

Reader::~Reader() {
  if (file_ptr_ != NULL) {
    fclose(file_ptr_);
    file_ptr_ = NULL;
  }

  delete data_samples_;
}

Samples* Reader::Samples() {
  return in_memory_ ? SampleFromMemory() : 
                      SampleFromDisk();
}

Samples* Reader::SampleFromDisk() {

}

Samples* Reader::SampleFromMemory() {
    
}

} // namespace f2m
