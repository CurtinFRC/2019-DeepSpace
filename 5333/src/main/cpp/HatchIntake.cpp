#include "HatchIntake.h"
#include "ControlMap.h"

void HatchIntake::IntakingPeriodic() { // Intake
  _config.servo.SetAngle(_config.forward);
}

void HatchIntake::OuttakingPeriodic() { // Eject
  _config.servo.SetAngle(_config.reverse);
}


void HatchIntakeManualStrategy::OnUpdate(double dt) {
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
