// Simulation harness for the curtinfrc::TalonSrx.

#include "CurtinCtre.h"
#include "simulation/ctre_sim.h"

using namespace curtinfrc;

static std::map<int, simulation::ctre::talon_data> _talons;
static std::map<int, simulation::ctre::victor_data> _victors;

std::map<int, simulation::ctre::talon_data> &simulation::ctre::all_talons() {
  return _talons;
}

std::map<int, simulation::ctre::victor_data> &simulation::ctre::all_victors() {
  return _victors;
}


// Talon SRX

TalonSrx::TalonSrx(int port, int encoderTicksPerRotation) : Encoder::Encoder(encoderTicksPerRotation) {
  _talons[port] = simulation::ctre::talon_data{};
  _talons[port].port = port;
  _port = port;
}

TalonSrx::~TalonSrx() {
  _talons.erase(_port);
}

void TalonSrx::SetUpdateRate(int hz) {
  // no op in sim
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

void TalonSrx::ZeroEncoder() {
  _talons[_port].sensor_pos = 0;
}

void TalonSrx::LoadConfig(TalonSrx::Configuration &config) {
  _talons[_port].config = config;
}

TalonSrx::Configuration TalonSrx::SaveConfig() {
  return _talons[_port].config;
}


// Victor SPX

VictorSpx::VictorSpx(int port) {
  _victors[port] = simulation::ctre::victor_data{};
  _victors[port].port = port;
  _port = port;
}

VictorSpx::~VictorSpx() {
  _victors.erase(_port);
}

void VictorSpx::SetUpdateRate(int hz) {
  // no op in sim
}

int VictorSpx::GetPort() {
  return (int) _port;
}

void VictorSpx::SetInverted(bool invert) {
  _victors[_port].inverted = true;
}

bool VictorSpx::GetInverted() const {
  return _victors[_port].inverted;
}

void VictorSpx::Disable() {
  _victors[_port].mode = ControlMode::Disabled;
}

void VictorSpx::Set(double speed) {
  Set(ControlMode::PercentOutput, speed);
}

void VictorSpx::Set(VictorSpx::ControlMode mode, double value) {
  _victors[_port].mode = mode;
  _victors[_port].value = value;
  _value = value;
}

VictorSpx::ControlMode VictorSpx::GetMode() {
  return _victors[_port].mode;
}

void VictorSpx::LoadConfig(VictorSpx::Configuration &config) {
  _victors[_port].config = config;
}

VictorSpx::Configuration VictorSpx::SaveConfig() {
  return _victors[_port].config;
}
