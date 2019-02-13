#pragma once

#include <opencv2/core/core.hpp>
#include <string>

namespace simulation {
class resource_loader {
 public:
  static cv::Mat load_img_resource(std::string name);
};
}  // namespace simulation