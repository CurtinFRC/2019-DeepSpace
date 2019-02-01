#pragma once

#include "Runnable.h"

#include <thread>
#include <vector>
#include <mutex>

class VisionRunner {
 public:
  void Run(Runnable &run);
  std::vector<std::thread> workers;
};

