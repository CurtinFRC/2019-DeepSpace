#include "actuators/BinaryLimitMotor.h"

bool curtinfrc::actuators::BinaryLimitMotor::Actuate() {
  bool done = Done();

  if (!done) {
    if (_state == kForward) {
      _config.motor.transmission->Set(1);
    } else {
      _config.motor.transmission->Set(-1);
    }
  } else {
    _config.motor.transmission->Set(0);
  }

  return done;
}

bool curtinfrc::actuators::BinaryLimitMotor::Done() {
  return _state == kForward ? _config.forward.Get() : _config.reverse.Get();
}
