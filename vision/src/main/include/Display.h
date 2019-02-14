#pragma once

#include "Process.h"
#include "Capture.h"
#include "Runnable.h"

#include <mutex>

class Display : public Runnable {
 public:
  Display(std::string name, Process &process);

  void Init() override;
  void Periodic() override;

 private:
  Process &_process;
  std::string _name;
  cs::CvSource _output;
  cs::VideoMode _videoMode;
  
  cv::Mat _displayMat;

  Capture &_capture;
};