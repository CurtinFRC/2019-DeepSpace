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

  //Capture sideCapture{"HatchSide", isDesktop ? 1 : 5};
  Capture frontCapture{"FrontSide", isDesktop ? 0 : 4};
  
  // TapeProcessing tapeSide{sideCapture};
  // HatchProcessing hatchSide{sideCapture};
  TapeProcessing tapeFront{frontCapture};
  HatchProcessing hatchFront{frontCapture};

  auto inst = nt::NetworkTableInstance::GetDefault();
  auto visionTable = inst.GetTable("VisionTracking");

  // Processing sideProcess{sideCapture, tapeSide, hatchSide, visionTable->GetEntry("Camera Set Side")};
  Processing frontProcess{frontCapture, tapeFront, hatchFront, visionTable->GetEntry("Camera Set Front")};

  // Display sideDisplay{"Side Display", sideProcess};
  Display frontDisplay{"Front Display", frontProcess};

  // sideCapture.StartThread(30.0);
  // sideProcess.StartThread(30.0);
  // sideDisplay.StartThread(30.0);

  frontCapture.StartThread(30.0);
  frontProcess.StartThread(30.0);
  frontDisplay.StartThread(30.0);

  // sideCapture.JoinThread();
  // sideProcess.JoinThread();
  // sideDisplay.JoinThread();

  frontCapture.JoinThread();
  frontProcess.JoinThread();
  frontDisplay.JoinThread();

  std::cout << "Vision Program Exited. Broken??" << std::endl;
  return -1;
}
#endif