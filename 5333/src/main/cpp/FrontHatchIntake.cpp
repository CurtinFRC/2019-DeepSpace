#include "FrontHatchIntake.h"
#include "ControlMap.h"

void FrontHatchIntake::IntakingPeriodic() { // Intake
  _config.ejector.SetTarget(curtinfrc::actuators::BinaryActuatorState::kReverse);
}

void FrontHatchIntake::OuttakingPeriodic() { // Eject
  _config.ejector.SetTarget(curtinfrc::actuators::BinaryActuatorState::kForward);
}


void FrontHatchIntakeManualStrategy::OnUpdate(double dt) {
  if (_enabledToggle.Update(_joy.GetRawButton(ControlMap::frontHatchToggleEnabled))) _enabled = !_enabled;

  if (_enabled) {
    if (_joy.GetRawButton(ControlMap::frontHatchGrab)) {
      _frontHatchIntake.SetIntaking();
    } else if (_joy.GetRawButton(ControlMap::frontHatchRelease)) {
      _frontHatchIntake.SetOuttaking();
    } else if (_joy.GetRawButton(ControlMap::frontHatchStow)) {
      _frontHatchIntake.SetStowed();
    }
  } else {
    _frontHatchIntake.SetStowed();
  }
}
