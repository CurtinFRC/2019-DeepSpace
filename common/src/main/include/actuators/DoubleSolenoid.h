#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"
#include "Gearbox.h"

namespace curtinfrc {
  namespace actuators {
    class DoubleSolenoid : public BinaryActuator, protected frc::DoubleSolenoid {
      public:
      DoubleSolenoid(int forwardChannel, int reverseChannel, BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), BinaryActuator(initialState) {};

      virtual void Update() override;
      virtual void Stop() final {};
      virtual bool IsDone() override;
    };
  } // ns actuators
} // ns curtinfrc
