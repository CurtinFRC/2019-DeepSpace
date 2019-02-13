#include "SideHatchIntake.h"
#include "ControlMap.h"

void SideHatchIntake::IntakingPeriodic() { // Intake
  _config.servo.SetAngle(_config.forward);
}

void SideHatchIntake::OuttakingPeriodic() { // Eject
  _config.servo.SetAngle(_config.reverse);
}


void SideHatchIntakeManualStrategy::OnUpdate(double dt) {
  if (_enabledToggle.Update(_joy.GetRawButton(ControlMap::sideHatchToggleEnabled))) _enabled = !_enabled;

  if (_enabled) {
    if (_joy.GetRawButton(ControlMap::sideHatchGrab)) {
      _sideHatchIntake.SetIntaking();
    } else if (_joy.GetRawButton(ControlMap::sideHatchRelease)) {
      _sideHatchIntake.SetOuttaking();
    } else if (_joy.GetRawButton(ControlMap::sideHatchStow)) {
      _sideHatchIntake.SetStowed();
    }
  } else {
    _sideHatchIntake.SetStowed();
  }
}
