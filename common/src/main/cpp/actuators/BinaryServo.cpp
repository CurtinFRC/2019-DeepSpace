#include "actuators/BinaryServo.h"

void curtinfrc::actuators::BinaryServo::Update(double dt) {
  SetAngle(_state == kForward ? _config.forward : _config.reverse);
}
