#pragma once

#include "Capture.h"
#include "Runnable.h"

class Process : public Runnable {
 public:
  Process(Capture &capture);
  Capture &GetCapture();
 private:
  Capture &_capture;
};