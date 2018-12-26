#include "simulation.h"
#include "windows/control.h"

#include "hal/HAL.h"
#include "mockdata/DriverStationData.h"

#include <iostream>
#include <thread>

using namespace simulation;

control_window _window{};

void update() {
  _window.update();
}

void harness::run() {
  std::cout << "[SIM] Simulation Starting!" << std::endl;
  HAL_Initialize(500, 0);
  HALSIM_SetDriverStationDsAttached(true);

  _window.start();

  std::cout << "[SIM] Starting UI Thread" << std::endl;
  std::thread thread([]() {
    while (true) {
      update();
      cv::waitKey(static_cast<int>(1000.0 / 45.0));
    }
  });
  thread.detach();

  std::cout << "[SIM] Simulation Initialization Complete" << std::endl;
}