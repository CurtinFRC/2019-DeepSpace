#include "DoubleSolenoid.h"

bool curtinfrc::DoubleSolenoid::Actuate() {
  Set(_state == curtinfrc::actuators::kForward ? kForward : kReverse);

  return Done();
}

bool curtinfrc::DoubleSolenoid::Done() {
  return true;
}
