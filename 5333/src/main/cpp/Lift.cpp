#include "Lift.h"
#include "ControlMap.h"

void LiftManualStrategy::OnUpdate(double dt) {
  double speed = (_joy.GetRawButton(ControlMap::raiseLift) - _joy.GetRawButton(ControlMap::lowerLift)) * 0.8;
  _lift.SetManual(speed);
}

void LiftGotoStrategy::OnStart() {
  _lift.SetSetpoint(_setpoint);
}

void LiftZeroStrategy::OnStart() {
  _lift.SetZeroing();
}

void LiftGotoStrategy::OnUpdate(double dt) {}
void LiftZeroStrategy::OnUpdate(double dt) {}

