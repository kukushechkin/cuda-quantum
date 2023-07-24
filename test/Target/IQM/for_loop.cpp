/*******************************************************************************
 * Copyright (c) 2022 - 2023 NVIDIA Corporation & Affiliates.                  *
 * All rights reserved.                                                        *
 *                                                                             *
 * This source code and the accompanying materials are made available under    *
 * the terms of the Apache License 2.0 which accompanies this distribution.    *
 ******************************************************************************/

#include <cudaq.h>

// RUN: nvq++ %s --target iqm --emulate --iqm-qpu-architecture Apollo -o %t.x && %t.x | FileCheck %s
// XFAIL: *

template <std::size_t N>
struct ghz {
  auto operator()() __qpu__ {
    cudaq::qreg<N> q;
    h(q[0]);

    // FIXME: an std::runtime_error exception when using --emulate
    // loc("for_loop.cpp":17:5): error: failed to legalize operation 'quake.phased_rx'
    for (int i = 0; i < N - 1; i++) {
      x<cudaq::ctrl>(q[i], q[i + 1]);
    }

    mz(q[0]);
  }
};

int main() {

  auto kernel = ghz<2>{};
  auto counts = cudaq::sample(kernel);
  counts.dump();
  return 0;
}

// CHECK: { 0:{{[0-9]+}}, 1:{{[0-9]+}} }