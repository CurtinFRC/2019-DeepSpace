#pragma once

#include "Elevator.h"
#include "HarvesterIntake.h"

#include "strategy/Strategy.h"

class Lift : public curtinfrc::Elevator {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig, curtinfrc::control::PIDGains gain) : curtinfrc::Elevator(elevatorConfig, gain) {};

 private:
  // Collision stuffs?
};

class LiftManualStrategy : public curtinfrc::Strategy {
 public: 
  LiftManualStrategy(Lift &lift, curtinfrc::Joystick &joy) : Strategy("Lift Manual"), _lift(lift), _joy(joy) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  };

  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
  curtinfrc::Joystick &_joy;
};

class LiftPresetStrategy : public curtinfrc::Strategy {
 public: 
  LiftPresetStrategy(Lift &lift, curtinfrc::Joystick &joy) : Strategy("Lift Presets"), _lift(lift), _joy(joy) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  };

  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
  curtinfrc::Joystick &_joy;
};
