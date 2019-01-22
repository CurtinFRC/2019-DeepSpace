#include "HatchIntake.h"

void HatchIntake::Set(HatchIntakeState state) {
  SetState(state);
}

void HatchIntake::OnStateChange(HatchIntakeState newState, HatchIntakeState lastState) {
  // Check if valid state change
  switch (newState) {
   case kGrab:
    switch (lastState) {
     case kAim:
      SetState(_config.limitSensorDown.Get() ? kGrab : kAim);
      break;

     default:
      Set(kAim);
      break;
    }
    break;

   case kEject:
    switch (lastState) {
     case kHold:
      SetState(_config.limitSensorUp.Get() ? kEject : kHold);
      break;

     default:
      Set(kHold);
      break;
    }
    break;

   case kAim:
    switch (lastState) {
     case kGrab:
     case kHold:
      SetState(kAim);
      break;

     default:
      Set(kHold);
      break;
    }
    break;

   case kHold:
    switch (lastState) {
     default:
      SetState(kHold);
      break;
    }
    break;
  }

  if (GetState() != newState) OnStateChange(GetState(), lastState);
}

void HatchIntake::OnStatePeriodic(HatchIntakeState state, double dt) {
  switch (state) { // TODO
   case kGrab:
    
    break;

   case kEject:
    break;

   case kAim:
    break;

   case kHold:
    break;
  }
}
