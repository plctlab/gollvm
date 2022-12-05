//===-- DummyCompileC.cpp -------------------------------------------------===//
//
// Copyright 2022 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//
//
// Gollvm driver helper class "DummyCompileC" methods.
//
//===----------------------------------------------------------------------===//

#include "DummyCompileC.h"

#include "Action.h"
#include "ArchCpuSetup.h"
#include "Artifact.h"
#include "Driver.h"
#include "ToolChain.h"
#include <iostream>

//......................................................................

namespace gollvm {
namespace driver {

DummyCompileC::DummyCompileC(ToolChain &tc, const std::string &executablePath)
    : InternalTool("dummyCcompiler", tc, executablePath)
{
}

DummyCompileC::~DummyCompileC()
{
}

bool DummyCompileC::performAction(Compilation &compilation,
                                  const Action &jobAction,
                                  const ArtifactList &inputArtifacts,
                                  const Artifact &output)
{
  std::cout << "no support for C compilation\n";
  return true;
}

} // end namespace driver
} // end namespace gollvm
