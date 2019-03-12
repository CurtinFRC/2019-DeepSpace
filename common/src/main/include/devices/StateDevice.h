#pragma once

#include <string>

#include "loops/LoopSystem.h"

namespace curtinfrc {
  namespace devices {
    class RawStateDevice {
     public:
      RawStateDevice(std::string name = "<no name>") : _name(name) {};

      virtual void Update(double dt) = 0; // unused rn
      virtual std::string GetState() = 0;

      std::string GetName() { return _name; };
    
     private:
      std::string _name;
    };

    template <typename StateType>
    class StateDevice : public loops::LoopSystem {
     public:
      StateDevice(StateType initialState = (StateType)0) : _state(initialState) {};

      virtual RawStateDevice *MakeRawStateDevice(std::string name = "<no name>") = 0;


      void Update(double dt) final {
        if (_state != _lastState) {
          OnStateChange(_state, _lastState);
          _lastState = _state;
        }

        OnStatePeriodic(_state, dt);
      };

      StateType GetState() { return _state; };

     protected:
      void SetState(StateType state) { _state = state; };
      StateType _state, _lastState;

      virtual void OnStateChange(StateType newState, StateType oldState){};
      virtual void OnStatePeriodic(StateType state, double dt) = 0;
    };
  }  // namespace devices
}  // namespace curtinfrc
