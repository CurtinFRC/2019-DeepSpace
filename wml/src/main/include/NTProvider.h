#pragma once

#include <string>
#include <vector>

#include <networktables/NetworkTableInstance.h>

#include "sensors/BinarySensor.h"
#include "sensors/DoubleSensor.h"

#include "devices/StateDevice.h"

namespace wml {
  class NTProvider {
   public:
    NTProvider(std::shared_ptr<nt::NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("Robot Data")) : _table(table) {};

    void Update();

    void Register(sensors::BinarySensor *binarySensor);
    void Register(sensors::DoubleSensor *doubleSensor);

    void Register(devices::StateDeviceBase *stateDevice);

   private:
    std::shared_ptr<nt::NetworkTable> _table;

    std::vector<sensors::BinarySensor*> _binarySensors;
    std::vector<sensors::DoubleSensor*> _doubleSensors;

    std::vector<devices::StateDeviceBase*> _stateDevices;
  };
}
