#pragma once

namespace curtinfrc {
  template <typename StateType>
  class StateDevice {
   public:
    void Update(double dt) {
      if (_state != _lastState) {
        OnStateChange(_state, _lastState);
        _lastState = _state;
      }

      OnStatePeriodic(_state, dt);
    };
    
    virtual void OnStateChange(StateType newState, StateType oldState) {};
    virtual void OnStatePeriodic(StateType state, double dt) = 0;

   protected:
    void SetState(StateType state) { _state = state; };
    StateType _state, _lastState;
  };
} // ns curtinfrc
