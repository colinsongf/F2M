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

This file defines the FFM loss.
*/

#ifndef F2M_LOSS_FFM_LOSS_H_
#define F2M_LOSS_FFM_LOSS_H_

#include <vector>

#include "src/common/common.h"
#include "src/common/data_structure.h"

#include "src/loss/loss.h"

namespace f2m {

class FFMLoss : public Loss {
 public:
  ~FFMLoss() {}

  void Predict(const DataMatrix& matrix,
               const Model& param,
               std::vector<real_t>* pred);

  void CalcGrad(const DataMatrix& matrix,
 	            const Model& param,
 	            SparseGrad* grad);

 private:
  DISALLOW_COPY_AND_ASSIGN(FMLoss);
};

} // namespace f2m

#endif // F2M_LOSS_FFM_LOSS_H_