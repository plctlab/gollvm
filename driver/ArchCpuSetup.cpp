//===-- ArchCpuSetup.cpp --------------------------------------------------===//
//
// Copyright 2018 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//
//
// Gollvm driver helper function setupArchCpu
//
//===----------------------------------------------------------------------===//

#include "ArchCpuSetup.h"

#include "llvm/Option/Arg.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

namespace gollvm { namespace arch {
#include "ArchCpusAttrs.h"
} }

using namespace llvm;

bool gollvm::driver::setupArchCpu(opt::Arg *cpuarg, std::string &cpu,
                               std::string &attrs, Triple triple,
                               const char *progname) {
  std::string cpuStr;
  if (cpuarg != nullptr) {
    std::string val(cpuarg->getValue());
    if (val == "native")
      cpuStr = sys::getHostCPUName().str();
    else
      cpuStr = cpuarg->getValue();
  }

  // Locate correct entry in architectures table for this triple
  const gollvm::arch::CpuAttrs *cpuAttrs = nullptr;
  for (unsigned i = 0; gollvm::arch::triples[i].cpuattrs != nullptr; i += 1) {
    if (!strcmp(triple.str().c_str(), gollvm::arch::triples[i].triple)) {
      cpuAttrs = gollvm::arch::triples[i].cpuattrs;
      break;
    }
  }
  if (cpuAttrs == nullptr) {
    errs() << progname << ": unable to determine target CPU features for "
           << "target " << triple.str() << "\n";
    return false;
  }

  // If no CPU specified, use first entry. Otherwise look for CPU name.
  if (!cpuStr.empty()) {
    bool found = false;
    while (strlen(cpuAttrs->cpu) != 0) {
      if (!strcmp(cpuAttrs->cpu, cpuStr.c_str())) {
        // found
        found = true;
        break;
      }
      cpuAttrs++;
    }
    if (!found) {
      errs() << progname << ": invalid setting for -march:"
             << " -- unable to identify CPU '" << cpuStr << "'\n";
      return false;
    }
  }
  cpu = cpuAttrs->cpu;
  attrs = cpuAttrs->attrs;
  return true;
}
