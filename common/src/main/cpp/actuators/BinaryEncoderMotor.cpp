#include "actuators/BinaryEncoderMotor.h"

bool curtinfrc::actuators::BinaryEncoderMotor::Actuate() {
  bool done = Done();

  if (!done) {
    int comp = _config.forward - _config.reverse >= 0 ? 1 : -1; // account for situations where forward is less than reverse

    if (_state == kForward) {
      _config.motor.transmission->Set(comp * 1);
    } else {
      _config.motor.transmission->Set(-comp * 1);
    }
  } else {
    _config.motor.transmission->Set(0);
  }

  return done;
}

bool curtinfrc::actuators::BinaryEncoderMotor::Done() {
  bool comp = _config.forward - _config.reverse >= 0; // account for situations where forward is less than reverse
  double rot = _config.motor.encoder->GetEncoderRotations();

  if (_state == kForward) {
    return comp ? rot > _config.forward : rot < _config.forward;
  } else {
    return comp ? rot < _config.reverse : rot > _config.reverse;
  }

  return false;
}
