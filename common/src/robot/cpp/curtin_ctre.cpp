// Robot code harness for the curtinfrc::talon_srx.

#include "curtin_ctre.h"

using namespace curtinfrc;
using namespace ctre::phoenix::motorcontrol;

inline can::TalonSRX *native_srx(const talon_srx *srx) {
  return static_cast<can::TalonSRX *>(srx->_handle);
}

talon_srx::talon_srx(int port) {
  _handle = (void *)new can::TalonSRX(port);
  _port = port;
}

talon_srx::~talon_srx() {
  delete native_srx(this);
}

int talon_srx::get_port() {
  return native_srx(this)->GetDeviceID();
}

void talon_srx::SetInverted(bool invert) {
  native_srx(this)->SetInverted(invert);
}

bool talon_srx::GetInverted() const {
  return native_srx(this)->GetInverted();
}

void talon_srx::Disable() {
  native_srx(this)->NeutralOutput();
}

void talon_srx::Set(double speed) {
  Set(control_mode::PercentOutput, speed);
}

void talon_srx::Set(talon_srx::control_mode mode, double value) {
  native_srx(this)->Set(mode, value);
  _value = value;
}

talon_srx::control_mode talon_srx::GetMode() {
  return native_srx(this)->GetControlMode();
}

int talon_srx::GetSensorPosition() {
  return native_srx(this)->GetSelectedSensorPosition();
}

int talon_srx::GetSensorVelocity() {
  return native_srx(this)->GetSelectedSensorVelocity();
}

void talon_srx::load_config(talon_srx::configuration &config) {
  native_srx(this)->ConfigAllSettings(config);
}

talon_srx::configuration talon_srx::save_config() {
  talon_srx::configuration config;
  native_srx(this)->GetAllConfigs(config);
  return config;
}