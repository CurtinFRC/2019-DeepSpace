#include "vision.h"

#include <iostream>

#ifndef RUNNING_FRC_TESTS
int main() {
  std::cout << "Starting CurtinFRC Vision Program" << std::endl;
  curtin_frc_vision vision;
  vision.run();
  std::cout << "Vision Program Exited. Was that meant to happen?" << std::endl;
  return -1;
}
#endif
