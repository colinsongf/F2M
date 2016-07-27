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
  /* The matrix should be pre-initialized.
   */
  virtual static void Parse(const StringList* list, DMatrix* matrix) {
  	CHECK_EQ(list.size(), matrix->size());
  	/* We use static variable here to avoid 
  	 * construct and de-construct overhead.*/
  	static StringList items, single_item;
  	for (int i = 0; i < list.size(); ++i) {
  	  // parse the following format of one line:
  	  // [0:1234 1:0.123 2:0.21 3:1 4:1 5:0.05]
      SplitStringUsing((*list)[i], "\t", &items);
      // parse every single items. 
      for (int n = 0; n < items.size(); ++n) {
      	SplitStringUsing(items[n], ":", single_item);

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
  virtual static void Parse(const StringList* list, DMatrix* matrix) {

  }
};

} // namespace f2m

#endif // F2M_READER_PARSER_H_