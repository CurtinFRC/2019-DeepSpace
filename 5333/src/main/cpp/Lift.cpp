#include "Lift.h"
#include "ControlMap.h"

void LiftManualStrategy::OnUpdate(double dt) {
  double speed = (_joy.GetRawButton(ControlMap::raiseLift) - _joy.GetRawButton(ControlMap::lowerLift)) * 0.8;
  _lift.SetManual(speed);
}

void LiftPresetStrategy::OnUpdate(double dt) {
  if (_joy.GetRawButton(ControlMap::goalGround)) {
    _lift.SetSetpoint(ControlMap::setpointGround);

  } else if (_joy.GetRawButton(ControlMap::goalLower1)) {
    _lift.SetSetpoint(ControlMap::setpointLower1);
  } else if (_joy.GetRawButton(ControlMap::goalLower2)) {
  _lift.SetSetpoint(ControlMap::setpointLower2);

  } else if (_joy.GetRawButton(ControlMap::goalMiddle1)) {
    _lift.SetSetpoint(ControlMap::setpointMiddle1);
  } else if (_joy.GetRawButton(ControlMap::goalMiddle2)) {
    _lift.SetSetpoint(ControlMap::setpointMiddle2);

  } else if (_joy.GetRawButton(ControlMap::goalUpper1)) {
    _lift.SetSetpoint(ControlMap::setpointUpper1);
  } else if (_joy.GetRawButton(ControlMap::goalUpper2)) {
    _lift.SetSetpoint(ControlMap::setpointUpper2);
  }
}
