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
  cs::CvSource _outputCam0;
  cs::CvSource _outputCam1;
  cs::VideoMode _videoMode;
  
  cv::Mat _imgOriginal;
  cv::Mat _imgProcessedTrack;
  cv::Mat _imgProcessedTrackHatch;
  cv::Mat _imgProcessedThresh;

  Capture &_capture;
};