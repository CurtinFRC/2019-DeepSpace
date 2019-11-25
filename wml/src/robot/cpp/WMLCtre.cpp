// Robot code harness for the wml::TalonSrx.

#include "WMLCtre.h"

using namespace wml;
using namespace ctre::phoenix::motorcontrol;


// Talon SRX

inline can::TalonSRX *NativeSrx(const TalonSrx *srx) {
  return static_cast<can::TalonSRX *>(srx->_handle);
}

TalonSrx::TalonSrx(int port, int encoderTicksPerRotation) : actuators::MotorVoltageController(this), Encoder::Encoder(encoderTicksPerRotation) {
  _handle = (void *)new can::TalonSRX(port);
  _port = port;
}

TalonSrx::~TalonSrx() {
  delete NativeSrx(this);
}

void TalonSrx::SetUpdateRate(int hz) {
  NativeSrx(this)->SetControlFramePeriod(ctre::phoenix::motorcontrol::ControlFrame::Control_3_General, 1000 / hz);
}

int TalonSrx::GetPort() {
  return NativeSrx(this)->GetDeviceID();
}

void TalonSrx::SetInverted(bool invert) {
  NativeSrx(this)->SetInverted(invert);
  NativeSrx(this)->SetSensorPhase(invert);
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

void TalonSrx::ZeroEncoder() {
  NativeSrx(this)->SetSelectedSensorPosition(0);
}

void TalonSrx::LoadConfig(TalonSrx::Configuration &config) {
  NativeSrx(this)->ConfigAllSettings(config);
}

TalonSrx::Configuration TalonSrx::SaveConfig() {
  TalonSrx::Configuration config;
  NativeSrx(this)->GetAllConfigs(config);
  return config;
}


// Victor SPX

inline can::VictorSPX *NativeSpx(const VictorSpx *srx) {
  return static_cast<can::VictorSPX *>(srx->_handle);
}

VictorSpx::VictorSpx(int port) : actuators::MotorVoltageController(this) {
  _handle = (void *)new can::VictorSPX(port);
  _port = port;
}

VictorSpx::~VictorSpx() {
  delete NativeSpx(this);
}

void VictorSpx::SetUpdateRate(int hz) {
  NativeSpx(this)->SetControlFramePeriod(ctre::phoenix::motorcontrol::ControlFrame::Control_3_General, 1000 / hz);
}

int VictorSpx::GetPort() {
  return NativeSpx(this)->GetDeviceID();
}

void VictorSpx::SetInverted(bool invert) {
  NativeSpx(this)->SetInverted(invert);
}

bool VictorSpx::GetInverted() const {
  return NativeSpx(this)->GetInverted();
}

void VictorSpx::Disable() {
  NativeSpx(this)->NeutralOutput();
}

void VictorSpx::Set(double speed) {
  Set(ControlMode::PercentOutput, speed);
}

void VictorSpx::Set(VictorSpx::ControlMode mode, double value) {
  NativeSpx(this)->Set(mode, value);
  _value = value;
}

VictorSpx::ControlMode VictorSpx::GetMode() {
  return NativeSpx(this)->GetControlMode();
}

void VictorSpx::LoadConfig(VictorSpx::Configuration &config) {
  NativeSpx(this)->ConfigAllSettings(config);
}

VictorSpx::Configuration VictorSpx::SaveConfig() {
  VictorSpx::Configuration config;
  NativeSpx(this)->GetAllConfigs(config);
  return config;
}
