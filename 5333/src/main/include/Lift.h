#pragma once

#include "Elevator.h"

class Lift {
 public:
  Lift(curtinfrc::ElevatorConfig elevatorConfig);

  void Set(double power);

 private:
  curtinfrc::Elevator *_elevator;
};
