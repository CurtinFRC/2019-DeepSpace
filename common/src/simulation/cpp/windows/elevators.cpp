#include "simulation/windows/elevators.h"

#include "sensors/Encoder.h"
#include "simulation/ctre_sim.h"

#include <mockdata/EncoderData.h>
#include <frc/RobotController.h>
#include <algorithm>
#include <iostream>
#include <thread>

using namespace simulation;
using namespace curtinfrc;

template<typename T, typename U>
T *try_cast(U *src) {
  return dynamic_cast<T *>(src);
}

void elevator_window::init() {
  static Usage<ElevatorConfig>::Registry<elevator_window> registry(&ui::init_window<elevator_window, ElevatorConfig *>);
}

elevator_window::elevator_window(ElevatorConfig *config) : ui::window("Elevator", 200, 600), _config(config), physics_aware() {
  if (auto *srx = try_cast<curtinfrc::TalonSrx>(_config->spool.encoder)) {
    // Talon SRX Encoder
    _set_enc_func = [srx](uint32_t count) {
      ctre::all_talons()[srx->GetPort()].sensor_pos = count;
    };
  } else if (auto *digital = try_cast<curtinfrc::sensors::DigitalEncoder>(_config->spool.encoder)) {
    // Digital RoboRIO Encoder
    _set_enc_func = [digital](uint32_t count) {
      HALSIM_SetEncoderCount(digital->GetChannelA(), count);
    };
  } else {
    // TODO: Fail condition
    std::cout << "Unknown Evelator Encoder Type" << std::endl;
  }
}

double elevator_window::get_motor_val() {
  return _config->spool.transmission->Get();
}

void elevator_window::add_encoder_position(double pos) {
  double C = 2 * 3.14159265 * _config->spoolRadius;
  double rots = pos / C;
  auto encoder = _config->spool.encoder;
  _set_enc_func(static_cast<int16_t>(encoder->GetEncoderTicks() + rots * encoder->GetEncoderTicksPerRotation()));
}

void elevator_window::update_physics(double time_delta) {
  
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