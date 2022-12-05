//===-- DummyCompileC.h ----------------------------------------------------===//
//
// Copyright 2022 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//
//
// Defines the DummyCompileC class (helper for driver functionality).
//
//===----------------------------------------------------------------------===//

#ifndef GOLLVM_DRIVER_DUMMYCOMPILEC_H
#define GOLLVM_DRIVER_DUMMYCOMPILEC_H

#include "Tool.h"

namespace gollvm {
namespace driver {

class ToolChain;
class Compilation;
class Action;
class Artifact;

// Concrete (dummy) C compiler tool. This tool is used by the driver to carry
// out "compile" actions when "-x c" is passed, e.g. "compile this set of C
// files into assembly". Since gollvm can't compile C, we require
// that the input file be empty, and don't produce any output.

class DummyCompileC : public InternalTool {
 public:
  DummyCompileC(ToolChain &tc, const std::string &executablePath);
  ~DummyCompileC();

  // Perform compilation.
  bool performAction(Compilation &compilation,
                     const Action &jobAction,
                     const ArtifactList &inputArtifacts,
                     const Artifact &output) override;
};

} // end namespace driver
} // end namespace gollvm

#endif // GOLLVM_DRIVER_DUMMYCOMPILEC_H
