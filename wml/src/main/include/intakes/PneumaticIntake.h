// #pragma once

// #include <frc/DoubleSolenoid.h>

// #include "../sensors/BinarySensor.h"

// namespace wml {
//   namespace intakes {
//     struct PneumaticIntakeConfig {
//       frc::DoubleSolenoid &solenoids;
//       sensors::BinarySensor *limitSensorOpen;
//       sensors::BinarySensor *limitSensorClosed;
//     };

//     class PneumaticIntake : public StateDevice<> {
//      public:
//       PneumaticIntake(PneumaticIntakeConfig config) : _config(config) {};

//       void Set(IntakeState state);

//      protected:
//       virtual void OnStatePeriodic(IntakeState state, double dt) override;

//      private:
//       PneumaticIntakeConfig _config;
//     };
//   } // ns intakes
// } // ns wml
