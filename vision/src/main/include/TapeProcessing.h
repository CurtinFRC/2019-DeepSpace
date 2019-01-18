#pragma once

#include "Process.h"

class TapeProcessing : public Process {
 public:
  TapeProcessing(Capture &capture) : Process(capture) {}

  void Init() override;
  void Periodic() override;
};