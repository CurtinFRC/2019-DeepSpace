#pragma once

#include "Runnable.h"

class Display : public Runnable {
 public:
  void Init() override;
  void Periodic() override;
  Display(Process &process);
 private:
  Process &_process;
  Capture &_capture;
};