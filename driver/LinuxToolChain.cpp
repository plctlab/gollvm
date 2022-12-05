//===-- LinuxToolChain.cpp ------------------------------------------------===//
//
// Copyright 2018 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//
//
// Gollvm driver helper class LinuxToolChain methods.
//
//===----------------------------------------------------------------------===//

#include "LinuxToolChain.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include "CompileGo.h"
#include "DummyCompileC.h"
#include "IntegAssembler.h"
#include "Driver.h"
#include "GnuTools.h"
#include "Tool.h"
#include "ToolChain.h"

using namespace gollvm::driver;

namespace toolchains {

static void addIfPathExists(pathlist &paths, const llvm::Twine &path)
{
  if (llvm::sys::fs::exists(path))
    paths.push_back(path.str());
}

static llvm::StringRef getOSLibDir(const llvm::Triple &triple)
{
  // multilib is not supported on major aarch64/arm64 and riscv64 linux distributions
  // subject to change when more scenarios to be taken into account
  if (triple.getArch() == llvm::Triple::aarch64 ||
      triple.getArch() == llvm::Triple::riscv64)
    return "lib";
  // x86 uses the lib32 variant, unlike other archs.
  if (triple.getArch() == llvm::Triple::x86)
    return "lib32";
  return triple.isArch32Bit() ? "lib" : "lib64";
}

Linux::Linux(gollvm::driver::Driver &driver,
             const llvm::Triple &targetTriple)
    : ToolChain(driver, targetTriple),
      inspectFS_(gnutools::gccdetect::InspectRealFS()),
      gccDetector_(targetTriple,
                   driver.gccToolchainDir(),
                   driver.sysRoot(),
                   inspectFS_),
      distro_(distro::DetectDistro(inspectFS_, targetTriple))
{
  gccDetector_.init();

  // Program paths
  pathlist &ppaths = programPaths();
  auto ftrip = gccDetector_.foundTriple().str();
  addIfPathExists(ppaths, llvm::Twine(gccDetector_.getParentLibPath() +
                                      "/../../" + ftrip +
                                      "/bin").str());

  // File paths
  pathlist &fpaths = filePaths();
  addIfPathExists(fpaths, gccDetector_.getLibPath());
  std::string osLibDir = getOSLibDir(targetTriple).str();
  if (!driver.sysRoot().empty()) {
    addIfPathExists(fpaths, llvm::Twine(driver.sysRoot() + "/usr/"
                                        + osLibDir).str());
    osLibDir = driver.sysRoot() + "/" + osLibDir;
  }
  addIfPathExists(fpaths, llvm::Twine(gccDetector_.getParentLibPath() +
                                      "/../" + ftrip).str());
  addIfPathExists(fpaths, llvm::Twine(osLibDir).str());
  addIfPathExists(fpaths, llvm::Twine(osLibDir + "/" + ftrip).str());
  if (this->distro_ == distro::DistroArchLinux || this->distro_ == distro::DistroRedhat)
    addIfPathExists(fpaths, llvm::Twine("/usr/" + osLibDir).str());

  // Include program and file paths in verbose output.
  if (driver.args().hasArg(gollvm::options::OPT_v)) {
    llvm::errs() << "Candidate GCC install:\n" << gccDetector_.toString();

    llvm::errs() << "ProgramPaths:\n";
    for (auto &path : programPaths())
      llvm::errs() << path << "\n";
    llvm::errs() << "FilePaths:\n";
    for (auto &path : filePaths())
      llvm::errs() << path << "\n";
  }
}

Linux::~Linux()
{
}

Tool *Linux::buildCompiler()
{
  llvm::opt::Arg *xarg = driver().args().getLastArg(gollvm::options::OPT_x);
  if (xarg != nullptr && llvm::StringRef(xarg->getValue()).equals("c")) {
    // This is a dummy C compile.
    return new DummyCompileC(*this, driver().executablePath());
  }
  return new CompileGo(*this, driver().executablePath());
}

Tool *Linux::buildAssembler()
{
  if (driver().useIntegratedAssembler())
    return new IntegAssembler(*this, driver().executablePath());
  else
    return new gnutools::Assembler(*this);
}

Tool *Linux::buildLinker()
{
  return new gnutools::Linker(*this);
}

std::string Linux::getDynamicLinker(const llvm::opt::ArgList &args)
{
  const llvm::Triple::ArchType arch = triple().getArch();

  std::string LibDir;
  std::string Loader;

  switch (arch) {
    default:
      assert(false && "unsupported architecture");
      return "<unknown_dynamic_linker>";
    case llvm::Triple::aarch64:
      LibDir = "lib";
      Loader = "ld-linux-aarch64.so.1";
      break;
    case llvm::Triple::x86:
      LibDir = "lib";
      Loader = "ld-linux.so.2";
      break;
    case llvm::Triple::x86_64: {
      LibDir = "lib64";
      Loader = "ld-linux-x86-64.so.2";
      break;
    }
  }
  if (auto *Arg = args.getLastArg(gollvm::options::OPT_sysroot_EQ)) {
    std::string Sysroot = Arg->getValue();
    return Sysroot + "/" + LibDir + "/" + Loader;
  }
  return "/" + LibDir + "/" + Loader;
}

} // end namespace toolchains
