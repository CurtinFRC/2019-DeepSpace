#pragma once

#include "Runnable.h"

class Capture : public Runnable {
 public:
  void Init() override;
  void Periodic() override;
  Capture();
};