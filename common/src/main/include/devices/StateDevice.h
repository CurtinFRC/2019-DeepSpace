#pragma once

#include "loops/LoopSystem.h"

namespace curtinfrc {
namespace devices {

  template <typename StateType>
  class StateDevice : public loops::LoopSystem {
   public:
    StateDevice(StateType initialState = (StateType)0) : _state(initialState) {};

    void Update(double dt) override {
      if (_state != _lastState) {
        OnStateChange(_state, _lastState);
        _lastState = _state;
      }

      OnStatePeriodic(_state, dt);
    };

    StateType GetState() {
      return _state;
    }

   protected:
    void      SetState(StateType state) { _state = state; };
    StateType _state, _lastState;

    virtual void OnStateChange(StateType newState, StateType oldState){};
    virtual void OnStatePeriodic(StateType state, double dt) = 0;
  };

}  // namespace devices
}  // namespace curtinfrc
