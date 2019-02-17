#pragma once

#include "Process.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

class HatchProcessing {
 public:
  HatchProcessing(Capture &capture);
  
  nt::NetworkTableEntry HatchDistanceEntry;
  nt::NetworkTableEntry HatchXoffsetEntry;
  nt::NetworkTableEntry HatchYoffsetEntry;

  void Init();
  void Periodic();
  void GetDisplayMat(cv::Mat &displayMat);
  cv::Size GetDisplaySize();

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
  Capture &_capture;
  cv::Mat _imgProcessing;
  cv::Mat _imgProcessedTrack;
  cs::VideoMode _videoMode;
};