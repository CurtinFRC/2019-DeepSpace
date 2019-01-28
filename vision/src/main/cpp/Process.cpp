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

void Process::CopyImgOriginal(cv::Mat &imgOriginal) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgOriginal.copyTo(imgOriginal);
}

void Process::CopyImgBallThresh(cv::Mat &imgballThresh) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgBallThresh.copyTo(imgballThresh);
}

// Copiers
void Process::CopyImgHatchTrack(cv::Mat &imghatchTrack) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgHatchTrack.copyTo(imghatchTrack);
}

void Process::Init() {
  _videoMode = _capture.GetVideoMode();
	_imgOriginal = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};

  _imgBallThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgBallTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};

  _imgHatchThresh = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
  _imgHatchTrack = cv::Mat{_videoMode.height, _videoMode.width, CV_8UC3};
}

void Process::Periodic() {}
