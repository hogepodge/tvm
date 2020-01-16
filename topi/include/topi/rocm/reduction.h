/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \file rocm/reduction.h
 * \brief rocm schedule for reduction operations
 */
#ifndef TOPI_ROCM_REDUCTION_H_
#define TOPI_ROCM_REDUCTION_H_

#include "topi/tags.h"
#include "topi/detail/fuse.h"
#include "tvm/top/operation.h"
#include "tvm/build_module.h"

#include "topi/cuda/reduction.h"

namespace topi {
using namespace tvm;
using namespace tvm::top;

namespace rocm {
/*!
* \brief Create a rocm schedule for a reduce operation.
*
* \param target The target to generate a schedule for.
* \param outs The output tensors.
*
* \return A schedule for the given ops.
*/
Schedule schedule_reduce(const Target& target, Array<Tensor> outs) {
  return topi::cuda::schedule_reduce(target, outs);
}

}  // namespace rocm
}  // namespace topi
#endif  // TOPI_ROCM_REDUCTION_H_
