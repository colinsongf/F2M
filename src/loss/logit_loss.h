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

This file defines the logistic regression loss.
*/

#ifndef F2M_LOSS_LOGIT_LOSS_H_
#define F2M_LOSS_LOGIT_LOSS_H_

#include <vector>

#include "src/common/common.h"
#include "src/common/data_structure.h"

#include "src/loss/loss.h"

namespace f2m {

/* -----------------------------------------------------------------------------
 * Logistic Regression Loss, Math:                                              *
 *                                                                              *
 *  [ loss(x, y, w) = log(1 + exp(-y * <w, x>)) ]                               *
 *                                                                              *
 * -----------------------------------------------------------------------------
 */

class LogitLoss : public Loss {
 public:
  ~LogitLoss() {}

  /* ---------------------------------------------------------------------------
   * Given the input data matrix and current model, return the prediction       *
   * results. Math:                                                             *
   *                                                                            *
   *  [ pred = X * w ]                                                          *
   *                                                                            *
   * where 'pred' is the prediction vector, X is the input sparse data matrix,  *
   * w is current model parameter represented as a dense vector.                *
   * ---------------------------------------------------------------------------
   */

  void Predict(const DataMatrix& matrix,
               const Model& param,
               std::vector<real_t>* pred) {

  }

  /* ---------------------------------------------------------------------------
   * Given the input data matrix and current model, return                      *
   * the calculated gradients. Math:                                            *
   *                                                                            *
   *  [ pred = X * w ]                                                          *
   *                                                                            *
   * where 'pred' is the prediction vector, X is input sparse data matarix,     *
   * w is current model paramter represented as a dense vector.                 *
   *                                                                            *
   * for every single value of the pred vector,                                 *
   *                                                                            *
   *  [ p[i] = -y / (1 + exp(y * pred[i])) ]                                    *
   *                                                                            *
   * where y is current label, p is partial gradients.                          *
   *                                                                            *
   * Next, we can calculate the final grandients.                               *
   * For every single line of the data matrix,                                  *
   *                                                                            *
   *  [ grad += X[i] * p[i] ]                                                   *
   *                                                                            *
   * where n is the row number of the data matrix X.                            *
   * ---------------------------------------------------------------------------
   */

  void CalcGrad(const DataMatrix& matrix,
                const Model& param,
                SparseGrad* grad) {
    
  }

 private: 
  DISALLOW_COPY_AND_ASSIGN(LogitLoss);
};

} // namespace f2m

#endif // F2M_LOSS_LOGIT_LOSS_H_
