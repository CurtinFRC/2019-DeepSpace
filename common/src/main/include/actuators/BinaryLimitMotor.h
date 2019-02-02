#pragma once

#include "BinaryActuator.h"
#include "../Gearbox.h"
#include "../sensors/BinarySensor.h"

namespace curtinfrc {
  namespace actuators {
    /**
     * @brief BinaryLimitMotor config
     * 
     */
    struct BinaryLimitMotorConfig {
      /**
       * The motor to treat as a BinaryActuator. Does not need an encoder.
       * 
       */
      Gearbox &motor;

      /**
       * @brief BinarySensor limits
       * 
       * The respective BinarySensors (e.g. Limit Switches) that trigger in kForward and kReverse.
       * 
       */
      sensors::BinarySensor &forward, &reverse;
    };

    /**
     * @brief Binary Motor (with limit switches)
     * 
     * BinaryLimitMotor class for motors that are to be treated as having two 'states'/positions,
     * where each triggers a respective BinarySensor (e.g. limit switch).
     * 
     */
    class BinaryLimitMotor : public BinaryActuator {
     public:
     /**
       * @brief Construct a new BinaryLimitMotor
       * 
       * @param config References to required objects and config settings
       * @param initialState The initial state of the BinaryLimitMotor. Defaults to kReverse.
       */
      BinaryLimitMotor(BinaryLimitMotorConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(initialState) {};

      /**
       * @brief Update the BinaryLimitMotor
       * 
       * Periodic update method. Should be called every 'tick' (or as fast/frequently as possible).
       * 
       * @param dt Time (in secs) since the function was last called.
       */
      virtual void Update(double dt) override;

      /**
       * @brief Stop the BinaryLimitMotor
       * 
       * Stops the motor. Should be called once IsDone() returns true
       * (i.e. the BinaryLimitMotor has reached the set state).
       * 
       */
      virtual void Stop() override;

      virtual bool IsDone() override;

     private:
      BinaryLimitMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
