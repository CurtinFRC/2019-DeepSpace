#include "Lift.h"
#include "ControlMap.h"

void Lift::Set(double power) {
  SetManual(power);
}

void LiftManualStrategy::OnUpdate(double dt) {
  double speed = (_joyGroup.GetButton(ControlMap::raiseLift) - _joyGroup.GetButton(ControlMap::lowerLift)) * 0.8;
  _lift.Set(speed);
}
