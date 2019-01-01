#pragma once

#include "simulation/ui/window.h"

namespace simulation {

class motor_window : public ui::window {
 public:
  motor_window();

  void render(cv::Mat &img) override;

  void render_motor(cv::Mat &img, double yorigin, std::string type, int index, bool initialized, double val, double min = 0, double max = 100, std::string unit = "%", int precision = 0);
  void render_motor(cv::Mat &img, double yorigin, std::string type, int index, std::string text);

 private:
};

}  // namespace simulation