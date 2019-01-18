#pragma once
#include "Process.h"
class TapeProcessing : public Process {
 public:
  void Init();
  void Periodic();
};