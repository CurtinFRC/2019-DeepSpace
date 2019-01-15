#pragma once

#include "SensoredTransmission.h"
#include "sensors/BinarySensor.h"

namespace curtinfrc {
  struct ElevatorConfig {
    SensoredTransmission &spool;
    double spoolRadius; // Plus average thickness of rope wrapped around spool...?
    sensors::BinarySensor *limitSensorTop;
    sensors::BinarySensor *limitSensorBottom;
  };

  class Elevator {
   public:
    Elevator(ElevatorConfig config) : _config(config) {};
    enum ElevatorState { kStationary, kMoving, kZeroing, kManual };

    void SetManual(double setpoint);
    void SetSetpoint(double setpoint);
    void SetZeroing();
    void SetHold();

    double GetSetpoint();

    void Update(double dt);
    
    virtual void OnStateChange(ElevatorState newState, ElevatorState oldState) {};
    virtual void OnStatePeriodic(ElevatorState state, double dt);

    ElevatorConfig &GetConfig();

    private:
    void SetState(ElevatorState state);
    
    ElevatorConfig _config;
    ElevatorState _state, _lastState;
    double _setpoint;
  };
} // ns curtinfrc
