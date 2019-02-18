#include "actuators/BinaryEncoderMotor.h"

void curtinfrc::actuators::BinaryEncoderMotor::Update(double dt) {
  int comp = _config.forward - _config.reverse >= 0 ? 1 : -1; // account for situations where forward is less than reverse

  if (_state == kForward) {
    _config.motor.transmission->SetVoltage(comp * 12);
  } else {
    _config.motor.transmission->SetVoltage(-comp * 12);
  }
}

void curtinfrc::actuators::BinaryEncoderMotor::Stop() {
  _config.motor.transmission->SetVoltage(0);
}

bool curtinfrc::actuators::BinaryEncoderMotor::IsDone() {
  bool done = false;

  bool comp = _config.forward - _config.reverse >= 0; // account for situations where forward is less than reverse
  double rot = _config.motor.encoder->GetEncoderRotations();

  if (_state == kForward) {
    done = comp ? rot > _config.forward : rot < _config.forward;
  } else {
    done = comp ? rot < _config.reverse : rot > _config.reverse;
  }

  return done;
}
