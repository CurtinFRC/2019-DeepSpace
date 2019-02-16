#include "HatchIntake.h"
#include "ControlMap.h"

void HatchIntake::IntakingPeriodic() { // Primary (e.g. intaking)
  _config.manipulator.SetTarget(curtinfrc::actuators::kForward);
}

void HatchIntake::OuttakingPeriodic() { // Reverse (e.g. ejecting)
  _config.manipulator.SetTarget(curtinfrc::actuators::kReverse);
}

void HatchIntake::StowedPeriodic() { // Stow
  _config.manipulator.SetTarget(curtinfrc::actuators::kForward);
}


void HatchIntakeManualStrategy::OnUpdate(double dt) {
  _hatchIntake.GetConfig().manipulator.Update(dt);
  if (_hatchIntake.GetConfig().manipulator.IsDone()) _hatchIntake.GetConfig().manipulator.Stop();

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
