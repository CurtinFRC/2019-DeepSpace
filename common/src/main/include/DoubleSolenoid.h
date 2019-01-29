#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"
#include "Gearbox.h"

namespace curtinfrc {
  class DoubleSolenoid : public actuators::BinaryActuator, protected frc::DoubleSolenoid {
    public:
    DoubleSolenoid(int forwardChannel, int reverseChannel, actuators::BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), BinaryActuator(initialState) {};

    virtual bool Actuate() override;
    virtual bool Done() override;
  };
} // ns curtinfrc
