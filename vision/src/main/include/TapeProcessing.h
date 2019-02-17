#pragma once

#include "Process.h"
#include "ProcessController.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"
#include <mutex>

class TapeProcessing : public Processing {
 public:
  nt::NetworkTableEntry TapeDistanceEntry;
  nt::NetworkTableEntry TapeAngleEntry;
  nt::NetworkTableEntry TapeTargetEntry;
  TapeProcessing(Capture &capture);

  void Init();
  void Periodic();
  void CopyProcessedTrack(cv::Mat &imgProcessedTrack);
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

 protected:
  std::mutex _classMutex;
};