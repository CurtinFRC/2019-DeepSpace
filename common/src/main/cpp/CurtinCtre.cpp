#include "CurtinCtre.h"

using namespace curtinfrc;

void TalonSrx::StopMotor() {
  Disable();
}

void TalonSrx::PIDWrite(double output) {
  Set(output);
}

double TalonSrx::Get() const {
  return _value;
}

int TalonSrx::GetEncoderTicks() {
  return GetSensorPosition();
}

void TalonSrx::ModifyConfig(std::function<void(TalonSrx::Configuration &)> func) {
  TalonSrx::Configuration config = SaveConfig();
  func(config);
  LoadConfig(config);
}