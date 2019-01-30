#include "HarvesterIntake.h"

void HarvesterIntake::DeployedPeriodic(HarvesterIntakeState state) {
  _config.motors.transmission->Set(state == HarvesterIntakeState::kIntaking ? 1 : -1);
}

void HarvesterIntake::DeployingPeriodic() {
  _config.motors.transmission->StopMotor();
}

void HarvesterIntake::StowingPeriodic() {
  _config.motors.transmission->StopMotor();
}

void HarvesterIntake::StowedPeriodic() {
  _config.motors.transmission->StopMotor(); // probably doesn't need this, but ~
}


void HarvesterIntakeController::Update(double dt) {
  if (_joy.GetRawButton(3)) {
    _harvesterIntake.SetDeployed(HarvesterIntakeState::kIntaking);
  } else if (_joy.GetRawButton(5)) {
    _harvesterIntake.SetDeployed(HarvesterIntakeState::kOuttaking);
  } else if (_joy.GetRawButton(11)) {
    _harvesterIntake.SetStowed();
  }

  _harvesterIntake.Update(dt);
}
