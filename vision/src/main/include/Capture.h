#pragma once

#include "Runnable.h"

#include <opencv2/core/core.hpp>
#include <cscore.h>
#include <mutex>

class Capture : public Runnable {
 public:
  Capture(int port);
  int GetPort();
  void Init() override;
  void Periodic() override;

  cs::VideoMode GetVideoMode();
  void CopyCaptureMat(cv::Mat &captureMat);
  bool IsValidFrame();

 private:
  std::mutex classMutex;
  cs::CvSink _sink{"USBSink"};
  cv::Mat _captureMat;
  cs::VideoMode _videoMode;
  bool _isValid = false;
  int camPort;
};