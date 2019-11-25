#include "Elevator.h"

#include <iostream>
#include <cmath>

// public

std::string wml::Elevator::GetStateString() {
  switch (GetState()) {
   case wml::ElevatorState::kStationary:
    return "kStationary";
    
   case wml::ElevatorState::kMoving:
    return "kMoving";

   case wml::ElevatorState::kZeroing:
    return "kZeroing";
    
   case wml::ElevatorState::kManual:
    return "kManual";
  }

  return "<state error>";
}


void wml::Elevator::SetManual(double power) {
  SetState(wml::ElevatorState::kManual);
  _controller.SetSetpoint(power);
}

void wml::Elevator::SetSetpoint(double setpoint) {
  SetState(wml::ElevatorState::kMoving);
  _controller.SetSetpoint(setpoint);
}

void wml::Elevator::SetZeroing() { // Reset encoder to zero
  SetState(wml::ElevatorState::kZeroing);
  _controller.SetSetpoint(0);
}

void wml::Elevator::SetHold() {
  SetState(wml::ElevatorState::kStationary);
  _controller.SetSetpoint(GetHeight() + 0.1);
}


double wml::Elevator::GetSetpoint() {
  return _controller.GetSetpoint();
}

double wml::Elevator::GetHeight() {
  double radius = _config.spoolRadius;
  double rotations = _config.spool.encoder->GetEncoderRotations();
  double height = 6.283 * radius * rotations;
  return height;
}

wml::ElevatorConfig &wml::Elevator::GetConfig() {
  return _config;
}

double wml::Elevator::GetFeedforward() {
  // V = IR, I = kt * t, I = kt * m * a * r
  return _config.spool.motor.kt() * _config.mass * -9.81 * _config.spoolRadius;
}

// virtual

void wml::Elevator::OnStatePeriodic(wml::ElevatorState state, double dt) {
  double voltage = 0;
  
  switch (state) {
   case wml::ElevatorState::kManual:
    voltage = _controller.GetSetpoint();
    break;

   case wml::ElevatorState::kMoving:
    if (_controller.IsDone()) SetHold(); // Good enough EPS for now
   case wml::ElevatorState::kStationary:
    voltage = _controller.Calculate(GetHeight(), dt, GetFeedforward());
    break;

   case wml::ElevatorState::kZeroing:
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

  voltage = std::min(12.0, std::max(-12.0, voltage)) * 0.7;
  voltage = _current_filter.Get(voltage);
  GetConfig().spool.transmission->SetVoltage(voltage);
}
