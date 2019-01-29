#pragma once

namespace curtinfrc {
  namespace actuators {
    enum BinaryActuatorState { kForward, kReverse };

    class BinaryActuator { // might implement curtinfrc::StateDevice<BinaryActuatorState> in future?
     public:
      BinaryActuator(BinaryActuatorState initialState = kReverse) : _state(initialState) {};
      
      virtual bool Actuate() = 0;
      virtual bool Done() = 0;

      void SetState(BinaryActuatorState state) { _state = state; };
      BinaryActuatorState Get() { return _state; };

     protected:
      BinaryActuatorState _state;
    };
  } // ns actuators
} // ns curtinfrc
