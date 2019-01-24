#include "VisionRunner.h"
#include "Capture.h"
#include "TapeProcessing.h"
#include "BallProcessing.h"
#include "Display.h"
#include <iostream>
#include <list>
#include <networktables/NetworkTableInstance.h>

#ifndef RUNNING_FRC_TESTS
int main(int argc, char **argv) {
  int team = 0;
  if (argc > 1) {
    team = std::stoi(argv[1]);
  }

#ifdef __DESKTOP__
  std::cout << "Running on Desktop - imshow enabled" << std::endl;
#else
  std::cout << "Running embedded  -imshow disabled" << std::endl;
#endif

  auto ntinst = nt::NetworkTableInstance::GetDefault();
  if (team != 0) {
    std::cout << "Starting CurtinFRC Vision Program (Client Mode - Team " << team << ")" << std::endl;
    ntinst.StartClientTeam(team);
  } else {
    std::cout << "Starting CurtinFRC Vision Program (Server Mode - For Testing Only)" << std::endl;
    ntinst.SetServer("CurtinFRCVision");
    ntinst.StartServer();
  }

  VisionRunner vision;
  Capture capture{0};
  TapeProcessing tapeProcess{capture};
  BallProcessing ballProcess{capture};
  Display display{tapeProcess};
  
  vision.Run(capture);
  vision.Run(tapeProcess);
  vision.Run(ballProcess);
  vision.Run(display);

  for (int i = 0; i < vision.workers.size(); i++) {
    vision.workers[i].join();
  }
  
  std::cout << "Vision Program Exited. Broken??" << std::endl;
  return -1;
}
#endif