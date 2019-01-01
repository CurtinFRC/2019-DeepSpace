#include "simulation/windows/motors.h"

#include "mockdata/PWMData.h"

using namespace simulation;

motor_window::motor_window() : ui::window("Motors", 250, 400) { }

void motor_window::render(cv::Mat &img) {
  ui::point{0.5, 0.05}.text(img, "Motor Readings", 1.0, ui::colour::white(), 2);

  double yorigin = 0.15;

  for (int pwm = 0; pwm < 10; pwm++) {
    bool initialized = HALSIM_GetPWMInitialized(pwm);
    double speed = HALSIM_GetPWMSpeed(pwm);
    render_motor(img, yorigin, "PWM", pwm, initialized, speed);
    yorigin += 0.05;
  }

  // TODO: CAN Controllers (Victor SPX and Talon SRX)
}

void motor_window::render_motor(cv::Mat &img, double yorigin, std::string type, int index, bool initialized, double speed) {
  std::string label = type + " " + std::to_string(index);
  ui::point{ 0.1, yorigin }.text(img, label.c_str(), 0.5, ui::colour::gray(), 1);

  if (initialized) {
    ui::colour col = speed > 0 ? ui::colour{ 0, 1, 0 } : ui::colour{ 1, 0, 0 };
    ui::box{ 0.5, yorigin - 0.02, 0.2 * speed, 0.04 }.fill(img, col);
    
    ui::point{ 0.85, yorigin }.text(img, (std::to_string((int)(speed * 100)) + "%").c_str(), 0.5, ui::colour::gray(), 1);
  } else {
    ui::box{ 0.3, yorigin - 0.02, 0.4, 0.04 }.fill(img, ui::colour::gray());
    ui::point{ 0.85, yorigin }.text(img, "---%", 0.5, ui::colour::gray(), 1);
  }
}