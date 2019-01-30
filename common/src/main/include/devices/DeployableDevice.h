#pragma once

#include "devices/StateDevice.h"
#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace devices {
    enum DeployableDeviceState { kIntaking, kOuttaking, kDeploying, kStowing, kStowed };

    struct DeployableDeviceConfig {
      actuators::BinaryActuator &actuator;

      DeployableDeviceConfig(actuators::BinaryActuator &actuatorIn) : actuator(actuatorIn) {};
    };

    class DeployableDevice : public StateDevice<DeployableDeviceState> {
    public:
      DeployableDevice(DeployableDeviceConfig config) : _config(config) {};

    protected:
      virtual void OnStatePeriodic(DeployableDeviceState state, double dt) final;

      virtual void DeployedPeriodic(DeployableDeviceState state) {}; // Caters for both kIntaking and kOuttaking
      virtual void DeployingPeriodic() {};
      virtual void StowingPeriodic() {};
      virtual void StowedPeriodic() {};

    private:
      DeployableDeviceConfig _config;
    };
  } // ns intakes
} // ns curtinfrc
