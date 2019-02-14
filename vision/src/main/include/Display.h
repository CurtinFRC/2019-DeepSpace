#pragma once

#include <opencv2/core/core.hpp>
#include <cscore.h>

#include "threading/Runnable.h"

#include <mutex>

class Displayable {
 public:
  virtual void GetDisplayMat(cv::Mat &displayMat) = 0;
  virtual cv::Size GetDisplaySize() = 0;
};

class Display : public Runnable {
 public:
  Display(std::string name, Displayable &target);

  void Init() override;
  void Periodic() override;

 private:
  std::string _name;
  Displayable &_target;
  
  cs::CvSource _output;
  
  cv::Mat _displayMat;
};