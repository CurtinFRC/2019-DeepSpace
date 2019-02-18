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
  double voltage = 0;
  
  switch (state) {
   case kManual:
    voltage = _controller.GetSetpoint();
    break;

   case kMoving:
    if (_controller.IsDone()) SetHold(); // Good enough EPS for now
   case kStationary:
    voltage = _controller.Calculate(GetHeight(), dt);
    break;

   case kZeroing:
    voltage = -2;
    
    if (_config.limitSensorBottom != nullptr) {
      if (_config.limitSensorBottom->Get()) {
        SetManual(0);
        GetConfig().spool.encoder->ZeroEncoder();
      }
    } else SetManual(0);
    break;
  }

  // Limiters
  if (_config.limitSensorTop != nullptr)
    if (voltage > 0)
      if (_config.limitSensorTop->Get())
        voltage = 0;

  if (_config.limitSensorBottom != nullptr)
    if (voltage < 0)
      if (_config.limitSensorBottom->Get()) {
        voltage = 0;
        GetConfig().spool.encoder->ZeroEncoder();
      }

  voltage = std::min(12.0, std::max(-12.0, voltage)) * 0.6;
  GetConfig().spool.transmission->SetVoltage(voltage);
}
