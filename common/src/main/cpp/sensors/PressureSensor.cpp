#include "sensors/PressureSensor.h"

#include <cmath>

curtinfrc::sensors::PressureSensor::PressureSensor(int channel) : _sensor(channel) { }

double curtinfrc::sensors::PressureSensor::Get() {
  double psi = _sensor.GetValue() * 250 / 4096 - 25;
  return std::min(1.0, std::max(0.0, psi / 120));
}
