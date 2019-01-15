#include "Elevator.h"

void curtinfrc::Elevator::Set(double power) {
  if (_config.limitSensorTop != nullptr)
    if (power > 0)
      if (_config.limitSensorTop->Get())
        power = 0;

  if (_config.limitSensorBottom != nullptr)
    if (power < 0)
      if (_config.limitSensorBottom->Get())
        power = 0;

  _config.spool.transmission->Set(power);
}
