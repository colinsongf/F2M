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

This file is the implementation of reader.h
*/

#include "src/reader/reader.h"

#include <string.h>

#include "src/common/common.h"

namespace f2m {

const int kDefaultMaxSizeLine = 100 * 1024; // 100 KB one line

/* Constructor */

Reader::Reader(const std::string& filename,
               int num_samples,
               bool in_memory)
  : filename_(filename),
    num_samples_(num_samples),
    in_memory_(in_memory) {

  CHECK_GT(num_samples_, 0);

  data_samples_ = new StringList(num_samples_);

  file_ptr_ = OpenFileOrDie(filename_.c_str(), "r");

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
  } else { // do not use memory buffer
    memory_buffer_ = NULL;
  }

  // read all data into memory (if needed)
  if (in_memory_) {
    uint64 result = fread(memory_buffer_, 1, 
                          size_memory_buffer_, file_ptr_);
    if (result != size_memory_buffer_) {
      LOG(FATAL) << "Read file error.";
    }
  }
}

/* Destructor */

Reader::~Reader() {
  if (file_ptr_ != NULL) {
    fclose(file_ptr_);
    file_ptr_ = NULL;
  }

  if (memory_buffer_ != NULL) {
    delete memory_buffer_;
  }

  delete data_samples_;
  
}

StringList* Reader::Samples() {
  return in_memory_ ? SampleFromMemory() : 
                      SampleFromDisk();
}

/* Sample data from disk files. */

StringList* Reader::SampleFromDisk() {
  static char* line = new char[kDefaultMaxSizeLine];
  // read num_samples_ lines of data from disk file.
  for (int i = 0; i < num_samples_; ++i) {
    if (fgets(line, kDefaultMaxSizeLine, file_ptr_) == NULL) {
      // Either ferror or feof. Anyway, 
      // return to the start of the file.
      fseek(file_ptr_, 0, SEEK_SET);
      i--; // re-read
      continue;
    }
    int read_size = strlen(line);
    if (line[read_size - 1] != '\n') {
      LOG(FATAL) << "Encountered a too-long line..";
    } else {
      line[read_size - 1] = '\0';
      // Handle some windows text format.
      if (read_size > 1 && line[read_size - 2] == '\r') { 
        line[read_size - 2] = '\0';
      }
    }
    (*data_samples_)[i].assign(line);
  }
  return data_samples_;
}

/* Read one line from a memory buffer.
   Used by Reader::SampleFromMemory() */

int ReadLineFromMemory(char* line, char* buf, uint64 buf_len) {
  static uint64 start_position = 0;
  static uint64 end_position = 0;
  // End of the buffer, return to the head
  if (end_position >= buf_len) {
    start_position = 0;
    end_position = 0;
  }
  // Read one line
  while (*(buf + end_position) != '\n') { ++end_position; }
  int read_size = end_position - start_position + 1;
  if (read_size > kDefaultMaxSizeLine) {
    LOG(FATAL) << "Encountered a too-long line..";   
  }
  memcpy(line, buf + start_position, read_size);
  start_position = ++end_position;
  return read_size;
}

/* Sample data from a memory buffer. */

StringList* Reader::SampleFromMemory() {
  static char* line = new char[kDefaultMaxSizeLine];
  // read num_samples_ lines of data from memory
  for (int i = 0; i < num_samples_; ++i) {
    int read_size = ReadLineFromMemory(line, memory_buffer_, 
                                       size_memory_buffer_);
    line[read_size - 1] = '\0';
    // Handle some windows text format.
    if (read_size > 1 && line[read_size - 2] == '\r') { 
      line[read_size - 2] = '\0';
    }
    (*data_samples_)[i].assign(line);
  }
  return data_samples_; 
}

} // namespace f2m
