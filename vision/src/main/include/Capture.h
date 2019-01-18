#pragma once

#include <opencv2/core/core.hpp>
#include <cscore.h>

#include "Runnable.h"

class Capture : public Runnable {
 public:
  void Init() override;
  void Periodic() override;

  cv::Mat &GetCaptureMat();
  bool IsValidFrame();

 private:
  cs::UsbCamera _cam{"USBCam", 0};
  cs::CvSink _sink{"USBSink"};
  cv::Mat _captureMat;
  bool _isValid = false;
};