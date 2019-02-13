#include "HatchIntake.h"
#include "ControlMap.h"

void HatchIntake::IntakingPeriodic() { // Intake
  _config.manipulator.SetTarget(curtinfrc::actuators::kForward);
}

void HatchIntake::OuttakingPeriodic() { // Eject
  _config.manipulator.SetTarget(curtinfrc::actuators::kReverse);
}


void HatchIntakeManualStrategy::OnUpdate(double dt) {
  _config.manipulator.Update(dt);
  if (_enabledToggle.Update(_joy.GetRawButton(ControlMap::hatchToggleEnabled))) _enabled = !_enabled;

  if (_enabled) {
    if (_joy.GetRawButton(ControlMap::hatchGrab)) {
      _hatchIntake.SetIntaking();
    } else if (_joy.GetRawButton(ControlMap::hatchRelease)) {
      _hatchIntake.SetOuttaking();
    } else if (_joy.GetRawButton(ControlMap::hatchStow)) {
      _hatchIntake.SetStowed();
    }
  } else {
    _hatchIntake.SetStowed();
  }
}
