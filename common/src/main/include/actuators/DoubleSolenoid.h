#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace actuators {
    class DoubleSolenoid : public BinaryActuator, protected frc::DoubleSolenoid {
     public:
      DoubleSolenoid(int forwardChannel, int reverseChannel, BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), BinaryActuator(initialState) {};
      DoubleSolenoid(int pcmID, int forwardChannel, int reverseChannel, BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(pcmID, forwardChannel, reverseChannel), BinaryActuator(initialState) {};

      virtual devices::RawStateDevice *MakeRawStateDevice(std::string name = "<DoubleSolenoid>") override {
        return BinaryActuator::MakeRawStateDevice(name);
      }

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() final {};
    };
  } // ns actuators
} // ns curtinfrc
