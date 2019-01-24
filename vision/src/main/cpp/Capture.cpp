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

using namespace cv;
using namespace std;

Capture::Capture(int port) {
  _camPort = port;
}

// Getters
cs::VideoMode Capture::GetVideoMode() {
  if (_videoMode.height == 0 || _videoMode.width == 0) {
    // If our videoMode is invalid, wait for Init() to be called (thus setting _videoMode)
    // using our condition variable
    std::unique_lock<std::mutex> lock(_classMutex);
    _initCondVar.wait(lock);
  }
  return _videoMode;
}

// Copiers
void Capture::CopyCaptureMat(cv::Mat &captureMat) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _captureMat.copyTo(captureMat);
}

bool Capture::IsValidFrame() {
  return _isValid;
}

int Capture::GetPort() {
  return _camPort;
}



void Capture::Init() {
  //std::lock_guard<std::mutex> lock(_classMutex); // do i need this ? *

  cs::UsbCamera _cam{"USBCam", _camPort};
  _sink.SetSource(_cam);
  _cam.SetExposureManual(-100);

  // The camera defaults to a lower resolution, but you can choose any compatible resolution here.
  _cam.SetResolution(640, 480);

  _videoMode = _cam.GetVideoMode();
  std::cout << "Width: " << _videoMode.width << " Height: " << _videoMode.height << std::endl;
  _captureMat = cv::Mat::zeros(_videoMode.height, _videoMode.width, CV_8UC3);
  _initCondVar.notify_all();
}

void Capture::Periodic() {
  _isValid = _sink.GrabFrame(_captureMat) == 0;
}
