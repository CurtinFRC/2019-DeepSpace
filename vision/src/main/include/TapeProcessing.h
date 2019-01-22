#pragma once

#include "Process.h"

class TapeProcessing : public Process {
 public:
  TapeProcessing(Capture &capture) : Process(capture) {}

  void Init() override;
  void Periodic() override;
 private:
  cv::Mat imgHSV;
  cv::Mat imgBinary;
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point>> filteredContours;
};