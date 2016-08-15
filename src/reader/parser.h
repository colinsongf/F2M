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

This files defines Parser class, which parse the Reader's 
output to a DMatrix.
*/

#ifndef F2M_READER_PARSER_H_
#define F2M_READER_PARSER_H_

#include <vector>
#include <string>

#include "src/common/common.h"
#include "src/common/data_structure.h"

namespace f2m {

typedef std::vector<std::string> StringList;

/* -----------------------------------------------------------------------------
 * The Parser class parse a set of StringList to a DataMatrix.                  * 
 * In default, the base Parse class parse the StringList to LR and FM format,   *
 * and we can implement different inhert classes to parser data for other       *
 * algorithms such as the FFMParser.                                            *
 * -----------------------------------------------------------------------------
 */

class Parser {
 public:
  /* The matrix should be pre-initialized with the
     the same row size of the StringList. */

  virtual void Parse(const StringList* list, DataMatrix* matrix) {
    CHECK_EQ(list->size(), matrix->size());
    for (int i = 0; i < list->size(); ++i) {
      // parse the following format:
      // [0:1234 1:0.123 2:0.21 3:1 4:1 5:0.05 0]
      StringList items;
      SplitStringUsing((*list)[i], "\t", &items);
      int item_size = items.size();
      // allocate memory for every single line.
      (*matrix)[i].size = item_size - 1;
      (*matrix)[i].x.reset(new real_t[item_size - 1]);
      (*matrix)[i].position.reset(new index_t[item_size - 1]);
      // parse every single items. 
      for (int n = 0; n < item_size; ++n) {
        char* ch_ptr = const_cast<char*>(items[n].c_str());
        // the last element is y.
      	if (n == item_size - 1) {
      	  float value = atof(ch_ptr);
      	  if (value != 0.0 && value != 1.0 && value != -1.0) {
      	  	LOG(FATAL) << "Error of Y value: " << value;
      	  }
      	  (*matrix)[i].y = value;
      	  break;
      	}
        // find the ':' position.
        int pos = 0;
        while (ch_ptr[pos] != ':' && pos < item_size - 1) { ++pos; }
        if (pos >= item_size - 1) {
          LOG(FATAL) << "Input data format error, the item is: "
                     << items[n];
        }
        // get index and value
        ch_ptr[pos] = '\0';
        int index = atoi(ch_ptr);
        float value = atof(ch_ptr + pos + 1);
        // add index and value to RowData.
        *((*matrix)[i].x.get() + n) = value;
        *((*matrix)[i].position.get() + n) = index;
      }
    }
  }
};

/* -----------------------------------------------------------------------------
 * FFMParser parse the StringList to a DataMatrix of FFM format.                *
 * -----------------------------------------------------------------------------
 */

class FFMParser : public Parser {
 public:
  /* The matrix should be pre-initialized with the
     the same size of the StringList. */

  virtual void Parse(const StringList* list, DataMatrix* matrix) {
    CHECK_EQ(list->size(), matrix->size());
    for (int i = 0; i < list->size(); ++i) {
      // parse the following format:
      // [1:1:1 2:2:1 3:3:1 3:4:1 4:5:0.999 1]
      StringList items;
      SplitStringUsing((*list)[i], "\t", &items);
      int item_size = items.size();
      // allocate memory for SparseRow
      (*matrix)[i].size = item_size - 1;
      (*matrix)[i].x.reset(new real_t[item_size - 1]);
      (*matrix)[i].position.reset(new index_t[item_size - 1]);
      (*matrix)[i].field.reset(new int[item_size - 1]);
      // parse every single items.
      for (int n = 0; n < items.size(); ++n) {
        char* ch_ptr = const_cast<char*>(items[n].c_str());
        // the last element is y.
        if (n == item_size - 1) {
          float value = atof(ch_ptr);
          if (value != 0.0 && value != 1.0 && value != -1.0) {
      	  	LOG(FATAL) << "Error of Y value: " << value;
      	  }
          (*matrix)[i].y = value;
          break;
        }
        // find the first ':' position.
        int pos = 0;
        while (ch_ptr[pos] != ':' && pos < item_size - 1) { ++pos; }
        if (pos >= item_size - 1) {
          LOG(FATAL) << "Input data format error, the item is: "
                     << items[n];
        }
        int pos_1 = pos++;
        // find the second ':' position.
        while (ch_ptr[pos] != ':' && pos < item_size - 1) { ++pos; }
        if (pos >= item_size - 1) {
          LOG(FATAL) << "Input data format error, the item is: "
                     << items[n];
        }
        int pos_2 = pos;
        ch_ptr[pos_1] = '\0';
        ch_ptr[pos_2] = '\0';
        // get field, index, and value.
        int field = atoi(ch_ptr);
        int index = atoi(ch_ptr + pos_1 + 1);
        float value = atof(ch_ptr + pos_2 + 1);
        // set RowData.
        *((*matrix)[i].x.get() + n) = value;
        *((*matrix)[i].position.get() + n) = index;
        *((*matrix)[i].field.get() + n) = field;       
      }
    }
  }
};

} // namespace f2m

#endif // F2M_READER_PARSER_H_