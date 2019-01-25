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
  cv::Mat _imgTrack;
  cv::Mat _imgOriginal;
  Capture &_capture;
};