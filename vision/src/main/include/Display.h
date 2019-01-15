#pragma once

#include "Runnable.h"

class Display : public Runnable {
 public:
  void Init() override;
  void Periodic() override;
};