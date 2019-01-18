#include "simulation/simulation.h"
#include "simulation/windows/control.h"
#include "simulation/windows/motors.h"

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
  _windows.push_back(std::make_unique<control_window>(_windows));
  _windows.push_back(std::make_unique<motor_window>());
}

void harness::run(std::function<int()> robot_thread) {
  std::cout << "[SIM] Simulation Starting!" << std::endl;
  HAL_Initialize(500, 0);
  HALSIM_SetDriverStationDsAttached(true);

  (new control_window())->start();
  (new motor_window())->start();

  std::cout << "[SIM] Starting Robot Thread" << std::endl;
  std::thread thread([&]() {
    robot_thread();
  });
  thread.detach();

  std::cout << "[SIM] Simulation Initialization Complete" << std::endl;

  while (true) {
    ui::window_manager::INSTANCE()->update();
    cv::waitKey(static_cast<int>(1000.0 / 45.0));
  }
}