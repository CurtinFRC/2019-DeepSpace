#include "simulation/simulation.h"
#include "startup.h"

void curtinfrc::robot_startup::start() {
  static simulation::harness sim_harness;
  sim_harness.run();
}