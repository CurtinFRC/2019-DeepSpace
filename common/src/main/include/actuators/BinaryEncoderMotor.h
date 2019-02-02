#pragma once

#include "BinaryActuator.h"
#include "../Gearbox.h"

namespace curtinfrc {
  namespace actuators {
    /**
     * @brief BinaryEncoderMotor config
     * 
     */
    struct BinaryEncoderMotorConfig {
      /**
       * The motor to be treated as a BinaryActuator. Must contain an encoder.
       * 
       */
      Gearbox &motor; // contains an encoder

      /**
       * @brief Encoder position(s)
       * 
       * The respective encoder positions for kForward and kReverse (in rotations).
       * 
       */
      double forward, reverse; // the values for kForward and kReverse in (encoder) rotations

      double kp, ki, kd, kf; // PID constants
    };

    /**
     * @brief Binary Motor (with encoder)
     * 
     * BinaryEncoderMotor class for motor/encoders that are to be treated as having
     * two 'states'/positions.
     * 
     */
    class BinaryEncoderMotor : public BinaryActuator {
     public:
      /**
       * @brief Construct a new BinaryEncoderMotor
       * 
       * @param config References to required objects and config settings
       * @param initialState The initial state of the BinaryEncoderMotor. Defaults to kReverse.
       */
      BinaryEncoderMotor(BinaryEncoderMotorConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(initialState) {};

      /**
       * @brief Update the BinaryEncoderMotor
       * 
       * Periodic update method. Should be called every 'tick' (or as fast/frequently as possible).
       * 
       * @param dt Time (in secs) since the function was last called.
       */
      virtual void Update(double dt) override;

      /**
       * @brief Stop the BinaryEncoderMotor
       * 
       * Stops the motor. Should be called once IsDone() returns true
       * (i.e. the BinaryEncoderMotor has reached the set state).
       * 
       */
      virtual void Stop() override;
      
      virtual bool IsDone() override;

     private:
      BinaryEncoderMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
