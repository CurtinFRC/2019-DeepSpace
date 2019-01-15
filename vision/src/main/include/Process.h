#pragma once

#include "Runnable.h"

class Process : public Runnable {
 public:
  void Init();
  void Periodic();
};