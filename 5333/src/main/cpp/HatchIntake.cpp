#include "HatchIntake.h"

void HatchIntake::IntakingPeriodic() { // Intake
  _config.servo.SetAngle(_config.forward);
}

void HatchIntake::OuttakingPeriodic() { // Eject
  _config.servo.SetAngle(_config.reverse);
}


void HatchIntakeManualStrategy::OnUpdate(double dt) {
  if (_enabledToggle.Update(_joy.GetRawButton(9))) _enabled = !_enabled;

  if (_enabled) {
    if (_joy.GetRawButton(4)) {
      _hatchIntake.SetIntaking();
    } else if (_joy.GetRawButton(6)) {
      _hatchIntake.SetOuttaking();
    } else if (_joy.GetRawButton(10)) {
      _hatchIntake.SetStowed();
    }
  } else {
    _hatchIntake.SetStowed();
  }

  _hatchIntake.Update(dt);
}
