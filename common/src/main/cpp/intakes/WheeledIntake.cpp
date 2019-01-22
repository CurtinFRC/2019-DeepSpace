#include "intakes/WheeledIntake.h"

void curtinfrc::intakes::WheeledIntake::Set(curtinfrc::intakes::IntakeState state) {
  SetState(state);
}

void curtinfrc::intakes::WheeledIntake::OnStatePeriodic(curtinfrc::intakes::IntakeState state, double dt) {
  switch(state){
    case kIn:
    _config.motors.transmission->Set(1);
    break;

    case kOut:
     _config.motors.transmission->Set(-1);
     break;

    case kOff:
     _config.motors.transmission->Set(0);
     break;
  }
}
