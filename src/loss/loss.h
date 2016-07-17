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
// The basic class of a loss function.
// Loss is an abstract class, which can be implemented by real 
// loss functions such as logistic regresiion loss (logit_loss.h),
// FM loss (fm_loss.h), and FFM loss (ffm_loss.h).
//
// There are 2 important functions in a loss class:
/* 
 *  typedef vector<string> StringList;
 *  typedef CArray<float> FloatList;
 *  typedef vector< CArray<float> > CArrayList;
 *
 *  1. void Predict(const StringList* row_data,
 *                  const CArrayList* model_param,
 *                  FloatList* pred_results) = 0;
 *
 *  2. void CalcGrad(const StringList* row_data,
 *                   const CArrayList* model_param,
 *                   FloatList* grad) = 0;
 */
// User can implement different Predict and CalcGrad functions in their
// derived classes by using different loss functions.
//
#ifndef F2M_LOSS_LOSS_H_
#define F2M_LOSS_LOSS_H_

namespace f2m {

class Loss {

};

} // namespace f2m

#endif // F2M_LOSS_LOSS_H_
