//===-- ArchCpuSetup.cpp --------------------------------------------------===//
//
// Copyright 2018 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//
//
// Declares gollvm driver helper function setupArchCpu.
//
//===----------------------------------------------------------------------===//

#ifndef GOLLVM_DRIVER_ARCHCPUSETUP_H
#define GOLLVM_DRIVER_ARCHCPUSETUP_H

#include "Tool.h"

namespace gollvm {
namespace driver {

bool setupArchCpu(llvm::opt::Arg *cpuarg, std::string &cpu, std::string &attrs,
               llvm::Triple triple_, const char *progname_);

} // end namespace driver
} // end namespace gollvm

#endif
