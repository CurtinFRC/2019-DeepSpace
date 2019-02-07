#include "BoxIntake.h"

void BoxIntake::IntakingPeriodic() {
  _config.motors.transmission->Set(1);
}

void BoxIntake::OuttakingPeriodic() {
  _config.motors.transmission->Set(-1);
}

void BoxIntake::DeployingPeriodic() {
  _config.motors.transmission->StopMotor();
}

void BoxIntake::StowingPeriodic() {
  _config.motors.transmission->StopMotor();
}

void BoxIntake::StowedPeriodic() {
  _config.motors.transmission->StopMotor();
}


void BoxIntakeManualStrategy::OnUpdate(double dt) {
  if (_joy.GetRawButton(3)) {
    _boxIntake.SetIntaking();
  } else if (_joy.GetRawButton(5)) {
    _boxIntake.SetOuttaking();
  } else if (_joy.GetRawButton(11)) {
    _boxIntake.SetStowed();
  }
}
