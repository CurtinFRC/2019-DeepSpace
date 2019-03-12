#include "Elevator.h"

#include <iostream>
#include <cmath>

// public

std::string curtinfrc::RawElevator::GetState() {
  switch (_elevator->GetState()) {
   case curtinfrc::ElevatorState::kStationary:
    return "kStationary";
    
   case curtinfrc::ElevatorState::kMoving:
    return "kMoving";

   case curtinfrc::ElevatorState::kZeroing:
    return "kZeroing";
    
   case curtinfrc::ElevatorState::kManual:
    return "kManual";
  }

  return "<state error>";
}


curtinfrc::devices::RawStateDevice *curtinfrc::Elevator::MakeRawStateDevice(std::string name) {
  return new curtinfrc::RawElevator(this, name);
}

void curtinfrc::Elevator::SetManual(double power) {
  SetState(curtinfrc::ElevatorState::kManual);
  _controller.SetSetpoint(power);
}

void curtinfrc::Elevator::SetSetpoint(double setpoint) {
  SetState(curtinfrc::ElevatorState::kMoving);
  _controller.SetSetpoint(setpoint);
}

void curtinfrc::Elevator::SetZeroing() { // Reset encoder to zero
  SetState(curtinfrc::ElevatorState::kZeroing);
  _controller.SetSetpoint(0);
}

void curtinfrc::Elevator::SetHold() {
  SetState(curtinfrc::ElevatorState::kStationary);
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

double curtinfrc::Elevator::GetFeedforward() {
  // V = IR, I = kt * t, I = kt * m * a * r
  return _config.spool.motor.kt() * _config.mass * -9.81 * _config.spoolRadius;
}

// virtual

void curtinfrc::Elevator::OnStatePeriodic(curtinfrc::ElevatorState state, double dt) {
  double voltage = 0;
  
  switch (state) {
   case curtinfrc::ElevatorState::kManual:
    voltage = _controller.GetSetpoint();
    break;

   case curtinfrc::ElevatorState::kMoving:
    if (_controller.IsDone()) SetHold(); // Good enough EPS for now
   case curtinfrc::ElevatorState::kStationary:
    voltage = _controller.Calculate(GetHeight(), dt, GetFeedforward());
    break;

   case curtinfrc::ElevatorState::kZeroing:
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
