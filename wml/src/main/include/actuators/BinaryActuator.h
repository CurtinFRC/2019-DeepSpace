#pragma once

#include <string>

#include "devices/StateDevice.h"

namespace curtinfrc {
  namespace actuators {
    enum BinaryActuatorState { kReverse = 0, kForward };

    class BinaryActuator : public devices::StateDevice<BinaryActuatorState> {
     public:
      BinaryActuator(std::string name = "<Binary Actuator>", BinaryActuatorState initialState = kReverse) : StateDevice(name, initialState) {};
      using ActuatorState = BinaryActuatorState;

      virtual std::string GetStateString() final {
        switch (Get()) {
         case kReverse:
          return "kReverse";

         case kForward:
          return "kForward";
        }

        return "<state error>";
      };

      void SetTarget(BinaryActuatorState state) { SetState(state); if (_lastState != _state) Init(); };
      virtual void UpdateActuator(double dt) = 0;
      virtual void Stop() {};
      virtual bool IsDone() { return true; };

      BinaryActuatorState Get() { return GetState(); };

     protected:
      virtual void Init() {};

     private:
      virtual void OnStateChange(BinaryActuatorState newState, BinaryActuatorState oldState) { Init(); };
      virtual void OnStatePeriodic(BinaryActuatorState state, double dt) { UpdateActuator(dt); if (IsDone()) Stop(); };
    };
  } // ns actuators
} // ns curtinfrc
