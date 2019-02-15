#pragma once

#include "threading/Runnable.h"
#include "Display.h"

#include <cscore.h>
#include <condition_variable>
#include <mutex>
#include <opencv2/core/core.hpp>

class Capture : public Runnable, public Displayable {
 public:
  Capture(std::string name, int port);
  int GetPort();

  void SetExposure(int exposure);

  void Init() override;
  void Periodic() override;

  cs::VideoMode GetVideoMode();

  void CopyCaptureMat(cv::Mat &captureMat);
  
  void GetDisplayMat(cv::Mat &displayMat) override;
  cv::Size GetDisplaySize() override;

  bool IsValidFrame();

 private:
  std::string   _name;
  cs::UsbCamera _cam;
  cs::CvSink    _sink;

  std::mutex              _classMutex;
  std::condition_variable _initCondVar;

  cv::Mat       _captureMat;
  cs::VideoMode _videoMode;
  bool          _validFrame = false;
};