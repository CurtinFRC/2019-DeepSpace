#pragma once

#include "Elevator.h"

#include "controllers/CurtinControllers.h"
#include "strategy/Strategy.h"

class Lift : public curtinfrc::Elevator {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig, curtinfrc::control::PIDGains gain) : curtinfrc::Elevator(elevatorConfig, gain) {};

 private:
  // Collision stuffs?
};

class LiftManualStrategy : public curtinfrc::Strategy {
 public:
  LiftManualStrategy(Lift &lift, curtinfrc::controllers::SmartControllerGroup &contGroup) : Strategy("Lift Manual"), _lift(lift), _contGroup(contGroup) {
    Requires(&lift);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  };

  void OnUpdate(double dt) override;

 private:
  Lift &_lift;
  curtinfrc::controllers::SmartControllerGroup &_contGroup;
};

class LiftGotoStrategy : public curtinfrc::Strategy {
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

class LiftZeroStrategy : public curtinfrc::Strategy {
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

