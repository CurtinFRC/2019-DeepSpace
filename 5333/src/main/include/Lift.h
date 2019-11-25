#pragma once

#include "Elevator.h"

#include "controllers/CurtinControllers.h"
#include "strategy/Strategy.h"

class Lift : public wml::Elevator {
 public:
  Lift(wml::ElevatorConfig elevatorConfig, wml::control::PIDGains gain) : wml::Elevator(elevatorConfig, gain) {};

 private:
  // Collision stuffs?
};

class LiftManualStrategy : public wml::Strategy {
 public:
  LiftManualStrategy(Lift &lift, wml::controllers::SmartControllerGroup &contGroup) : Strategy("Lift Manual"), _lift(lift), _contGroup(contGroup) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  };

  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
  wml::controllers::SmartControllerGroup &_contGroup;
};

class LiftGotoStrategy : public wml::Strategy {
 public: 
  LiftGotoStrategy(Lift &lift, double setpoint) : Strategy("Lift Goto"), _lift(lift), _setpoint(setpoint) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(false);
  };

  void OnStart() override;
  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
  double _setpoint;
};

class LiftZeroStrategy : public wml::Strategy {
 public: 
  LiftZeroStrategy(Lift &lift) : Strategy("Lift Zero"), _lift(lift) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(false);
  };

  void OnStart() override;
  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
};

