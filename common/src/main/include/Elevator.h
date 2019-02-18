#pragma once

#include "devices/StateDevice.h"
#include "Gearbox.h"
#include "sensors/BinarySensor.h"
#include "control/PIDController.h"

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

  enum ElevatorState { kStationary = 0, kMoving, kZeroing, kManual };
  class Elevator : public devices::StateDevice<ElevatorState> {
   public:
    Elevator(ElevatorConfig config, control::PIDGains gain) : _config(config), _gain(gain), _controller(gain) {};

    void SetManual(double power);
    void SetSetpoint(double setpoint);
    void SetZeroing();
    void SetHold();

    double GetSetpoint(); // Do we need that?
    double GetHeight();

    ElevatorConfig &GetConfig();
    
   protected:
    virtual void OnStatePeriodic(ElevatorState state, double dt) override;

   private:
    ElevatorConfig _config;

    control::PIDGains _gain;
    control::PIDController _controller;

    Usage<ElevatorConfig>::Scoped _usage{&_config};
  };
} // ns curtinfrc
