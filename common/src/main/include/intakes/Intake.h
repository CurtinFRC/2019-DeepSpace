#pragma once

#include "StateDevice.h"

namespace curtinfrc {
  namespace intakes {
    enum IntakeState { kIn, kOut, kOff };
    template <typename StateType = IntakeState>
    class Intake : public StateDevice<StateType> {
     public:
      virtual void OnStateChange(StateType newState, StateType oldState) override {};
      virtual void OnStatePeriodic(StateType state, double dt) override = 0;

      StateType GetState() { return _state; };
      // StateType GetLastState() { return _lastState; };

     private:
      StateType _state, _lastState;
    };
  } // ns intakes
} // ns curtinfrc
