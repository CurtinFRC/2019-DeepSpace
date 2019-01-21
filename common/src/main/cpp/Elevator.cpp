#include "Elevator.h"

// public

void curtinfrc::Elevator::SetManual(double setpoint) {
  SetState(kManual);
  _setpoint = setpoint;
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

curtinfrc::ElevatorConfig &curtinfrc::Elevator::GetConfig() {
  return _config;
}


// virtual

void curtinfrc::Elevator::OnStatePeriodic(curtinfrc::ElevatorState state, double dt) { // Good enough default
  double power = 0;

  switch (state) {
   case kManual:
    power = GetSetpoint();
    break;

   case kMoving:
    power = 0; // Motion profiling/PID stuff
    break;

   case kStationary:
    power = 0; // PID to hold position
    break;

   case kZeroing:
    power = -0.6;
    
    if (_config.limitSensorBottom != nullptr) {
      if (_config.limitSensorBottom->Get()) {
        SetState(kStationary);
        GetConfig().spool.encoder->ResetEncoder();
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
