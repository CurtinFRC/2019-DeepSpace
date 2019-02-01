#pragma once

#include "Process.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

class TapeProcessing : public Process {
 public:
  nt::NetworkTableEntry TapeDistanceEntry;
  nt::NetworkTableEntry TapeAngleEntry;
  nt::NetworkTableEntry TapeTargetEntry;
  TapeProcessing(Capture &capture) : Process(capture) {}

  void Init() override;
  void Periodic() override;

 private:
  cv::Mat imgHSV;
  cv::Mat _captureMat;
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point>> filteredContours;
  // Target vectors
  std::vector<cv::Point2f> centres;
  std::vector<cv::Point2f> targets;
  std::vector<bool> lefts;
  std::vector<bool> rights;
  std::vector<float> angles;
  std::vector<float> heights;
  std::vector<float> distances;
};