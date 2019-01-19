#include "simulation/simulation.h"
#include "simulation/physics_updater.h"
#include "simulation/windows/control.h"
#include "simulation/windows/motors.h"
#include "simulation/windows/elevators.h"

#include "hal/HAL.h"
#include "mockdata/DriverStationData.h"
#include "mockdata/RoboRioData.h"

#include <iostream>
#include <thread>
#include <functional>

using namespace simulation;

// Convenience function that allows you to use a member function in std::for_each.
template<typename func_type>
auto bind(func_type func_ref) {
  return std::bind(func_ref, std::placeholders::_1);
}

void harness::run(std::function<int()> robot_thread) {
  std::cout << "[SIM] Simulation Starting!" << std::endl;
  HAL_Initialize(500, 0);
  HALSIM_SetDriverStationDsAttached(true);
  HALSIM_SetRoboRioVInVoltage(0, 12.9);

  (new control_window())->start();
  (new motor_window())->start();
  elevator_window::init();

  std::cout << "[SIM] Starting Robot Thread" << std::endl;
  std::thread r_thread([&]() {
    robot_thread();
  });
  r_thread.detach();

  std::cout << "[SIM] Starting Physics Thread" << std::endl;
  std::thread p_thread([&]() {
    physics_thread::INSTANCE()->threadfunc();
  });
  p_thread.detach();

  std::cout << "[SIM] Simulation Initialization Complete" << std::endl;

  while (true) {
    ui::window_manager::INSTANCE()->update();
    cv::waitKey(static_cast<int>(1000.0 / 45.0));
  }
}