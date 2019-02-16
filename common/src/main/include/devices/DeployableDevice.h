#pragma once

#include "devices/StateDevice.h"
#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace devices {
    enum DeployableDeviceState { kStowed = 0, kStowing, kDeploying, kOuttaking, kIntaking };

    struct DeployableDeviceConfig {
      actuators::BinaryActuator &actuator;
      const bool canEject;

      DeployableDeviceConfig(actuators::BinaryActuator &actuatorIn, bool canEjectIn = false) : actuator(actuatorIn), canEject(canEjectIn) {};
    };

    class DeployableDevice : public StateDevice<DeployableDeviceState> {
     public:
      DeployableDevice(DeployableDeviceConfig config) : _config(config) {};

      virtual void SetIntaking();
      virtual void SetOuttaking();
      virtual void SetStowed();

     protected:
      virtual void OnStatePeriodic(DeployableDeviceState state, double dt) final;

      virtual void IntakingPeriodic() {};
      virtual void OuttakingPeriodic() {};
      virtual void DeployingPeriodic() {};
      virtual void StowingPeriodic() {};
      virtual void StowedPeriodic() {};

     private:
      DeployableDeviceConfig _config;
    };
  } // ns intakes
} // ns curtinfrc
