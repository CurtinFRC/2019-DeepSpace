#pragma once

#include <frc/Compressor.h>

#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace actuators {
    class Compressor : public BinaryActuator, protected frc::Compressor {
      public:
      Compressor(std::string name = "<Compressor>", BinaryActuatorState initialState = actuators::kReverse) : frc::Compressor(), BinaryActuator(name, initialState) { SetClosedLoopControl(false); };
      Compressor(int pcmID, std::string name = "<Compressor>", BinaryActuatorState initialState = actuators::kReverse) : frc::Compressor(pcmID), BinaryActuator(name, initialState) { SetClosedLoopControl(false); };

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() final {};
      virtual bool IsDone() override;
    };
  } // ns actuators
} // ns curtinfrc
