#pragma once

#include <frc/DoubleSolenoid.h>

#include "actuators/BinaryActuator.h"

namespace wml {
  namespace actuators {
    class DoubleSolenoid : public BinaryActuator, protected frc::DoubleSolenoid {
     public:
      static constexpr double StandardActuationTime = 0.2;
      DoubleSolenoid(int forwardChannel, int reverseChannel, double actuationTime, std::string name = "<DoubleSolenoid>", BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(forwardChannel, reverseChannel), _actuationTime(actuationTime), BinaryActuator(name, initialState) {};
      DoubleSolenoid(int pcmID, int forwardChannel, int reverseChannel, double actuationTime, std::string name = "<DoubleSolenoid>", BinaryActuatorState initialState = actuators::kReverse) : frc::DoubleSolenoid(pcmID, forwardChannel, reverseChannel), _actuationTime(actuationTime), BinaryActuator(name, initialState) {};

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() final {};
      virtual bool IsDone() override;

     protected:
      virtual void Init() override;

     private:
      double _actuationTime;
      double _timer;
    };
  } // ns actuators
} // ns wml
