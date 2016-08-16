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

This file defines the base class Loss, which is the 
core part of ML algorithms.
*/

#ifndef F2M_LOSS_LOSS_H_
#define F2M_LOSS_LOSS_H_

#include <vector>
#include <cmath>  // for log() and exp()

#include "src/common/common.h"
#include "src/common/data_structure.h"

namespace f2m {

/* -----------------------------------------------------------------------------
 * The basic class of a loss function.                                          *
 * Loss is an abstract class,  which can be implemented by real loss functions  *
 * such as logistic regresiion loss (logit_loss.h), FM loss (fm_loss.h),        *
 * and FFM loss (ffm_loss.h).                                                   *
 * The Loss class can be used in both classification and regression tasks.      *
 * -----------------------------------------------------------------------------
 */

class Loss {
 public:
  virtual ~Loss() {}

  /* ---------------------------------------------------------------------------
   * Given the input data matrix and current model, return the prediction       *
   * results. Note that the prediction result is represented as a real          *
   * number (float point type) in both classification and                       *
   * regression problems.                                                       *
   * ---------------------------------------------------------------------------
   */

  virtual void Predict(const DataMatrix& matrix, 
                       const Model& param,
                       std::vector<int>* pred) = 0;

  /* ---------------------------------------------------------------------------
   * Given the input data matrix and current model, return                      *
   * the calculated gradients.                                                  *
   * ---------------------------------------------------------------------------
   */

  virtual void CalcGrad(const DataMatrix& matrix,
                        const Model& param,
                        SparseGrad* grad) = 0;

  /* ---------------------------------------------------------------------------
   * Given the prediction results and the true labels, return                   *
   * current loss value. Here we use LogLoss in default.                        *
   * Note that, in this LogLoss, the y is -1, 0, or 1.                          *
   * ---------------------------------------------------------------------------
   */

  virtual real_t Evaluate(const std::vector<int>& pred,
                          const std::vector<int>& label) const {
    real_t objv = 0.0;
    for (size_t i = 0; i < pred.size(); ++i) {
      // normalize y to -1 and 1.
      real_t y = label[i] > 0 ? 1 : -1;
      objv += log(1 + exp(- y * pred[i]));
    }
    return objv;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Loss);
};

} // namespace f2m

#endif // F2M_LOSS_LOSS_H_
