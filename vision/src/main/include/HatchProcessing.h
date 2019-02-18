#pragma once

#include "Process.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

class HatchProcessing {
 public:
  HatchProcessing(Capture &capture);
  
  nt::NetworkTableEntry HatchDistanceEntry;
  nt::NetworkTableEntry HatchXOffsetEntry;
  nt::NetworkTableEntry HatchYOffsetEntry;

  void Init();
  void Periodic();
  void GetDisplayMat(cv::Mat &displayMat);
  cv::Size GetDisplaySize();
  void CopyProcessedTrack(cv::Mat &imgProcessedTrack);


 private:
  cv::Mat imgHSV;
  cv::Mat _captureMat;
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point>> filteredContours;
  Capture &_capture;
  cv::Mat _imgProcessing;
  cv::Mat _imgProcessedTrack;
  cs::VideoMode _videoMode;

 protected:
  std::mutex _classMutex;
};
