#include "simulation/simulation.h"
#include "startup.h"

void curtinfrc::robot_startup::start(std::function<int()> robot_func) {
  static simulation::harness sim_harness;
  sim_harness.run(robot_func);
}