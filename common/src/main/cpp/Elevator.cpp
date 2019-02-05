#include "Elevator.h"

#include <iostream>

// public

void curtinfrc::Elevator::SetManual(double power) {
  SetState(kManual);
  _setpoint = power;
}

void curtinfrc::Elevator::SetSetpoint(double setpoint) {
  SetState(kMoving);
  _setpoint = setpoint;
}

void curtinfrc::Elevator::SetZeroing() {
  SetState(kZeroing);
  _setpoint = 0;
}

void curtinfrc::Elevator::SetHold() {
  SetState(kManual);
  _setpoint = 0;
}


double curtinfrc::Elevator::GetSetpoint() {
  return _setpoint;
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

void curtinfrc::Elevator::OnStatePeriodic(curtinfrc::ElevatorState state, double dt) { // Good enough default
  double power = 0;
  double goal;
  double height;
  double kP = 30;
  double kI = 0;
  double kD = 0;
  double integral;
  double derivative;
  double error;
  double lastError;
  double voltage;
  
  switch (state) {
   case kManual:
    power = GetSetpoint();
    break;

   case kMoving:
    goal = GetSetpoint();
    height = GetHeight();

    error = goal - height;
    integral += error * dt;
    if(dt != 0) {
      derivative = (error - 1) / dt;
    } else {
      derivative = 0;
    }
    voltage = kP * error + kI * integral + kD * derivative;
    power = voltage / 12;
    
    // lastError = error;
    break;

   case kStationary:
    power = 0; // PID to hold position
    break;

   case kZeroing:
    power = -0.6;
    
    if (_config.limitSensorBottom != nullptr) {
      if (_config.limitSensorBottom->Get()) {
        SetState(kStationary);
        GetConfig().spool.encoder->ZeroEncoder();
      }
    }
    break;
  }

  if (_config.limitSensorTop != nullptr)
    if (power > 0)
      if (_config.limitSensorTop->Get())
        power = 0;

  if (_config.limitSensorBottom != nullptr)
    if (power < 0)
      if (_config.limitSensorBottom->Get())
        power = 0;

  GetConfig().spool.transmission->Set(power);
}
