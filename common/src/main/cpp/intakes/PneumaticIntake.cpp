#include "intakes/PneumaticIntake.h"

void curtinfrc::intakes::PneumaticIntake::Set(curtinfrc::intakes::IntakeState state) {
  SetState(state);
}
void curtinfrc::intakes::PneumaticIntake::OnStatePeriodic(curtinfrc::intakes::IntakeState state, double dt) {
  switch(state){
    case kIn:
    _config.solenoids.Set(frc::DoubleSolenoid::kForward);
    break;

    case kOut:
    case kOff:
    _config.solenoids.Set(frc::DoubleSolenoid::kReverse);
    break;
  }
}
