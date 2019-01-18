#pragma once
#include <thread>
#include <vector>
#include "Runnable.h"
class VisionRunner {
 public:
  void Run(Runnable &run);
  std::vector<std::thread> workers;
};

