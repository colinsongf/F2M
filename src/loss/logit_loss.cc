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

This file implements the logit_loss.h
*/

#include "src/loss/logit_loss.h"

#include "src/common/common.h"
#include "src/common/data_structure.h"
#include "src/common/linear_algebra.h"

/* ---------------------------------------------------------------------------
 * Given the input data matrix and current model, return the prediction 
 * results. Note that the prediction result is represented as a real 
 * number (float point type) in both classification and 
 * regression problems. Math: 
 *   
 *  [ pred += X * w ] 
 *
 * where 'pred' is the prediction vector, X is the input data matrix, 
 * w is current model parameter represented as a vector.
 * ---------------------------------------------------------------------------
 */

void LogitLoss::Predict(const DMatrix& data_matrix,
                        const DSVector& model_param,
                        DSVector* pred) {
  
}

/* ---------------------------------------------------------------------------
 * Given the input data matrix and current model, return 
 * the calculated gradients. Math: 
 *
 *  [ pred = X * w ]
 *  [ p = -y / (1 + exp(y * pred)) ]
 *  [ grad += X' * p ]
 *
 * where, 
 * ---------------------------------------------------------------------------
 */

void LogitLoss::CalcGrad(const DMatrix& data_matrix,
                         const DSVector& model_param,
                         DSVector* grad) {

}
