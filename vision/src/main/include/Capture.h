#pragma once

#include <opencv2/core/core.hpp>
#include <cscore.h>

#include "Runnable.h"

class Capture : public Runnable {
 public:
  Capture(int port);
  int GetPort();
  void Init() override;
  void Periodic() override;
  int &GetHeight();
  int &GetWidth();

  cv::Mat &GetCaptureMat();
  bool IsValidFrame();

 private:
  cs::CvSink _sink{"USBSink"};
  cv::Mat _captureMat;
  bool _isValid = false;
  int camPort;
  int videoWidth;
  int videoHeight;
};