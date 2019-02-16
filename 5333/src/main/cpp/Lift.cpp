#include "Lift.h"
#include "ControlMap.h"

void LiftManualStrategy::OnUpdate(double dt) {
  double speed = (_joyGroup.GetButton(ControlMap::raiseLift) - _joyGroup.GetButton(ControlMap::lowerLift)) * 0.8;
  if (std::abs(speed) < 0.001) {
    if (_lift.GetState() != curtinfrc::ElevatorState::kStationary)
      _lift.SetHold();
  } else
    _lift.SetManual(speed);
}

void LiftGotoStrategy::OnStart() {
  _lift.SetSetpoint(_setpoint);
}

void LiftGotoStrategy::OnUpdate(double dt) {
  if (_lift.GetState() != curtinfrc::ElevatorState::kMoving) 
    SetDone();
}

void LiftZeroStrategy::OnStart() {
  _lift.SetZeroing();
}

void LiftZeroStrategy::OnUpdate(double dt) {
  if (_lift.GetState() != curtinfrc::ElevatorState::kZeroing) 
    SetDone();
}
