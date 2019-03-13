#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace actuators {
    class DoubleSolenoid : public BinaryActuator, protected frc::DoubleSolenoid {
     public:
      DoubleSolenoid(int forwardChannel, int reverseChannel, std::string name = "<DoubleSolenoid>", BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), BinaryActuator(name, initialState) {};
      DoubleSolenoid(int pcmID, int forwardChannel, int reverseChannel, std::string name = "<DoubleSolenoid>", BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(pcmID, forwardChannel, reverseChannel), BinaryActuator(name, initialState) {};

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() final {};
    };
  } // ns actuators
} // ns curtinfrc
