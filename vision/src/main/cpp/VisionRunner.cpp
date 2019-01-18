#include "VisionRunner.h"
#include <chrono>

// Handles threading
void VisionRunner::Run(Runnable &run) {
    workers.push_back(std::thread([&]() {
      run.Init();
      while (true) {
        run.Periodic();
        std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / 120.0));
      }
    }));
}