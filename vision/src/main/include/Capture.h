#pragma once

#include "Runnable.h"

#include <opencv2/core/core.hpp>
#include <cscore.h>
#include <mutex>
#include <condition_variable>

class Capture : public Runnable {
 public:
  Capture(int port);
  int GetPort();
  void Init() override;
  void Periodic() override;

  cs::VideoMode GetVideoMode();
  void CopyCaptureMat(cv::Mat &captureMat);
  bool IsValidFrameThresh();
  bool IsValidFrameTrack();
  
 private:
  cs::UsbCamera _cam;
  std::mutex _classMutex;
  std::condition_variable _initCondVar;
  cs::CvSink _sink{"USBSink"};
  cv::Mat _captureMat;
  cs::VideoMode _videoMode;
  bool _isValidThresh = false;
  bool _isValidTrack = false;
};