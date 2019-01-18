#pragma once

#include "Elevator.h"

class Lift : public curtinfrc::Elevator {
 public:
  using Elevator::Elevator;

  void Set(double power);
};
