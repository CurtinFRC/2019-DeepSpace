#include "intakes/DeployableIntake.h"

void curtinfrc::intakes::DeployableIntake::OnStatePeriodic(curtinfrc::intakes::DeployableIntakeState state, double dt) {
  _config.actuator.Update(dt);

  switch (state) {
   case kIntaking:
   case kOuttaking:
    DeployedPeriodic(state);
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
