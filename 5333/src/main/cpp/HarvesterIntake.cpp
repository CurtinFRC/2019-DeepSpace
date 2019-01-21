#include "HarvesterIntake.h"

void HarvesterIntake::OnStateChange(IntakeState newState, IntakeState oldState) {
  switch (newState) {
   case IntakeState::kIn:
   case IntakeState::kOut:
    _config.solenoid.Set(frc::DoubleSolenoid::kForward);
    break;

   case IntakeState::kOff:
    _config.solenoid.Set(frc::DoubleSolenoid::kReverse);
    break;
  }
}
