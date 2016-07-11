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
#include "src/common/stl-util.h"

namespace f2m {

Reader::Reader(const std::string& filename,
               int num_samples,
               bool in_memory)
  : filename_(filename),
    num_samples_(num_samples),
    in_memory_(in_memory)
{
  CHECK_GT(num_samples_, 0);

  data_samples_ = new StringList(num_samples_);

  // allocate memory for buffer
  if (in_memory_) {
    // get the size of current file
    fseek(file_ptr_, 0, SEEK_END);
    size_memory_buffer_ = ftell(file_ptr_);
    rewind(file_ptr_);
    try {
      memory_buffer_ = new char[size_memory_buffer_];
    } catch(std::bad_alloc&) {
      LOG(FATAL) << "Cannot allocate enough memory for Reader.";
    }
  }

  // open file and read all data into memory (if needed)
  file_ptr_ = OpenFileOrDie(filename_.c_str(), "r");
  if (in_memory_) {
    result = fread(memory_buffer_, 1, 
                   size_memory_buffer_, file_ptr_);
    if (result != size_memory_buffer_) {
      LOG(FATAL) << "Read file error.";
    }
  }
}

Reader::~Reader() {
  if (file_ptr_ != NULL) {
    fclose(file_ptr_);
    file_ptr_ = NULL;
  }

  delete memory_buffer_;

  delete data_samples_;
}

StringList* Reader::Samples() {
  return in_memory_ ? SampleFromMemory() : 
                      SampleFromDisk();
}

StringList* Reader::SampleFromDisk() {

}

StringList* Reader::SampleFromMemory() {
    
}

} // namespace f2m
