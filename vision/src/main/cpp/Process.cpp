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
void Process::CopyImgOriginal(cv::Mat &imgOriginal) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgOriginal.copyTo(imgOriginal);
}

void Process::CopyProcessed(cv::Mat &imgProcessed) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgProcessed.copyTo(imgProcessed);
}

void Process::CopyImgBallThresh(cv::Mat &imgballThresh) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgBallThresh.copyTo(imgballThresh);
}

void Process::CopyImgBallTrack(cv::Mat &imgballTrack) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgBallTrack.copyTo(imgballTrack);
}

void Process::CopyImgHatchThresh(cv::Mat &imghatchThresh) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgHatchThresh.copyTo(imghatchThresh);
}

void Process::CopyImgHatchTrack(cv::Mat &imghatchTrack) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgHatchTrack.copyTo(imghatchTrack);
}

bool Process::GetValid() {
  return _imgProcessed.rows > 3;
}

std::string Process::GetProcessType() {
  return processType;
}

void Process::Init() {
  _videoMode = _capture.GetVideoMode();
	_imgOriginal = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgProcessed = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};

  _imgBallThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgBallTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};

  _imgHatchThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgHatchTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
}

void Process::Periodic() {}
