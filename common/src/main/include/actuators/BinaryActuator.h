#pragma once

namespace curtinfrc {
  namespace actuators {
    enum BinaryActuatorState { kForward, kReverse };

    class BinaryActuator { // might implement curtinfrc::StateDevice<BinaryActuatorState> in future?
     public:
      BinaryActuator(BinaryActuatorState initialState = kReverse) : _state(initialState) {};
      using ActuatorState = BinaryActuatorState;

      void SetTarget(BinaryActuatorState state) { SetState(state); Init(); };
      virtual void Update(double dt) = 0;
      virtual void Stop() = 0;
      virtual bool IsDone() = 0;

      BinaryActuatorState Get() { return _state; };

     protected:
      void SetState(BinaryActuatorState state) { _state = state; };
      BinaryActuatorState _state;

      virtual void Init() {};
    };
  } // ns actuators
} // ns curtinfrc
