#include "BoxIntake.h"
#include "ControlMap.h"

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
  if (_joy.GetRawButton(ControlMap::boxIn)) {
    _boxIntake.SetIntaking();
  } else if (_joy.GetRawButton(ControlMap::boxOut)) {
    _boxIntake.SetOuttaking();
  } else if (_joy.GetRawButton(ControlMap::boxStow)) {
    _boxIntake.SetStowed();
  }
}
