#pragma once

#include <wpi/SmallVector.h>
#include <wpi/FileSystem.h>
#include <wpi/Path.h>

#include <string>

namespace wml {
namespace files {
  std::string GetDeployDirectory(std::string project);
}
}  // namespace wml
