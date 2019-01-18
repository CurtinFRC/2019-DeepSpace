#pragma once
#include <thread>
#include <vector>
#include "Runnable.h"
class VisionRunner {
 public:
  void Run(Runnable &run);

 protected:
  std::vector<std::thread> threads;
};

