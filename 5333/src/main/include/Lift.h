#pragma once

#include "Elevator.h"

class Lift : protected curtinfrc::Elevator {
 public:
  using Elevator::Elevator;

  void Set(double power);

  using Elevator::Update;
};
