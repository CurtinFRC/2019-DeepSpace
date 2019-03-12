#include "devices/DeployableDevice.h"

void curtinfrc::devices::DeployableDevice::SetIntaking() {
  if (!_config.canEject) { // default
    switch (_state) {
     case kIntaking:
      break;

     case kOuttaking:
      SetState(kIntaking);
      break;

     default:
      SetState(kDeploying);
      break;
    }
  } else { // can eject while stowed
    switch (_state) {
     case kIntaking:
      break;

     default:
      SetState(kDeploying);
      break;
    }
  }
}

void curtinfrc::devices::DeployableDevice::SetOuttaking() {
  if (!_config.canEject) { // default
    switch (_state) {
     case kOuttaking:
      break;

     case kIntaking:
      SetState(kOuttaking);
      break;

     default:
      SetState(kDeploying);
      break;
    }
  } else { // can eject while stowed
    switch (_state) {
     case kOuttaking:
      break;

     case kIntaking:
     case kStowed:
      SetState(kOuttaking);
      break;

     default:
      SetState(kDeploying);
      break;
    }
  }
}

void curtinfrc::devices::DeployableDevice::SetStowed() {
  switch (_state) {
   case kStowed:
    break;

   default:
    SetState(kStowing);
    break;
  }
}

void curtinfrc::devices::DeployableDevice::OnStatePeriodic(curtinfrc::devices::DeployableDeviceState state, double dt) {
  _config.actuator.Update(dt);

  switch (state) {
   case kIntaking:
    IntakingPeriodic();
    break;
    
   case kOuttaking:
    OuttakingPeriodic();
    break;

   case kDeploying:
    _config.actuator.SetTarget(curtinfrc::actuators::kForward);

    if (_config.actuator.IsDone()) {
      SetState(kIntaking); // Changes to kIntaking (as opposed to kOuttaking) by default
      break;
    }

    DeployingPeriodic();
    break;

   case kStowing:
    _config.actuator.SetTarget(curtinfrc::actuators::kReverse);

    if (_config.actuator.IsDone()) {
      SetState(kStowed);
      break;
    }

    StowingPeriodic();
    break;

   case kStowed:
    StowedPeriodic();
    break;
  }
}
