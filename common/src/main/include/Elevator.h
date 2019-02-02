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

    void SetManual(double power);
    void SetSetpoint(double setpoint);
    void SetZeroing();
    void SetHold();

    double GetSetpoint();
    double GetHeight();

    ElevatorConfig &GetConfig();
    
   protected:
    virtual void OnStatePeriodic(ElevatorState state, double dt) override;
    double power = 0;
    double goal;
    double height;
    double kP;
    double error;
    double voltage;

   private:
    ElevatorConfig _config;
    double _setpoint;

    Usage<ElevatorConfig>::Scoped _usage{&_config};
  };
} // ns curtinfrc
