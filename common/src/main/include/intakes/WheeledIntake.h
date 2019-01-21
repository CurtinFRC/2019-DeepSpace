#pragma once

#include "SensoredTransmission.h"

#include "Intake.h"
#include "../sensors/BinarySensor.h"

namespace curtinfrc {
  namespace intakes {
    struct WheeledIntakeConfig  {
      SensoredTransmission &motors;
      sensors::BinarySensor *limitSensorOpen;
      sensors::BinarySensor *limitSensorClosed;
    };

    class WheeledIntake : public Intake {
     public:
    WheeledIntake(WheeledIntakeConfig config) : _config(config) {};

     void Set(IntakeState State);

     protected:
      virtual void OnStatePeriodic(IntakeState state, double dt) override;

     private:
      WheeledIntakeConfig _config;
    };
  } // ns intakes
} // ns curtinfrc
