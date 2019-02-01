#include "Lift.h"

void Lift::Set(double power) {
  SetManual(power);
}


void LiftManualStrategy::OnUpdate(double dt) {
  if (_joy.GetRawButton(8)) {
    _lift.SetManual(1);
  } else if (_joy.GetRawButton(7)) {
    _lift.SetManual(-1);
  } else {
    _lift.SetManual(0);
  }

  _lift.Update(dt);
}
