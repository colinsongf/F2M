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
//  Loss is an abstract class, which will be implemented
//  by ``real" loss class such as logistic loss, FM loss, and
//  FFM loss.
//
//  For a Loss class, we focus 3 core functions:
//
//    1. LogLoss function:  LogLoss (w, x, y)
//    2. Hypothesis function: H (w, x)
//    3. Gradient function: CalGrad (w, x, y)
//
#ifndef F2M_LOSS_LOSS_H_
#define F2M_LOSS_LOSS_H_

namespace f2m {

class Loss {
 public:
  virtual ~Loss() {};

 // For classification problems, we usually use the LogLoss and a
 // L1 or L2 regularizer as the objective function. The objective function
 // is to minmize the sum of these two terms.
 //
 // Here, the LogLoss() only return a result of one training data record.
 //
 // To calcualte the loss on the whole trainning data set, we can use this 
 // function in this way:
 // 
 /*
  *
  *
  */
 //
 virtual float LogLoss(const Model* w, const Item* x, const int y) = 0;

 //
 virtual float Hypothesis() = 0;

 virtual float CalGrad() = 0;

};

} // namespace f2m

#endif // F2M_LOSS_LOSS_H_
