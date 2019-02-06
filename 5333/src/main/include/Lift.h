#pragma once

#include "Elevator.h"
#include "HarvesterIntake.h"

#include "strategy/Strategy.h"

class Lift : public curtinfrc::Elevator {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig, curtinfrc::control::PIDGains gain) : curtinfrc::Elevator(elevatorConfig, gain) {};

  void Set(double power);

 private:
  // Collision stuffs?
};

class LiftManualStrategy : public curtinfrc::Strategy {
 public: 
  LiftManualStrategy(Lift &lift, curtinfrc::Joystick &joy) : Strategy("Harvester Manual"), _lift(lift), _joy(joy) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  };

  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
  curtinfrc::Joystick &_joy;
};
