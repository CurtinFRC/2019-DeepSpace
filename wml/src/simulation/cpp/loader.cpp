#include "simulation/loader.h"

#include <frc/Filesystem.h>

#include <wpi/Path.h>
#include <wpi/FileSystem.h>

#include <opencv2/highgui/highgui.hpp>

using namespace simulation;

cv::Mat resource_loader::load_img_resource(std::string name) {
  wpi::SmallString<256> full_path;
  frc::filesystem::GetLaunchDirectory(full_path);
  wpi::sys::path::append(full_path, "common", "src", "simulation", "resources");
  wpi::sys::path::append(full_path, name);

  if (!wpi::sys::fs::exists(full_path)) {
    throw std::runtime_error("File " + std::string(full_path.c_str()) + " does not exist!");
  }

  return cv::imread(std::string(full_path.c_str()), CV_LOAD_IMAGE_COLOR);
}