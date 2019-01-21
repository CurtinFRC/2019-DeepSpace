#include "simulation/simulation.h"
#include "startup.h"

void curtinfrc::RobotStartup::Start(std::function<int()> robotFunc) {
  static simulation::harness sim_harness;
  sim_harness.run(robotFunc);
}
