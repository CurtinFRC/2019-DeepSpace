#include "vision.h"

#include <iostream>

#ifndef RUNNING_FRC_TESTS
int main() {

#ifdef __DESKTOP__
  std::cout << "Running on Desktop - imshow enabled" << std::endl;
#else
  std::cout << "Running embedded - imshow disabled" << std::endl; // Imshow is only a windows functions. And tinkerboards/PI's are linux 
#endif

  std::cout << "Starting Vision Program" << std::endl;
  std::cout << "This Program was made for the 2019 Game DeepSpace, And tracks and outputs green values" << std::endl;
  curtin_frc_vision vision;
  vision.run();
  std::cout << "Vision Program Exited. Was that meant to happen?" << std::endl;
  return -1;
}
#endif
