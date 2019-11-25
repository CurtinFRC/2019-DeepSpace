#include "actuators/DoubleSolenoid.h"

void curtinfrc::actuators::DoubleSolenoid::UpdateActuator(double dt) {
  Set(_state == actuators::kForward ? kForward : kReverse);
  _timer += dt;
}

bool curtinfrc::actuators::DoubleSolenoid::IsDone() {
  return _timer >= _actuationTime;
  // return true;
}


void curtinfrc::actuators::DoubleSolenoid::Init() {
  _timer = 0;
}
