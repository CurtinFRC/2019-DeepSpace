// Simulation harness for the curtinfrc::talon_srx.

#include "curtin_ctre.h"
#include "simulation/ctre_sim.h"

using namespace curtinfrc;

static std::map<int, simulation::ctre::talon_data> _talons;

std::map<int, simulation::ctre::talon_data> &simulation::ctre::all_talons() {
  return _talons;
}

talon_srx::talon_srx(int port) {
  _talons[port] = simulation::ctre::talon_data{};
  _talons[port].port = port;
  _port = port;
}

talon_srx::~talon_srx() {
  _talons.erase(_port);
}

int talon_srx::get_port() {
  return (int) _port;
}

void talon_srx::SetInverted(bool invert) {
  _talons[_port].inverted = true;
}

bool talon_srx::GetInverted() const {
  return _talons[_port].inverted;
}

void talon_srx::Disable() {
  _talons[_port].mode = control_mode::Disabled;
}

void talon_srx::Set(double speed) {
  Set(control_mode::PercentOutput, speed);
}

void talon_srx::Set(talon_srx::control_mode mode, double value) {
  _talons[_port].mode = mode;
  _talons[_port].value = value;
  _value = value;
}

talon_srx::control_mode talon_srx::GetMode() {
  return _talons[_port].mode;
}

int talon_srx::GetSensorPosition() {
  return _talons[_port].sensor_pos;
}

int talon_srx::GetSensorVelocity() {
  return _talons[_port].sensor_vel;
}

void talon_srx::load_config(talon_srx::configuration &config) {
  _talons[_port].config = config;
}

talon_srx::configuration talon_srx::save_config() {
  return _talons[_port].config;
}