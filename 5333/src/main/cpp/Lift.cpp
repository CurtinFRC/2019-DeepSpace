#include "Lift.h"
#include "ControlMap.h"

void LiftManualStrategy::OnUpdate(double dt) {
  double speed = (_joyGroup.GetButton(ControlMap::raiseLift) - _joyGroup.GetButton(ControlMap::lowerLift)) * 0.8;
  _lift.SetManual(speed);
}

void LiftGotoStrategy::OnStart() {
  _lift.SetSetpoint(_setpoint);
}

void LiftGotoStrategy::OnUpdate(double dt) {}
