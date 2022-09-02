//===-- CallingConv.h -----------------------------------------------------===//
//
// Copyright 2022 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//
//
// Defines the CallingConvId type.
//
//===----------------------------------------------------------------------===//

#ifndef GOLLVM_DRIVER_CALLINGCONV_H
#define GOLLVM_DRIVER_CALLINGCONV_H

namespace gollvm {
namespace driver {

// Used inplace of llvm::CallingConv::Id which does not record arch and cpu
enum class CallingConvId { X86_64_SysV, ARM_AAPCS, RISCV64_C, MaxID };

} // end namespace driver
} // end namespace gollvm

#endif // GOLLVM_DRIVER_CALLINGCONV_H
