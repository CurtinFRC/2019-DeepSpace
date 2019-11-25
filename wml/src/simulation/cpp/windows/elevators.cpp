#include "simulation/windows/elevators.h"

#include <frc/RobotController.h>
#include <algorithm>
#include <iostream>
#include <thread>
#include <cmath>

using namespace simulation;
using namespace wml;

void elevator_window::init() {
  static Usage<ElevatorConfig>::Registry<elevator_window> registry(&ui::init_window<elevator_window, ElevatorConfig *>);
}

elevator_window::elevator_window(ElevatorConfig *config) : ui::window("Elevator", 200, 600), _config(config), physics_aware() {
  _enc_sim = components::create_encoder(config->spool.encoder);

  register_button(resetPos);

  resetPos.set_can_activate(false);
  resetPos.on_click([&](bool, ui::button&) {
    _position = 0;
    if (_enc_sim != nullptr)
      _enc_sim->set_counts(0);
  });
}

double elevator_window::get_motor_val() {
  auto bat_volt = frc::RobotController::GetInputVoltage();
  return std::min(bat_volt, std::max(-bat_volt, _config->spool.transmission->GetVoltage()));
}

void elevator_window::update_encoder(double pos, double vel) {
  double C = 2 * 3.14159265 * _config->spoolRadius;
  double rots = pos / C;
  double rotspersec = vel / C;
  auto encoder = _config->spool.encoder;
  if (encoder != nullptr) {
    _enc_sim->set_counts(static_cast<int>(rots * encoder->GetEncoderTicksPerRotation()));
    _enc_sim->set_counts_per_sec(static_cast<int>(rotspersec * encoder->GetEncoderTicksPerRotation()));
  }
}

void elevator_window::update_physics(double time_delta) {
  _voltage = get_motor_val();

  physics::DcMotor motor = _config->spool.motor;
  motor = motor.reduce(_config->spool.reduction);

  double angular_vel = _velocity / _config->spoolRadius;

  _current = motor.current(_voltage, angular_vel);
  double torque = motor.torque(_current);
  double accel = torque / (_config->mass * _config->spoolRadius) - 9.81;

  _velocity += accel * time_delta;

  if (accel <= 0.001 && _limit_bottom)
    _velocity = 0;
  else if (accel >= -0.001 && _limit_top)
    _velocity = 0;

  _position += _velocity * time_delta;
  update_encoder(_position, _velocity);

  _limit_bottom = _position < 0.01;
  _limit_top = _position > _config->height - 0.01;

  // TODO: Limit Switch Sim
}

void elevator_window::render(cv::Mat &img) {
  ui::point{0.5, 0.05}.text(img, "Elevator", 1.0, ui::colour::white(), 2);

  ui::point start{0.5, 0.1};
  ui::point end{0.5, 0.95};

  ui::box{start.x, start.y, 0.01, end.y - start.y}.fill(img, ui::colour::white());

  double perc = _position / _config->height;
  double height_rel = perc * (start.y - end.y) + end.y;

  ui::colour col{get_motor_val() < -0.1 ? 1 : 0.5, get_motor_val() > 0.1 ? 1 : 0.5, 0.5};
  if (_limit_bottom || _limit_top)
    col = ui::colour{1, 0.5, 1};

  ui::colour orange{1, 0.5, 0};

  ui::box{start.x - 0.05, height_rel, 0.1, 0.01}.fill(img, col);

  ui::point{start.x + 0.1, height_rel}.textl(img, (ui::utils::fmt_precision(_position, 2) + "m").c_str(),   0.5, col, 1);
  ui::point{start.x + 0.1, height_rel + 0.025}.textl(img, (ui::utils::fmt_precision(_velocity, 2) + "m/s").c_str(), 0.5, col, 1);

  ui::point{start.x - 0.4, height_rel}.textl(img, (ui::utils::fmt_precision(_voltage, 2) + "V").c_str(), 0.5, std::abs(_voltage) > 12 ? orange : col, 1);
  ui::point{start.x - 0.4, height_rel + 0.025}.textl(img, (ui::utils::fmt_precision(_current, 2) + "A").c_str(), 0.5, std::abs(_current) > 40 ? orange : col, 1);

  if (_config->spool.encoder != nullptr)
    ui::point{start.x, height_rel - 0.025}.text(img, ("Enc: " + std::to_string(_config->spool.encoder->GetEncoderTicks())).c_str(), 0.5, col, 1);
}