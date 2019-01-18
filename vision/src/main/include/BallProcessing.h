#pragma once

#include "Process.h"

class BallProcessing : public Process {
 public:
  BallProcessing(Capture &capture) : Process(capture) {}

  void Init() override;
  void Periodic() override;
};