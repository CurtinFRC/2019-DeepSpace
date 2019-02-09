#include "control/MotorFilters.h"

using namespace curtinfrc;

control::AccelerationFilter::AccelerationFilter(double min, double max, Gearbox &gb, double radius,
                                                double mass)
    : _min_torque(mass * min * radius),
      _max_torque(mass * max * radius),
      _radius(radius),
      _mass(mass),
      _gearbox(gb) {}

double control::AccelerationFilter::Get(double voltage) {
  double angular_vel = _gearbox.encoder->GetEncoderAngularVelocity();

  auto motor = _gearbox.motor.reduce(_gearbox.reduction);

  double current_min = _min_torque * motor.kt();
  double current_max = _max_torque * motor.kt();

  // V = IR + kw * w
  double voltage_min = current_min * motor.R() + motor.kw() * angular_vel;
  double voltage_max = current_max * motor.R() + motor.kw() * angular_vel;

  voltage = std::max(voltage, voltage_min);
  voltage = std::min(voltage, voltage_max);

  return voltage;
}
