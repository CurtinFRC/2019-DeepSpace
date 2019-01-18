#pragma once

#include "Runnable.h"

class Process : public Runnable {
 public:
  virtual void Init();
  virtual void Periodic();
  Process(Capture &capture);
  Capture &GetCapture();
 private:
  Capture &_capture;
};