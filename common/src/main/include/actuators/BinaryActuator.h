#pragma once

#include <string>

#include "devices/StateDevice.h"

namespace curtinfrc {
  namespace actuators {
    enum BinaryActuatorState { kReverse = 0, kForward };

    /**
     * @brief Generic StateDevice for Binary Actuators (2 digital states).
     */
    class BinaryActuator : public devices::StateDevice<BinaryActuatorState> {
     public:
      /**
       * @brief Construct a new Binary Actuator object.
       * 
       * Note that BinaryActuator should not be construced itself and this is only a default constructor
       * that passes values through to the StateDevice.
       * 
       * @param name The name of the actuator. If a extending this class, should default to the name of the class. If an instance of a subclass, then should describe the actuator's purpose.
       * @param initialState The starting state of the actuator.
       */
      BinaryActuator(std::string name = "<Binary Actuator>", BinaryActuatorState initialState = kReverse) : StateDevice(name, initialState) {};
      using ActuatorState = BinaryActuatorState;

      /**
       * @brief Get the state String.
       * 
       * Final override of virtual StateDeviceBase::GetStateString().
       * 
       * @return std::string The current state as a String.
       */
      virtual std::string GetStateString() final {
        switch (Get()) {
         case kReverse:
          return "kReverse";

         case kForward:
          return "kForward";
        }

        return "<state error>";
      };

      /**
       * @brief Set the actuator Target.
       * 
       * @param state The desired actuator state.
       */
      void SetTarget(BinaryActuatorState state) { SetState(state); if (_lastState != _state) Init(); };

      /**
       * @brief Update the actuator
       * 
       * Should be overwritten with any code that needs to be executed continuously during the Actuator's movement.
       * Should be called every cycle.
       * 
       * @param dt The time since the last call to the function (typically the cycle time).
       */
      virtual void UpdateActuator(double dt) = 0;

      /**
       * @brief Stop the actuator.
       * 
       * Should be overwritten with code required to stop the actuator, and called once when IsDone is true.
       */
      virtual void Stop() {};

      /**
       * @brief Check if the actuator is done 'actuating'.
       * 
       * @return true 
       * @return false 
       */
      virtual bool IsDone() { return true; };

      BinaryActuatorState Get() { return GetState(); };

     protected:
      /**
       * @brief Init actuator movement
       * 
       * Called whenever the actuator state changes. Override it with any initialisation (e.g. resetting encoders)
       * required when the actuator moves.
       */
      virtual void Init() {};

     private:
      virtual void OnStateChange(BinaryActuatorState newState, BinaryActuatorState oldState) final { Init(); };
      virtual void OnStatePeriodic(BinaryActuatorState state, double dt) final { UpdateActuator(dt); if (IsDone()) Stop(); };
    };
  } // ns actuators
} // ns curtinfrc
