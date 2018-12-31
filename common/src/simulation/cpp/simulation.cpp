#include "simulation/simulation.h"
#include "simulation/windows/control.h"

#include "hal/HAL.h"
#include "mockdata/DriverStationData.h"

#include <iostream>
#include <thread>
#include <functional>

using namespace simulation;

// Convenience function that allows you to use a member function in std::for_each.
template<typename func_type>
auto bind(func_type func_ref) {
  return std::bind(func_ref, std::placeholders::_1);
}

harness::harness() {
  _windows.push_back(std::make_unique<control_window>());
}

void harness::run() {
  std::cout << "[SIM] Simulation Starting!" << std::endl;
  HAL_Initialize(500, 0);
  HALSIM_SetDriverStationDsAttached(true);

  auto bound = &ui::window::start;

  std::for_each(_windows.begin(), _windows.end(), bind(&ui::window::start));

  std::cout << "[SIM] Starting UI Thread" << std::endl;
  std::thread thread([&]() {
    while (true) {
      std::for_each(_windows.begin(), _windows.end(), bind(&ui::window::update));
      cv::waitKey(static_cast<int>(1000.0 / 45.0));
    }
  });
  thread.detach();

  std::cout << "[SIM] Simulation Initialization Complete" << std::endl;
}