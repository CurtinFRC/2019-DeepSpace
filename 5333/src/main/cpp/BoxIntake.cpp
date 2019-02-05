#include "BoxIntake.h"

void BoxIntake::IntakingPeriodic() { // Intake
  _config.servo.SetAngle(_config.forward);
}

void BoxIntake::OuttakingPeriodic() { // Eject
  _config.servo.SetAngle(_config.reverse);
}


void BoxIntakeManualStrategy::OnUpdate(double dt) {
  if (_enabledToggle.Update(_joy.GetRawButton(9))) _enabled = !_enabled;

  if (_enabled) {
    if (_joy.GetRawButton(4)) {
      _boxIntake.SetIntaking();
    } else if (_joy.GetRawButton(6)) {
      _boxIntake.SetOuttaking();
    } else if (_joy.GetRawButton(10)) {
      _boxIntake.SetStowed();
    }
  } else {
    _boxIntake.SetStowed();
  }
}
