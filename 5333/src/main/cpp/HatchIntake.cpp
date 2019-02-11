#include "HatchIntake.h"
#include "ControlMap.h"

void HatchIntake::IntakingPeriodic() { // Intake
  _config.servo.SetAngle(_config.forward);
}

void HatchIntake::OuttakingPeriodic() { // Eject
  _config.servo.SetAngle(_config.reverse);
}


void HatchIntakeManualStrategy::OnUpdate(double dt) {
  if (_enabledToggle.Update(_joyGroup.GetButton(ControlMap::hatchToggleEnabled))) _enabled = !_enabled;

  if (_enabled) {
    if (_joyGroup.GetButton(ControlMap::hatchGrab)) {
      _hatchIntake.SetIntaking();
    } else if (_joyGroup.GetButton(ControlMap::hatchRelease)) {
      _hatchIntake.SetOuttaking();
    } else if (_joyGroup.GetButton(ControlMap::hatchStow)) {
      _hatchIntake.SetStowed();
    }
  } else {
    _hatchIntake.SetStowed();
  }
}
