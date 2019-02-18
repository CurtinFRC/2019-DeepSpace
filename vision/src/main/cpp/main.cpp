#include "Capture.h"
#include "TapeProcessing.h"
#include "ProcessController.h"
#include "HatchProcessing.h"
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
  
  TapeProcessing tapeBoi{sideHatchCapture};
  HatchProcessing hatchBoi{sideHatchCapture};
  
  Processing sideTape{sideHatchCapture, tapeBoi, hatchBoi};
  
  Display display{"Side Hatch", sideTape};

  sideHatchCapture.StartThread(30.0);
  sideTape.StartThread(30.0);
  display.StartThread(30.0);

  sideHatchCapture.JoinThread();
  sideTape.JoinThread();
  display.JoinThread();

  while (true) {
    sideTape.ProcessPick();
  }
  
  std::cout << "Vision Program Exited. Broken??" << std::endl;
  return -1;
}
#endif