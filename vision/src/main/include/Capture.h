#pragma once

#include "Runnable.h"

class Capture : public Runnable {
 public:
  Capture();
  
  void Init() override;
  void Periodic() override;
};