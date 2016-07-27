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
 * The Parser class parse the StringList (Reader's output) to a DMatrix.
 *
 * In default, the Parse class parse the StringList to LR and FFM format.
 * -----------------------------------------------------------------------------
 */
class Parser {
 public:
  /* The matrix should be pre-initialized with the
   * the same size of the StringList.
   */
  virtual static void Parse(const StringList* list, DMatrix* matrix) {
    CHECK_EQ(list.size(), matrix->size());
    for (int i = 0; i < list.size(); ++i) {
      // parse the following format of one line:
      // [0:1234 1:0.123 2:0.21 3:1 4:1 5:0.05 0]
      StringList items;
      SplitStringUsing((*list)[i], "\t", &items);
      // parse every single items. 
      int item_size = items[i].size();
      for (int n = 0; n < item_size; ++n) {
        char* ch_ptr = const_cast<char*>(items[n].c_str());
        // the last element is y.
      	if (n == item_size - 1) {
      	  float value = atof(ch_ptr);
      	  (*matrix)[i].feature_value_.push_back(value);
      	  break;
      	}
        // find the ':' position.
        int pos = 0;
        while (ch_ptr[pos] != ':') { ++pos; }
        if (pos >= item_size - 1) {
          LOG(FATAL) << "Input data format error: "
                     << items[i];
        }
        // get index and value
        ch_ptr[pos] = '\0';
        int index = atoi(ch_ptr)
        float value = atof(ch_ptr + pos + 1);
        // add index and value to RowData.
        (*matrix)[i].feature_value_.push_back(value);
        (*matrix)[i].feature_index_.push_back(index);
      }
    }
  }
};

/* -----------------------------------------------------------------------------
 * FFMParser parse the StringList to a DMatrix of FFM format.
 * -----------------------------------------------------------------------------
 */
class FFMParser : public Parser {
 public:
  /* The matrix should be pre-initialized with the
   * the same size of the StringList.
   */
  virtual static void Parse(const StringList* list, DMatrix* matrix) {
    CHECK_EQ(list.size(), matrix->size());
    for (int i = 0; i < list.size(); ++i) {
      // parse the following format of one line:
      // [1:1:1 2:2:1 3:3:1 3:4:1 4:5:0.999 1]
      StringList items;
      SplitStringUsing((*list)[i], "\t", &items);
      // parse every single items.
      int item_size = items[i].size();
      for (int n = 0; n < items.size(); ++n) {
        char* ch_ptr = const_cast<char*>(items[n].c_str());
        // the last element is y.
        if (n == item_size - 1) {
          float value = atof(ch_ptr);
          (*matrix)[i].feature_value_.push_back(value);
          break;
        }
        // find the first ':' position.
        int pos = 0;
        while (ch_ptr[pos] != ':') { ++pos; }
        if (pos >= item_size - 1) {
          LOG(FATAL) << "Input data format error: "
                     << items[i];
        }
        int pos_1 = pos++;
        // find the second ':' position.
        while (ch_ptr[pos] != ':') { ++pos; }
        if (pos >= item_size - 1) {
          LOG(FATAL) << "Input data format error: "
                     << items[i];
        }
        int pos_2 = pos;
        ch_ptr[pos_1] = '\0';
        ch_ptr[pos_2] = '\0';
        // get field, index, and value.
        int field = atoi(ch_ptr);
        int index = atoi(ch_ptr + pos_1 + 1);
        float value = atof(ch_ptr + pos_2 + 1);
        // set RowData.
        (*matrix)[i].feature_value_.push_back(value);
        (*matrix)[i].feature_index_.push_back(index);
        (*matrix)[i].field_.push_back(field);
      }
    }
  }
};

} // namespace f2m

#endif // F2M_READER_PARSER_H_