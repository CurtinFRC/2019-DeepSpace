#pragma once

#include "Process.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

class HatchProcessing : public Process {
 public:
  HatchProcessing(Capture &capture) : Process(capture) {}
  nt::NetworkTableEntry HatchDistanceEntry;
  nt::NetworkTableEntry HatchXoffsetEntry;
  nt::NetworkTableEntry HatchYoffsetEntry;

  void Init() override;
  void Periodic() override;
};