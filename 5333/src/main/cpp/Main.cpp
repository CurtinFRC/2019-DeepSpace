#include "Robot.h"

#ifdef SIMULATION
#include <simulation.h>
#endif

#include <iostream>

#ifndef RUNNING_FRC_TESTS
int main() { 
#ifdef SIMULATION
  simulation::harness harness;
  harness.run();
#endif
  std::cout << "[MAIN] Starting Robot (frc::StartRobot)..." << std::endl;
  return frc::StartRobot<Robot>();
}
#endif
