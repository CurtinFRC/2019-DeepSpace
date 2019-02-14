#include "Display.h"
#include "Process.h"
#include "Capture.h"
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>
#include <cameraserver/CameraServer.h>
#include <cscore.h>
#include <chrono>
#include <thread>

#include "devices/kinect.h"

Display::Display(std::string name, Displayable &target) : _name(name), _target(target) {}

void Display::Init() {
  cv::Size size = _target.GetDisplaySize();
  _output = frc::CameraServer::GetInstance()->PutVideo(_name, size.width, size.height);

#ifdef __DESKTOP__
  cv::namedWindow(_name);
  SetUseCVWait(true);
#endif
}

void Display::Periodic() {
  _target.GetDisplayMat(_displayMat);
  if (_displayMat.rows > 0 && _displayMat.cols > 0) {
#ifdef __DESKTOP__
    cv::imshow(_name, _displayMat);
#endif
    _output.PutFrame(_displayMat);
  }
}

