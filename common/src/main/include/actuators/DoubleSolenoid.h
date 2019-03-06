#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace actuators {
    class DoubleSolenoid : public BinaryActuator, protected frc::DoubleSolenoid {
     public:
      DoubleSolenoid(int forwardChannel, int reverseChannel, BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), BinaryActuator(initialState) {};
      DoubleSolenoid(int pcmID, int forwardChannel, int reverseChannel, BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(pcmID, forwardChannel, reverseChannel), BinaryActuator(initialState) {};

      virtual void Update(double dt) override;
      virtual void Stop() final {};
    };
  } // ns actuators
} // ns curtinfrc
