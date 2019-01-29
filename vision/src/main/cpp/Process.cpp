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
  _imgProcessedTrack.copyTo(imgProcessedTrack);
}

void Process::CopyProcessedThresh(cv::Mat &imgProcessedThresh) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgProcessedThresh.copyTo(imgProcessedThresh);
}

// Getters
bool Process::GetValid() {
  return _imgProcessedThresh.rows > 0; // check this CJ, do we need a track ver ?
}

std::string Process::GetProcessType() {
  return processType;
}

void Process::Init() {
  _videoMode = _capture.GetVideoMode();
  _imgProcessedTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgProcessedThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
}

void Process::Periodic() {}
