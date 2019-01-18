#pragma once

#include "Process.h"
#include "Capture.h"
#include "Runnable.h"

class Display : public Runnable {
 public:
  Display(Process &process);

  void Init() override;
  void Periodic() override;

 private:
  Process &_process;
};