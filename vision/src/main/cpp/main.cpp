#include "Capture.h"
// #include "TapeProcessing.h"
// #include "BallProcessing.h"
// #include "HatchProcessing.h"
#include "Display.h"
#include <iostream>
#include <networktables/NetworkTableInstance.h>

#ifndef RUNNING_FRC_TESTS
int main(int argc, char **argv) {
  int team = 0;
  if (argc > 1) {
    team = std::stoi(argv[1]);
  }


#ifdef __DESKTOP__
  std::cout << "Running on Desktop - imshow enabled" << std::endl;
  bool isDesktop = true;
#else
  std::cout << "Running embedded - imshow disabled" << std::endl;
  bool isDesktop = false;
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

  Capture sideHatchCapture{"HatchSide", isDesktop ? 0 : 4};
  Display display{"Side Hatch", sideHatchCapture};

  sideHatchCapture.StartThread(30.0);
  display.StartThread(30.0);

  display.JoinThread();

  // VisionRunner vision;
  // #ifdef __DESKTOP__
  // Capture capture{0, -100};
  // // Capture captureGamePiece{1, 50};
  // #else
  // Capture capture{4, -100};
  // Capture captureGamePiece{5, 60};
  // #endif
  // // HatchProcessing hatchProcess{captureGamePiece};
  // // BallProcessing ballProcess{capture};
  // TapeProcessing tapeProcess{capture};
  
  // // Display displayBall{ballProcess};
  // // Display displayHatch{"Hatch Tracking", hatchProcess};
  // Display displayTape{"Tape Tracking", tapeProcess};
  
  // vision.Run(capture);
  // // vision.Run(captureGamePiece);
  // // vision.Run(ballProcess);
  // // vision.Run(hatchProcess);
  // vision.Run(tapeProcess);

  // // vision.Run(displayBall); 
  // vision.Run(displayHatch);
  // vision.Run(displayTape);


  // for (int i = 0; i < vision.workers.size(); i++) {
  //   vision.workers[i].join();
  // }
  
  std::cout << "Vision Program Exited. Broken??" << std::endl;
  return -1;
}
#endif