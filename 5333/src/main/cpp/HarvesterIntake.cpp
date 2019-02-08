#include "HarvesterIntake.h"
#include "ControlMap.h"

void HarvesterIntake::IntakingPeriodic() {
  _config.motors.transmission->Set(1 * ControlMap::harvesterThrottle);
}

void HarvesterIntake::OuttakingPeriodic() {
  _config.motors.transmission->Set(-1 * ControlMap::harvesterThrottle);
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

void HarvesterIntakeManualStrategy::OnUpdate(double dt) {
  if (_joy.GetRawButton(ControlMap::harveserIn)) {
    _harvesterIntake.SetIntaking();
  } else if (_joy.GetRawButton(ControlMap::harveserOut)) {
    _harvesterIntake.SetOuttaking();
  } else if (_joy.GetRawButton(ControlMap::harveserStow)) {
    _harvesterIntake.SetStowed();
  }
}
