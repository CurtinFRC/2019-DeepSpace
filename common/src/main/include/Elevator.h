#pragma once

#include "devices/StateDevice.h"
#include "Gearbox.h"
#include "sensors/BinarySensor.h"

#include "Usage.h"

namespace curtinfrc {
  struct ElevatorConfig {
    Gearbox &spool;
    
    sensors::BinarySensor *limitSensorTop;
    sensors::BinarySensor *limitSensorBottom;

    /**
     * Height of the elevator, in metres.
     */
    double height;

    /**
     * Effective spool radius, in metres.
     */
    double spoolRadius;

    /**
     * Mass of the Elevator and Load in kg.
     */
    double mass;
  };

  enum ElevatorState { kStationary, kMoving, kZeroing, kManual };
  class Elevator : public devices::StateDevice<ElevatorState> {
   public:
    Elevator(ElevatorConfig config) : _config(config) {};

    void SetManual(double setpoint);
    void SetSetpoint(double setpoint);
    void SetZeroing();
    void SetHold();

    double GetSetpoint();

    ElevatorConfig &GetConfig();
    
   protected:
    virtual void OnStateChange(ElevatorState newState, ElevatorState oldState) override {};
    virtual void OnStatePeriodic(ElevatorState state, double dt) override;

   private:
    ElevatorConfig _config;
    double _setpoint;

    Usage<ElevatorConfig>::Scoped _usage{&_config};
  };
} // ns curtinfrc
