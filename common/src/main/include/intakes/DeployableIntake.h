#pragma once

#include "..\StateDevice.h"
#include "..\actuators\BinaryActuator.h"


namespace curtinfrc {
  namespace intakes {
    enum DeployableIntakeState { kIntaking, kOuttaking, kDeploying, kStowing, kStowed };

    struct DeployableIntakeConfig {
      actuators::BinaryActuator &actuator;

      DeployableIntakeConfig(actuators::BinaryActuator &actuatorIn) : actuator(actuatorIn) {};
    };

    class DeployableIntake : public StateDevice<DeployableIntakeState> {
    public:
      DeployableIntake(DeployableIntakeConfig config) : _config(config) {};

    protected:
      virtual void OnStatePeriodic(DeployableIntakeState state, double dt) final;

      virtual void DeployedPeriodic(DeployableIntakeState state) {}; // Caters for both kIntaking and kOuttaking
      virtual void DeployingPeriodic() {};
      virtual void StowingPeriodic() {};
      virtual void StowedPeriodic() {};

    private:
      DeployableIntakeConfig _config;
    };
  } // ns intakes
} // ns curtinfrc
