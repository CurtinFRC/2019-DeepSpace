#include "BoxIntake.h"
#include "ControlMap.h"

void BoxIntake::IntakingPeriodic() {
  _config.motors.transmission->SetVoltage(12);
}

void BoxIntake::OuttakingPeriodic() {
  _config.motors.transmission->SetVoltage(-12);
}

void BoxIntake::DeployingPeriodic() {
  _config.motors.transmission->SetVoltage(0);
}

void BoxIntake::StowingPeriodic() {
  _config.motors.transmission->SetVoltage(-1.5);
}

void BoxIntake::StowedPeriodic() {
  _config.motors.transmission->SetVoltage(-0.6);
}


void BoxIntakeManualStrategy::OnUpdate(double dt) {
  if (_joyGroup.GetButton(ControlMap::boxIn)) {
    _boxIntake.SetIntaking();
  } else if (_joyGroup.GetButton(ControlMap::boxOut)) {
    _boxIntake.SetOuttaking();
  } else if (_joyGroup.GetButton(ControlMap::boxStow)) {
    _boxIntake.SetStowed();
  }
}
