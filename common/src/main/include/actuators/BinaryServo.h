#pragma once

#include <frc/Servo.h>


#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace actuators {
    class BinaryServo : public BinaryActuator, protected frc::Servo {
     public:
      BinaryServo(int channel, int forwardPos, int reversePos) : frc::Servo(channel), _forwardPos(forwardPos), _reversePos(reversePos) {};
      
      virtual void UpdateActuator(double dt) override;
      virtual void Stop() final {};
      virtual bool IsDone() override;
      
     private:
      int _forwardPos, _reversePos; // Forward and Reverse positions on the servo, in degrees

    };
  } // ns actuators
} // ns curtinfrc
