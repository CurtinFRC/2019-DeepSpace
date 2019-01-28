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
void Process::CopyImgTrack(cv::Mat &imgTrack) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgTrack.copyTo(imgTrack);
}

void Process::CopyImgOriginal(cv::Mat &imgOriginal) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgOriginal.copyTo(imgOriginal);
}

void Process::CopyImgThresh(cv::Mat &imgThresh) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgThresh.copyTo(imgThresh);
}

void Process::Init() {
  _videoMode = _capture.GetVideoMode();
  _imgTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
	_imgOriginal = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
}

void Process::Periodic() {}
