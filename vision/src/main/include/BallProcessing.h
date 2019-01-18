#pragma once

#include "Process.h"

class BallProcessing : public Process {
 public:
  void Init() override;
  void Periodic() override;
};