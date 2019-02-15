#include "Process.h"

#include <stdio.h>
#include <iostream>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

#include "devices/kinect.h"

using namespace cv;
using namespace std;
//Set _capture as a Capture object reference
Process::Process(Capture &capture) : _capture(capture) {}

Capture &Process::GetCapture() {
  return _capture;
}

// Copiers
void Process::CopyProcessedTrack(cv::Mat &imgProcessedTrack) {
  std::lock_guard<std::mutex> lock(_classMutex);
  try {
    _imgProcessedTrack.copyTo(imgProcessedTrack);
  } catch (...) {}
}

void Process::CopyProcessedThresh(cv::Mat &imgProcessedThresh) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgProcessedThresh.copyTo(imgProcessedThresh);
}

// Getters
bool Process::GetValidThresh() {
  return _imgProcessedThresh.rows > 0;
}

bool Process::GetValidTrack() {
  return _imgProcessedTrack.rows > 0;
}

std::string Process::GetProcessType() {
  return processType;
}

void Process::GetDisplayMat(cv::Mat &displayMat) {
  CopyProcessedTrack(displayMat);
}

cv::Size Process::GetDisplaySize() {
  return _capture.GetDisplaySize();
}

void Process::Init() {
  _videoMode = _capture.GetVideoMode();
  _imgProcessedTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgProcessedThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
}

void Process::Periodic() {}
