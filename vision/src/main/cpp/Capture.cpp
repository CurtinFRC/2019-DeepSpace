#include "Capture.h"
#include "Process.h"

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

#include "devices/kinect.h"

#include <mutex>
#include <condition_variable>

#include <stdlib.h>

using namespace cv;
using namespace std;

Capture::Capture(std::string name, int port) : _name(name), _cam(name, port), _sink(name) {
  _sink.SetSource(_cam);
}

void Capture::SetExposure(int exposure) {
  _cam.SetExposureManual(exposure);
}

void Capture::Init() {
  _cam.SetFPS(30);
  _cam.SetResolution(640, 360);

  for (auto it : _cam.EnumerateProperties()) {
    std::cout << "[CAM " << _name << "] Property: " << it.GetName() << " -> " << it.Get() << std::endl;
  }

  _videoMode = _cam.GetVideoMode(); 
  std::cout << "[CAM " << _name << "] Width: " << _videoMode.width << " Height: " << _videoMode.height << std::endl;
  _captureMat = cv::Mat::zeros(_videoMode.height, _videoMode.width, CV_8UC3);
  _initCondVar.notify_all();
}

void Capture::Periodic() {
  _validFrame = _sink.GrabFrame(_captureMat) != 0;

  static int i = 0;
  i++;
  if (i % 10 == 0)
    std::cout << GetActualFramerate() << std::endl;
}

cs::VideoMode Capture::GetVideoMode() {
  if (_videoMode.height == 0 || _videoMode.width == 0) {
    std::unique_lock<std::mutex> lock(_classMutex);
    _initCondVar.wait(lock);
  }
  return _videoMode;
}

void Capture::CopyCaptureMat(cv::Mat &captureMat) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _captureMat.copyTo(captureMat);
}

void Capture::GetDisplayMat(cv::Mat &displayMat) {
  CopyCaptureMat(displayMat);
}

cv::Size Capture::GetDisplaySize() {
  auto mode = GetVideoMode();
  return cv::Size(mode.width, mode.height);
}

bool Capture::IsValidFrame() {
  return _validFrame;
}