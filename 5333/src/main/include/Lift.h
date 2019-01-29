#pragma once

#include "Elevator.h"
#include "HarvesterIntake.h"

class Lift : public curtinfrc::Elevator {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig, HarvesterIntake &harvesterIntake) : curtinfrc::Elevator(elevatorConfig), _harvesterIntake(harvesterIntake) {};

  void Set(double power);

 private:
  HarvesterIntake &_harvesterIntake;
};
