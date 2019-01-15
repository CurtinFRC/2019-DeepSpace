#pragma once
#include "Process.h"
class TapeProcessing : public Process {
 public:
  void Init() override;
  void Periodic() override;
};