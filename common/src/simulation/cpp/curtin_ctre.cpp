// Simulation harness for the curtinfrc::TalonSrx.

#include "CurtinCtre.h"
#include "simulation/ctre_sim.h"

using namespace curtinfrc;

static std::map<int, simulation::ctre::talon_data> _talons;

std::map<int, simulation::ctre::talon_data> &simulation::ctre::all_talons() {
  return _talons;
}

TalonSrx::TalonSrx(int port) {
  _talons[port] = simulation::ctre::talon_data{};
  _talons[port].port = port;
  _port = port;
}

TalonSrx::~TalonSrx() {
  _talons.erase(_port);
}

int TalonSrx::GetPort() {
  return (int) _port;
}

void TalonSrx::SetInverted(bool invert) {
  _talons[_port].inverted = true;
}

bool TalonSrx::GetInverted() const {
  return _talons[_port].inverted;
}

void TalonSrx::Disable() {
  _talons[_port].mode = ControlMode::Disabled;
}

void TalonSrx::Set(double speed) {
  Set(ControlMode::PercentOutput, speed);
}

void TalonSrx::Set(TalonSrx::ControlMode mode, double value) {
  _talons[_port].mode = mode;
  _talons[_port].value = value;
  _value = value;
}

TalonSrx::ControlMode TalonSrx::GetMode() {
  return _talons[_port].mode;
}

int TalonSrx::GetSensorPosition() {
  return _talons[_port].sensor_pos;
}

int TalonSrx::GetSensorVelocity() {
  return _talons[_port].sensor_vel;
}

void TalonSrx::LoadConfig(TalonSrx::Configuration &config) {
  _talons[_port].config = config;
}

TalonSrx::Configuration TalonSrx::SaveConfig() {
  return _talons[_port].config;
}