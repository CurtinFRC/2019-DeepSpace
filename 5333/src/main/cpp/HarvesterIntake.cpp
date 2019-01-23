#include "HarvesterIntake.h"

void HarvesterIntake::OnStatePeriodic(HarvesterIntakeState state, double dt) {
  switch (state) {
   case kDeployed:
    _config.motors.transmission->Set(1); // set to some value based on forwards/backwards
    break;

   case kDeploying:
    _config.motors.transmission->StopMotor();
    _config.solenoid.Set(frc::DoubleSolenoid::kForward);

    if (true) { // on actuation complete; would run a check on curtinfrc::binaryActuator once made
      SetState(kDeployed);
    }

    break;

   case kStowing:
    _config.motors.transmission->StopMotor();
    _config.solenoid.Set(frc::DoubleSolenoid::kReverse);

    if (true) { // on actuation complete; would run a check on curtinfrc::binaryActuator once made
      SetState(kStowed);
    }

    break;

   case kStowed:
    _config.motors.transmission->StopMotor(); // probably doesn't need this, but ~
    break;
  }
}
