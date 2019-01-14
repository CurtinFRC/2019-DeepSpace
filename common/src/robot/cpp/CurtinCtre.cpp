// Robot code harness for the curtinfrc::TalonSrx.

#include "CurtinCtre.h"

using namespace curtinfrc;
using namespace ctre::phoenix::motorcontrol;

inline can::TalonSRX *NativeSrx(const TalonSrx *srx) {
  return static_cast<can::TalonSRX *>(srx->_handle);
}

TalonSrx::TalonSrx(int port) {
  _handle = (void *)new can::TalonSRX(port);
  _port = port;
}

TalonSrx::~TalonSrx() {
  delete NativeSrx(this);
}

int TalonSrx::GetPort() {
  return NativeSrx(this)->GetDeviceID();
}

void TalonSrx::SetInverted(bool invert) {
  NativeSrx(this)->SetInverted(invert);
}

bool TalonSrx::GetInverted() const {
  return NativeSrx(this)->GetInverted();
}

void TalonSrx::Disable() {
  NativeSrx(this)->NeutralOutput();
}

void TalonSrx::Set(double speed) {
  Set(ControlMode::PercentOutput, speed);
}

void TalonSrx::Set(TalonSrx::ControlMode mode, double value) {
  NativeSrx(this)->Set(mode, value);
  _value = value;
}

TalonSrx::ControlMode TalonSrx::GetMode() {
  return NativeSrx(this)->GetControlMode();
}

int TalonSrx::GetSensorPosition() {
  return NativeSrx(this)->GetSelectedSensorPosition();
}

int TalonSrx::GetSensorVelocity() {
  return NativeSrx(this)->GetSelectedSensorVelocity();
}

void TalonSrx::LoadConfig(TalonSrx::Configuration &config) {
  NativeSrx(this)->ConfigAllSettings(config);
}

TalonSrx::Configuration TalonSrx::SaveConfig() {
  TalonSrx::Configuration config;
  NativeSrx(this)->GetAllConfigs(config);
  return config;
}