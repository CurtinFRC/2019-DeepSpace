#include "actuators/DoubleSolenoid.h"

void curtinfrc::actuators::DoubleSolenoid::Update() {
  Set(_state == actuators::kForward ? kForward : kReverse);
}

bool curtinfrc::actuators::DoubleSolenoid::IsDone() {
  return true;
}
