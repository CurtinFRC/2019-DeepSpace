#pragma once

#include <string>

#include "devices/StateDevice.h"
#include "Gearbox.h"
#include "sensors/BinarySensor.h"
#include "control/PIDController.h"
#include "strategy/StrategySystem.h"
#include "control/MotorFilters.h"

#include "Usage.h"

namespace wml {
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

    std::string name = "<Elevator>";
  };

  enum class ElevatorState { kStationary = 0, kMoving, kZeroing, kManual };

  class Elevator : public devices::StateDevice<ElevatorState>, public StrategySystem {
   public:
    Elevator(ElevatorConfig config, control::PIDGains gain) : StateDevice(config.name), _config(config), _gain(gain), _controller(gain), _current_filter(-20.0, 120.0, config.spool) {};

    virtual std::string GetStateString() final;

    void SetManual(double power);
    void SetSetpoint(double setpoint);
    void SetZeroing();
    void SetHold();

    double GetFeedforward();

    double GetSetpoint(); // Do we need that?
    double GetHeight();

    ElevatorConfig &GetConfig();
    
   protected:
    virtual void OnStatePeriodic(ElevatorState state, double dt) override;

   private:
    ElevatorConfig _config;

    control::PIDGains _gain;
    control::PIDController _controller;
    control::CurrentFFFilter _current_filter;

    Usage<ElevatorConfig>::Scoped _usage{&_config};
  };
} // ns wml
