#include "Files.h"

#include <frc/RobotBase.h>
#include <frc/Filesystem.h>

#include <wpi/FileSystem.h>
#include <wpi/Path.h>

#include <cstdarg>

using namespace wml;

std::string files::GetDeployDirectory(std::string project) {
  // GradleRIO launches sim in the project root. This project is a bit special so 
  // we have to do it ourself.
  wpi::SmallString<256> result;
  if (frc::RobotBase::IsReal()) {
    frc::filesystem::GetDeployDirectory(result);
    return std::string(result.c_str());
  } else {
    frc::filesystem::GetLaunchDirectory(result);
    wpi::sys::path::append(result, project, "src", "main", "deploy");
    return std::string(result.c_str());
  }
}
