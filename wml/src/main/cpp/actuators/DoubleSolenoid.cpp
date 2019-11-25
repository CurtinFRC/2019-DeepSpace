#include "actuators/DoubleSolenoid.h"

void wml::actuators::DoubleSolenoid::UpdateActuator(double dt) {
  Set(_state == actuators::kForward ? kForward : kReverse);
  _timer += dt;
}

bool wml::actuators::DoubleSolenoid::IsDone() {
  return _timer >= _actuationTime;
  // return true;
}


void wml::actuators::DoubleSolenoid::Init() {
  _timer = 0;
}
