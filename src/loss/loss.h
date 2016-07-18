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
// There are 3 important functions in a loss class:
/* 
 *  1. virtual void Predict(const StringList* row_data,
 *                          const CArrayList* model_param,
 *                          FloatList* pred_results) = 0;
 *
 *  2. virtual void CalcGrad(const StringList* row_data,
 *                           const CArrayList* model_param,
 *                           FloatList* grad) = 0;
 *
 *  3. virtual float Evaluate(const FloatList* pred_results,
 *                            const FloatList* labels);
 */ 
// User can implement different Predict, CalcGrad, and Evaluate functions 
// in their derived classes by using different algorithms. 
//
// The Loss class can be used in both classification and regression problems.
//
#ifndef F2M_LOSS_LOSS_H_
#define F2M_LOSS_LOSS_H_

#include <vector>
#include <string>
#include <cmath>

#include "src/common/carray.h"
#include "src/common/common.h"

namespace f2m {

typedef std::vector<std::string> StringList;     // To store the training and testing data.
typedef CArray<float> FloatList;                 // float is sufficient enough for ML problems.
typedef vector< CArray<float> > CArrayList;      // To handle model of LR, FM, as well as FFM.

class Loss {
 public:
  virtual ~Loss() {}

  // Given the testing data records, return their prediction result.
  // Note that the prediction result is represented as a float number.
  virtual void Predict(const StringList* row_data,
                       const CArrayList* model_param,
                       FloatList* pred_results) = 0;

  // Given the traning data records, return the gradients.
  virtual void CalcGrad(const StringList* row_data,
                        const CArrayList* model_param,
                        FloatList* grad) = 0;

  // Given the prediction results and the true labels,
  // return current loss value. Here we use LogLoss in default.
  virtual float Evaluate(const FloatList* pred_results,
                         const FloatList* labels) const {
    float objv = 0.0;
    for (size_t i = 0; i < pred_results.size(); ++i) {
      float y = label[i] > 0 ? 1 : -1;
      objv += log(1 + exp(- y * pred_results[i]));
    }
    return objv;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Loss);
};

} // namespace f2m

#endif // F2M_LOSS_LOSS_H_
