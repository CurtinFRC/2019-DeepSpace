#pragma once

#include "Process.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

class BallProcessing : public Process {
 public:
  BallProcessing(Capture &capture) : Process(capture) {}
  nt::NetworkTableEntry BallDistanceEntry;
  nt::NetworkTableEntry BallXoffsetEntry;
  nt::NetworkTableEntry BallYoffsetEntry;

  void Init() override;
  void Periodic() override;
};