#include "Lift.h"

void Lift::Set(double power) {
  SetManual(power);
}

void LiftManualStrategy::OnUpdate(double dt) {
  double speed = (_joy.GetRawButton(8) - _joy.GetRawButton(7)) * 0.8;
  _lift.Set(speed);
}
