#include "actuators/BinaryServo.h"

using namespace curtinfrc::actuators;

void BinaryServo::UpdateActuator(double dt) {
  SetAngle(_state == kForward ? _forwardPos : _reversePos);
}

bool BinaryServo::IsDone() {
  return true;
}
