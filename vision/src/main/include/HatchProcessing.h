#pragma once

#include "Process.h"

class HatchProcessing : public Process {
 public:
  HatchProcessing(Capture &capture) : Process(capture) {}

  void Init() override;
  void Periodic() override;
};