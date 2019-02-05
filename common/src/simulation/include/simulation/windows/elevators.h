#pragma once

#include "Elevator.h"
#include "simulation/ui/window.h"
#include "simulation/physics_updater.h"
#include "simulation/components/encodersim.h"

namespace simulation {

class elevator_window : public ui::window, physics_aware {
 public:
  static void init();

  elevator_window(curtinfrc::ElevatorConfig *config);

  double get_motor_val();
  // Add to current encoder position, pos in metres
  void set_abs_encoder_pos(double pos);

  void update_physics(double dt) override;
  void render(cv::Mat &img) override;

 private:
  curtinfrc::ElevatorConfig *_config;
  std::shared_ptr<components::encoder_sim> _enc_sim;

  ui::button resetPos{ui::box{ 0, 0, 0.25, 0.05 }, "Reset", ui::colour{1, 0.75, 0.75}, ui::colour{1, 0.5, 0.5}};

  double _position = 0;
  double _velocity = 0;
  double _voltage = 0;
  double _current = 0;

  bool _limit_top = false;
  bool _limit_bottom = false;
};

}  // namespace simulation