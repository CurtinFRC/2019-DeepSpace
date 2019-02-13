#pragma once

#include "Drivetrain.h"
#include "simulation/components/encodersim.h"
#include "simulation/components/gyrosim.h"
#include "simulation/physics_updater.h"
#include "simulation/ui/window.h"

namespace simulation {

struct wheel_state {
  double angular_position = 0;  // radians
  double angular_vel      = 0;  // radians / sec
};

class drivetrain_window : public ui::window, physics_aware {
 public:
  static void init();

  drivetrain_window(curtinfrc::DrivetrainConfig *config);

  double get_motor_val(bool left);
  void   update_encoder(bool left, double pos, double vel);

  void update_physics(double dt) override;
  void render(cv::Mat &img) override;

  void draw_robot(cv::Mat &img);

 private:
  curtinfrc::DrivetrainConfig *            _config;
  std::shared_ptr<components::encoder_sim> _enc_sim_left;
  std::shared_ptr<components::encoder_sim> _enc_sim_right;
  std::shared_ptr<components::gyro_sim>    _gyro_sim;

  double _x = 0, _y = 0, _heading = 0;
  double _linear_vel = 0;

  double scale = 1.0 / 5.0;

  wheel_state _wheel_left, _wheel_right;

  ui::button resetPos{ui::box{ 0, 0, 0.25, 0.05 }, "Reset XY", ui::colour{1, 0.75, 0.75}, ui::colour{1, 0.5, 0.5}};
  ui::button scalePlus{ ui::box{0.25, 0, 0.25, 0.05}, "Scale +", ui::colour{0.75, 1, 0.75}, ui::colour{0.5, 1, 0.5}};
  ui::button scaleMinus{ ui::box{0.50, 0, 0.25, 0.05}, "Scale -", ui::colour{0.75, 0.75, 1}, ui::colour{0.5, 0.5, 1}};

  ui::colour _left_colour{1, 0, 0};
  ui::colour _right_colour{0, 0, 1};
  ui::colour _front_colour{0, 0.5, 0};
};

}  // namespace simulation