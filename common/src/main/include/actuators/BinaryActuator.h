#pragma once

#include <string>

#include "devices/StateDevice.h"

namespace curtinfrc {
  namespace actuators {
    enum BinaryActuatorState { kReverse = 0, kForward };

    class BinaryActuator : public devices::StateDevice<BinaryActuatorState> {
     public:
      BinaryActuator(BinaryActuatorState initialState = kReverse) : StateDevice(initialState) {};
      using ActuatorState = BinaryActuatorState;

      virtual devices::RawStateDevice *MakeRawStateDevice(std::string name = "<Binary Actuator>") override;

      void SetTarget(BinaryActuatorState state) { SetState(state); Init(); };
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

    class RawBinaryActuator : public devices::RawStateDevice {
     public:
      RawBinaryActuator(BinaryActuator *actuator, std::string name = "<Binary Actuator>") : RawStateDevice(name), _actuator(actuator) {};

      virtual void Update(double dt) { _actuator->Update(dt); };
      virtual std::string GetState() {
        switch (_actuator->Get()) {
         case kReverse:
          return "kReverse";

         case kForward:
          return "kForward";
        }

        return "<state error>";
      };

     private:
      BinaryActuator *_actuator;
    };
  } // ns actuators
} // ns curtinfrc
