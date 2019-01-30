#pragma once

#include "Elevator.h"
#include "HarvesterIntake.h"

class Lift : public curtinfrc::Elevator {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig) : curtinfrc::Elevator(elevatorConfig) {};

  void Set(double power);

 private:
  // Collision stuffs?
};
