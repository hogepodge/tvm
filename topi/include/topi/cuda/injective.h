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
 * \file cuda/injective.h
 * \brief CUDA schedule for injective operations
 */
#ifndef TOPI_CUDA_INJECTIVE_H_
#define TOPI_CUDA_INJECTIVE_H_

#include "topi/tags.h"
#include "topi/detail/fuse.h"
#include "tvm/top/operation.h"
#include "tvm/build_module.h"

namespace topi {
using namespace tvm;
using namespace tvm::top;

namespace cuda {

/*!
 * \brief Updates an existing schedule for the given injective ops.
 *
 * \param sch The schedule to update.
 * \param out The tensor representing the injective op.
 *
 * \return The updated schedule.
 */
inline Schedule schedule_injective_from_existing(Schedule sch, const Tensor& out) {
  auto fused = detail::Fuse(sch[out], sch[out]->op.as<ComputeOpNode>()->axis);
  auto target = Target::Current(false);
  auto num_thread = target->max_num_threads;
  IterVar bx, tx;
  sch[out].split(fused, num_thread, &bx, &tx);
  sch[out].bind(bx, thread_axis(Range(), "blockIdx.x"));
  sch[out].bind(tx, thread_axis(Range(), "threadIdx.x"));
  return sch;
}

/*!
 * \brief Create a CUDA schedule for the given output tensors.
 *
 * \param target The target to generate a schedule for.
 * \param outs The output tensors.
 *
 * \return A schedule for the given ops.
 */
inline Schedule schedule_injective(const Target &target, const Array<Tensor>& outs) {
  Array<Operation> out_ops;
  for (auto t : outs) {
    out_ops.push_back(t->op);
  }
  auto s = create_schedule(out_ops);
  tvm::top::AutoInlineInjective(s);
  for (auto out : outs) {
    schedule_injective_from_existing(s, out);
  }
  return s;
}

}  // namespace cuda
}  // namespace topi
#endif  // TOPI_CUDA_INJECTIVE_H_
