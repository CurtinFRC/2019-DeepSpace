#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"
#include "Gearbox.h"

namespace curtinfrc {
  namespace actuators {
    /**
     * @brief DoubleSolenoid
     * 
     * DoubleSolenoid class that extends frc::DoubleSolenoid by implementing BinaryActuator.
     * Note: Since DoubleSolenoid implements BinaryActuator curtinfrc::BinaryActuatorState
     *       is used instead of frc::DoubleSolenoid::Value to designate states.
     * 
     */
    class DoubleSolenoid : public BinaryActuator, protected frc::DoubleSolenoid {
     public:
      /**
       * @brief Construct a new DoubleSolenoid
       * 
       * @param forwardChannel The forward channel number on the PCM (0..7).
       * @param reverseChannel The reverse channel number on the PCM (0..7).
       * @param initialState The initial state of the DoubleSolenoid. Defaults to kReverse.
       */
      DoubleSolenoid(int forwardChannel, int reverseChannel, BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), BinaryActuator(initialState) {};

      virtual void Update(double dt) override;
      virtual void Stop() final {};

      /**
       * @brief Query completion of DoubleSolenoid
       * 
       * DoubleSolenoid::IsDone exists due to the implementation of BinaryActuator,
       * however it always returns true as solenoids are "infinitely fast".
       * 
       * @return true
       */
      virtual bool IsDone() override;
    };
  } // ns actuators
} // ns curtinfrc
