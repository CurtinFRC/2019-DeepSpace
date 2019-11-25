#include "actuators/BinaryServo.h"

using namespace wml::actuators;

void BinaryServo::UpdateActuator(double dt) {
  SetAngle(_state == kForward ? _forwardPos : _reversePos);
}

bool BinaryServo::IsDone() {
  return true;
}
