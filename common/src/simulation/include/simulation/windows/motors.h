#pragma once

#include "simulation/ui/window.h"

namespace simulation {

class motor_window : public ui::window {
 public:
  motor_window();

  void render(cv::Mat &img) override;

  void render_motor(cv::Mat &img, double yorigin, std::string type, int index, bool initialized, double speed);

 private:
};

}  // namespace simulation