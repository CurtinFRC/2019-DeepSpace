#include "actuators/DoubleSolenoid.h"

void curtinfrc::actuators::DoubleSolenoid::UpdateActuator(double dt) {
  Set(_state == actuators::kForward ? kForward : kReverse);
}
