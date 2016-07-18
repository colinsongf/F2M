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
// Loss class for Logistic Regression.
//
// For Logistic Regression, we use LogLoss for classification problems.
//
#ifndef F2M_LOSS_LOGIT_LOSS_H_
#define F2M_LOSS_LOGIT_LOSS_H_

#include <vector>
#include <string>

#include "src/common/carray.h"
#include "src/common/common.h"

#include "src/loss/loss.h"

namespace f2m {

class LogitLoss : public Loss {
 public:
  LogitLoss() {}
  ~Logistic() {}

  // Given the testing data records, return the prediction results.
  // Note that, for LR, the model_param has only one element, i.e.,
  // model_param[0] is the real parameter vector.
  void Predict(const StringList* row_data,
               const CArrayList* model_param,
               FloatList* pred_results);

  // Given the training data records, return the gradients.
  void CalcGrad(const StringList* row_data,
                const CArrayList* model_param,
                FloatList* grad);

 private:
  
};

} // namespace f2m

#endif // F2M_LOSS_LOGIT_LOSS_H_
