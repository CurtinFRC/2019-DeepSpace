#include "Elevator.h"

#include <iostream>
#include <cmath>

// public

void curtinfrc::Elevator::SetManual(double power) {
  SetState(kManual);
  _controller.SetSetpoint(power);
}

void curtinfrc::Elevator::SetSetpoint(double setpoint) {
  SetState(kMoving);
  _controller.SetSetpoint(setpoint);
}

void curtinfrc::Elevator::SetZeroing() { // Reset encoder to zero
  SetState(kZeroing);
  _controller.SetSetpoint(0);
}

void curtinfrc::Elevator::SetHold() {
  SetState(kStationary);
  _controller.SetSetpoint(GetHeight() + 0.1);
}


double curtinfrc::Elevator::GetSetpoint() {
  return _controller.GetSetpoint();
}

double curtinfrc::Elevator::GetHeight() {
  double radius = _config.spoolRadius;
  double rotations = _config.spool.encoder->GetEncoderRotations();
  double height = 6.283 * radius * rotations;
  return height;
}

curtinfrc::ElevatorConfig &curtinfrc::Elevator::GetConfig() {
  return _config;
}

// virtual

void curtinfrc::Elevator::OnStatePeriodic(curtinfrc::ElevatorState state, double dt) {
  double power = 0;
  
  switch (state) {
   case kManual:
    power = _controller.GetSetpoint();
    break;

   case kMoving:
    if (_controller.IsDone()) SetHold(); // Good enough EPS for now
   case kStationary:
    power = _controller.Calculate(GetHeight(), dt);
    break;

   case kZeroing:
    power = -0.6;
    
    if (_config.limitSensorBottom != nullptr) {
      if (_config.limitSensorBottom->Get()) {
        SetManual(0);
        GetConfig().spool.encoder->ZeroEncoder();
      }
    } else power = -0.25;
    break;
  }

  // Limiters
  if (_config.limitSensorTop != nullptr)
    if (power > 0)
      if (_config.limitSensorTop->Get())
        power = 0;

  if (_config.limitSensorBottom != nullptr)
    if (power < 0)
      if (_config.limitSensorBottom->Get()) {
        power = 0;
        GetConfig().spool.encoder->ZeroEncoder();
      }

  power = std::min(1.0, std::max(-1.0, power)) * 0.6;
  GetConfig().spool.transmission->Set(power);
}
