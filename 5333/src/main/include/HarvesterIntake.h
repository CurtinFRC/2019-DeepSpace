#pragma once

#include "intakes/WheeledIntake.h"
#include "StateDevice.h"
#include "Gearbox.h"

#include <frc/DoubleSolenoid.h>

enum HarvesterIntakeState { kDeployed, kDeploying, kStowing, kStowed };

struct HarvesterIntakeConfig : curtinfrc::intakes::WheeledIntakeConfig {
  frc::DoubleSolenoid &solenoid;

  HarvesterIntakeConfig(curtinfrc::Gearbox &motorsIn, frc::DoubleSolenoid &solenoidiIn) : curtinfrc::intakes::WheeledIntakeConfig(motorsIn), solenoid(solenoidiIn) {};
};

class HarvesterIntake : public curtinfrc::StateDevice<HarvesterIntakeState> {
 public:
  HarvesterIntake(HarvesterIntakeConfig config) : _config(config) {};

 protected:
  virtual void OnStatePeriodic(HarvesterIntakeState state, double dt) override;

 private:
  HarvesterIntakeConfig _config;
};
