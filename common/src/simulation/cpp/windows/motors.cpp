#include "simulation/windows/motors.h"
#include "simulation/ctre_sim.h"

#include "mockdata/PWMData.h"

#include <sstream>
#include <math.h>

using namespace simulation;

using ControlMode = typename curtinfrc::TalonSrx::ControlMode;

motor_window::motor_window() : ui::window("Motors", 250, 400) {}

static std::string ctre_to_string(ControlMode mode, double val) {
  return (mode == ControlMode::Position
              ? "Position " + ui::utils::fmt_precision(val, 0)
              : mode == ControlMode::Velocity
                    ? "Velocity " + ui::utils::fmt_precision(val, 1)
                          : mode == ControlMode::Follower
                                ? "Following ID " + ui::utils::fmt_precision(val, 0)
                                : mode == ControlMode::Disabled ? "Disabled" : "?????");
}

void motor_window::render(cv::Mat &img) {
  ui::point{0.5, 0.05}.text(img, "Motor Readings", 1.0, ui::colour::white(), 2);

  double yorigin = 0.15;

  for (int pwm = 0; pwm < 10; pwm++) {
    bool   initialized = HALSIM_GetPWMInitialized(pwm);
    double speed       = HALSIM_GetPWMSpeed(pwm);
    render_motor(img, yorigin, "PWM", pwm, initialized, speed * 100);
    yorigin += 0.05;
  }

  yorigin += 0.05;

  auto talons = ctre::all_talons();
  for (auto it = talons.begin(); it != talons.end(); it++) {
    ControlMode mode = it->second.mode;
    double       val  = it->second.value;
    if (mode == ControlMode::PercentOutput) {
      render_motor(img, yorigin, "SRX", it->second.port, true, val * 100);
    } else if (mode == ControlMode::Current) {
      render_motor(img, yorigin, "SRX", it->second.port, true, val, 0, 40, "A");
    } else {
      render_motor(img, yorigin, "SRX", it->second.port, ctre_to_string(mode, val));
    }
    yorigin += 0.05;
  }

  auto victors = ctre::all_victors();
  for (auto it = victors.begin(); it != victors.end(); it++) {
    ControlMode mode = it->second.mode;
    double       val  = it->second.value;
    if (mode == ControlMode::PercentOutput) {
      render_motor(img, yorigin, "SPX", it->second.port, true, val * 100);
    } else if (mode == ControlMode::Current) {
      render_motor(img, yorigin, "SPX", it->second.port, true, val, 0, 40, "A");
    } else {
      render_motor(img, yorigin, "SPX", it->second.port, ctre_to_string(mode, val));
    }
    yorigin += 0.05;
  }
}

void motor_window::render_motor(cv::Mat &img, double yorigin, std::string type, int index, bool initialized,
                                double val, double min, double max, std::string unit, int precision) {
  std::string label = type + " " + std::to_string(index);
  ui::point{0.02, yorigin}.textl(img, label.c_str(), 0.5, ui::colour::gray(), 1);

  if (initialized) {
    ui::colour col = val > 0 ? ui::colour{0, 1, 0} : ui::colour{1, 0, 0};
    
    double clamped = std::min(max, std::max(min, std::abs(val)));
    double perc = clamped / (max - min);
    ui::box{0.3, yorigin - 0.02, 0.4 * perc, 0.04}.fill(img, col);

    ui::point{0.85, yorigin}.text(img, (ui::utils::fmt_precision(val, precision) + unit).c_str(), 0.5,
                                  ui::colour::gray(), 1);
  } else {
    ui::box{0.3, yorigin - 0.02, 0.4, 0.04}.fill(img, ui::colour::gray());
    ui::point{0.85, yorigin}.text(img, ("---" + unit).c_str(), 0.5, ui::colour::gray(), 1);
  }
}

void motor_window::render_motor(cv::Mat &img, double yorigin, std::string type, int index, std::string text) {
  std::string label = type + " " + std::to_string(index);
  ui::point{0.02, yorigin}.textl(img, label.c_str(), 0.5, ui::colour::gray(), 1);

  ui::point{0.3, yorigin}.textl(img, text.c_str(), 0.5, ui::colour::gray(), 1);
}