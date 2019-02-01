#pragma once

#include "Gearbox.h"

// #include "Intake.h"
// #include "../sensors/BinarySensor.h"

namespace curtinfrc {
  namespace intakes {
    struct WheeledIntakeConfig  {
      Gearbox &motors;

      WheeledIntakeConfig(Gearbox &motorsIn) : motors(motorsIn) {};
    };

//     class WheeledIntake : public Intake<> {
//      public:
//     WheeledIntake(WheeledIntakeConfig config) : _config(config) {};

//      void Set(IntakeState State);

//      protected:
//       virtual void OnStatePeriodic(IntakeState state, double dt) override;

//      private:
//       WheeledIntakeConfig _config;
//     };
  } // ns intakes
} // ns curtinfrc
