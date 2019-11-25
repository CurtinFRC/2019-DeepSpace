#include "control/MotorFilters.h"

using namespace wml;

control::CurrentFFFilter::CurrentFFFilter(double min, double max, Gearbox &gb)
    : _min_curr(min), _max_curr(max), _gearbox(gb) {}

double control::CurrentFFFilter::Get(double voltage) {
  double angular_vel = _gearbox.encoder->GetEncoderAngularVelocity();

  auto motor = _gearbox.motor.reduce(_gearbox.reduction);

  // V = IR + kw * w
  double voltage_min = _min_curr * motor.R() + motor.kw() * angular_vel;
  double voltage_max = _max_curr * motor.R() + motor.kw() * angular_vel;

  voltage = std::max(voltage, voltage_min);
  voltage = std::min(voltage, voltage_max);

  return voltage;
}

void control::CurrentFFFilter::SetLimits(double min, double max) {
  _min_curr = min;
  _max_curr = max;
}

static double accel_to_current(wml::Gearbox &gb, double mass, double radius, double accel) {
  // I = kt * t, t = mar
  return mass * accel * radius * gb.motor.reduce(gb.reduction).kt();
}

control::AccelerationFFFilter::AccelerationFFFilter(double min, double max, Gearbox &gb, double radius,
                                                    double mass)
    : _mass(mass), _radius(radius), CurrentFFFilter(accel_to_current(gb, mass, radius, min), accel_to_current(gb, mass, radius, max), gb) {}

void control::AccelerationFFFilter::SetLimits(double min, double max) {
  CurrentFFFilter::SetLimits(accel_to_current(_gearbox, _mass, _radius, min), accel_to_current(_gearbox, _mass, _radius, max));
}