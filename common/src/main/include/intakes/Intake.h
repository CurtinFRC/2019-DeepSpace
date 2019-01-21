#pragma once

#include "StateDevice.h"

namespace curtinfrc {
  namespace intakes {
    enum IntakeState { kIn, kOut, kOff };
    class Intake : public StateDevice<IntakeState> {
    public:
      virtual void OnStateChange(IntakeState newState, IntakeState oldState) override {};
      virtual void OnStatePeriodic(IntakeState state, double dt) override = 0;

      private:
      IntakeState _state;
    };
  } // ns intakes
} // ns curtinfrc
