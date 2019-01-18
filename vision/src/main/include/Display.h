#pragma once

#include "Runnable.h"

class Display : public Runnable {
 public:
  Display(Process &process);
  
  void Init() override;
  void Periodic() override;

 private:
  Process &_process;
  Capture &_capture;
};