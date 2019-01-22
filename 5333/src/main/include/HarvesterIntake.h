#pragma once

#include "intakes/WheeledIntake.h"
#include "Gearbox.h"

#include <frc/DoubleSolenoid.h>

struct HarvesterIntakeConfig : curtinfrc::intakes::WheeledIntakeConfig {
  frc::DoubleSolenoid &solenoid;

  HarvesterIntakeConfig(curtinfrc::Gearbox &motorsIn, frc::DoubleSolenoid &solenoidiIn) : curtinfrc::intakes::WheeledIntakeConfig(motorsIn), solenoid(solenoidiIn) {};
};

class HarvesterIntake : public curtinfrc::intakes::WheeledIntake {
 public:
  HarvesterIntake(HarvesterIntakeConfig config) : WheeledIntake(config), _config(config) {};
  using IntakeState = curtinfrc::intakes::IntakeState;

 protected:
  virtual void OnStateChange(IntakeState newState, IntakeState oldState) override;

 private:
  HarvesterIntakeConfig _config;
};
