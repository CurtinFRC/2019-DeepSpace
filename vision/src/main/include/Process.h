#pragma once

#include "Runnable.h"

class Process : public Runnable {
 public:
  void Init() override;
  void Periodic() override;
};