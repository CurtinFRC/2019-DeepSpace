#include "actuators/BinaryLimitMotor.h"

void wml::actuators::BinaryLimitMotor::UpdateActuator(double dt) {
  if (_state == kForward) {
    _config.motor.transmission->SetVoltage(12);
  } else {
    _config.motor.transmission->SetVoltage(-12);
  }
}

void wml::actuators::BinaryLimitMotor::Stop() {
  _config.motor.transmission->SetVoltage(0);
}

bool wml::actuators::BinaryLimitMotor::IsDone() {
  return _state == kForward ? _config.forward.Get() : _config.reverse.Get();
}
