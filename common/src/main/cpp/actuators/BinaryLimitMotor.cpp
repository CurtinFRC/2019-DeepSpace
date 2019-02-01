#include "actuators/BinaryLimitMotor.h"

void curtinfrc::actuators::BinaryLimitMotor::Update(double dt) {
  if (_state == kForward) {
    _config.motor.transmission->Set(1);
  } else {
    _config.motor.transmission->Set(-1);
  }
}

void curtinfrc::actuators::BinaryLimitMotor::Stop() {
  _config.motor.transmission->Set(0);
}

bool curtinfrc::actuators::BinaryLimitMotor::IsDone() {
  return _state == kForward ? _config.forward.Get() : _config.reverse.Get();
}
