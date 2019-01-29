#pragma once

#include "Process.h"
#include "Capture.h"
#include "Runnable.h"

#include <mutex>

class Display : public Runnable {
 public:
  Display(Process &process);

  void Init() override;
  void Periodic() override;

 private:
  Process &_process;
  cs::CvSource _output;
  cs::VideoMode _videoMode;
  
  cv::Mat _imgOriginal;
  cv::Mat _imgProcessed;

  cv::Mat _imgBallThresh;
  cv::Mat _imgBallTrack;

  cv::Mat _imgHatchThresh;
  cv::Mat _imgHatchTrack;

  cv::Mat _imgTapeThresh;
  cv::Mat _imgTapeTrack;

  Capture &_capture;
};