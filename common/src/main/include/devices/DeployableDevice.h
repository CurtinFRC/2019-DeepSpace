#pragma once

#include <string>

#include "devices/StateDevice.h"
#include "actuators/BinaryActuator.h"

namespace curtinfrc {
  namespace devices {
    enum DeployableDeviceState { kStowed = 0, kStowing, kDeploying, kOuttaking, kIntaking };

    struct DeployableDeviceConfig {
      std::string name;

      actuators::BinaryActuator &actuator;
      const bool canEject;

      DeployableDeviceConfig(actuators::BinaryActuator &actuatorIn, bool canEjectIn = false, std::string nameIn = "<Deployable Device>") : actuator(actuatorIn), name(nameIn), canEject(canEjectIn) {};
    };

    class DeployableDevice : public StateDevice<DeployableDeviceState> {
     public:
      DeployableDevice(DeployableDeviceConfig config) : StateDevice(config.name), _config(config) {};

      virtual std::string GetStateString() final;

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
