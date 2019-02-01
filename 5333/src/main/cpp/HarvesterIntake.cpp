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
