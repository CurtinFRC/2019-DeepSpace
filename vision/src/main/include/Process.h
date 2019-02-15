#pragma once

#include "Capture.h"
#include "Display.h"
#include "threading/Runnable.h"

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <mutex>

class Process : public Runnable, public Displayable {
 public:
  virtual void Init() override;
  virtual void Periodic() override;

  Process(Capture &capture);
  Capture &GetCapture();

  void CopyProcessedTrack(cv::Mat &imgProcessedTrack);
  void CopyProcessedThresh(cv::Mat &imgProcessedThresh);
  
  void GetDisplayMat(cv::Mat &displayMat) override;
  cv::Size GetDisplaySize() override;

  std::string GetProcessType();
  bool GetValidThresh();
  bool GetValidTrack();

 protected:
  std::mutex _classMutex;
  std::string processType;
  Capture &_capture;
  cs::VideoMode _videoMode;

  cv::Mat _imgProcessedTrack;
  cv::Mat _imgProcessedThresh;
  cv::Mat _imgProcessing;
};
