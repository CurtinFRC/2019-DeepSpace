#pragma once

#include "Elevator.h"
#include "HarvesterIntake.h"

class Lift : public curtinfrc::Elevator {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig, HarvesterIntakeConfig harvesterConfig) : curtinfrc::Elevator(elevatorConfig), _harvesterIntake(harvesterConfig) {};

  void Set(double power);

 private:
  HarvesterIntake _harvesterIntake;
};
