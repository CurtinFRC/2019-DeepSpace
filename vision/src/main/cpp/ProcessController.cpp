#include "ProcessController.h"
#include "networktables/NetworkTableInstance.h"
#include "Capture.h"
#include "TapeProcessing.h"
#include "HatchProcessing.h"
#include "Display.h"
#include <iostream>

#include "threading/Runnable.h"

void Processing::Init() {
  _useTape = true;
  _lastUseTape = false;

  auto inst = nt::NetworkTableInstance::GetDefault();
  auto visionTable = inst.GetTable("VisionTracking");
  _usingTapeEntry = visionTable->GetEntry("Camera Set");
}

void Processing::Periodic() {
  _useTape = _usingTapeEntry.GetBoolean(true);

  if (_useTape) {
    // Using Tape Tracking
    if (_useTape != _lastUseTape)
      _tape.Init();
    _tape.Periodic();
    // Switch to Hatch Port, And Increase Tape Exposure
  } else {
    // Using Hatch Tracking
    if (_useTape != _lastUseTape)
      _hatch.Init();
    _hatch.Periodic();
    // Switch Tape Port, And Lower Tape Exposure
  }

  _lastUseTape = _useTape;
}

void Processing::GetDisplayMat(cv::Mat &displayMat) {
  if (_useTape) {
    _tape.GetDisplayMat(displayMat);
  } else {
    _hatch.GetDisplayMat(displayMat);
  }
}

cv::Size Processing::GetDisplaySize() {
  if (_useTape) {
    return _tape.GetDisplaySize();
  } else {
    return _hatch.GetDisplaySize();
  }
}